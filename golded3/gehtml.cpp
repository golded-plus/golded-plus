//  ------------------------------------------------------------------
//  GoldED+
//  Copyright (C) 2003 Alexander S. Aganichev
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
//  HTML tag remover.
//  ------------------------------------------------------------------

#include <golded.h>
//  ------------------------------------------------------------------
const static struct html_entities
{
    const char * tag;
    char         replacement;
} entities[] = {{"nbsp",  ' '                 }, {"brvbar", '|'}, {"laquo", '<'        },
    {"shy",   '-'                 },
    {"raquo", '>'                 }, {"divide", '/'}, {"quot",  '\"'       },{"amp", '&'}, {"lt", '<'},
    {"gt",    '>'                 }};
//  ------------------------------------------------------------------
void RemoveHTML(char *& txt)
{
    long i, j, len = strlen(txt) + 1;
    char * new_txt           = (char *)throw_malloc(len);
    bool strip               = false;
    bool quoted              = false;
    bool inside_html         = false;
    bool last_char_was_space = true;

    for(i = j = 0; txt[i] != NUL; i++)
    {
        if(not quoted and not strip and (txt[i] == '<'))
        {
            if(strnieql(txt + i, "<html", 5) or strnieql(txt + i, "<!DOCTYPE",
                                                         9) or strnieql(txt + i, "<!--",
                                                                        4))
            {
                inside_html = true;
                strip       = true;
            }
            else if(strnieql(txt + i, "</html>", 7))
            {
                inside_html = false;
                strip       = true;
            }
            else if(not inside_html and (txt[i + 1] == '/'))
            {
                inside_html = true; // closing html tag, force html mode
                strip       = true;
            }
            else if(inside_html)
            {
                strip = true;

                if(strnieql(txt + i, "<b>", 3) or strnieql(txt + i, "</b>", 4))
                {
                    new_txt[j++] = '*';
                }

                if(strnieql(txt + i, "<i>", 3) or strnieql(txt + i, "</i>", 4))
                {
                    new_txt[j++] = '/';
                }

                if(strnieql(txt + i, "<u>", 3) or strnieql(txt + i, "</u>", 4))
                {
                    new_txt[j++] = '_';
                }

                if((strnieql(txt + i, "</h",
                             3) and isdigit(txt[i + 3])) or strnieql(txt + i, "</p>",
                                                                     4) or strnieql(txt +
                                                                                    i,
                                                                                    "</tr>",
                                                                                    5) or
                   strnieql(txt + i, "</div>", 6) or strnieql(txt + i, "<br>", 4))
                {
                    new_txt[j++] = CR;
                }
            }
            else
            {
                new_txt[j++] = txt[i];
            }
        }
        else if(not strip and not inside_html)
        {
            new_txt[j++] = txt[i];
        }
        else if(strip and not quoted and (txt[i] == '>'))
        {
            strip = false;
        }
        else if(inside_html)
        {
            if(strip and (txt[1] == '\"'))
            {
                quoted = not quoted;
            }
            else if(not strip and (iscntrl(txt[i]) or (txt[i] == ' ')))
            {
                if((i > 0) && (txt[i - 1] == '=')) // compensate for quoted-printable
                {
                    new_txt[j++] = txt[i];
                }
                else if(not last_char_was_space)
                {
                    new_txt[j++] = ' ';
                }

                last_char_was_space = true;
            }
            else if(not strip and (txt[i] == '&'))
            {
                bool found = false;

                for(int k = 0; k < (sizeof(entities) / sizeof(html_entities)); k++)
                {
                    long taglen = strlen(entities[k].tag);

                    if(strnieql(txt + i + 1, entities[k].tag, taglen))
                    {
                        new_txt[j++] = entities[k].replacement;
                        i           += taglen + ((txt[i + taglen + 1] == ';') ? 1 : 0);
                        found        = true;
                        break;
                    }
                }

                if(not found)
                {
                    new_txt[j++] = txt[i];
                }

                last_char_was_space = false;
            }
            else if(not strip)
            {
                new_txt[j++]        = txt[i];
                last_char_was_space = false;
            }
        }
    }
    new_txt[j] = NUL;

    if(i != j)
    {
        txt = (char *)throw_realloc(txt, j + 17);
        memcpy(txt, new_txt, j + 1);
    }

    throw_free(new_txt);
} // RemoveHTML

//  ------------------------------------------------------------------
