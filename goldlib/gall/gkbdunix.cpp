//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
//  ------------------------------------------------------------------
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Library General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Library General Public License for more details.
//
//  You should have received a copy of the GNU Library General Public
//  License along with this program; if not, write to the Free
//  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
//  MA 02111-1307, USA
//  ------------------------------------------------------------------
//  $Id$
//  ------------------------------------------------------------------
//  Unix keyboard functions. Based on SLang source code.
//  ------------------------------------------------------------------

#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <gutlunix.h>
#include <gsigunix.h>
#include <gkbdunix.h>

//  ------------------------------------------------------------------

int gkbd_stdin = -1;


//  ------------------------------------------------------------------

static termios gkbd_oldtty;


//  ------------------------------------------------------------------

static bool gkbd_tty_inited = false;
static bool gkbd_tty_open = false;


//  ------------------------------------------------------------------

int gkbd_tty_init() {

  gsig_block_signals();

  gkbd_tty_open = false;

  if((gkbd_stdin == -1) or (isatty(gkbd_stdin) != 1)) {

    gkbd_stdin = open("/dev/tty", O_RDWR);

    if(gkbd_stdin >= 0)
      gkbd_tty_open = true;

    if(not gkbd_tty_open) {
      gkbd_stdin = fileno(stderr);
      if(isatty(gkbd_stdin) != 1) {
        gkbd_stdin = fileno (stdin);
        if(isatty(gkbd_stdin) != 1)
          return -1;
      }
    }
  }

  while(GET_TERMIOS(gkbd_stdin, &gkbd_oldtty) == -1) {
    if(errno != EINTR) {
      gsig_unblock_signals();
      return -1;
    }
  }

  termios newtty;

  while(GET_TERMIOS(gkbd_stdin, &newtty) == -1) {
    if(errno != EINTR) {
      gsig_unblock_signals();
      return -1;
    }
  }

  newtty.c_oflag &= ~OPOST;

  newtty.c_lflag = ISIG | NOFLSH;

  newtty.c_iflag &= ~(ECHO | INLCR | ICRNL | IXON);

  newtty.c_cc[VMIN] = 1;
  newtty.c_cc[VEOF] = 1;
  newtty.c_cc[VTIME] = 0;
  newtty.c_cc[VINTR] = NULL_VALUE;
  newtty.c_cc[VQUIT] = NULL_VALUE;
  newtty.c_cc[VSUSP] = NULL_VALUE;

  while(SET_TERMIOS(gkbd_stdin, &newtty) == -1) {
    if(errno != EINTR) {
      gsig_unblock_signals();
      return -1;
    }
  }

  gkbd_tty_inited = true;

  gsig_unblock_signals();

  return 0;
}


//  ------------------------------------------------------------------

void gkbd_tty_reset() {

  gsig_block_signals();

  if(not gkbd_tty_inited) {
    gsig_unblock_signals();
    return;
  }

  while((SET_TERMIOS(gkbd_stdin, &gkbd_oldtty) == -1) and (errno == EINTR))
    ;

  if(gkbd_tty_open) {
    while((close(gkbd_stdin) == -1) and (errno == EINTR))
      ;
    gkbd_tty_open = false;
    gkbd_stdin = -1;
  }

  gkbd_tty_inited = false;

  gsig_unblock_signals();
}


//  ------------------------------------------------------------------

int gkbd_sys_input_pending(int tsecs) {

#ifndef __BEOS__
  static fd_set read_fd_set;
  struct timeval wait;
  long usecs, secs;

  if(tsecs >= 0) {
    secs = tsecs / 10;
    usecs = (tsecs % 10) * 100000;
  }
  else {
    tsecs = -tsecs;
    secs = tsecs / 1000;
    usecs = (tsecs % 1000) * 1000;
  }

  wait.tv_sec = secs;
  wait.tv_usec = usecs;

  FD_ZERO(&read_fd_set);
  FD_SET(gkbd_stdin, &read_fd_set);

  return select(gkbd_stdin+1, &read_fd_set, NULL, NULL, &wait);
#else
  struct termios term, oterm;
  int fd = gkbd_stdin;
  int c = 0;

  // get the terminal settings
  tcgetattr(fd, &oterm);

  // get a copy of the settings, which we modify
  memcpy(&term, &oterm, sizeof(term));

  // put the terminal in non-canonical mode, any
  // reads timeout after 0.1 seconds or when a
  // single character is read
  term.c_lflag = term.c_lflag & (!ICANON);
  term.c_cc[VMIN] = 0;
  term.c_cc[VTIME] = 1;
  tcsetattr(fd, TCSANOW, &term);

  int bytes = -1;
  ioctl(fd, TCWAITEVENT, &bytes);

  // reset the terminal to original state
  tcsetattr(fd, TCSANOW, &oterm);

  return bytes;
#endif
}


//  ------------------------------------------------------------------

