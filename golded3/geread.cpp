
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
//  Main reader and function key dispatcher.
//  ------------------------------------------------------------------

#include <golded.h>


//  ------------------------------------------------------------------
//  Global data

extern bool cmdlinenoscan;
extern bool cmdlineexportsoup;

GMsg* reader_msg;
bool reader_gen_confirm = false;
int reader_finished;
int reader_done = false;
int reader_topline;
int reader_keyok;
int reader_direction;
int reader_rcv_noise = 0;
gkey reader_keycode;
gkey reader_lastcode = 0;
bool reader_msglistfirst = false;

GMsgHeaderView *HeaderView;
GMsgBodyView   *BodyView;

void TwitDeleteMsg(GMsg* msg);
void GoNextMsg();


//  ------------------------------------------------------------------

gkey ReaderGetKey() {

  gkey keycode;
  do {
    keycode = getxchtick();
    if(keycode == Key_Tick)
      CheckTick(KK_ReadQuitNow);
  } while(keycode == Key_Tick);
  if(keycode < KK_Commands) {
    keycode = key_tolower(keycode);
    gkey kk = SearchKey(keycode, ReadKey, ReadKeys);
    if(kk)
      keycode = kk;
  }
  return keycode;
}


//  ------------------------------------------------------------------

gkey ViewMessage(int istwit) {

  gkey keycode;
  int keyok;

  do {

    keycode = ReaderGetKey();
    keyok = true;

    switch(keycode) {
      case KK_ReadMsgLineUp:
        if(istwit <= TWIT_SHOW)
          BodyView->LineUp();
        break;

      case KK_ReadMsgLineDown:
        if(istwit <= TWIT_SHOW)
          BodyView->LineDown();
        break;

      case KK_ReadMsgPgUp:
        if(istwit <= TWIT_SHOW)
          BodyView->PageUp();
        break;

      case KK_ReadMsgPgDn:
        if(istwit <= TWIT_SHOW)
          BodyView->PageDown();
        break;

      case KK_ReadMsgContinue:
        if((istwit > TWIT_SHOW) or not BodyView->Continue())
          keyok = false;
        break;

      case KK_ReadMsgHome:
        BodyView->Top(istwit > TWIT_SHOW);
        istwit = NOT_TWIT;
        break;

      case KK_ReadMsgEnd:
        BodyView->Bottom();
        istwit = NOT_TWIT;
        break;

      default:
        keyok = false;
    }

    update_statuslines();

  } while(keyok);

  return keycode;
}


//  ------------------------------------------------------------------

