
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
//  Message lister.
//  ------------------------------------------------------------------

#include <golded.h>


//  ------------------------------------------------------------------

extern GMsg* reader_msg;

static GMsg* MLstMsgPtr;
static GMsg* mlstmsg = NULL;
static uint msgmark2;
static MLst* mlst;
static int mlst_bysiz;
static int mlst_tosiz;
static int mlst_resiz;
static int fldadd1;
static int fldadd2;


//  ------------------------------------------------------------------

const byte MLST_HIGH_FROM   =  1;
const byte MLST_HIGH_TO     =  2;
const byte MLST_HIGH_BOOK   =  4;
const byte MLST_HIGH_MARK   =  8;
const byte MLST_HIGH_UNREAD = 16;
const byte MLST_HIGH_UNSENT = 32;


//  ------------------------------------------------------------------

inline void mlst_with_date(int with_date) {

  if(with_date) {
    mlst_bysiz = 19;
    mlst_tosiz = 19;
    mlst_resiz = 20;
  }
  else {
    mlst_bysiz = 19+3;
    mlst_tosiz = 19+3;
    mlst_resiz = 20+4;
  }
}


//  ------------------------------------------------------------------

static void mlst_dodelayed(PInf* p) {
  
  // Update header and statusline

  if(AA->Msglistheader()) {
    AA->LoadMsg(MLstMsgPtr, mlst[p->pos].msgno, CFG->dispmargin-(int)CFG->switches.get(disppagebar));
    mlst[p->pos].goldmark = goldmark;
    int mlstwh = whandle();
    HeaderView->Use(AA, MLstMsgPtr);
    HeaderView->Paint();
    wactiv_(mlstwh);
  }

  if(CFG->switches.get(msglistviewsubj))
    wtitle(mlst[p->pos].re, TCENTER|TBOTTOM, p->tattr);

  if(CFG->switches.get(msglistpagebar))
    wscrollbar(W_VERT, p->maxidx+1, p->maxidx, p->idx);

  update_statuslinef(LNG->MsgLister, p->idx+1, p->maxidx+1, p->maxidx-p->idx);
}


//  ------------------------------------------------------------------

static void mlst_dispbuf(char* abuf, MLst* ml) {

  int bysiz = mlst_bysiz + fldadd1;
  int tosiz = mlst_tosiz + fldadd1;
  int resiz = mlst_resiz + fldadd2;

  if(AA->Msglistwidesubj()) {
    resiz += tosiz + 1;
    tosiz = 0;
  }

  char nbuf[33], dbuf[20];
  strcpy(dbuf, LNG->n_a);

  time_t dt = 0;
  switch(AA->Msglistdate()) {
    case MSGLISTDATE_WRITTEN:   dt = ml->written;   break;
    case MSGLISTDATE_ARRIVED:   dt = ml->arrived;   break;
    case MSGLISTDATE_RECEIVED:  dt = ml->received;  break;
  }
  if(dt)
    strftimei(dbuf, 20, "%d %b %y", gmtime(&dt));
  if(AA->Msglistdate())
    strsetsz(dbuf, 10);
  else
    *dbuf = NUL;
  sprintf(nbuf, "%5lu", CFG->switches.get(disprealmsgno) ? ml->msgno : AA->Msgn.ToReln(ml->msgno));
  sprintf(abuf, "%-5.5s%s%-*.*s %-*.*s%s%-*.*s %s",
    nbuf, ml->marks,
    bysiz, bysiz, ml->by,
    tosiz, tosiz, ml->to,
    (tosiz ? " " : ""),
    resiz, resiz, ml->re,
    dbuf
  );
}


//  ------------------------------------------------------------------

static void mlst_dispit(PInf* p, uint pos, int type) {

  int bycol = 7;
  int tocol = bycol + mlst_bysiz + 1 + fldadd1;
  int bysiz = mlst_bysiz + fldadd1;
  int tosiz = mlst_tosiz + fldadd1;

  MLst* ml = &mlst[pos];

  int wattr, hattr, mattr=p->hattr;
  if(type == PICK_BAR) {
    wattr = p->sattr;
    hattr = p->sattr;
    mattr = p->sattr;
  }
  else if(ml->high & MLST_HIGH_UNSENT) {
    wattr = C_MENUW_UNSENT;
    hattr = C_MENUQ_UNSENTHIGH;
  }
  else if(ml->high & MLST_HIGH_UNREAD) {
    wattr = C_MENUW_UNREAD;
    hattr = C_MENUQ_UNREADHIGH;
  }
  else {
    wattr = p->wattr;
    hattr = p->hattr;
  }

  char buf[256];
  mlst_dispbuf(buf, ml);
  wprints(pos, 0, wattr, buf);

  if(ml->high & (MLST_HIGH_BOOK|MLST_HIGH_MARK))
    wprints(pos, 5, mattr, ml->marks);
  if(ml->high & MLST_HIGH_FROM)
    wprintns(pos, bycol, hattr, ml->by, bysiz);
  if((ml->high & MLST_HIGH_TO) and not AA->Msglistwidesubj())
    wprintns(pos, tocol, hattr, ml->to, tosiz);

  goldmark = ml->goldmark;
}


