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
//  Read areas from PCBoard.
//  ------------------------------------------------------------------

#include <cstdlib>
#include <gmemdbg.h>
#include <gfile.h>
#include <gstrall.h>
#undef GCFG_NOPCB
#include <gedacfg.h>
#include <gs_pcb.h>


//  ------------------------------------------------------------------

void gareafile::ReadPCBoard(char* tag) {

  AreaCfg aa;
  Path _path;
  char _options[80];
  Path _usersidxpath;
  Path _userspath;
  Path _cnamespath;
  Path _fidopath;

  PcbFidoArea* areap = NULL;
  PcbFidoDirectories* dirp = NULL;
  PcbFidoAddress* akap = NULL;
  int akanumrecs = 0;
  int numareas = 0;

  PcbDirectories* dir3 = NULL;
  PcbAreasDat* area3 = NULL;
  PcbAkasDat* aka3 = NULL;
  PcbOriginsDat* origin3 = NULL;

  word fido_version = 0;

  *_fidopath = NUL;
  strcpy(_path, pcboardpath);
  strcpy(_options, tag);

  char* ptr = strtok(tag, " \t");
  while(ptr) {
    if(*ptr != '-') {
      AddBackslash(strcpy(_path, ptr));
      break;
    }
    ptr = strtok(NULL, " \t");
  }
  if(*_path == NUL) {
    ptr = getenv("PCBOARD");
    if(ptr)
      AddBackslash(strcpy(_path, ptr));
  }
  if(*_path == NUL)
    strcpy(_path, areapath);

  CfgPcboardpath(_path);

  gfile fp;
  const char* _file = AddPath(_path, "pcboard.dat");
  fp.fopen(_file, "rt");
  if(fp.isopen()) {

    if(not quiet)
      std::cout << "* Reading " << _file << std::endl;

    int _line = 0;

    char _buf[256];
    while(fp.fgets(_buf, sizeof(_buf))) {
      _line++;
      switch(_line) {
        case 28:  // Location of User INDEX Files
          strxcpy(_usersidxpath, strbtrim(_buf), GMAXPATH);
          break;
        case 29:  // Name/Location of USERS File
          strxcpy(_userspath, strbtrim(_buf), GMAXPATH);
          break;
        case 31:  // Name/Location of CNAMES File
          strxcpy(_cnamespath, strbtrim(_buf), GMAXPATH);
          break;
        case 324: // Path to FidoNet config files
          strxcpy(_fidopath, strbtrim(_buf), GMAXPATH);
          break;
      }
    }

    fp.fclose();

    if(*_fidopath) {
      const char* _file = AddPath(_fidopath, "pcbfido.cfg");
      fp.fopen(_file, "rb");
      if(fp.isopen()) {

        if(not quiet)
          std::cout << "* Reading " << _file << std::endl;

        // Get configuration file version
        fp.fread(&fido_version, 2);

        if(fido_version == 2) {

          word numrecs = 0;

          // Get areas
          fp.fread(&numrecs, 2);
          numareas = numrecs;
          areap = (PcbFidoArea*)throw_calloc(1+numrecs, sizeof(PcbFidoArea));
          fp.fread(areap, sizeof(PcbFidoArea), numrecs);

          // Skip archivers
          fp.fseek(sizeof(PcbFidoArchivers), SEEK_CUR);

          // Get directories
          dirp = (PcbFidoDirectories*)throw_calloc(1, sizeof(PcbFidoDirectories));
          fp.fread(dirp, sizeof(PcbFidoDirectories));

          // Skip EMSI profile
          fp.fseek(sizeof(PcbFidoEmsiData), SEEK_CUR);

          // Get akas
          fp.fread(&numrecs, 2);
          akanumrecs = numrecs;
          akap = (PcbFidoAddress*)throw_calloc(1+numrecs, sizeof(PcbFidoAddress));
          fp.fread(akap, sizeof(PcbFidoAddress), numrecs);
          int akano = 0;
          while(akano < numrecs) {
            CfgAddress(akap[akano].nodestr);
            akano++;
          }
        }
        else if(fido_version == 3) {

          dir3 = (PcbDirectories*)throw_calloc(1, sizeof(PcbDirectories));
          fp.fread(dir3, sizeof(PcbDirectories));
          fp.fclose();

          _file = AddPath(_fidopath, "areas.dat");
          fp.fopen(_file, "rb");
          if(fp.isopen()) {
            if(not quiet)
              std::cout << "* Reading " << _file << std::endl;
            word cfgver = 0;
            fp.fread(&cfgver, 2);
            if(cfgver == 3) {
              word numrecs = (word)(fp.filelength() / sizeof(PcbAreasDat));
              area3 = (PcbAreasDat*)throw_calloc(1+numrecs, sizeof(PcbAreasDat));
              fp.fread(area3, sizeof(PcbAreasDat), numrecs);
              numareas = numrecs;
            }
            fp.fclose();
          }

          _file = AddPath(_fidopath, "akas.dat");
          fp.fopen(_file, "rb");
          if(fp.isopen()) {
            if(not quiet)
              std::cout << "* Reading " << _file << std::endl;
            word cfgver = 0;
            fp.fread(&cfgver, 2);
            if(cfgver == 3) {
              word numrecs = (word)(fp.filelength() / sizeof(PcbAkasDat));
              akanumrecs = numrecs;
              aka3 = (PcbAkasDat*)throw_calloc(1+numrecs, sizeof(PcbAkasDat));
              fp.fread(aka3, sizeof(PcbAkasDat), numrecs);
              int akano = 0;
              while(akano < numrecs) {
                char abuf[40];
                CfgAddress(aka3[akano].addr.make_string(abuf));
                akano++;
              }
            }
            fp.fclose();
          }

          _file = AddPath(_fidopath, "origins.dat");
          fp.fopen(_file, "rb");
          if(fp.isopen()) {
            if(not quiet)
              std::cout << "* Reading " << _file << std::endl;
            word cfgver = 0;
            fp.fread(&cfgver, 2);
            if(cfgver == 3) {
              word numrecs = (word)(fp.filelength() / sizeof(PcbOriginsDat));
              origin3 = (PcbOriginsDat*)throw_calloc(1+numrecs, sizeof(PcbOriginsDat));
              fp.fread(origin3, sizeof(PcbOriginsDat), numrecs);
            }
            fp.fclose();
          }
        }

        fp.fclose();
      }

      Path netmailpath;
      *netmailpath = NUL;
      if(dirp)
        strcpy(netmailpath, dirp->outgoing_msg);
      else if(dir3)
        strcpy(netmailpath, dir3->outgoing_msg);
      if(*netmailpath) {
        aa.reset();
        aa.basetype = fidomsgtype;
        aa.type = GMB_NET;
        aa.attr = attribsnet;
        if(fido_version == 2)
          aa.aka.reset(akap[0].nodestr);
        else if(fido_version == 3) {
          for(int n=0; n<akanumrecs; n++) {
            if(aka3[n].primary_address) {
              aa.aka = aka3[n].addr;
              break;
            }
          }
        }
        aa.setpath(netmailpath);
        aa.setdesc("PCBoard Netmail (*.MSG)");
        aa.setautoid("NETMAIL.MSG");
        AddNewArea(aa);
      }
    }

    _file = AddPath(_cnamespath, ".@@@");
    fp.fopen(_file, "rb");
    if(fp.isopen()) {

      if(not quiet)
        std::cout << "* Reading " << _file << std::endl;

      gfile fp2;
      _file = AddPath(_cnamespath, ".add");
      fp2.fopen(_file, "rb");
      if(fp2.isopen()) {

        if(not quiet)
          std::cout << "* Reading " << _file << std::endl;

        word _recsize = 0;
        fp.fread(&_recsize, 2);

        int confno = 0;
        PcbConf* _cnames = (PcbConf*)throw_calloc(1, _recsize);
        PcbAddConf* _cnamesadd = (PcbAddConf*)throw_calloc(1, sizeof(PcbAddConf));
        while(fp.fread(_cnames, _recsize) == 1) {
          fp2.fread(_cnamesadd, sizeof(PcbAddConf));
          if(*_cnames->name and *_cnames->msgfile) {
            aa.reset();
            aa.basetype = "PCBOARD";
            switch(_cnamesadd->conftype) {
              case PCBCONFTYPE_EMAIL:
              case PCBCONFTYPE_USENETMOD:
                aa.type = GMB_EMAIL;
                aa.attr = attribsemail;
                break;
              case PCBCONFTYPE_USENETJUNK:
              case PCBCONFTYPE_USENETPUB:
                aa.type = GMB_NEWSGROUP;
                aa.attr = attribsnews;
                break;
              case PCBCONFTYPE_FIDOCONF:
                aa.type = GMB_ECHO;
                aa.attr = attribsecho;
                break;
              case PCBCONFTYPE_NORMAL:
              default:
                if(_cnames->echomail) {
                  aa.type = GMB_ECHO;
                  aa.attr = attribsecho;
                }
                else {
                  aa.type = GMB_LOCAL;
                  aa.attr = attribslocal;
                }
            }
            if(_cnamesadd->readonly)
              aa.attr.r_o1();
            if(_cnames->privmsgs)
              aa.attr.pvt1();
            aa.board = confno;
            aa.setpath(_cnames->msgfile);
            char namebuf[61];
            strcpy(stpcpy(namebuf, _cnames->name), _cnamesadd->name2);
            aa.setdesc(namebuf);
            strchg(namebuf, ' ', '_');
            aa.setechoid(namebuf);
            int areano = 0;
            while(areano < numareas) {
              if(fido_version == 2) {
                if(areap[areano].pcbconference == confno) {
                  strchg(areap[areano].areaname, ' ', '_');
                  aa.setechoid(areap[areano].areaname);
                  aa.aka.set(areap[areano].defaultaka);
                  aa.setorigin(areap[areano].origin);
                  aa.setpath(areap[areano].messages);
                  break;
                }
              }
              else if(fido_version == 3) {
                if(area3[areano].confno == confno) {
                  aa.setechoid(area3[areano].areatag);
                  aa.aka = aka3[area3[areano].aka].addr;
                  aa.setorigin(origin3[area3[areano].origin].origin);
                  break;
                }
              }
              areano++;
            }
            if(strieql(aa.echoid, "NETMAIL")) {
              aa.type = GMB_NET;
              aa.attr = attribsnet;
            }
            AddNewArea(aa);
          }
          confno++;
        }

        throw_free(_cnamesadd);
        throw_free(_cnames);

        fp2.fclose();
      }

      fp.fclose();
    }
  }

  throw_free(origin3);
  throw_free(area3);
  throw_free(dir3);
  throw_free(aka3);
  throw_free(areap);
  throw_free(dirp);
  throw_free(akap);
}


//  ------------------------------------------------------------------
