
//  ------------------------------------------------------------------
//  The Goldware Library. Copyright (C) Odinn Sorensen.
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
//  License along with this library; if not, write to the Free
//  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//  ------------------------------------------------------------------
//  $Id$
//  ------------------------------------------------------------------
//  Read areas from FastEcho 1.41 (config revision level 5)
//  ------------------------------------------------------------------

#include <cstring>
#include <cstdlib>
#include <gfilutil.h>
#include <gmemdbg.h>
#undef GCFG_NOFE
#include <gedacfg.h>
#include <gs_fech5.h>


//  ------------------------------------------------------------------
//  Read FASTECHO.CFG

void gareafile::ReadFastecho141(int fh) {

  AreaCfg aa;

  CONFIG5* cfg = (CONFIG5*)throw_calloc(1, sizeof(CONFIG5));
  FeArea5* area = (FeArea5*)throw_calloc(1, sizeof(FeArea5));

  // Read main config record
  read(fh, cfg, sizeof(CONFIG5));

  // Get usernames
  //for(int u=0; u<11; u++)
    //CfgUsername(cfg->sysops[u].name);

  // Get Hudson msgbase path
  if(*hudsonpath == NUL)
    PathCopy(hudsonpath, MapPath(cfg->MsgBase));

  // Setup aka list
  SysAddress* aka = (SysAddress*)throw_calloc(cfg->AkaCnt, sizeof(SysAddress));
  for(int c=0; c<11; c++)
    aka[c].main = cfg->oldakas[c].main;

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
      default:
        lseek(fh, ehdr.offset, SEEK_CUR);
    }
    offset += ehdr.offset;
  }

  // Skip node records
  lseek(fh, (long)(cfg->NodeCnt)*(long)sizeof(FeNode5), SEEK_CUR);

  // The *.MSG netmail area
  aa.reset();
  aa.aka = CAST(ftn_addr, aka[0].main);
  aa.type = GMB_NET;
  aa.attr = attribsnet;
  aa.msgbase = fidomsgtype;
  aa.setpath(cfg->NetMPath);
  aa.setdesc("FastEcho Netmail");
  aa.setautoid("NETMAIL");
  AddNewArea(aa);

  // All the echomail areas
  for(int n=0; n<cfg->AreaCnt; n++) {
    read(fh, area, sizeof(FeArea5));
    if(area->board != AREA_DELETED) {
      aa.reset();
      aa.aka = CAST(ftn_addr, aka[area->flags.aka].main);
      switch(area->flags.type) {
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
        case PT_BOARD:
        default:
          // Passthrough or unknown
          continue;
      }

      switch(area->type) {
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
        case AREA_BADMAILBOARD:
          aa.type = GMB_ECHO;
          aa.attr = attribsecho;
          break;
        case AREA_DUPEBOARD:
          aa.type = GMB_ECHO;
          aa.attr = attribsecho;
          break;
        default:
          // Unknown type
          continue;

      }
      aa.setdesc(area->desc);
      aa.setechoid(area->name);
      aa.setorigin(cfg->OriginLine[area->flags.origin]);
      aa.groupid = (char)('A' + area->flags.group);
      AddNewArea(aa);
    }
  }

  throw_free(aka);
  throw_free(area);
  throw_free(cfg);
}


//  ------------------------------------------------------------------

