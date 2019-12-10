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
//  Decoder/encoders.
//  ------------------------------------------------------------------

#ifndef __gutlcode_h
#define __gutlcode_h
//  ------------------------------------------------------------------

#ifdef __GNUG__
#pragma interface "gutlcode.h"
#endif
//  ------------------------------------------------------------------

#include <gdefs.h>
//  ------------------------------------------------------------------
class code_engines
{
public: virtual char * decode(char * outputbuffer, const char * inputbuffer) = 0;
};

//  ------------------------------------------------------------------
class uucode_engine : public code_engines
{
protected: struct
    {
        char blank;
        int  len[63];
        int  ch[256];
    } table;

    int initialized;
    int defining_table;
    int table_index;
    void initialize();

public: int complete;
    uucode_engine();
    char * decode(char * outputbuffer, const char * inputbuffer);
};

//  ------------------------------------------------------------------
class quoted_printable_engine : public code_engines
{
protected: public: char * decode(char * outputbuffer, const char * inputbuffer);
};

//  ------------------------------------------------------------------
class base64_engine : public code_engines
{
protected: static char table[64];
public: char * decode(char * outputbuffer, const char * inputbuffer);
    char * encode(char * outputbuffer,
                  const char * inputbuffer,
                  uint length,
                  bool padding = true);
};

//  ------------------------------------------------------------------

#endif // ifndef __gutlcode_h
//  ------------------------------------------------------------------
