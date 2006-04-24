
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
//  Reader secondary functions
//  ------------------------------------------------------------------

#include <golded.h>
#include <gwinput.h>
#include <gftnall.h>
#include <vector>
#define PROTOTYPES
#include <uudeview.h>


//  ------------------------------------------------------------------
//  Global data from GEREAD

extern GMsg* reader_msg;
extern bool reader_msglistfirst;


//  ------------------------------------------------------------------

void ChangeAttributes() {

  if(reader_msg->msgno) {

    // Reload if hexdumped
    if(AA->attr().hex()) {
      AA->attr().hex0();
      AA->LoadMsg(reader_msg, reader_msg->msgno, CFG->dispmargin-(int)CFG->switches.get(disppagebar));
    }

    GMsg* msg = (GMsg*)throw_malloc(sizeof(GMsg));
    memcpy(msg, reader_msg, sizeof(GMsg));
    AskAttributes(reader_msg);
    if(memcmp(msg, reader_msg, sizeof(GMsg))) {
      msg->charsetlevel = LoadCharset(CFG->xlatlocalset, msg->charset);
      DoKludges(MODE_CHANGE, reader_msg, GKLUD_FLAGS);
      reader_msg->LinesToText();
      AA->SaveMsg(GMSG_UPDATE, reader_msg);
    }
    throw_free(msg);
  }
}


//  ------------------------------------------------------------------

void DecMargin() {

  if(CFG->dispmargin > (MAXCOL/3))
    CFG->dispmargin--;
  else {
    SayBibi();
    reader_keyok = YES;
  }
}


//  ------------------------------------------------------------------

void IncMargin() {

  if(CFG->dispmargin < MAXCOL)
    CFG->dispmargin++;
  else {
    SayBibi();
    reader_keyok = YES;
  }
}

//  ------------------------------------------------------------------

void ToggleMark() {

  uint tmp = AA->Mark.Find(reader_msg->msgno);
  if(tmp)
    AA->Mark.DelReln(tmp);
  else
    AA->Mark.Add(reader_msg->msgno);
  HeaderView->Use(AA, reader_msg);
  HeaderView->Paint();
  reader_keyok = YES;
}


//  ------------------------------------------------------------------

void ToggleBookMark() {

  if(AA->bookmark == reader_msg->msgno)
    AA->bookmark = 0;
  else
    AA->bookmark = reader_msg->msgno;
  HeaderView->Use(AA, reader_msg);
  HeaderView->Paint();
  reader_keyok = YES;
}


//  ------------------------------------------------------------------

void ToggleHiddKlud() {

  AA->ToggleViewhidden();
  AA->ToggleViewkludge();
  MsgLineReIndex(reader_msg);
  reader_topline = 0;
  reader_keyok = YES;
}


//  ------------------------------------------------------------------

void ToggleKludge() {

  AA->ToggleViewkludge();
  MsgLineReIndex(reader_msg);
  reader_topline = 0;
  reader_keyok = YES;
}


//  ------------------------------------------------------------------

void ToggleHidden() {

  AA->ToggleViewhidden();
  MsgLineReIndex(reader_msg);
  reader_topline = 0;
  reader_keyok = YES;
}


//  ------------------------------------------------------------------

void ToggleStripHTML() {

  AA->ToggleStripHTML();
  MsgLineReIndex(reader_msg);
  reader_topline = 0;
}


//  ------------------------------------------------------------------

void ToggleQuote() {

  AA->ToggleViewquote();
  MsgLineReIndex(reader_msg);
  reader_topline = 0;
  reader_keyok = YES;
}


//  ------------------------------------------------------------------

void ToggleROT13() {

  if(AA->Msgn.Count()) {
    reader_msg->attr.rotX();
    Rot13(reader_msg);
    reader_keyok = YES;
  }
}


//  ------------------------------------------------------------------

