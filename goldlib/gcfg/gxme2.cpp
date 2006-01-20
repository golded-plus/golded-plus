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
//  Read areas from ME2
//  ------------------------------------------------------------------

#include <gstrall.h>
#if defined(__GOLD_GUI__)
#include <gvidall.h>
#include <gvidgui.h>
#endif
#undef GCFG_NOME2
#include <gedacfg.h>


//  ------------------------------------------------------------------
//  Read AREADESC.ME2 and AREAS.BBS

void gareafile::ReadME2(char* tag) {

  AreaCfg aa;
  FILE* fp;
  Path file;
  char* ptr;
  char* ptr2;
  char* desc;
  char* group;
  char* echoid;
  char buf[256], options[80];

  if(*tag) {
    ptr2 = strskip_wht(tag);
    if(*ptr2) {
      ptr = ptr2;
      ptr2 = strskip_txt(ptr2);
      if(*ptr2) *ptr2++ = NUL;
      strcpy(options, ptr2);
      strcpy(file, ptr);

      MakePathname(file, areapath, file);

      fp = fsopen(file, "rt", sharemode);
      if (fp)
      {
        setvbuf(fp, NULL, _IOFBF, 8192);

        if (not quiet)
          STD_PRINTNL("* Reading " << file);

        while(fgets(buf, 255, fp)) {

          ptr = strbtrim(buf);
          if(*ptr and *ptr != '*' and *ptr != ';') {
            if(strnieql(ptr, "LOCAL", 5) and (ptr[5] == ' ' or ptr[5] == HT)) {

              // Found local area - add it
              aa.reset();
              aa.type = GMB_LOCAL;
              aa.attr = attribslocal;
              aa.basetype = fidomsgtype;
              ptr = strskip_wht(ptr+5);
              ptr2 = strskip_to(++ptr, '\'');
              if(*ptr2) *ptr2++ = NUL;
              aa.setdesc(ptr);
              ptr = strskip_wht(ptr2);
              ptr2 = strskip_to(++ptr2, '\'');
              if(*ptr2) *ptr2 = NUL;
              aa.setpath(ptr);
              AddNewArea(aa);
            }
            else {
              ptr2 = strskip_txt(ptr);
              if(*ptr2) *ptr2++ = NUL;
              echoid = ptr;
              ptr2 = strskip_wht(ptr2);
              ptr = ++ptr2;
              ptr2 = strskip_to(ptr2, '\'');
              if(*ptr2) *ptr2++ = NUL;
              desc = ptr;
              ptr = strskip_wht(ptr2);
              ptr2 = strskip_to(++ptr, '\'');
              if(*ptr2) *ptr2 = NUL;
              group = ptr;
              sprintf(file, "%-33s%-5s", desc, group);
              echolist.AddDesc(echoid, file);
            }
          }
        }
        ReadAreasBBS(options);
      }
    }
  }
}


//  ------------------------------------------------------------------
