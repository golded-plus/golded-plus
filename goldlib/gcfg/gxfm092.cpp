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
//  Read areas from FMail
//  ------------------------------------------------------------------

#include <gstrall.h>
#include <gmemdbg.h>
#undef GCFG_NOFMAIL
#include <gedacfg.h>
#include <gs_fm092.h>


//  ------------------------------------------------------------------

void gareafile::ReadFMail092(FILE* fp, char* path, char* file, char* options) {

  AreaCfg aa;

  configType* cfg = new configType; throw_new(cfg);

  fread(cfg, sizeof(configType), 1, fp);
  fclose(fp);

  // Get Hudson msgbase path
  CfgHudsonpath(cfg->bbsPath);

  // Opus-.MSG style netmail
  if(not strblank(cfg->netPath)) {
    aa.reset();
    aa.aka = cfg->akaList[0].nodeNum;
    aa.basetype = fidomsgtype;
    aa.type = GMB_NET;
    aa.attr = attribsnet;
    aa.setpath(cfg->netPath);
    aa.setdesc("FMail Netmail");
    aa.setautoid("NETMAIL");
    AddNewArea(aa);
  }

  // Opus-.MSG style netmail sent
  if(not strblank(cfg->sentPath)) {
    aa.reset();
    aa.aka = cfg->akaList[0].nodeNum;
    aa.basetype = fidomsgtype;
    aa.type = GMB_NET;
    aa.attr = attribsnet;
    aa.setpath(cfg->sentPath);
    aa.setdesc("FMail Netmail Sent");
    aa.setautoid("NET_SENT");
    AddNewArea(aa);
  }

  // Opus-.MSG style netmail rcvd
  if(not strblank(cfg->rcvdPath)) {
    aa.reset();
    aa.aka = cfg->akaList[0].nodeNum;
    aa.basetype = fidomsgtype;
    aa.type = GMB_NET;
    aa.attr = attribsnet;
    aa.setpath(cfg->rcvdPath);
    aa.setdesc("FMail Netmail Received");
    aa.setautoid("NET_RECEIVED");
    AddNewArea(aa);
  }

  // Q-style netmail
  for(int x=0; x<MAX_AKAS; x++) {
    if(cfg->netmailBoard[x] and cfg->netmailBoard[x] < 201) {
      aa.reset();
      aa.basetype = "HUDSON";
      aa.type = GMB_NET;
      aa.attr = attribsnet;
      aa.board = cfg->netmailBoard[x];
      aa.aka = cfg->akaList[x].nodeNum;
      Desc desc;
      sprintf(desc, "%s", cfg->descrAKA[x]);
      aa.setdesc(desc);
      sprintf(desc, "NET_AKA%u", x);
      aa.setautoid(desc);
      AddNewArea(aa);
    }
  }

  // Q-style recovery board
  if(cfg->recBoard and cfg->recBoard < 201) {
    aa.reset();
    aa.aka = cfg->akaList[0].nodeNum;
    aa.basetype = "HUDSON";
    aa.type = GMB_ECHO;
    aa.attr = attribsecho;
    aa.board = cfg->recBoard;
    aa.setdesc("FMail Recovery Board");
    aa.setautoid("ECHO_RECOVERY");
    AddNewArea(aa);
  }

  // Q-style badmsgs
  if(cfg->badBoard and cfg->badBoard < 201) {
    aa.reset();
    aa.aka = cfg->akaList[0].nodeNum;
    aa.basetype = "HUDSON";
    aa.type = GMB_ECHO;
    aa.attr = attribsecho;
    aa.board = cfg->badBoard;
    aa.setdesc("FMail Bad Echomail Board");
    aa.setautoid("ECHO_BAD");
    AddNewArea(aa);
  }

  // Q-style dupmsgs
  if(cfg->dupBoard and cfg->dupBoard < 201) {
    aa.reset();
    aa.aka = cfg->akaList[0].nodeNum;
    aa.basetype = "HUDSON";
    aa.type = GMB_ECHO;
    aa.attr = attribsecho;
    aa.board = cfg->dupBoard;
    aa.setdesc("FMail Duplicates Board");
    aa.setautoid("ECHO_DUPES");
    AddNewArea(aa);
  }

  // Personal mail
  if(not strblank(cfg->pmailPath)) {
    aa.reset();
    aa.aka = cfg->akaList[0].nodeNum;
    aa.basetype = fidomsgtype;
    aa.type = GMB_ECHO;
    aa.attr = attribsecho;
    aa.setpath(cfg->pmailPath);
    aa.setdesc("FMail Personal Mail");
    aa.setautoid("ECHO_PERSONAL");
    AddNewArea(aa);
  }


  rawEchoType* ar = new rawEchoType; throw_new(ar);

  MakePathname(file, path, "fmail.ar");

  fp = fsopen(file, "rb", sharemode);
  if(fp) {

    while(fread(ar, sizeof(rawEchoType), 1, fp) == 1) {

      if(ar->options.active and ar->board and ar->board < 201) {

        aa.reset();
        aa.aka = cfg->akaList[ar->address].nodeNum;
        if(ar->options.local) {
          aa.type = GMB_LOCAL;
          aa.attr = attribslocal;
        }
        else {
          aa.type = GMB_ECHO;
          aa.attr = attribsecho;
        }
        switch(ar->msgKindsRA) {
          case 1:
            aa.attr.pvt1();
            break;
          case 0:
          case 2:
            aa.attr.pvt0();
            break;
          case 3:
            aa.attr.r_o1();
            break;
        }
        aa.board = ar->board;
        aa.basetype = "HUDSON";
        aa.setdesc(ar->comment);
        aa.setechoid(ar->areaNameOld[0] ? ar->areaNameOld : ar->areaName);
        aa.setorigin(ar->originLine);
        AddNewArea(aa);
      }
    }

    fclose(fp);
  }

  throw_delete(ar);
  throw_delete(cfg);
}


//  ------------------------------------------------------------------
