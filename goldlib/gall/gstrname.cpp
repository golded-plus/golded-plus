//  ------------------------------------------------------------------
//  The Goldware Library. Copyright (C) Odinn Sorensen.
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
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//  ------------------------------------------------------------------
//  $Id$
//  ------------------------------------------------------------------
//  Name handling.
//  ------------------------------------------------------------------


#include <gstrall.h>
//  ------------------------------------------------------------------
//  Reverse a name to "lastname, firstname(s)" form.
char * strrevname(char * reversedname, const char * name)
{
    char tmpname[256];
    char midname[256];

    strtrim(strcpy(tmpname, name));
    char * c = midname;                    // Start of temp name buffer
    char * p = tmpname;                    // Point to start of name
    char * m = NULL;                       // Init pointer to space

    *c = *p++;

    while(*c)                              // Go entire length of name
    {
        if(*c == ' ')                      // Look for space
        {
            m = c;                         // Save location
        }

        c++;
        *c = *p++;
    }

    if(m != NULL)
    {
        *m++ = NUL;
        strxmerge(reversedname, 256, m, ", ", midname, NULL);
    }
    else
    {
        strcpy(reversedname, midname);
    }

    return reversedname;
} // strrevname

//  ------------------------------------------------------------------
//  Unreverse a name from "lastname, firstname(s)" form.
char * strunrevname(char * unreversedname, const char * name)
{
    char tmpname[256];

    strcpy(tmpname, name);
    char * comma = strchr(tmpname, ',');

    if(comma)
    {
        *comma = NUL;
        char * firstname = strskip_wht(comma + 1);
        strxmerge(unreversedname, 256, firstname, " ", tmpname, NULL);
        *comma = ',';
    }
    else
    {
        strcpy(unreversedname, tmpname);
    }

    return unreversedname;
}

//  ------------------------------------------------------------------
