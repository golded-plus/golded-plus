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
//  Read areas from FastEcho 1.42 (config revision level 6)
//  ------------------------------------------------------------------

#include <cstring>
#include <cstdlib>
#include <gfilutil.h>
#include <gmemdbg.h>
#undef GCFG_NOFE
#include <gedacfg.h>
#include <gs_fech6.h>


//  ------------------------------------------------------------------
//  Read FASTECHO.CFG

void gareafile::ReadFastecho142(int fh) {

  AreaCfg aa;

  CONFIG6* cfg = (CONFIG6*)throw_calloc(1, sizeof(CONFIG6));
  FeArea6* area = (FeArea6*)throw_calloc(1, sizeof(FeArea6));

  // Read main config record
  read(fh, cfg, sizeof(CONFIG6));

  // Get Hudson msgbase path
  CfgHudsonpath(cfg->MsgBase);

  // Setup aka and origin lists
  SysAddress* aka = (SysAddress*)throw_calloc(cfg->AkaCnt, sizeof(SysAddress));
  OriginLines* origin = (OriginLines*)throw_calloc(cfg->OriginCnt, sizeof(OriginLines));

  // Process extended headers
  ulong offset = 0;
  while(offset < cfg->offset) {
    ExtensionHeader ehdr;
    read(fh, &ehdr, sizeof(ExtensionHeader));
    offset += sizeof(ExtensionHeader);
    switch(ehdr.type) {
      case EH_AKAS:
        read(fh, aka, cfg->AkaCnt*sizeof(SysAddress));
        break;
      case EH_ORIGINS:
        read(fh, origin, cfg->OriginCnt*sizeof(OriginLines));
        break;
      default:
        lseek(fh, ehdr.offset, SEEK_CUR);
    }
    offset += ehdr.offset;
  }

  // Skip node records
  lseek(fh, (long)(cfg->NodeCnt)*(long)cfg->NodeRecSize, SEEK_CUR);

  // The *.MSG netmail area
  aa.reset();
  aa.aka = aka[0].main;
  aa.type = GMB_NET;
  aa.attr = attribsnet;
  aa.msgbase = fidomsgtype;
  aa.setpath(cfg->NetMPath);
  aa.setdesc("FastEcho Netmail");
  aa.setautoid("NETMAIL");
  AddNewArea(aa);

  // All the echomail areas
  for(int n=0; n<cfg->AreaCnt; n++) {
    read(fh, area, sizeof(FeArea6));
    if(area->board != AREA_DELETED) {

      aa.reset();

      aa.aka = aka[area->info.aka].main;

      switch(area->flags.storage) {
        case QBBS:
          aa.msgbase = GMB_HUDSON;
          aa.board = area->board;
          break;
        case FIDO:
          aa.msgbase = fidomsgtype;
          aa.setpath(area->path);
          break;
        case SQUISH:
          aa.msgbase = GMB_SQUISH;
          aa.setpath(area->path);
          break;
        case JAM:
          aa.msgbase = GMB_JAM;
          aa.setpath(area->path);
          break;
        default:
          // Passthrough or unknown
          continue;
      }

      switch(area->flags.atype) {
        case AREA_BADMAILBOARD:
        case AREA_DUPEBOARD:
        case AREA_ECHOMAIL:
          aa.type = GMB_ECHO;
          aa.attr = attribsecho;
          break;
        case AREA_NETMAIL:
          aa.type = GMB_NET;
          aa.attr = attribsnet;
          break;
        case AREA_LOCAL:
          aa.type = GMB_LOCAL;
          aa.attr = attribslocal;
          break;
        default:
          // Unknown type
          continue;

      }
      aa.setdesc(area->desc);
      aa.setechoid(area->name);
      if(area->flags.origin < cfg->OriginCnt)
        aa.setorigin(origin[area->flags.origin].line);
      aa.groupid = (char)('A' + area->info.group);
      if(aa.groupid > 'Z')
        aa.groupid = 0x8000u + (area->info.group-25);

      AddNewArea(aa);
    }
  }

  throw_free(origin);
  throw_free(aka);
  throw_free(area);
  throw_free(cfg);
}


//  ------------------------------------------------------------------
//  Read FASTECHO.CFG

void gareafile::ReadFastecho(char* tag) {

  char* ptr;
  Path file;
  char options[80];
  int fh;
  short revision;

  *file = NUL;
  strcpy(options, tag);
  ptr = strtok(tag, " \t");
  while(ptr) {
    if(*ptr != '-') {
      strcpy(file, ptr);
      break;
    }
    ptr = strtok(NULL, " \t");
  }
  if(*file == NUL) {
    ptr = getenv("FASTECHO");
    if(ptr)
      AddBackslash(strcpy(file, ptr));
  }
  if(*file == NUL)
    strcpy(file, areapath);

  if(not fexist(file)) {
    AddBackslash(file);
    strcat(file, "fastecho.cfg");
  }

  fh = sopen(file, O_RDONLY|O_BINARY, sharemode, S_STDRD);
  if(fh != -1) {

    if(not quiet)
      std::cout << "* Reading " << file << std::endl;

    read(fh, &revision, sizeof(revision));
    lseek(fh, 0L, SEEK_SET);  // rewind

    if(revision == 4)
      ReadFastecho11x(fh);
    else if(revision == 5)
      ReadFastecho141(fh);
    else if(revision == 6)
      ReadFastecho142(fh);
    else
      std::cout << "* Error: FastEcho system file revision level " << revision << " is not supported - Skipping." << std::endl;

    close(fh);
  }
}


//  ------------------------------------------------------------------
