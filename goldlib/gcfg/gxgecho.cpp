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
//  Read areas from GEcho
//  ------------------------------------------------------------------

#include <cstdlib>
#include <gmemdbg.h>
#include <gstrall.h>
#if defined(__GOLD_GUI__)
#include <gvidall.h>
#include <gvidgui.h>
#endif
#undef GCFG_NOGECHO
#include <gedacfg.h>

#if defined(ADDRESS)
#undef ADDRESS
#endif

#include <gs_ge120.h>


//  ------------------------------------------------------------------
//  Read GEcho AREAFILE.GE and SETUP.GE

void gareafile::ReadGEcho(char* tag) {

  FILE* fp;
  char* ptr;
  char* ptr2;
  AreaCfg aa;
  int n;
  word sysrev;
  AREAFILE_GE* area;
  AREAFILE_HDR ahdr;
  SETUP_GE* gesetup;
  Path gepath, file;
  char options[80], abuf[40];

  *gepath = NUL;
  
  strcpy(options, tag);
  ptr = strtok(tag, " \t");
  while(ptr) {
    if(*ptr != '-') {
      AddBackslash(strcpy(gepath, ptr));
      break;
    }
    ptr = strtok(NULL, " \t");
  }
  if(*gepath == NUL) {
    ptr = getenv("GE");
    if(ptr) {
      ptr = strcpy(gepath, ptr);
      ptr2 = strchr(ptr, ' ');
      if(ptr2)
        *ptr2 = NUL;
      AddBackslash(gepath);
    }
  }
  if(*gepath == NUL)
    strcpy(gepath, areapath);

  MakePathname(file, gepath, "setup.ge");

  gesetup = (SETUP_GE*)throw_calloc(1, sizeof(SETUP_GE));
  if(gesetup) {

    fp = fsopen(file, "rb", sharemode);
    if (fp)
    {
      if (not quiet)
        STD_PRINT("* Reading " << file << std::endl);

      fread(&sysrev, sizeof(word), 1, fp);
      rewind(fp);

      if(sysrev == GE_THISREV) {

        fread(gesetup, sizeof(SETUP_GE), 1, fp);
        fclose(fp);

        int ge_version = (gesetup->version_major * 100) + gesetup->version_minor;

        //for(n=0; n<USERS; n++)
          //CfgUsername(gesetup->username[n]);

        CfgHudsonpath(gesetup->hmbpath);
        CfgJampath(gesetup->jampath);

        const char *_fidomsgtype = fidomsgtype;

        if(ge_version >= 102)
        {
          _fidomsgtype = (gesetup->extraoptions & OPUSDATES) ? "OPUS" : "FTS1";
          if((streql(_fidomsgtype, "FTS1")) and (streql(fidomsgtype, "OPUS")))
          {
            STD_PRINT("* Warning - FTS-1 format is used for *.MSG. For better compatibility set this" << std::endl);
            STD_PRINT("* in GSETUP: Miscellanous->GEcho Options->MSG compatibilty = Opus (not Fido)." << std::endl);
            STD_PRINT("* To disable this warning, set FIDOMSGTYPE FTS1 in your GoldED setup." << std::endl);
          }
        }

        int _AKAS = AKAS;
        ftn_addr* _aka = (ftn_addr*)gesetup->aka;
        if(ge_version < 110) {
          _aka = (ftn_addr*)gesetup->oldaka;
          _AKAS = OLDAKAS;
        }

        // Old netmail board (no longer in use by GEcho, but just in case...)
        if(gesetup->oldnetmailboard and gesetup->oldnetmailboard < 201) {
          aa.reset();
          aa.basetype = "HUDSON";
          aa.type = GMB_NET;
          aa.attr = attribsnet;
          aa.aka = _aka[0];
          aa.board = gesetup->oldnetmailboard;
          aa.setdesc("GEcho Netmail (HMB)");
          aa.setautoid("NETMAIL");
          AddNewArea(aa);
        }
        else if(not strblank(gesetup->mailpath)) {
          aa.reset();
          aa.basetype = _fidomsgtype;
          aa.type = GMB_NET;
          aa.attr = attribsnet;
          aa.aka = _aka[0];
          aa.setpath(gesetup->mailpath);
          aa.setdesc("GEcho Netmail");
          aa.setautoid("NETMAIL");
          AddNewArea(aa);
        }

        // Bad msgs
        uint _badboard = (ge_version >= 110) ? gesetup->badarea : gesetup->oldbadboard;
        if(_badboard and _badboard < 201) {
          aa.reset();
          aa.basetype = "HUDSON";
          aa.type = GMB_ECHO;
          aa.attr = attribsecho;
          aa.board = _badboard;
          aa.aka = _aka[0];
          aa.setdesc("GEcho Bad Msgs (HMB)");
          aa.setautoid("ECHO_BAD");
          AddNewArea(aa);
        }
        else if(not strblank(gesetup->badecho_path)) {
          aa.reset();
          aa.basetype = _fidomsgtype;
          aa.type = GMB_ECHO;
          aa.attr = attribsecho;
          aa.aka = _aka[0];
          aa.setpath(gesetup->badecho_path);
          aa.setdesc("GEcho Bad Msgs");
          aa.setautoid("ECHO_BAD");
          AddNewArea(aa);
        }

        // Dupe msgs
        if(ge_version < 120) {
          //uint _dupboard = (ge_version >= 110) ? gesetup->duparea : gesetup->olddupboard;
          uint _dupboard = (ge_version >= 110) ? gesetup->reserved5 : gesetup->olddupboard;
          if(_dupboard and _dupboard < 201) {
            aa.reset();
            aa.basetype = "HUDSON";
            aa.type = GMB_ECHO;
            aa.attr = attribsecho;
            aa.board = _dupboard;
            aa.aka = _aka[0];
            aa.setdesc("GEcho Dupe Msgs (HMB)");
            aa.setautoid("ECHO_DUPES");
            AddNewArea(aa);
          }
        }

        // Recovery
        if(gesetup->recoveryboard and gesetup->recoveryboard < 201) {
          aa.reset();
          aa.basetype = "HUDSON";
          aa.type = GMB_ECHO;
          aa.attr = attribsecho;
          aa.board = gesetup->recoveryboard;
          aa.aka = _aka[0];
          aa.setdesc("GEcho Recovered Msgs (HMB)");
          aa.setautoid("ECHO_RECOVERED");
          AddNewArea(aa);
        }

        // Received msgs
        uint _rcvdboard = (ge_version >= 110) ? gesetup->rcvdarea : gesetup->oldrcvdboard;
        if(_rcvdboard and _rcvdboard < 201) {
          aa.reset();
          aa.basetype = "HUDSON";
          aa.type = GMB_ECHO;
          aa.attr = attribsecho;
          aa.board = _rcvdboard;
          aa.aka = _aka[0];
          aa.setdesc("GEcho Received Netmail (HMB)");
          aa.setautoid("NET_RECEIVED");
          AddNewArea(aa);
        }
        else if(not strblank(gesetup->rcvdmailpath)) {
          aa.reset();
          aa.basetype = _fidomsgtype;
          aa.type = GMB_NET;
          aa.attr = attribsnet;
          aa.aka = _aka[0];
          aa.setpath(gesetup->rcvdmailpath);
          aa.setdesc("GEcho Received Netmail");
          aa.setautoid("NET_RECEIVED");
          AddNewArea(aa);
        }

        // Sent msgs
        uint _sentboard = (ge_version >= 110) ? gesetup->sentarea : gesetup->oldsentboard;
        if(gesetup->oldsentboard and gesetup->oldsentboard < 201) {
          aa.reset();
          aa.basetype = "HUDSON";
          aa.type = GMB_ECHO;
          aa.attr = attribsecho;
          aa.board = _sentboard;
          aa.aka = _aka[0];
          aa.setdesc("GEcho Sent Netmail (HMB)");
          aa.setautoid("NET_SENT");
          AddNewArea(aa);
        }
        else if(not strblank(gesetup->sentmailpath)) {
          aa.reset();
          aa.basetype = _fidomsgtype;
          aa.type = GMB_NET;
          aa.attr = attribsnet;
          aa.aka = _aka[0];
          aa.setpath(gesetup->sentmailpath);
          aa.setdesc("GEcho Sent Netmail");
          aa.setautoid("NET_SENT");
          AddNewArea(aa);
        }

        // Personal mail boards
        for(n=0; n<USERS; n++) {
          int u = USERS - n - 1;
          uint _persmailboard = (ge_version >= 110) ? gesetup->persmailarea[u] : gesetup->oldpersmailboard[u];
          if(_persmailboard and _persmailboard < 201) {
            aa.reset();
            aa.basetype = "HUDSON";
            aa.type = GMB_ECHO;
            aa.attr = attribsecho;
            aa.aka = _aka[0];
            aa.board = _persmailboard;
            Desc desc;
            sprintf(desc, "GEcho PM for %s", gesetup->username[u]);
            aa.setdesc(desc);
            sprintf(desc, "ECHO_PM%u", u);
            aa.setautoid(desc);
            AddNewArea(aa);
          }
        }

        // Personal mail
        if(not strblank(gesetup->persmail_path)) {
          aa.reset();
          aa.basetype = _fidomsgtype;
          aa.type = GMB_ECHO;
          aa.attr = attribsecho;
          aa.aka = _aka[0];
          aa.setpath(gesetup->persmail_path);
          aa.setdesc("GEcho Personal Mail");
          aa.setautoid("ECHO_PERSONAL");
          AddNewArea(aa);
        }

        // User netmail boards
        for(n=0; n<USERS; n++) {
          int u = USERS - n - 1;
          uint _userboard = (ge_version >= 110) ? gesetup->userarea[u] : gesetup->olduserboard[u];
          if(_userboard and _userboard < 201) {
            aa.reset();
            aa.basetype = "HUDSON";
            aa.type = GMB_NET;
            aa.attr = attribsnet;
            aa.aka = _aka[0];
            aa.board = _userboard;
            Desc desc;
            sprintf(desc, "GEcho Netmail for %s", gesetup->username[u]);
            aa.setdesc(desc);
            sprintf(desc, "NET_USER%u", u);
            aa.setautoid(desc);
            AddNewArea(aa);
          }
        }

        // AKA net areas
        for(n=0; n<_AKAS; n++) {
          int a = _AKAS - n - 1;
          uint _akaboard = (ge_version >= 110) ? gesetup->akaarea[a] : gesetup->oldakaboard[a];
          if(_aka[a].net and _akaboard and _akaboard < 201) {
            aa.reset();
            aa.basetype = "HUDSON";
            aa.type = GMB_NET;
            aa.attr = attribsnet;
            aa.aka = _aka[a];
            aa.board = _akaboard;
            Desc desc;
            sprintf(desc, "GEcho Netmail, %s", _aka[a].make_string(abuf));
            aa.setdesc(desc);
            sprintf(desc, "NET_AKA%u", a);
            aa.setautoid(desc);
            AddNewArea(aa);
          }
        }

        //  --------------------------------------------------------------

        MakePathname(file, gepath, "areafile.ge");

        fp = fsopen(file, "rb", sharemode);
        if (fp)
        {
          setvbuf(fp, NULL, _IOFBF, 8192);

          if (not quiet)
            STD_PRINT("* Reading " << file << std::endl);

          fread(&ahdr, sizeof(AREAFILE_HDR), 1, fp);
          uint arearecsize = (ahdr.maxconnections * sizeof(CONNECTION)) + ahdr.recsize;

          fseek(fp, (long)ahdr.hdrsize, SEEK_SET);  // Allow for changed struct

          area = (AREAFILE_GE*)throw_malloc(arearecsize);

          if(area) {
            while(fread(area, arearecsize, 1, fp) == 1) {
              if(not (area->options & (PASSTHRU|REMOVED))) {

                memset(((byte*)area)+ahdr.recsize, 0, arearecsize-ahdr.recsize);

                aa.reset();

                switch(area->areaformat) {
                  case FORMAT_HMB:
                    if(area->areanumber and area->areanumber < 201) {
                      aa.basetype = "HUDSON";
                      aa.board = area->areanumber;
                      break;
                    }
                    continue;
                  case FORMAT_SDM:
                    if(not strblank(area->path)) {
                      aa.basetype = _fidomsgtype;
                      aa.setpath(area->path);
                      break;
                    }
                    continue;
                  case FORMAT_JAM:
                    if(not strblank(area->path)) {
                      aa.basetype = "JAM";
                      aa.setpath(area->path);
                      break;
                    }
                    continue;
                  case FORMAT_SQUISH:
                    if(not strblank(area->path)) {
                      aa.basetype = "SQUISH";
                      aa.setpath(area->path);
                      break;
                    }
                    continue;
                  case FORMAT_PCB:
                    aa.basetype = "PCBOARD";
                    aa.board = area->areanumber;
                    aa.setpath(area->path);
                    break;
                  case FORMAT_WC:
                    aa.basetype = "WILDCAT";
                    aa.board = area->areanumber;
                    aa.setpath(area->path);
                    break;
                  default:
                    if(ge_version >= 102)
                      continue;
                    if(area->areanumber and area->areanumber < 201) {
                      aa.basetype = "HUDSON";
                      aa.board = area->areanumber;
                    }
                    else if((area->options & SDM) and not strblank(area->path)) {
                      aa.basetype = _fidomsgtype;
                      aa.setpath(area->path);
                    }
                }

                aa.groupid = area->group + ((ge_version >= 120) ? 0x8001 : 0);
                if(_aka[area->pkt_origin].net)
                  aa.aka = _aka[area->pkt_origin];
                switch(area->areatype) {
                  case NETMAIL:
                    aa.type = GMB_NET;
                    aa.attr = attribsnet;
                    break;
                  case LOCAL:
                    aa.type = GMB_LOCAL;
                    aa.attr = attribslocal;
                    break;
                  case BADECHO:
                  case ECHOMAIL:
                  case PERSONAL:
                  default:
                    aa.type = GMB_ECHO;
                    aa.attr = attribsecho;
                }
                aa.setdesc(area->comment);
                aa.setechoid(area->name);
                aa.setorigin(area->originlinenr ? gesetup->originline[area->originlinenr-1] : area->originline);
                AddNewArea(aa);
              }
            }
            throw_free(area);
          }

          fclose(fp);
        }
      }
      else
        STD_PRINT("* Error: GEcho system file revision level " << sysrev << " is not supported - Skipping." << std::endl);
    }
    throw_free(gesetup);
  }
}


//  ------------------------------------------------------------------
