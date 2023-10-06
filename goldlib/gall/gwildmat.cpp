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

#include <gstrall.h>
#include <gwildmat.h>

#if defined(__USE_ALLOCA__)
    #include <malloc.h>
#endif


//  ------------------------------------------------------------------

/*
**  Do shell-style pattern matching for ?, \, [], and * characters.
**  Might not be robust in face of malformed patterns; e.g., "foo[a-"
**  could cause a segmentation violation.  It is 8bit clean.
**
**  Written by Rich $alz, mirror!rs, Wed Nov 26 19:03:17 EST 1986.
**  Rich $alz is now <rsalz@osf.org>.
**  April, 1991:  Replaced mutually-recursive calls with in-line code
**  for the star character.
**
**  Special thanks to Lars Mathiesen <thorinn@diku.dk> for the ABORT code.
**  This can greatly speed up failing wildcard patterns.  For example:
**  pattern: -*-*-*-*-*-*-12-*-*-*-m-*-*-*
**  text 1:  -adobe-courier-bold-o-normal--12-120-75-75-m-70-iso8859-1
**  text 2:  -adobe-courier-bold-o-normal--12-120-75-75-X-70-iso8859-1
**  Text 1 matches with 51 calls, while text 2 fails with 54 calls.  Without
**  the ABORT code, it takes 22310 calls to fail.  Ugh.  The following
**  explanation is from Lars:
**  The precondition that must be fulfilled is that DoMatch will consume
**  at least one character in text.  This is true if *p is neither '*' nor
**  '\0'.)  The last return has ABORT instead of FALSE to avoid quadratic
**  behaviour in cases like pattern "*a*b*c*d" with text "abcxxxxx".  With
**  FALSE, each star-loop has to run to the end of the text; with ABORT
**  only the last one does.
**
**  Once the control of one instance of DoMatch enters the star-loop, that
**  instance will return either TRUE or ABORT, and any calling instance
**  will therefore return immediately after (without calling recursively
**  again).  In effect, only one star-loop is ever active.  It would be
**  possible to modify the code to maintain this context explicitly,
**  eliminating all recursive calls at the cost of some complication and
**  loss of clarity (and the ABORT stuff seems to be unclear enough by
**  itself).  I think it would be unwise to try to get this into a
**  released version unless you have a good test data base to try it out
**  on.
*/


//  ------------------------------------------------------------------

int gwildmatch::match_internal(const char* text, const char* pattern, bool ignorecase)
{

    int last;
    int matched;
    int reverse;
    const char* p = pattern;

    for( ; *p; text++, p++)
    {
        if(*text == NUL and *p != '*')
            return -1;
        switch (*p)
        {
        case '?':
            // Match anything
            continue;
        case '*':
            while(*++p == '*')    // Consecutive stars act just like one.
                continue;
            if(*p == NUL)         // Trailing star matches everything
                return true;
            while(*text)
            {
                if((matched = match_internal(text++, p, ignorecase)) != false)
                    return matched;
            }
            return -1;
        case '[':
            reverse = (p[1] == '^');
            if(reverse)           // Inverted character class
                p++;
            matched = false;
            if(p[1] == ']' or p[1] == '-')
            {
                if(ignorecase)
                {
                    if(g_tolower(*++p) == g_tolower(*text))
                        matched = true;
                }
                else
                {
                    if(*++p == *text)
                        matched = true;
                }
            }
            for(last = *p; *++p and *p != ']'; last = *p)
            {
                // This next line requires a good C compiler
                if(ignorecase)
                {
                    if(*p == '-' and p[1] != ']' ? g_tolower(*text) <= g_tolower(*++p) and g_tolower(*text) >= g_tolower(last) : g_tolower(*text) == g_tolower(*p))
                        matched = true;
                }
                else
                {
                    if(*p == '-' and p[1] != ']' ? *text <= *++p and *text >= last : *text == *p)
                        matched = true;
                }
            }
            if(matched == reverse)
                return false;
            continue;
        case '\\':
            // Literal match with following character
            p++;
        // FALLTHROUGH
        default:
            if(ignorecase)
            {
                if(g_tolower(*text) != g_tolower(*p))
                    return false;
            }
            else
            {
                if(*text != *p)
                    return false;
            }
            continue;
        }
    }

    return *text == NUL;
}


//  ------------------------------------------------------------------

bool gwildmatch::match(const char* text, const char* pattern, bool ignorecase)
{

    if(*pattern == '*' and pattern[1] == NUL)
        return true;
    return match_internal(text, pattern, ignorecase) == true;
}


//  ------------------------------------------------------------------
//  4DOS-style wildcard string match.

bool strwild(const char* str, const char* wild)
{

    while(*str)
    {
        if(*wild == '*')                          // Match all
        {
            if(wild[1] == NUL)
                return true;
            else
            {
#if defined(__USE_ALLOCA__)
                char *buf = (char *)alloca(strlen(wild));
#else
                __extension__ char buf[strlen(wild)];
#endif
                strcpy(buf, wild+1);
                char* ptr = strpbrk(buf, "*?");
                if(ptr)
                    *ptr = NUL;
                ptr = (char*)striinc(buf, str);
                if(ptr)
                {
                    str = ptr + strlen(buf);
                    wild += strlen(buf) + 1;
                }
                else
                    break;
            }
        }
        else if(g_toupper(*str) == g_toupper(*wild) or *wild == '?')
        {
            wild++;
            str++;
        }
        else
            break;
    }
    if((*str == NUL) and (*wild == NUL or (*wild == '*' and wild[1] == NUL)))
        return true;

    return false;
}


//  ------------------------------------------------------------------
