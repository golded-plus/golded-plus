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
//  Regular expressions C++ wrapper class.
//  ------------------------------------------------------------------

#ifndef __gregex_h
#define __gregex_h


//  ------------------------------------------------------------------

#include <gdefs.h>


//  ------------------------------------------------------------------

class gregex {

protected:

  void* preg;

public:

  enum {

    // cflags
    extended = 0x01,
    icase    = 0x02,
    newline  = 0x04,

    // eflags
    notbol   = 0x10,
    noteol   = 0x20
  };

  gregex();
  ~gregex();

  void reset();

  bool compile(const char* pattern, int cflags=0);
  bool match(const char* str, int eflags=0);

};


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
