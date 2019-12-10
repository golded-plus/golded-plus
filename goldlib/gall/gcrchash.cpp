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
//  CRC-16, CRC-32 and Hashing
//  ------------------------------------------------------------------

#include <gctype.h>
#include <gcrcall.h>
//  ------------------------------------------------------------------
//  Hash value generation
//  ------------------------------------------------------------------
//  From the Maximus/Squish MSGAPI by Scott J. Dudley, 1991.
//  The prime is 65521. Whatever that means.. ;-)
//  ------------------------------------------------------------------
//  ------------------------------------------------------------------
//  Generate 32-bit hash value from string
dword strHash32(const char * s, bool __case)
{
    const char * p = s;
    dword g, hash = 0;

    while(*p)
    {
        hash = (hash << 4) + (__case ? g_tolower(*p) : *p);
        g    = hash & 0xF0000000UL;

        if(g)
        {
            hash |= g >> 24;
            hash |= g;
        }

        p++;
    }
    return hash & 0x7FFFFFFFUL; // Strip off high bit (used as a flag)
}

//  ------------------------------------------------------------------
