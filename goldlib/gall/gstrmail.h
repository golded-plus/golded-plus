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

#ifndef __gstrmail_h
#define __gstrmail_h
//  ------------------------------------------------------------------

#include <gdefs.h>
//  ------------------------------------------------------------------
class mail_ctype
{
public: enum
    {
        mail_char    = 0x01, mail_alpha = 0x02, mail_ctl = 0x04, mail_lwsp = 0x08,
        mail_special = 0x10, mail_delimiters = 0x20, mime_tspecial = 0x40,  // RFC1521 parameter
                                                                            // token specials
        mime_especial = 0x80 // RFC1522 encoded-word token specials
    };

    uint table[256];
    mail_ctype();
};

extern mail_ctype mail_ctype_global;
//  ------------------------------------------------------------------
inline bool is_mail_char(uint c)
{
    return make_bool(mail_ctype_global.table[c] & mail_ctype::mail_char);
}

inline bool is_mail_alpha(uint c)
{
    return make_bool(mail_ctype_global.table[c] & mail_ctype::mail_alpha);
}

inline bool is_mail_ctl(uint c)
{
    return make_bool(mail_ctype_global.table[c] & mail_ctype::mail_ctl);
}

inline bool is_mail_cr(uint c)
{
    return c == CR;
}

inline bool is_mail_lf(uint c)
{
    return c == LF;
}

inline bool is_mail_space(uint c)
{
    return c == ' ';
}

inline bool is_mail_htab(uint c)
{
    return c == HT;
}

inline bool is_mail_crlf(uint c)
{
    return is_mail_cr(c) or is_mail_lf(c);
}

inline bool is_mail_lwsp(uint c)
{
    return make_bool(mail_ctype_global.table[c] & mail_ctype::mail_lwsp);
}

inline bool is_mail_special(uint c)
{
    return make_bool(mail_ctype_global.table[c] & mail_ctype::mail_special);
}

inline bool is_mail_delimiters(uint c)
{
    return make_bool(mail_ctype_global.table[c] & mail_ctype::mail_delimiters);
}

inline bool is_mime_tspecial(uint c)
{
    return make_bool(mail_ctype_global.table[c] & mail_ctype::mime_tspecial);
}

inline bool is_mime_especial(uint c)
{
    return make_bool(mail_ctype_global.table[c] & mail_ctype::mime_especial);
}

inline bool is_mail_atom_delimiters(uint c)
{
    return is_mail_delimiters(c) or is_mail_ctl(c);
}

inline bool is_mime_ct_token_valid(uint c)
{
    return not (is_mail_lwsp(c) or is_mail_ctl(c) or is_mime_tspecial(c));
}

//  ------------------------------------------------------------------
const char * strskip_lwsp(const char * p);
const char * strskip_mail_quote(const char * p);
const char * strskip_delimiters(const char * p);
char * strtrim_lwsp(char * p);
char * strmail_quote(char * ot, const char * it);
char * strmail_unquote(char * ot, const char * it);
char * strmail_unquote_qtext(char * ot, const char * it);
int is_mail_lwsp(const char * s);

//  ------------------------------------------------------------------

#endif // ifndef __gstrmail_h
//  ------------------------------------------------------------------
