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

#ifndef __gshare_h
#define __gshare_h


//  ------------------------------------------------------------------

#include <gdefs.h>


//  ------------------------------------------------------------------

#if not defined(__GNUC__) or defined(__DJGPP__) or defined(__EMX__) or defined(__MINGW32__)
#include <share.h>
#if defined(__MINGW32__)        // SH_COMPAT doesn't work in Mingw32
#undef SH_COMPAT
#endif
#elif !defined(SH_DENYNO)
#define	SH_DENYRW	0x10	// Deny read/write
#define	SH_DENYWR	0x20	// Deny write
#define	SH_DENYRD	0x30	// Deny read
#define	SH_DENYNO	0x40	// Deny nothing
#endif

#ifndef SH_COMPAT
#define SH_COMPAT SH_DENYNO
#endif


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
