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

#include <cstdlib>
#include <gmemdbg.h>
#include <gstrall.h>
#undef GCFG_NOFMAIL
#include <gedacfg.h>
#include <gs_fm116.h>


//  ------------------------------------------------------------------

void gareafile::ReadFMail116(FILE* fp, char* path, char* file, char* options) {

  AreaCfg aa;

  configType* cfg = new configType; throw_new(cfg);

  fread(cfg, sizeof(configType), 1, fp);
  fclose(fp);

  // Get Hudson msgbase path
  CfgHudsonpath(cfg->bbsPath);

  // .MSG style netmail
  if(not strblank(cfg->netPath)) {
    aa.reset();
    aa.aka = cfg->akaList[0].nodeNum;
    aa.msgbase = fidomsgtype;
    aa.type = GMB_NET;
    aa.attr = attribsnet;
    aa.setpath(cfg->netPath);
    aa.setdesc("FMail Netmail");
    aa.setautoid("NETMAIL");
    AddNewArea(aa);
  }

  // .MSG style netmail sent
  if(not strblank(cfg->sentPath)) {
    aa.reset();
    aa.aka = cfg->akaList[0].nodeNum;
    aa.msgbase = fidomsgtype;
    aa.type = GMB_NET;
    aa.attr = attribsnet;
    aa.setpath(cfg->sentPath);
    aa.setdesc("FMail Netmail Sent");
    aa.setautoid("NET_SENT");
    AddNewArea(aa);
  }

  // .MSG style netmail rcvd
  if(not strblank(cfg->rcvdPath)) {
    aa.reset();
    aa.aka = cfg->akaList[0].nodeNum;
    aa.msgbase = fidomsgtype;
    aa.type = GMB_NET;
    aa.attr = attribsnet;
    aa.setpath(cfg->rcvdPath);
    aa.setdesc("FMail Netmail Received");
    aa.setautoid("NET_RECEIVED");
    AddNewArea(aa);
  }

  // Hudson netmail
  for(int x=0; x<MAX_NETAKAS; x++) {
    if(cfg->netmailBoard[x] and cfg->netmailBoard[x] < 201) {
      aa.reset();
      aa.msgbase = GMB_HUDSON;
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

  // Hudson recovery board
  if(cfg->recBoard and cfg->recBoard < 201) {
    aa.reset();
    aa.aka = cfg->akaList[0].nodeNum;
    aa.msgbase = GMB_HUDSON;
    aa.type = GMB_ECHO;
    aa.attr = attribsecho;
    aa.board = cfg->recBoard;
    aa.setdesc("FMail Recovery Board");
    aa.setautoid("ECHO_RECOVERY");
    AddNewArea(aa);
  }

  // Hudson badmsgs
  if(cfg->badBoard and cfg->badBoard < 201) {
    aa.reset();
    aa.aka = cfg->akaList[0].nodeNum;
    aa.msgbase = GMB_HUDSON;
    aa.type = GMB_ECHO;
    aa.attr = attribsecho;
    aa.board = cfg->badBoard;
    aa.setdesc("FMail Bad Echomail Board");
    aa.setautoid("ECHO_BAD");
    AddNewArea(aa);
  }

  // Hudson dupmsgs
  if(cfg->dupBoard and cfg->dupBoard < 201) {
    aa.reset();
    aa.aka = cfg->akaList[0].nodeNum;
    aa.msgbase = GMB_HUDSON;
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
    aa.msgbase = fidomsgtype;
    aa.type = GMB_ECHO;
    aa.attr = attribsecho;
    aa.setpath(cfg->pmailPath);
    aa.setdesc("FMail Personal Mail");
    aa.setautoid("ECHO_PERSONAL");
    AddNewArea(aa);
  }        

  headerType hdr;

  rawEchoType116* ar;

  MakePathname(file, path, "fmail.ar");

  fp = fsopen(file, "rb", sharemode);
  if(fp) {


    fread(&hdr, sizeof(headerType), 1, fp);

    uint headerSize = hdr.headerSize;
    uint recordSize = hdr.recordSize;

    fseek(fp, headerSize, SEEK_SET);

    ar = (rawEchoType116*)throw_calloc(1, recordSize);
    if(ar) {

      while(fread(ar, recordSize, 1, fp) == 1) {

        if(ar->options.active) {

          aa.reset();

          aa.groupid = 'A';
          dword grp = ar->group;
          while((grp & 1L) == 0) {
            if((++aa.groupid) == 'Z')
              break;
            grp >>= 1;
          }

          //aa.groupid = (char)toupper((char)ar->group);
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
          if(ar->board) {
            aa.board = ar->board;
            aa.msgbase = GMB_HUDSON;
          }
          else if(*ar->msgBasePath) {
            aa.setpath(ar->msgBasePath);
            aa.msgbase = GMB_JAM;
          }

          aa.setdesc(ar->comment);
          aa.setechoid(ar->areaName);
          aa.setorigin(ar->originLine);
          AddNewArea(aa);
        }
      }
    }

    throw_free(ar);

    fclose(fp);
  }
  
  throw_delete(cfg);
}


//  ------------------------------------------------------------------

void gareafile::ReadFMail(char* tag) {

  char* ptr;
  FILE* fp;
  char options[80];
  Path path, file;

  *file = NUL;
  strcpy(options, tag);
  ptr = strtok(tag, " \t");
  while(ptr) {
    if(*ptr != '-') {
      AddBackslash(strcpy(path, ptr));
      break;
    }
    ptr = strtok(NULL, " \t");
  }
  if(*file == NUL) {
    ptr = getenv("FMAIL");
    if(ptr)
      AddBackslash(strcpy(path, ptr));
  }
  if(*path == NUL)
    strcpy(path, areapath);

  MakePathname(file, path, "fmail.cfg");

  fp = fsopen(file, "rb", sharemode);
  if(fp) {

    struct {
      byte vmajor;
      byte vminor;
    } _rev;

    fread(&_rev, sizeof(_rev), 1, fp);
    rewind(fp);

    uint fmver = (_rev.vmajor << 8) | _rev.vminor;
    
    if(fmver < 94)
      ReadFMail092(fp, path, file, options);
    else {

      FILE* fpar;
      headerType ar;
      fpar = fsopen(AddPath(path, "fmail.ar"), "rb", sharemode);
      if(fpar) {
        fread(&ar, sizeof(ar), 1, fpar);
        fclose(fpar);
      }

      uint ar_rev = (ushort)ar.revNumber;

      if((ar_rev >= 0x0100) and (ar_rev < 0x0110))
        ReadFMail098(fp, path, file, options);
      else if((ar_rev >= 0x0110) and (ar_rev < 0x0200))
        ReadFMail116(fp, path, file, options);
      else
        std::cout << "* Error: Unknown FMail config revision " << std::setfill('0') << std::setw(4) << std::hex << ar_rev << "h - Skipping." << std::endl;
    }
  }
}


//  ------------------------------------------------------------------
