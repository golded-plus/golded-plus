//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999-2000 Alexander S. Aganichev
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
//  GCUI: Golded+ Character-oriented User Interface.
//  Help.
//  ------------------------------------------------------------------

#ifndef __gwinhelp_h
#define __gwinhelp_h


//  ------------------------------------------------------------------

#include <gfile.h>
#include <gwinall.h>


//  ------------------------------------------------------------------
//  Window help information record

class _help_t {

public:

  int         help[20];       // help stack
  const char* file;           // help file name
  int         helpptr;        // help stack pointer
  gkey        key;            // help hot key
  vattr       winattr;        // help window attribute
  vattr       textattr;       // help window text attribute
  vattr       selattr;        // selection text attribute
  vattr       barattr;        // selection bar attribute
  int         srow;           // help window start row
  int         scol;           // help window start column
  int         erow;           // help window end row
  int         ecol;           // help window end column
  int         btype;          // help window box type
  int         title;          // display "Help" title?
  VfvCP       open;           // pointer to open function
  gfile*      fp;             // help file
  long        offset;         // help file offset

  _help_t()
  {
    memset(help, 0, sizeof(help));
    file = NULL;
    helpptr = -1;
    key = 0;
    winattr = BLACK_|_BLACK;
    textattr = BLACK_|_BLACK;
    selattr = BLACK_|_BLACK;
    barattr = BLACK_|_BLACK;
    srow = 3;
    scol = 8;
    erow = 21;
    ecol = 71;
    btype = 0;
    title = YES;
    open = NULL;
    fp = NULL;
    offset = 0;
  }
};


//  ------------------------------------------------------------------
//  Help index file record definition

#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif

typedef struct _hlpidx_t {
  word help;
  char category[30];
  long offset;
} Hlpr;

#if defined(GOLD_CANPACK)
#pragma pack()
#endif


//  ------------------------------------------------------------------

int whelpcat(int cat);
int whelpclr();
int whelpdef(const char* file, gkey key, vattr winattr, vattr textattr, vattr selattr, vattr barattr, VfvCP open);
int whelpop();
int whelpopc();
int whelppcat(int cat);
int whelpush();
int whelpushc(int cat);
int whelpwin(int srow, int scol, int erow, int ecol, int btype, int title);
void whelpcompile(const char* helpfile, long& offset);
inline int whelpundef() { return whelpdef(NULL,0,BLACK_|_BLACK,BLACK_|_BLACK,BLACK_|_BLACK,BLACK_|_BLACK,NULL); }


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
