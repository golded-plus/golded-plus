//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1999 Alexander S. Aganichev
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
//  Midnight Commander compatible clipboard.
//  ------------------------------------------------------------------

#include <cstdlib>
#include <cstdio>
#include <gstrall.h>
#include <gmemdbg.h>
#include <gutlos.h>
#include <gmemi86.h>
#include <gfilutil.h>


//  ------------------------------------------------------------------

#define CLIPDIR  "~/.cedit"
#define CLIPFILE "~/.cedit/cooledit.clip"

#ifdef __BEOS__
char          ge_beos_title[GMAXTITLE] = "";
int           ge_beos_ext_title;
#endif

//  ------------------------------------------------------------------

int g_init_os(int flags) {

  NW(flags);
  return 0;
}


//  ------------------------------------------------------------------

void g_deinit_os(void) {

  // do nothing
}


//  ------------------------------------------------------------------

void g_init_title(char *tasktitle, int titlestatus) {

#ifndef __BEOS__
  NW(tasktitle); NW(titlestatus);
#else
  strncpy(ge_beos_title, tasktitle, GMAXTITLE);
  ge_beos_title[GMAXTITLE-1] = '\0';
  ge_beos_ext_title = titlestatus;
#endif
}


//  ------------------------------------------------------------------

void g_increase_priority(void) {

  // Do nothing
}


//  ------------------------------------------------------------------

void g_set_ostitle(char* title, word dx) {

 NW(dx);
#ifndef __BEOS__
  NW(title);
#else
  char fmt[]={'\x1b','\x5d','\x32','\x3b', '%', 's', '\x07', 0};
  fprintf(stdout, fmt, title);
  fflush(stdout);
#endif
}


//  ------------------------------------------------------------------

void g_set_osicon(void) {

  // do nothing
}


//  ------------------------------------------------------------------

bool g_is_clip_available(void) {

  std::string clipdir = CLIPDIR;
  strschg_environ(clipdir);
  return is_dir(clipdir);
}


//  ------------------------------------------------------------------

char* g_get_clip_text(void) {

  std::string clipfile = CLIPFILE;
  strschg_environ(clipfile);
  size_t size = GetFilesize(clipfile.c_str());

  if(size != -1) {
    char *text = (char *)throw_malloc(size+1);
    *text = NUL;

    FILE *f = fopen(clipfile.c_str(), "rt");
    if(f != NULL) {
      fread(text, 1, size, f);
      text[size] = NUL;
      fclose(f);
    }

    return text;
  }

  return NULL;
}


//  ------------------------------------------------------------------

int g_put_clip_text(const char* buf) {

  std::string clipfile = CLIPFILE;
  strschg_environ(clipfile);
  FILE *f = fopen(clipfile.c_str(), "wt");
  if(f != NULL) {
    fwrite(buf, 1, strlen(buf), f);
    fclose(f);
    return 0;
  }
  return -1;
}


//  ------------------------------------------------------------------

void g_get_ostitle_name(char* title) {

  *title = NUL;
}


//  ------------------------------------------------------------------

void g_set_ostitle_name(char* title, int mode) {

#ifndef __BEOS__
  NW(title); NW(mode);
#else
  if(mode == 0) {
    char fulltitle[GMAXTITLE];
    strcpy(fulltitle, ge_beos_title);
    if(ge_beos_ext_title) {
      int len = strlen(fulltitle);
      if(len < GMAXTITLE-4) {
        if(len)
          strcat(fulltitle, " - ");
        strncpy(fulltitle+len+3, title, GMAXTITLE-len-3);
        fulltitle[GMAXTITLE-1] = '\0';
      }
    }
    g_set_ostitle(fulltitle, 0);
  }
  else
    g_set_ostitle(title, 0);
#endif
}


//  ------------------------------------------------------------------

int g_send_mci_string(char* str, char* his_buffer) {

  NW(str); NW(his_buffer);
  return 1;
}


//  ------------------------------------------------------------------
