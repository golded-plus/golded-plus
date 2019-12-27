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
//  String upper/lower and NLS functions.
//  ------------------------------------------------------------------

#include <gstrall.h>
#include <gutlmisc.h>


//  ------------------------------------------------------------------
#ifndef g_islower
int g_islower(int c)
{
    return isascii(c) ? islower(c) : (c == g_tolower(c)) && (c != g_toupper(c));
}
#endif

#ifndef g_isupper
int g_isupper(int c)
{
    return isascii(c) ? isupper(c) : (c != g_tolower(c)) && (c == g_toupper(c));
}
#endif

#ifndef g_isalpha
int g_isalpha(int c)
{
    return isascii(c) ? isalpha(c) : (c != g_tolower(c)) || (c != g_toupper(c));
}
#endif

int isxalnum(int c)
{
    return isascii(c) ? isalnum(c) : (c != g_tolower(c)) || (c != g_toupper(c));
}

//  ------------------------------------------------------------------
//  Converts a character to upper or lower case depending on the
//  previous character in the string

static char touplow(char* str, char* pos, char ch)
{

    char i;

    if(pos == str)
        return g_toupper(ch);

    switch(*(pos-1))                    // check previous character
    {
    case ' ':                         // see if it is a separator
    case '-':
    case '_':
    case ',':
    case '.':
    case '/':
        i = (char)g_toupper(ch);        // if it is, convert to upper
        break;
    default:
        i = (char)g_tolower(ch);        // otherwise, convert to lower
        break;
    }

    return i;                           // return converted character
}


//  ------------------------------------------------------------------
//  Converts a string to mixed upper & lower case

char* struplow(char* str)
{

    int i;

    for(i=0; *(str+i); i++)
        *(str+i) = touplow(str, str+i, *(str+i));

    return str;
}


//  ------------------------------------------------------------------

char* strcvtc(char* str)
{

    char c;
    char* s = str;
    char* d = s;
    while(*s)
    {
        if(*s == '\\')
        {
            s++;
            switch(*s)
            {
            case 'n':
                *d++ = '\n';
                goto skip_constant;
            case 't':
                *d++ = '\t';
                goto skip_constant;
            case 'v':
                *d++ = '\v';
                goto skip_constant;
            case 'b':
                *d++ = '\b';
                goto skip_constant;
            case 'r':
                *d++ = '\r';
                goto skip_constant;
            case 'f':
                *d++ = '\f';
                goto skip_constant;
            case 'a':
                *d++ = '\a';
                goto skip_constant;
            case '\\':
                *d++ = '\\';
                goto skip_constant;
            case '\'':
                *d++ = '\'';
                goto skip_constant;
            case '\"':
                *d++ = '\"';
                goto skip_constant;
            case '\?':
                *d++ = '\?';
                goto skip_constant;
            case 'x':
                if(isxdigit(s[1]))
                {
                    s++;
                    c = 0;
                    while(isxdigit(*s))
                    {
                        c <<= 4;
                        c |= (char)xtoi(*s);
                        s++;
                    }
                    *d++ = c;
                    continue;
                }
                break;
            default:
                if(isoctal(*s))
                {
                    c = 0;
                    while(isoctal(*s))
                    {
                        c <<= 3;
                        c |= (char)(*s - '0');
                        s++;
                    }
                    *d++ = c;
                }
                else
                {
                    s++;
                    continue;
                }
            }
            *d++ = *s++;
            continue;

skip_constant:
            s++;
            continue;
        }
        *d++ = *s++;
    }
    *d = NUL;
    return str;
}


//  ------------------------------------------------------------------
