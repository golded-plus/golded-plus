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
//  Multitasking.
//  ------------------------------------------------------------------

#ifndef __gutlmtsk_h
#define __gutlmtsk_h


//  ------------------------------------------------------------------

#include <gdefs.h>


//  ------------------------------------------------------------------
//  Supported multitaskers

const int GMTSK_NONE     = 0;
const int GMTSK_OS2      = 1;
const int GMTSK_WINDOWS  = 2;
const int GMTSK_DESQVIEW = 3;
const int GMTSK_DOS      = 4;
const int GMTSK_W32      = 5;
const int GMTSK_LINUX    = 6;


//  ------------------------------------------------------------------
//  Multitasker base class

class GMTsk {

public:

  int detected;
  const char* name;

  int os2();
  int win32();
  int windows();
  int desqview();
  int dosint28();

  void timeslice();

  GMTsk();
};


//  ------------------------------------------------------------------
//  Globals

extern GMTsk gmtsk;


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