void TogglePageBar() {

  CFG->switches.set(disppagebar, not CFG->switches.get(disppagebar));
}


//  ------------------------------------------------------------------

void ToggleRealMsgno() {

  CFG->switches.set(disprealmsgno, not CFG->switches.get(disprealmsgno));
  HeaderView->Use(AA, reader_msg);
  HeaderView->Paint();
  reader_keyok = YES;
}


//  ------------------------------------------------------------------

void ToggleTwits() {

  gkey k;

  do {
    switch(AA->Twitmode()) {
      case TWIT_SHOW:
        AA->SetTwitmode(TWIT_BLANK);
        w_info(LNG->TwitBlanked);
        break;
      case TWIT_BLANK:
        AA->SetTwitmode(TWIT_SKIP);
        w_info(LNG->TwitSkipped);
        break;
      case TWIT_SKIP:
        AA->SetTwitmode(TWIT_IGNORE);
        w_info(LNG->TwitIgnoreSkip);
        break;
      case TWIT_IGNORE:
        AA->SetTwitmode(TWIT_KILL);
        w_info(LNG->TwitKilled);
        break;
      case TWIT_KILL:
        AA->SetTwitmode(TWIT_SHOW);
        w_info(LNG->TwitDisplayed);
        break;
    }

    k = waitkeyt(3000);

  } while(k and k != Key_Ent and k != Key_Esc);

  w_info(NULL);

  if(k == Key_Esc)
    AA->SetTwitmode(CFG->twitmode);
  else
    CFG->twitmode = AA->Twitmode();
}


//  ------------------------------------------------------------------

void ToggleStyles() {

  gkey k;
  int setting = AA->adat->usestylies + AA->adat->hidestylies*2;

  do {
    switch(setting) {
      case 2:
      case 0: w_info(LNG->StylecodesYes);  setting = 1; break;
      case 1: w_info(LNG->StylecodesHide); setting = 3; break;
      case 3: w_info(LNG->StylecodesNo);   setting = 0; break;
    }
    k = waitkeyt(3000);
  } while (k and k != Key_Ent and k != Key_Esc);
  w_info(NULL);

  if(k != Key_Esc) {
    AA->adat->usestylies = make_bool(setting & 1);
    AA->adat->hidestylies = make_bool(setting & 2);
  }
}


//  ------------------------------------------------------------------