void Reader() {

  int istwit;
  bool istwitto, istwitsubj;
  char buf[256], buf2[200];

  GMsg* msg = (GMsg*)throw_calloc(1, sizeof(GMsg));

  reader_finished = false;
  reader_msg = msg;

  // Sort all areas by the given sort order before scanning
  AL.Sort();

  int personalmails = 0;
  int pmareas = 0;
  int pmscan = CFG->personalmail & PM_STARTUP;
  if(not cmdlinenoscan) {
    if(cmdlinedebughg)
      LOG.printf("- AreaScan during startup");
    AL.AreaScan(SCAN_STARTUP, 0, pmscan, personalmails, pmareas);
    startupscan_success = true;
  }

  CheckSemaphores();

  if(cmdlineimportsoup)
    ImportSOUP();

  if(cmdlineexportsoup)
    ExportSOUP();

  AL.Sort();
  startecho = *stecho ? AL.AreaEchoToNo(stecho) : -1;

  inforow = (MAXROW-1)/2;
  wclear();

  // Reset tick values to avoid triggering screenblanker or timeout
  gkbdtickpressreset();
  gkbdtickvaluereset();

  if(startecho == -1) {
    int i = 0;
    do {
      AL.SetActiveAreaNo(i++);
    } while(AA->isseparator());
    NewArea();
  }
  else {
    AL.SetActiveAreaNo(startecho);
    AA->RandomizeData();
    if(AA->Msglistfirst()) {
      reader_msglistfirst = true;
      kbput(KK_ReadMessageList);
    }
    else
      reader_msglistfirst = false;
  }

  if(not reader_finished) {

    // Open the editor/reader window

    HeaderView->width           = MAXCOL;
    HeaderView->height          = 6;
    HeaderView->border_color    = C_HEADB;
    HeaderView->window_color    = C_HEADW;
    HeaderView->title_color     = C_HEADT;
    HeaderView->highlight_color = C_HEADQ;
    HeaderView->from_color      = C_HEADBY;
    HeaderView->to_color        = C_HEADTO;
    HeaderView->subject_color   = C_HEADRE;
    HeaderView->Create();

    BodyView->at_row          = 6;
    BodyView->width           = MAXCOL;
    BodyView->height          = MAXROW - 6 - 1;
    BodyView->border_color    = C_READB;
    BodyView->window_color    = C_READW;
    BodyView->scrollbar_color = C_READPB;
    BodyView->highlight_color = C_READH;
    BodyView->Create();

    do {

      AA->isreadpm = false;
      AA->isreadmark = false;
      reader_direction = DIR_NEXT;
      reader_msg->msgno = 0;
      CFG->tplno = 0;
      AL.SetActiveAreaId(CurrArea);
      OrigArea = CurrArea;

      AA->Open();

      AA->RandomizeData();
      if(AA->Play().type)
        AA->Play().Play();

      // Jump to next message. Go to personal mail if any.
      if(AA->PMrk.Tags()) {
        AA->SetBookmark(AA->lastread());
        AA->isreadpm = false;
        ToggleMarkRead();
      }
      else {
        if(CFG->switches.get(dispautonext) or (AA->lastread() == 0))
          next_msg(DIR_NEXT);
        AA->SetBookmark(AA->lastread());
      }

      // Main message reader loop
      do {

        // Set the default help for this window
        whelpcat(H_Reader);

        if(not AA->attr().hex())
          reader_topline = 0;

        if(AA->Msgn.Count() and not AA->lastread())
          AA->set_lastread(1);

        if(LoadMessage(msg, CFG->dispmargin-(int)CFG->switches.get(disppagebar))) {

          do {

            HeaderView->Use(AA, msg);

            HeaderView->Paint();

            if(reader_msglistfirst == false) {
              if(AA->isreadmark or AA->isreadpm) {
                GTag& tag = AA->isreadpm ? AA->PMrk : AA->Mark;
                uint mtemp = tag.Find(msg->msgno);
                sprintf(buf2, "%s [%s%s%s%s%s%s]",
                  LNG->ReadMarked,
                  AA->Viewhidden() ? "H" : "",
                  AA->Viewkludge() ? "K" : "",
                  AA->Twitmode() == TWIT_IGNORE ? "Ti" : AA->Twitmode() == TWIT_SKIP ? "Ts" : AA->Twitmode() == TWIT_BLANK ? "Tb" : AA->Twitmode() == TWIT_KILL ? "Tk" : "",
                  CFG->showdeleted ? "D" : "",
                  AA->Viewquote() ? "Q" : "",
                  AA->StripHTML() ? "S" : ""
                );
                sprintf(buf, buf2,
                  mtemp, tag.Count(), tag.Count()-mtemp
                );
              }
              else {
                sprintf(buf2, "%s [%s%s%s%s%s%s]",
                  LNG->ReadAll,
                  AA->Viewhidden() ? "H" : "",
                  AA->Viewkludge() ? "K" : "",
                  AA->Twitmode() == TWIT_IGNORE ? "Ti" : AA->Twitmode() == TWIT_SKIP ? "Ts" : AA->Twitmode() == TWIT_BLANK ? "Tb" : AA->Twitmode() == TWIT_KILL ? "Tk" : "",
                  CFG->showdeleted ? "D" : "",
                  AA->Viewquote() ? "Q" : "",
                  AA->StripHTML() ? "S" : ""
                );
                sprintf(buf, buf2,
                  AA->lastread(), AA->Msgn.Count(), AA->Msgn.Count()-AA->lastread()
                );
              }
              update_statusline(buf);
            }

            reader_done = false;
            reader_keyok = false;

            // Do some updating first
            AA->set_lastread(AA->Msgn.ToReln(msg->msgno));
            AA->UpdateAreadata();

            if(reader_msglistfirst == false) {

              // Determine if the message is of the "twit" type
              istwit = MsgIsTwit(msg, istwitto, istwitsubj);

              if(istwit)
                HandleGEvent(EVTT_MSGISTWIT);

              if((istwit == TWIT_SKIP) or (not CFG->showdeleted and msg->attr.del())) {
                if(reader_direction == DIR_NEXT) {
                  if(AA->lastread() < AA->Msgn.Count()) {
                    if(gkbd.kbuf == NULL) {
                      switch(reader_lastcode) {
                        case KK_ReadGotoReplies:
                        case KK_ReadGotoReply1st:
                        case KK_ReadGotoReplyNext:
                          if(MsgHasReplies(msg)) {
                            kbput(KK_ReadGotoReplies);
                          }
                          else {
                            SayBibi();
                            kbput(KK_ReadGotoReplyPrev);
                          }
                          break;
                        default:
                          kbput(KK_ReadGotoNextMsg);
                      }
                    }
                  }
                }
                else {
                  if(AA->lastread() > 1) {
                    if(gkbd.kbuf == NULL) {
                      if(reader_lastcode == KK_ReadGotoReplyPrev) {
                        if(AA->Msgn.ToReln(msg->link.to()))
                          kbput(KK_ReadGotoReplyPrev);
                        else {
                          SayBibi();
                          kbput(KK_ReadGotoReplies);
                        }
                      }
                      else
                        kbput(KK_ReadGotoPrevMsg);
                    }
                  }
                }
              }

              BodyView->Use(AA, msg, reader_topline);
              if(istwit > TWIT_SHOW) {
                BodyView->window.clear();
                if(istwit == TWIT_SKIP)
                  BodyView->window.prints(1, 0, C_READW, LNG->SkippingTwit);
                else if(istwit == TWIT_KILL)
                  BodyView->window.prints(1, 0, C_READW, LNG->KillingTwit);
                else {
                  BodyView->window.prints(1, 0, C_READW, LNG->HidingTwit);
                }
              }
              else {
                if(CFG->showdeleted or not msg->attr.del())
                  BodyView->Paint();
                else {
                  BodyView->window.clear();
                  BodyView->window.prints(1, 0, C_READW, LNG->SkippingDeleted);
                }
              }

              if(reader_rcv_noise) {
                if(CFG->beepyourmail == ALWAYS)
                  HandleGEvent(EVTT_MSGTOYOU);
                else if(reader_rcv_noise > 1 and CFG->beepyourmail == true)
                  HandleGEvent(EVTT_MSGTOYOU);
                reader_rcv_noise = false;
              }
              else {
                if(msg->attr.fmu())
                  HandleGEvent(EVTT_MSGFROMYOU);
              }

              if(msg->attr.loc() and CFG->switches.get(beeplocalmsg))
                HandleGEvent(EVTT_MSGISLOCAL);

              if(AA->Msgn.Count() and CFG->switches.get(highlightunread) and (msg->orig_timesread == 0))
                AA->UpdateTimesread(msg);

              switch(istwit) {
                case TWIT_KILL:
                  if(not msg->attr.del()) {
                    // prevent infinite loop on the last message
                    TwitDeleteMsg(msg);
                    continue;
                  }
                case TWIT_SKIP:
                  reader_keycode = ReaderGetKey();
                  break;
                default:
                  reader_keycode = ViewMessage(istwit);
              }
            }
            else
              reader_keycode = ReaderGetKey();

            reader_topline = BodyView->UpperLine();

            if(reader_gen_confirm)
              ConfirmMsg();

            reader_gen_confirm = false;
            reader_lastcode = reader_keycode;

            switch(reader_keycode) {

              case KK_ReadMainMenu:
                {
                  GMenuNavigate MenuNavigate;
                  int finaltag = MenuNavigate.Run();
                  if((finaltag != W_ESCPRESS) and (finaltag != -1))
                    kbput((gkey)finaltag);
                }
                break;

              case KK_ReadSoundkill:
                HandleGEvent(EVTT_STOPVOICE);
                reader_keyok = true;
                break;

              case KK_ReadChangeAttrs:
                ChangeAttributes();
                break;

              case KK_ReadChangeAka:
                ChangeAka();
                reader_keyok = true;
                break;

              case KK_ReadChangeTagline:
                ChangeTagline();
                reader_keyok = true;
                break;

              case KK_ReadChangeOrigin:
                ChangeOrigin();
                reader_keyok = true;
                break;

              case KK_ReadChangeUsername:
                ChangeUsername();
                reader_keyok = true;
                break;

              case KK_ReadChangeTemplate:
                ChangeTemplate();
                reader_keyok = true;
                break;

              case KK_ReadChangeXlatImport:
                ChangeXlatImport();
                break;

              case KK_ReadLookupDest:
                LookupNode(msg, "", LOOK_DEST);
                reader_keyok = true;
                break;

              case KK_ReadLookupOrig:
                LookupNode(msg, "", LOOK_ORIG);
                reader_keyok = true;
                break;

              case KK_ReadFidoRenumber:
                RenumberArea();
                break;

              case KK_ReadDecreaseMargin:
                DecMargin();
                break;

              case KK_ReadIncreaseMargin:
                IncMargin();
                break;

              case KK_ReadStylesNone:
                if(CFG->usestylies or CFG->hidestylies)
                  CFG->usestylies = CFG->hidestylies = false;
                else
                  reader_keyok = true;
                break;

              case KK_ReadStylesShow:
                if(not CFG->usestylies or CFG->hidestylies) {
                  CFG->usestylies = true;
                  CFG->hidestylies = false;
                }
                else
                  reader_keyok = true;
                break;

              case KK_ReadStylesStrip:
                if(not CFG->usestylies or not CFG->hidestylies)
                  CFG->usestylies = CFG->hidestylies = true;
                else
                  reader_keyok = true;
                break;

              case KK_ReadToggleStyles:
                ToggleStyles();
                break;

              case KK_ReadToggleHexdump:
                AA->attr().hexX();
                break;

              case KK_ReadStripHTML:
                if(not AA->StripHTML())
                  ToggleStripHTML();
                else
                  reader_keyok = true;
                break;

              case KK_ReadShowHTML:
                if(AA->StripHTML())
                  ToggleStripHTML();
                else
                  reader_keyok = true;
                break;

              case KK_ReadToggleHTML:
                ToggleStripHTML();
                break;

              case KK_ReadTwitsShow:
                if(AA->Twitmode() != TWIT_SHOW) {
                  AA->SetTwitmode(TWIT_SHOW);
                  CFG->twitmode = TWIT_SHOW;
                }
                else
                  reader_keyok = true;
                break;

              case KK_ReadTwitsBlank:
                if(AA->Twitmode() != TWIT_BLANK) {
                  AA->SetTwitmode(TWIT_BLANK);
                  CFG->twitmode = TWIT_BLANK;
                }
                else
                  reader_keyok = true;
                break;

              case KK_ReadTwitsSkip:
                if(AA->Twitmode() != TWIT_SKIP) {
                  AA->SetTwitmode(TWIT_SKIP);
                  CFG->twitmode = TWIT_SKIP;
                }
                else
                  reader_keyok = true;
                break;

              case KK_ReadTwitsIgnore:
                if(AA->Twitmode() != TWIT_IGNORE) {
                  AA->SetTwitmode(TWIT_IGNORE);
                  CFG->twitmode = TWIT_IGNORE;
                }
                else
                  reader_keyok = true;
                break;

              case KK_ReadTwitsKill:
                if(AA->Twitmode() != TWIT_KILL) {
                  AA->SetTwitmode(TWIT_KILL);
                  CFG->twitmode = TWIT_KILL;
                }
                else
                  reader_keyok = true;
                break;

              case KK_ReadToggleTwits:
                ToggleTwits();
                break;

              case KK_ReadReadMarked:
                if(not AA->isreadmark)
                  ToggleMarkRead();
                else
                  reader_keyok = true;
                break;

              case KK_ReadReadAll:
                if(AA->isreadmark)
                  ToggleMarkRead();
                else
                  reader_keyok = true;
                break;

              case KK_ReadToggleMarkRead:
                ToggleMarkRead();
                break;

              case KK_ReadMark:
                if(not AA->Mark.Find(reader_msg->msgno))
                  ToggleMark();
                else
                  reader_keyok = true;
                break;

              case KK_ReadUnmark:
                if(AA->Mark.Find(reader_msg->msgno))
                  ToggleMark();
                else
                  reader_keyok = true;
                break;

              case KK_ReadToggleMark:
                ToggleMark();
                break;

              case KK_ReadToggleBookMark:
                ToggleBookMark();
                break;

              case KK_ReadGotoBookMark:
                GotoBookMark();
                break;

              case KK_ReadGotoPrevMsg:
                GotoPrevMsg();
                break;

              case KK_ReadGotoNextMsg:
              case KK_ReadMsgContinue:
                GotoNextMsg();
                break;

              case KK_ReadGotoFirstMsg:
                GotoFirstMsg();
                break;

              case KK_ReadGotoLastMsg:
                GotoLastMsg();
                break;

              case KK_ReadGotoReplies:
                GotoReplies();
                break;

              case KK_ReadGotoReply1st:
                GotoReply1st();
                break;

              case KK_ReadGotoReplyNext:
                GotoReplyNext();
                break;

              case KK_ReadGotoReplyPrev:
                GotoReplyPrev();
                break;

              case KK_ReadGotoNextUnread:
                GotoNextUnread();
                break;

              case KK_ReadGotoPrevUnread:
                GotoPrevUnread();
                break;

              case KK_ReadShowHiddKlud:
                if(not AA->Viewhidden() and not AA->Viewkludge())
                  ToggleHiddKlud();
                else if(not AA->Viewhidden())
                  ToggleHidden();
                else if(not AA->Viewkludge())
                  ToggleKludge();
                else
                  reader_keyok = true;
                break;

              case KK_ReadHideHiddKlud:
                if(AA->Viewhidden() and AA->Viewkludge())
                  ToggleHiddKlud();
                else if(AA->Viewhidden())
                  ToggleHidden();
                else if(AA->Viewkludge())
                  ToggleKludge();
                else
                  reader_keyok = true;
                break;

              case KK_ReadToggleHiddKlud:
                ToggleHiddKlud();
                break;

              case KK_ReadShowKludge:
                if(not AA->Viewkludge())
                  ToggleKludge();
                else
                  reader_keyok = true;
                break;

              case KK_ReadHideKludge:
                if(AA->Viewkludge())
                  ToggleKludge();
                else
                  reader_keyok = true;
                break;

              case KK_ReadToggleKludge:
                ToggleKludge();
                break;

              case KK_ReadShowHidden:
                if(not AA->Viewhidden())
                  ToggleHidden();
                else
                  reader_keyok = true;
                break;

              case KK_ReadHideHidden:
                if(AA->Viewhidden())
                  ToggleHidden();
                else
                  reader_keyok = true;
                break;

              case KK_ReadToggleHidden:
                ToggleHidden();
                break;

              case KK_ReadShowQuote:
                if(not AA->Viewquote())
                  ToggleQuote();
                else
                  reader_keyok = true;
                break;

              case KK_ReadHideQuote:
                if(AA->Viewquote())
                  ToggleQuote();
                else
                  reader_keyok = true;
                break;

              case KK_ReadToggleQuote:
                ToggleQuote();
                break;

              case KK_ReadToggleROT13:
                ToggleROT13();
                break;

              case KK_ReadTogglePageBar:
                TogglePageBar();
                break;

              case KK_ReadRealMsgno:
                if(not CFG->switches.get(disprealmsgno))
                  ToggleRealMsgno();
                else
                  reader_keyok = true;
                break;

              case KK_ReadSequentMsgno:
                if(CFG->switches.get(disprealmsgno))
                  ToggleRealMsgno();
                else
                  reader_keyok = true;
                break;

              case KK_ReadToggleRealMsgno:
                ToggleRealMsgno();
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
                GotoMsgno();
                break;

              case KK_ReadMessageList:
                MessageBrowse();
                reader_msglistfirst = false;
                break;

              case KK_ReadThreadtree:
                MsgThreadlist();
                break;

              case KK_ReadDosShell:
                DosShell();
                reader_keyok = true;
                break;

              case KK_ReadFindAll:
                FindAll(msg, reader_topline, reader_keyok);
                break;

              case KK_ReadFindHeader:
                FindHdr(msg, reader_topline, reader_keyok);
                break;

              case KK_ReadSearch:
                AdvancedSearch(msg, reader_topline, reader_keyok);
                break;

              case KK_ReadFileRequest:
                FileRequest(msg);
                break;

              case KK_ReadNewMsg:
                NewMsg();
                break;

              case KK_ReadMoveQuoteMsg:
                OtherAreaQuoteMsg(false);
                break;

              case KK_ReadMoveDirQuoteMsg:
                OtherAreaQuoteMsg(true);
                break;

              case KK_ReadMoveCommentMsg:
                OtherAreaCommentMsg();
                break;

              case KK_ReadQuoteBuf:
                QuoteBuf(msg);
                break;

              case KK_ReadQuoteMsg:
                QuoteMsg(false);
                break;

              case KK_ReadDirQuoteMsg:
                QuoteMsg(true);
                break;

              case KK_ReadCommentMsg:
                CommentMsg();
                break;

              case KK_ReadReplyMsg:
                ReplyMsg();
                break;

              case KK_ReadChangeMsg:
                ChangeMsg();
                break;

              case KK_ReadCopyMoveForward:
                CopyMoveForward();
                break;

              case KK_ReadDeleteMsg:
                if(AA->attr().r_o()) {
                  GMenuReadonly MenuReadonly;
                  if(not MenuReadonly.Run())
                    break;
                }
                AA->DelMsg();
                break;

              case KK_ReadMakeUserlist:
                MakeUserList();
                reader_keyok = true;
                break;

              case KK_ReadMakePathreport:
                MakePathreport();
                reader_keyok = true;
                break;

              case KK_ReadWriteMsg:
                WriteMsg(msg);
                break;

              case KK_ReadMarkingOptions:
                MarkingOptions();
                break;

              case KK_ReadGotoPrevArea:
                PrevArea();
                break;

              case KK_ReadGotoNextArea:
                NextArea();
                break;

              case KK_ReadQuitNow:
                QuitNow();
                break;

              case KK_ReadAskExit:
                ExitAsk();
                break;

              case KK_ReadNewArea:
                NewArea();
                break;

              case KK_ReadTouchNetscan:
                TouchNetscan();
                reader_keyok = true;
                break;

              case KK_ReadTouchSemaphore:
                TouchSemaphore();
                reader_keyok = true;
                break;

              case KK_ReadExternUtilMenu:
                ExternUtilMenu(msg);
                break;

              case KK_ReadPeekURLs:
                ReadPeekURLs(msg);
                reader_keyok = true;
                break;

              case KK_ReadAddressbookAdd:
                {
                  GMenuDomarks MenuDomarks;

                  int source = AA->Mark.Count() ? MenuDomarks.Run(LNG->Addressbook) : WRITE_CURRENT;
                  if(source != WRITE_QUIT) {
                    if(source == WRITE_MARKED) {
                      for(uint n=0; n<AA->Mark.Count(); n++) {
                        AA->LoadMsg(msg, AA->Mark[n], CFG->dispmargin-(int)CFG->switches.get(disppagebar));
                        update_addressbook(msg, true, true);
                      }
                    }
                    else // Write current
                      update_addressbook(msg, true, true);
                  }
                }
                break;

              case KK_ReadAddressbook:
                edit_addressbook(msg);
                break;

              case KK_ReadUUDecode:
                UUDecode(msg);
                break;

              case KK_ExternUtil01:   reader_keyok = ExternUtil(msg,  1);   break;
              case KK_ExternUtil02:   reader_keyok = ExternUtil(msg,  2);   break;
              case KK_ExternUtil03:   reader_keyok = ExternUtil(msg,  3);   break;
              case KK_ExternUtil04:   reader_keyok = ExternUtil(msg,  4);   break;
              case KK_ExternUtil05:   reader_keyok = ExternUtil(msg,  5);   break;
              case KK_ExternUtil06:   reader_keyok = ExternUtil(msg,  6);   break;
              case KK_ExternUtil07:   reader_keyok = ExternUtil(msg,  7);   break;
              case KK_ExternUtil08:   reader_keyok = ExternUtil(msg,  8);   break;
              case KK_ExternUtil09:   reader_keyok = ExternUtil(msg,  9);   break;
              case KK_ExternUtil10:   reader_keyok = ExternUtil(msg, 10);   break;
              case KK_ExternUtil11:   reader_keyok = ExternUtil(msg, 11);   break;
              case KK_ExternUtil12:   reader_keyok = ExternUtil(msg, 12);   break;
              case KK_ExternUtil13:   reader_keyok = ExternUtil(msg, 13);   break;
              case KK_ExternUtil14:   reader_keyok = ExternUtil(msg, 14);   break;
              case KK_ExternUtil15:   reader_keyok = ExternUtil(msg, 15);   break;
              case KK_ExternUtil16:   reader_keyok = ExternUtil(msg, 16);   break;
              case KK_ExternUtil17:   reader_keyok = ExternUtil(msg, 17);   break;
              case KK_ExternUtil18:   reader_keyok = ExternUtil(msg, 18);   break;
              case KK_ExternUtil19:   reader_keyok = ExternUtil(msg, 19);   break;
              case KK_ExternUtil20:   reader_keyok = ExternUtil(msg, 20);   break;
              case KK_ExternUtil21:   reader_keyok = ExternUtil(msg, 21);   break;
              case KK_ExternUtil22:   reader_keyok = ExternUtil(msg, 22);   break;
              case KK_ExternUtil23:   reader_keyok = ExternUtil(msg, 23);   break;
              case KK_ExternUtil24:   reader_keyok = ExternUtil(msg, 24);   break;

              case KK_ReadShowDel:
                CFG->showdeleted = not CFG->showdeleted;
                reader_keyok = true;
                break;

              case KK_ReadUndefine:
                reader_keyok = true;
                break;

              default:
                if(not PlayMacro(reader_keycode, KT_R)) {
                  SayBibi();
                  reader_keyok = true;
                }
            }
          } while(reader_keyok);
        }

        if(gkbd.quitall)
          break;

      } while(not reader_done);

      if(AA->isopen())
        AA->Close();

    } while(not reader_finished and not gkbd.quitall);

    HeaderView->Destroy();
    BodyView->Destroy();
  }

  ResetMsg(msg);
  throw_free(msg);
  // Invalidate reader_msg since the actual data has just been freed.
  reader_msg = NULL;

  HandleGEvent(EVTT_EXIT);
}


