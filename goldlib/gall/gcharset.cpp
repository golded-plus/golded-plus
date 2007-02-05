//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
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
//  Utility functions.
//  ------------------------------------------------------------------

#include <cstdlib>
#include <cstdio>
#include <clocale>
#include <stdlib.h>
#include <gcmpall.h>
#include <gstrall.h>
#ifdef __WIN32__
#include <windows.h>
#endif
#ifdef __OS2__
#define INCL_DOS
#include <os2.h>
#endif
#ifdef __DJGPP__
#include <dpmi.h>
#include <sys/farptr.h>
#endif
#include <gcharset.h>

static char charsetbuf[256];

const char *get_charset(void)
{
#if defined(__DJGPP__)
  int segment, selector;
  __dpmi_regs regs;

  strcpy(charsetbuf, GOLDED_DEFAULT_CHARSET);
  if ((segment = __dpmi_allocate_dos_memory(3, &selector)) != -1) {
    regs.h.ah = 0x65;
    regs.h.al = 0x01;
    regs.x.bx = 0xffff;
    regs.x.dx = 0xffff;
    regs.x.cx = 41;
    regs.x.es = segment;
    regs.x.di = 0;
    __dpmi_int(0x21, &regs);
    if (!(regs.x.flags & 1) and (regs.x.cx == 41)) {
      int CCP = _farpeekw(selector, 5);
      sprintf(charsetbuf, "CP%i", CCP);
    }
    __dpmi_free_dos_memory(selector);
  }
#elif defined(__WIN32__)
  sprintf(charsetbuf, "CP%i", GetOEMCP());
#elif defined(__OS2__)
  ULONG CCP[8];
  ULONG cb;

  strcpy(charsetbuf, GOLDED_DEFAULT_CHARSET);
  if(DosQueryCp(sizeof (CCP), CCP, &cb) == 0)
    sprintf(charsetbuf, "CP%i", CCP[0]);
#else
  const char *cp;

  strcpy(charsetbuf, GOLDED_DEFAULT_CHARSET);
  cp = setlocale(LC_CTYPE, "");
  if((cp != NULL) and ((cp = strchr(cp, '.')) != NULL)) {
    cp++;
/* Commented on reason: Non-standard charset values isn't used in unix locale, may be in very old implementations?
// Check this.
    if(strieql(cp, "KOI8R") or strieql(cp, "KOI8"))
      cp = "KOI8-R";
    if(strieql(cp, "KOI8U"))
      cp = "KOI8-U";
*/
    strxcpy(charsetbuf, cp, sizeof(charsetbuf));
  }
#endif
  return charsetbuf;
}

const char *get_dos_charset(const char *cpfrom)
{
#if defined(__WIN32__) || defined(__MSDOS__) || defined(__OS2__)
  (void)cpfrom; // These platforms use DOS CP on console, so ignore request
                // Another way is possible in Windows: get charset using GetANSICP()
  return "";
#else
  static const struct _cpmap {
    char *from, *to;
  } cpmap[] = {
    { "IBMPC", "CP850" },
    { "LATIN-1", "CP437" },
    { "KOI8", "CP866" },
    { "KOI8-R", "CP866" },
    { "KOI8-U", "CP1125" },
    { NULL, NULL }
  };

  int i;
  for(i = 0; cpmap[i].from != NULL; i++) {
    if(strieql(cpfrom, cpmap[i].from))
      return cpmap[i].to;
  }
 #if defined(__unix__)
  char* lang = getenv("LANG");
  if( lang && strncmp(lang,"ru_RU",5) ) {
    return "CP866";
  }else{
    return "CP437";
  }
 #endif
  return "";
#endif
}
