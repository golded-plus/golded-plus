
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
//  More menus.
//  ------------------------------------------------------------------

#include <golded.h>


//  ------------------------------------------------------------------

void GMnuAsk::AskInit(int __row, int __col, char* __title, int __help) {

  Init();
  SetColor(C_ASKW, C_ASKQ, C_ASKN, C_ASKS, CFG->switches.get(screenshadows) ? C_SHADOW : -1);
  SetTitle(__title, C_ASKT);
  SetBorder(W_BASK, C_ASKB);
  SetPos(__row, __col);
  SetMask(M_CLOSE);
  SetHelp(__help);
}


//  ------------------------------------------------------------------

int GMnuYesNo::YesNo(int __row, int __col, char* __title, char* __yes, char* __no, int __help, int __yesno) {
  
  enum { TAG_ASKYES=100, TAG_ASKNO };

  HandleGEvent(EVTT_ASKYESNO);

  AskInit(__row, __col, __title, __help);

  Begin();
    Item(TAG_ASKYES, __yes);
    Item(TAG_ASKNO,  __no);
    SetTag(__yesno ? TAG_ASKYES : TAG_ASKNO);
  End();

  Start();

  return (finaltag == TAG_ASKYES) ? YES : NO;
}


//  ------------------------------------------------------------------

int GMenuQuit::Run() {

  int _yn = YesNo(1, 2, LNG->QuitGoldED, LNG->QuitYes, LNG->QuitNo, H_AskExit);
  return _yn ? MODE_QUIT : NO;
}


//  ------------------------------------------------------------------

int GMenuReadonly::Run() {

  int _yn = NO;
  w_info(LNG->ReadOnlyWarn);
  if(CFG->areareadonly == READONLY_SOFT)
    _yn = YesNo(6, 0, LNG->IsReadOnly, LNG->ReadOnlyYes, LNG->ReadOnlyNo, H_AskReadOnly);
  else
    waitkeyt(10000);
  w_info(NULL);
  return _yn;
}


//  ------------------------------------------------------------------

int GMenuChange::Run(char* info) {

  w_info(info);
  int _yn = YesNo(6, 0, LNG->Change, LNG->ChangeYes, LNG->ChangeNo, H_AskChange);
  w_info(NULL);
  return _yn;
}


//  ------------------------------------------------------------------

int GMenuLockLoop::Run() {

  w_info(LNG->WaitOrExit);
  int _yn = YesNo(6, 0, LNG->ReallyExit, LNG->QuitYes, LNG->QuitNo, H_AskExit);
  w_info(NULL);
  return _yn;
}


//  ------------------------------------------------------------------

int GMenuDelorig::Run() {

  return YesNo(6, 0, LNG->DelOrig, LNG->DelOrigYes, LNG->DelOrigNo, H_AskDelOrig);
}


//  ------------------------------------------------------------------

int GMenuDropmsg::Run() {

  return YesNo(6, 0, LNG->DropMsg, LNG->DropMsgYes, LNG->DropMsgNo, H_AskDropMsg, CFG->switches.get(menudropmsg));
}


//  ------------------------------------------------------------------

int GMenuZonegate::Run() {

  return YesNo(6, 0, LNG->ZoneGate, LNG->ZoneGateYes, LNG->ZoneGateNo, H_AskZoneGate);
}


//  ------------------------------------------------------------------

int GMenuForward::Run() {

  return YesNo(6, 0, LNG->Forward, LNG->ForwardYes, LNG->ForwardNo, H_AskForward);
}


//  ------------------------------------------------------------------

int GMenuOverwrite::Run() {

  enum {TAG_APPEND=100, TAG_OVERWRITE, TAG_QUIT };

  HandleGEvent(EVTT_ASKYESNO);

  AskInit(6, 0, LNG->FileExists, H_AskOverWrite);

  Begin();
    Item(TAG_APPEND,    LNG->Append);
    Item(TAG_OVERWRITE, LNG->OverWrite);
    Item(TAG_QUIT,      LNG->QuitExist);
  End();

  Start();

  switch(finaltag) {
    case TAG_OVERWRITE:
      return YES;
    case TAG_APPEND:
      return NO;
  }

  return -1;
}


//  ------------------------------------------------------------------

int GMenuDelete::Run(int all, GMsg* __msg) {

  enum { TAG_YES=100, TAG_NO, TAG_NOASK };

  HandleGEvent(EVTT_ASKYESNO);

  const char* _info = (__msg->attr.uns() and not __msg->attr.rcv()) ? LNG->WarnUnsent : "";
  if(__msg->attr.lok())
    _info = LNG->WarnLocked;

  if(*_info)
    w_info(_info);

  AskInit(6, 0, LNG->DeleteThis, H_AskDelete);

  Begin();
    Item(TAG_YES,   LNG->DeleteYes);
    Item(TAG_NO,    LNG->DeleteNo);
    if(all)
      Item(TAG_NOASK, LNG->DeleteNoAsk);
  End();

  Start();

  if(*_info)
    w_info(NULL);

  switch(finaltag) {
    case TAG_YES:
      return YES;
    case TAG_NOASK:
      return ALWAYS;
  }

  return NO;
}


