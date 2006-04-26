
//  ------------------------------------------------------------------
//  GoldED+
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999-2000 Alexander S. Aganichev
//  ------------------------------------------------------------------
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License as
//  published by the Free Software Foundation; either version 2 of the
//  License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston,
//  MA 02111-1307 USA
//  ------------------------------------------------------------------
//  $Id$
//  ------------------------------------------------------------------
//  Arealist configuration and other one-time stuff.
//  ------------------------------------------------------------------

#include <cerrno>
#include <fcntl.h>
#include <golded.h>
#include <gmoprot.h>

// FTS-0001 is FTS so leave it anyway :-)
#include <gmofido.h>
#ifndef GMB_NOEZY
#include <gmoezyc.h>
#endif
#if !defined(GMB_NOHUDS) || !defined(GMB_NOGOLD)
#include <gmohuds.h>
#endif
#ifndef GMB_NOJAM
#include <gmojamm.h>
#endif
#ifndef GMB_NOSQSH
#include <gmosqsh.h>
#endif
#ifndef GMB_NOPCB
#include <gmopcbd.h>
#endif
#ifndef GMB_NOWCAT
#include <gmowcat.h>
#endif
#ifndef GMB_NOXBBS
#include <gmoxbbs.h>
#endif
#ifndef GMB_NOSMB
#include <gmosmb.h>
#endif

//  ------------------------------------------------------------------
//  Arealist constructor

AreaList::AreaList() : idx() {

  item = idx.begin();
  *sortspec = NUL;

  for(uint i = 0; i < 16; i++)
    *alistselections[0] = NUL;
}


//  ------------------------------------------------------------------
//  Arealist destructor

AreaList::~AreaList() {

  Reset();
}


//  ------------------------------------------------------------------

void AreaList::SetDefaultMarks() {

  strcpy(alistselections[0], LNG->ArealistSelections1);

  for(uint i = 1; i < 16; i++)
    strcpy(alistselections[i], LNG->ArealistSelections2);
}


//  ------------------------------------------------------------------
//  Arealist deallocate and reset data

void AreaList::Reset() {

  ListScan.Reset();
  while(not idx.empty()) {
    delete idx.back();
    idx.pop_back();
  }
}


//  ------------------------------------------------------------------
//  Return a new'ed area of the specified format

Area* AreaList::NewArea(const char *basetype) {

  gmo_area* ap = NULL;
  if(streql(basetype, "SEPARATOR")) ap = new SepArea;
  else if(streql(basetype, "FTS1") or streql(basetype, "OPUS")) ap = new FidoArea;
  #ifndef GMB_NOEZY
  else if(streql(basetype, "EZYCOM")) ap = new EzycomArea;
  #endif
  #ifndef GMB_NOGOLD
  else if(streql(basetype, "GOLDBASE")) ap = new GoldArea;
  #endif
  #ifndef GMB_NOHUDS
  else if(streql(basetype, "HUDSON")) ap = new HudsArea;
  #endif
  #ifndef GMB_NOJAM
  else if(streql(basetype, "JAM")) ap = new JamArea;
  #endif
  #ifndef GMB_NOPCB
  else if(streql(basetype, "PCBOARD")) ap = new PcbArea;
  #endif
  #ifndef GMB_NOSQSH
  else if(streql(basetype, "SQUISH")) ap = new SquishArea;
  #endif
  #ifndef GMB_NOWCAT
  else if(streql(basetype, "WILDCAT")) ap = new WCatArea;
  #endif
  #ifndef GMB_NOXBBS
  else if(streql(basetype, "ADEPTXBBS")) ap = new XbbsArea;
  #endif
  #ifndef GMB_NOSMB
  else if(streql(basetype, "SMB")) ap = new SMBArea;
  #endif
  return new Area(ap);
}


//  ------------------------------------------------------------------
//  Write lastreads for the next session

