//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999-2000 Alexander S. Aganichev
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
//  GoldED areafile helper classes.
//  ------------------------------------------------------------------

#include <cstdlib>
#include <gmemdbg.h>
#include <gstrall.h>
#include <gedacfg.h>


//  ------------------------------------------------------------------

gareafile::gareafile() {

  *pathprefix = NUL;

  quiet = true;

  sharemode = 0;
  fidomsgtype = 0;
  ra2usersbbs = 0;
  squishuserno = 0;

  areapath = NULL;
  adeptxbbspath = NULL;
  jampath = NULL;
  squishuserpath = NULL;
  hudsonpath = NULL;
  goldbasepath = NULL;
  pcboardpath = NULL;
  ezycom_msgbasepath = NULL;
  ezycom_userbasepath = NULL;
  fidolastread = NULL;
}


//  ------------------------------------------------------------------

void gareafile::adjustpath(char* path) {

  Path tmp;
  if(path[1] != ':') {
    strcpy(tmp, path);
    strcpy(stpcpy(path, pathprefix), tmp);
  }
}


//  ------------------------------------------------------------------

int AreaCfg::autoid = 0;


//  ------------------------------------------------------------------
//  Set area origin

const char* AreaCfg::setorigin(const char* s) {

  origin = s;
  AreaCfgBase::setorigin(origin);
  return origin.c_str();
}


//  ------------------------------------------------------------------

EchoListClass::EchoListClass() {

  echos = 0;
  descs = 0;
  echolist = NULL;
  desclist = NULL;
}


//  ------------------------------------------------------------------

EchoListClass::~EchoListClass() {

  FreeAll();
}


//  ------------------------------------------------------------------

void EchoListClass::FreeAll() {

  int n;

  for(n=0; n<echos; n++)
    throw_release(echolist[n]);
  throw_xrelease(echolist);
  echos = 0;

  for(n=0; n<descs; n++)
    throw_release(echolist[n]);
  throw_xrelease(desclist);
  descs = 0;
}


//  ------------------------------------------------------------------

int CmpEchos(const EchoList** a, const EchoList** b) {

  int cmp;

  if((cmp = stricmp((*a)->path, (*b)->path)) != 0)
    return(cmp);

  return(0);
}


//  ------------------------------------------------------------------

void EchoListClass::SortEchos() {

  if(echolist)
    qsort(echolist, echos, sizeof(EchoList*), (StdCmpCP)CmpEchos);
}


//  ------------------------------------------------------------------

void EchoListClass::AddDesc(char* echoid, char* desc) {

  desclist = (DescList**)throw_reallox(desclist, (descs+1), sizeof(DescList*), 50);
  desclist[descs] = (DescList*)throw_calloc(1, sizeof(DescList));
  strxcpy(desclist[descs]->echoid, echoid, sizeof(Echo));
  strxcpy(desclist[descs]->desc, desc, sizeof(Desc));
  descs++;
}


//  ------------------------------------------------------------------

int EchoListClass::FindDesc(char* echoid, char** desc) {

  if(desclist) {
    for(int n=0; n<descs; n++) {
      if(strieql(desclist[n]->echoid, echoid)) {
        *desc = desclist[n]->desc;
        return 1;
      }
    }
  }
  return 0;
}


//  ------------------------------------------------------------------

void EchoListClass::AddEcho(char* echoid, char* path, char* desc) {

  echolist = (EchoList**)throw_reallox(echolist, (echos+1), sizeof(EchoList*), 50);
  echolist[echos] = (EchoList*)throw_calloc(1, sizeof(EchoList));
  strxcpy(echolist[echos]->echoid, echoid, sizeof(Echo));
  if(atoi(path))            // QBBS
    sprintf(echolist[echos]->path, "%u", atoi(path));
  else if(*path == '$')     // Squish
    MapPath(StripBackslash(strxcpy(echolist[echos]->path, path, sizeof(Path)))+1);
  else if(*path == '!')     // JAM
    MapPath(StripBackslash(strxcpy(echolist[echos]->path, path, sizeof(Path)))+1);
  else                      // *.MSG
    MapPath(AddBackslash(strxcpy(echolist[echos]->path, path, sizeof(Path))));
  if(desc) {
    desc = strskip_wht(desc);
    if(strlen(desc) >= sizeof(Desc)) {
      desc[sizeof(Desc)-1] = NUL;
      strtrim(desc);
    }
  }
  else
    FindDesc(echoid, &desc);
  if(desc)
    strxcpy(echolist[echos]->desc, desc, sizeof(Desc));
  echos++;
}


//  ------------------------------------------------------------------

