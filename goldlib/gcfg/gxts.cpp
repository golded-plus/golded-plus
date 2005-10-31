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
//  Read areas from TosScan 1.00 (and FrontDoor)
//  ------------------------------------------------------------------

#include <cstdlib>
#include <gstrall.h>
#include <gmemdbg.h>
#undef GCFG_NOTOSSCAN
#include <gedacfg.h>
#include <gs_fd.h>      // Structures for FrontDoor fd.sys+folder.sys / setup.fd+folder.fd
#include <gs_ts.h>      // Structures for TosScan   areafile.fd


//  ------------------------------------------------------------------
//  Read TosScan AREAFILE.FD

void gareafile::ReadTosScan(char* tag) {

  AreaCfg aa;
  FILE* fp;
  char* ptr;
  word sysrev;
  FD_Editor* editor;
  FD_Shared* shared;
  Path tspath, file;
  TS_Areafile* areafile;
  char buf[256], options[80];

  *tspath = NUL;
  editor   = new FD_Editor; throw_new(editor);
  shared   = new FD_Shared; throw_new(shared);
  areafile = new TS_Areafile; throw_new(areafile);

  strcpy(options, tag);
  ptr = strtok(tag, " \t");
  while(ptr) {
    if(*ptr != '-') {
      AddBackslash(strcpy(tspath, ptr));
      break;
    }
    ptr = strtok(NULL, " \t");
  }
  if(*tspath == NUL) {
    ptr = getenv("FD");
    if(ptr)
      AddBackslash(strcpy(tspath, ptr));
  }
  if(*tspath == NUL)
    strcpy(tspath, areapath);

  if(fexist(AddPath(tspath, "areafile.fd"))) {

    if(fexist(AddPath(tspath, "setup.fd")))
      MakePathname(file, tspath, "setup.fd");
    else
      MakePathname(file, tspath, "fd.sys");

    fp = fsopen(file, "rb", sharemode);
    if(fp) {

      if(not quiet)
        std::cout << "* Reading " << file << std::endl;

      fread(buf, 5, 1, fp);
      if(streql(buf, "JoHo")) {   // Check to see that it is v1.99b or higher
        fread(&sysrev, sizeof(word), 1, fp);
        // This probably ought to be if(sysrev == FD_THISREV)..
        fseek(fp, 4, SEEK_CUR);                   // Seek past CRC32
        fseek(fp, sizeof(FD_Mailer), SEEK_CUR);   // Seek past some data
        fread(editor, sizeof(FD_Editor), 1, fp);
        fread(shared, sizeof(FD_Shared), 1, fp);
        CfgHudsonpath(editor->qbase);
        aa.reset();
        aa.type = GMB_NET;
        aa.attr = attribsnet;
        aa.basetype = fidomsgtype;
        aa.attr.r_o(editor->netfolderflags & EDREADONLY);
        aa.attr.pvt(editor->msgbits & MSGPRIVATE);
        aa.attr.cra(editor->msgbits & MSGCRASH);
        aa.attr.k_s(editor->msgbits & MSGKILL);
        aa.setpath(shared->mailpath);
        aa.setdesc("FrontDoor Netmail");
        aa.setautoid("NETMAIL");
        AddNewArea(aa);
      }
      fclose(fp);
    }

    MakePathname(file, tspath, "areafile.fd");

    fp = fsopen(file, "rb", sharemode);
    if(fp) {
      setvbuf(fp, NULL, _IOFBF, 8192);

      if(not quiet)
        std::cout << "* Reading " << file << std::endl;

      fseek(fp, 4L, SEEK_SET);  // Skip CRC32

      while(fread(areafile, sizeof(TS_Areafile), 1, fp) == 1) {
        if(not (areafile->deleted or areafile->passthru) and not strblank(areafile->echoname) and (areafile->board ? areafile->board < 201 : 1)) {
          aa.reset();
          aa.groupid = (char)g_toupper(areafile->groupid);
          aa.aka = shared->aka[areafile->akano];
          aa.type = GMB_ECHO;
          aa.attr = attribsecho;
          if(areafile->board) {
            aa.basetype = "HUDSON";
            aa.board = areafile->board;
          }
          else {
            aa.basetype = fidomsgtype;
            aa.setpath(areafile->path);
          }
          aa.setdesc(areafile->desc);
          aa.setechoid(areafile->echoname);
          aa.setorigin(editor->origin[areafile->origno-1]);
          AddNewArea(aa);
        }
      }
      fclose(fp);
    }
  }

  throw_delete(areafile);
  throw_delete(editor);
  throw_delete(shared);
}


//  ------------------------------------------------------------------
