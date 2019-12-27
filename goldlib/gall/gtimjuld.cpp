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
//  Julian date conversion functions.
//  ------------------------------------------------------------------

#include <gtimall.h>


//  ------------------------------------------------------------------

inline int isleap(unsigned yr)
{
    return (yr % 400 == 0) or ((yr % 4 == 0) and (yr % 100 != 0));
}
inline unsigned months_to_days(unsigned month)
{
    return (month * 3057 - 3007) / 100;
}
inline long years_to_days(unsigned yr)
{
    return yr * 365L + yr / 4 - yr / 100 + yr / 400;
}


//  ------------------------------------------------------------------

long YMD2JDN(unsigned yr, unsigned mo, unsigned day)
{

    long scalar;
    scalar = day + months_to_days(mo);
    if(mo > 2)                           /* adjust if past February */
        scalar -= isleap(yr) ? 1 : 2;
    yr--;
    scalar += years_to_days(yr);
    return scalar - 693595L;
}


//  ------------------------------------------------------------------

void JDN2YMD(long scalar, unsigned *yr, unsigned *mo, unsigned *day)
{

    scalar += 693595L;

    unsigned n;                /* compute inverse of years_to_days() */

    for(n = (unsigned)((scalar * 400L) / 146097L); years_to_days(n) < scalar;)
        n++;                           /* 146097 == years_to_days(400) */
    *yr = n;
    n = (unsigned)(scalar - years_to_days(n-1));
    if(n > 59)                            /* adjust if past February */
    {
        n += 2;
        if(isleap(*yr))
            n -= n > 62 ? 1 : 2;
    }
    *mo = (n * 100 + 3007) / 3057;    /* inverse of months_to_days() */
    *day = n - months_to_days(*mo);
}


//  ------------------------------------------------------------------
