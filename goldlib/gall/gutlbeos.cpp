//  This may look like C code, but it is really -*- C++ -*-
//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1999 Alexander S. Aganichev
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
//  Midnight Commander compatible clipboard.
//  ------------------------------------------------------------------

#include <cstdlib>
#include <cstdio>
#include <gstrall.h>
#include <gmemdbg.h>
#include <gutlos.h>
#include <gmemi86.h>
#include <gfilutil.h>

#include <Clipboard.h>
#include <UTF8.h>
//  ------------------------------------------------------------------
char ge_beos_title[GMAXTITLE + 1] = "";
int ge_beos_ext_title;
static BClipboard g_clipboard("system", true);
//  ------------------------------------------------------------------
int g_init_os(int flags)
{
    NW(flags);
    return 0;
}

//  ------------------------------------------------------------------
void g_deinit_os(void)
{
    // do nothing
}

//  ------------------------------------------------------------------
void g_init_title(char * tasktitle, int titlestatus)
{
    strxcpy(ge_beos_title, tasktitle, GMAXTITLE);
    ge_beos_ext_title = titlestatus;
}

//  ------------------------------------------------------------------
void g_increase_priority(void)
{
    // Do nothing
}

//  ------------------------------------------------------------------
void g_set_ostitle(char * title)
{
    printf("\x1b\x5d\x32\x3b%s\x07", title);
    fflush(stdout);
}

//  ------------------------------------------------------------------
void g_set_osicon(void)
{
    // do nothing
}

//  ------------------------------------------------------------------
bool g_is_clip_available(void)
{
    return true;
}

//  ------------------------------------------------------------------
char * g_get_clip_text(void)
{
    char * clip_text  = NUL;
    const char * text = NUL;
    int32 textLen;
    BMessage * clip = (BMessage *)NUL;

    if(g_clipboard.Lock())
    {
        if((clip = g_clipboard.Data()) != NUL)
        {
            if(B_OK ==
               clip->FindData("text/plain", B_MIME_TYPE, (const void **)&text, &textLen))
            {
                clip_text  = (char *)throw_malloc(textLen * 2 + 1);
                *clip_text = NUL;
                int32 state = 0, destLen = textLen * 2;
                convert_from_utf8(B_ISO5_CONVERSION,
                                  text,
                                  &textLen,
                                  clip_text,
                                  &destLen,
                                  &state);
                clip_text[destLen] = NUL;
            }
        }

        g_clipboard.Unlock();
    }

    return clip_text;
}

//  ------------------------------------------------------------------
int g_put_clip_text(const char * buf)
{
    int ret         = -1;
    BMessage * clip = (BMessage *)NULL;

    if(g_clipboard.Lock())
    {
        g_clipboard.Clear();

        if((clip = g_clipboard.Data()) != NUL)
        {
            int32 textLen    = strlen(buf);
            int32 destLen    = textLen * 3;
            char * clip_text = (char *)throw_malloc(destLen);
            *clip_text = NUL;
            int32 state = 0;
            convert_to_utf8(B_ISO5_CONVERSION, buf, &textLen, clip_text, &destLen,
                            &state);
            clip->AddData("text/plain", B_MIME_TYPE, clip_text, destLen);
            g_clipboard.Commit();
            ret = 0;
        }

        g_clipboard.Unlock();
    }

    return -1;
} // g_put_clip_text

//  ------------------------------------------------------------------
void g_get_ostitle_name(char * title)
{
    *title = NUL;
}

//  ------------------------------------------------------------------
void g_set_ostitle_name(char * title, int mode)
{
    if(mode == 0)
    {
        char fulltitle[GMAXTITLE + 1];
        strcpy(fulltitle, ge_beos_title);

        if(ge_beos_ext_title)
        {
            int len = strlen(fulltitle);

            if(len < GMAXTITLE - 3)
            {
                if(len)
                {
                    strcat(fulltitle, " - ");
                }

                strxcpy(fulltitle + len + 3, title, GMAXTITLE - len - 3);
            }
        }

        g_set_ostitle(fulltitle);
    }
    else
    {
        g_set_ostitle(title);
    }
} // g_set_ostitle_name

//  ------------------------------------------------------------------
int g_send_mci_string(char * str, char * his_buffer)
{
    NW(str);
    NW(his_buffer);
    return 1;
}

//  ------------------------------------------------------------------
