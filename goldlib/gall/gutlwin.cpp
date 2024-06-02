//  This may look like C code, but it is really -*- C++ -*-
//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1999-2000 Alexander S. Aganichev
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
//  Various Windows functions.
//  ------------------------------------------------------------------
//  Clipboard handling donated by Eugene Roshal
//  ------------------------------------------------------------------

#include <clocale>
#include <cstdio>
#include <cctype>
#define __gctype_h // prevent gctype from being included
#include <gstrall.h>
#include <gmemdbg.h>
#include <gutlos.h>
#include <windows.h>
#ifndef __NO_MMSYSTEM
    #include <mmsystem.h>
#endif
//  ------------------------------------------------------------------
OSVERSIONINFO WinVer;
char ge_win_coldtitle[GMAXTITLE + 1] = "";
char ge_win_title[GMAXTITLE + 1]     = "";
int ge_win_ext_title;

#if defined (_MSC_VER)
extern "C" {
#endif
char tu[256];
char tl[256];
#if defined (_MSC_VER)
}
#endif

WCHAR oem2unicode[256];
//  ------------------------------------------------------------------
int g_init_os(int flags)
{
    NW(flags);
    int i;
    memset(&WinVer, 0, sizeof(OSVERSIONINFO));
    WinVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&WinVer);
    SetFileApisToOEM();
    GetConsoleTitle(ge_win_coldtitle, sizeof(ge_win_coldtitle));

    if(WinVer.dwPlatformId == VER_PLATFORM_WIN32_NT)
    {
#ifdef __MSVCRT__
        char locale[256] = "";
        char * lc        = setlocale(LC_CTYPE, "");

        if(lc)
        {
            strxcpy(locale, lc, 256);
            lc = strchr(locale, '.');

            if(lc)
            {
                sprintf(lc + 1, "%u", GetOEMCP());
            }
        }

        setlocale(LC_CTYPE, locale);
#endif

        for(i = 0; i < 256; i++)
        {
            tu[i] = toupper(i);
            tl[i] = tolower(i);
            CHAR chr = (CHAR)i;
            MultiByteToWideChar(CP_OEMCP, MB_USEGLYPHCHARS, &chr, 1, oem2unicode + i, 1);
        }
        return 0;
    }

#ifdef __MSVCRT__
    else
    {
        setlocale(LC_CTYPE, "C");
    }
#endif
    // Due to Win9x doesn't have proper locale support we should rebuild
    // tolower/toupper tables
    char src[2], dst[2], tst[2];

    for(i = 0; i < 32; i++)
    {
        tu[i] = toupper(i);
        tl[i] = tolower(i);
    }

    for(i = 32; i < 256; i++)
    {
        tu[i]  = tl[i] = i;
        *dst   = i;
        dst[1] = 0;
        OemToChar(dst, src);
        CharToOem(src, tst);

        if(*dst != *tst)
        {
            continue;
        }

        if(LCMapString(LOCALE_SYSTEM_DEFAULT, LCMAP_UPPERCASE, src, 1, dst, 2))
        {
            CharToOem(dst, src);
            tu[i] = *src;
        }

        *dst   = i;
        dst[1] = 0;
        OemToChar(dst, src);

        if(LCMapString(LOCALE_SYSTEM_DEFAULT, LCMAP_LOWERCASE, src, 1, dst, 2))
        {
            CharToOem(dst, src);
            tl[i] = *src;
        }
    }
    return 0;
} // g_init_os

//  ------------------------------------------------------------------
void g_deinit_os(void)
{
    SetConsoleTitle(ge_win_coldtitle);
}

//  ------------------------------------------------------------------
void g_init_title(char * tasktitle, int titlestatus)
{
    strxcpy(ge_win_title, tasktitle, GMAXTITLE);
    ge_win_ext_title = titlestatus;
}

//  ------------------------------------------------------------------
void g_increase_priority(void)
{
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
}

