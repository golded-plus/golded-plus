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
//  Read areas from Opus 1.1x/1.7x
//  ------------------------------------------------------------------

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <gfilutil.h>
#include <gdirposx.h>
#include <gwildmat.h>
#include <gedacfg.h>
#undef _NORMAL
#include <gs_opus.h>    // Structures for Opus 1.1x SYSTEM??.DAT


//  ------------------------------------------------------------------
//  Read Opus

void gareafile::ReadOpus(char* tag) {

  FILE* fp;
  AreaCfg aa;
  char* ptr;
  _msgsys msgsys;
  _systemdat sysdat;
  Path oppath, file;
  char buf[256], options[80];

  *oppath = NUL;
  strcpy(options, tag);

  ptr = strtok(tag, " \t");
  while(ptr) {
    if(*ptr != '-') {
      AddBackslash(strcpy(oppath, ptr));
      break;
    }
    ptr = strtok(NULL, " \t");
  }
  if(*oppath == NUL) {
    ptr = getenv("OPUS");
    if(ptr)
      AddBackslash(strcpy(oppath, ptr));
  }
  if(*oppath == NUL)
    strcpy(oppath, areapath);

  MakePathname(file, oppath, "sysmsg.dat");

  if(fexist(file)) {

    // Found Opus 1.7x config files

    fp = fsopen(file, "rb", sharemode);
    if(fp) {
      setvbuf(fp, NULL, _IOFBF, 8192);

      if(not quiet)
        cout << "* Reading " << file << endl;

      while(fread(&msgsys, sizeof(_msgsys), 1, fp) == 1) {

        aa.reset();

        memset(buf, 0, sizeof(buf));
        fread(buf, msgsys.Path_Len, 1, fp);
        aa.setpath(buf);
        memset(buf, 0, sizeof(buf));
        fread(buf, msgsys.Title_Len, 1, fp);
        aa.setdesc(buf);
        fread(buf, msgsys.Barricade_Len, 1, fp);
        memset(buf, 0, sizeof(buf));
        fread(buf, msgsys.Origin_Len, 1, fp);
        aa.setorigin(buf);
        fseek(fp, (long)msgsys.Domain_Len, SEEK_CUR);
        fseek(fp, (long)msgsys.Menu_Len, SEEK_CUR);
        fseek(fp, (long)msgsys.Vol_Len, SEEK_CUR);
        fseek(fp, (long)msgsys.Help_Len, SEEK_CUR);
        fseek(fp, (long)msgsys.Scan_Len*(long)sizeof(struct _ascan), SEEK_CUR);
        fseek(fp, (long)msgsys.Other_Len, SEEK_CUR);

        aa.msgbase = fidomsgtype;
        aa.type = ((msgsys.Attrib & SYSMAIL) ? GMB_NET : (msgsys.Attrib & _ECHOMAIL) ? GMB_ECHO : GMB_LOCAL);
        aa.attr = ((msgsys.Attrib & SYSMAIL) ? attribsnet : (msgsys.Attrib & _ECHOMAIL) ? attribsecho : attribslocal);
        aa.setechoid(*msgsys.Echo_Name ? msgsys.Echo_Name : msgsys.Area_Name);

        if(msgsys.Net) {
          aa.aka.zone = msgsys.Zone;
          aa.aka.net = msgsys.Net;
          aa.aka.node = msgsys.Node;
          aa.aka.point = msgsys.Point;
        }

        AddNewArea(aa);
      }

      fclose(fp);
    }
  }
  else {

    // Try for the old 1.1x files

    gposixdir d(oppath);
    const gdirentry *de;
    if(d.ok) {
      while((de = d.nextentry("system*.dat", true)) != NULL) {

        MakePathname(file, oppath, de->name.c_str());
        fp = fsopen(file, "rb", sharemode);
        if(fp) {
          setvbuf(fp, NULL, _IOFBF, 8192);

          if(not quiet)
            cout << "* Reading " << file << endl;

          fread(&sysdat, sizeof(_systemdat), 1, fp);
          if(*sysdat.msgpath and *sysdat.msgtitle) {
            aa.reset();
            aa.msgbase = fidomsgtype;
            aa.setpath(sysdat.msgpath);
            aa.setdesc(sysdat.msgtitle);
            aa.setechoid(sysdat.EchoName);
            aa.type = ((sysdat.attrib & SYSMAIL) ? GMB_NET : (sysdat.attrib & _ECHOMAIL) ? GMB_ECHO : GMB_LOCAL);
            aa.attr = ((sysdat.attrib & SYSMAIL) ? attribsnet : (sysdat.attrib & _ECHOMAIL) ? attribsecho : attribslocal);
            AddNewArea(aa);
          }

          fclose(fp);
        }
      }
    }
  }
}


//  ------------------------------------------------------------------