//  ------------------------------------------------------------------

static void PressY() { if(gkbd.kbuf == NULL) kbput(*LNG->GotoNextYes); }
static void PressN() { if(gkbd.kbuf == NULL) kbput(*LNG->GotoNextNo);  }

int GMenuNewarea::Run() {

  enum { TAG_NO=100, TAG_YES, TAG_JUMP };

  HandleGEvent(EVTT_ENDOFMSGS);

  gkey _yeskey = Key_Rgt;
  gkey _nokey  = Key_Lft;

  if (reader_direction == DIR_PREV)
  {
    _yeskey = Key_Lft;
    _nokey  = Key_Rgt;
  }

  AskInit(6, 0, LNG->GotoNext, H_AskNextArea);

  Begin();
    Item(TAG_YES,  LNG->GotoNextYes, M_CLOSE, PressY, _yeskey);
    Item(TAG_NO,   LNG->GotoNextNo,  M_CLOSE, PressN, _nokey);
    Item(TAG_JUMP, LNG->GotoNextNew);
    SetTag(CFG->menunextarea + TAG_NO);
  End();

  Start();

  // Get the extra character from PressY/N
  if(finaltag != -1 and finaltag != TAG_JUMP)
    getxch();

  if (finaltag == -1)
    finaltag = TAG_NO;
  else if ((finaltag != overtag) && (finaltag == TAG_YES))
    finaltag = overtag;

  return (int) (finaltag - TAG_NO);
}


//  ------------------------------------------------------------------

int GMenuCMF::Run() {

  enum { TAG_COPY=100, TAG_MOVE, TAG_FORWARD, TAG_TOGGLESENT, TAG_QUIT };

  static int _prevtag = TAG_FORWARD;

  update_statusline(LNG->CopyMoveForward);

  AskInit(6, 0, LNG->SelectAction, H_CopyMoveForward);

  Begin();
    Item(TAG_FORWARD,     LNG->ForwardMessage);
    Item(TAG_MOVE,        LNG->MoveMessage);
    Item(TAG_COPY,        LNG->CopyMessage);
    Item(TAG_TOGGLESENT,  LNG->ToggleSent);
    Item(TAG_QUIT,        LNG->QuitCMF);
    SetTag(_prevtag);
  End();

  Start();

  _prevtag = finaltag;
  switch(finaltag) {
    case TAG_FORWARD:   return MODE_FORWARD;
    case TAG_MOVE:      return MODE_MOVE;
    case TAG_COPY:      return MODE_COPY;
    case TAG_TOGGLESENT:return MODE_UPDATE;
  }
  _prevtag = TAG_FORWARD;
  return -1;
}


//  ------------------------------------------------------------------

int GMenuDomarks::Run(char* dowhat) {
                                    
  enum { TAG_MARKED=100, TAG_CURRENT, TAG_QUIT };

  static int _prevtag = TAG_CURRENT;
  if(CFG->menumarked == MODE_MARKED)
    _prevtag = TAG_MARKED;
  else if(CFG->menumarked == MODE_CURRENT)
    _prevtag = TAG_CURRENT;
  else if(CFG->menumarked == MODE_DONT)
    _prevtag = AA->Mark.Count() ? TAG_MARKED : TAG_CURRENT;

  update_statuslinef("%i %s %s", AA->Mark.Count(), AA->Mark.Count() == 1 ? LNG->is : LNG->are, LNG->marked);

  AskInit(6, 0, dowhat, H_AskDoMarks);

  Begin();
    Item(TAG_MARKED,  LNG->MarkedMsg);
    Item(TAG_CURRENT, LNG->CurrentMsg);
    Item(TAG_QUIT,    LNG->QuitMsgs);
    SetTag(_prevtag);
  End();

  Start();

  _prevtag = finaltag;
  switch(finaltag) {
    case TAG_CURRENT:   return MODE_CURRENT;
    case TAG_MARKED:    return MODE_MARKED;
  }
  return MODE_DONT;
}


//  ------------------------------------------------------------------

void do_togglehdr() {
  ++AA->adat->writeheader; AA->adat->writeheader %= 3;
  gwin.cmenu->citem->str = (AA->Writeheader() == 1 ? LNG->WriteToHdrYES : AA->Writeheader() == 2 ? LNG->WriteToHdrONLY : LNG->WriteToHdrNO) + 1;
  gwin.cmenu->citem->redisp = true;
}

