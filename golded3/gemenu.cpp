
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
//  Menus.
//  ------------------------------------------------------------------

#include <golded.h>


//  ------------------------------------------------------------------

GMsg* MenuMsgPtr;


//  ------------------------------------------------------------------

void DispHeadAttrs(GMsg* msg) {

  char atrs[200];
  MakeAttrStr(atrs, sizeof(atrs), &msg->attr);
  strsetsz(atrs, MAXCOL-CFG->disphdrnodeset.pos);

  HeaderView->window.prints(1, CFG->disphdrnodeset.pos, HeaderView->window_color, atrs);
  HeaderView->window.prints(4, 0, HeaderView->window_color, (msg->attr.att() or msg->attr.frq() or msg->attr.urq()) ? LNG->File : LNG->Subj);
}


//  ------------------------------------------------------------------

static void DispHeadAttrs() {

  DispHeadAttrs(MenuMsgPtr);
}


//  ------------------------------------------------------------------

static void toggle_pvt() {

  MenuMsgPtr->attr.pvtX();
  DispHeadAttrs();
}


//  ------------------------------------------------------------------

static void toggle_received() {

  MenuMsgPtr->attr.rcvX();
  DispHeadAttrs();
}


//  ------------------------------------------------------------------

static void toggle_crash() {

  MenuMsgPtr->attr.craX();
  if(MenuMsgPtr->attr.cra())
    MenuMsgPtr->attr.hld0();
  DispHeadAttrs();
}


//  ------------------------------------------------------------------

static void toggle_hold() {

  MenuMsgPtr->attr.hldX();
  if(MenuMsgPtr->attr.hld()) {
    MenuMsgPtr->attr.cra0();
    MenuMsgPtr->attr.imm0();
  }
  DispHeadAttrs();
}


//  ------------------------------------------------------------------

static void toggle_file() {

  MenuMsgPtr->attr.attX();
  if(MenuMsgPtr->attr.att()) {
    AttrAdd(&MenuMsgPtr->attr, &CFG->attribsattach);
    MenuMsgPtr->attr.urq0();
    MenuMsgPtr->attr.frq0();
  }
  DispHeadAttrs();
}


//  ------------------------------------------------------------------

static void toggle_freq() {

  MenuMsgPtr->attr.frqX();
  if(MenuMsgPtr->attr.frq()) {
    MenuMsgPtr->attr.att0();
    MenuMsgPtr->attr.urq0();
  }
  DispHeadAttrs();
}


//  ------------------------------------------------------------------

static void toggle_updreq() {

  MenuMsgPtr->attr.urqX();
  if(MenuMsgPtr->attr.urq()) {
    MenuMsgPtr->attr.att0();
    MenuMsgPtr->attr.frq0();
  }
  DispHeadAttrs();
}


//  ------------------------------------------------------------------

static void toggle_kill() {

  MenuMsgPtr->attr.k_sX();
  if(MenuMsgPtr->attr.k_s())
    MenuMsgPtr->attr.a_s0();
  DispHeadAttrs();
}


//  ------------------------------------------------------------------

static void toggle_sent() {

  MenuMsgPtr->attr.sntX();
  if(MenuMsgPtr->attr.snt()) {
    MenuMsgPtr->attr.uns0();
    MenuMsgPtr->attr.scn1();
  }
  else {
    MenuMsgPtr->attr.uns1();
    MenuMsgPtr->attr.scn0();
    MenuMsgPtr->attr.loc1();
  }

  DispHeadAttrs();
}


//  ------------------------------------------------------------------

static void toggle_trunc() {

  MenuMsgPtr->attr.tfsX();
  if(MenuMsgPtr->attr.tfs())
    MenuMsgPtr->attr.kfs0();
  DispHeadAttrs();
}


//  ------------------------------------------------------------------

static void toggle_delsent() {

  MenuMsgPtr->attr.kfsX();
  if(MenuMsgPtr->attr.kfs())
    MenuMsgPtr->attr.tfs0();
  DispHeadAttrs();
}


//  ------------------------------------------------------------------

static void toggle_direct() {

  MenuMsgPtr->attr.dirX();
  if(MenuMsgPtr->attr.dir()) {
    MenuMsgPtr->attr.zon0();
    MenuMsgPtr->attr.hub0();
  }
  DispHeadAttrs();
}


//  ------------------------------------------------------------------

