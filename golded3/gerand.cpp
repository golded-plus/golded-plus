
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
//  Random system and control info handling.
//  ------------------------------------------------------------------

#include <golded.h>


//  ------------------------------------------------------------------

char* GetRandomLine(char* __buf, size_t __bufsize, const char* __file) {
  
  Path indexfile;

  replaceextension(indexfile, __file, ".sdx");

  FILE* fp = fsopen(AddPath(CFG->goldpath, __file), "rb", CFG->sharemode);
  if(fp) {

    // Check if index exists or if it is older than the textfile
    const char* idxfile = AddPath(CFG->goldpath, indexfile);
    int idxexist = fexist(idxfile);
    if(idxexist)
      if(FiletimeCmp(__file, idxfile) > 0)
        idxexist = false;

    FILE* fpi = fsopen(idxfile, idxexist ? "rb" : "wb+", CFG->sharemode);
    if(fpi) {

      // Create index if one was missing
      if(not idxexist) {
        setvbuf(fp, NULL, _IOFBF, 32000);
        setvbuf(fpi, NULL, _IOFBF, 16000);
        long fpos = 0;
        char buf[512];
        while(fgets(buf, sizeof(buf), fp)) {
          fwrite(&fpos, sizeof(long), 1, fpi);
          fpos += strlen(buf);
        }
      }

      // Get random line if there is at least one
      int _lines = (int)(fsize(fpi)/sizeof(long));
      if(_lines > 0) {

        // Select a random line
        long _offset = 0;
        long _ourline = rand() % _lines;
        fseek(fpi, _ourline*(long)sizeof(long), SEEK_SET);
        fread(&_offset, sizeof(long), 1, fpi);

        // Get it
        char buf[512];
        fseek(fp, _offset, SEEK_SET);
        fgets(buf, sizeof(buf), fp);
        StripQuotes(strbtrim(buf));
        strxcpy(__buf, buf, __bufsize);
      }
      fclose(fpi);
    }
    fclose(fp);
  }

  return __buf;
}


//  ------------------------------------------------------------------

char* HandleRandomLine(char* buf, size_t bufsize) {

  if(*buf == '@')
    GetRandomLine(buf, bufsize, buf+1);

  return buf;
}


//  ------------------------------------------------------------------ 

