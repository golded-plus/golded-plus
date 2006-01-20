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
//  Read areas from SuperBBS
//  ------------------------------------------------------------------

#include <cstdlib>
#include <gstrall.h>
#include <gmemdbg.h>
#if defined(__GOLD_GUI__)
#include <gvidall.h>
#include <gvidgui.h>
#endif
#undef GCFG_NOSBBS
#include <gedacfg.h>
#include <gs_sbbs.h>


//  ------------------------------------------------------------------
//  Read SuperBBS

void gareafile::ReadSuperBBS(char* tag) {

  AreaCfg aa;
  FILE* fp;
  char* ptr;
  Path sbpath, file;
  char origin[80], options[80];

  ExtraConfigP sconfig = new ExtraConfigT; throw_new(sconfig);
  ConfigRecordP config = new ConfigRecordT; throw_new(config);
  MsgBoardRecordP boards = new MsgBoardRecordT; throw_new(boards);

  *sbpath = NUL;
  *origin = NUL;

  ptr = getenv("SUPERBBS");
  if(ptr == NULL)
    ptr = getenv("SBBS");
  if(ptr)
    AddBackslash(strcpy(sbpath, ptr));
  else
    strcpy(sbpath, areapath);

  // Read AREAS.BBS
  strcpy(options, tag);
  ptr = strtok(tag, " \t");
  while(ptr) {
    if(*ptr != '-') {
      if(is_dir(ptr) and (*sbpath == NUL))
        AddBackslash(strcpy(sbpath, ptr));
      else
        GetAreasBBS(ptr, origin, options);
    }
    ptr = strtok(NULL, " \t");
  }

  MakePathname(file, sbpath, "config.bbs");
  fp = fsopen(file, "rb", sharemode);
  if (fp)
  {
    if (not quiet)
      STD_PRINTNL("* Reading " << file);

    fread(config, sizeof(ConfigRecordT), 1, fp);
    STRNP2C(config->OriginLine);
    fclose(fp);

    MakePathname(file, sbpath, "sconfig.bbs");
    fp = fsopen(file, "rb", sharemode);
    if (fp)
    {
      if (not quiet)
        STD_PRINTNL("* Reading " << file);

      fread(sconfig, sizeof(ExtraConfigT), 1, fp);
      fclose(fp);

      STRNP2C(sconfig->MsgBasePath);
      CfgHudsonpath(sconfig->MsgBasePath);

      // Is it 1.16 or later?
      if(sconfig->VersionNumber >= 0x0110) {

        MakePathname(file, sbpath, "boards.bbs");
        fp = fsopen(file, "rb", sharemode);
        if (fp)
        {
          if (not quiet)
            STD_PRINTNL("* Reading " << file);

          for(int n=0; n<200; n++) {

            fread(boards, sizeof(MsgBoardRecordT), 1, fp);

            if(*boards->Name) {

              aa.reset();

              aa.board = n + 1;
              aa.basetype = "HUDSON";
              aa.groupid = boards->Group;
              aa.setorigin(*config->OriginLine ? config->OriginLine : origin);

              aa.setdesc(STRNP2C(boards->Name));

              switch(boards->Typ) {
                case 0:
                  aa.type = GMB_LOCAL;
                  aa.attr = attribslocal;
                  break;
                case 1:
                  aa.type = GMB_NET;
                  aa.attr = attribsnet;
                  break;
                default:
                  aa.type = GMB_ECHO;
                  aa.attr = attribsecho;
              }

              switch(boards->Kinds) {
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

              // Is it 1.17 or newer?
              if(sconfig->VersionNumber >= 0x0111) {
                // This doesn't work in 1.16
                aa.aka.zone  = sconfig->AkaZone [boards->UseAka];
                aa.aka.net   = sconfig->AkaNet  [boards->UseAka];
                aa.aka.node  = sconfig->AkaNode [boards->UseAka];
                aa.aka.point = sconfig->AkaPoint[boards->UseAka];
              }
              else {
                // But this does
                aa.aka.zone  = sconfig->AkaZone [config->UseAka[n]];
                aa.aka.net   = sconfig->AkaNet  [config->UseAka[n]];
                aa.aka.node  = sconfig->AkaNode [config->UseAka[n]];
                aa.aka.point = sconfig->AkaPoint[config->UseAka[n]];
              }

              AddNewArea(aa);
            }
          }

          fclose(fp);
        }
      }
      else
        STD_PRINTNL("* Error: Unsupported version of SuperBBS: " << (word)(sconfig->VersionNumber >> 8) << '.' << (word)(sconfig->VersionNumber & 0xFF));
    }
  }

  throw_delete(boards);
  throw_delete(config);
  throw_delete(sconfig);
}


//  ------------------------------------------------------------------