static void toggle_imm() {

  MenuMsgPtr->attr.immX();
  if(MenuMsgPtr->attr.imm())
    MenuMsgPtr->attr.hld0();
  DispHeadAttrs();
}


//  ------------------------------------------------------------------

static void toggle_locked() {

  MenuMsgPtr->attr.lokX();
  DispHeadAttrs();
}


//  ------------------------------------------------------------------

static void toggle_reserved() {

  MenuMsgPtr->attr.rsvX();
  DispHeadAttrs();
}


//  ------------------------------------------------------------------

static void toggle_groupmsg() {

  MenuMsgPtr->attr.grpX();
  DispHeadAttrs();
}


//  ------------------------------------------------------------------

static void toggle_arcsent() {

  MenuMsgPtr->attr.a_sX();
  if(MenuMsgPtr->attr.a_s())
    MenuMsgPtr->attr.k_s0();
  DispHeadAttrs();
}


//  ------------------------------------------------------------------

static void toggle_zonegate() {

  MenuMsgPtr->attr.zonX();
  if(MenuMsgPtr->attr.zon()) {
    MenuMsgPtr->attr.dir0();
    MenuMsgPtr->attr.hub0();
  }
  DispHeadAttrs();
}


//  ------------------------------------------------------------------

static void toggle_transit() {

  MenuMsgPtr->attr.trsX();
  DispHeadAttrs();
}


//  ------------------------------------------------------------------

static void toggle_retrecreq() {

  MenuMsgPtr->attr.rrqX();
  DispHeadAttrs();
}


//  ------------------------------------------------------------------

static void toggle_retrec() {

  MenuMsgPtr->attr.rrcX();
  DispHeadAttrs();
}


//  ------------------------------------------------------------------

static void toggle_orphan() {

  MenuMsgPtr->attr.orpX();
  DispHeadAttrs();
}


//  ------------------------------------------------------------------

static void toggle_audit() {

  MenuMsgPtr->attr.arqX();
  DispHeadAttrs();
}


//  ------------------------------------------------------------------

static void toggle_hubhost() {

  MenuMsgPtr->attr.hubX();
  if(MenuMsgPtr->attr.hub()) {
    MenuMsgPtr->attr.dir0();
    MenuMsgPtr->attr.zon0();
  }
  DispHeadAttrs();
}


//  ------------------------------------------------------------------

static void toggle_local() {

  MenuMsgPtr->attr.locX();
  if(MenuMsgPtr->attr.loc())
    MenuMsgPtr->attr.trs0();
  DispHeadAttrs();
}


//  ------------------------------------------------------------------

static void toggle_xmail() {

  MenuMsgPtr->attr.xmaX();
  DispHeadAttrs();
}


//  ------------------------------------------------------------------

static void toggle_cfmrecreq() {

  MenuMsgPtr->attr.cfmX();
  DispHeadAttrs();
}


//  ------------------------------------------------------------------

static void toggle_scanned() {

  MenuMsgPtr->attr.scnX();
  if(MenuMsgPtr->attr.scn()) {
    MenuMsgPtr->attr.snt1();
    MenuMsgPtr->attr.uns0();
  }
  else {
    MenuMsgPtr->attr.snt0();
    MenuMsgPtr->attr.uns1();
    MenuMsgPtr->attr.loc1();
  }
  DispHeadAttrs();
}


//  ------------------------------------------------------------------

static void clear_attrib() {

  MenuMsgPtr->attr.reset();
  DispHeadAttrs();
}


//  ------------------------------------------------------------------

