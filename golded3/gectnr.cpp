
//  ------------------------------------------------------------------
//  GoldED+
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999-2000 Alexander S. Aganichev
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
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston,
//  MA 02111-1307 USA
//  ------------------------------------------------------------------
//  $Id$
//  ------------------------------------------------------------------
//  Container class.
//  ------------------------------------------------------------------

#include "golded.h"


//  ------------------------------------------------------------------

static bool strncont(const char *beginword, const char *stylestopchars, int n)
{
    for(; (n > 0) and (*beginword != NUL); n--, beginword++)
    {
        if(strchr(stylestopchars, *beginword) != NULL)
            return true;
    }
    return false;
}


//  ------------------------------------------------------------------

static bool in_ftn_domained_address(const char *ptr, const char *txt)
{

    while((ptr != txt) and (not isspace(*ptr) and not g_isalpha(*ptr)))
    {
        if(isdigit(ptr[0]) and ((ptr[1] == ':') or (ptr[1] == '/')) and isdigit(ptr[2]))
            return true;
        --ptr;
    }
    return false;
}


//  ------------------------------------------------------------------

const char *url_begin(const char *ptr)
{

    if(strnieql(ptr, "http://", 7))
        return ptr+7;
    if(strnieql(ptr, "https://", 8))
        return ptr+8;
    if(strnieql(ptr, "ftp://", 6))
        return ptr+6;
    if(strnieql(ptr, "www.", 4))
        return ptr+4;
    if(strnieql(ptr, "ftp.", 4))
        return ptr+4;
    if(strnieql(ptr, "mailto:", 7))
        return ptr+7;
    if(strnieql(ptr, "news:", 5))
        return ptr+5;
    if(strnieql(ptr, "ed2k://", 7))
        return ptr+7;
    return NULL;
}


//  ------------------------------------------------------------------

inline bool isstylechar(char c)
{

    if(strchr(CFG->stylecodestops, c) != NULL)
        return false;
    return (c == '*') or (c == '/') or (c == '_') or (c == '#');
}

