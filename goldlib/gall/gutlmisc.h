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

#ifndef __gutlmisc_h
#define __gutlmisc_h


//  ------------------------------------------------------------------

#include <gdefs.h>
#include <gctype.h>
#include <cstdlib>


//  ------------------------------------------------------------------
//  Prototypes

uint32_t atoulx(const char* s);

inline word atow(const char* p) { return (word)atoi(p); }
inline int xtoi(char c)  { return isdigit(c) ? (c - '0') : (toupper(c) - ('A' - 10)); }
inline int atox(const char* s) { return (int)atoulx(s); }

char* ltob(char* dst, uint32_t value, int fill=32);

int GetYesno(const char* value);
int Pct(uint32_t x, uint32_t y);
int tabstop(int col, int tabwidth);

char* ggetosstring();

// Convert between Microsoft MKS format and long int
dword B2L(dword b);
dword L2B(dword l);

inline void SwapWord32(uint32_t* dw) { *dw = ((*dw) << 16) | ((*dw) >> 16); }


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------

