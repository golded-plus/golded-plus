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
//  Miscellaneous memory functions
//  ------------------------------------------------------------------

#include <cstdio>
#include <gmemall.h>


//  ------------------------------------------------------------------
//  Dump 16 bytes as hex and characters

char* HexDump16(char* strbuf, const char* memptr, int limit, const char* fmt, int fmtno) {

  int n;
  word mem[16];
  char str[17];
  word* mptr=mem;
  char* sptr=str;
  const char* fmts[] = {
    fmt,
    "%02X%02X %02X%02X %02X%02X %02X%02X  %02X%02X %02X%02X %02X%02X %02X%02X  %s",
    "%02X %02X %02X %02X  %02X %02X %02X %02X  %02X %02X %02X %02X  %02X %02X %02X %02X  %s"
  };

  limit = (limit > 16) ? 16 : limit;

  for(n=0; n<limit; n++, memptr++) {
    *mptr++ = *memptr;
    *sptr++ = *memptr >= ' ' ? *memptr : '.';
  }
  for(; n<16; n++, memptr++) {
    *mptr++ = 0;
    *sptr++ = '.';
  }
  *sptr = NUL;

  sprintf(strbuf, fmts[fmtno],
    mem[0], mem[1], mem[2],  mem[3],  mem[4],  mem[5],  mem[6],  mem[7],
    mem[8], mem[9], mem[10], mem[11], mem[12], mem[13], mem[14], mem[15],
    str
  );

  return strbuf;
}


//  ------------------------------------------------------------------