void GotoMsgno() {

  if(AA->Msgn.Count()) {

    // Set the active window
    int lastwh = whandle();
    int inhdr = AA->Msglistheader() or not _in_msglist;

    if(reader_keycode < KK_Macro)
      if(gkbd.kbuf == NULL)
        kbput(reader_keycode); // Put back the key

    char buf[150];
    char* ptr = buf;
    int list_max = reader_msg->link.list_max();
    uint32_t* replies = (uint32_t*)throw_calloc(list_max+1, sizeof(uint32_t));
    uint32_t replyto, replynext;
    if(CFG->switches.get(disprealmsgno)) {
      ptr += sprintf(ptr, " %-5.5s:       %s %u", LNG->MsgReal, LNG->of, AA->Msgn.Count());
      replyto = reader_msg->link.to();
      replies[0] = reader_msg->link.first();
      replynext = reader_msg->link.next();
      for(int replyn=1; replyn<list_max+1; replyn++)
        replies[replyn] = reader_msg->link.list(replyn-1);
    }
    else {
      ptr += sprintf(ptr, " %-5.5s:       %s %u", LNG->Msg, LNG->of, AA->Msgn.Count());
      replyto = AA->Msgn.ToReln(reader_msg->link.to());
      replies[0] = AA->Msgn.ToReln(reader_msg->link.first());
      replynext = AA->Msgn.ToReln(reader_msg->link.next());
      for(int replyn=1; replyn<list_max+1; replyn++)
        replies[replyn] = AA->Msgn.ToReln(reader_msg->link.list(replyn-1));
    }
    if(replyto)
      ptr += sprintf(ptr, " -%u", replyto);
    for(int replyn=0,plus=0; replyn<list_max+1; replyn++)
      if(replies[replyn])
        ptr += sprintf(ptr, " %s%u", (plus++?"":"+"), replies[replyn]);
    if(replynext)
      sprintf(ptr, " *%u", replynext);

    if(inhdr) {
      HeaderView->window.activate_quick();
    }
    else {
      wopen_(0, 0, 3, strlen(buf)+3, W_BHEAD, C_HEADB, C_HEADW);
      w_shadow();
      wtitle(LNG->EnterMsgno, TCENTER, C_HEADT);
    }

    int wrow = inhdr ? 1 : 0;
    wprints(wrow, 0, C_HEADW, buf);
    *buf = NUL;

    gwindow iwindow(whandle());

    std::string fbuf = buf;
    gwinput2 iform(iwindow);
    iform.setup(C_HEADW, C_HEADW, C_HEADI, _box_table(W_BHEAD, 13), true);
    iform.add_field(0, wrow, 8, 5, fbuf, 20, gwinput::cvt_none, gwinput::entry_new);
    vcurshow();
    iform.run(H_InputMsgno);
    vcurhide();
    if(iform.dropped)
      fbuf = "";

    long active = AA->Msgn.Count();
    long msgno = atol(fbuf.c_str());

    if(CFG->switches.get(disprealmsgno))
      if(msgno > 0)
        msgno = AA->Msgn.ToReln(msgno);

    if((msgno < 0) and ((active+msgno) > 0))
      msgno = active + msgno;

    if(RngV(msgno, 1L, active))
      AA->set_lastread((uint)msgno);

    if(inhdr)
      wactiv_(lastwh);
    else
      wclose();

    throw_free(replies);
  }
}


//  ------------------------------------------------------------------

void PrevArea() {

  AA->attr().hex0();
  AA->Close();
  int currarea = AL.AreaIdToNo(CurrArea);
  int testarea = currarea;
  while(testarea > 0) {
    testarea--;
    if(not AL[testarea]->isseparator()) {
      currarea = testarea;
      break;
    }
  }
  if(currarea == testarea) {
    AL.SetActiveAreaNo(currarea);
    OrigArea = CurrArea;
  }
  else
    HandleGEvent(EVTT_ENDOFMSGS);
  AA->Open();
  AA->RandomizeData();
  AA->SetBookmark(AA->lastread());
  if (CFG->disppmfirst && AA->PMrk.Tags())
  {
    AA->isreadpm = false;
    ToggleMarkRead();
  }
}


//  ------------------------------------------------------------------

void NextArea() {

  AA->attr().hex0();
  AA->Close();
  uint currarea = AL.AreaIdToNo(CurrArea);
  uint testarea = currarea;
  while(testarea < (AL.size()-1)) {
    testarea++;
    if(not AL[testarea]->isseparator()) {
      currarea = testarea;
      break;
    }
  }
  if(currarea == testarea) {
    AL.SetActiveAreaNo(currarea);
    OrigArea = CurrArea;
  }
  else
    HandleGEvent(EVTT_ENDOFMSGS);
  AA->Open();
  AA->RandomizeData();
  AA->SetBookmark(AA->lastread());
  if (CFG->disppmfirst && AA->PMrk.Tags())
  {
    AA->isreadpm = false;
    ToggleMarkRead();
  }
}


//  ------------------------------------------------------------------

void QuitNow() {

  AA->attr().hex0();
  reader_finished = YES;
  reader_done = YES;
}


//  ------------------------------------------------------------------

void ExitAsk() {

  AA->attr().hex0();
  GMenuQuit MenuQuit;
  reader_done = reader_finished = MenuQuit.Run();
}


//  ------------------------------------------------------------------