//  ------------------------------------------------------------------
//  Determine if the message has replies

ulong MsgHasReplies(GMsg* msg) {

  if(msg->link.first())
    return msg->link.first();

  for(int n=0; n<msg->link.list_max(); n++)
    if(msg->link.list(n))
      return msg->link.list(n);

  if(msg->link.next())
    return msg->link.next();

  return 0;
}


//  ------------------------------------------------------------------
//  Determine if the message is of the "twit" type

int MsgIsTwit(GMsg* msg, bool& istwitto, bool& istwitsubj) {

  bool istwit = false;
  istwitto = istwitsubj = false;

  // Check for twit names
  std::vector<Node>::iterator tn;
  for(tn = CFG->twitname.begin(); tn != CFG->twitname.end(); tn++) {
    if(msg->orig.match(tn->addr)) {
      if(*tn->name == NUL or strwild(msg->By(), tn->name)) {
        istwit = true;
        break;
      }
    }
    if(CFG->switches.get(twitto)) {
      if(msg->dest.match(tn->addr)) {
        if(*tn->name == NUL or strwild(msg->To(), tn->name)) {
          istwitto = true;
          istwit = true;
          break;
        }
      }
    }
  }

  // Check for twit subjects
  if(not istwit) {
    gstrarray::iterator n;
    for(n = CFG->twitsubj.begin(); n != CFG->twitsubj.end(); n++) {
      if(striinc(n->c_str(), msg->re)) {
        istwitsubj = true;
        istwit = true;
        break;
      }
    }
  }

  if(not istwit)
    return NOT_TWIT;

  if(AA->Twitmode() == TWIT_BLANK)
    return TWIT_BLANK;

  if(AA->Twitmode() == TWIT_KILL)
    return TWIT_KILL;

  if(AA->Twitmode() == TWIT_IGNORE)
    return TWIT_SKIP;

  if((AA->Twitmode() == TWIT_SKIP) and not msg->attr.tou())
    return TWIT_SKIP;

  return TWIT_SHOW;
}


