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
//  Read areas from SpaceToss 1.10
//  ------------------------------------------------------------------

#include <cstdlib>
#include <gcrcall.h>
#include <gstrall.h>
#include <gutlmisc.h>
#undef GCFG_NOSPCT
#include <gedacfg.h>


//  ------------------------------------------------------------------

void gareafile::ParseSpaceArea(const char *type_path, AreaCfg &aa) {

  if(strnieql(type_path, "msg", 3))
    aa.msgbase = fidomsgtype;
  else if(strnieql(type_path, "hud", 3))
    aa.msgbase = GMB_HUDSON;
  else if(strnieql(type_path, "jam", 3))
    aa.msgbase = GMB_JAM;
  else if(strnieql(type_path, "sqh", 3))
    aa.msgbase = GMB_SQUISH;
  else {
    aa.msgbase = 0;
    return;
  }
  aa.setpath(type_path+3);
}


//  ------------------------------------------------------------------

void gareafile::ReadSpaceAr(const char* file) {

  const word CRC_BEGINAREA = 0x78EE;
  const word CRC_NAME = 0x3B63;
  const word CRC_DESC = 0x8A2A;
  const word CRC_PATH = 0x0212;
  const word CRC_FLAGS = 0xF81A;
  const word CRC_GROUP = 0x1C9B;
  const word CRC_ENDAREA = 0x3E9F;

  FILE* fp = fsopen(file, "rb", sharemode);
  if(fp) {
    setvbuf(fp, NULL, _IOFBF, 8192);

    if(not quiet)
      cout << "* Reading " << file << endl;

    char buf[4000];
    AreaCfg aa;

    aa.reset();

    while(fgets(buf, 4000, fp) != NULL) {

      char* key;
      char* val = buf;
      switch(getkeyvalcrc(&key, &val)) {
        case CRC_BEGINAREA:
          aa.type = GMB_ECHO;
          aa.attr = attribsecho;
          aa.aka = primary_aka;
          break;
        case CRC_NAME:
          aa.setechoid(val);
          break;
        case CRC_DESC:
          aa.setdesc(val);
          break;
        case CRC_PATH:
          ParseSpaceArea(val, aa);
          break;
        case CRC_FLAGS:
          if(strpbrk(val,"Ll")) {
            aa.type = GMB_LOCAL;
            aa.attr = attribslocal;
          }
          break;
        case CRC_GROUP:
          aa.groupid = toupper(*val);
          break;
        case CRC_ENDAREA:
          if(aa.msgbase)
            AddNewArea(aa);
          aa.reset();
          break;
      }
    }

    fclose(fp);
  }
}


//  ------------------------------------------------------------------

void gareafile::ReadSpaceNtm(const char* file) {

  const word CRC_BEGINNETMAIL = 0x4DF0;
  const word CRC_NAME = 0x3B63;
  const word CRC_ADDRESS = 0xFDD6;
  const word CRC_PATH = 0x0212;
  const word CRC_AUTOEXPORT = 0xE0E1;
  const word CRC_ENDNETMAIL = 0xD995;

  FILE* fp = fsopen(file, "rb", sharemode);
  if(fp) {
    setvbuf(fp, NULL, _IOFBF, 8192);

    if(not quiet)
      cout << "* Reading " << file << endl;

    char buf[4000];
    AreaCfg aa;
    bool exportarea = false;

    aa.reset();

    while(fgets(buf, 4000, fp) != NULL) {

      char* key;
      char* val = buf;
      switch(getkeyvalcrc(&key, &val)) {
        case CRC_BEGINNETMAIL:
          aa.setautoid("NETMAIL");
          aa.type = GMB_NET;
          aa.attr = attribsnet;
          aa.aka = primary_aka;
          exportarea = false;
          break;
        case CRC_ADDRESS:
          aa.aka.set(val);
        case CRC_NAME: {
          char echodesc[256];
          strxmerge(echodesc, 256, "Netmail for ", val, NULL);
          aa.setdesc(echodesc);
          break;
        }
        case CRC_PATH:
          ParseSpaceArea(val, aa);
          break;
        case CRC_AUTOEXPORT:
          exportarea = GetYesno(val) ? true : false;
          break;
        case CRC_ENDNETMAIL:
          if(exportarea and aa.msgbase)
            AddNewArea(aa);
          aa.reset();
          break;
      }
    }

    fclose(fp);
  }
}


//  ------------------------------------------------------------------

void gareafile::ReadSpaceCtl(const char* file) {

  const word CRC_ADDRESS = 0xFDD6;
  const word CRC_AKA = 0x13A4;
  const word CRC_NETMAIL = 0xE42E;
  const word CRC_BADMAIL = 0xE697;
  const word CRC_DUPEMAIL = 0xB38B;

  FILE* fp = fsopen(file, "rb", sharemode);
  if(fp) {
    setvbuf(fp, NULL, _IOFBF, 8192);

    if(not quiet)
      cout << "* Reading " << file << endl;

    char buf[4000];
    AreaCfg aa;

    aa.reset();

    while(fgets(buf, 4000, fp) != NULL) {

      aa.type = 0xFF;

      char* key;
      char* val = buf;
      switch(getkeyvalcrc(&key, &val)) {
        case CRC_ADDRESS:
        case CRC_AKA:
          CfgAddress(buf);
          break;
        case CRC_NETMAIL:
          aa.setautoid("NETMAIL");
          aa.setdesc("SpaceToss Netmail");
          aa.type = GMB_NET;
          aa.attr = attribsnet;
          break;
        case CRC_BADMAIL:
          aa.setautoid("BADMAIL");
          aa.setdesc("SpaceToss Badmail");
          aa.type = GMB_LOCAL;
          aa.attr = attribslocal;
          break;
        case CRC_DUPEMAIL:
          aa.setautoid("DUPEMAIL");
          aa.setdesc("SpaceToss Dupemail");
          aa.type = GMB_LOCAL;
          aa.attr = attribslocal;
          break;
      }

      if(aa.type != 0xFF) {

        aa.aka = primary_aka;

        // Get type/path
        ParseSpaceArea(val, aa);

        if(aa.msgbase) {
          AddNewArea(aa);
        }
      }

      aa.reset();
    }

    fclose(fp);
  }
}


//  ------------------------------------------------------------------
//  Read areas from SpaceToss (echomail processor)

void gareafile::ReadSpaceToss(char* tag) {

  Path file, path;
  char options[80];

  strcpy(options, tag);
  char* ptr = strtok(tag, " \t");
  while(ptr) {
    if(*ptr != '-') {
      strcpy(file, ptr);
    }
    ptr = strtok(NULL, " \t");
  }

  extractdirname(path, file);

  CfgSquishuserpath(path);

  MakePathname(file, path, "spctoss.ctl");
  ReadSpaceCtl(file);
  MakePathname(file, path, "spctoss.ntm");
  ReadSpaceNtm(file);
  MakePathname(file, path, "spctoss.ar");
  ReadSpaceAr(file);
}


//  ------------------------------------------------------------------