//  ------------------------------------------------------------------

static void mlst_disp(PInf* p) {

  mlst_dispit(p, p->pos, p->type);
}


//  ------------------------------------------------------------------

static void mlst_get_mlst(MLst* ml, PInf* p, int n) {

  GMsg* msg = mlstmsg;

  ml->high = 0;

  strcpy(ml->marks, "  ");

  ml->msgno = AA->Msgn.CvtReln(p->idx + n + 1);

  if(AA->bookmark == AA->Msgn.CvtReln(p->idx + n + 1)) {
    ml->marks[0] = MMRK_BOOK;
    ml->high |= MLST_HIGH_BOOK;
  }

  if(AA->Mark.Count()) {
    if(AA->Mark.Find(ml->msgno)) {
      ml->marks[1] = MMRK_MARK;
      ml->high |= MLST_HIGH_MARK;
    }
  }

  if(AA->Msglistfast()) {
    AA->LoadHdr(msg, ml->msgno);
  }
  else {
    AA->LoadMsg(msg, ml->msgno, CFG->dispmargin-(int)CFG->switches.get(disppagebar));
  }
  ml->goldmark = goldmark;

  for(vector<Node>::iterator x = CFG->username.begin(); x != CFG->username.end(); x++) {
    if(strieql(msg->By(), x->name)) {
      ml->high |= MLST_HIGH_FROM;
      msg->attr.fmu1();
    }
    if(strieql(msg->to, x->name)) {
      ml->high |= MLST_HIGH_TO;
      msg->attr.tou1();
    }
  }
  if(strieql(msg->to, AA->Internetaddress())) {
    ml->high |= MLST_HIGH_TO;
    msg->attr.tou1();
  }

  // Highlight FROM if local
  if(CFG->switches.get(displocalhigh) and msg->attr.loc())
    ml->high |= MLST_HIGH_FROM;

  // Highlight if unread
  if(msg->timesread == 0 and CFG->switches.get(highlightunread))
    ml->high |= MLST_HIGH_UNREAD;

  // Highlight if unsent
  if(msg->attr.uns()and not msg->attr.rcv() and not msg->attr.del())
    ml->high |= MLST_HIGH_UNSENT;

  ml->written = msg->written;
  ml->arrived = msg->arrived;
  ml->received = msg->received;
  strcpy(ml->by, msg->By());
  strcpy(ml->to, msg->To());
  strcpy(ml->re, msg->re);
}


//  ------------------------------------------------------------------

static void mlst_page(PInf* p) {

  char linebuf[200];
  vchar vlinebuf[200];
  uint m, n;

  if(not AA->Msglistfast())
    w_info(LNG->Wait);

  p->idx -= p->pos;
  m = p->maxidx - p->idx;

  for(n=0; n<=p->maxpos and n<=m; n++)
    mlst_get_mlst(&mlst[n], p, n);

  if(not AA->Msglistfast())
    w_info(NULL);

  for(n=0; n<=p->maxpos and n<=m; n++)
    mlst_dispit(p, n, (n==p->pos) ? PICK_BAR : PICK_DISP);

  for(int c = 0; c < MAXCOL-2; c++)
    vlinebuf[c] = _box_table(p->btype, 1);
  vlinebuf[MAXCOL-2] = NUL;

  if(n <= p->maxpos or (p->maxpos == p->maxidx and p->maxpos < (p->height-3)))
    wprintvs(n, 0, p->wattr|ACSET, vlinebuf);

  memset(linebuf, ' ', MAXCOL-2);
  linebuf[MAXCOL-2] = NUL;

  for(++n; n<=p->height-2; n++)
    wprints(n, 0, p->wattr, linebuf);
  p->idx += p->pos;
}


//  ------------------------------------------------------------------