//  ------------------------------------------------------------------

int LoadMessage(GMsg* msg, int margin) {

  GFTRK("LoadMessage");
  if(AA->LoadMsg(msg, AA->Msgn.CvtReln(AA->lastread()), margin)) {

    // Mark message as received, if it is for us
    if(msg->msgno and not AA->attr().hex()) {
      for(std::vector<Node>::iterator n = CFG->username.begin(); n != CFG->username.end(); n++) {
        // Check TO:
        if(msg->dest.match(n->addr)) {
          if(strieql(n->name, msg->To())) {
            if(n->addr.net != GFTN_ALL or msg->dest.net == 0 or not AA->isnet())
              msg->attr.tou1();   // Set highlight mark
            else {
              for(std::vector<gaka>::iterator x = CFG->aka.begin(); x != CFG->aka.end(); x++) {
                if(msg->dest.match(x->addr)) {
                  msg->attr.tou1();   // Set highlight mark
                  break;
                }
              }
            }
          }
        }

        // Check FROM:
        if(msg->orig.match(n->addr)) {
          if(strieql(n->name, msg->by)) {
            if(n->addr.net != GFTN_ALL or msg->orig.net == 0 or not AA->isnet())
              msg->attr.fmu1();   // Set highlight mark
            else {
              for(std::vector<gaka>::iterator x = CFG->aka.begin(); x != CFG->aka.end(); x++) {
                if(msg->orig.match(x->addr)) {
                  msg->attr.fmu1();   // Set highlight mark
                  break;
                }
              }
            }
          }
        }
      }

      if(*AA->Internetaddress()) {
        if(strieql(msg->to, AA->Internetaddress()) or strieql(msg->idest, AA->Internetaddress()))
          msg->attr.tou1();
      }

      if(reader_msglistfirst == false) {
        if(msg->attr.tou()) {
          reader_rcv_noise = 1;
          if(not msg->attr.rcv()) {         // Have we seen it?
            time_t a = time(NULL);
            struct tm *tp = gmtime(&a);
            tp->tm_isdst = -1;
            time_t b = mktime(tp);
            msg->received = a + a - b;      // Get current date
            msg->attr.rcv1();               // Mark as received
            reader_rcv_noise++;
          }
          else if(not CFG->switches.get(rcvdisablescfm))
            reader_rcv_noise++;
        }

        // Touch the msg if it's the first time received
        if(reader_rcv_noise > 1)
          msg->attr.upd1();   // Tell SaveHdr to "touch" the time stamp of the *.MSG file

        // Update the "Times Read" field
        msg->orig_timesread = msg->timesread++;

        if(reader_rcv_noise > 1) {
          GMsg* tmsg = (GMsg*) throw_calloc(1, sizeof(GMsg));
          AA->LoadHdr(tmsg, msg->msgno, false);
          tmsg->attr = msg->attr;
          tmsg->orig_timesread = msg->orig_timesread;
          tmsg->received = msg->received;
          AA->SaveHdr(GMSG_UPDATE, tmsg);
          throw_free(tmsg);
        }

        msg->attr.upd0();

        if((reader_rcv_noise > 1) and AA->isnet() and (msg->attr.cfm() or msg->attr.rrq()))
          reader_gen_confirm = true;
      }
    }
  }
  else {
    msg->txt = throw_strdup("");
  }

  GFTRK(NULL);
  return true;
}


