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
//  Read areas from XMail
//  ------------------------------------------------------------------

#include <cstdlib>
#include <gcrcall.h>
#include <gstrall.h>
#if defined(__GOLD_GUI__)
#include <gvidall.h>
#include <gvidgui.h>
#endif
#undef GCFG_NOXMAIL
#include <gedacfg.h>
#include <gs_xmail.h>


//  ------------------------------------------------------------------
//  Clip a long filename to DOS compatible 8 chars

char* gareafile::ClipDosFilename(char* __file) {

  // Only work on strings longer than 8 chars
  uint _len = strlen(__file);
  if(_len > 8) {

    // Point to last char before NUL
    char* _ptr = __file + _len - 1;

    // Find the last slash, if any
    while(_len and not isslash(*_ptr))
      _ptr--, _len--;

    // Point to first char after the slash, if any
    if(isslash(*_ptr))
      _ptr++;

    // Cut filename if too long
    if(strlen(_ptr) > 8)
      _ptr[8] = NUL;
  }

  // Return pointer to the filename
  return __file;
}


//  ------------------------------------------------------------------

void gareafile::ReadxMailFile(char* file, char* options) {

  const word CRC_SYSOP = 0x967F;
  const word CRC_NODEADR = 0x4B51;
  const word CRC_AKA = 0x13A4;
  const word CRC_NETMAIL = 0xE42E;
  const word CRC_ORIGIN = 0x4CE5;
  const word CRC_MESSAGEBASE = 0x213E;

  AreaCfg aa;
  char buf[512];

  FILE* fp = fsopen(file, "rb", sharemode);
  if (fp)
  {
    setvbuf(fp, NULL, _IOFBF, 8192);

    if (not quiet)
      STD_PRINT("* Reading " << file << std::endl);

    aa.reset();

    while(fgets(buf, sizeof(buf), fp)) {

      char* ptr = strskip_wht(strtrim(buf));
      if(*ptr != ';' and *ptr) {

        char* key;
        char* val = ptr;
        switch(getkeyvalcrc(&key, &val)) {
          case CRC_SYSOP:
            //CfgUsername(val);
            break;
          case CRC_NODEADR:
          case CRC_AKA:
            CfgAddress(val);
            break;
          case CRC_NETMAIL:
            aa.reset();
            aa.type = GMB_NET;
            aa.attr = attribsnet;
            aa.basetype = fidomsgtype;
            aa.setdesc("xMail Netmail");
            aa.setautoid("NETMAIL");
            aa.setpath(val);
            AddNewArea(aa);
            break;
          case CRC_ORIGIN:
            CfgOrigin(val);
            break;
          case CRC_MESSAGEBASE:
            CfgHudsonpath(val);
            break;
        }
      }
    }

    fclose(fp);
  }
}


//  ------------------------------------------------------------------
//  Read areas from XMail

void gareafile::ReadXMail(char* tag) {

  AreaCfg aa;
  FILE* fp;
  char* ptr;
  int areano;
  char options[80];
  Path xmailpath, file;
  EchoAreaRec area;

  *xmailpath = NUL;
  strcpy(options, tag);
  ptr = strtok(tag, " \t");
  while(ptr) {
    if(*ptr != '-') {
      AddBackslash(strcpy(xmailpath, ptr));
      break;
    }
    ptr = strtok(NULL, " \t");
  }
  if(*xmailpath == NUL) {
    ptr = getenv("XM");
    if(ptr)
      AddBackslash(strcpy(xmailpath, ptr));
  }
  if(*xmailpath == NUL)
    strcpy(xmailpath, areapath);

  MakePathname(file, xmailpath, "xmail.cfg");
  ReadxMailFile(file, options);

  MakePathname(file, xmailpath, "areas.xm");
  fp = fsopen(file, "rb", sharemode);
  if (fp)
  {
    setvbuf(fp, NULL, _IOFBF, 8192);

    if (not quiet)
      STD_PRINT("* Reading " << file << std::endl);

    areano = 1;
    while(fread(&area, sizeof(EchoAreaRec), 1, fp) == 1) {

      if(*area.Name) {

        aa.reset();
        STRNP2C(area.MsgDirectory);

        switch(area.StoreType) {
          case FmtHudson:
            aa.basetype = "HUDSON";
            aa.board = areano;
            break;
          case FmtMsg:
            aa.basetype = fidomsgtype;
            aa.setpath(ClipDosFilename(area.MsgDirectory));
            break;
          case FmtSquish:
            aa.basetype = "SQUISH";
            aa.setpath(ClipDosFilename(area.MsgDirectory));
            break;
          case FmtJam:
            aa.basetype = "JAM";
            aa.setpath(ClipDosFilename(area.MsgDirectory));
            break;
          default:
            // Passthrough or unknown
            continue;
        }

        aa.setdesc(STRNP2C(area.Descr));
        aa.setechoid(STRNP2C(area.Name));
        aa.setorigin(STRNP2C(area.OriginLine));

        aa.aka.zone  = area.AreaAddress.Zone;
        aa.aka.net   = area.AreaAddress.Net;
        aa.aka.node  = area.AreaAddress.Node;
        aa.aka.point = area.AreaAddress.Point;

        switch(area.MailType) {
          case Localmail:
            aa.type = GMB_LOCAL;
            aa.attr = attribslocal;
            break;
          case Netmail:
            aa.type = GMB_NET;
            aa.attr = attribsnet;
            break;
          case Echomail:
          default:
            aa.type = GMB_ECHO;
            aa.attr = attribsecho;
        }

        AddNewArea(aa);
      }

      areano++;
    }

    fclose(fp);
  }
}


//  ------------------------------------------------------------------
