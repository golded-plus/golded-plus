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
//  Read areas from IMail 1.7x
//  ------------------------------------------------------------------

#include <cstdlib>
#include <gmemdbg.h>
#include <gstrall.h>
#if defined(__GOLD_GUI__)
#include <gvidall.h>
#include <gvidgui.h>
#endif
#undef GCFG_NOIMAIL
#include <gedacfg.h>

#undef MAXPATH
#undef MAXDRIVE
#undef MAXDIR
#undef MAXFILE
#undef MAXEXT

#undef except

#include <gs_im175.h>


//  ------------------------------------------------------------------
//  Read IMail IMAIL.CF and IMAIL.AR

void gareafile::ReadIMail170(char* options, char* file, char* impath) {

  AreaCfg aa;
  FILE* fp;
  im_config_type* CF;
  areas_record_type AR;

  CF = new im_config_type; throw_new(CF);

  fp = fsopen(file, "rb", sharemode);
  if (fp)
  {
    if (not quiet)
      STD_PRINT("* Reading " << file << std::endl);

    fread(CF, sizeof(im_config_type), 1, fp);
    fclose(fp);

    CfgJampath(CF->echojam);
    CfgHudsonpath(CF->quickbbs);

    // Fido netmail
    if(not strblank(CF->netmail)) {
      aa.reset();
      aa.basetype = fidomsgtype;
      aa.type = GMB_NET;
      aa.attr = attribsnet;
      aa.aka = CAST(ftn_addr, CF->aka[0]);
      aa.setpath(CF->netmail);
      aa.setdesc("IMAIL Netmail");
      aa.setautoid("NETMAIL");
      AddNewArea(aa);
    }

    MakePathname(file, impath, "IMAIL.AR");

    fp = fsopen(file, "rb", sharemode);
    if (fp)
    {
      setvbuf(fp, NULL, _IOFBF, 8192);

      if (not quiet)
        STD_PRINT("* Reading " << file << std::endl);

      while(fread(&AR, sizeof(areas_record_type), 1, fp) == 1) {

        if(AR.active and (not AR.deleted)) {

          aa.reset();

          switch(AR.msg_base_type & BASEMASK) {

            case MSGTYPE_SDM:
              aa.basetype = fidomsgtype;
              strcpy(aa.path, AR.msg_path);
              break;

            case MSGTYPE_SQUISH:
              aa.basetype = "SQUISH";
              strcpy(aa.path, AR.msg_path);
              break;

            case MSGTYPE_JAM:
              aa.basetype = "JAM";
              strcpy(aa.path, AR.msg_path);
              break;

            case MSGTYPE_HUDSON:
              aa.basetype = "HUDSON";
              if ((AR.brd >= 1) and (AR.brd <= 200))
                aa.board = AR.brd;
              else
              {
                STD_PRINT("* Warning: Invalid board " << AR.brd << " (" << AR.aname << ") in IMAIL.AR - Skipping." << std::endl);
                continue;
              }
              break;

            case MSGTYPE_PASSTH:
            default:
              // Passthrough or unknown msgbase type
              continue;
          }

          switch(AR.msg_base_type & TYPEMASK) {

            case MSGTYPE_LOCAL:
              aa.type = GMB_LOCAL;
              aa.attr = attribslocal;
              break;

            case MSGTYPE_NET:
              aa.type = GMB_NET;
              aa.attr = attribsnet;
              break;

            case MSGTYPE_ECHO:
            default:
              aa.type = GMB_ECHO;
              aa.attr = attribsecho;
          }

          if(AR.o_addr)
            aa.aka = CAST(ftn_addr, CF->aka[AR.o_addr-1]);

          aa.groupid = AR.grp + 0x8000u;

          aa.setdesc(AR.comment);
          aa.setechoid(AR.aname);
          aa.setorigin(AR.origin);

          AddNewArea(aa);
        }
      }

      fclose(fp);
    }
  }

  throw_delete(CF);
}


//  ------------------------------------------------------------------
