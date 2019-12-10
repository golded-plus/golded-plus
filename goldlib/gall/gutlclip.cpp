//  This may look like C code, but it is really -*- C++ -*-
//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
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
//  Misc. clipboards support.
//  ------------------------------------------------------------------

#include <gstrall.h>
#include <gutlmisc.h>
#include <gutlclip.h>
#include <gmemdbg.h>
#include <gutlos.h>
//  ------------------------------------------------------------------
int clipboard_available = -1;
char * fake_clipboard   = NULL;
//  ------------------------------------------------------------------
void fake_clipboard_destroy()
{
    if(fake_clipboard)
    {
        throw_free(fake_clipboard);
    }
}

//  ------------------------------------------------------------------
gclipbrd::gclipbrd()
{
    len      = -1;
    clipdata = NULL;
    cliphdl  = NULL;
  #if defined (GUTLOS_FUNCS)

    if(clipboard_available == -1)
    {
        clipboard_available = g_is_clip_available() ? 1 : 0;
    }

  #else
    clipboard_available = 0;
  #endif

    if(not clipboard_available and not fake_clipboard)
    {
        fake_clipboard = throw_strdup("");
        atexit(fake_clipboard_destroy);
    }
}

//  ------------------------------------------------------------------
bool gclipbrd::openread()
{
    if(not clipboard_available)
    {
        cliphdl = clipdata = fake_clipboard;

        if(fake_clipboard != NULL)
        {
            len = strlen(cliphdl);
            return true;
        }
        else
        {
            return false;
        }
    }

  #if defined (GUTLOS_FUNCS)
    cliphdl = clipdata = g_get_clip_text();

    if(clipdata != NULL)
    {
        len = strlen(clipdata);
        return true;
    }

  #endif

    return false;
} // gclipbrd::openread

//  ------------------------------------------------------------------
bool gclipbrd::writeclipbrd(const char * buf)
{
    if(not clipboard_available)
    {
        if(fake_clipboard)
        {
            throw_free(fake_clipboard);
        }

        fake_clipboard = throw_strdup(buf);
        return fake_clipboard != NULL;
    }

  #if defined (GUTLOS_FUNCS)
    return g_put_clip_text(buf) == 0;

  #else
    return false;

  #endif
}

//  ------------------------------------------------------------------
char * gclipbrd::read(char * buffer, int maxlen)
{
    if(len > 0)
    {
        int i    = MinV(len, maxlen - 1);
        char * p = strpbrk(clipdata, "\r\n");

        if(p)
        {
            if(p - clipdata < i)
            {
                i = p - clipdata;

                if((len > i) and strchr("\r\n", *(p + 1)) and (*p != *(p + 1)))
                {
                    ++i;
                }
            }
            else
            {
                p = NULL;
            }
        }
        else
        {
            p = NULL;
        }

        strxcpy(buffer, clipdata, i + 1);
        char * p2 = strpbrk(buffer, "\r\n");

        if(p2)
        {
            *p2 = 0;
        }

        if(p)
        {
            strcat(buffer, "\n");
            ++i;
        }

        len      -= MinV(len, i);
        clipdata += i;
        return buffer;
    }

    return NULL;
} // gclipbrd::read

//  ------------------------------------------------------------------
void gclipbrd::close()
{
    if(not clipboard_available)
    {
        return;
    }

  #if defined (GUTLOS_FUNCS)

    if(len >= 0)
    {
        if(cliphdl != NULL)
        {
            throw_free(cliphdl);
        }

        len     = -1;
        cliphdl = clipdata = NULL;
    }

  #endif
}

//  ------------------------------------------------------------------