static void DispAttrWindow(int show=-1) {

  static int wh_background = -1;
  static int wh_attributes = -1;

  if(show == -1)
    show = wh_attributes == -1;

  if(show) {
    wh_background = whandle();
    int wide = MaxV(strlen(LNG->AttrTitle)+2, strlen(LNG->AttrPvt)+2);
    wide = MinV(wide, MAXCOL-4);
    wh_attributes = wopen_(6, 0, 17, wide, W_BMENU, C_MENUB, C_MENUW);
    set_title(LNG->AttrTitle, TCENTER, C_MENUT);
    if(*LNG->AttrTurnOff)
      wtitle(LNG->AttrTurnOff, TCENTER|TBOTTOM, C_MENUT);
    title_shadow();
    int n = 0;
    wide -= 2;
    wprintns(n++, 0, C_MENUW, LNG->AttrPvt, wide, ' ', C_MENUW);
    wprintns(n++, 0, C_MENUW, LNG->AttrRcv, wide, ' ', C_MENUW);
    wprintns(n++, 0, C_MENUW, LNG->AttrSnt, wide, ' ', C_MENUW);
    wprintns(n++, 0, C_MENUW, LNG->AttrCrs, wide, ' ', C_MENUW);
    wprintns(n++, 0, C_MENUW, LNG->AttrHld, wide, ' ', C_MENUW);
    wprintns(n++, 0, C_MENUW, LNG->AttrFil, wide, ' ', C_MENUW);
    wprintns(n++, 0, C_MENUW, LNG->AttrFrq, wide, ' ', C_MENUW);
    wprintns(n++, 0, C_MENUW, LNG->AttrUpd, wide, ' ', C_MENUW);
    wprintns(n++, 0, C_MENUW, LNG->AttrKS , wide, ' ', C_MENUW);
    wprintns(n++, 0, C_MENUW, LNG->AttrKfs, wide, ' ', C_MENUW);
    wprintns(n++, 0, C_MENUW, LNG->AttrTfs, wide, ' ', C_MENUW);
    wprintns(n++, 0, C_MENUW, LNG->AttrDir, wide, ' ', C_MENUW);
    wprintns(n++, 0, C_MENUW, LNG->AttrImm, wide, ' ', C_MENUW);
    wprintns(n++, 0, C_MENUW, LNG->AttrLok, wide, ' ', C_MENUW);
    wprintns(n++, 0, C_MENUW, LNG->AttrZap, wide, ' ', C_MENUW);
    wactiv_(wh_background);
  }
  else if(wh_attributes != -1) {
    wactiv_(wh_attributes);
    wclose();
    wactiv_(wh_background);
    wh_attributes = -1;
  }
}


//  ------------------------------------------------------------------

static void toggle_dispattrwindow() {

  DispAttrWindow();
}


//  ------------------------------------------------------------------

bool ProcessAttrs(gkey &key) {

  switch(key) {
    case KK_HeaderToggleScanned:
      toggle_scanned();
      return true;

    case KK_HeaderToggleGroupmsg:
      toggle_groupmsg();
      return true;

    case KK_HeaderToggleZonegate:
      toggle_zonegate();
      return true;

    case KK_HeaderToggleHubhost:
      toggle_hubhost();
      return true;

    case KK_HeaderToggleRetrecreq:
      toggle_retrecreq();
      return true;

    case KK_HeaderToggleCrash:
      toggle_crash();
      return true;

    case KK_HeaderToggleLocked:
      toggle_locked();
      return true;

    case KK_HeaderToggleReceived:
      toggle_received();
      return true;

    case KK_HeaderToggleLocal:
      toggle_local();
      return true;

    case KK_HeaderToggleRetrec:
      toggle_retrec();
      return true;

    case KK_HeaderToggleFreq:
      toggle_freq();
      return true;

    case KK_HeaderToggleImm:
      toggle_imm();
      return true;

    case KK_HeaderToggleAttrWin:
      toggle_dispattrwindow();
      return true;

    case KK_HeaderToggleArcsent:
      toggle_arcsent();
      return true;

    case KK_HeaderToggleHold:
      toggle_hold();
      return true;

    case KK_HeaderToggleAudit:
      toggle_audit();
      return true;

    case KK_HeaderToggleXmail:
      toggle_xmail();
      return true;

    case KK_HeaderToggleTrunc:
      toggle_trunc();
      return true;

    case KK_HeaderToggleUpdreq:
      toggle_updreq();
      return true;

    case KK_HeaderClearAttrib:
      clear_attrib();
      return true;

    case KK_HeaderToggleKill:
      toggle_kill();
      return true;

    case KK_HeaderToggleTransit:
      toggle_transit();
      return true;

    case KK_HeaderToggleCfmrecreq:
      toggle_cfmrecreq();
      return true;

    case KK_HeaderToggleOrphan:
      toggle_orphan();
      return true;

    case KK_HeaderToggleFile:
      toggle_file();
      return true;

    case KK_HeaderToggleDelsent:
      toggle_delsent();
      return true;

    case KK_HeaderToggleDirect:
      toggle_direct();
      return true;

    case KK_HeaderToggleReserved:
      toggle_reserved();
      return true;

    case KK_HeaderTogglePvt:
      toggle_pvt();
      return true;

    case KK_HeaderToggleSent:
      toggle_sent();
      return true;
  }
  return false;
}