void Area::InitData() {

  // Set defaults
  adat->aka = CFG->aka[GetAkaNo(aka())];
  adat->akamatching = (CFG->switches.get(akamatchnet) and isnet()) or
                      (CFG->switches.get(akamatchecho) and isecho()) or
                      (CFG->switches.get(akamatchlocal) and islocal());
  adat->attributes = attr();
  adat->areacopyaddid = CFG->areacopyaddid;
  adat->areacopydirect = CFG->areacopydirect;
  strcpy(adat->areacopyto, CFG->areacopyto);
  adat->areaforwarddirect = CFG->areaforwarddirect;
  adat->areafreqdirect = CFG->areafreqdirect;
  strcpy(adat->areafreqto, CFG->areafreqto);
  adat->areareplydirect = CFG->areareplydirect;
  strcpy(adat->areareplyto, CFG->areareplyto);
  strcpy(adat->areayouwroteto, CFG->areayouwroteto);
  if(isemail())
    adat->ctrlinfo = CFG->ctrlinfoemail;
  else if(isnewsgroup())
    adat->ctrlinfo = CFG->ctrlinfonews;
  else if(isnet())
    adat->ctrlinfo = CFG->ctrlinfonet;
  else if(isecho())
    adat->ctrlinfo = CFG->ctrlinfoecho;
  else
    adat->ctrlinfo = CFG->ctrlinfolocal;
  adat->edithardterm = EDIT->HardTerm();
  adat->editmixcase = EDIT->MixCase();
  adat->forcetemplate = CFG->forcetemplate;
  strcpy(adat->inputfile, CFG->inputfile);
  strcpy(adat->internetaddress, CFG->internetaddress);
  memcpy(&adat->internetgate, &CFG->internetgate, sizeof(Node));
  adat->internetmsgid = CFG->internetmsgid;
  adat->internetrfcbody = CFG->internetrfcbody;
  strcpy(adat->loadlanguage, CFG->loadlanguage);
  adat->msglistdate = CFG->msglistdate;
  adat->msglistfast = CFG->msglistfast;
  adat->msglistfirst = CFG->msglistfirst;
  adat->msglistheader = CFG->msglistheader;
  adat->msglistwidesubj = CFG->msglistwidesubj;
  strcpy(adat->organization, CFG->organization);
  strxcpy(adat->origin, CFG->origin.empty() ? "" : CFG->origin[CFG->originno].c_str(), sizeof(adat->origin));
  strcpy(adat->outputfile, CFG->outputfile);
  memcpy(&adat->play, &CFG->play, sizeof(GPlay));
  strcpy(adat->quotebuffile, CFG->quotebuffile);
  strcpy(adat->quotechars, CFG->quotechars);
  adat->quotectrl = CFG->quotectrl;
  strcpy(adat->quotestring, CFG->quotestring);
  adat->quotewraphard = CFG->quotewraphard;
  adat->replyre = EDIT->ReplyRe();
  if(*CFG->searchfor)
    strcpy(adat->searchfor, CFG->searchfor);
  strcpy(adat->tagline, CFG->tagline.empty() ? "" : CFG->tagline[CFG->taglineno].c_str());
  adat->taglinechar = CFG->taglinechar;
  adat->taglinesupport = CFG->taglinesupport;
  strcpy(adat->tearline, CFG->tearline);
  strcpy(adat->tpl, (CFG->tplno and (CFG->tplno < CFG->tpl.size())) ? CFG->tpl[CFG->tplno].file : "");
  adat->templatematch = CFG->templatematch;
  adat->twitmode = CFG->twitmode;
  adat->usefwd = CFG->usefwd;
  strcpy(adat->username.name, CFG->username.empty() ? "" : CFG->username[CFG->usernameno].name);
  adat->username.addr = adat->aka.addr;
  adat->usetzutc = CFG->usetzutc;
  adat->viewhidden = CFG->viewhidden;
  adat->viewkludge = CFG->viewkludge;
  adat->viewquote = CFG->viewquote;
  strcpy(adat->nickname, CFG->nickname);
  strcpy(adat->whoto, CFG->whoto);
  strcpy(adat->xlatexport, CFG->xlatexport);
  strcpy(adat->xlatimport, CFG->xlatimport);

  LoadLanguage(adat->loadlanguage);
}


//  ------------------------------------------------------------------
//  Get data from the Random System

