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

#include <sys/types.h>
#include <cstddef>
#include <regex.h>
#include <gregex.h>
#include <gmemdbg.h>


//  ------------------------------------------------------------------

gregex::gregex() {

  preg = NULL;
}


//  ------------------------------------------------------------------

gregex::~gregex() {

  reset();
}


//  ------------------------------------------------------------------

void gregex::reset() {

  if(preg) {
    regfree((regex_t*)preg);
    throw_delete((regex_t*)preg);
  }
}


//  ------------------------------------------------------------------

bool gregex::compile(const char* pattern, int cflags) {

  int cflgs = REG_NOSUB;
  if(cflags & extended) cflgs |= REG_EXTENDED;
  if(cflags & icase)    cflgs |= REG_ICASE;
  if(cflags & newline)  cflgs |= REG_NEWLINE;

  if(not preg) {
    preg = new regex_t;
    throw_new(preg);
  }

  return (bool)regcomp((regex_t*)preg, pattern, cflgs);
}


//  ------------------------------------------------------------------

bool gregex::match(const char* str, int eflags) {

  int eflgs = 0;
  if(eflags & notbol) eflgs |= REG_NOTBOL;
  if(eflags & noteol) eflgs |= REG_NOTEOL;

  return not regexec((regex_t*)preg, str, 0, NULL, eflgs);
}


//  ------------------------------------------------------------------