void NewArea(bool jumpnext) {

  AA->attr().hex0();
  if(AA->isopen())
    AA->Close();

  int newarea = CurrArea;

  if(jumpnext) {
    uint idx = AL.AreaIdToNo(CurrArea);
    uint oidx = idx;

    do {
      idx++;

      // Wrap around
      if(idx >= AL.size())
        idx = 0;

      // Exit loop if no area with new mail found
      if(idx == oidx) {
        newarea = AreaPick(LNG->NewArea, 0, &newarea);
        if(newarea != -1)
          idx = AL.AreaIdToNo(newarea);
        break;
      }

    } while(AL.AreaNoToPtr(idx)->unread == 0);

    CurrArea = AL.AreaNoToId(idx);
  }
  else {
    newarea = AreaPick(LNG->NewArea, 0, &newarea);
    if(newarea != -1)
      CurrArea = newarea;
  }

  AL.Sort();
  reader_done = YES;

  if(gkbd.quitall)
    QuitNow();
  else {
    AL.SetActiveAreaId(CurrArea);
    OrigArea = CurrArea;
    strcpy(stecho, AA->echoid());
    AA->RandomizeData();

    if (AA->Msglistfirst() && (kbput(KK_ReadMessageList) != -1))
      reader_msglistfirst = true;
    else
      reader_msglistfirst = false;
  }
}


//  ------------------------------------------------------------------

void DosShell() {

  ShellToDos(getenv(GOLD_SHELL_ENV), LNG->DOS_Shell, LGREY_|_BLACK, YES);
}


//  ------------------------------------------------------------------

void TouchNetscan(int popup) {

  FILE* fp = fsopen(AddPath(CFG->areapath, CFG->semaphore.netscan), "wb", CFG->sharemode);
  if(fp) {
    fclose(fp);
    if(popup) {
      w_infof(" %s ", CFG->semaphore.netscan);
      HandleGEvent(EVTT_JOBDONE);
      waitkeyt(10000);
      w_info(NULL);
    }
  }
}


//  ------------------------------------------------------------------

