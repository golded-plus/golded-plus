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

  NW(tasktitle); NW(titlestatus);
}


//  ------------------------------------------------------------------

void g_increase_priority(void) {

  // Do nothing
}


//  ------------------------------------------------------------------

void g_set_ostitle(char* title, word dx) {

  NW(title); NW(dx);
}


//  ------------------------------------------------------------------

void g_set_osicon(void) {

  // do nothing
}


//  ------------------------------------------------------------------

bool g_is_clip_available(void) {

  return is_dir(CLIPDIR);
}


//  ------------------------------------------------------------------

char* g_get_clip_text(void) {

  size_t size = GetFilesize(CLIPFILE);
  char *text = (char *)throw_malloc(size+1);
  *text = NUL;

  FILE *f = fopen(CLIPFILE, "rt");
  if(f != NULL) {
    fread(text, 1, size, f);
    text[size] = NUL;
    fclose(f);
  }

  return text;
}


//  ------------------------------------------------------------------

int g_put_clip_text(const char* buf) {

  FILE *f = fopen(CLIPFILE, "wt");
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

  NW(title); NW(mode);
}


//  ------------------------------------------------------------------

int g_send_mci_string(char* str, char* his_buffer) {

  NW(str); NW(his_buffer);
  return 1;
}


//  ------------------------------------------------------------------
