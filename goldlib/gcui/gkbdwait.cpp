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
//  GCUI: Golded+ Character-oriented User Interface.
//  Keyboard functions
//  ------------------------------------------------------------------

#include <gkbdbase.h>
#include <gkbdcode.h>
#include <gtimall.h>
#include <gutlmtsk.h>


//  ------------------------------------------------------------------
//  Halts execution until a key is pressed

gkey waitkey()
{

    clearkeys();
    return getxch();
}


//  ------------------------------------------------------------------
//  Halts execution until a key is pressed or the specified time
//  period has expired

gkey waitkeyt(int duration)
{

    clearkeys();
    Clock stop = gclock() + duration;
    Clock sliced_time = gclock();
    while(1)
    {
        if(kbmhit())
            return getxch();
        if(gclock() >= stop)
            return 0;
        if(gclock() - sliced_time >= 10)
        {
            if(gkbd.tickfunc)
                (*gkbd.tickfunc)();
            sliced_time = gclock();
        }
        gmtsk.timeslice();
    }
}


//  ------------------------------------------------------------------