int EchoListClass::FindEcho(char* echoid, char* path, char* desc) {

  EchoList echokey;
  EchoList* eptr;
  EchoList* iptr=&echokey;

  if(echos) {
    memset(iptr, 0, sizeof(EchoList));
    strxcpy(echokey.path, path, sizeof(Path));
    for(int n=0; n<echos; n++) {
      eptr = echolist[n];
      if(CmpEchos((const EchoList**)&iptr, (const EchoList**)&eptr) == 0) {  // Found it
        strcpy(echoid, eptr->echoid);
        if(strblank(desc))
          strcpy(desc, eptr->desc);
        return(1);
      }
    }
  }
  return 0;
}


//  ------------------------------------------------------------------

int EchoListClass::GetEcho(int n, char** echoid, char** path, char** desc) {

  if(echos) {
    *echoid = echolist[n]->echoid;
    *path = echolist[n]->path;
    *desc = echolist[n]->desc;
    return(1);
  }
  return(0);
}


//  ------------------------------------------------------------------
//  Read AREAS.BBS (any flavor!) and store echoid, path and desc.

void gareafile::GetAreasBBS(char* name, char* origin, char* options) {

  FILE* fp;
  char buf[256];
  Path areafile;
  char* ptr;
  char* echoid;
  char* path;
  char* desc;
  char* orig;

  strcpy(areafile, name);
  MakePathname(areafile, areapath, areafile);

  fp = fsopen(areafile, "rt", sharemode);
  if(fp) {
    setvbuf(fp, NULL, _IOFBF, 8192);

    if(not quiet)
      cout << "* Reading " << areafile << endl;

    bool firstline = true;

    while(fgets(buf, 255, fp)) {
      ptr = strskip_wht(buf);
      if(*ptr != ';' and *ptr != '%' and *ptr != '-' and *ptr != '#' and *ptr != '@' and strnicmp(ptr, "P ", 2) and strnicmp(ptr, "PASSTHRU", 8) and *ptr) {

        // Check for comment
        desc = strchr(ptr, ';');
        if(desc) {
          *desc++ = NUL;
          strbtrim(desc);
        }

        if (firstline) {
          // Check for origin
          orig = strchr(ptr+1, '!');
          if(orig) {
            *orig = NUL;
            strcpy(origin, ptr);
            continue;
          }
        }

        firstline = false;

        // Get echoid and path/boardno
        path = ptr;
        ptr = strskip_txt(ptr);
        if(*ptr) {
          *ptr++ = NUL;
          ptr = strskip_wht(ptr);
          if(*ptr) {
            echoid = ptr;
            ptr = strskip_txt(ptr);
            *ptr++ = NUL;
            ptr = strskip_wht(ptr);
            if(strnieql(ptr, "JAM", 3))
              strins("!", path, 0);
            echolist.AddEcho(echoid, path, desc);
          }
        }
      }
    }
    fclose(fp);
  }
}


//  ------------------------------------------------------------------

