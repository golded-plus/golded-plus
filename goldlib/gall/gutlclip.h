//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
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
//  Goldware misc. clipboards support.
//  ------------------------------------------------------------------

#ifndef __gutlclip_h
#define __gutlclip_h


//  ------------------------------------------------------------------

#include <gdefs.h>


//  ------------------------------------------------------------------

#if defined(__OS2__)
#define CLIP_NAME "OS/2 Clipboard"
#elif defined(__WIN32__) || defined(__MSDOS__)
#define CLIP_NAME "Windows Clipboard"
#elif defined(__BEOS__)
#define CLIP_NAME "BeOS Clipboard"
#else
#define CLIP_NAME "Fake Clipboard"
#endif


//  ------------------------------------------------------------------

class gclipbrd {

  char *clipdata, *cliphdl;
  int len;

public:

  bool openread();
  char* read(char* buffer, int maxlen);
  void close();

  bool writeclipbrd(const char* buf);

  gclipbrd();

};


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
