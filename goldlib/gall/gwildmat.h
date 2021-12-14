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
//  A wildcard pattern matching function. Based on TIN source.
//  ------------------------------------------------------------------


#ifndef __GWILDMAT_H
#define __GWILDMAT_H

#include <gdefs.h>


//  ------------------------------------------------------------------

class gwildmatch
{

protected:

    int match_internal(const char* text, const char* pattern, bool ignorecase);

public:

    bool match(const char* text, const char* pattern, bool ignorecase);

};


//  ------------------------------------------------------------------

inline bool gwildmat(const char* text, const char* pattern, bool ignorecase=true)
{

    class gwildmatch m;
    return m.match(text, pattern, ignorecase);
}


//  ------------------------------------------------------------------

inline bool gwildmati(const char* text, const char* pattern)
{

    class gwildmatch m;
    return m.match(text, pattern, true);
}


//  ------------------------------------------------------------------
//  4DOS-style wildcard pattern match

bool strwild(const char* str, const char* wild);


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
