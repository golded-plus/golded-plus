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
//  Portal of Power configuration reader.
//  ------------------------------------------------------------------

#include <cstdlib>
#include <gstrall.h>
#include <gmemdbg.h>
#undef GCFG_NOPORTAL
#include <gedacfg.h>
#include <gs_pop.h>


//  ------------------------------------------------------------------

void gareafile::ReadPortal(char* tag) {

  FILE* fp;
  char* ptr;
  char* ptr2;
  AreaCfg aa;
  int poptask = 0;
  Path path, file;
  char options[80];

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
    ptr = getenv("POPCMDLINE");
    if(ptr) {
      while(*ptr) {
        if(strnieql(ptr, "-T", 2))
          poptask = atoi(ptr+2);
        else if(strnieql(ptr, "-H", 2)) {
          ptr2 = strskip_txt(strcpy(path, ptr+2));
          *ptr2 = NUL;
          AddBackslash(path);
        }
        else if(*ptr != '-' and *ptr != '/') {
          ptr2 = strskip_txt(strcpy(file, ptr));
          *ptr2 = NUL;
        }
        ptr = strskip_wht(strskip_txt(ptr));
      }
    }
  }
  if(*path == NUL)
    strcpy(path, areapath);

  ConfigP popcfg = new ConfigT; throw_new(popcfg);

  if(*file == NUL) {
    if(poptask)
      sprintf(file, "portal%02u.cfg", poptask);
    else
      strcpy(file, "portal.cfg");
    MakePathname(file, path, file);
  }

  fp = fsopen(file, "rb", sharemode);
  if(fp) {


    fread(popcfg, sizeof(ConfigT), 1, fp);
    fclose(fp);

    // Netmail
    if(popcfg->MailScanner.NetMailBoard and popcfg->MailScanner.NetMailBoard < 201) {
      aa.reset();
      aa.aka = CAST(ftn_addr, popcfg->Adresses[popcfg->MainAdrNum]);
      aa.msgbase = GMB_HUDSON;
      aa.type = GMB_NET;
      aa.attr = attribsnet;
      aa.board = popcfg->MailScanner.NetMailBoard;
      strcpy(aa.desc, "Portal Netmail");
      aa.setautoid("NETMAIL");
      AddNewArea(aa);
    }
    else if(not strblank(popcfg->MailScanner.NetMailDir)) {
      aa.reset();
      aa.aka = CAST(ftn_addr, popcfg->Adresses[popcfg->MainAdrNum]);
      aa.msgbase = fidomsgtype;
      aa.type = GMB_NET;
      aa.attr = attribsnet;
      aa.setpath(popcfg->MailScanner.NetMailDir);
      aa.setdesc("Portal Netmail");
      aa.setautoid("NETMAIL");
      AddNewArea(aa);
    }

    // *.MSG badmsgs
    if(not strblank(popcfg->MailScanner.BadMsgs)) {
      aa.reset();
      aa.aka = CAST(ftn_addr, popcfg->Adresses[popcfg->MainAdrNum]);
      aa.msgbase = fidomsgtype;
      aa.type = GMB_ECHO;
      aa.attr = attribsecho;
      aa.setpath(popcfg->MailScanner.BadMsgs);
      aa.setdesc("Portal Bad Msgs");
      aa.setautoid("ECHO_BAD");
      AddNewArea(aa);
    }

    // *.MSG dupes
    if(not strblank(popcfg->MailScanner.SaveDupesDir)) {
      aa.reset();
      aa.aka = CAST(ftn_addr, popcfg->Adresses[popcfg->MainAdrNum]);
      aa.msgbase = fidomsgtype;
      aa.type = GMB_ECHO;
      aa.attr = attribsecho;
      aa.setpath(popcfg->MailScanner.SaveDupesDir);
      aa.setdesc("Portal Saved Dupes");
      aa.setautoid("ECHO_DUPES");
      AddNewArea(aa);
    }

    AreasTypeP poparea = new AreasTypeT; throw_new(poparea);

    MakePathname(file, path, "portal.are");

    fp = fsopen(file, "rb", sharemode);
    if(fp) {


      while(fread(poparea, sizeof(AreasTypeT), 1, fp) == 1) {

        if(*poparea->Directory != 0) {

          aa.reset();
          aa.aka = CAST(ftn_addr, poparea->UsedAka ? popcfg->Adresses[popcfg->MainAdrNum] : popcfg->Adresses[poparea->UsedAka]);
          aa.type = GMB_ECHO;
          aa.attr = attribsecho;
          aa.msgbase = fidomsgtype;
          aa.setdesc(STRNP2C(poparea->Description));
          aa.setechoid(STRNP2C(poparea->EchoNames[0]));
          aa.setorigin(STRNP2C(poparea->Origin));
          AddNewArea(aa);
        }
      }

      fclose(fp);
    }

    throw_delete(poparea);
  }

  throw_delete(popcfg);
}


//  ------------------------------------------------------------------