void AreaList::WriteGoldLast()
{
  word GOLDLAST_VER = CUR_GOLDLAST_VER;
  ggoldlast entry;
  Path lst;

  strcpy(lst, AddPath(CFG->goldpath, CFG->goldlast));

  gfile fp(lst, "wb", CFG->sharemode);
  if (fp.isopen())
  {
    fp.SetvBuf(NULL, _IOFBF, 8192);
    fp.Fwrite(&GOLDLAST_VER, sizeof(word));
    fp.Fwrite(AL.alistselections, sizeof(AL.alistselections));

    for(area_iterator ap = idx.begin(); ap != idx.end(); ap++) {

      if((*ap)->isscanned and not (*ap)->isseparator()) {

        // Write fixed header
        entry.crcechoid    = strCrc32((*ap)->echoid(), false);
        entry.lastread     = (*ap)->lastread();
        entry.msgncount    = (*ap)->Msgn.Count();
        entry.unread       = (*ap)->unread;
        entry.marks        = (*ap)->marks;
        entry.flags        = 0;
        if((*ap)->isscanned)
          entry.flags |= 1;
        if((*ap)->isvalidchg)
          entry.flags |= 2;
        if((*ap)->isunreadchg)
          entry.flags |= 4;

        fp.Fwrite(&entry, sizeof(entry));

        // Write variable length extensions
        (*ap)->Mark.Save(fp);
        (*ap)->PMrk.Save(fp);
      }
    }
  }
}


//  ------------------------------------------------------------------
//  Read the lastreads from the last session

void AreaList::ReadGoldLast()
{
  word GOLDLAST_VER;
  ggoldlast entry;
  
  gfile fp(AddPath(CFG->goldpath, CFG->goldlast), "rb", CFG->sharemode);
  if (fp.isopen())
  {
    fp.SetvBuf(NULL, _IOFBF, 8192);
    fp.Fread(&GOLDLAST_VER, sizeof(word));

    if (GOLDLAST_VER != CUR_GOLDLAST_VER)
      return;

    fp.Fread(AL.alistselections, sizeof(AL.alistselections));

    while (fp.Fread(&entry, sizeof(entry)))
    {
      bool found = false;
      
      for(area_iterator ap = idx.begin(); ap != idx.end(); ap++) {
        if(strCrc32((*ap)->echoid(), false) == entry.crcechoid) {

          (*ap)->set_lastread(entry.lastread);
          (*ap)->Msgn.count  = entry.msgncount;
          (*ap)->unread      = entry.unread;
          (*ap)->marks       = entry.marks;
          (*ap)->isscanned   = make_bool(entry.flags & 1);
          (*ap)->isvalidchg  = make_bool(entry.flags & 2);
          (*ap)->UpdateAreadata();
          (*ap)->isunreadchg = make_bool(entry.flags & 4);

          (*ap)->Mark.Load(fp);
          (*ap)->PMrk.Load(fp);
          
          found = true;
          break;
        }
      }
      
      if (not found)
      {
        // skip stored message marks
        dword dw;
        fp.Fread(&dw, sizeof(dword));
        fp.Fseek(dw*sizeof(dword), SEEK_CUR);
        fp.Fread(&dw, sizeof(dword));
        fp.Fseek(dw*sizeof(dword), SEEK_CUR);
      }
    }
  }
}


//  ------------------------------------------------------------------
//  Write all areas to GOLDAREA.INC in AREADEF format