int ExternUtil(GMsg *msg, ExtUtil *extutil) {

  Path editorfile, tmpfile, buf;
  strxcpy(editorfile, AddPath(CFG->goldpath, EDIT->File()), sizeof(Path));

  std::string cmdline = extutil->cmdline;

  int mode = (extutil->options & EXTUTIL_KEEPCTRL) ? MODE_SAVE : MODE_SAVENOCTRL;
  SaveLines(mode, editorfile, msg, 79);
  strcpy(buf, editorfile);
  strchg(buf, GOLD_WRONG_SLASH_CHR, GOLD_SLASH_CHR);
  strischg(cmdline, "@file", buf);
  if(striinc("@tmpfile", cmdline.c_str()))
  {
    mktemp(strcpy(tmpfile, AddPath(CFG->temppath, "GDXXXXXX")));
    SaveLines(mode, tmpfile, msg, 79);
    strcpy(buf, tmpfile);
    strchg(buf, GOLD_WRONG_SLASH_CHR, GOLD_SLASH_CHR);
    strischg(cmdline, "@tmpfile", buf);
  }
  else
    tmpfile[0] = NUL;
  strcpy(buf, CFG->goldpath);
  strchg(buf, GOLD_WRONG_SLASH_CHR, GOLD_SLASH_CHR);
  strischg(cmdline, "@path", buf);
  TokenXlat(MODE_NEW, cmdline, msg, msg, CurrArea);

  int pauseval = 0;
  if(extutil->options & EXTUTIL_PAUSEONERROR)
    pauseval = -1;
  if(extutil->options & EXTUTIL_PAUSE)
    pauseval = 1;

  ShellToDos(cmdline.c_str(), "",
    extutil->options & EXTUTIL_CLS ? LGREY_|_BLACK : BLACK_|_BLACK,
    extutil->options & EXTUTIL_CURSOR,
    pauseval
  );

  if(extutil->options & EXTUTIL_RELOAD) {

    if(not (extutil->options & EXTUTIL_KEEPCTRL)) {
      if (*msg->tearline or *msg->origin)
      {
        gfile fp(editorfile, "at");
        if (fp.isopen())
        {
          if (*msg->tearline)
            fp.Printf("--- %s\n", msg->tearline);
          if (*msg->origin)
            fp.Printf(" * Origin: %s\n", msg->origin);
          fp.Fclose();
        }
      }
    }

    bool hardlines = EDIT->HardLines();
    EDIT->HardLines(false);
    LoadText(msg, editorfile);
    EDIT->HardLines(hardlines);

    if(not strstr(msg->txt, "\001CHRS:") and not strstr(msg->txt, "\001CHARSET:")) {
      // Prepend "@CHRS: XLATLOCALSET 2\n" to force proper encoding
      const char *charset = AA->Viewkludge() ? msg->charset : CFG->xlatlocalset;
      char *msg_txt_chrs = (char *)throw_malloc(strlen(msg->txt)+strlen(charset)+sizeof("\001CHRS:  2\r")+16);
      sprintf(msg_txt_chrs, "\001CHRS: %s 2\r%s", charset, msg->txt);
      throw_free(msg->txt);
      msg->txt = msg_txt_chrs;
    }

    // Ignore any kludge address found
    msg->TextToLines(CFG->dispmargin-(int)CFG->switches.get(disppagebar), false);

    if(AA->Viewkludge()) {
      // Try to convert to the original codepage
      char *inpcharset = msg->charset;
      int level = LoadCharset(CFG->xlatlocalset, IsQuotedPrintable(inpcharset) ? ExtractPlainCharset(inpcharset) : inpcharset);
      if(level) {
        // do recode
        char *msg_txt_chrs = (char *)throw_malloc(strlen(msg->txt)*3+16);
        XlatStr(msg_txt_chrs, msg->txt, level, CharTable);
        throw_free(msg->txt);
        msg->txt = (char *)throw_realloc(msg_txt_chrs, strlen(msg_txt_chrs)+16);
      }
    }
    msg->charsetlevel = LoadCharset(msg->charset, CFG->xlatlocalset);
  }

  if(extutil->options & EXTUTIL_WIPE)
    WipeFile(editorfile, 0);
  if(tmpfile[0] != NUL)
    WipeFile(tmpfile, 0);

  return YES;
}


//  ------------------------------------------------------------------

int ExternUtil(GMsg *msg, int utilno) {

  std::vector<ExtUtil>::iterator extutil = CFG->externutil.begin();

  for(int utlno=0; extutil != CFG->externutil.end(); utlno++, extutil++) {

    if(extutil->utilno == utilno) {
      return ExternUtil(msg, &(*extutil));
    }
  }

  SayBibi();
  return YES;
}


//  ------------------------------------------------------------------

void ExternUtilMenu(GMsg* msg) {
  int n;
  std::vector<ExtUtil>::iterator i;
  static int startat = 0;
  char** Listi;
  char buf[100];
  static char cmdline[1024] = "";

  Listi = (char**)throw_calloc(CFG->externutil.size()+2, sizeof(char*));
  Listi[0] = throw_strdup(LNG->EnterCmdLine);
  for(i = CFG->externutil.begin(), n=1; i != CFG->externutil.end(); n++, i++) {
    sprintf(buf, " %02d %.59s%s ", i->utilno, i->cmdline, strlen(i->cmdline) > 59 ? ">" : "" );
    Listi[n] = throw_strdup(buf);
  }
  n = MinV(n, (MAXROW-10));
  set_title(LNG->ExternUtil, TCENTER, C_ASKT);
  update_statusline(LNG->SelectExternUtil);
  whelppcat(H_ReadExternUtil);
  n = wpickstr(6, 0, 6+n+1, -1, W_BASK, C_ASKB, C_ASKW, C_ASKS, Listi, startat, title_shadow);
  whelpop();
  if(n != -1) {
    if(n == 0) {
      if(edit_string(cmdline, sizeof(cmdline), LNG->ExecCmdLine, H_ReadExternUtil)) {
        ExtUtil extutil;
        extutil.utilno = 0;
        strxcpy(extutil.cmdline, cmdline, sizeof(extutil.cmdline));
        extutil.options = CFG->externoptions;
        reader_keyok = ExternUtil(msg, &extutil);
      }
    }
    else {
      reader_keyok = ExternUtil(msg, &CFG->externutil[n-1]);
    }
  }
  for(n=CFG->externutil.size()+1; n; n--)
    throw_free(Listi[n-1]);
  throw_free(Listi);
}