//  ------------------------------------------------------------------
void g_set_ostitle(char * title)
{
    SetConsoleTitle(title);
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
    HANDLE hClipData;

    if(not OpenClipboard(NULL))
    {
        return NULL;
    }

    int Unicode  = false;
    int Format   = 0;
    int ReadType = CF_OEMTEXT;

    while((Format = EnumClipboardFormats(Format)) != 0)
    {
        if((Format == CF_UNICODETEXT) and (WinVer.dwPlatformId == VER_PLATFORM_WIN32_NT))
        {
            Unicode = true;
            break;
        }

        if(Format == CF_TEXT)
        {
            ReadType = CF_TEXT;
            break;
        }

        if(Format == CF_OEMTEXT)
        {
            break;
        }
    }
    char * ClipText = NULL;

    if((hClipData = GetClipboardData(Unicode ? CF_UNICODETEXT : ReadType)) != NULL)
    {
        int BufferSize;
        char * ClipAddr = (char *)GlobalLock(hClipData);

        if(Unicode)
        {
            BufferSize = lstrlenW((LPCWSTR)ClipAddr) + 1;
        }
        else
        {
            BufferSize = strlen(ClipAddr) + 1;
        }

        ClipText = (char *)throw_malloc(BufferSize);

        if(ClipText != NULL)
        {
            if(Unicode)
            {
                WideCharToMultiByte(CP_OEMCP,
                                    0,
                                    (LPCWSTR)ClipAddr,
                                    -1,
                                    ClipText,
                                    BufferSize,
                                    NULL,
                                    NULL);
            }
            else if(ReadType == CF_TEXT)
            {
                CharToOem(ClipAddr, ClipText);
            }
            else
            {
                strcpy(ClipText, ClipAddr);
            }
        }

        GlobalUnlock(hClipData);
    }

    CloseClipboard();
    return ClipText;
} // g_get_clip_text

//  ------------------------------------------------------------------
int g_put_clip_text(const char * Data)
{
    long DataSize;

    if((Data != NULL) and ((DataSize = strlen(Data)) != 0))
    {
        HGLOBAL hData;
        void * GData;

        if(not OpenClipboard(NULL))
        {
            return -1;
        }

        EmptyClipboard();
        int BufferSize = DataSize + 1;

        if((hData = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, BufferSize)) != NULL)
        {
            if((GData = GlobalLock(hData)) != NULL)
            {
                memcpy(GData, Data, DataSize + 1);
                GlobalUnlock(hData);
                SetClipboardData(CF_OEMTEXT, (HANDLE)hData);
            }
        }

        if((hData = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, BufferSize)) != NULL)
        {
            if((GData = GlobalLock(hData)) != NULL)
            {
                memcpy(GData, Data, DataSize + 1);
                OemToChar((LPCSTR)GData, (LPTSTR)GData);
                GlobalUnlock(hData);
                SetClipboardData(CF_TEXT, (HANDLE)hData);
            }
        }

        if(WinVer.dwPlatformId == VER_PLATFORM_WIN32_NT)
        {
            if((hData = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE,
                                    BufferSize * 2)) != NULL)
            {
                if((GData = GlobalLock(hData)) != NULL)
                {
                    WCHAR * UData = (WCHAR *)GData;

                    while(*Data)
                    {
                        if((*Data == '\r') or (*Data == '\n') or (*Data == '\t'))
                        {
                            *UData++ = *Data++; // no translation for real control chars
                        }
                        else
                        {
                            *UData++ = oem2unicode[*Data++ & 0xff];
                        }
                    }
                    *UData = 0;
                    GlobalUnlock(hData);
                    SetClipboardData(CF_UNICODETEXT, (HANDLE)hData);
                }
            }
        }

        CloseClipboard();
    }

    return 0;
} // g_put_clip_text

//  ------------------------------------------------------------------
void g_get_ostitle_name(char * title)
{
    GetConsoleTitle(title, GMAXTITLE);
}

//  ------------------------------------------------------------------
void g_set_ostitle_name(char * title, int mode)
{
    if(mode == 0)
    {
        char fulltitle[GMAXTITLE + 1];
        strcpy(fulltitle, ge_win_title);

        if(ge_win_ext_title)
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
