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
//  Various WinOldAp functions.
//  ------------------------------------------------------------------

#include <cstdlib>
#include <cstdio>
#include <gstrall.h>
#include <gmemdbg.h>
#include <gutlos.h>
#include <gmemi86.h>
#ifdef __DJGPP__
#include <dos.h>
#include <unistd.h>
#include <dpmi.h>
#include <go32.h>
#include <sys/farptr.h>
#endif


//  ------------------------------------------------------------------

enum OSAPI {
  NOAPI,
  WIN3x,
  WIN9x,
  ANSIPLUS
};

OSAPI         winapi;
char          ge_win_oldtitle[GMAXTITLE] = "";
char          ge_win_title[GMAXTITLE] = "";
int           ge_win_ext_title;


//  ------------------------------------------------------------------

int g_init_os(int flags) {

  NW(flags);

  i86 cpu;

  // Identify WinOldAp version
  cpu.ax(0x1700);
  cpu.genint(0x2f);
  winapi = NOAPI;
  if(cpu.ax() != 0x1700) {
    winapi = WIN3x;
    // Identify Windows version and type
    cpu.ax(0x160a);
    cpu.genint(0x2f);
    if((cpu.ax() == 0x0000) and (cpu.bh() > 3))
      winapi = WIN9x;
  }
  else {
    // Check for ANSIPLUS.SYS v4.00+
    cpu.ax(0x1a00);
    cpu.bx(0x414e);
    cpu.cx(0x5349);
    cpu.dx(0x2b2b);
    cpu.genint(0x2f);
    if((cpu.al() == 0xff) and (cpu.ch() > 3))
      winapi = ANSIPLUS;
  }
  #ifdef __DJGPP__
  // reload internal djgpp structures for toupper/tolower
  int segment, selector;

  if((segment = __dpmi_allocate_dos_memory(1, &selector)) != -1) {
    cpu.ah(0x65);
    cpu.al(0x02);
    cpu.bx(0xffff);
    cpu.dx(0xffff);
    cpu.cx(5);
    cpu.es(segment);
    cpu.di(0);
    cpu.genint(0x21);
    if(not (cpu.flags() & 1) and (cpu.cx() == 5)) {
      unsigned int table = _farpeekw(selector, 3) * 16 + _farpeekw(selector, 1);
      int i, size = _farpeekw(_dos_ds, table);
      movedata(_dos_ds, table + 2, _my_ds(), (unsigned int)&(toupper(128)), size);
      // let's build lowercase table from uppercase...
      for(i = 0; i < size; i++) {
        int c = toupper(i + 128);
        if((c != i + 128) and (c > 127))
          tolower(c) = i + 128;
      }
      for(i=128; i<256; i++) {
        if((toupper(tolower(i)) != i) && (tolower(toupper(i)) != i))
          tolower(i) = toupper(i) = i;
        if(toupper(tolower(i)) != toupper(i))
          toupper(i) = i;
        if(tolower(toupper(i)) != tolower(i))
          tolower(i) = i;
      }
    }
    __dpmi_free_dos_memory(selector);
  }
  #endif
  g_get_ostitle_name(ge_win_oldtitle);
  return 0;
}


//  ------------------------------------------------------------------

void g_deinit_os(void) {

  if(*ge_win_oldtitle)
    g_set_ostitle_name(ge_win_oldtitle, 1);
}


//  ------------------------------------------------------------------

void g_init_title(char *tasktitle, int titlestatus) {

  strxcpy(ge_win_title, tasktitle, GMAXTITLE);
  ge_win_ext_title = titlestatus;
}


//  ------------------------------------------------------------------

void g_increase_priority(void) {

  // Do nothing
}


//  ------------------------------------------------------------------

void g_set_ostitle(char* title, word dx) {

  if(winapi == WIN9x) {

    if(dx and not *title)
      return;

    i86 cpu;
    int segment, selector;
    size_t len = strlen(title) + 1;

    segment = __dpmi_allocate_dos_memory((len >> 4) + 1, &selector);
    if(segment == -1)
      return;
    movedata(_my_ds(), (unsigned)title, selector, 0, len);
    // Set application title
    cpu.ax(0x168e);
    cpu.dx(dx);
    cpu.es(segment);
    cpu.di(0x0000);
    cpu.genint(0x2f);
    __dpmi_yield();
    __dpmi_free_dos_memory(selector);
  }
}


//  ------------------------------------------------------------------

void g_set_osicon(void) {

  // do nothing
}


//  ------------------------------------------------------------------

bool g_is_clip_available(void) {

  return (winapi == NOAPI) ? false : true;
}


//  ------------------------------------------------------------------