static void mlst_center(PInf* p) {

  uint botroom = p->maxidx - p->idx;

  switch(CFG->displistcursor) {
    case LIST_TOP:
      if(botroom > p->maxpos)
        p->pos = 0;
      else
        p->pos = p->maxpos - botroom;
      break;
    case LIST_NEARTOP:
      {
        uint room;
        uint toproom = p->idx;
        if(toproom > (p->maxpos/4)) {
          if(botroom > (p->maxpos/4))
            room = p->maxpos/4;
          else if(botroom)
            room = p->maxpos - botroom;
          else
            room = p->maxpos;
        }
        else
          room = toproom;
        p->pos = room;
      }
      break;
    case LIST_MIDDLE:
      {
        uint room;
        uint toproom = p->idx;
        if(toproom > (p->maxpos/2)) {
          if(botroom > (p->maxpos/2))
            room = p->maxpos/2;
          else if(botroom)
            room = p->maxpos - botroom;
          else
            room = p->maxpos;
        }
        else
          room = toproom;
        p->pos = room;
      }
      break;
    case LIST_NEARBOTTOM:
      {
        uint room;
        uint toproom = p->idx;
        if(toproom > 3*(p->maxpos/4)) {
          if(botroom > 3*(p->maxpos/4))
            room = 3*(p->maxpos/4);
          else if(botroom)
            room = p->maxpos - botroom;
          else
            room = p->maxpos;
        }
        else
          room = toproom;
        p->pos = room;
      }
      break;
    case LIST_BOTTOM:
      p->pos = p->maxpos;
      break;
  }
  mlst_page(p);
}


//  ------------------------------------------------------------------

static void mlst_scroll(PInf* p, int direction) {

  if(direction == SUP) {
    memmove(&mlst[0], &mlst[1], p->maxpos*sizeof(MLst));
    mlst_get_mlst(&mlst[p->maxpos], p, 0);
  }
  else {
    memmove(&mlst[1], &mlst[0], p->maxpos*sizeof(MLst));
    mlst_get_mlst(&mlst[0], p, 0);
  }
  wscroll(1, direction);
}


//  ------------------------------------------------------------------

static void mlst_title(PInf* p) {

  int bycol = 8;
  int tocol = bycol + mlst_bysiz + 1 + fldadd1;
  int recol = tocol + mlst_tosiz + 1 + fldadd1;
  int dtcol = recol + mlst_resiz + 1 + fldadd2;
  if(AA->Msglistwidesubj())
    recol = tocol;

  wtitle(NULL, TCENTER, p->tattr);
  wmessage(CFG->switches.get(disprealmsgno) ? LNG->MsgReal : LNG->Msg, TP_BORD, 3, p->tattr);
  wmessage(LNG->FromL, TP_BORD, bycol, p->tattr);
  if(not AA->Msglistwidesubj())
    wmessage(LNG->ToL, TP_BORD, tocol, p->tattr);
  wmessage(LNG->SubjL, TP_BORD, recol, p->tattr);
  switch(AA->Msglistdate()) {
    case MSGLISTDATE_WRITTEN:   wmessage(LNG->Written, TP_BORD, dtcol, p->tattr);   break;
    case MSGLISTDATE_ARRIVED:   wmessage(LNG->Arrived, TP_BORD, dtcol, p->tattr);   break;
    case MSGLISTDATE_RECEIVED:  wmessage(LNG->Received, TP_BORD, dtcol, p->tattr);  break;
  }
}


//  ------------------------------------------------------------------

static void mlst_open(PInf* p) {

  wopen_(p->row, p->column, p->height, p->width, p->btype, p->battr, p->wattr, p->sbattr);
  mlst_title(p);
  mlst_center(p);
}


//  ------------------------------------------------------------------

static void mlst_close(PInf* p) {

  NW(p);
  wclose();
}


//  ------------------------------------------------------------------

