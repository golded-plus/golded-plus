
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
//  Configuration functions, part 8.
//  ------------------------------------------------------------------

#include <golded.h>
#include <gcprot.h>


//  ------------------------------------------------------------------

extern char* val;


//  ------------------------------------------------------------------

void CfgSquishuserno() {

  CFG->squishuserno = atoi(val);
}

//  ------------------------------------------------------------------

void CfgSquishuserpath() {

  PathCopy(CFG->squishuserpath, val);
}

//  ------------------------------------------------------------------

void CfgStatuslinehelp() {

  CFG->statuslinehelp = strieql(val, "NOLOGO") ? -1 : GetYesno(val);
}

//  ------------------------------------------------------------------

void CfgStylecodepunct() {

  char* key;
  getkeyval(&key, &val);
  strxcpy(CFG->stylecodepunct, strcvtc(key), sizeof(CFG->stylecodepunct));
}

//  ------------------------------------------------------------------

void CfgStylecodes() {

  if(strieql(val, "HIDE")) {
    CFG->hidestylies = true;
    CFG->usestylies = true;
  }
  else {
    CFG->hidestylies = false;
    CFG->usestylies = GetYesno(val);
  }
}

//  ------------------------------------------------------------------

void CfgStylecodestops() {

  char* key;
  getkeyval(&key, &val);
  strxcpy(CFG->stylecodestops, strcvtc(key), sizeof(CFG->stylecodestops));
}

//  ------------------------------------------------------------------

void CfgTagline() {

  char buf[76];
  strxcpy(buf, val, sizeof(buf));
  if(cfgingroup)
    CFG->grp.AddItm(GRP_TAGLINE, buf, strlen(buf)+1);
  else
    CFG->tagline.push_back(buf);
}

//  ------------------------------------------------------------------

void CfgTaglinechar() {

  StripQuotes(val);
  if(cfgingroup)
    CFG->grp.AddItm(GRP_TAGLINECHAR, *val);
  else
    CFG->taglinechar = *val;
}

//  ------------------------------------------------------------------

void CfgTaglinesupport() {

  bool flag = GetYesno(val);
  if(cfgingroup)
    CFG->grp.AddItm(GRP_TAGLINESUPPORT, flag);
  else
    CFG->taglinesupport = flag;
}

//  ------------------------------------------------------------------

void CfgTasktitle(){

  strxcpy(CFG->tasktitle, val,sizeof(CFG->tasktitle));
}

//  ------------------------------------------------------------------

void CfgTearline() {

  StripQuotes(val);
  Tear buf;
  strxcpy(buf, val, sizeof(buf));
  if(cfgingroup)
    CFG->grp.AddItm(GRP_TEARLINE, buf, strlen(buf)+1);
  else
    strcpy(CFG->tearline, buf);
}

//  ------------------------------------------------------------------

void CfgTemplate() {

  if(cfgingroup) {
    Path buf;
    strxcpy(buf, val, sizeof(buf));
    CFG->grp.AddItm(GRP_TEMPLATE, buf, strlen(buf)+1);
  }
  else {
    char* key;
    Tpl tp;
    *tp.name = NUL;
    tp.match.reset();
    getkeyval(&key, &val);
    strcpy(tp.file, key);
    if((*val == '\'') or (*val == '\"')) {
      getkeyval(&key, &val);
      strcpy(tp.name, key);
      if(*val)
        tp.match.set(val);
    }
    else if(*val)
      tp.match.set(val);
    if(not *tp.name)
      strcpy(tp.name, tp.file);
    CFG->tpl.push_back(tp);
  }
}

//  ------------------------------------------------------------------

void CfgTemplatematch() {

  bool flag = GetYesno(val);
  if(cfgingroup)
    CFG->grp.AddItm(GRP_TEMPLATEMATCH, flag);
  else
    CFG->templatematch = flag;
}


//  ------------------------------------------------------------------

void CfgTemplatepath() {

  PathCopy(CFG->templatepath, val);
}

//  ------------------------------------------------------------------

void CfgTemppath() {

  PathCopy(CFG->temppath, val);
}

//  ------------------------------------------------------------------

void CfgTimeout() {

  CFG->timeout = atoi(val);
}

//  ------------------------------------------------------------------

void CfgTitlestatus(){

  CFG->titlestatus = GetYesno(val);
}

