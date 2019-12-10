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
//  RFC822 related character/string classes and functions.
//  ------------------------------------------------------------------

#include <gstrall.h>
#include <gstrmail.h>
//  ------------------------------------------------------------------
mail_ctype mail_ctype_global;
//  ------------------------------------------------------------------
mail_ctype::mail_ctype()
{
    memset(table, mail_char | mail_ctl, 32 * sizeof(uint));
    memset(table + 32, mail_char, 96 * sizeof(uint));
    memset(table + 'A', mail_char | mail_alpha, ('Z' - 'A' + 1) * sizeof(uint));
    memset(table + 'a', mail_char | mail_alpha, ('z' - 'a' + 1) * sizeof(uint));
    memset(table + 128, 0, 128 * sizeof(uint));
    table[127 & 0xff]  |= mail_ctl;
    table[0 & 0xff]    |= mail_delimiters;
    table[' ' & 0xff]  |= mail_delimiters | mail_lwsp;
    table[HT & 0xff]   |= mail_delimiters | mail_lwsp;
    table['(' & 0xff]  |= mail_delimiters | mail_special | mime_tspecial | mime_especial;
    table[')' & 0xff]  |= mail_delimiters | mail_special | mime_tspecial | mime_especial;
    table['<' & 0xff]  |= mail_delimiters | mail_special | mime_tspecial | mime_especial;
    table['>' & 0xff]  |= mail_delimiters | mail_special | mime_tspecial | mime_especial;
    table['@' & 0xff]  |= mail_delimiters | mail_special | mime_tspecial | mime_especial;
    table[',' & 0xff]  |= mail_delimiters | mail_special | mime_tspecial | mime_especial;
    table[';' & 0xff]  |= mail_delimiters | mail_special | mime_tspecial | mime_especial;
    table[':' & 0xff]  |= mail_delimiters | mail_special | mime_tspecial | mime_especial;
    table['\\' & 0xff] |= mail_delimiters | mail_special | mime_tspecial | mime_especial;
    table['\"' & 0xff] |= mail_delimiters | mail_special | mime_tspecial | mime_especial;
    table['[' & 0xff]  |= mail_delimiters | mail_special | mime_tspecial | mime_especial;
    table[']' & 0xff]  |= mail_delimiters | mail_special | mime_tspecial | mime_especial;
    table['.' & 0xff]  |= mail_delimiters | mail_special | mime_especial;
    table['/' & 0xff]  |= mime_tspecial | mime_especial;
    table['?' & 0xff]  |= mime_tspecial | mime_especial;
    table['=' & 0xff]  |= mime_tspecial | mime_especial;
}

//  ------------------------------------------------------------------
const char * strskip_lwsp(const char * p)
{
    while(is_mail_lwsp(*p))
    {
        p++;
    }
    return p;
}

//  ------------------------------------------------------------------
const char * strskip_mail_quote(const char * p)
{
    if(*p == '\"')
    {
        p++;
    }

    while(*p and (*p != '\"'))
    {
        if(*p == '\\')
        {
            p++;
        }

        p++;
    }

    if(*p == '\"')
    {
        p++;
    }

    return p;
}

//  ------------------------------------------------------------------
const char * strskip_delimiters(const char * p)
{
    while(is_mail_delimiters(*p))
    {
        p++;
    }
    return p;
}

//  ------------------------------------------------------------------
char * strtrim_lwsp(char * p)
{
    char * p0 = p;

    p += strlen(p) - 1;

    while(is_mail_lwsp(*p))
    {
        if(p == p0)
        {
            break;
        }

        p--;
    }
    return p;
}

//  ------------------------------------------------------------------
char * strmail_quote(char * ot, const char * it)
{
    char * o       = ot;
    const char * i = it;

    if(*o == '\"')
    {
        o++;
    }

    while(*i)
    {
        if(is_mail_special(*i))
        {
            *o++ = '\\';
        }

        *o++ = *i++;
    }

    if(*ot == '\"')
    {
        *o++ = '\"';
    }

    *o = NUL;
    return ot;
} // strmail_quote

//  ------------------------------------------------------------------
char * strmail_unquote(char * ot, const char * it)
{
    char * o        = ot;
    const char * i  = it;
    bool was_quoted = false;

    if(*i == '\"')
    {
        was_quoted = true;
        i++;
    }

    while(*i)
    {
        if(*i == '\\')
        {
            i++;
        }

        *o++ = *i++;
    }

    if(was_quoted)
    {
        o--;
    }

    *o = NUL;
    return ot;
} // strmail_unquote

//  ------------------------------------------------------------------
int is_mail_lwsp(const char * s)
{
    while(is_mail_lwsp(*s))
    {
        s++;
    }
    return *s == NUL;
}

//  ------------------------------------------------------------------