//  ------------------------------------------------------------------

uint next_msg(int direction) {

  uint relno = 0;

  if(direction == DIR_NEXT) {
    if(AA->lastread() < AA->Msgn.Count()) {
      AA->set_lastread(AA->lastread()+1);
      relno = AA->lastread();
    }
  }
  else {
    if(1 < AA->lastread()) {
      AA->set_lastread(AA->lastread()-1);
      relno = AA->lastread();
    }
  }
  return relno;
}


//  ------------------------------------------------------------------

void ToggleMarkRead() {

  if(AA->PMrk.Tags()) {
    AA->isreadpm = not AA->isreadpm;
    if(AA->isreadpm) {
      bool is_marked = false;
      if(AA->PMrk.Find(reader_msg->msgno))
        is_marked = true;
      if(not is_marked)
        NextMarkedmsg(reader_direction, reader_msg);
    }
  }
  else if(AA->Mark.Count()) {
    AA->isreadmark = not AA->isreadmark;
    if(AA->isreadmark) {
      bool is_marked = false;
      if(AA->Mark.Find(reader_msg->msgno))
        is_marked = true;
      if(not is_marked)
        NextMarkedmsg(reader_direction, reader_msg);
    }
  }
  else if(AA->isreadmark)
    AA->isreadmark = false;
  else
    reader_keyok = true;
}


