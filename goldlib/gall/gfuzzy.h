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
//  Fuzzy string search.
//  ------------------------------------------------------------------

#ifndef __gfuzzy_h
#define __gfuzzy_h
//  ------------------------------------------------------------------

#include <gdefs.h>
//  ------------------------------------------------------------------
class gfuzzy
{
protected: const char * text;
    const char * pattern; // Pointers to search strings
    int textloc;    // Current search position in Text
    int plen;       // Length of Pattern
    int degree;     // Max degree of allowed mismatch
    int * ldiffs;
    int * ldiff;
    int * rdiff;    // Dynamic difference arrays
    int * loffs;
    int * roffs;    // Used to calculate start of match
    bool casing;
public: gfuzzy();
    ~gfuzzy();
    const char * start;
    const char * end;
    int howclose;
int length()
{
    return end - start + 1;
}

    void init(const char * pat, int fuzzydegree, bool case_sensitive);
    bool findfirst(const char * string);
    bool findnext();
};

//  ------------------------------------------------------------------

#endif // ifndef __gfuzzy_h
//  ------------------------------------------------------------------
