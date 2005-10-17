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
//  Midnight Commander's clipboard support

#define CLIPDIR  "~/.mc/cedit/"
#define CLIPDIR_OLD  "~/.cedit/"
#define CLIPFILE "cooledit.clip"

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

  NW(dx); NW(title);
}


//  ------------------------------------------------------------------

void g_set_osicon(void) {

  // do nothing
}


//  ------------------------------------------------------------------

bool g_is_clip_available(void) {
  bool rc;

  std::string clipdir = CLIPDIR;
  strschg_environ(clipdir);
  rc = is_dir(clipdir);
  if( !rc ) {
    std::string clipdir = CLIPDIR_OLD;
    strschg_environ(clipdir);
    rc = is_dir(clipdir);
  }
  return rc;
}


//  ------------------------------------------------------------------

char* g_get_clip_text(void) {

  std::string clipfile = CLIPDIR;
  clipfile += CLIPFILE;
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

  std::string clipfile = CLIPDIR;
  clipfile += CLIPFILE;
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

  NW(title); NW(mode);
}


//  ------------------------------------------------------------------

int g_send_mci_string(char* str, char* his_buffer) {

  NW(str); NW(his_buffer);
  return 1;
}


//  ------------------------------------------------------------------