//  ------------------------------------------------------------------

static int PeekURLCmp(const char** a, const char** b) {

  return stricmp(*a, *b);
}


//  ------------------------------------------------------------------

void ReadPeekURLs(GMsg* msg) {

  Line **lin = msg->line;
  const char *ptr, *end, *begin;
  char buf[256];
  std::vector<char *> urls;
  std::vector<char *>::iterator i;
  int n;

  w_info(LNG->Wait);

  // Scan the current msg for urls
  int tline = reader_topline;
  if(CFG->peekurloptions & PEEK_FROMTOP)
    tline = 0;
  for(n=tline; n<msg->lines; n++) {

    ptr = lin[n]->txt.c_str();

    while(*ptr) {
      if(((begin = url_begin(ptr)) != NULL) and not strneql(ptr, "mailto:", 7)) {
        end = begin+strcspn(begin, " \t\"\'<>()[]");
        if(ispunct(end[-1]) and (end[-1] != '/'))
          --end;
        if(begin < end) {
          buf[0] = ' ';
          strxcpy(buf+1, ptr, MinV((long)((end-ptr)+1), (long)(MAXCOL-2-2-2)));
          strcat(buf, " ");
          for(i = urls.begin(); i != urls.end(); i++)
            if(strieql(*i, buf))
              break;
          if(i == urls.end())
            urls.push_back(throw_strdup(buf));
          ptr = end-1;
        }
      }
      ptr++;
    }
  }

  w_info(NULL);

  if(not urls.empty()) {

    char** Listi = (char**)throw_calloc(urls.size()+1, sizeof(char*));

    for(n = 0, i = urls.begin(); i != urls.end(); i++, n++)
      Listi[n] = *i;

    // Sort list if requested
    if(CFG->peekurloptions & FREQ_SORT)
      qsort(Listi, urls.size(), sizeof(char*), (StdCmpCP)PeekURLCmp);

    // Run the picker
    n = MinV((int)urls.size(), (MAXROW-10));
    set_title(LNG->PeekURLMenuTitle, TCENTER, C_ASKT);
    update_statusline(LNG->PeekURLStat);
    whelppcat(H_PeekURL);
    n = wpickstr(6, 0, 6+n+1, -1, W_BASK, C_ASKB, C_ASKW, C_ASKS, Listi, 0, title_shadow);
    whelpop();

    if(n != -1) {
      std::string cmdline = CFG->urlhandler.cmdline;
      strxmerge(buf, sizeof(buf), "\"", strtrim(strltrim(Listi[n])), "\"", NULL);
      strischg(cmdline, "@url", buf);
      strxcpy(buf, CFG->goldpath, sizeof(buf));
      strchg(buf, GOLD_WRONG_SLASH_CHR, GOLD_SLASH_CHR);
      strischg(cmdline, "@path", buf);
      TokenXlat(MODE_NEW, cmdline, msg, msg, CurrArea);

      int pauseval = 0;
      if(CFG->urlhandler.options & EXTUTIL_PAUSEONERROR)
        pauseval = -1;
      if(CFG->urlhandler.options & EXTUTIL_PAUSE)
        pauseval = 1;

      ShellToDos(cmdline.c_str(), "",
        CFG->urlhandler.options & EXTUTIL_CLS ? LGREY_|_BLACK : BLACK_|_BLACK,
        CFG->urlhandler.options & EXTUTIL_CURSOR,
        pauseval
      );
    }

    for(n = 0, i = urls.begin(); i != urls.end(); i++, n++)
      throw_free(Listi[n]);
    throw_free(Listi);
  }
  else {
    w_info(LNG->PeekInfoNoURLs);
    waitkeyt(10000);
    w_info(NULL);
  }
}