void AreaList::WriteAreaDef(const char* file)
{
  int tmp;
  Path path;
  char groupid[10], echoid[sizeof(Echo)+2];
  int maxechoid=0, maxdesc=0, maxgroupid=0, maxpath=0, maxaddr=0, maxattr=0;
  char desc[sizeof(Desc)+2], type[6], msgbase[9], addr[40], attr[150], origin[163];

  gfile fp(file, "wt", CFG->sharemode);
  if (fp.isopen())
  {
    area_iterator aa;
    for(aa = idx.begin(); aa != idx.end(); aa++) {
      tmp = strlen((*aa)->echoid());
      if(strchr((*aa)->echoid(), ' '))
        tmp += 2;
      maxechoid = MaxV(maxechoid, tmp);
      tmp = strlen((*aa)->desc())+2;
      maxdesc = MaxV(maxdesc, tmp);
      tmp = strlen((*aa)->path());
      if(strchr((*aa)->path(), ' '))
        tmp += 2;
      maxpath = MaxV(maxpath, tmp);
      if((*aa)->groupid() & 0x8000u)
        sprintf(groupid, "#%u", (*aa)->groupid()&0x7FFF);
      else if(g_isupper((*aa)->groupid()))
        *groupid = (char)(*aa)->groupid(), groupid[1] = NUL;
      else
        *groupid = '0', groupid[1] = NUL;
      tmp = strlen(groupid);
      maxgroupid = MaxV(maxgroupid, tmp);
      if(memcmp(&(*aa)->aka(), &CFG->aka[0], sizeof(Addr)))
        (*aa)->aka().make_string(addr);
      else
        strcpy(addr, ".");
      tmp = strlen(addr);
      maxaddr = MaxV(maxaddr, tmp);
      tmp = strlen(MakeAttrStr(attr, sizeof(attr), &(*aa)->attr()));
      maxattr = MaxV(maxattr, tmp+2);
    }

    for(aa = idx.begin(); aa != idx.end(); aa++) {
      sprintf(desc, "\"%s\"", (*aa)->desc());
      if((*aa)->groupid() & 0x8000u)
        sprintf(groupid, "#%u", (*aa)->groupid()&0x7FFF);
      else if(g_isupper((*aa)->groupid()))
        *groupid = (char)(*aa)->groupid(), groupid[1] = NUL;
      else
        *groupid = '0', groupid[1] = NUL;
      if((*aa)->isemail())
        strcpy(type, "EMail");
      else if((*aa)->isnewsgroup())
        strcpy(type, "News ");
      else if((*aa)->isnet())
        strcpy(type, "Net  ");
      else if((*aa)->isecho())
        strcpy(type, "Echo ");
      else if((*aa)->islocal())
        strcpy(type, "Local");
      #ifndef GMB_NOXBBS
      if(streql((*aa)->basetype(), "ADEPTXBBS"))
        strcpy(msgbase, "XBBS");
      else
      #endif
        strxcpy(msgbase, (*aa)->basetype(), sizeof(msgbase));
      if(strchr((*aa)->echoid(), ' '))
        sprintf(echoid, "\"%s\"", (*aa)->echoid());
      else
        strcpy(echoid, (*aa)->echoid());
      if ((*aa)->isseparator())
      {
        fp.Printf("AREASEP %-*s %-*s %*s %s\n",
          maxechoid,  echoid,
          maxdesc,    desc,
          maxgroupid, groupid,
          strtrim(type)
        );
      }
      else {
        if(strchr((*aa)->path(), ' '))
          sprintf(path, "\"%s\"", (*aa)->path());
        else
          strcpy(path, (*aa)->path());
        if(memcmp(&(*aa)->aka(), &CFG->aka[0], sizeof(Addr)))
          (*aa)->aka().make_string(addr);
        else
          strcpy(addr, ".");
        *attr = '(';  /*)*/
        MakeAttrStr(attr+1, sizeof(attr)-2, &(*aa)->attr());
        strcat(attr, /*(*/ ")");
        if((*aa)->originno())
          sprintf(origin, " \"%.*s\"", (int)sizeof(origin)-4, CFG->origin[(*aa)->originno()].c_str());
        else
          *origin = NUL;
        fp.Printf("AREADEF %-*s %-*s %*s %s %s %-*s %-*s %-*s%s\n",
          maxechoid,  echoid,
          maxdesc,    desc,
          maxgroupid, groupid,
          type,
          msgbase,
          maxpath, path,
          maxaddr, addr,
          maxattr, attr,
          origin
        );
      }
    }
  }
}


//  ------------------------------------------------------------------

void SetAreaDesc(char* echoid, char* desc) {

  AL.SetAreaDesc(echoid, desc);
}


//  ------------------------------------------------------------------

void AreaList::SetAreaDesc(char* echoid, char* desc) {

  for(area_iterator ap = idx.begin(); ap != idx.end(); ap++) {
    if(strieql(echoid, (*ap)->echoid())) {
      (*ap)->set_desc(desc);
      break;
    }
  }
}


//  ------------------------------------------------------------------

#ifndef GMB_NOPCB
void PcbAdjustArea(uint rec, const char* msgfile) {

  for(uint n=0; n<AL.size(); n++) {
    Area* a = AL[n];
    if(streql(a->basetype(), "PCBOARD")) {
      if((a->board() == rec) and (*a->path() == NUL)) {
        a->set_path(msgfile);
        break;
      }
      else if(strieql(a->path(), msgfile)) {
        a->set_board(rec);
        break;
      }
    }
  }
}
#endif


//  ------------------------------------------------------------------