static int mlst_dokey(PInf* p, gkey* keycode) {

  int tpos;
  gkey key, kk;
  uint temp, tmpmsgno;

  key = *keycode;
  *keycode = 0;

  if(key < KK_Commands) {
    key = key_tolower(key);
    kk = SearchKey(key, ListKey, ListKeys);
    if(kk)
      key = kk;
  }

  // See if it's a listkey
  switch(key) {
    case KK_ListGotoPrev:
    case KK_ListGotoNext:
    case KK_ListGotoFirst:
    case KK_ListGotoLast:
    case KK_ListAskExit:
    case KK_ListQuitNow:
    case KK_ListAbort:
    case KK_ListSelect:
    case KK_ListToggleMark:
    case KK_ListToggleBookMark:
    case KK_ListGotoBookMark:
    case KK_ListMarkingOptions:
    case KK_ListDosShell:
    case KK_ListMacro:
    case Key_0:
    case Key_1:
    case Key_2:
    case Key_3:
    case Key_4:
    case Key_5:
    case Key_6:
    case Key_7:
    case Key_8:
    case Key_9:
      break;

    // If not a listkey, see if it matches a readkey
    default:
      if(not IsMacro(key, KT_M)) {
        kk = SearchKey(key, ReadKey, ReadKeys);
        if(kk)
          key = kk;
      }
  }

  switch(key) {
    case KK_ListGotoPrev:
      *keycode = Key_Up;
      break;

    case KK_ListGotoNext:
      *keycode = Key_Dwn;
      break;

    case KK_ListGotoFirst:
      *keycode = Key_Home;
      break;

    case KK_ListGotoLast:
      *keycode = Key_End;
      break;

    case KK_ListAskExit:
      {
        GMenuQuit MenuQuit;
        p->aborted = gkbd.quitall = (MenuQuit.Run());
        if(gkbd.quitall) {
          AA->bookmark = AA->Msgn.CvtReln(msgmark2);
          return NO;
        }
      }
      break;

    case KK_ListQuitNow:
      p->aborted = gkbd.quitall = true;
      AA->bookmark = AA->Msgn.CvtReln(msgmark2);
      return NO;

    case KK_ListAbort:
      AA->bookmark = AA->Msgn.CvtReln(msgmark2);
      p->aborted = YES;
      ///////////////// Drop Through

    case KK_ListSelect:
      return NO;

    case KK_ListToggleMark:
      temp = AA->Mark.Find(mlst[p->pos].msgno);
      if(temp) {
        AA->Mark.DelReln(temp);
        mlst[p->pos].marks[1] = ' ';
        mlst[p->pos].high &= ~MLST_HIGH_MARK;
      }
      else {
        AA->Mark.Add(mlst[p->pos].msgno);
        mlst[p->pos].marks[1] = MMRK_MARK;
        mlst[p->pos].high |= MLST_HIGH_MARK;
      }
      if(p->idx < p->maxidx) {
        mlst_disp(p);
        *keycode = Key_Dwn;
      }
      break;

    case KK_ListToggleBookMark:
      if(AA->bookmark == mlst[p->pos].msgno) {
        mlst[p->pos].marks[0] = ' ';
        AA->bookmark = 0;
        mlst[p->pos].high &= ~MLST_HIGH_BOOK;
        mlst_disp(p);
      }
      else {
        temp = AA->Msgn.ToReln(AA->bookmark-1);
        AA->bookmark = mlst[p->pos].msgno;
        mlst[p->pos].marks[0] = MMRK_BOOK;
        mlst[p->pos].high |= MLST_HIGH_BOOK;
        mlst_disp(p);
        if(temp) {
          tpos = p->pos;
          p->pos += (temp - p->idx);
          if(p->pos <= p->maxpos) {
            mlst[p->pos].marks[0] = ' ';
            mlst[p->pos].high &= ~MLST_HIGH_BOOK;
            mlst_disp(p);
          }
          p->pos = tpos;
        }
      }
      break;

    case KK_ListGotoBookMark:
      if(AA->bookmark) {
        tpos = p->pos + ((AA->Msgn.ToReln(AA->bookmark-1)) - p->idx);
        temp = p->idx + 1;
        p->idx = AA->Msgn.ToReln(AA->bookmark-1);
        AA->bookmark = AA->Msgn.CvtReln(temp);
        if(tpos >= 0 and (uint)tpos <= p->maxpos) {
          mlst[tpos].marks[0] = ' ';
          mlst[tpos].high &= ~MLST_HIGH_BOOK;
          mlst[p->pos].marks[0] = MMRK_BOOK;
          mlst[p->pos].high |= MLST_HIGH_BOOK;
          mlst_disp(p);
          p->pos = tpos;
          mlst_disp(p);
        }
        else
          mlst_center(p);
      }
      else
        SayBibi();
      break;

    case KK_ListMarkingOptions:
      {
        uint lrbak = AA->lastread();
        AA->set_lastread(p->idx + 1);
        MLstMsgPtr->msgno = AA->Msgn.CvtReln(AA->lastread());
        MarkMsgs(MLstMsgPtr);
        AA->set_lastread(lrbak);
        mlst_page(p);
      }
      break;

    case KK_ListDosShell:
      DosShell();
      break;

    case KK_ListToggleWideSubj:
      AA->ToggleMsglistwidesubj();
      mlst_title(p);
      mlst_page(p);
      break;

    case KK_ListToggleDate:
      AA->NextMsglistdate();
      mlst_with_date(AA->Msglistdate());
      mlst_title(p);
      mlst_page(p);
      break;

    case Key_0:
    case Key_1:
    case Key_2:
    case Key_3:
    case Key_4:
    case Key_5:
    case Key_6:
    case Key_7:
    case Key_8:
    case Key_9:
    case KK_ReadGotoMsgno:
      tmpmsgno = p->idx;
      reader_keycode = key;
      GotoMsgno();
      if(AA->lastread()-1 != tmpmsgno) {
        p->idx = AA->lastread()-1;
        mlst_center(p);
      }
      break;

    case KK_ReadMessageList:
      mlst_center(p);
      break;

    case Key_Tick:
      CheckTick(KK_ListQuitNow);
      break;

    case KK_ListUndefine:
      break;

    default:
      if(not PlayMacro(key, KT_M)) {
        switch(key) {
          case KK_ReadNewArea:
            p->aborted = true;
        }
        if(gkbd.kbuf == NULL)
          kbput(key);
        return NO;
      }
  }
  return YES;
}