//  ------------------------------------------------------------------

void CfgTwitmode() {

  int tmp = TWIT_SHOW;
  if(strieql(val, "Skip"))          tmp = TWIT_SKIP;
  else if(strieql(val, "Blank"))    tmp = TWIT_BLANK;
  else if(strieql(val, "Show"))     tmp = TWIT_SHOW;
  else if(strieql(val, "Ignore"))   tmp = TWIT_IGNORE;
  else if(strieql(val, "Kill"))     tmp = TWIT_KILL;
  else tmp = GetYesno(val) ? TWIT_SHOW : TWIT_BLANK;

  if(cfgingroup)
    CFG->grp.AddItm(GRP_TWITMODE, tmp);
  else
    CFG->twitmode = tmp;
}

//  ------------------------------------------------------------------

void CfgTwitname() {

  Node tn;
  tn.addr.zone = GFTN_ALL;
  tn.addr.net = GFTN_ALL;
  tn.addr.node = GFTN_ALL;
  tn.addr.point = GFTN_ALL;
  char* ptr = strrchr(val, ' ');
  if(ptr == NULL)
    ptr = val;
  ptr = strskip_wht(ptr);
  if(isdigit(*ptr)) {
    if(not CFG->aka.empty())
      tn.addr = CFG->aka[0].addr;
    tn.addr.set(ptr);
    if(tn.addr.net) {
      // Address was given
      *ptr = NUL;
      strbtrim(val);
    }
  }
  strcpy(tn.name, val);
  CFG->twitname.push_back(tn);
}

//  ------------------------------------------------------------------

void CfgTwitsubj() {

  CFG->twitsubj.push_back(StripQuotes(val));
}

//  ------------------------------------------------------------------

void CfgUsecharset() {

  CFG->usecharset = GetYesno(val);
}

//  ------------------------------------------------------------------

void CfgUsefwd() {

  int flag = GetYesno(val);
  if(cfgingroup)
    CFG->grp.AddItm(GRP_USEFWD, flag);
  else
    CFG->usefwd = flag;
}

//  ------------------------------------------------------------------

void CfgUseintl() {

  CFG->useintl = GetYesno(val);
}

//  ------------------------------------------------------------------

void CfgUsepid() {

  CFG->usepid = GetYesno(val);
}

//  ------------------------------------------------------------------

void CfgUserlist() {

  // Only used by GoldNODE
}

//  ------------------------------------------------------------------

void CfgUserlistfile() {

  strcpy(CFG->userlistfile, val);
}

//  ------------------------------------------------------------------

void CfgUsername(char* v)  { val = v; CfgUsername(); }
void CfgUsername() {

  StripQuotes(val);
  if(not strblank(val)) {
    if(cfgingroup) {
      char buf[256];
      strxcpy(buf, val, sizeof(buf));
      CFG->grp.AddItm(GRP_USERNAME, buf, strlen(buf)+1);
    }
    else {
      Node tmp;

      char* aptr = strrchr(val, ',');
      if(not aptr)
        aptr = strrchr(val, ' ');

      if(aptr) {
        char* p = strskip_wht(aptr+1);
        tmp.addr.reset(p);
        if(not tmp.addr.valid())
          aptr = NULL;
        else {
          *aptr = NUL;
          aptr = p;
        }
      }

      tmp.addr.set_all(GFTN_ALL);
      if(aptr) {
        if(not CFG->aka.empty())
          tmp.addr = CFG->aka[0].addr;
        tmp.addr.set(aptr);
        if(CFG->aka.empty() and tmp.addr.zone != GFTN_ALL
                         and tmp.addr.net != GFTN_ALL
                         and tmp.addr.node != GFTN_ALL
                         and tmp.addr.point != GFTN_ALL)
          CfgAddress(tmp.addr.make_string(tmp.name));  // tmp.name is temp-var
      }

      strxcpy(tmp.name, strbtrim(val), sizeof(Name));

      // Check if we have it already
      vector<Node>::iterator u;
      for(u = CFG->username.begin(); u != CFG->username.end(); u++)
        if(strieql(tmp.name, u->name))
          return;

      CFG->username.push_back(tmp);
    }
  }
}

//  ------------------------------------------------------------------