int GMenuWriteMsg::Run() {

  enum {
    TAG_DISKFILE=100,
    TAG_PRINTER,
    TAG_CLIPBRD,
    TAG_TOGGLEHDR,
    TAG_QUIT
  };

  static int _prevtag = TAG_DISKFILE;

  update_statusline(LNG->WriteMsgsTo);

  AskInit(6, 0, LNG->WriteTo, H_WriteMessage);
  Begin();
    Item(TAG_DISKFILE,       LNG->Diskfile);
    Item(TAG_PRINTER,        LNG->Printer);
    Item(TAG_CLIPBRD,        LNG->Clipboard);
    Item(TAG_TOGGLEHDR,      (AA->Writeheader() == 1 ? LNG->WriteToHdrYES : AA->Writeheader() == 2 ? LNG->WriteToHdrONLY : LNG->WriteToHdrNO), 0, do_togglehdr);
    Item(TAG_QUIT,           LNG->QuitWrite);
    SetTag(_prevtag);
  End();

  Start();

  _prevtag = finaltag;
  int header = AA->Writeheader() == 1 ? 0 : AA->Writeheader() == 2 ? WRITE_ONLY_HEADER : WRITE_NO_HEADER;
  switch(finaltag) {
    case TAG_DISKFILE:        return WRITE_FILE|header;
    case TAG_PRINTER:         return WRITE_PRINTER|header;
    case TAG_CLIPBRD:         return WRITE_CLIPBRD|header;
  }
  _prevtag = TAG_DISKFILE;
  return -1;
}


//  ------------------------------------------------------------------

static void do_changexlatimport() { ChangeXlatImport(); wmenuinext(100); }

int GMenuImportTxt::Run() {

  enum {
    TAG_TEXT = 100,
    TAG_QUOTE,
    TAG_BINUUE,
    TAG_BINMIME,
    TAG_CLIPBRD,
    TAG_XLAT,
    TAG_QUIT
  };

  // Dirk: Don't put a menu in the middle of the screen.
  // AskInit((MAXROW-8)/2+3, (MAXCOL-strlen(LNG->ImportTxtText))/2, LNG->ImportTxt, H_ImportTxt);
  AskInit(6, 0, LNG->ImportTxt, H_ImportTxt);
  Begin();
    Item(TAG_TEXT   , LNG->ImportTxtText);
    Item(TAG_QUOTE  , LNG->ImportTxtQuote);
    Item(TAG_BINUUE , LNG->ImportTxtUue);
    Item(TAG_BINMIME, LNG->ImportTxtMime);
    Item(TAG_CLIPBRD, LNG->ImportTxtClip);

    if(not CFG->xlatcharset.empty())
      Item(TAG_XLAT   , LNG->ImportTxtXlat, 0, do_changexlatimport);

    Item(TAG_QUIT   , LNG->ImportTxtQuit);
  End();

  vcurhide();
  Start();
  vcurshow();

  return finaltag - TAG_TEXT;
}


//  ------------------------------------------------------------------

int GMenuMarkMsgs::Run() {
                                    
  static int _prevtag = TAG_MARKYOURMAIL;

  update_statusline(LNG->MarkingOptions);

  AskInit(6, 0, LNG->MarkWhat, H_MarkMessages);
  Begin();
    Item(TAG_MARKYOURMAIL, LNG->Yourmail);
    Item(TAG_MARKHEADER,   LNG->FromToSubj);
    Item(TAG_MARKTXTHDR,   LNG->TextHdr);
    Item(TAG_MARKTHREAD,   LNG->Thread);
    Item(TAG_MARKASREAD,   LNG->AsRead);
    Item(TAG_MARKNEWMSGS,  LNG->NewMsgs);
    Item(TAG_MARKOLDMSGS,  LNG->OldMsgs);
    Item(TAG_MARKALLMSGS,  LNG->AllMsgs);
    Item(TAG_MARKUNMARK,   LNG->Unmark);
    Item(TAG_MARKRANGE,    LNG->Range);
    Item(TAG_MARKTOGGLE,   LNG->Markstoggle);
    Item(TAG_MARKQUIT,     LNG->QuitMarks);
    SetTag(_prevtag);
  End();

  Start();

  if(finaltag == -1)
    return -1;
  else
    _prevtag = finaltag;

  return _prevtag;
}


//  ------------------------------------------------------------------

static void _ask_attributes() {

  AskAttributes(MenuMsgPtr);
}


//  ------------------------------------------------------------------