//  ------------------------------------------------------------------

uint MsgBrowser(GMsg* msg) {

  GFTRK("MsgBrowser");

  Pick pick;
  PInf p;

  memset(&p, 0, sizeof(PInf));
  memset(&pick, 0, sizeof(Pick));
  p.btype = W_BMENU;
  p.battr = C_MENUB;
  p.wattr = C_MENUW;
  p.sattr = C_MENUS;
  p.tattr = C_MENUT;
  p.hattr = C_MENUQ;
  p.sbattr = C_MENUPB;
  p.row = AA->Msglistheader() ? 6 : 1;
  p.height = MAXROW-p.row-1;
  p.width = MAXCOL;
  p.helpcat = H_MessageBrowser;
  p.delay = 150;  // milliseconds
  p.listwrap = CFG->switches.get(displistwrap);

  pick.open      = mlst_open;
  pick.close     = mlst_close;
  pick.disp      = mlst_disp;
  pick.page      = mlst_page;
  pick.scroll    = mlst_scroll;
  pick.dokey     = mlst_dokey;
  pick.dodelayed = mlst_dodelayed;
  
  p.maxidx = AA->Msgn.Count()-1;
  p.maxpos = MinV((uint)(p.height-3), p.maxidx);
  p.aborted = NO;

  p.idx = AA->Msgn.ToReln(msg->msgno)-1;
  msgmark2 = AA->Msgn.ToReln(AA->bookmark);

  MLstMsgPtr = msg;

  mlst = (MLst*)throw_calloc(p.maxpos+2, sizeof(MLst));
  mlstmsg = (GMsg*)throw_calloc(1, sizeof(GMsg));

  if(AA->Msglistdate() != MSGLISTDATE_NONE) {
    if(AA->Msglistdate() != MSGLISTDATE_WRITTEN) {
      if(AA->ishudson() or AA->isgoldbase() or AA->ispcboard())
        AA->SetMsglistdate(MSGLISTDATE_WRITTEN);
      else if((AA->isezycom() or AA->isfido()) and (AA->Msglistdate() == MSGLISTDATE_RECEIVED))
        AA->SetMsglistdate(MSGLISTDATE_WRITTEN);
    }
  }

  mlst_with_date(AA->Msglistdate());

  fldadd1 = (MAXCOL-80)/3;
  fldadd2 = (MAXCOL-80) - (fldadd1*2);

  _in_msglist = true;

  Picker(&p, &pick);

  _in_msglist = false;

  ResetMsg(mlstmsg);
  throw_release(mlstmsg);
  throw_release(mlst);

  GFTRK(NULL);

  if(not p.aborted)
    return p.idx+1;

  return 0;
}


//  ------------------------------------------------------------------

void MessageBrowse() {

  if(AA->Msgn.Count()) {
    uint temp = AA->lastread();
    AA->set_lastread(MsgBrowser(reader_msg));
    if(AA->lastread() == 0)
      AA->set_lastread(temp);
    if(AA->PMrk.Tags() == 0)
      AA->isreadpm = false;
    if(AA->Mark.Count() == 0)
      AA->isreadmark = false;
    if(gkbd.quitall)
      QuitNow();
  }
}


//  ------------------------------------------------------------------

#include <vector>

class ThreadEntry {

public:
  ulong msgno;
  ulong replyto;
  ulong reply1st;
  ulong replynext;
};

#define MAX_LEVEL 20

class GThreadlist : public gwinpick {

private:

  gwindow               window;
  GMsg                  msg;
  vector<ThreadEntry>   list;
  ThreadEntry           t;
  uint                  h_offset;

  void BuildThreadIndex(dword msgno);
  void recursive_build(ulong msgn, ulong rn);
  void GenTree(char* buf2, int idx);
  void update_title();
  bool NextThread(bool next);

public:

  void open();                        // Called after window is opened
  void close();                       // Called after window is closed
  void print_line(uint idx, uint pos, bool isbar);
  void do_delayed();
  bool handle_key();                  // Handles keypress

  void Run();

