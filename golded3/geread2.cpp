
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
      DoKludges(MODE_CHANGE, reader_msg, true);
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
  int setting = CFG->usestylies + CFG->hidestylies*2;

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
    CFG->usestylies = (setting & 1) ? true : false;
    CFG->hidestylies = (setting & 2) ? true : false;
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
    ulong* replies = (ulong*)throw_calloc(list_max+1, sizeof(ulong));
    ulong replyto, replynext;
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
      ptr += sprintf(ptr, " -%lu", replyto);
    for(int replyn=0,plus=0; replyn<list_max+1; replyn++)
      if(replies[replyn])
        ptr += sprintf(ptr, " %s%lu", (plus++?"":"+"), replies[replyn]);
    if(replynext)
      sprintf(ptr, " *%lu", replynext);

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
  if(AA->PMrk.Tags()) {
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
  if(AA->PMrk.Tags()) {
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
    if(AA->Msglistfirst())
      kbput(KK_ReadMessageList);
  }
}


//  ------------------------------------------------------------------

void DosShell() {

  ShellToDos(getenv(GOLD_SHELL_ENV), LNG->DOS_Shell, LGREY|_BLACK, YES);
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

int ExternUtil(GMsg* msg, int utilno) {

  Path editorfile, tmpfile;
  strcpy(editorfile, AddPath(CFG->goldpath, EDIT->File()));
  mktemp(strcpy(tmpfile, AddPath(CFG->goldpath, "GDXXXXXX")));

  std::vector<ExtUtil>::iterator extutil = CFG->externutil.begin();

  for(int utlno=0; extutil != CFG->externutil.end(); utlno++, extutil++) {

    if(extutil->utilno == utilno) {

      char cmdline[256], buf[256];
      strcpy(cmdline, extutil->cmdline);

      int mode = (extutil->options & EXTUTIL_KEEPCTRL) ? MODE_SAVE : MODE_SAVENOCTRL;
      SaveLines(mode, editorfile, msg, 79);
      if(striinc("@tmpfile", cmdline))
        SaveLines(mode, tmpfile, msg, 79);

      strcpy(buf, CFG->goldpath);
      strchg(buf, GOLD_WRONG_SLASH_CHR, GOLD_SLASH_CHR);
      strischg(cmdline, "@path", buf);
      strcpy(buf, editorfile);
      strchg(buf, GOLD_WRONG_SLASH_CHR, GOLD_SLASH_CHR);
      strischg(cmdline, "@file", buf);
      strcpy(buf, tmpfile);
      strchg(buf, GOLD_WRONG_SLASH_CHR, GOLD_SLASH_CHR);
      strischg(cmdline, "@tmpfile", buf);
      TokenXlat(MODE_NEW, cmdline, msg, msg, CurrArea);

      int pauseval = 0;
      if(extutil->options & EXTUTIL_PAUSEONERROR)
        pauseval = -1;
      if(extutil->options & EXTUTIL_PAUSE)
        pauseval = 1;

      ShellToDos(cmdline, "",
        extutil->options & EXTUTIL_CLS ? LGREY|_BLACK : 0,
        extutil->options & EXTUTIL_CURSOR,
        pauseval
      );

      if(extutil->options & EXTUTIL_RELOAD) {

        if(not (extutil->options & EXTUTIL_KEEPCTRL)) {
          if(*msg->tearline or *msg->origin) {
            gfile fp;
            fp.fopen(editorfile, "at");
            if(fp.isopen()) {
              if(*msg->tearline)
                fp.printf("--- %s\n", msg->tearline);
              if(*msg->origin)
                fp.printf(" * Origin: %s\n", msg->origin);
              fp.fclose();
            }
          }
        }

        int hardlines = EDIT->HardLines();
        EDIT->HardLines(false);
        LoadText(msg, editorfile);
        if(extutil->options & EXTUTIL_WIPE)
          WipeFile(editorfile, 0);
        WipeFile(tmpfile, 0);

        EDIT->HardLines(hardlines);
        msg->attr.tou1();   // Ignore any kludge address found
        msg->TextToLines(CFG->dispmargin-(int)CFG->switches.get(disppagebar));
        msg->attr.tou0();
      }

      return YES;
    }
  }

  SayBibi();
  return YES;
}


//  ------------------------------------------------------------------

void ExternUtilMenu() {

  // Not implemented yet
}


//  ------------------------------------------------------------------

void UUDecode(GMsg* msg) {

  Path infile, outfile;
  bool ok = false;

  if((*CFG->uudecodepath == NUL) or is_dir(CFG->uudecodepath)) {

    mktemp(strcpy(infile, AddPath(CFG->goldpath, "GDXXXXXX")));
    strcpy(outfile, CFG->uudecodepath);

    GMenuDomarks MenuDomarks;

    int source = AA->Mark.Count() ? MenuDomarks.Run(LNG->Decode) : WRITE_CURRENT;
    bool overwrite = true;

    if(source == WRITE_QUIT)
      return;

    if(source == WRITE_MARKED) {
      for(uint n=0; n<AA->Mark.Count(); n++) {
        if(overwrite and n)
          overwrite = false;  // Overwrite only the first time
        w_progress(MODE_UPDATE, C_INFOW, n+1, AA->Mark.Count(), LNG->Preparing);
        AA->LoadMsg(msg, AA->Mark[n], 79);
        SaveLines(overwrite ? MODE_WRITE : MODE_APPEND, infile, msg, 79);
      }
      if(AA->Mark.Count())
        w_progress(MODE_QUIT, 0, 0, 0, NULL);
    }
    else if(source == WRITE_CURRENT)
      SaveLines(MODE_WRITE, infile, msg, 79);

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
      update_statuslinef(LNG->ReadingMsg, n, AA->Msgn.Count());
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
    w_progress(MODE_QUIT, 0, 0, 0, NULL);
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

void make_pathreport(const char* reportfile) {

  gfile fp;
  fp.fopen(reportfile, "wt");
  if(fp) {
    std::string path;
    ftn_addr address;
    std::vector<ftn_addr> alist;
    GMsg* msg = (GMsg*)throw_calloc(1, sizeof(GMsg));
    w_progress(MODE_NEW, C_INFOW, 0, AA->Msgn.Count(), "Generating PATH report");
    for(int n=AA->Msgn.Count(); n>=AA->lastread(); n--) {
      update_statuslinef(LNG->ReadingMsg, n, AA->Msgn.Count());
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
        fp.printf("%s  %s  ", buf, address.make_string(temp).c_str());
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
        fp.printf("%s\n", path.c_str());
      }
    }
    w_progress(MODE_QUIT, 0, 0, 0, NULL);
    fp.fclose();
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