int GMenuEditfile::Run(GMsg* __msg) {

  if(not EDIT->SaveMenu())
    return MODE_SAVE;

  enum {
    TAG_YES = 100,
    TAG_NO,
    TAG_CONTINUE,
    TAG_ROT13,
    TAG_ATTRS,
    TAG_TAGLINE,
    TAG_ORIGIN,
    TAG_VIEW,
    TAG_HEADER,
    TAG_UTILS = 1000
  };

  MenuMsgPtr = __msg;

  int _topline = 0;

  for(;;) {

    update_statuslinef(LNG->Quotepct, __msg->quotepct, __msg->quotepct > 80 ? "!!!" : "");
    
    char _title[80];
    sprintf(_title, LNG->SaveMsg, __msg->lines);
    AskInit(6, 0, _title, H_SaveMsg);

    Begin();
      Item(TAG_YES,      LNG->YesGreat);
      Item(TAG_NO,       LNG->Kickit);
      Item(TAG_CONTINUE, LNG->Continue);
      Item(TAG_ROT13,    LNG->Rot13,    M_CLOSE);
      Item(TAG_ATTRS,    LNG->AttribS,  _ask_attributes, 0);
      if(not CFG->tagline.empty())
        Item(TAG_TAGLINE,  LNG->TaglineS);
      Item(TAG_ORIGIN,   LNG->OriginS);
      Item(TAG_VIEW,     LNG->View);
      Item(TAG_HEADER,   LNG->HeaderEdit);
      if(EDIT->SaveUtil.First()) {
        int n = 0;
        do {
          Item(TAG_UTILS+(n++), (char*)EDIT->SaveUtil.Text());  // WARNING!!!
        } while(EDIT->SaveUtil.Next());
      }
    End();

    Start();

    switch(finaltag) {

      case TAG_YES:
        return MODE_SAVE;

      case -1:
      case TAG_NO:
        {
          GMenuDropmsg MenuDropmsg;
          if(MenuDropmsg.Run())
            return MODE_QUIT;
        }
        break;

      case TAG_CONTINUE:
        return MODE_CHANGE;

      case TAG_ROT13:
        Rot13(__msg);
        BodyView->Use(AA, __msg, _topline);
        BodyView->Paint();
        break;

      case TAG_TAGLINE:
        if(ChangeTagline()) {
          char _buf[256];
          strcpy(_buf, AA->Tagline());
          if(*_buf == '@')
            GetRandomLine(_buf, sizeof(_buf), _buf+1);
          strcpy(__msg->tagline, _buf);
          Line* _line = __msg->lin;
          while(_line) {
            if(_line->type & GLINE_TAGL) {
              sprintf(_buf, "%c%c%c %s ", AA->Taglinechar(), AA->Taglinechar(), AA->Taglinechar(), __msg->tagline);
              strtrim(_buf);
              _line->txt = _buf;
              break;
            }
            _line = _line->next;
          }
        }
        BodyView->Use(AA, __msg, _topline);
        BodyView->Paint();
        break;

      case TAG_ORIGIN:
        if(ChangeOrigin()) {
          char _buf[256];
          strcpy(_buf, AA->Origin());
          if(*_buf == '@')
            GetRandomLine(_buf, sizeof(_buf), _buf+1);
          MakeOrigin(__msg, _buf);
          Line* _line = __msg->lin;
          while(_line) {
            if(_line->type & GLINE_ORIG) {
              sprintf(_buf, " * Origin: %s ", __msg->origin);
              strtrim(_buf);
              _line->txt = _buf;
              break;
            }
            _line = _line->next;
          }
        }
        BodyView->Use(AA, __msg, _topline);
        BodyView->Paint();
        break;

      case TAG_VIEW:
        return MODE_VIEW;

      case TAG_HEADER:
        EditHeaderinfo(MODE_CHANGE, *HeaderView);
        break;

      default:
        if(finaltag >= TAG_UTILS) {
          XlatName __oldxlatimport;
          // Save current charset
          strcpy(__oldxlatimport, AA->Xlatimport());
          AA->SetXlatimport(CFG->xlatlocalset);
          ExternUtil(__msg, EDIT->SaveUtil.Number(finaltag-TAG_UTILS));
          AA->SetXlatimport(__oldxlatimport);
          BodyView->Use(AA, __msg, _topline);
          BodyView->Paint();
        }
    }
  }

  return MODE_SAVE;
}


//  ------------------------------------------------------------------

enum {
  TAG_CCPROCESS = 100,
  TAG_CCIGNORE,
  TAG_CCATTRS,
  TAG_CCLIST,
    TAG_CCLISTKEEP,
    TAG_CCLISTNAMES,
    TAG_CCLISTVISIBLE,
    TAG_CCLISTHIDDEN,
    TAG_CCLISTREMOVE
};

static void SetCCListKeep()    { CFG->carboncopylist = CC_KEEP; }
static void SetCCListNames()   { CFG->carboncopylist = CC_NAMES; }
static void SetCCListVisible() { CFG->carboncopylist = CC_VISIBLE; }
static void SetCCListHidden()  { CFG->carboncopylist = CC_HIDDEN; }
static void SetCCListRemove()  { CFG->carboncopylist = CC_REMOVE; }

