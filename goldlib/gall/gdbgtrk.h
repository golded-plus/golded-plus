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
//  Function tracker.
//  ------------------------------------------------------------------

#ifndef __gdbgtrck_h
#define __gdbgtrck_h
//  ------------------------------------------------------------------

#include <gdefs.h>
//  ------------------------------------------------------------------
struct GFTrk
{
    dword        tick;
    const char * text;
    int          indent;
};

//  ------------------------------------------------------------------
extern int __gftrk;
extern int __gftrk_on;
extern int __gftrk_max;
extern int __gftrk_curr;
extern int __gftrk_indent;
extern GFTrk * __gftrk_ptr;
extern GFTrk * __gftrk_list;
//  ------------------------------------------------------------------

#if defined (GFTRK_ENABLE)
    #if defined (__GNUC__)
#define GFTRK(t) if(__gftrk) __gftrk_track(t ? __PRETTY_FUNCTION__ : t);
    #elif defined (_MSC_VER) && defined (__FUNCSIG__)
#define GFTRK(t) if(__gftrk) __gftrk_track(t ? __FUNCSIG__ : t);
    #else
#define GFTRK(t) if(__gftrk) __gftrk_track(t);
    #endif
#define GFTrkInit(t) __gftrk_init(t)
#else
#define GFTRK(t)
#define GFTrkInit(t)
#endif
//  ------------------------------------------------------------------
void __gftrk_init(int trackmax = -1);
void __gftrk_track(const char * text);
void __gftrk_log();

//  ------------------------------------------------------------------

#endif // ifndef __gdbgtrck_h
//  ------------------------------------------------------------------