//  ------------------------------------------------------------------

void GotoBookMark() {

  if(AA->bookmark) {
    ulong temp = reader_msg->msgno;
    AA->set_lastread(AA->Msgn.ToReln(AA->bookmark));
    AA->bookmark = temp;
  }
  else {
    reader_keyok = true;
    SayBibi();
  }
}


//  ------------------------------------------------------------------

void GoNextMsg() {

  GMenuNewarea MenuNewarea;
  int mode;

  if(AA->isreadmark or AA->isreadpm) {
    if(not NextMarkedmsg(reader_direction, reader_msg)) {
      if((mode = MenuNewarea.Run()) != 0) {
        NewArea(mode == 2);
        return;
      }
      reader_keyok = true;
    }
  }
  else if(not next_msg(reader_direction)) {
    if((mode = MenuNewarea.Run())!=0) {
      if(AA->Msgn.ToReln(reader_msg->msgno) == 1)
        if(reader_direction == DIR_PREV)
          AA->set_lastread(0);
      NewArea(mode == 2);
      return;
    }
    reader_keyok = true;
  }
}


//  ------------------------------------------------------------------

void GotoPrevMsg() {

  reader_direction = DIR_PREV;
  GoNextMsg();
}


//  ------------------------------------------------------------------

void GotoNextMsg() {

  reader_direction = DIR_NEXT;
  GoNextMsg();
}


