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
//  GoldED printer definition helper.
//  ------------------------------------------------------------------

#include <cstdio>
#include <cstdlib>
#include <gstrall.h>


//  ------------------------------------------------------------------
//  Convert printer definition string to binary codes

char* CvtPrnstr(char* str, char* prn)
{

    uint value;
    byte len=0;
    char buf[256];
    char* ptr=prn;

    while(*ptr)
    {
        switch(*ptr)
        {
        case '$':
            ptr++;
            do
            {
                sscanf(ptr, "%2x", &value);
                buf[len++] = (byte)value;
                if(isxdigit(*ptr) and *ptr)   // Skip first digit
                    ptr++;
                if(isxdigit(*ptr) and *ptr)   // Skip second digit
                    ptr++;
            }
            while(isxdigit(*ptr) and *ptr);
            break;
        case '#':
            ptr++;
            value = (byte)atoi(ptr);
            buf[len++] = value;
            while(isdigit(*ptr) and *ptr)
                ptr++;
            break;
        case '\"':
            ptr++;
            while(*ptr != '\"' and *ptr)
                buf[len++] = *ptr++;
            if(*ptr)
                ptr++;
            break;
        case '\'':
            ptr++;
            while(*ptr != '\'' and *ptr)
                buf[len++] = *ptr++;
            if(*ptr)
                ptr++;
            break;
        default:
            ptr++;
        }
    }

    *str = len;
    memcpy(str+1, buf, len);

    return str;
}


//  ------------------------------------------------------------------