  GThreadlist() { memset(&msg, 0, sizeof(GMsg)); };
  ~GThreadlist() { ResetMsg(&msg); };

};


//  ------------------------------------------------------------------

void GThreadlist::open() {

  window.openxy(ypos, xpos, ylen+2, xlen+2,  btype, battr, 7);
  update_title();

  center(CFG->displistcursor);
}


//  ------------------------------------------------------------------

void GThreadlist::update_title() {

  window.title(title, tattr);
  window.message(CFG->switches.get(disprealmsgno) ? LNG->MsgReal : LNG->Msg, TP_BORD, 3, tattr);

  switch(AA->Msglistdate()) {
    case MSGLISTDATE_WRITTEN:   window.message(LNG->Written, TP_BORD, xlen-9, tattr);   break;
    case MSGLISTDATE_ARRIVED:   window.message(LNG->Arrived, TP_BORD, xlen-9, tattr);   break;
    case MSGLISTDATE_RECEIVED:  window.message(LNG->Received, TP_BORD, xlen-9, tattr);  break;
  }
}


//  ------------------------------------------------------------------

void GThreadlist::do_delayed() {
  
  // Update header and statusline
  if(AA->Msglistheader()) {
    AA->LoadMsg(&msg, list[index].msgno, CFG->dispmargin-(int)CFG->switches.get(disppagebar));
    int mlstwh = whandle();
    HeaderView->Use(AA, &msg);
    HeaderView->Paint();
    wactiv_(mlstwh);
  }

  if(CFG->switches.get(msglistviewsubj))
    wtitle(msg.re, TCENTER|TBOTTOM, tattr);

  if(CFG->switches.get(msglistpagebar))
    wscrollbar(W_VERT, maximum_index+1, maximum_index, index);

  update_statuslinef(LNG->MsgLister, index+1, maximum_index+1, maximum_index-index);
}


//  ------------------------------------------------------------------

void GThreadlist::close() {

  window.close();
  ResetMsg(&msg);
}


//  ------------------------------------------------------------------

void GThreadlist::GenTree(char* buf, int idx) {

#ifdef KOI8
  static char graph[4]="†„";
#else
  static char graph_ibmpc[4]="ÃÀ³";
  static char graph[]="";

  if(graph[0] == NUL) {
    int table = LoadCharset(NULL, NULL, 1);
    int level = LoadCharset("IBMPC", CFG->xlatlocalset);
    XlatStr(graph, graph_ibmpc, level, CharTable);
    if(table == -1)
      LoadCharset(CFG->xlatimport, CFG->xlatlocalset);
    else
      LoadCharset(CFG->xlatcharset[table].imp, CFG->xlatcharset[table].exp);
  }
#endif

  t = list[idx];

  uint level = 0;
  char* q = &buf[1000];

  *q-- = NUL;
  *q-- = ' ';
  *q-- = (t.replynext) ? graph[0] : graph[1];

  while(t.replyto) {
    for(uint i=0; i<list.size(); i++) {
      if(list[i].msgno == t.replyto) {
        t = list[i];
        level++;
        break;
      }
    }
    *q-- = ' ';
    *q-- = (t.replynext) ? graph[2] :  ' ';
  }

  t = list[idx];

  if(level) {
    q+=2;
    memcpy(buf, q, strlen(q)+1);
  }
  else {
    buf[0] = ' '; buf[1] = NUL;
  }
}


//  ------------------------------------------------------------------

