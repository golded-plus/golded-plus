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
//  Portable NLS functions for ctype.
//  ------------------------------------------------------------------

#ifndef __gctype_h
#define __gctype_h


//  ------------------------------------------------------------------

#include <gcmpall.h>


//  ------------------------------------------------------------------

#ifdef __BORLANDC__
#define __USELOCALES__
#elif defined(__EMX__)
#define _CTYPE_FUN
#endif
#include <ctype.h>
#if defined(__EMX__)
#include <sys/nls.h>
#define tolower(c) _nls_tolower((unsigned char)(c))
#define toupper(c) _nls_toupper((unsigned char)(c))
#elif defined(__WIN32__)
#ifdef __cplusplus
extern "C" {
extern char tl[256], tu[256];
__inline__ int tolower(int c) { return tl[c]; }
__inline__ int toupper(int c) { return tu[c]; }
}
#else
extern char tl[256], tu[256];
#define tolower(c) tl[(int)(unsigned char)(c)]
#define toupper(c) tu[(int)(unsigned char)(c)]
#endif
#endif


//  ------------------------------------------------------------------

// NLS chars detected by converting to lower or upper case and in case they don't match they treated as characters

#ifdef __cplusplus
extern "C" {
#endif
extern __inline__ int isxalnum(char c) { return isascii(c) ? isalnum(c) : (c != tolower(c)) || (c != toupper(c)); }
#ifdef __cplusplus
}
#endif


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
