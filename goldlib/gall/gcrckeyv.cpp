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
//  CRC-16, CRC-32 and Hashing
//  ------------------------------------------------------------------

#include <gstrall.h>
#include <gcrcall.h>


//  ------------------------------------------------------------------
//  Split current value (val) into keyword (key) and new value
//  If "eql" is true, the '=' char is a keyword separator char.

void getkeyvaleql(char** key, char** val, bool eql)
{

    // Skip leading whitespace
    char *p = strskip_wht(*val);

    // Get the keyword
    switch(*p)
    {

    // Strip "double" quotes
    case '\"':
        *key = ++p;
        do
        {
            p = strskip_to(p, '\"');
            if(not *p or (*(p-1) != '\\'))
                break;
            else
                p++;
        }
        while(*p);
        break;

    // Strip 'single' quotes
    case '\'':
        *key = ++p;
        do
        {
            p = strskip_to(p, '\'');
            if(not *p or (*(p-1) != '\\'))
                break;
            else
                p++;
        }
        while(*p);
        break;

    // Strip round brackets
    case '(': /*)*/
        *key = ++p;
        p = strskip_to(p, /*(*/ ')');
        break;

    // Get straight keyword
    default:
        *key = p;
        while(*p and not isspace(*p) and (*p != ','))
        {
            if(eql and (*p == '='))
                break;
            p++;
        }
    }

    // Nul-terminate keyword and remove trailing comma if any
    if(*p)
    {
        if((*p != '\"') and (*p != '\'') and (*p != /*(*/ ')'))
        {
            char* q = p;
            while(*(--q) == ',')
                *q = NUL;
        }
        *p++ = NUL;
    }

    // Skip whitespace and '=' if any
    p = strskip_wht(p);
    if(eql and (*p == '='))
        p = strskip_wht(++p);

    // Get the value
    *val = p;

    // Handle comments, allowing ;'s in quoted strings
    while(*p and *p != ';')
    {
        if((*p == '\"') and (*(p-1) != '\\'))
        {
            p++;
            while(*p and not ((*p == '\"') and (*(p-1) != '\\')))
                p++;
        }
        if(*p)
            p++;
    }
    *p = NUL;

    // Trim whitespace from end of value
    p = *val;
    if(*p)
    {
        p = strskip_to(p, NUL);
        while((*(--p) <= ' ') and *p)
            *p = NUL;
    }
    strschg(*key, "\\\"", "\"");
}


//  ------------------------------------------------------------------
//  Split current value (val) into keyword (key) and new value

void getkeyval(char** key, char** val)
{

    getkeyvaleql(key, val, false);
}


//  ------------------------------------------------------------------
//  Get keyword and value and calculate/return CRC16 of keyword

word getkeyvalcrc(char** key, char** val)
{

    // First get keyword and value
    getkeyvaleql(key, val, false);

    // Init variables
    char* p = *key;
    word c = 0;

    // Calculate CRC16 of keyword
    while(*p)
        c = updCrc16((byte)g_toupper(*p++), c);

    // Return keyword CRC
    return c;
}


//  ------------------------------------------------------------------