void Area::RandomizeData(int mode) {

  if(not adat) {
    adat = (AreaData*)throw_calloc(1, sizeof(AreaData));
    InitData();
  }
  else {       // reset to global defaults
    strcpy(adat->tpl, (CFG->tplno and (CFG->tplno < CFG->tpl.size())) ? CFG->tpl[CFG->tplno].file : "");
  }

  char _groupid[10];
  if(CFG->areafilegroups and groupid()) {
    if(groupid() & 0x8000u)
      sprintf(_groupid, "#%u", groupid() & 0x7FFF);
    else {
      _groupid[0] = (char)(isalpha(groupid()) ? groupid() : NUL);
      _groupid[1] = NUL;
    }
  }
  else {
    *_groupid = NUL;
  }

  Echo _echoid;
  strcpy(_echoid, echoid());

  const char* found = CFG->grp.SetGrp(_echoid);
  int curgrp = CFG->grp.currgrpno;
  if(*_groupid) {
    found = CFG->grp.SetGrp(_groupid);
    if(found and (strieql(found, "*") or ((curgrp >= 0) and (curgrp < CFG->grp.currgrpno))))
      found = NULL;
    if(not found)
      found = CFG->grp.SetGrp(_echoid);
  }

  if(found) {
    char buf[256];

    if(CFG->grp.GetItm(GRP_AKA, &adat->aka, sizeof(gaka)))
      SetAka(adat->aka.addr);

    CFG->grp.GetItm(GRP_AKAMATCHING, adat->akamatching);

    if(CFG->grp.GetItm(GRP_ATTRIBUTES, buf, sizeof(buf))) {
      if(isnet())
        adat->attributes = CFG->attribsnet;
      else if(isecho())
        adat->attributes = CFG->attribsecho;
      else
        adat->attributes = CFG->attribslocal;
      GetAttribstr(&adat->attributes, buf);
    }

    CFG->grp.GetItm(GRP_AREACOPYADDID, adat->areacopyaddid);
    CFG->grp.GetItm(GRP_AREACOPYDIRECT, adat->areacopydirect);
    CFG->grp.GetItm(GRP_AREACOPYTO, adat->areacopyto, sizeof(adat->areacopyto));
    CFG->grp.GetItm(GRP_AREAFORWARDDIRECT, adat->areaforwarddirect);
    CFG->grp.GetItm(GRP_AREAFREQDIRECT, adat->areafreqdirect);
    CFG->grp.GetItm(GRP_AREAFREQTO, adat->areafreqto, sizeof(adat->areareplyto));
    CFG->grp.GetItm(GRP_AREAREPLYDIRECT, adat->areareplydirect);
    CFG->grp.GetItm(GRP_AREAREPLYTO, adat->areareplyto, sizeof(adat->areareplyto));
    CFG->grp.GetItm(GRP_AREAYOUWROTETO, adat->areayouwroteto, sizeof(adat->areayouwroteto));
    CFG->grp.GetItm(GRP_CTRLINFO, adat->ctrlinfo);
    CFG->grp.GetItm(GRP_EDITHARDTERM, adat->edithardterm);
    CFG->grp.GetItm(GRP_EDITMIXCASE, adat->editmixcase);
    CFG->grp.GetItm(GRP_EDITREPLYRE, adat->replyre);
    CFG->grp.GetItm(GRP_FORCETEMPLATE, adat->forcetemplate);
    CFG->grp.GetItm(GRP_INPUTFILE, adat->inputfile, sizeof(adat->inputfile));
    CFG->grp.GetItm(GRP_INTERNETADDRESS, adat->internetaddress, sizeof(adat->internetaddress));
    CFG->grp.GetItm(GRP_INTERNETGATE, &adat->internetgate, sizeof(Node));
    CFG->grp.GetItm(GRP_INTERNETMSGID, adat->internetmsgid);
    CFG->grp.GetItm(GRP_INTERNETRFCBODY, adat->internetrfcbody);

    if(CFG->grp.GetItm(GRP_LOADLANGUAGE, adat->loadlanguage, sizeof(adat->loadlanguage)))
      LoadLanguage(adat->loadlanguage);

    CFG->grp.GetItm(GRP_MSGLISTDATE, adat->msglistdate);
    CFG->grp.GetItm(GRP_MSGLISTFAST, adat->msglistfast);
    CFG->grp.GetItm(GRP_MSGLISTFIRST, adat->msglistfirst);
    CFG->grp.GetItm(GRP_MSGLISTHEADER, adat->msglistheader);
    CFG->grp.GetItm(GRP_MSGLISTWIDESUBJ, adat->msglistwidesubj);
    CFG->grp.GetItm(GRP_ORGANIZATION, adat->organization, sizeof(adat->organization));

    if(CFG->grp.GetItm(GRP_ORIGIN, buf, sizeof(buf)))
      strxcpy(adat->origin, buf, sizeof(adat->origin));

    CFG->grp.GetItm(GRP_OUTPUTFILE, adat->outputfile, sizeof(adat->outputfile));
    CFG->grp.GetItm(GRP_QUOTEBUFFILE, adat->quotebuffile, sizeof(adat->quotebuffile));
    CFG->grp.GetItm(GRP_QUOTECHARS, adat->quotechars, sizeof(adat->quotechars));
    CFG->grp.GetItm(GRP_QUOTECTRL, adat->quotectrl);
    CFG->grp.GetItm(GRP_QUOTESTRING, adat->quotestring, sizeof(adat->quotestring));
    CFG->grp.GetItm(GRP_QUOTEWRAPHARD, adat->quotewraphard);

    if(CFG->grp.GetItm(GRP_PLAY, buf, sizeof(buf))) {
      if(*buf == '@')
        GetRandomLine(buf, sizeof(buf), buf+1);
      FileToGPlay(&adat->play, buf);
    }

    CFG->grp.GetItm(GRP_SEARCHFOR, adat->searchfor, sizeof(adat->searchfor));

    if(CFG->grp.GetItm(GRP_TAGLINE, buf, sizeof(buf)))
      strxcpy(adat->tagline, buf, sizeof(adat->tagline));
    
    CFG->grp.GetItm(GRP_TAGLINECHAR, adat->taglinechar);
    CFG->grp.GetItm(GRP_TAGLINESUPPORT, adat->taglinesupport);

    if(CFG->grp.GetItm(GRP_TEARLINE, buf, sizeof(buf)))
      strxcpy(adat->tearline, buf, sizeof(adat->tearline));
    
    if(not adat->forcetemplate) {
      if(CFG->tplno and (CFG->tplno < CFG->tpl.size()))
        strxcpy(adat->tpl, CFG->tpl[CFG->tplno].file, sizeof(adat->tpl));
      else
        if(CFG->grp.GetItm(GRP_TEMPLATE, buf, sizeof(buf))) {
          if(*buf == '@')
            GetRandomLine(buf, sizeof(buf), buf+1);
          strxcpy(adat->tpl, buf, sizeof(adat->tpl));
        }
    }

    CFG->grp.GetItm(GRP_TEMPLATEMATCH, adat->templatematch);
    CFG->grp.GetItm(GRP_TWITMODE, adat->twitmode);
    CFG->grp.GetItm(GRP_USEFWD, adat->usefwd);

    if(CFG->grp.GetItm(GRP_USERNAME, buf, sizeof(buf))) {
      if(*buf == '@')
        GetRandomLine(buf, sizeof(buf), buf+1);
      Addr tmp;
      tmp.reset();
      switch(mode) {
        case MODE_NEW:
        case MODE_QUOTE:
        case MODE_REPLY:
        case MODE_REPLYCOMMENT:
        case MODE_FORWARD:
          char* aptr = strrchr(buf, ',');
          if(aptr) {
            *aptr++ = NUL;
            aptr = strskip_wht(aptr);
          }
          else {
            aptr = strrchr(buf, ' ');
            if(aptr) {
              aptr++;
              tmp.set(aptr);
              if(tmp.net == 0)
                aptr = NULL;
              else
                *aptr++ = NUL;
            }
          }
          if(aptr) {
            tmp = adat->aka.addr;
            tmp.set(aptr);
            if(tmp.net != 0)
              *aptr = NUL;
          }
          strbtrim(buf);
          break;
      }
      if(tmp.net == 0)
        tmp = adat->aka.addr;
      strxcpy(adat->username.name, buf, sizeof(adat->username.name));
      adat->username.addr = tmp;
      SetAka(tmp);
    }

    CFG->grp.GetItm(GRP_USETZUTC, adat->usetzutc);

    if(not mode) {
      CFG->grp.GetItm(GRP_VIEWHIDDEN, adat->viewhidden);
      CFG->grp.GetItm(GRP_VIEWKLUDGE, adat->viewkludge);
      CFG->grp.GetItm(GRP_VIEWQUOTE, adat->viewquote);
    }

    if(CFG->grp.GetItm(GRP_NICKNAME, buf, sizeof(buf)))
      strxcpy(adat->nickname, buf, sizeof(adat->nickname));

    if(CFG->grp.GetItm(GRP_WHOTO, buf, sizeof(buf))) {
      if(*buf == '@')
        GetRandomLine(buf, sizeof(buf), buf+1);
      strxcpy(adat->whoto, buf, sizeof(adat->whoto));
    }

    CFG->grp.GetItm(GRP_XLATEXPORT, adat->xlatexport, sizeof(adat->xlatexport));
    CFG->grp.GetItm(GRP_XLATIMPORT, adat->xlatimport, sizeof(adat->xlatimport));
  }

  vector<MailList>::iterator z;
  for(z = CFG->mailinglist.begin(); z != CFG->mailinglist.end(); z++)
    if(strieql(echoid(), z->echoid)) {
      strcpy(adat->whoto, *z->contribution ? z->contribution : z->sender);
      break;
    }
}


//  ------------------------------------------------------------------

