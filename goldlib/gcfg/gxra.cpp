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
//  Read areas from RemoteAccess 0.xx - 1.xx, 2.0x, 2.5x
//  ------------------------------------------------------------------

#include <cstdlib>
#include <gmemdbg.h>
#include <gstrall.h>
#undef GCFG_NORA
#include <gedacfg.h>
#include <gs_ra.h>
#include <gs_ra2.h>


//  ------------------------------------------------------------------
//  Read RemoteAccess MESSAGES.RA

void gareafile::ReadRemoteAccess(char* tag) {

  AreaCfg aa;
  FILE* fp;
  char* ptr;
  Path rapath, file;
  char origin[80], options[80];

  *rapath = NUL;
  *origin = NUL;

  CONFIGrecord* config = (CONFIGrecord*)throw_calloc(1, sizeof(CONFIGrecord));
  if(config) {

    ptr = getenv("RA");
    if(ptr)
      AddBackslash(strcpy(rapath, ptr));
    else
      strcpy(rapath, areapath);

    // Read AREAS.BBS
    strcpy(options, tag);
    ptr = strtok(tag, " \t");
    while(ptr) {
      if(*ptr != '-') {
        if(is_dir(ptr) and (*rapath == NUL))
          AddBackslash(strcpy(rapath, ptr));
        else
          GetAreasBBS(ptr, origin, options);
      }
      ptr = strtok(NULL, " \t");
    }

    MakePathname(file, rapath, "config.ra");

    fp = fsopen(file, "rb", sharemode);
    if(fp) {

      if(not quiet)
        cout << "* Reading " << file << endl;

      fread(config, sizeof(CONFIGrecord), 1, fp);
      fclose(fp);

      if(config->VersionID >= 0x200)
        if(ra2usersbbs == 0)
          ra2usersbbs = 2;

      STRNP2C(config->MsgBasePath);
      if(*hudsonpath == NUL)
        PathCopy(hudsonpath, MapPath(config->MsgBasePath));
    }

    MakePathname(file, rapath, "messages.ra");
    if(not fexist(file))
      if(*config->SysPath)
        strnp2cc(rapath, config->SysPath, sizeof(Path));

    fp = fsopen(file, "rb", sharemode);
    if(fp) {
      setvbuf(fp, NULL, _IOFBF, 8192);

      if(not quiet)
        cout << "* Reading " << file << endl;

      if(config->VersionID >= 0x200) {
        MESSAGErecord* area = new MESSAGErecord; throw_new(area);
        int n=0;
        while(fread(area, sizeof(MESSAGErecord), 1, fp) == 1) {
          n++;
          if(*area->Name) {
            aa.reset();
            aa.aka = CAST(ftn_addr, config->Address[area->AkaAddress]);
            switch(area->Typ) {
              case LocalMail:   // Local
                aa.type = GMB_LOCAL;
                aa.attr = attribslocal;
                break;
              case NetMail:     // Netmail
                aa.type = GMB_NET;
                aa.attr = attribsnet;
                break;
              default:          // Echomail
                aa.type = GMB_ECHO;
                aa.attr = attribsecho;
            }
            switch(area->MsgKinds) {
              case Both:
              case Private:
                aa.attr.pvt1();
                break;
              case Public:
                aa.attr.pvt0();
                break;
              case ROnly:
                aa.attr.r_o1();
                break;
            }
            if(area->Group)
              aa.groupid = (char)(area->Group+'A'-1);
            aa.setdesc(STRNP2C(area->Name));
            STRNP2C(area->OriginLine);
            strchg(area->OriginLine, '@', '0');
            aa.setorigin(area->OriginLine);

            if(area->Attribute & 0x80) {
              aa.setpath(STRNP2C(area->JAMbase));
              aa.msgbase = GMB_JAM;
              AddNewArea(aa);
            }
            else {
              if(config->VersionID >= 0x210)
                aa.board = area->AreaNum;
              else
                aa.board = n;
              aa.msgbase = GMB_HUDSON;
              AddNewArea(aa);
            }
          }
        }
        throw_delete(area);
      }
      else {
        _messagesra* area = new _messagesra; throw_new(area);
        for(int n=0; n<200; n++) {
          fread(area, sizeof(_messagesra), 1, fp);
          if(*area->name) {
            aa.reset();
            aa.msgbase = GMB_HUDSON;
            aa.board = n+1;
            aa.aka = CAST(ftn_addr, config->Address[area->akaaddress]);
            switch(area->type) {
              case LocalMail:   // Local
                aa.type = GMB_LOCAL;
                aa.attr = attribslocal;
                break;
              case NetMail:     // Netmail
                aa.type = GMB_NET;
                aa.attr = attribsnet;
                break;
              default:          // Echomail
                aa.type = GMB_ECHO;
                aa.attr = attribsecho;
            }
            switch(area->msgkinds) {
              case Both:
              case Private:
                aa.attr.pvt1();
                break;
              case Public:
                aa.attr.pvt0();
                break;
              case ROnly:
                aa.attr.r_o1();
                break;
            }
            aa.setdesc(STRNP2C(area->name));
            STRNP2C(area->originline);
            strchg(area->originline, '@', '0');
            aa.setorigin(area->originline);
            AddNewArea(aa);
          }
        }
        throw_delete(area);
      }

      fclose(fp);
    }

    throw_free(config);
  }
}


//  ------------------------------------------------------------------
