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
#include <gstrall.h>
#include <gmemdbg.h>
#include <gutlos.h>
#include <gctype.h>
#include <windows.h>
#ifndef __NO_MMSYSTEM
#include <mmsystem.h>
#endif


//  ------------------------------------------------------------------

OSVERSIONINFO WinVer;
char          ge_win_coldtitle[GMAXTITLE] = "";
char          ge_win_title[GMAXTITLE] = "";
int           ge_win_ext_title;

char          tu[256] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
  0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
  0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
  0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
  0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
  0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
  0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
  0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
  0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
  0x60, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
  0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
  0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
  0x58, 0x59, 0x5a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
  0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
  0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
  0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
  0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
  0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
  0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
  0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
  0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
  0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
  0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
  0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
  0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
  0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
  0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
  0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
  0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff
};

char          tl[256] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
  0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
  0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
  0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
  0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
  0x40, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
  0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
  0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
  0x78, 0x79, 0x7a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
  0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
  0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
  0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
  0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
  0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
  0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
  0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
  0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
  0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
  0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
  0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
  0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
  0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
  0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
  0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
  0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
  0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
  0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
  0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
  0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff
};

WCHAR         oem2unicode[256] = {
//  MB_USEGLYPHCHARS should do the same :-)
//  0x0000, 0x263a, 0x263b, 0x2665, 0x2666, 0x2663, 0x2660, 0x2219,
//  0x25d8, 0x25cb, 0x25d9, 0x2642, 0x2640, 0x266a, 0x266b, 0x263c,
//  0x25ba, 0x25c4, 0x2195, 0x203c, 0x00b6, 0x00a7, 0x25a0, 0x21a8,
//  0x2191, 0x2193, 0x2192, 0x2190, 0x221f, 0x2194, 0x25b2, 0x25bc
};

//  ------------------------------------------------------------------

int g_init_os(int flags) {

  NW(flags);

  int i;
  memset(&WinVer, 0, sizeof(OSVERSIONINFO));
  WinVer.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
  GetVersionEx(&WinVer);
  SetFileApisToOEM();
  GetConsoleTitle(ge_win_coldtitle, sizeof(ge_win_coldtitle));
  if(WinVer.dwPlatformId == VER_PLATFORM_WIN32_NT) {
#ifdef __MSVCRT__
    char locale[256] = "";
    char *lc = setlocale(LC_CTYPE, "");
    if(lc) {
      strxcpy(locale, lc, 256);
      lc = strchr(locale, '.');
      if(lc) {
        sprintf(lc+1, "%u", GetOEMCP());
      }
    }
    setlocale(LC_CTYPE, locale);
#endif
    for(i = 0; i < 256; i++) {
      tu[i] = (toupper)(i);
      tl[i] = (tolower)(i);
      CHAR chr = (CHAR)i;
      MultiByteToWideChar(CP_OEMCP, MB_USEGLYPHCHARS, &chr, 1, oem2unicode+i, 1);
    }
    return 0;
  }
#ifdef __MSVCRT__
  else
    setlocale(LC_CTYPE, "C");
#endif
  // Due to Win9x doesn't have proper locale support we should rebuild
  // tolower/toupper tables
  char src[2], dst[2];
  for(i = 0; i < 128; i++) {
    tu[i] = (toupper)(i);
    tl[i] = (tolower)(i);
  }
  for(i = 128; i < 256; i++) {
    *dst = i; dst[1] = 0;
    OemToChar(dst, src);
    LCMapString(LOCALE_SYSTEM_DEFAULT, LCMAP_UPPERCASE, src, 1, dst, 2);
    CharToOem(dst, src);
    tu[i] = (*src & 0xff);
    *dst = i; dst[1] = 0;
    OemToChar(dst, src);
    LCMapString(LOCALE_SYSTEM_DEFAULT, LCMAP_LOWERCASE, src, 1, dst, 2);
    CharToOem(dst, src);
    tl[i] = (*src & 0xff);
  }
  for(i = 128; i < 256; i++) {
    if((tu[tl[i]] != i) && (tl[tu[i]] != i))
      tu[i] = tl[i] = i;
    if(tu[tl[i]] != tu[i])
      tu[i] = i;
    if(tl[tu[i]] != tl[i])
      tl[i] = i;
  }
  return 0;
}


//  ------------------------------------------------------------------

void g_deinit_os(void) {

  SetConsoleTitle(ge_win_coldtitle);
}


//  ------------------------------------------------------------------

void g_init_title(char* tasktitle, int titlestatus) {

  strncpy(ge_win_title, tasktitle, GMAXTITLE);
  ge_win_title[GMAXTITLE-1] = '\0';
  ge_win_ext_title = titlestatus;
}