//  ------------------------------------------------------------------

void GotoFirstMsg() {

  reader_direction = DIR_NEXT;
  if(AA->isreadpm)
    AA->set_lastread(AA->PMrk.ToReln(AA->PMrk[0]));
  else if(AA->isreadmark)
    AA->set_lastread(AA->Msgn.ToReln(AA->Mark[0]));
  else
    AA->set_lastread(AA->Msgn.Count() ? 1 : 0);
}


//  ------------------------------------------------------------------

void GotoLastMsg() {

  reader_direction = DIR_PREV;
  if(AA->isreadpm)
    AA->set_lastread(AA->Msgn.ToReln(AA->PMrk[AA->PMrk.Tags()-1]));
  else if(AA->isreadmark)
    AA->set_lastread(AA->Msgn.ToReln(AA->Mark[AA->Mark.Count()-1]));
  else
    AA->set_lastread(AA->Msgn.Count());
}


//  ------------------------------------------------------------------

struct ReplySel {
  uint reln;
  char isread;
  char msgno[12];
  Name name;
  Name addr;
  char written[25];
};


//  ------------------------------------------------------------------

void GotoReplies() {

  reader_direction = DIR_NEXT;

  GMsg* msg = reader_msg;
  const int list_max = msg->link.list_max();

  char buf[200];
  int replies = 0;
  int cursorbar = -1;
  uint maxname = 0;
  uint maxaddr = 0;
  uint gotolink = 0;
  uint maxmsgno = 0;
  uint maxwritten = 0;

  GMsg* rmsg = (GMsg*)throw_calloc(1, sizeof(GMsg));
  ReplySel* rlist = (ReplySel*)throw_calloc(list_max+3, sizeof(ReplySel));

  for(int n=0; n<list_max+2; n++) {
    ulong msgn = n ? ((n!=list_max+1) ? msg->link.list(n-1) : msg->link.next()) : msg->link.first();
    uint reln = AA->Msgn.ToReln(msgn);
    if(reln) {
      if(CFG->replylinklist == REPLYLINKLIST_FULL)
        AA->LoadMsg(rmsg, msgn, CFG->dispmargin-(int)CFG->switches.get(disppagebar));
      else
        AA->LoadHdr(rmsg, msgn);
      rlist[replies].isread = ((0 == rmsg->timesread) and CFG->switches.get(highlightunread)) ? '\x10' : ' ';
      if((cursorbar == -1) and (0 == rmsg->timesread) and (CFG->switches.get(highlightunread)))
        cursorbar = replies;
      rlist[replies].msgno[0] = (n==list_max+1) ? '*' : '+';
      ulong msgno = CFG->switches.get(disprealmsgno) ? msgn : reln;
      sprintf(rlist[replies].msgno+1, "%lu", msgno);
      maxmsgno = MaxV(maxmsgno, (uint)strlen(rlist[replies].msgno+1));
      strcpy(rlist[replies].name, rmsg->by);
      maxname = MaxV(maxname, (uint)strlen(rlist[replies].name));
      if(not AA->isinternet()) {
        rmsg->orig.make_string(buf);
        if(*buf)
          sprintf(rlist[replies].addr, " (%s) ", buf);
      }
      maxaddr = MaxV(maxaddr, (uint)strlen(rlist[replies].addr));
      strftimei(rlist[replies].written, CFG->disphdrdateset.len, LNG->DateTimeFmt, gmtime(&rmsg->written));
      maxwritten = MaxV(maxwritten, (uint)strlen(rlist[replies].written));
      rlist[replies].reln = reln;
      replies++;
      if(gotolink == 0)
        gotolink = reln;
    }
  }
  ResetMsg(rmsg);
  throw_free(rmsg);

  int selected = 0;
  if(replies > 1) {
    uint maxname2 = MAXCOL-16-maxmsgno-maxaddr-maxwritten;
    maxname = MinV(maxname, maxname2);
    char** listr = (char**)throw_calloc(list_max+3, sizeof(char*));
    int n;
    for(n=0; n<list_max+3; n++) {
      if(n<replies) {
        sprintf(buf, "%c %c %*s : %-*.*s  %-*s  %-*s ",
          rlist[n].isread,
          rlist[n].msgno[0],
          maxmsgno, rlist[n].msgno+1,
          (int) maxname, (int) maxname, rlist[n].name,
          maxaddr, rlist[n].addr,
          maxwritten, rlist[n].written
        );
        listr[n] = throw_strdup(buf);
      }
      else {
        listr[n] = NULL;
        break;
      }
    }
    set_title(LNG->Replies, TCENTER, C_ASKT);
    update_statusline(LNG->SelectReply);
    whelppcat(H_GotoReplies);
    int pick_max = MinV(replies, (MAXROW-10));
    if(cursorbar < 0)
      cursorbar = 0;
    selected = wpickstr(6, 0, 6+pick_max+1, -1, W_BASK, C_ASKB, C_ASKW, C_ASKS, listr, cursorbar, title_shadow);
    whelpop();
    gotolink = (selected != -1) ? rlist[selected].reln : 0;
    for(n=0; n<replies; n++)
      throw_free(listr[n]);
    throw_free(listr);
  }

  throw_free(rlist);

  if(gotolink)
    AA->set_lastread(gotolink);
  else {
    if(selected != -1)
      SayBibi();
    reader_keyok = true;
  }
}