bool gareafile::ReadAreafile(word crc, char* parameters) {

  const word CRC_ADEPTXBBS     = 0x61F5;
  const word CRC_AREAS_BBS     = 0xBCEC;
  const word CRC_AREASBBS      = 0xF77C;
  const word CRC_CRASHMAIL     = 0x7551;
  const word CRC_DBRIDGE       = 0xD365;
  const word CRC_DUTCHIE       = 0x0B08;
  const word CRC_D_BRIDGE      = 0x48DA;
  const word CRC_EZYCOM        = 0xC81B;
  const word CRC_FASTECHO      = 0xF2F0;
  const word CRC_FE_ABS        = 0x8007;
  const word CRC_FIDOCONFIG    = 0x1341;
  const word CRC_FIDOPCB       = 0x842E;
  const word CRC_FMAIL         = 0xC534;
  const word CRC_FRONTDOOR     = 0x8A35;
  const word CRC_GECHO         = 0x5AEC;
  const word CRC_IMAIL         = 0xE905;
  const word CRC_INTERMAIL     = 0x08E9;
  const word CRC_LORABBS       = 0x04B7;
  const word CRC_MAXIMUS       = 0xA43B;
  const word CRC_ME2           = 0xDC5B;
  const word CRC_OPUS          = 0x1254;
  const word CRC_PARTOSS       = 0x15B7;
  const word CRC_PCBOARD       = 0x84EC;
  const word CRC_PORTAL        = 0x7747;
  const word CRC_PROBOARD      = 0xFFC9;
  const word CRC_QECHO         = 0xAB2F;
  const word CRC_QFRONT        = 0x3320;
  const word CRC_QUICKBBS      = 0x21A4;
  const word CRC_RAECHO        = 0x701F;
  const word CRC_RA_ECHO       = 0x4FDF;
  const word CRC_REMOTEACCESS  = 0xECD0;
  const word CRC_SQUISH        = 0xFCF6;
  const word CRC_SUPERBBS      = 0x497F;
  const word CRC_TERMAIL       = 0x147A;
  const word CRC_TIMED         = 0xE977;
  const word CRC_TMAIL         = 0xE837;
  const word CRC_TOSSCAN       = 0x43DD;
  const word CRC_WATERGATE     = 0x3ADB;
  const word CRC_WMAIL         = 0xB167;
  const word CRC_XMAIL         = 0x9D56;

  switch(crc) {
#ifndef GCFG_NOXBBS
    case CRC_ADEPTXBBS:     ReadAdeptXBBS(parameters);     break;
#endif
    case CRC_AREAS_BBS:
    case CRC_AREASBBS:      ReadAreasBBS(parameters);      break;
#ifndef GCFG_NOCMAIL
    case CRC_CRASHMAIL:     ReadCrashmail(parameters);     break;
#endif
#ifndef GCFG_NODB
    case CRC_D_BRIDGE:
    case CRC_DBRIDGE:       ReadDBridge(parameters);       break;
#endif
#ifndef GCFG_NODUTCHIE
    case CRC_DUTCHIE:       ReadDutchie(parameters);       break;
#endif
#ifndef GCFG_NOEZY
    case CRC_EZYCOM:        ReadEzycom(parameters);        break;
#endif
#ifndef GCFG_NOFE
    case CRC_FE_ABS:
    case CRC_FASTECHO:      ReadFastecho(parameters);      break;
#endif
#ifndef GCFG_NOFIDOCONF
    case CRC_FIDOCONFIG:    ReadHPT(parameters);           break;
#endif
#ifndef GCFG_NOFIDOPCB
    case CRC_FIDOPCB:       ReadFidoPCB(parameters);       break;
#endif
#ifndef GCFG_NOFMAIL
    case CRC_FMAIL:         ReadFMail(parameters);         break;
#endif
#ifndef GCFG_NOFD
    case CRC_FRONTDOOR:     ReadFrontDoor(parameters);     break;
#endif
#ifndef GCFG_NOGECHO
    case CRC_GECHO:         ReadGEcho(parameters);         break;
#endif
#ifndef GCFG_NOIMAIL
    case CRC_IMAIL:         ReadIMail(parameters);         break;
#endif
#ifndef GCFG_NOINTERMAIL
    case CRC_INTERMAIL:     ReadInterMail(parameters);     break;
#endif
#ifndef GCFG_NOLORA
    case CRC_LORABBS:       ReadLoraBBS(parameters);       break;
#endif
#ifndef GCFG_NOMAXIMUS
    case CRC_MAXIMUS:       ReadMaximus(parameters);       break;
#endif
#ifndef GCFG_NOME2
    case CRC_ME2:           ReadME2(parameters);           break;
#endif
#ifndef GCFG_NOOPUS
    case CRC_OPUS:          ReadOpus(parameters);          break;
#endif
#ifndef GCFG_NOPCB
    case CRC_PCBOARD:       ReadPCBoard(parameters);       break;
#endif
#ifndef GCFG_NOPORTAL
    case CRC_PORTAL:        ReadPortal(parameters);        break;
#endif
#ifndef GCFG_NOPROBOARD
    case CRC_PROBOARD:      ReadProBoard(parameters);      break;
#endif
#ifndef GCFG_NOQECHO
    case CRC_QECHO:         ReadQEcho(parameters);         break;
#endif
#ifndef GCFG_NOQFRONT
    case CRC_QFRONT:        ReadQFront(parameters);        break;
#endif
#ifndef GCFG_NOQBBS
    case CRC_QUICKBBS:      ReadQuickBBS(parameters);      break;
#endif
#ifndef GCFG_NORAECHO
    case CRC_RA_ECHO:
    case CRC_RAECHO:        ReadRaEcho(parameters);        break;
#endif
#ifndef GCFG_NORA
    case CRC_REMOTEACCESS:  ReadRemoteAccess(parameters);  break;
#endif
#if !defined(GCFG_NOSQSH) || !defined(GCFG_NOPARTOSS)
    case CRC_PARTOSS:
    case CRC_SQUISH:        ReadSquish(parameters);        break;
#endif
#ifndef GCFG_NOSBBS
    case CRC_SUPERBBS:      ReadSuperBBS(parameters);      break;
#endif
#ifndef GCFG_NOTERMAIL
    case CRC_TMAIL:
    case CRC_TERMAIL:       ReadTmail(parameters);         break;
#endif
#ifndef GCFG_NOTIMED
    case CRC_TIMED:         ReadTimed(parameters);         break;
#endif
#ifndef GCFG_NOTOSSCAN
    case CRC_TOSSCAN:       ReadTosScan(parameters);       break;
#endif
#ifndef GCFG_NOWATERGATE
    case CRC_WATERGATE:     ReadWtrGte(parameters);        break;
#endif
#ifndef GCFG_NOWMAIL
    case CRC_WMAIL:         ReadWMail(parameters);         break;
#endif
#ifndef GCFG_NOXMAIL
    case CRC_XMAIL:         ReadXMail(parameters);         break;
#endif
    default:                return false;
  }

  return true;
}


//  ------------------------------------------------------------------

AreaCfg& AreaCfg::operator=(const AreaCfg&) {

  return *this;
}


//  ------------------------------------------------------------------
