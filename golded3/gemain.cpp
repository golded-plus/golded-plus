
//  ------------------------------------------------------------------
//  GoldED+
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999-2000 Alexander S. Aganichev
//  ------------------------------------------------------------------
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License as
//  published by the Free Software Foundation; either version 2 of the
//  License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston,
//  MA 02111-1307 USA
//  ------------------------------------------------------------------
//  $Id$
//  ------------------------------------------------------------------
//  Main entry point and overlay size setting function.
//  ------------------------------------------------------------------

#include <golded.h>
#include <gutlos.h>

#if 0 && defined(_MSC_VER) && defined(_DEBUG) && defined(_M_IX86)
#include <memleak.h>

static struct LeakFinder
{
    LeakFinder()
    {
        InitAllocCheck();
    }

    ~LeakFinder()
    {
        DeInitAllocCheck();
    }
} leakFinder;
#endif

//  ------------------------------------------------------------------
//  Main function

int main(int argc, char *argv[])
{

    Initialize(argc, argv);
    Reader();
    Uninitialize();
    return errorlevel;
}


//  ------------------------------------------------------------------