//  ------------------------------------------------------------------

void UUDecode(GMsg* msg) {

  Path infile, outfile;
  bool ok = false;

  if((*CFG->uudecodepath == NUL) or is_dir(CFG->uudecodepath)) {

    mktemp(strcpy(infile, AddPath(CFG->temppath, "GDXXXXXX")));
    strcpy(outfile, CFG->uudecodepath);

    GMenuDomarks MenuDomarks;

    int source = AA->Mark.Count() ? MenuDomarks.Run(LNG->Decode) : WRITE_CURRENT;
    bool overwrite = true;

    if(source == WRITE_QUIT)
      return;

    bool old_quotespacing = CFG->switches.get(quotespacing);
    CFG->switches.set(quotespacing, false);

    if(source == WRITE_MARKED) {
      for(uint n=0; n<AA->Mark.Count(); n++) {
        if(overwrite and n)
          overwrite = false;  // Overwrite only the first time
        w_progress(MODE_UPDATE, C_INFOW, n+1, AA->Mark.Count(), LNG->Preparing);
        AA->LoadMsg(msg, AA->Mark[n], 79);
        SaveLines(overwrite ? MODE_WRITE : MODE_APPEND, infile, msg, 79);
      }
      if(AA->Mark.Count())
        w_progress(MODE_QUIT, BLACK_|_BLACK, 0, 0, NULL);
    }
    else if(source == WRITE_CURRENT) {
      AA->LoadMsg(msg, msg->msgno, 79);
      SaveLines(MODE_WRITE, infile, msg, 79);
    }

    CFG->switches.set(quotespacing, old_quotespacing);

    uulist* item;
    int i, res;

    w_info(LNG->WaitUUdecoding);

    UUInitialize();
    UUSetOption(UUOPT_SAVEPATH, 0, outfile);
    if((res = UULoadFile(infile, NULL, 0)) == UURET_OK) {

      for (i=0; (item=UUGetFileListItem(i)) != NULL; i++) {
        if ((item->state & UUFILE_OK) == 0)
          continue;
        if ((res = UUDecodeFile(item, NULL)) == UURET_OK) {
          ok = true;
          w_infof(LNG->CompletedUUdecode, item->filename);
        }
      }
    }
    UUCleanUp ();

    remove(infile);

    if(not ok)
      w_info(LNG->NotUUencoded);
  }
  else
    w_info(LNG->UUEinvalidpath);

  if(not ok)
    waitkeyt(10000);

  w_info(NULL);
}


//  ------------------------------------------------------------------