//  ------------------------------------------------------------------

void g_increase_priority(void) {

  SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
}


//  ------------------------------------------------------------------

void g_set_ostitle(char *title) {

  SetConsoleTitle(title);
}


//  ------------------------------------------------------------------

void g_set_osicon(void) {

  // do nothing
}


//  ------------------------------------------------------------------

bool g_is_clip_available(void) {

  return true;
}


//  ------------------------------------------------------------------

char* g_get_clip_text(void) {

  HANDLE hClipData;
  if(not OpenClipboard(NULL))
    return NULL;
  int Unicode = false;
  int Format = 0;
  int ReadType = CF_OEMTEXT;
  while((Format = EnumClipboardFormats(Format)) != 0) {
    if((Format == CF_UNICODETEXT) and (WinVer.dwPlatformId == VER_PLATFORM_WIN32_NT)) {
      Unicode = true;
      break;
    }
    if(Format == CF_TEXT) {
      ReadType = CF_TEXT;
      break;
    }
    if(Format == CF_OEMTEXT)
      break;
  }
  char *ClipText = NULL;
  if((hClipData = GetClipboardData(Unicode ? CF_UNICODETEXT : ReadType)) != NULL) {
    int BufferSize;
    char *ClipAddr = (char *)GlobalLock(hClipData);
    if(Unicode)
      BufferSize = lstrlenW((LPCWSTR)ClipAddr) + 1;
    else
      BufferSize = strlen(ClipAddr) + 1;
    ClipText = (char *) throw_malloc(BufferSize);
    if(ClipText != NULL)
      if(Unicode)
        WideCharToMultiByte(CP_OEMCP, 0, (LPCWSTR)ClipAddr, -1, ClipText, BufferSize, NULL, NULL);
      else
        if(ReadType == CF_TEXT)
          CharToOem(ClipAddr, ClipText);
        else
          strcpy(ClipText, ClipAddr);
    GlobalUnlock(hClipData);
  }
  CloseClipboard();
  return ClipText;
}


//  ------------------------------------------------------------------

int g_put_clip_text(const char *Data) {

  long DataSize;
  if((Data != NULL) and ((DataSize = strlen(Data)) != 0)) {
    HGLOBAL hData;
    void *GData;
    if(not OpenClipboard(NULL))
      return -1;
    EmptyClipboard();
    int BufferSize = DataSize + 1;
    if((hData=GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, BufferSize)) != NULL)
      if((GData = GlobalLock(hData)) != NULL) {
        memcpy(GData, Data, DataSize + 1);
        GlobalUnlock(hData);
        SetClipboardData(CF_OEMTEXT, (HANDLE)hData);
      }
    if((hData=GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, BufferSize)) != NULL)
      if((GData=GlobalLock(hData)) != NULL) {
        memcpy(GData, Data, DataSize + 1);
        OemToChar((LPCSTR)GData, (LPTSTR)GData);
        GlobalUnlock(hData);
        SetClipboardData(CF_TEXT, (HANDLE)hData);
      }
    if(WinVer.dwPlatformId == VER_PLATFORM_WIN32_NT)
      if((hData = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, BufferSize * 2)) != NULL)
        if((GData = GlobalLock(hData)) != NULL) {
          WCHAR *UData = (WCHAR *)GData;

          while(*Data) {
            if((*Data == '\r') or (*Data == '\n') or (*Data == '\t'))
              *UData++ = *Data++; // no translation for real control chars
            else
              *UData++ = oem2unicode[*Data++];
          }
          *UData = 0;
          GlobalUnlock(hData);
          SetClipboardData(CF_UNICODETEXT, (HANDLE)hData);
        }
    CloseClipboard();
  }
  return 0;
}


//  ------------------------------------------------------------------

void g_get_ostitle_name(char *title) {

  GetConsoleTitle(title, GMAXTITLE);
}


//  ------------------------------------------------------------------

void g_set_ostitle_name(char *title, int mode) {

  if(mode == 0) {
    char fulltitle[GMAXTITLE];
    strcpy(fulltitle, ge_win_title);
    if(ge_win_ext_title) {
      int len = strlen(fulltitle);
      if(len < GMAXTITLE-4) {
        if(len)
          strcat(fulltitle, " - ");
        strncpy(fulltitle+len+3, title, GMAXTITLE-len-3);
        fulltitle[GMAXTITLE-1] = '\0';
      }
    }
    g_set_ostitle(fulltitle);
  }
  else
    g_set_ostitle(title);
}


//  ------------------------------------------------------------------
