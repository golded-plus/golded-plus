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
//  Read areas from Squish
//  ------------------------------------------------------------------

#include <cstdlib>
#include <gcrcall.h>
#include <gstrall.h>
#include <gedacfg.h>


//  ------------------------------------------------------------------

void gareafile::ReadSquishFile(char* path, char* file, char* options, char* origin, int group) {

  const word CRC_ADDRESS = 0xFDD6;
  const word CRC_ORIGIN = 0x4CE5;
  const word CRC_INCLUDE = 0x379B;
  const word CRC_AREASBBS = 0xF77C;
  const word CRC_NETAREA = 0x8F1C;
  const word CRC_ECHOAREA = 0x0D63;
  const word CRC_LOCALAREA = 0xAEC1;
  const word CRC_DUPEAREA = 0xD8B9;
  const word CRC_BADAREA = 0x8DA5;

  AreaCfg aa;
  char buf[512];
  Path buf2;

  FILE* fp = fsopen(file, "rb", sharemode);
  if(fp) {
    setvbuf(fp, NULL, _IOFBF, 8192);

    if(not quiet)
      cout << "* Reading " << file << endl;

    aa.reset();

    while(fgets(buf, sizeof(buf), fp)) {

      char* ptr = strskip_wht(strtrim(buf));
      if(*ptr != ';' and *ptr) {

        aa.type = 0xFF;

        char* key;
        char* val = ptr;
        switch(getkeyvalcrc(&key, &val)) {
          case CRC_ADDRESS:
            CfgAddress(val);
            break;
          case CRC_ORIGIN:
            strcpy(origin, val);
            break;
          case CRC_INCLUDE:
            strxcpy(buf2, val, sizeof(buf2));
            MakePathname(buf2, path, buf2);
            ReadSquishFile(path, buf2, options, origin, group);
            break;
          case CRC_AREASBBS:
            strcpy(buf2, val);
            MakePathname(buf2, path, buf2);
            if(striinc("/NOCHK", options) or striinc("-NOCHK", options))
              strcat(buf2, " /NOCHK");
            ReadAreasBBS(buf2);
            break;
          case CRC_NETAREA:
            aa.type = GMB_NET;
            break;
          case CRC_ECHOAREA:
          case CRC_DUPEAREA:
          case CRC_BADAREA:
            aa.type = GMB_ECHO;
            break;
          case CRC_LOCALAREA:
            aa.type = GMB_LOCAL;
            break;
        }

        if(aa.type != 0xFF) {

          // Get echoid
          getkeyval(&key, &val);
          aa.setechoid(key);

          // Get path
          getkeyval(&key, &val);
          strxcpy(buf2, key, sizeof(buf2));
          aa.setpath(MapPath(buf2));

          // If not pass-through
          if(not striinc("-0", val)) {

            aa.msgbase = fidomsgtype;
            aa.groupid = group;
            char* p = val;

            while(*p) {
              if(strnieql(p, "-$", 2)) {
                aa.msgbase = GMB_SQUISH;
                p += 2;
                if((tolower(*p) == 'g') and isalpha(p[1]))
                  aa.groupid = toupper(p[1]);
                else if(tolower(*p) == 'n') {
                  key = ++p;
                  getkeyval(&key, &p);
                  aa.setdesc(key);
                  continue;
                }
              }
              else if(strnieql(p, "-f", 2)) {
                aa.msgbase = fidomsgtype;
                p += 2;
              }
              else if(strnieql(p, "-p", 2)) {
                aa.aka = primary_aka;
                aa.aka.set(p+2);
              }
              p = strskip_wht(strskip_txt(p));
            }
                                
            switch(aa.type) {
              case GMB_LOCAL:
                aa.attr = attribslocal;
                break;
              case GMB_NET:
                aa.attr = attribsnet;
                break;
              case GMB_ECHO:
                aa.attr = attribsecho;
                break;
            }
            aa.setorigin(origin);
            AddNewArea(aa);
          }

          aa.reset();
        }
      }
    }

    fclose(fp);
  }
}


//  ------------------------------------------------------------------
//  Read areas from Squish (echomail processor)

void gareafile::ReadSquish(char* tag) {

  char origin[80];
  char options[80];
  word defaultgroup = 0;
  Path file, path;

  *origin = NUL;
  *file = NUL;
  strcpy(options, tag);
  char* ptr = strtok(tag, " \t");
  while(ptr) {
    if(*ptr != '-') {
      strcpy(file, ptr);
      if(is_dir(file)) {
        AddBackslash(file);
        strcat(file, "squish.cfg");
      }
    }
    else {
      if(toupper(*(++ptr)) == 'G') {
        if(*(++ptr) == '=')
          ptr++;
        if(*ptr == '#')
          defaultgroup = (word)(atoi(ptr+1)+0x8000u);
        else
          defaultgroup = (word)(isupper(*ptr) ? *ptr : 0);
      }
    }
    ptr = strtok(NULL, " \t");
  }

  if(not *file and ((ptr = getenv("SQUISH"))!=NULL)) {
    strcpy(file, ptr);
    if(is_dir(file)) {
      AddBackslash(file);
      strcat(file, "squish.cfg");
    }
  }

  extractdirname(path, file);

  if(*squishuserpath == NUL)
    PathCopy(squishuserpath, path);

  ReadSquishFile(path, file, options, origin, defaultgroup);
}


//  ------------------------------------------------------------------