void Container::StyleCodeHighlight(const char* text, int row, int col, bool dohide, vattr color)
{

    uint sclen = 0;
    const char* txptr = text;
    char buf[200];
    const char* ptr = text;
    const char* stylemargins = " -|\\";    // we probably have to make a keyword for it
    char* punctchars = CFG->stylecodepunct;
    char* stylestopchars = CFG->stylecodestops;
    char prevchar = ' ';
    bool usestylies = dohide or AA->adat->usestylies;

    if(usestylies or CFG->highlighturls)
    {
        while(*ptr)
        {
            if(usestylies and isstylechar(*ptr))
            {
                if(strchr(punctchars, prevchar))
                {
                    int bb = 0, bi = 0, bu = 0, br = 0;
                    const char* beginstyle = ptr;
                    while(isstylechar(*ptr))
                    {
                        switch(*ptr)
                        {
                        case '*':
                            bb++;
                            break;
                        case '/':
                            bi++;
                            break;
                        case '_':
                            bu++;
                            break;
                        case '#':
                            br++;
                            break;
                        }
                        ptr++;
                    }
                    if((bb <= 1) and (bi <= 1) and (br <= 1) and (bu <= 1) and *ptr)
                    {
                        const char* beginword = ptr;                 //  _/*>another*/_
                        const char* end = ptr;
                        do
                        {
                            end = strpbrk(++end, punctchars);
                        }
                        while ((end != NULL) and not isstylechar(*(end-1)));
                        if(end == NULL)
                            end = ptr+strlen(ptr);
                        const char* endstyle = end-1;                //  _/*another*/>_
                        if(isstylechar(*endstyle) and not strchr(stylemargins, *beginword))
                        {
                            const char* endword = endstyle;
                            int eb = 0, ei = 0, eu = 0, er = 0;
                            while(isstylechar(*endword))
                            {
                                switch(*endword)
                                {
                                case '*':
                                    eb++;
                                    break;
                                case '/':
                                    ei++;
                                    break;
                                case '_':
                                    eu++;
                                    break;
                                case '#':
                                    er++;
                                    break;
                                }
                                endword--;
                            }                                          //  _/*anothe>r*/_
                            if(endword >= beginword and not strchr(stylemargins, *endword))
                            {
                                if((bb == eb) and (bi == ei) and (bu == eu) and (br == er))
                                {
                                    bool style_stops_present = strncont(beginword, stylestopchars, endword-beginword);
                                    if(not style_stops_present)
                                    {
                                        int colorindex = (bb ? 1 : 0) | (bi ? 2 : 0) | (bu ? 4 : 0) | (br ? 8 : 0);
                                        strxcpy(buf, txptr, (uint)(beginstyle-txptr)+1);
                                        prints(row, col+sclen, color, buf);
                                        sclen += strlen(buf);
                                        if(dohide)
                                            strxcpy(buf, beginword, (uint)(endword-beginword)+2);
                                        else
                                            strxcpy(buf, beginstyle, (uint)(endstyle-beginstyle)+2);
                                        prints(row, col+sclen, C_STYLE[colorindex], buf);
                                        sclen += strlen(buf);
                                        txptr = end;
                                        ptr = end-1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else if(CFG->highlighturls)
            {
                const char *begin;

                if(((begin = url_begin(ptr)) != NULL) and
                        ((ptr == text) or (not isxalnum(ptr[-1]) and (ptr[-1] != '@'))))
                {
                    const char *end = begin+strcspn(begin, " \t\"\'<>()[]");

                    if(ispunct(end[-1]) and (end[-1] != '/'))
                        --end;
                    if(begin < end)
                    {
                        strxcpy(buf, txptr, (uint)(ptr-txptr)+1);
                        prints(row, col+sclen, color, buf);
                        sclen += strlen(buf);
                        strxcpy(buf, ptr, (uint)(end-ptr)+1);
                        prints(row, col+sclen, C_READU, buf);
                        sclen += strlen(buf);
                        txptr = end;
                        ptr = end-1;
                    }
                }
                else if(((ptr == text) or not in_ftn_domained_address(ptr, text)) and
                        (isascii(*ptr) and not isspace(*ptr) and not ispunct(*ptr) and
                         ((ptr == text) or (not isxalnum(ptr[-1]) and (ptr[-1] != '@')))))
                {
                    // try to guess e-mail address...
                    const char *commerce_at = NULL;

                    if(*ptr == '\"')
                    {
                        const char *pair_quote = strchr(ptr+1, '\"');
                        if(pair_quote != NULL)
                        {
                            commerce_at = pair_quote+1;
                        }
                    }
                    else
                    {
                        commerce_at = strpbrk(ptr, " \t:/@");
                    }
                    if((commerce_at != NULL) and (*commerce_at == '@'))
                    {
                        int dots_found = 0;
                        ++commerce_at;
                        while((*commerce_at != NUL) and (isalnum(*commerce_at) or (*commerce_at == '.') or (*commerce_at == '-')))
                        {
                            if(*commerce_at == '.')
                                ++dots_found;
                            ++commerce_at;
                        }
                        while((commerce_at[-1] == '.') or (commerce_at[-1] == '-'))
                        {
                            --commerce_at;
                            if(*commerce_at == '.')
                                --dots_found;
                        }
                        if(dots_found)
                        {
                            strxcpy(buf, txptr, (uint)(ptr-txptr)+1);
                            prints(row, col+sclen, color, buf);
                            sclen += strlen(buf);
                            strxcpy(buf, ptr, (uint)(commerce_at-ptr)+1);
                            prints(row, col+sclen, C_READU, buf);
                            sclen += strlen(buf);
                            txptr = commerce_at;
                            ptr = commerce_at-1;
                        }
                    }
                }
            }
            if(*ptr)
                prevchar = *ptr++;
        }
    }
    if(*txptr)
    {
        prints(row, col+sclen, color, txptr);
        sclen += strlen(txptr);
    }
    uint splen = strlen(text) - sclen;
    if(splen)
    {
        memset(buf, ' ', splen);
        buf[splen] = NUL;
        prints(row, col+sclen, color, buf);
    }
}


//  ------------------------------------------------------------------