void GThreadlist::print_line(uint idx, uint pos, bool isbar) {

  char buf[256];
  char buf2[1001];
  int attrh, attrw;
  uint tdlen;

  t = list[idx];

  tdlen = xlen - ((AA->Msglistdate() == MSGLISTDATE_NONE) ? 8 : 18);

  AA->LoadHdr(&msg, t.msgno);

  if(msg.attr.uns() and not msg.attr.rcv() and not msg.attr.del()) {
    attrw = C_MENUW_UNSENT;
    attrh = C_MENUQ_UNSENTHIGH;
  }
  else if(CFG->switches.get(highlightunread) and (msg.timesread == 0)) {
    attrh = C_MENUQ_UNREADHIGH;
    attrw = C_MENUW_UNREAD;
  }
  else {
    attrw = wattr;
    attrh = hattr;
  }

  GenTree(buf2, idx);
  
  #if defined(__UNIX__) and not defined(__USE_NCURSES__)
  gvid_boxcvt(buf2);
  #endif
  
  char marks[3];

  strcpy(marks, "  ");

  if(AA->bookmark == t.msgno)
    marks[0] = MMRK_BOOK;

  if(AA->Mark.Count()) {
    if(AA->Mark.Find(t.msgno))
      marks[1] = MMRK_MARK;
  }

  sprintf(buf, "%6lu  %*c", CFG->switches.get(disprealmsgno) ? t.msgno : AA->Msgn.ToReln(t.msgno), tdlen, ' ');

  if(AA->Msglistdate() != MSGLISTDATE_NONE) {
    char dbuf[11];
    time_t dt = 0;

    memset(dbuf, ' ', 10);
    dbuf[10] = NUL;
    strncpy(dbuf, LNG->n_a, strlen(LNG->n_a));

    switch(AA->Msglistdate()) {
      case MSGLISTDATE_WRITTEN:   dt = msg.written;   break;
      case MSGLISTDATE_ARRIVED:   dt = msg.arrived;   break;
      case MSGLISTDATE_RECEIVED:  dt = msg.received;  break;
    }

    if(dt)
      strftimei(dbuf, 20, "%d %b %y", gmtime(&dt));
    strcat(buf, dbuf);
  }

  window.prints(pos, 0, isbar ? sattr : attrw, buf);
  window.prints(pos, 6, isbar ? sattr : hattr, marks);

  if(strlen(buf2) > h_offset) {
    strxcpy(buf, &buf2[h_offset], tdlen);
    window.prints(pos, 8, isbar ? (sattr|ACSET) : (wattr|ACSET), buf);
  }

  int attr = attrw;

  for(vector<Node>::iterator x = CFG->username.begin(); x != CFG->username.end(); x++)
    if(strieql(msg.By(), x->name)) {
      attr = attrh;
      break;
    }

  if((strlen(buf2) > h_offset) and (strlen(&buf2[h_offset]) < tdlen)) {
    strxcpy(buf, msg.By(), tdlen - strlen(&buf2[h_offset]));
    window.prints(pos, 8+strlen(&buf2[h_offset]), isbar ? sattr : attr, buf);
  }
}


//  ------------------------------------------------------------------

void GThreadlist::recursive_build(ulong msgn, ulong rn) {

  ulong oldmsgno = msg.msgno;

  if(AA->Msgn.ToReln(msgn) and AA->LoadHdr(&msg, msgn)) {

    t.msgno     = msgn;
    t.replyto   = msg.link.to();
    t.reply1st  = msg.link.first();
    t.replynext = rn;

    if(not AA->Msgn.ToReln(t.replyto))
      t.replyto = 0;
    if(not AA->Msgn.ToReln(t.reply1st))
      t.reply1st = 0;
    if(not AA->Msgn.ToReln(t.replynext))
      t.replynext = 0;

    uint j;
    bool found = false;
    for(j=0; j<list.size(); j++) {
      if(list[j].msgno == t.replyto) {
        found = true;
        break;
      }
    }

    if(found and list[j].msgno != t.replyto)
      list.erase(&list[j]);

    if(found or list.size() == 0)
      list.push_back(t);

    recursive_build(msg.link.first(), msg.link.list(0));

    for(int n=0; n < msg.link.list_max()-1; n++) {
      if(msg.link.list(n)) {
        recursive_build(msg.link.list(n), msg.link.list(n+1));
      } else
        break;
    }
    AA->LoadHdr(&msg, oldmsgno);
  }
}


//  ------------------------------------------------------------------

void GThreadlist::BuildThreadIndex(dword msgn) {

  w_info(LNG->Wait);

  index = maximum_index = position = maximum_position = 0;
  list.clear();

  AA->LoadHdr(&msg, msgn);

  ulong msgno = msg.link.to();

  // Search backwards
  while(AA->Msgn.ToReln(msgno)) {

    if(not AA->LoadHdr(&msg, msgno))
      msg.link.to_set(0);

    msgno = msg.link.to();
  }

  recursive_build(msg.msgno, 0);

  w_info(NULL);

  minimum_index    = 0;
  maximum_index    = list.size() - 1;
  maximum_position = MinV((uint) list.size() - 1, (uint) ylen - 1);
  index            = 0;
  h_offset         = 0;

  for(uint i = 0; i<list.size(); i++) {
    if(list[i].msgno == msgn)
      index = i;
  }
}


//  ------------------------------------------------------------------

bool GThreadlist::NextThread(bool next) {

  for(uint m = AA->Msgn.ToReln(reader_msg->msgno)-1;
      next ? m < AA->Msgn.Count() : m;
      next ? m++ : m--) {

    dword msgn = AA->Msgn[m];
    bool found = false;

    for(uint i = 0; i<list.size(); i++) {
      if(list[i].msgno == msgn) {
        found = true;
        break;
      }
    }

    if(not found) {
      reader_msg->msgno = msgn;
      AA->set_lastread(AA->Msgn.ToReln(msgn));

      BuildThreadIndex(msgn);
      return true;
    }
  }
  return true;
}

