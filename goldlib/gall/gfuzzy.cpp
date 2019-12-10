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
//
//  C++ port and cleanup by Odinn Sorensen, August 1992.
//  Dusted off and re-used, March 1994.
//  Converted to C++ class, December 1997.
//
//  Original source: APPROX.C (found in SNIP1091).
//  Original author: John Rex, August 1988.
//
//  References: (1) Computer Algorithms, by Sara Baase Addison-Wesley,
//                  1988, pp 242-4.
//              (2) Hall PAV, Dowling GR: "Approximate string match-
//                  ing", ACM Computing Surveys, 12:381-402, 1980.
//
//  Usage:
//
//    pattern, string - Search for pattern in text
//    degree          - Degree of allowed mismatch (no of chars)
//
//    init(pattern, degree, casing) - Setup routine
//    findfirst(string)             - Find first match
//    findnext()                    - Find next match
//
//  Searching is finished when findfirst/next() returns false
//
//  ------------------------------------------------------------------

#include <gctype.h>
#include <gstrall.h>
#include <gmemdbg.h>
#include <gfuzzy.h>
//  ------------------------------------------------------------------
gfuzzy::gfuzzy()
{
    ldiffs = NULL;
}

//  ------------------------------------------------------------------
gfuzzy::~gfuzzy()
{
    throw_deletearray(ldiffs);
}

//  ------------------------------------------------------------------
//  Fuzzy search init
void gfuzzy::init(const char * pat, int fuzzydegree, bool case_sensitive)
{
    casing  = case_sensitive;
    degree  = fuzzydegree;
    pattern = pat;
    plen    = strlen(pattern);
    ldiffs  = new int[(plen + 1) * 4];
    throw_new(ldiffs);
}

//  ------------------------------------------------------------------
bool gfuzzy::findfirst(const char * string)
{
    textloc = -1;
    text    = string;
    start   = text;
    ldiff   = ldiffs;
    rdiff   = ldiff + plen + 1;
    loffs   = rdiff + plen + 1;
    roffs   = loffs + plen + 1;

    for(int i = 0; i <= plen; i++)
    {
        rdiff[i] = i; // Initial values for right-hand column
        roffs[i] = 1;
    }
    return findnext();
}

//  ------------------------------------------------------------------
//  Fuzzy search next
bool gfuzzy::findnext()
{
    if(start)
    {
        start    = NULL;
        howclose = -1;

        while(start == NULL)     // Start computing columns
        {
            if(text[++textloc] == NUL) // Out of text to search!
            {
                break;
            }

            int * temp = rdiff; // Move right-hand column to left ...
            rdiff    = ldiff; // ... so that we can compute new ...
            ldiff    = temp;  // ... right-hand column
            rdiff[0] = 0;     // Top (boundary) row
            temp     = roffs; // And swap offset arrays, too
            roffs    = loffs;
            loffs    = temp;
            roffs[1] = 0;

            for(int i = 0; i < plen; i++) // Run through pattern
            {
                // Compute a, b, & c as the three adjacent cells ...
                bool charmatch;

                if(casing)
                {
                    charmatch = pattern[i] == text[textloc];
                }
                else
                {
                    charmatch = g_toupper(pattern[i]) == g_toupper(text[textloc]);
                }

                int a = ldiff[i] + (charmatch ? 0 : 1);
                int b = ldiff[i + 1] + 1;
                int c = rdiff[i] + 1;

                // ... now pick minimum ...
                if(b < a)
                {
                    a = b;
                }

                if(c < a)
                {
                    a = c;
                }

                // ... and store
                rdiff[i + 1] = a;
            }

            // Now update offset array
            // The values in the offset arrays are added to the
            // current location to determine the beginning of the
            // mismatched substring. (See refs for details)
            if(plen > 1)
            {
                for(int i = 2; i <= plen; i++)
                {
                    if(ldiff[i - 1] < rdiff[i])
                    {
                        roffs[i] = loffs[i - 1] - 1;
                    }
                    else if(rdiff[i - 1] < rdiff[i])
                    {
                        roffs[i] = roffs[i - 1];
                    }
                    else if(ldiff[i] < rdiff[i])
                    {
                        roffs[i] = loffs[i] - 1;
                    }
                    else // Then we have ldiff[i-1] == rdiff[i]
                    {
                        roffs[i] = loffs[i - 1] - 1;
                    }
                }
            }

            // Now, do we have an approximate match?
            if(rdiff[plen] <= degree) // indeed so!
            {
                end      = text + textloc;
                start    = end + roffs[plen];
                howclose = rdiff[plen];
            }
        }
    }

    return make_bool(start);
} // gfuzzy::findnext

//  ------------------------------------------------------------------