char* g_get_clip_text(void) {

  if(winapi == NOAPI)
    return NULL;

  i86 cpu;
  int seg, selector;
  char* text = NULL;
  size_t len;

  if((winapi == WIN3x) or (winapi == WIN9x)) {

    // Open clipboard
    cpu.ax(0x1701);
    cpu.genint(0x2f);
    if(cpu.ax() == 0x0000)
      return NULL;

    // Get clipboard data size
    cpu.ax(0x1704);
    cpu.dx(0x07);
    cpu.genint(0x2f);
    len = cpu.ax() + (cpu.dx() << 16);
    if(len != 0)
      if((seg = __dpmi_allocate_dos_memory(len >> 4, &selector)) != -1) {
        // Get clipboard data
        cpu.ax(0x1705);
        cpu.dx(0x07); // OEM text
        cpu.es(seg);
        cpu.bx(0x0000);
        cpu.genint(0x2f);
        if(cpu.ax() != 0x0000) {
          text = (char *) throw_malloc(len);
          if(text)
            movedata(selector, 0, _my_ds(), (unsigned) text, len);
        }
        __dpmi_free_dos_memory(selector);
      }

    // Close clipboard
    cpu.ax(0x1708);
    cpu.genint(0x2f);
  }
  else {
    // Get clipboard information
    cpu.ax(0x1aa5);
    cpu.dh(0x00);
    cpu.genint(0x2f);
    if(cpu.al() == 0x00) {
      len = cpu.cx();
      if(len)
        if((seg = __dpmi_allocate_dos_memory((len >> 4) + 1, &selector)) != -1) {
          // Get clipboard text
          cpu.ax(0x1aa5);
          cpu.dh(0x01);
          cpu.es(seg);
          cpu.bx(0x0000);
          cpu.genint(0x2f);
          if(cpu.al() == 0x00) {
            text = (char *) throw_malloc(len);
            if(text)
              movedata(selector, 0, _my_ds(), (unsigned) text, len);
          }
          __dpmi_free_dos_memory(selector);
        }
    }
  }

  return text;
}


//  ------------------------------------------------------------------

int g_put_clip_text(char* buf) {

  if(winapi == NOAPI)
    return -1;

  i86 cpu;
  int seg, selector;
  int result = -1;
  size_t len = strlen(buf);

  if((winapi == WIN3x) or (winapi == WIN9x)) {
    // Open clipboard
    cpu.ax(0x1701);
    cpu.genint(0x2f);
    if(cpu.ax() == 0x0000)
      return -1;

    // Empty clipboard
    cpu.ax(0x1702);
    cpu.genint(0x2f);

    if(len>0) {
      if((seg = __dpmi_allocate_dos_memory((len >> 4) + 1, &selector)) != -1) {
        movedata(_my_ds(), (unsigned) buf, selector, 0, len);
        // Set clipboard data
        cpu.ax(0x1703);
        cpu.dx(0x07);
        cpu.si(len >> 16);
        cpu.cx(len & 0xffff);
        cpu.es(seg);
        cpu.bx(0);
        cpu.genint(0x2f);
        __dpmi_free_dos_memory(selector);
        if(cpu.ax() != 0x0000)
          result = 0;
      }
    }

    // Close clipboard
    cpu.ax(0x1708);
    cpu.genint(0x2f);
  }
  else {
    // Clear clipboard
    cpu.ax(0x1aa5);
    cpu.dh(0x04);
    cpu.genint(0x2f);
    if(len > 0)
      if((seg = __dpmi_allocate_dos_memory((len >> 4) + 1, &selector)) != -1) {
        movedata(_my_ds(), (unsigned) buf, selector, 0, len);
        // Set clipboard text
        cpu.ax(0x1aa5);
        cpu.dh(0x01);
        cpu.es(seg);
        cpu.bx(0x0000);
        cpu.cx(len & 0xffff);
        cpu.genint(0x2f);
        __dpmi_free_dos_memory(selector);
        if(cpu.al() == 0x00)
          result = 0;
      }
  }
  return result;
}


//  ------------------------------------------------------------------

void g_get_ostitle_name(char* title) {

  *title = NUL;
  if(winapi == WIN9x) {
    i86 cpu;
    int segment, selector;

    segment = __dpmi_allocate_dos_memory((GMAXTITLE >> 4) + 1, &selector);
    if(segment == -1)
      return;
    // Get vdm title
    cpu.ax(0x168e);
    cpu.dx(3);
    cpu.es(segment);
    cpu.di(0);
    cpu.cx(GMAXTITLE);
    cpu.genint(0x2f);
    movedata(selector, 0, _my_ds(), (unsigned)title, GMAXTITLE);
    strcat(title, " - ");
    int len = strlen(title);
    // Get application title
    cpu.ax(0x168e);
    cpu.dx(2);
    cpu.es(segment);
    cpu.di(0);
    cpu.cx(GMAXTITLE - len);
    cpu.genint(0x2f);
    movedata(selector, 0, _my_ds(), (unsigned)title + len, GMAXTITLE - len);
    __dpmi_free_dos_memory(selector);
    len = strlen(title);
    if(streql(title + len - 3, " - "))
      title[len-3] = NUL;
  }
}


//  ------------------------------------------------------------------

void g_set_ostitle_name(char* title, int mode) {

  if(mode == 0) {
    if(ge_win_ext_title)
      g_set_ostitle(title, 0);
  }
  else {
    char* p;
    char* s = throw_xstrdup(title);
    if((p = strstr(s, " - ")) != NULL) {
      *p = NUL;
      p += 3;
    }
    else
      p = s + strlen(s);
    g_set_ostitle(p, 0);
    g_set_ostitle(s, 1);
    throw_xfree(s);
  }
}


//  ------------------------------------------------------------------