//  ------------------------------------------------------------------


bool GThreadlist::handle_key() {

  gkey kk;

  if(key < KK_Commands) {
    key = key_tolower(key);
    kk = SearchKey(key, ListKey, ListKeys);
    if(kk)
      key = kk;
  }

  // See if it's a listkey
  switch(key) {
    case KK_ListGotoPrev:
    case KK_ListGotoNext:
    case KK_ListGotoFirst:
    case KK_ListGotoLast:
    case KK_ListAskExit:
    case KK_ListQuitNow:
    case KK_ListAbort:
    case KK_ListSelect:
    case KK_ListToggleMark:
    case KK_ListToggleBookMark:
    case KK_ListGotoBookMark:
    case KK_ListMarkingOptions:
    case KK_ListDosShell:
    case Key_0:
    case Key_1:
    case Key_2:
    case Key_3:
    case Key_4:
    case Key_5:
    case Key_6:
    case Key_7:
    case Key_8:
    case Key_9:
      break;

    // If not a listkey, see if it matches a readkey
    default:
      if(not IsMacro(key, KT_M)) {
        kk = SearchKey(key, ReadKey, ReadKeys);
        if(kk)
          key = kk;
      }
  }

  switch(key) {
    case Key_C_PgUp:
    case Key_C_PgDn:
      NextThread((key == Key_C_PgDn));
      if(list.size() <= 1)
        return false;
      center(CFG->displistcursor);
      break;
    case KK_ListGotoPrev:
    case KK_ListGotoNext:
      NextThread((key == KK_ListGotoNext));
      if(list.size() <= 1)
        return false;
      center(CFG->displistcursor);
      break;
    case KK_ListGotoFirst: precursor(); cursor_first(); break;
    case KK_ListGotoLast:  precursor(); cursor_last();  break;

    case KK_ListAskExit:
      {
        GMenuQuit MenuQuit;
        aborted = gkbd.quitall = (MenuQuit.Run());
      }
      break;

    case KK_ListQuitNow:
      gkbd.quitall = true;
      ///////////////// Drop Through

    case KK_ListAbort:
      aborted = true;
      ///////////////// Drop Through

    case KK_ListSelect:
      return false;

    case KK_ListToggleMark:
    {
      ulong temp = AA->Mark.Find(list[index].msgno);
      if(temp) {
        AA->Mark.DelReln(temp);
      }
      else {
        AA->Mark.Add(list[index].msgno);
      }

      if(index < maximum_index)
        cursor_down();
      else
        display_bar();
      break;
    }

    case KK_ListDosShell:
      DosShell();
      break;

    case KK_ListToggleDate:
      AA->NextMsglistdate();
      mlst_with_date(AA->Msglistdate());
      update_title();
      update();
      break;

    case Key_Tick:
      CheckTick(KK_ListQuitNow);
      break;

    case KK_ListUndefine:
      break;

    default:
      if(not PlayMacro(key, KT_M)) {
        if(gkbd.kbuf == NULL)
          kbput(key);
        switch(key) {
          case KK_ListAbort:
          case KK_ReadNewArea:
            aborted = true;
        }
        return NO;
      }
  }
  return true;
}


//  ------------------------------------------------------------------

void GThreadlist::Run() {

  ypos    = AA->Msglistheader() ? 6 : 1;      // Window Starting Row
  xpos    = 0;                                // Window Starting Column
  ylen    = MAXROW-3-ypos;                    // Window Height
  xlen    = MAXCOL-2;                         // Window Width
  btype   = W_BMENU;                          // Window Border Type
  battr   = C_MENUB;                          // Window Border Color
  wattr   = C_MENUW;                          // Window Color
  tattr   = C_MENUT;                          // Window Title Color
  sattr   = C_MENUS;                          // Window Selection Bar Color
  hattr   = C_MENUQ;                          // Window Highlight Color
  sbattr  = C_MENUPB;                         // Window Scrollbar Color
  title   = LNG->ThreadlistTitle;             // Window Title
  helpcat = H_ReplyThread;                    // Window Help Category
  listwrap  = CFG->switches.get(displistwrap);

  BuildThreadIndex(reader_msg->msgno);

  if(list.size() > 1)
    run_picker();

  if(list.size() <= 1) {
    w_info(LNG->NoThreadlist);
    waitkeyt(5000);
    w_info(NULL);
    aborted = true;
  }

  if(not aborted)
    AA->set_lastread(AA->Msgn.ToReln(list[index].msgno));
}


//  ------------------------------------------------------------------

void MsgThreadlist() {

  GThreadlist p;

  p.Run();

}


//  ------------------------------------------------------------------