uint gkbd_sys_getkey() {

  while(1) {
    int ret = gkbd_sys_input_pending(100);
    if(ret == 0)
      continue;
    if(ret != -1)
      break;
    if(errno == EINTR)
      continue;
    break;
  }

  char c;

  while(read(gkbd_stdin, &c, 1) == -1) {
    if(errno == EINTR)
      continue;
    if(errno == EAGAIN) {
      sleep(1);
      continue;
    }
    #ifndef __DJGPP__
    if(errno == EWOULDBLOCK) {
      sleep(1);
      continue;
    }
    #endif
    return (uint)-1;
  }

  return (uint)c;
}


//  ------------------------------------------------------------------

uint gkbd_input_buffer_len = 0;
char gkbd_input_buffer[GKBD_MAX_INPUT_BUFFER_LEN];


//  ------------------------------------------------------------------

uint gkbd_getkey() {

  uint imax;
  uint ch;

  if(gkbd_input_buffer_len) {
    ch = (uint)*gkbd_input_buffer;
    gkbd_input_buffer_len--;
    imax = gkbd_input_buffer_len;
    memcpy(gkbd_input_buffer, gkbd_input_buffer+1, imax);
  }
  else {
    ch = gkbd_sys_getkey();
  }

  return ch;
}


//  ------------------------------------------------------------------

void gkbd_ungetkey_string(char *s, uint n) {

  char* bmax;
  char* b1;
  char* b;

  if(gkbd_input_buffer_len + n + 3 > GKBD_MAX_INPUT_BUFFER_LEN)
    return;

  b = gkbd_input_buffer;
  bmax = (b - 1) + gkbd_input_buffer_len;
  b1 = bmax + n;
  while(bmax >= b)
    *b1-- = *bmax--;
  bmax = b + n;
  while(b < bmax)
    *b++ = *s++;
  gkbd_input_buffer_len += n;
}


//  ------------------------------------------------------------------

int gkbd_input_pending(int tsecs) {

  if(gkbd_input_buffer_len)
    return gkbd_input_buffer_len;

  int n = gkbd_sys_input_pending(tsecs);
  if(n <= 0)
    return 0;

  char c = (char)gkbd_getkey();
  gkbd_ungetkey_string(&c, 1);

  return n;
}


//  ------------------------------------------------------------------

void gkbd_flush_input() {

  gkbd_input_buffer_len = 0;
  while(gkbd_sys_input_pending(0) > 0)
    gkbd_sys_getkey();
}


//  ------------------------------------------------------------------

struct gkbd_key_type {

  char str[7];
  bool active;
  uint keysym;
  gkbd_key_type* next;
};


//  ------------------------------------------------------------------

#define UPPER_CASE_KEY(x) (((x) >= 'a') and ((x) <= 'z') ? (x) - 32 : (x))
#define LOWER_CASE_KEY(x) (((x) >= 'A') and ((x) <= 'Z') ? (x) + 32 : (x))


//  ------------------------------------------------------------------

gkbd_key_type* gkbd_keymap = NULL;


//  ------------------------------------------------------------------

void gkbd_keymap_init() {

  gkbd_keymap = (gkbd_key_type*)calloc(256, sizeof(gkbd_key_type));
}


//  ------------------------------------------------------------------

void gkbd_keymap_reset() {

  if(gkbd_keymap) {
    for(int n=0; n<256; n++) {
      gkbd_key_type* next = gkbd_keymap[n].next;
      while(next) {
        gkbd_key_type* current = next;
        next = current->next;
        free(current);
      }
    }
    free(gkbd_keymap);
    gkbd_keymap = NULL;
  }
}


//  ------------------------------------------------------------------

static gkbd_key_type* malloc_key(char *str) {

  gkbd_key_type* key = (gkbd_key_type*)malloc(sizeof(gkbd_key_type));

  memset(key, 0, sizeof(gkbd_key_type));
  memcpy(key->str, str, *str);

  return key;
} 


//  ------------------------------------------------------------------
//  Convert things like "^A" to 1 etc... The 0th char is the strlen
//  INCLUDING the length character itself.

static char* gkbd_process_keystring(char* s) {

  static char str[32];
  char ch;

  int i = 1;
  while(*s != 0) {
    ch = *s++;
    if(ch == '^') {
      ch = *s++;
      if(ch == 0) {
        if(i < 32)
          str[i++] = '^';
        break;
      }
      ch = UPPER_CASE_KEY(ch);
      if(ch == '?')
        ch = 127;
      else
        ch = ch - 'A' + 1;
    }

    if(i >= 32)
      break;
    str[i++] = ch;
  }
  str[0] = i;

  return str;
}


//  ------------------------------------------------------------------