void CfgUsetzutc() {

  bool flag = GetYesno(val);
  if(cfgingroup)
    CFG->grp.AddItm(GRP_USETZUTC, flag);
  else
    CFG->usetzutc = flag;
}

//  ------------------------------------------------------------------

void CfgUudecodepath() {

  PathCopy(CFG->uudecodepath, val);
}

//  ------------------------------------------------------------------

void CfgViewhidden() {

  bool flag = GetYesno(val);
  if(cfgingroup)
    CFG->grp.AddItm(GRP_VIEWHIDDEN, flag);
  else
    CFG->viewhidden = flag;
}

//  ------------------------------------------------------------------

void CfgViewkludge() {

  bool flag = GetYesno(val);
  if(cfgingroup)
    CFG->grp.AddItm(GRP_VIEWKLUDGE, flag);
  else
    CFG->viewkludge = flag;
}

//  ------------------------------------------------------------------

void CfgViewquote() {

  bool flag = GetYesno(val);
  if(cfgingroup)
    CFG->grp.AddItm(GRP_VIEWQUOTE, flag);
  else
    CFG->viewquote = flag;
}

//  ------------------------------------------------------------------

void CfgWhoto() {

  Name buf;
  strxcpy(buf, val, sizeof(buf));
  if(cfgingroup)
    CFG->grp.AddItm(GRP_WHOTO, buf, strlen(buf)+1);
  else
    strcpy(CFG->whoto, buf);
}

//  ------------------------------------------------------------------

void CfgWildcatuserno() {

  CFG->wildcatuserno = atoi(val);
}

//  ------------------------------------------------------------------

void CfgXlatcharset() {

  Map xlt;
  char* ptr = strtok(val, " \t\n\r");
  if(ptr) {
    strchg(strupr(strcpy(xlt.imp, ptr)), '_', ' ');
    ptr = strtok(NULL, " \t\n\r");
    if(ptr) {
      strchg(strupr(strcpy(xlt.exp, ptr)), '_', ' ');
      ptr = strtok(NULL, " \t\n\r");
      if(ptr) {
        if(*CFG->xlatpath == NUL)
          strcpy(CFG->xlatpath, CFG->goldpath);
        MakePathname(ptr, CFG->xlatpath, ptr);
        xlt.mapfile = throw_strdup(ptr);
        CFG->xlatcharset.push_back(xlt);
      }
    }
  }
}

//  ------------------------------------------------------------------

void CfgXlatescset() {

  Map xlt;
  char* ptr = strtok(val, " \t\n\r");
  if(ptr) {
    strchg(strupr(strcpy(xlt.imp, ptr)), '_', ' ');
    ptr = strtok(NULL, " \t\n\r");
    if(ptr) {
      strchg(strupr(strcpy(xlt.exp, ptr)), '_', ' ');
      ptr = strtok(NULL, " \t\n\r");
      if(ptr) {
        if(*CFG->xlatpath == NUL)
          strcpy(CFG->xlatpath, CFG->goldpath);
        MakePathname(ptr, CFG->xlatpath, ptr);
        xlt.mapfile = throw_strdup(ptr);
        CFG->xlatescset.push_back(xlt);
      }
    }
  }
}

//  ------------------------------------------------------------------

void CfgXlatexport() {

  XlatName buf;
  strchg(strupr(strxcpy(buf, val, sizeof(buf))), '_', ' ');
  if(cfgingroup)
    CFG->grp.AddItm(GRP_XLATEXPORT, buf, strlen(buf)+1);
  else
    strcpy(CFG->xlatexport, buf);
}

//  ------------------------------------------------------------------

void CfgXlatimport() {

  XlatName buf;
  strchg(strupr(strxcpy(buf, val, sizeof(buf))), '_', ' ');
  if(cfgingroup)
    CFG->grp.AddItm(GRP_XLATIMPORT, buf, strlen(buf)+1);
  else
    strcpy(CFG->xlatimport, buf);
}

//  ------------------------------------------------------------------

void CfgXlatlocalset() {

  strupr(strxcpy(CFG->xlatlocalset, val, sizeof(CFG->xlatlocalset)));
}

//  ------------------------------------------------------------------

void CfgXlatpath() {

  PathCopy(CFG->xlatpath, val);
}

//  ------------------------------------------------------------------

void CfgZonegating() {

  CFG->zonegating = GetYesno(val);
}

//  ------------------------------------------------------------------