//  ------------------------------------------------------------------

void ChgAttrs(int mode, GMsg* __msg) {

  if(mode) {
    MenuMsgPtr = __msg;

    if(EDIT->HeaderAttrs() or (mode == ALWAYS))
      DispAttrWindow(true);
  }
  else {
    DispAttrWindow(false);
  }
}


//  ------------------------------------------------------------------

void AskAttributes(GMsg* __msg) {

  ChgAttrs(ALWAYS, __msg);
  update_statusline(LNG->ChangeAttrs);
  whelppcat(H_Attributes);

  gkey key;
  do {
    gkey kk;

    key = getxch();
    if(key < KK_Commands) {
      key = key_tolower(key);
      kk = SearchKey(key, HeaderKey, HeaderKeys);
      if(kk)
        key = kk;
    }
  }
  while(ProcessAttrs(key) == true);

  whelpop();
  ChgAttrs(NO, __msg);
}


//  ------------------------------------------------------------------

int SelectFromFile(const char* file, char* selection, const char* title, const char* nolines) {

  char buf[256];
  int n;
  bool retval=false;
  char** Listi;
  int lines = 0;

  FILE* fp = fsopen(AddPath(CFG->goldpath, file), "rt", CFG->sharemode);
  if(fp) {
    while(fgets(buf, sizeof(buf), fp))
      lines++;
  }

  if(lines) {
    Listi = (char**)throw_calloc(lines+1, sizeof(char*));
    rewind(fp);
    for(n=0; n<lines; n++) {
      fgets(buf, sizeof(buf)-2, fp);
      strtrim(buf);
      strins(" ", buf, 0);
      strcat(buf, " ");
      if(strlen(buf) > MAXCOL-2-2)
        buf[MAXCOL-2-2] = NUL;
      Listi[n] = throw_strdup(buf);
    }
    n = MinV(n, (MAXROW-10));
    set_title(title, TCENTER, C_ASKT);
    n = wpickstr(6, 0, 6+n+1, -1, W_BASK, C_ASKB, C_ASKW, C_ASKS, Listi, 0, title_shadow);
    if(n != -1) {
      strcpy(selection, Listi[n]);
      strtrim(selection);
      strltrim(selection);
      retval = true;
    }
    for(n=0; n<lines; n++)
      throw_free(Listi[n]);
    throw_free(Listi);
  }
  else {
    w_info(nolines);
    waitkeyt(10000);
    w_info(NULL);
  }

  if(fp)
    fclose(fp);

  return retval;
}


//  ------------------------------------------------------------------

int ChangeTagline() {

  char buf[256];
  int n;
  bool retval=false;
  char** Listi;

  if(not CFG->tagline.empty()) {
    Listi = (char**)throw_calloc(CFG->tagline.size()+1, sizeof(char*));
    gstrarray::iterator i;
    for(n = 0, i = CFG->tagline.begin(); i != CFG->tagline.end(); i++, n++) {
      if((*i)[0] == '@')
        strxmerge(buf, MAXCOL-2-2, " [", CleanFilename(i->c_str()+1), "] ", NULL);
      else
        strxmerge(buf, MAXCOL-2-2, " ", i->c_str(), " ", NULL);
      Listi[n] = throw_strdup(buf);
    }
    n = MinV(n, (MAXROW-10));
    set_title(LNG->Taglines, TCENTER, C_ASKT);
    update_statusline(LNG->ChangeTagline);
    whelppcat(H_ChangeTagline);
    n = wpickstr(6, 0, 6+n+1, -1, W_BASK, C_ASKB, C_ASKW, C_ASKS, Listi, CFG->taglineno, title_shadow);
    if(n != -1) {
      const char *tagl = CFG->tagline[n].c_str();
      if(tagl[0] == '@') {
        strxmerge(buf, MAXCOL-2-2, LNG->Taglines, " [", CleanFilename(tagl+1), "] ", NULL);
        if(SelectFromFile(tagl+1, buf, LNG->Taglines, LNG->NoTagline)) {
          AA->SetTagline(buf);
          retval = true;
        }
      }
      else {
        CFG->taglineno = n;
        AA->SetTagline(CFG->tagline[n].c_str());
        retval = true;
      }
    }
    for(n=CFG->tagline.size(); n; n--)
      throw_free(Listi[n-1]);
    throw_free(Listi);
    whelpop();
  }
  else {
    w_info(LNG->NoTagline);
    waitkeyt(10000);
    w_info(NULL);
  }
  return(retval);
}


