
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

char* GetRandomLine(char* __buf, size_t __bufsize, const char* file)
{
  Path indexfile;
  Path __file;

  strxcpy(__buf, "", __bufsize);
  strxcpy(__file, file, sizeof(Path));
  strschg_environ(__file);

  replaceextension(indexfile, __file, ".sdx");

  gfile fp(AddPath(CFG->goldpath, __file), "rb", CFG->sharemode);
  if (fp.isopen())
  {
    fp.SetvBuf(NULL, _IOFBF, 32000);

    // Check if index exists or if it is older than the textfile
    const char* idxfile = AddPath(CFG->goldpath, indexfile);
    int idxexist = fexist(idxfile);
    if(idxexist)
      if(FiletimeCmp(__file, idxfile) > 0)
        idxexist = false;

    // Create index if one was missing
    if (not idxexist)
    {
      gfile fpi(idxfile, "wb", CFG->sharemode);
      if (fpi.isopen())
      {
        fpi.SetvBuf(NULL, _IOFBF, 16000);
        long fpos = 0;
        char buf[512];
        while (fp.Fgets(buf, sizeof(buf)))
        {
          fpi.Fwrite(&fpos, sizeof(long));
          fpos += strlen(buf);
        }
      }
    }

    gfile fpi(idxfile, "rb", CFG->sharemode);
    if (fpi.isopen())
    {
      fpi.SetvBuf(NULL, _IOFBF, 16000);

      // Get random line if there is at least one
      int _lines = (int)(fpi.FileLength()/sizeof(long));
      if(_lines > 0) {

        // Select a random line
        long _offset = 0;
        long _ourline = rand() % _lines;
        fpi.FseekSet(_ourline, (long)sizeof(long));
        fpi.Fread(&_offset, sizeof(long));

        // Get it
        char buf[512];
        fp.FseekSet(_offset);
        fp.Fgets(buf, sizeof(buf));
        StripQuotes(strbtrim(buf));
        strxcpy(__buf, buf, __bufsize);
      }
    }
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
  WideDispsoftcr = adat->dispsoftcr = CFG->dispsoftcr;
  adat->edithardterm = EDIT->HardTerm();
  adat->editmixcase = EDIT->MixCase();
  adat->forcetemplate = CFG->forcetemplate;
  adat->hidestylies = CFG->hidestylies;
  adat->usestylies = CFG->usestylies;
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
  strcpy(adat->quotestops, CFG->quotestops);
  adat->quotewraphard = CFG->quotewraphard;
  adat->replyre = EDIT->ReplyRe();
#if defined(GCFG_SPELL_INCLUDED)
  strcpy(adat->scheckerdeflang, CFG->scheckerdeflang);
#endif
  if(*CFG->searchfor)
    strcpy(adat->searchfor, CFG->searchfor);
  strcpy(adat->tagline, CFG->tagline.empty() ? "" : CFG->tagline[CFG->taglineno].c_str());
  adat->taglinechar = CFG->taglinechar;
  adat->taglinesupport = CFG->taglinesupport;
  strcpy(adat->tearline, CFG->tearline);
  strcpy(adat->tpl, (CFG->tplno and (CFG->tplno < CFG->tpl.size())) ? CFG->tpl[CFG->tplno].file : "");
  strxcpy(adat->wtpl, CFG->wtpl.c_str(), sizeof(adat->wtpl));
  adat->templatematch = CFG->templatematch;
  adat->twitmode = CFG->twitmode;
  adat->inittwit = true;
  adat->twitname = CFG->twitname;
  adat->twitsubj = CFG->twitsubj;
  adat->usearea = CFG->usearea;
  adat->usefwd = CFG->usefwd;
  strcpy(adat->username.name, CFG->username.empty() ? "" : CFG->username[CFG->usernameno].name);
  adat->username.addr = adat->aka.addr;
  adat->usesoftcrxlat = CFG->usesoftcrxlat;
  adat->usetzutc = CFG->usetzutc;
  adat->viewhidden = CFG->viewhidden;
  adat->viewkludge = CFG->viewkludge;
  adat->viewquote = CFG->viewquote;
  adat->writeheader = CFG->writeheader;
  adat->striphtml = CFG->striphtml;
  strcpy(adat->nickname, CFG->nickname);
  strcpy(adat->netname, CFG->netname);
  strcpy(adat->whoto, CFG->whoto);
  strcpy(adat->xlatexport, CFG->xlatexport);
  strcpy(adat->xlatimport, CFG->xlatimport);

  char _groupid[10];
  if(CFG->areafilegroups and groupid()) {
    if(groupid() & 0x8000u)
      sprintf(_groupid, "#%u", groupid() & 0x7FFF);
    else {
      _groupid[0] = (char)(g_isalpha(groupid()) ? groupid() : NUL);
      _groupid[1] = NUL;
    }
  }
  else {
    *_groupid = NUL;
  }

  const char* found = CFG->grp.SetGrp(echoid());
  int curgrp = CFG->grp.currgrpno;
  if(*_groupid) {
    found = CFG->grp.SetGrp(_groupid);
    if(found and (strieql(found, "*") or ((curgrp >= 0) and (curgrp < CFG->grp.currgrpno))))
      found = NULL;
    if(not found)
      found = CFG->grp.SetGrp(echoid());
  }

  if(found) {
    CFG->grp.GetItm(GRP_XLATEXPORT, adat->xlatexport, sizeof(adat->xlatexport));
    CFG->grp.GetItm(GRP_XLATIMPORT, adat->xlatimport, sizeof(adat->xlatimport));
  }

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
      _groupid[0] = (char)(g_isalpha(groupid()) ? groupid() : NUL);
      _groupid[1] = NUL;
    }
  }
  else {
    *_groupid = NUL;
  }

  const char* found = CFG->grp.SetGrp(echoid());
  int curgrp = CFG->grp.currgrpno;
  if(*_groupid) {
    found = CFG->grp.SetGrp(_groupid);
    if(found and (strieql(found, "*") or ((curgrp >= 0) and (curgrp < CFG->grp.currgrpno))))
      found = NULL;
    if(not found)
      found = CFG->grp.SetGrp(echoid());
  }

  if(found) {
    char buf[256];

    if(CFG->grp.GetItm(GRP_AKA, &adat->aka, sizeof(gaka)))
      SetAka(adat->aka.addr);

    CFG->grp.GetItm(GRP_AKAMATCHING, adat->akamatching);

    if(CFG->grp.GetItm(GRP_ATTRIBUTES, buf, sizeof(buf))) {
      if(isemail())
        adat->attributes = CFG->attribsemail;
      else if(isnewsgroup())
        adat->attributes = CFG->attribsnews;
      else if(isnet())
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
    CFG->grp.GetItm(GRP_DISPSOFTCR, adat->dispsoftcr);
    WideDispsoftcr = adat->dispsoftcr;
    CFG->grp.GetItm(GRP_EDITHARDTERM, adat->edithardterm);
    CFG->grp.GetItm(GRP_EDITMIXCASE, adat->editmixcase);
    CFG->grp.GetItm(GRP_EDITREPLYRE, adat->replyre);
    CFG->grp.GetItm(GRP_FORCETEMPLATE, adat->forcetemplate);
    CFG->grp.GetItm(GRP_HIDESTYLIES, adat->hidestylies);
    CFG->grp.GetItm(GRP_USESTYLIES, adat->usestylies);
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
    CFG->grp.GetItm(GRP_QUOTESTOPS, adat->quotestops, sizeof(adat->quotestops));

    CFG->grp.GetItm(GRP_QUOTEWRAPHARD, adat->quotewraphard);

    if(CFG->grp.GetItm(GRP_PLAY, buf, sizeof(buf))) {
      if(*buf == '@')
        GetRandomLine(buf, sizeof(buf), buf+1);
      FileToGPlay(&adat->play, buf);
    }

#if defined(GCFG_SPELL_INCLUDED)
    if (CFG->grp.GetItm(GRP_SCHECKERDEFLANG, buf, sizeof(buf)))
      strxcpy(adat->scheckerdeflang, buf, sizeof(adat->scheckerdeflang));
#endif

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
    if(CFG->grp.GetItm(GRP_WRITETEMPLATE, buf, sizeof(buf))) {
      if(*buf == '@')
        GetRandomLine(buf, sizeof(buf), buf+1);
      strxcpy(adat->wtpl, buf, sizeof(adat->wtpl));
    }

    CFG->grp.GetItm(GRP_TEMPLATEMATCH, adat->templatematch);
    CFG->grp.GetItm(GRP_TWITMODE, adat->twitmode);

    if (adat->inittwit)
    {
      Node node;
      std::string subj;

      int i, nr = CFG->grp.GetItm(GRP_TWITNAME, &node, sizeof(node));
      for (i = 0; i < nr; i++)
      {
        CFG->grp.GetItm(GRP_TWITNAME, &node, sizeof(node), i);
        adat->twitname.push_back(node);
      }

      nr = CFG->grp.GetItm(GRP_TWITSUBJ, subj);
      for (i = 0; i < nr; i++)
      {
        CFG->grp.GetItm(GRP_TWITSUBJ, subj, i);
        adat->twitsubj.push_back(subj);
      }

      adat->inittwit = false;
    }

    CFG->grp.GetItm(GRP_USEAREA, adat->usearea);
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

    CFG->grp.GetItm(GRP_USESOFTCRXLAT, adat->usesoftcrxlat);
    CFG->grp.GetItm(GRP_USETZUTC, adat->usetzutc);

    if(not mode) {
      CFG->grp.GetItm(GRP_VIEWHIDDEN, adat->viewhidden);
      CFG->grp.GetItm(GRP_VIEWKLUDGE, adat->viewkludge);
      CFG->grp.GetItm(GRP_VIEWQUOTE, adat->viewquote);
      CFG->grp.GetItm(GRP_STRIPHTML, adat->striphtml);
      CFG->grp.GetItm(GRP_WRITEHEADER, adat->writeheader);
    }

    if(CFG->grp.GetItm(GRP_NICKNAME, buf, sizeof(buf)))
      strxcpy(adat->nickname, buf, sizeof(adat->nickname));

    if(CFG->grp.GetItm(GRP_NETNAME, buf, sizeof(buf)))
      strxcpy(adat->netname, buf, sizeof(adat->netname));

    if(CFG->grp.GetItm(GRP_WHOTO, buf, sizeof(buf))) {
      if(*buf == '@')
        GetRandomLine(buf, sizeof(buf), buf+1);
      strxcpy(adat->whoto, buf, sizeof(adat->whoto));
    }
  }

  std::vector<MailList>::iterator z;
  for(z = CFG->mailinglist.begin(); z != CFG->mailinglist.end(); z++) {
    if(strieql(echoid(), z->echoid)) {
      strcpy(adat->whoto, *z->contribution ? z->contribution : z->sender);
      break;
    }
  }

  if(adat->attributes.r_o())
    area->attr().r_o1();
}


//  ------------------------------------------------------------------