int GMenuCarbon::Run(GMsg* __msg) {

  MenuMsgPtr = __msg;

  int _listtag = TAG_CCLISTKEEP;
  switch(CFG->carboncopylist) {
    case CC_KEEP:     _listtag = TAG_CCLISTKEEP;      break;
    case CC_NAMES:    _listtag = TAG_CCLISTNAMES;     break;
    case CC_VISIBLE:  _listtag = TAG_CCLISTVISIBLE;   break;
    case CC_HIDDEN:   _listtag = TAG_CCLISTHIDDEN;    break;
    case CC_REMOVE:   _listtag = TAG_CCLISTREMOVE;    break;
  }

  AskInit(6, 0, LNG->CarbonCopy, H_CarbonCopy);

  Begin();
    Item(TAG_CCPROCESS, LNG->CCProcess);
    Item(TAG_CCIGNORE,  LNG->CCIgnore);
    Item(TAG_CCATTRS,   LNG->CCAttribs, _ask_attributes, 0);
    Item(TAG_CCLIST,    LNG->CCListFmt);
    SetTitle(LNG->CCList);
    Begin();
      Item(TAG_CCLISTKEEP,    LNG->CCListKeep,    SetCCListKeep);
      Item(TAG_CCLISTNAMES,   LNG->CCListNames,   SetCCListNames);
      Item(TAG_CCLISTVISIBLE, LNG->CCListVisible, SetCCListVisible);
      Item(TAG_CCLISTHIDDEN,  LNG->CCListHidden,  SetCCListHidden);
      Item(TAG_CCLISTREMOVE,  LNG->CCListRemove,  SetCCListRemove);
      SetTag(_listtag);
    End();
  End();

  SetEsc(false);

  Start();

  return finaltag == TAG_CCPROCESS;
}


//  ------------------------------------------------------------------

static void SetXCListKeep()    { CFG->crosspostlist = CC_KEEP; }
static void SetXCListNames()   { CFG->crosspostlist = CC_NAMES; }
static void SetXCListVisible() { CFG->crosspostlist = CC_VISIBLE; }
static void SetXCListRemove()  { CFG->crosspostlist = CC_REMOVE; }

int GMenuCross::Run(GMsg* __msg) {

  MenuMsgPtr = __msg;

  int _listtag = TAG_CCLISTKEEP;
  switch(CFG->crosspostlist) {
    case CC_KEEP:     _listtag = TAG_CCLISTKEEP;      break;
    case CC_NAMES:    _listtag = TAG_CCLISTNAMES;     break;
    case CC_VISIBLE:  _listtag = TAG_CCLISTVISIBLE;   break;
    case CC_REMOVE:   _listtag = TAG_CCLISTREMOVE;    break;
  }

  AskInit(6, 0, LNG->Crosspost, H_Crosspost);

  Begin();
    Item(TAG_CCPROCESS, LNG->XCProcess);
    Item(TAG_CCIGNORE,  LNG->XCIgnore);
    Item(TAG_CCLIST,    LNG->XCListFmt);
    SetTitle(LNG->XCList);
    Begin();
      Item(TAG_CCLISTKEEP,    LNG->XCListKeep,    SetXCListKeep);
      Item(TAG_CCLISTNAMES,   LNG->XCListNames,   SetXCListNames);
      Item(TAG_CCLISTVISIBLE, LNG->XCListVisible, SetXCListVisible);
      Item(TAG_CCLISTREMOVE,  LNG->XCListRemove,  SetXCListRemove);
      SetTag(_listtag);
    End();
  End();

  SetEsc(false);

  Start();

  return finaltag == TAG_CCPROCESS;
}


//  ------------------------------------------------------------------

int GMnuAreaDo::With(char* __title, char* __all, char* __marked, char* __current, int __full, char* __no, int __help) {

  enum {
    TAG_ALL=100,
    TAG_MARKED,
    TAG_CURRENT,
    TAG_MATCHING,
    TAG_UNSCANNED,
    TAG_GROUP,
    TAG_QWK,
      TAG_IMPORTQWK,
      TAG_EXPORTQWK,
    TAG_SOUP,
      TAG_EXPORTSOUP,
      TAG_IMPORTSOUP,
    TAG_NO,
    TAG_LIST = 1000
  };

  AskInit(1,2, __title, __help);

  Begin();
    Item(TAG_ALL, __all);
    Item(TAG_MARKED, __marked);
    Item(TAG_CURRENT, __current);
    if(__full) {
      Item(TAG_MATCHING, LNG->Scan_Matching);
      Item(TAG_UNSCANNED, LNG->Scan_Unscanned);
      Item(TAG_GROUP, LNG->ScanGroup);
      if(AL.ListScan.First()) {
        int count = 0;
        do {
          Item(TAG_LIST+count, (char*)AL.ListScan.MenuText());  // WARNING!!!
          count++;
        } while(AL.ListScan.Next());
      }
      if(*QWK->ImportPath()) {
      Item(TAG_QWK, LNG->ScanQWK);
        SetTitle(LNG->QWKPacket);
        Begin();
          Item(TAG_IMPORTQWK, LNG->ScanImport, M_CLALL);
          Item(TAG_EXPORTQWK, LNG->ScanExport, M_CLALL);
        End();
      }
      if(*CFG->soupimportpath) {
      Item(TAG_SOUP, LNG->ScanSoup);
        SetTitle(LNG->SoupPacket);
        Begin();
          Item(TAG_IMPORTSOUP, LNG->ScanImport, M_CLALL);
          Item(TAG_EXPORTSOUP, LNG->ScanExport, M_CLALL);
        End();
      }
      if(*area_maybe)
        SetTag(TAG_MATCHING);
    }
    else
      SetTag(TAG_CURRENT);

    Item(TAG_NO, __no);
  End();

  Start();

  if(finaltag >= TAG_LIST) {
    AL.ListScan.CurrNo(finaltag-TAG_LIST);
    return SCAN_LIST;
  }

  switch(finaltag) {
    case TAG_ALL:         return SCAN_ALL;
    case TAG_GROUP:       return SCAN_GROUP;
    case TAG_MARKED:      return SCAN_MARKED;
    case TAG_CURRENT:     return SCAN_CURRENT;
    case TAG_MATCHING:    return SCAN_MATCHING;
    case TAG_UNSCANNED:   return SCAN_UNSCANNED;
    case TAG_EXPORTQWK:   return SCAN_EXPORTQWK;
    case TAG_IMPORTQWK:   return SCAN_IMPORTQWK;
    case TAG_EXPORTSOUP:  return SCAN_EXPORTSOUP;
    case TAG_IMPORTSOUP:  return SCAN_IMPORTSOUP;
  }

  return SCAN_QUIT;
}