void Make_Userlist(const char* userlist) {

  FILE* fp;
  word* crclist;
  word crc, crcs=0;
  uint n, x;
  char userline[80], adrs[40];

  GMsg* msg = (GMsg*)throw_calloc(1, sizeof(GMsg));

  crclist = (word*)throw_calloc(AA->Msgn.Count()+1, sizeof(word));
  fp = fsopen(userlist, "ab", CFG->sharemode);
  if(fp) {
    w_progress(MODE_NEW, C_INFOW, 0, AA->Msgn.Count(), LNG->GenUserlist);
    for(n=AA->Msgn.Count(); n; n--) {
      update_statuslinef(LNG->ReadingMsg, "ST_READINGMSG", n, AA->Msgn.Count());
      w_progress(MODE_UPDATE, C_INFOW, (AA->Msgn.Count()-n)+1, AA->Msgn.Count(), NULL);
      AA->LoadMsg(msg, AA->Msgn.CvtReln(n), CFG->dispmargin);
      crc = strCrc16(msg->by, false);
      for(x=0; x<crcs; x++)
        if(crclist[x] == crc)
          break;  // We have already used it
      if(x >= crcs) {
        crclist[crcs++] = crc;
        strrevname(userline, msg->by);
        msg->orig.make_string(adrs);
        fprintf(fp, "%-36.36s%24.24s\r\n", userline, adrs);
      }
    }
    throw_free(crclist);
    w_progress(MODE_QUIT, BLACK_|_BLACK, 0, 0, NULL);
    fclose(fp);
  }

  ResetMsg(msg);
  throw_free(msg);
}


//  ------------------------------------------------------------------

void MakeUserList() {

  if(AA->Msgn.Count()) {
    uint temp = AA->lastread();
    if(not edit_pathname(CFG->userlistfile, sizeof(Path), LNG->UserlistName, 0))
      return;
    Make_Userlist(AddPath(CFG->goldpath, CFG->userlistfile));
    AA->set_lastread(temp);
  }
}


//  ------------------------------------------------------------------

void TouchSemaphore() {

  Path file;
  *file = NUL;
  if(edit_string(file, sizeof(Path), LNG->TouchSemaphore, 0))
    TouchFile(file);
}


//  ------------------------------------------------------------------

void make_pathreport(const char* reportfile)
{
  gfile fp(reportfile, "wt");
  if (fp.isopen())
  {
    std::string path;
    ftn_addr address;
    std::vector<ftn_addr> alist;
    GMsg* msg = (GMsg*)throw_calloc(1, sizeof(GMsg));
    w_progress(MODE_NEW, C_INFOW, 0, AA->Msgn.Count(), "Generating PATH report");
    for(int n=AA->Msgn.Count(); n>=AA->lastread(); n--) {
      update_statuslinef(LNG->ReadingMsg, "ST_READINGMSG", n, AA->Msgn.Count());
      w_progress(MODE_UPDATE, C_INFOW, n, AA->Msgn.Count(), NULL);
      AA->LoadMsg(msg, AA->Msgn.CvtReln(n), CFG->dispmargin-(int)CFG->switches.get(disppagebar));
      address = msg->orig;
      int x;
      for(x=0; x<alist.size(); x++) {
        if(alist[x] == address)
          break;  // We have already used it
      }
      if(x >= alist.size()) {
        alist.push_back(address);
        INam buf;
        strcpy(buf, msg->By());
        strchg(buf, ' ', '_');
        std::string temp;
        fp.Printf("%s  %s  ", buf, address.make_string(temp).c_str());
        path = "";
        Line* line = msg->lin;
        while(line) {
          if(line->kludge == GKLUD_PATH)
            path += line->txt.c_str() + 6;
          line = line->next;
        }
        if(path.length()) {
          gstrarray links;
          tokenize(links, path.c_str(), " ");
          for(int i=0; i<links.size(); i++)
            address = links[i];
        }
        fp.Printf("%s\n", path.c_str());
      }
    }
    w_progress(MODE_QUIT, BLACK_|_BLACK, 0, 0, NULL);
    fp.Fclose();
    ResetMsg(msg);
    throw_free(msg);
  }
}


//  ------------------------------------------------------------------

void MakePathreport() {

  if(AA->Msgn.Count()) {
    uint temp = AA->lastread();
    if(not edit_pathname(CFG->pathreportfile, sizeof(Path), LNG->Pathreport, 0))
      return;
    make_pathreport(AddPath(CFG->goldpath, CFG->pathreportfile));
    AA->set_lastread(temp);
  }
}


//  ------------------------------------------------------------------

