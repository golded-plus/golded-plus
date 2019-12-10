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
//  Windowing wrapper class.
//  ------------------------------------------------------------------

#include <cstdio>
#include <cstdarg>
#include <gwindow.h>
//  ------------------------------------------------------------------
int gwindow::printf(const char * format, ...)
{
    char buf[255];
    va_list argptr;

    va_start(argptr, format);
    int result = vsprintf(buf, format, argptr);
    va_end(argptr);
    puts(buf);
    return result;
}

//  ------------------------------------------------------------------
int gwindow::printf(vattr color, const char * format, ...)
{
    char buf[255];
    va_list argptr;

    va_start(argptr, format);
    int result = vsprintf(buf, format, argptr);
    va_end(argptr);
    text_color(color);
    puts(buf);
    return result;
}

//  ------------------------------------------------------------------
int gwindow::printf(int row, int col, const char * format, ...)
{
    va_list argptr;
    char buf[256];

    *buf = NUL;
    va_start(argptr, format);
    int result = vsprintf(buf, format, argptr);
    va_end(argptr);
    prints(row, col, window_color, buf);
    return result;
}

//  ------------------------------------------------------------------
int gwindow::printf(int row, int col, vattr color, const char * format, ...)
{
    va_list argptr;
    char buf[256];

    *buf = NUL;
    va_start(argptr, format);
    int result = vsprintf(buf, format, argptr);
    va_end(argptr);
    prints(row, col, color, buf);
    return result;
}

//  ------------------------------------------------------------------