//  ------------------------------------------------------------------

int GMenuAreaScan::Run(int pmscan) {

  return With((pmscan ? LNG->ScanPM : LNG->Scan_Areas), LNG->Scan_All, LNG->Scan_Marked, LNG->Scan_Current, true, LNG->No_Scan, H_ScanAreas);
}


//  ------------------------------------------------------------------

int GMenuAreaHeat::Run() {

  return With(LNG->Heat_Areas, LNG->Heat_All, LNG->Heat_Marked, LNG->Heat_Current, false, LNG->No_Heat, H_HeatAreas);
}


//  ------------------------------------------------------------------

int GMenuAreaZap::Run() {

  return With(LNG->Zap_Areas, LNG->Zap_All, LNG->Zap_Marked, LNG->Zap_Current, false, LNG->No_Zap, H_ZapAreas);
}


//  ------------------------------------------------------------------

int GMenuAreaCatchup::Run() {

  return With(LNG->CatchAreas, LNG->CatchAll, LNG->CatchMarked, LNG->CatchCurrent, false, LNG->NoCatch, H_AreaCatchUp);
}


//  ------------------------------------------------------------------

int GMenuAreaDropMarks::Run() {

  return With(LNG->DropMarks, LNG->DropAll, LNG->DropMarked, LNG->DropCurrent, false, LNG->NoDrop, H_AreaDropMarks);
}


//  ------------------------------------------------------------------

int GMenuEditHeader::Run(int mode, GMsg* msg) {

  enum {
    TAG_INTERNAL = 100,
    TAG_EXTERNAL,
    TAG_SAVE,
    TAG_ATTRS,
    TAG_TEMPLATE,
    TAG_TAGLINE,
    TAG_ORIGIN,
    TAG_QUIT
  };

  HeaderView->Use(AA, msg);
  HeaderView->Paint();

  int _tag;
  bool _again;

  int doedithdr = EDIT->HeaderFirst();
  if(doedithdr) {
    switch(mode) {
      case MODE_NEW:
        doedithdr &= EDITHEADERFIRST_NEW;
        break;
      case MODE_CHANGE:
        doedithdr &= EDITHEADERFIRST_CHANGES;
        break;
      case MODE_FORWARD:
        doedithdr &= EDITHEADERFIRST_FORWARDS;
        break;
      default:
        doedithdr &= EDITHEADERFIRST_REPLIES;
    }
  }

  do {

    update_statusline(LNG->EditHeader);

    _tag = EditHeaderinfo(mode, *HeaderView, make_bool(doedithdr));
    if((_tag == W_ESCPRESS) or gkbd.quitall)
      break;

    do {

      _again = false;

      _tag = (EDIT->Internal() ? TAG_INTERNAL : (*EDIT->External() ? TAG_EXTERNAL : TAG_INTERNAL));

      if(EDIT->Menu()) {

        Init();
        SetColor(C_MENUW, C_MENUQ, C_MENUN, C_MENUS, CFG->switches.get(screenshadows) ? C_SHADOW : -1);
        SetTitle(LNG->MsgOptions, C_MENUT);
        SetBorder(W_BMENU, C_MENUB);
        SetPos(6, 0);
        SetMask(0);
        SetHelp(H_Message);

        Begin();
          Item(TAG_INTERNAL, LNG->InternalEd);
          if(*EDIT->External())
            Item(TAG_EXTERNAL, LNG->ExternalEd);
          Item(TAG_SAVE,     LNG->SaveNoEdit);
          Item(TAG_ATTRS,    LNG->AttrO);
          Item(TAG_TEMPLATE, LNG->Template);
          if(not CFG->tagline.empty())
            Item(TAG_TAGLINE,  LNG->Tagline);
          Item(TAG_ORIGIN,   LNG->Origin);
          Item(TAG_QUIT,     LNG->QuitMessage);
          SetTag(_tag);
        End();
        Start();
        _tag = finaltag;
      }
      if(_tag == -1)
        _tag = W_ESCPRESS;
      else {
        switch(_tag) {
          case TAG_ATTRS:
            AskAttributes(msg);
            CheckSubject(msg, msg->re);
            HeaderView->Use(AA, msg);
            HeaderView->Paint();
            _again = true;
            break;
          case TAG_ORIGIN:
            if(ChangeOrigin()) {
              if(*AA->Origin() == '@') {
                char o[128];
                GetRandomLine(o, sizeof(o), AA->Origin()+1);
                AA->SetOrigin(o);
              }
              strxcpy(msg->origin, AA->Origin(), sizeof(msg->origin));
            }
            _again = true;
            break;
          case TAG_TAGLINE:
            if(ChangeTagline()) {
              if(*AA->Tagline() == '@') {
                char t[76];
                GetRandomLine(t, sizeof(t), AA->Tagline()+1);
                AA->SetTagline(t);
              }
              strcpy(msg->tagline, AA->Tagline());
            }
            _again = true;
            break;
          case TAG_TEMPLATE:
            if(ChangeTemplate() != -1)
              AA->SetTpl(CFG->tpl[CFG->tplno].file);
            _again = true;
            break;
        }
      }
      update_statusline(LNG->EditHeader);
    } while(_again);
  } while((_tag == W_ESCPRESS) and doedithdr);

  HeaderView->Use(AA, msg);
  HeaderView->Paint();
  
  if(not gkbd.quitall) {
    switch(_tag) {
      case W_ESCPRESS:
      case TAG_QUIT:
        return MODE_QUIT;
      case TAG_EXTERNAL:
        EDIT->Internal(false);
        return MODE_CHANGE;
      case TAG_INTERNAL:
        EDIT->Internal(true);
        return MODE_CHANGE;
      case TAG_SAVE:
        return NO;
    }
  }
  return MODE_QUIT;
}