//  ------------------------------------------------------------------

int ChangeOrigin() {

  char buf[256];
  int n;
  bool retval=false;
  char** Listi;

  if(not CFG->origin.empty()) {
    Listi = (char**)throw_calloc(CFG->origin.size()+1, sizeof(char*));
    gstrarray::iterator i;
    for(n = 0, i = CFG->origin.begin(); i != CFG->origin.end(); n++, i++) {
      if((*i)[0] == '@')
        strxmerge(buf, MAXCOL-2-2, " [", CleanFilename(i->c_str()+1), "] ", NULL);
      else
        strxmerge(buf, MAXCOL-2-2, " ", i->c_str(), " ", NULL);
      Listi[n] = throw_strdup(buf);
    }
    n = MinV(n, (MAXROW-10));
    set_title(LNG->Origins, TCENTER, C_ASKT);
    update_statusline(LNG->ChangeOrigin);
    whelppcat(H_ChangeOrigin);
    n = wpickstr(6, 0, 6+n+1, -1, W_BASK, C_ASKB, C_ASKW, C_ASKS, Listi, CFG->originno, title_shadow);
    if(n != -1) {
      const char *orig = CFG->origin[n].c_str();
      if(orig[0] == '@') {
        strxmerge(buf, MAXCOL-2-2, LNG->Origins, " [", CleanFilename(orig+1), "] ", NULL);
        if(SelectFromFile(orig+1, buf, LNG->Origins, LNG->NoOrigDefined)) {
          AA->SetOrigin(buf);
          retval = true;
        }
      }
      else {
        CFG->originno = n;
        AA->SetOrigin(orig);
        retval = true;
      }
    }
    for(n = CFG->origin.size(); n; n--)
      throw_free(Listi[n-1]);
    throw_free(Listi);
    whelpop();
  }
  else {
    w_info(LNG->NoOrigDefined);
    waitkeyt(10000);
    w_info(NULL);
  }
  return(retval);
}


//  ------------------------------------------------------------------

int ChangeUsername() {

  char buf[256], adrs[40];
  int n;
  char** Listi;

  if(not CFG->username.empty()) {
    Listi = (char**)throw_calloc(CFG->username.size()+1, sizeof(char*));
    vector<Node>::iterator i;
    for(n = 0, i = CFG->username.begin(); i != CFG->username.end(); n++, i++) {
      i->addr.make_string(adrs);
      sprintf(buf, " %-35s %s ", i->name, adrs);
      Listi[n] = throw_strdup(buf);
    }
    n = MinV(n, (MAXROW-10));
    set_title(LNG->Usernames, TCENTER, C_ASKT);
    update_statusline(LNG->ChangeUsername);
    whelppcat(H_ChangeUsername);
    n = wpickstr(6, 0, 6+n+1, -1, W_BASK, C_ASKB, C_ASKW, C_ASKS, Listi, CFG->usernameno, title_shadow);
    if(n != -1) {
      CFG->usernameno = n;
      AA->SetUsername(CFG->username[n]);
      for(vector<gaka>::iterator a = CFG->aka.begin(); a != CFG->aka.end(); a++) {
        if(AA->Username().addr.match(a->addr)) {
          AA->SetAka(a->addr);
          break;
        }
      }
    }
    for(n = CFG->username.size(); n; n--)
      throw_free(Listi[n-1]);
    throw_free(Listi);
    whelpop();
  }
  else {
    w_info(LNG->NoUserDefined);
    waitkeyt(10000);
    w_info(NULL);
  }
  return(YES);
}


//  ------------------------------------------------------------------

int ChangeTemplate() {

  char buf[256], adrs[40];
  int n;
  int selected=-1;
  char** Listi;

  if(not CFG->tpl.empty()) {
    Listi = (char**)throw_calloc(CFG->tpl.size()+1, sizeof(char*));
    vector<Tpl>::iterator t;
    for(n = 0, t = CFG->tpl.begin(); t != CFG->tpl.end(); n++, t++) {
      t->match.make_string(adrs);
      sprintf(buf, " %-45s %s ", t->name, adrs);
      Listi[n] = throw_strdup(buf);
    }
    n = MinV(n, (MAXROW-10));
    set_title(LNG->Templates, TCENTER, C_ASKT);
    update_statusline(LNG->ChangeTemplate);
    whelppcat(H_ChangeTemplate);
    n = wpickstr(6, 0, 6+n+1, -1, W_BASK, C_ASKB, C_ASKW, C_ASKS, Listi, CFG->tplno, title_shadow);
    whelpop();
    if(n != -1) {
      AA->SetTpl(CFG->tpl[n].file);
      CFG->tplno = n;
    }
    selected = n;
    for(n = CFG->tpl.size(); n; n--)
      throw_free(Listi[n-1]);
    throw_free(Listi);
  }
  else {
    w_info(LNG->NoTplDefined);
    waitkeyt(10000);
    w_info(NULL);
  }
  return selected;
}