//  ------------------------------------------------------------------

void GotoReply1st() {

  reader_direction = DIR_NEXT;
  uint gotolink = AA->Msgn.ToReln(reader_msg->link.first());
  if(gotolink)
    AA->set_lastread(gotolink);
  else {
    SayBibi();
    reader_keyok = true;
  }
}


//  ------------------------------------------------------------------

void GotoReplyNext() {

  reader_direction = DIR_NEXT;
  uint gotolink = AA->Msgn.ToReln(reader_msg->link.next());
  if(gotolink)
    AA->set_lastread(gotolink);
  else {
    SayBibi();
    reader_keyok = true;
  }
}


//  ------------------------------------------------------------------

void GotoReplyPrev() {

  reader_direction = DIR_PREV;
  uint gotolink = AA->Msgn.ToReln(reader_msg->link.to());
  if(gotolink)
    AA->set_lastread(gotolink);
  else {
    SayBibi();
    reader_keyok = true;
  }
}


//  ------------------------------------------------------------------

void GotoPrevUnread() {

  reader_direction = DIR_PREV;
  w_info(LNG->Wait);

  GMsg* msg = (GMsg*)throw_calloc(1, sizeof(GMsg));

  bool found = false;
  int prev = AA->lastread();
  while(1 < prev) {
    prev--;
    AA->LoadHdr(msg, AA->Msgn.CvtReln(prev), false);
    if(msg->timesread == 0) {
      AA->set_lastread(prev);
      found = true;
      break;
    }
  }

  ResetMsg(msg);
  throw_free(msg);

  if(not found) {
    SayBibi();
    reader_keyok = true;
  }
  w_info(NULL);
}


//  ------------------------------------------------------------------

void GotoNextUnread() {

  w_info(LNG->Wait);
  reader_direction = DIR_NEXT;

  GMsg* msg = (GMsg*)throw_calloc(1, sizeof(GMsg));

  bool found = false;
  int count = AA->Msgn.Count();
  int next = AA->lastread();
  while(next < count) {
    next++;
    AA->LoadHdr(msg, AA->Msgn.CvtReln(next));
    if(msg->timesread == 0) {
      AA->set_lastread(next);
      found = true;
      break;
    }
  }

  ResetMsg(msg);
  throw_free(msg);

  if(not found) {
    SayBibi();
    reader_keyok = true;
  }
  w_info(NULL);
}


//  ------------------------------------------------------------------