static int key_string_compare(char* a, char* b, uint len) {

  char* amax = a + len;

  while(a < amax) {

    int cha = *a++;
    int chb = *b++;

    if(cha == chb)
      continue;

    int cha_up = UPPER_CASE_KEY(cha);
    int chb_up = UPPER_CASE_KEY(chb);

    if(cha_up == chb_up) {
      // Use case-sensitive result.
      return cha - chb;
    }
    // Use case-insensitive result.
    return cha_up - chb_up;
  }

  return 0;
}


//  ------------------------------------------------------------------
//  This function also performs an insertion in an ordered way.

static int find_the_key(char* s, gkbd_key_type** keyp) {

  *keyp = NULL;

  char* str = gkbd_process_keystring(s);

  uint str_len = str[0];
  if(str_len == 1)
    return 0;

  char ch = str[1];
  gkbd_key_type* key = gkbd_keymap + ch;

  if(str_len == 2) {
    if(key->next)
      return -2;

    key->str[0] = str_len;
    key->str[1] = ch;

    *keyp = key;
    return 0;
  }

  // insert the key definition
  while(1) {

    gkbd_key_type* last = key;
    key = key->next;

    if(key and key->str) {

      uint key_len = key->str[0];
      uint len = key_len;
      if(len > str_len)
        len = str_len;

      int cmp = key_string_compare(str+1, key->str+1, len-1);

      if(cmp > 0)
        continue;

      if(cmp == 0) {
        if(key_len != str_len)
          return -2;

        *keyp = key;

        return 0;
      }
      // Drop to cmp < 0 case
    }

    gkbd_key_type* neew = malloc_key(str);

    neew->next = key;
    last->next = neew;

    *keyp = neew;

    return 0;
  }
}


//  ------------------------------------------------------------------

int gkbd_define_keysym(char* s, uint keysym) {

  gkbd_key_type* key;

  int ret = find_the_key(s, &key);

  if((ret != 0) or (key == NULL))
    return ret;

  key->active = true;
  key->keysym = keysym;

  return 0;
}


//  ------------------------------------------------------------------

uint gkbd_last_key_char = 0;

static gkbd_key_type* gkbd_do_key() {

  gkbd_last_key_char = gkbd_getkey();

  if(gkbd_last_key_char == (uint)-1)
    return NULL;

  char input_ch = (char)gkbd_last_key_char;

  gkbd_key_type* key = gkbd_keymap + input_ch;

  // if the next one is null, then we know this MAY be it.
  while(key->next == NULL) {

    if(key->active)
      return key;

    // Try its opposite case counterpart
    char chlow = LOWER_CASE_KEY(input_ch);
    if(input_ch == chlow)
      input_ch = UPPER_CASE_KEY(input_ch);

    key = gkbd_keymap + input_ch;
    if(not key->active)
      return NULL;
  }

  if(gkbd_input_pending(1) == 0)
    if(key->active)
      return key;

  // It appears to be a prefix character in a key sequence.

  uint len = 1;                 // already read one character
  key = key->next;              // Now we are in the key list
  gkbd_key_type* kmax = NULL;   // set to end of list
  gkbd_key_type* next;

  while(1) {

    gkbd_last_key_char = gkbd_getkey();

    len++;

    if(gkbd_last_key_char == (uint)-1)
      break;

    input_ch = (char)gkbd_last_key_char;

    char key_ch = 0;
    char chup = UPPER_CASE_KEY(input_ch);

    while(key != kmax) {
      if(key->str[0] > len) {
        key_ch = key->str[len];
        if(chup == UPPER_CASE_KEY(key_ch))
          break;
      }
      key = key->next;
    }

    if(key == kmax)
      break;

    // If the input character is lowercase, check to see if there is
    // a lowercase match.  If so, set key to it.  Note: the
    // algorithm assumes the sorting performed by key_string_compare.

    if(input_ch != key_ch) {
      next = key->next;
      while(next != kmax) {
        if(next->str[0] > len) {
          char next_ch = next->str[len];
          if(next_ch == input_ch) {
            key = next;
            break;
          }
          if(next_ch != chup)
            break;
        }
        next = next->next;
      }
    }

    // Ok, we found the first position of a possible match.  If it
    // is exact, we are done.

    if(key->str[0] == len + 1)
      return key;

    // Apparently, there are some ambiguities. Read next key to
    // resolve the ambiguity.  Adjust kmax to encompass ambiguities.

    next = key->next;
    while(next != kmax) {
      if(next->str[0] > len) {
        char key_ch = next->str[len];
        if(chup != UPPER_CASE_KEY(key_ch))
          break;
      }
      next = next->next;
    }
    kmax = next;
  }

  return NULL;
}


//  ------------------------------------------------------------------

uint gkbd_getmappedkey() {

  gkbd_key_type* key = gkbd_do_key();
  if((key == NULL) or (not key->active)) {
    gkbd_flush_input();
    return (uint)-1;
  }

  return key->keysym;
}


//  ------------------------------------------------------------------
