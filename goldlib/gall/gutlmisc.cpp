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
//  Utility functions.
//  ------------------------------------------------------------------

#include <cstdlib>
#include <gutlmisc.h>
#include <gstrall.h>


//  ------------------------------------------------------------------
//  Get yes/no value

int GetYesno(const char* value) {

  if(isdigit(*value))
    return atoi(value);

  switch(toupper(*value)) {

    case NUL:   // Blank
    case 'T':   // True
    case 'Y':   // Yes
      return YES;

    case 'F':   // False
    case 'N':   // No
      return NO;

    case 'O':   // On or Off
      if(toupper(value[1]) == 'N')
        return YES;
      else
        return NO;

    case 'A':   // Always, Ask or Auto
      if(toupper(value[1]) == 'L')
        return ALWAYS;
      else if(toupper(value[1]) == 'S')
        return ASK;
      else
        return GAUTO;

    case 'M':   // Maybe
      return MAYBE;
  }

  return NO;
}


//  ------------------------------------------------------------------
//  Calculates a percentage

int Pct(ulong x, ulong y) {

  if(x) {

    ulong p = (((x-y)*100)/x);
    ulong r = (((x-y)*100)%x);

    if(((r*10)/x)>4)
      p++;

    return (int)p;
  }

  return 100;
}


//  ------------------------------------------------------------------
//  Calculates next tab stop from given column

int tabstop(int column, int tabwidth) {

  int sum = column + tabwidth;
  return sum - (sum % tabwidth);
}


//  ------------------------------------------------------------------
//  Convert hex string to integer

ulong atoulx(const char* s) {

  ulong retval = 0;

  s = strskip_wht(s);

  while(isxdigit(*s)) {
    retval <<= 4;
    retval |= xtoi(*s);
    s++;
  }

  return retval;
}


//  ------------------------------------------------------------------

char* ltob(char* dst, ulong value, int fill) {

  char* p = dst;

  for(int b=1,g=0; b<33; b++) {
    if(value & 0x80000000L) {
      g = 1;
      *p++ = '1';
    }
    else {
      if(g or (b >= fill))
        *p++ = '0';
    }
    value <<= 1;
  }
  *p = NUL;

  return dst;
}


// ------------------------------------------------------------------

dword B2L(dword b) {

  byte* bp = (byte*)&b;
  return ((*bp + ((dword)bp[1] << 8) + ((dword)bp[2] << 16)) | 0x800000L) >> (24 - (bp[3] - 0x80));
}


// ------------------------------------------------------------------

dword L2B(dword l) {

  // Special case for zero
  if(l == 0)
    return 0;

  // Look for the largest power of 2
  int s = 31;
  dword p = 0x80000000UL;
  while(s > 0) {
    if(p & l)
      break;
    p >>= 1;
    s--;
  }

  // Fillup the mantisse with useful information
  dword b = 0;
  int n = 0;
  int e = s--;
  l -= 1UL << e;
  while(l > 0) {
    dword t = (1UL << s);
    if(l >= t) {
      b += 1UL << (22 - n);
      if(l < t)
        break;
      l -= t;
    }
    n++;
    s--;
  }

  return b | ((0x81UL+e) << 24);
}


//  ------------------------------------------------------------------