//  ------------------------------------------------------------------

int GMenuNavigate::Run() {

  enum {
    TAG_MAIN = 100,
      TAG_MAIN_SHELL    = (int)KK_ReadDosShell,
      TAG_MAIN_QUIT     = (int)KK_ReadQuitNow,
    TAG_EDIT = 101,
      TAG_EDIT_ENTER    = (int)KK_ReadNewMsg,
      TAG_EDIT_QUOTE    = (int)KK_ReadQuoteMsg,
      TAG_EDIT_DIRQUOTE = (int)KK_ReadDirQuoteMsg,
      TAG_EDIT_COMMENT  = (int)KK_ReadCommentMsg,
      TAG_EDIT_OTHER = 1010,
        TAG_EDIT_OTHER_QUOTE    = (int)KK_ReadMoveQuoteMsg,
        TAG_EDIT_OTHER_DIRQUOTE = (int)KK_ReadMoveQuoteMsg,
        TAG_EDIT_OTHER_COMMENT  = (int)KK_ReadMoveCommentMsg,
    TAG_SEL = 102,
      TAG_SEL_ORIGIN    = (int)KK_ReadChangeOrigin,
      TAG_SEL_USERNAME  = (int)KK_ReadChangeUsername,
      TAG_SEL_AKA       = (int)KK_ReadChangeAka,
      TAG_SEL_ATTRS     = (int)KK_ReadChangeAttrs,
      TAG_SEL_TEMPLATE  = (int)KK_ReadChangeTemplate,
    TAG_UTIL = 103,
      TAG_UTIL_CMF      = (int)KK_ReadCopyMoveForward,
      TAG_UTIL_LIST     = (int)KK_ReadMessageList,
      TAG_UTIL_WRITE    = (int)KK_ReadWriteMsg,
      TAG_UTIL_FREQ     = (int)KK_ReadFileRequest,
    TAG_END = 104
  };

  Init();
  SetColor(C_ASKW, C_ASKQ, C_ASKN, C_ASKS);
  SetTitle(NULL, C_ASKT);
  SetBorder(5, C_ASKB);
  SetPos(6, 0, MAXCOL, 1);
  SetMask(M_CLALL);
  SetHelp(H_MainMenu);

  Begin(M_HORZ);
    SetBorder(W_BASK, C_ASKB);
    SetColor(C_ASKW, C_ASKQ, C_ASKN, C_ASKS, CFG->switches.get(screenshadows) ? C_SHADOW : -1);
    Item(TAG_MAIN, LNG->MMMain);
    BeginPullDown();
      Item(TAG_MAIN_SHELL, LNG->MMShell);
      Item(TAG_MAIN_QUIT,  LNG->MMQuit);
    End();
    Item(TAG_EDIT, LNG->MMEdit);
    BeginPullDown();
      Item(TAG_EDIT_ENTER,    LNG->MMNew);
      Item(TAG_EDIT_QUOTE,    LNG->MMQuote);
      Item(TAG_EDIT_DIRQUOTE, LNG->MMDirQuote);
      Item(TAG_EDIT_COMMENT,  LNG->MMComment);
      Item(TAG_EDIT_OTHER,    LNG->MMOtherAreaReply);
      SetTitle(LNG->MMOtherArea);
      Begin();
        Item(TAG_EDIT_OTHER_QUOTE,    LNG->MMOQuote);
        Item(TAG_EDIT_OTHER_DIRQUOTE, LNG->MMODirQuote);
        Item(TAG_EDIT_OTHER_COMMENT,  LNG->MMOComment);
      End();
    End();
    Item(TAG_SEL,  LNG->MMChange);
    BeginPullDown();
      Item(TAG_SEL_ORIGIN,   LNG->MMOrigin);
      Item(TAG_SEL_USERNAME, LNG->MMUsername);
      Item(TAG_SEL_AKA,      LNG->MMAka);
      Item(TAG_SEL_ATTRS,    LNG->MMAttrs);
      Item(TAG_SEL_TEMPLATE, LNG->MMTemplate);
    End();
    Item(TAG_UTIL, LNG->MMUtil);
    BeginPullDown();
      Item(TAG_UTIL_CMF,   LNG->MMCopyMoveForward);
      Item(TAG_UTIL_LIST,  LNG->MMList);
      Item(TAG_UTIL_WRITE, LNG->MMWrite);
      Item(TAG_UTIL_FREQ,  LNG->MMFreq);
    End();
  End();
  Start();
  if((finaltag >= TAG_MAIN) and (finaltag <= TAG_END))
    finaltag = -1;
  return finaltag;
}


