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
//  Read areas from AdeptXBBS
//  ------------------------------------------------------------------

#include <cstdlib>
#include <gcrcall.h>
#include <gstrall.h>
#undef GCFG_NOXBBS
#include <gedacfg.h>
#include <gs_xbbs.h>


//  ------------------------------------------------------------------

void gareafile::ReadAdeptXbbsFile(char* path, char* file, char* options) {

  const word CRC_ADDRESS   = 0xFDD6;
  const word CRC_AREABEGIN = 0x44D7;
  const word CRC_AREAEND   = 0xBDEF;
  const word CRC_DESC      = 0x8A2A;
  const word CRC_FLAGS     = 0xF81A;
  const word CRC_NAME      = 0x3B63;
  const word CRC_NUMBER    = 0x2FBC;
  const word CRC_ORIGIN    = 0x4CE5;
  const word CRC_PATH      = 0x0212;
  const word CRC_USENET    = 0xD087;

  AreaCfg aa;
  ulong flags;
  char buf[512];
  char name[256];
  char usenet[256];
  Path apath;

  FILE* fp = fsopen(file, "rb", sharemode);
  if(fp) {
    setvbuf(fp, NULL, _IOFBF, 8192);

    if(not quiet)
      std::cout << "* Reading " << file << std::endl;

    aa.reset();

    while(fgets(buf, sizeof(buf), fp)) {

      char* ptr = strskip_wht(strtrim(buf));
      if(*ptr != ';' and *ptr) {

        char* key;
        char* val = ptr;
        switch(getkeyvalcrc(&key, &val)) {
          case CRC_AREABEGIN:
            aa.reset();
            sprintf(apath, "%sMessage_Bases\\", path);
            *usenet = NUL;
            break;
          case CRC_AREAEND:
            aa.setechoid(*usenet ? usenet : name);
            if(streql(aa.basetype, "OPUS") or streql(aa.basetype, "FTS1"))
              aa.setpath(apath);
            else {
              sprintf(buf, "%s%s", apath, name);
              aa.setpath(buf);
            }
            AddNewArea(aa);
            aa.reset();
            break;
          case CRC_ADDRESS:
            aa.aka.set(val);
            break;
          case CRC_DESC:
            aa.setdesc(val);
            break;
          case CRC_FLAGS:
            flags = atol(val);
            if(flags & (M_NET | M_EMAIL)) {
              if(flags & M_EMAIL) {
                aa.type = GMB_NET|GMB_EMAIL;
                aa.attr = attribsemail;
              }
              else {
                aa.type = GMB_NET;
                aa.attr = attribsnet;
              }
            }
            else if(flags & (M_ECHO | M_GROUP | M_USENET)) {
              if(flags & M_USENET) {
                aa.type = GMB_ECHO|GMB_NEWSGROUP;
                aa.attr = attribsnews;
              }
              else {
                aa.type = GMB_ECHO;
                aa.attr = attribsecho;
              }
            }
            else {
              aa.type = GMB_LOCAL;
              aa.attr = attribslocal;
            }
            if(flags & M_SQUISH)
              aa.basetype = "SQUISH";
            else if(flags & M_FIDO)
              aa.basetype = "OPUS";
            else if(flags & M_JAM)
              aa.basetype = "JAM";
            else
              aa.basetype = "ADEPTXBBS";
            break;
          case CRC_USENET:
            strcpy(usenet, val);
            break;
          case CRC_NAME:
            strcpy(name, val);
            break;
          case CRC_NUMBER:
            aa.board = atoi(val);
            break;
          case CRC_ORIGIN:
            aa.setorigin(val);
            break;
          case CRC_PATH:
            PathCopy(apath, val);
            break;
        }
      }
    }

    fclose(fp);
  }
}


//  ------------------------------------------------------------------
//  Read areas from AdeptXBBS

void gareafile::ReadAdeptXBBS(char* tag) {

  char options[80];
  Path file, path, cfg;

  *path = NUL;
  strcpy(cfg, "System\\Message_Areas");
  strcpy(options, tag);
  char* ptr = strtok(tag, " \t");
  while(ptr) {
    if(*ptr != '-')
      AddBackslash(strcpy(path, ptr));
    ptr = strtok(NULL, " \t");
  }
  if(*path == NUL)
    strcpy(path, areapath);

  strcpy(stpcpy(file, path), cfg);

  CfgAdeptxbbspath(path);

  ReadAdeptXbbsFile(path, file, options);
}


//  ------------------------------------------------------------------