//  ------------------------------------------------------------------

int ChangeAka() {
  int n;
  vector<gaka>::iterator i;
  int startat = 0;
  char** Listi;
  char addr[100], buf[100];

  if(CFG->aka.size() > 1) {
    Listi = (char**)throw_calloc(CFG->aka.size()+1, sizeof(char*));
    for(i = CFG->aka.begin(), n=0; i != CFG->aka.end(); n++, i++) {
      i->addr.make_string(addr, i->domain);
      sprintf(buf, " %s ", addr);
      Listi[n] = throw_strdup(buf);
      if(AA->Aka().addr.equals(i->addr))
        startat = n;
    }
    n = MinV(n, (MAXROW-10));
    set_title(LNG->Akas, TCENTER, C_ASKT);
    update_statusline(LNG->ChangeAka);
    whelppcat(H_ChangeAka);
    n = wpickstr(6, 0, 6+n+1, -1, W_BASK, C_ASKB, C_ASKW, C_ASKS, Listi, startat, title_shadow);
    whelpop();
    if(n != -1)
      AA->SetAka(CFG->aka[n].addr);
    for(n=CFG->aka.size(); n; n--)
      throw_free(Listi[n-1]);
    throw_free(Listi);
  }
  else {
    w_info(LNG->NoAkaDefined);
    waitkeyt(10000);
    w_info(NULL);
  }
  return(YES);
}


//  ------------------------------------------------------------------

int ChangeXlatImport() {

  int n, startat = 0;
  int xlatimports = 1;
  int maximport = 0;
  int maxexport = 0;
  char** Listi;
  char buf[100];

  if(not CFG->xlatcharset.empty()) {
    Listi = (char**)throw_calloc(CFG->xlatcharset.size()+2, sizeof(char*));
    vector<Map>::iterator xlt;
    for(xlt = CFG->xlatcharset.begin(); xlt != CFG->xlatcharset.end(); xlt++) {
      if(strieql(xlt->exp, CFG->xlatlocalset)) {
        maximport = MaxV(maximport, (int)strlen(xlt->imp));
        maxexport = MaxV(maxexport, (int)strlen(xlt->exp));
        if(strieql(xlt->imp, AA->Xlatimport()))
          startat = xlatimports;
        xlatimports++;
      }
    }
    Listi[0] = throw_strdup(LNG->CharsetAuto);
    xlatimports = 1;
    for(xlt = CFG->xlatcharset.begin(); xlt != CFG->xlatcharset.end(); xlt++) {
      if(strieql(xlt->exp, CFG->xlatlocalset)) {
        sprintf(buf, " %*.*s -> %-*.*s ",
          maximport, maximport, xlt->imp,
          maxexport, maxexport, xlt->exp
        );
        Listi[xlatimports++] = throw_strdup(buf);
      }
    }
    n = MinV(xlatimports, (MAXROW-10));
    set_title(LNG->Charsets, TCENTER, C_ASKT);
    update_statusline(LNG->ChangeXlatImp);
    whelppcat(H_ChangeXlatImport);
    n = wpickstr(6, 0, 6+n+1, -1, W_BASK, C_ASKB, C_ASKW, C_ASKS, Listi, startat, title_shadow);
    whelpop();
    if(n == 0) {
      CFG->ignorecharset = false;
    }
    else if(n != -1) {
      CFG->ignorecharset = true;
      AA->SetXlatimport(strtok(Listi[n], " "));
    }
    LoadCharset(AA->Xlatimport(), CFG->xlatlocalset);
    for(n=0; n<xlatimports; n++)
      throw_free(Listi[n]);
    throw_free(Listi);
  }
  else {
    w_info(LNG->NoXlatImport);
    waitkeyt(10000);
    w_info(NULL);
  }
  return true;
}


//  ------------------------------------------------------------------