//  ------------------------------------------------------------------

int GMenuConfirm::Run() {

  w_info(LNG->ConfirmInfo);
  int _yn = YesNo(6, 0, LNG->Confirm, LNG->ConfirmYes, LNG->ConfirmNo, H_MenuConfirm);
  w_info(NULL);
  return _yn;
}


//  ------------------------------------------------------------------

#if defined(__GOLD_SPELL__)
int GMenuSChecker::Run(CSpellChecker &schecker, const char *word)
{
  enum
  {
    TAG_ADDWORD = 100,
    TAG_LANG    = 0x10000,
    TAG_MORE    = 0x20000,
    TAG_INDEX   = 0x30000
  };

  schecker.Check(word);
  CSpellSuggestV &suggest = schecker.Suggest();

  std::string title;
  if (!*word)
    title = " SChecker ";
  else
  {
    title = " ";
    title += word;
    title += " ";
  }

  Init();
  SetColor(C_ASKW, C_ASKQ, C_ASKN, C_ASKS, CFG->switches.get(screenshadows) ? C_SHADOW : -1);
  SetTitle(title.c_str(), C_ASKT, TLEFT);
  SetPos(6, 0, title.length()+2, 0);
  SetBorder(W_BASK, C_ASKB);
  SetMask(M_CLALL);
  SetHelp(0);

  Begin();
  SetTitle("");

  size_t idx;
  size_t levels = 0;
  size_t numrows = 7;

  CSpellLangV &langs = schecker.GetLangs();
  LIDC lidc = schecker.GetLangCode();

  std::vector<std::string> langstr;
  size_t langcount = langs.size();

  for (idx = 0; idx < langcount; idx++)
  {
    char buff[10];
    LIDC code = langs[idx].GetLangCode();

    buff[0] = ' ';
    buff[1] = (code == lidc) ? '\x10' : ' ';

    itoa(code, &buff[2], 10);
    strcat(buff, " ");

    langstr.push_back(std::string(buff));
  }

  if (langcount)
  {
    Item(TAG_LANG, "L Language... ", 0);
    numrows++;

    SetPos(numrows, 1, 0, 0);
    Begin();

    for (idx = 0; idx < langcount; idx++)
      Item(TAG_LANG+idx+1, langstr[idx].c_str());
    
    End();
  }

  if (*word)
  {
    Item(TAG_ADDWORD, "A Add Word... ");
    numrows++;
  }

  if (suggest.size())
  {
    ItemSep();
    numrows++;
  }

  for (idx = 0; idx < suggest.size(); idx++)
  {
    Item(TAG_INDEX + idx, suggest[idx].second.c_str());

    if ((numrows == (gvid->numrows - 5)) && (idx < suggest.size()-3))
    {
      ItemSep();
      Item(TAG_MORE + levels, "M More... ", 0);
      
      levels++;
      numrows = levels + 6;

      SetPos(numrows, levels, title.length()+2, 0);
      BeginPullDown();
    }

    numrows++;
  }

  for (size_t n = 0; n < levels; n++)
    End();

  if (suggest.size())
    SetTag(TAG_INDEX);

  End();

  vcurhide();
  Start();
  vcurshow();

  if (finaltag == TAG_ADDWORD)
  {
    schecker.AddWord();
    return -2;
  }
  else if ((finaltag > TAG_LANG) && (finaltag < TAG_MORE))
  {
    schecker.Load(langs[finaltag-TAG_LANG-1].GetLangCode(), CFG->scheckeruserdic);
    return -2;
  }

  return (finaltag < TAG_INDEX) ? -1 : (finaltag - TAG_INDEX);
}
#endif


//  ------------------------------------------------------------------

