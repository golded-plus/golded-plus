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
//  Read areas from RA-ECHO (1.00 - 1.xx)
//  ------------------------------------------------------------------

#include <cstdlib>
#include <gstrall.h>
#undef GCFG_NORAECHO
#include <gedacfg.h>
#include <gs_recho.h>


//  ------------------------------------------------------------------
//  Read Ra-Echo AREAS.RAE

void gareafile::ReadRaEcho(char* tag) {

  AreaCfg aa;
  FILE* fp;
  char* ptr;
  long raesize;
  char options[80];
  Path repath, file;
  int raever, areano;
  TRaEchoArea101 area;

  *repath = NUL;
  strcpy(options, tag);
  ptr = strtok(tag, " \t");
  while(ptr) {
    if(*ptr != '-') {
      AddBackslash(strcpy(repath, ptr));
      break;
    }
    ptr = strtok(NULL, " \t");
  }
  if(*repath == NUL) {
    ptr = getenv("RAECHO");
    if(ptr)
      AddBackslash(strcpy(repath, ptr));
  }
  if(*repath == NUL)
    strcpy(repath, areapath);

  MakePathname(file, repath, "areas.rae");

  raesize = GetFilesize(file);
  raever = 0;
  if(raesize == -1) {
    // Determine RA-ECHO version...
    if((raesize%(long)sizeof(TRaEchoArea100)) == 0)
      raever = sizeof(TRaEchoArea100);
    else if((raesize%(long)sizeof(TRaEchoArea101)) == 0)
      raever = sizeof(TRaEchoArea101);
  }
  if(raever == 0) {
    if(not quiet)
      cout << "* Could not determine version of RA-ECHO - skipping." << endl;
    return;
  }

  fp = fsopen(file, "rb", sharemode);
  if(fp) {

    setvbuf(fp, NULL, _IOFBF, 8192);

    if(not quiet)
      cout << "* Reading " << file << endl;

    areano = 1;
    while(fread(&area, raever, 1, fp) == 1) {

      if(*area.echoid) {
        aa.reset();
        STRNP2C(area.desc);
        STRNP2C(area.path);
        STRNP2C(area.echoid);
        STRNP2C(area.origin);
        if(area.isopus and *area.path and stricmp(area.path, "P")) {
          aa.msgbase = fidomsgtype;
          aa.setpath(area.path);
        }
        else if(areano <= 200) {
          aa.msgbase = GMB_HUDSON;
          aa.board = areano;
        }
        if(aa.msgbase) {
          aa.aka = area.addr;
          switch(area.type) {
            case RAE_LOCAL:
              aa.type = GMB_LOCAL;
              aa.attr = attribslocal;
              break;
            case RAE_NET:
              aa.type = GMB_NET;
              aa.attr = attribsnet;
              break;
            case RAE_ECHO:
            default:
              aa.type = GMB_ECHO;
              aa.attr = attribsecho;
          }
          aa.setdesc(area.desc);
          aa.setechoid(area.echoid);
          aa.setorigin(area.origin);
          AddNewArea(aa);
        }
      }

      areano++;
    }

    fclose(fp);
  }
}


//  ------------------------------------------------------------------
