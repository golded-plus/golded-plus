
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
//  Copy, Move, Forward and Delete.
//  ------------------------------------------------------------------

#include <golded.h>


//  ------------------------------------------------------------------
//  Global data from GEREAD

extern GMsg* reader_msg;


//  ------------------------------------------------------------------

void Area::DeleteMsg(GMsg* msg, int direction) {

  ulong replyto=0, reply1st=0, lread;

  GMsg* uplink = (GMsg*)throw_calloc(1, sizeof(GMsg));
  GMsg* downlink = (GMsg*)throw_calloc(1, sizeof(GMsg));

  if(msg->msgno) {

    // Fix the lastread pointer
    lread = Msgn.CvtReln(lastread());
    if(msg->msgno == lread) {
      uint l = lastread();
      if(direction == DIR_PREV) {
        if(l-1) {
          l--;
        }
        else if((l+1) <= Msgn.Count())
          l++;
        else
          l = 0;
      }
      else {
        if((l+1) <= Msgn.Count())
          l++;
        else if(l-1)
          l--;
        else
          l = 0;
      }
      set_lastread(l);
      lread = Msgn.CvtReln(lastread());
    }

    // Get reply links
    if(Msgn.ToReln(msg->link.to())) {
      replyto = msg->link.to();
    }
    if(Msgn.ToReln(msg->link.first())) {
      reply1st = msg->link.first();
    }
    if(replyto) {
      if(not LoadHdr(downlink, replyto, false))
        downlink->link.first_set(0);
    }
    if(reply1st) {
      if(not LoadHdr(uplink, reply1st, false))
        uplink->link.to_set(0);
    }

    //  --------------------------------------------------------------
    //  Diagram of splicing the links of a deleted msg
    //  --------------------------------------------------------------
    //               Reply1st    Replyto
    //                  ÚÄÄÄÄÄÄÄÄÄÄ¿
    //  Downlink --> Deleted --> Uplink
    //      ÀÄÄÄÄÄÄÄÄÄÄÄÙ
    //  Reply1st     Replyto                            Confused ? :-)
    //  --------------------------------------------------------------

    // Only update the up/downlinks if they are pointing to this msg
    if(replyto) {
      if(downlink->link.first() == msg->msgno) {
        downlink->link.first_set(reply1st);
        SaveHdr(GMSG_UPDATE, downlink);
      }
      else {
        replyto = 0;
      }
    }
    if(reply1st) {
      if(uplink->link.to() == msg->msgno) {
        uplink->link.to_set(replyto);
        SaveHdr(GMSG_UPDATE, uplink);
      }
    }

    // Delete the message
    msg->attr.del1();
    DelMsg(msg);

    // Update scanning files
    if(isnet() and isfido())
      TouchNetscan(false);

    if(not issmb() and not (isjam() and not CFG->switches.get(jamharddelete))) {
      // Remove message from internal table
      Msgn.Del(msg->msgno);
    }

    // Update lastreads
    if(Msgn.Count())
      set_lastread(Msgn.ToReln(lread));
    else
      set_lastread(0);
    msg->msgno = lread;
    UpdateAreadata();
  }

  ResetMsg(downlink);
  throw_free(downlink);

  ResetMsg(uplink);
  throw_free(uplink);
}


//  ------------------------------------------------------------------

void Area::DelMsgs(GMsg* msg) {

  GFTRK("DelMsgs");

  uint n, x;
  int topline=0;
  bool delask=true, dellocked=false;

  n = 0;
  if(Mark.Count()) {
    GMenuDomarks MenuDomarks;
    n = MenuDomarks.Run(LNG->Delete);
    if(n == 1) {
      HandleGEvent(EVTT_MSGDELETING);
      w_progress(MODE_NEW, C_INFOW, 0, Mark.Count(), LNG->Deleting);
      Lock();
      int escaped = false;
      for(n=0; n<Mark.Count(); n++) {
        x = n;
        if(kbxhit()) {
          if(kbxget() == Key_Esc) {
            HandleGEvent(EVTT_JOBFAILED);
            escaped = true;
            break;
          }
        }
        update_statuslinef(LNG->DeletingMsg, x+1, Mark.Count());
        w_progress(MODE_UPDATE, C_INFOW, x+1, Mark.Count(), LNG->Deleting);
        ulong msgno = Mark[x];
        if(Msgn.ToReln(msgno)) {
          if(LoadHdr(msg, msgno, false)) {
            bool deletethis = false;
            if(delask) {
              if(msg->attr.uns() and not (msg->attr.rcv() or msg->attr.del())) {
                AA->LoadMsg(msg, msgno, CFG->dispmargin-(int)CFG->switches.get(disppagebar));
                w_progress(MODE_QUIT, 0, 0, 0, NULL);
                HeaderView->Use(AA, msg);
                HeaderView->Paint();
                BodyView->Use(AA, msg, topline);
                BodyView->Paint();
                GMenuDelete MenuDelete;
                switch(MenuDelete.Run(YES, msg)) {
                  case YES:       // Yes, delete
                    deletethis = true;
                    break;
                  case NO:        // No, dont delete
                    continue;
                  default:        // Delete without asking
                    delask = false;
                    if(msg->attr.lok())
                      dellocked = true;
                }
              }
            }
            if(msg->attr.lok() and not dellocked and not deletethis) {
              AA->LoadMsg(msg, msgno, CFG->dispmargin-(int)CFG->switches.get(disppagebar));
              w_progress(MODE_QUIT, 0, 0, 0, NULL);
              HeaderView->Use(AA, msg);
              HeaderView->Paint();
              BodyView->Use(AA, msg, topline);
              BodyView->Paint();
              GMenuDelete MenuDelete;
              switch(MenuDelete.Run(YES, msg)) {
                case YES:       // Yes, delete
                  break;
                case NO:        // No, dont delete
                  continue;
                default:        // Delete without asking
                  dellocked = true;
              }
            }
            DeleteMsg(msg, DIR_PREV);
            PMrk.Del(msg->msgno);
          }
        }
      }
      Unlock();
      if(not escaped) {
        isreadmark = false;
        Mark.ResetAll();
      }
      w_progress(MODE_QUIT, 0, 0, 0, NULL);
    }
  }
  if(n == 0) {
    Mark.Del(msg->msgno);
    PMrk.Del(msg->msgno);
    GMenuDelete MenuDelete;
    if(Mark.Count() or MenuDelete.Run(NO, msg)) {
      HandleGEvent(EVTT_MSGDELETING);
      DeleteMsg(msg, reader_direction);
    }
  }

  HandleGEvent(EVTT_BREAKLOOP);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void TwitDeleteMsg(GMsg* msg) {

  GFTRK("TwitDeleteMsg");

  HandleGEvent(EVTT_MSGDELETING);
  AA->Mark.Del(msg->msgno);
  AA->PMrk.Del(msg->msgno);
  AA->DeleteMsg(msg, reader_direction);
  HandleGEvent(EVTT_BREAKLOOP);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void Area::DelMsg() {

  if(Msgn.Count())
    DelMsgs(reader_msg);
}


//  ------------------------------------------------------------------

void CmfMsgs(GMsg* msg) {

  // Select action
  GMenuCMF MenuCMF;
  int cmf = MenuCMF.Run();
  if(cmf == -1)
    return;
  
  // Set language strings
  char* pickstr = NULL;
  char* markstr = NULL;
  char* progstr = NULL;
  char* statstr = NULL;
  int loadmode = GMSG_UNS_NOT_RCV;
  switch(cmf) {
    case MODE_COPY:
      GFTRK("CopyMsgs");
      pickstr = LNG->CopyArea;
      markstr = LNG->Copy;
      progstr = LNG->Copying;
      statstr = LNG->CopyingMsg;
      loadmode |= GMSG_COPY;
      break;
    case MODE_MOVE:
      GFTRK("MoveMsgs");
      pickstr = LNG->MoveArea;
      markstr = LNG->Move;
      progstr = LNG->Moving;
      statstr = LNG->MovingMsg;
      loadmode |= GMSG_MOVE;
      break;
    case MODE_FORWARD:
      GFTRK("ForwardMsgs");
      pickstr = LNG->ForwardArea;
      break;
  }

  // Do with current or marked msgs?
  int do_mode = MODE_CURRENT;
  if(cmf != MODE_FORWARD) {
    if(AA->Mark.Count()) {
      GMenuDomarks MenuDomarks;
      do_mode = MenuDomarks.Run(markstr);
      if(do_mode == MODE_DONT) {
        GFTRK(NULL);
        return;
      }
    }
  }

  // Pick the destination area
  int destarea = CurrArea;
  const char* cmfptr = cmf == MODE_FORWARD ? AA->Areareplyto() : AA->Areacopyto();
  if(*cmfptr) {
    int a = AL.AreaEchoToNo(cmfptr);
    if(a != -1)
      destarea = AL.AreaNoToId(a);
  }
  if(cmf == MODE_FORWARD ? not AA->Areaforwarddirect() : not AA->Areacopydirect())
    destarea = AreaPick(pickstr, 6, &destarea);
  if(destarea == -1) {
    GFTRK(NULL);
    return;
  }

  AL.SetActiveAreaId(OrigArea);
  AreaData* orig_adat = AA->adat;
  AA->adat = (AreaData*)throw_calloc(1, sizeof(AreaData));
  memcpy(AA->adat, orig_adat, sizeof(AreaData));
  AL.SetActiveAreaId(destarea);
  Area* AAdest = AA;

  // Is it readonly?
  if(AA->attr().r_o()) {
    GMenuReadonly MenuReadonly;
    if(not MenuReadonly.Run()) {
      AL.SetActiveAreaId(OrigArea);
      throw_free(AA->adat);
      AA->adat = orig_adat;
      GFTRK(NULL);
      return;
    }
  }

  // Handle a forward
  if(cmf == MODE_FORWARD) {
    extern int _use_fwd;
    _use_fwd = orig_adat->usefwd;
    if(_use_fwd == ASK) {
      GMenuForward MenuForward;
      _use_fwd = MenuForward.Run();
    }
    if(CurrArea != OrigArea)
      AA->Open();
    MakeMsg(MODE_FORWARD, msg);
    if(CurrArea != OrigArea)
      AA->Close();
    AL.SetActiveAreaId(OrigArea);
    throw_free(AA->adat);
    AA->adat = orig_adat;
    GFTRK(NULL);
    return;
  }

  // Popup wait window
  w_info(progstr);

  // Open destination area and lock it
  AA->Open();
  AA->Lock();
  AA->RandomizeData();

  // Re-activeate original area and lock that too
  AL.SetActiveAreaId(OrigArea);
  AA->Lock();

  // Setup some variables for the loop
  Area* AAorig = AA;
  const char* echoid = AAdest->echoid();
  ulong loadmsgno = msg->msgno;
  ulong* mrkp = AA->Mark.tag;
  int  mrks = AA->Mark.Count();
  int  mrk = 0;

  // Copy/move loop
  do {

    // Check for escape key
    if(kbxhit()) {
      if(kbxget() == Key_Esc) {
        HandleGEvent(EVTT_JOBFAILED);
        break;
      }
    }

    // Show progress and load the marked msg
    if(do_mode == MODE_MARKED) {
      update_statuslinef(statstr, mrk+1, mrks, echoid);
      loadmsgno = *mrkp++;
    }

    int mode = 0;
    ulong msgno = 0;
    if(AA->LoadMsg(msg, loadmsgno, CFG->dispmargin-(int)CFG->switches.get(disppagebar), loadmode)) {

      // Handle unsent msgs
      mode = cmf;
      if((cmf == MODE_MOVE) and (loadmode & GMSG_UNS_NOT_RCV) and (msg->attr.uns() and not msg->attr.rcv())) {

        // Axe the popup wait window
        w_info(NULL);

        // Display header and message
        int top = 0;
        HeaderView->Use(AA, msg);
        HeaderView->Paint();
        BodyView->Use(AA, msg, top);
        BodyView->Paint();

        // Ask if it should be deleted
        GMenuDelete MenuDelete;
        switch(MenuDelete.Run(YES, msg)) {
          case YES:  break;
          case NO:   mode = MODE_COPY;  break;
          default:   loadmode &= ~GMSG_UNS_NOT_RCV;
        }

        // Put up the wait window again
        w_info(progstr);
      }

      // Switch to destination area
      AA = AAdest;

      // Change things in the header to match the destination area
      msg->attr.del0();   // Allows deleted msgs to be undeleted
      msg->board = AA->board();
      msg->link.reset();

      // Fake Scn-Attribute if copied to squisharea
      if(AA->issquish() and not AAorig->issquish())
        if(msg->attr.loc() and msg->attr.snt())
          msg->attr.scn1();

      if(AA->Areacopyaddid() and not AA->isnet()) {
        char* ptr = msg->txt + (*msg->txt == CTRL_A);
        if(not strnieql(ptr, "AREA:", 5)) {
          uint elen = 6 + strlen(AAorig->echoid()) + 1;
          uint mlen = strlen(msg->txt)+1;
          msg->txt = (char*)throw_realloc(msg->txt, elen+mlen);
          memmove(msg->txt+elen, msg->txt, mlen);
          sprintf(msg->txt, "\001AREA:%s", AAorig->echoid());
          msg->txt[elen-1] = CR;
        }
      }

      bool need_netmail_kludges = not AAorig->isnet() and AAdest->isnet();
      bool need_fmpt = true;
      bool need_topt = true;

      if(not need_netmail_kludges and AAorig->issquish() and not AAdest->issquish() and AAdest->isnet() and AAorig->isnet()) {
        if(msg->orig.point and not strstr(msg->txt, "\001FMPT"))
          need_netmail_kludges = true;
        else
          need_fmpt = false;
        if(msg->dest.point and not strstr(msg->txt, "\001TOPT"))
          need_netmail_kludges = true;
        else
          need_topt = false;
      }

      if(need_netmail_kludges) {
        char buf[256] = "";

        if(not strstr(msg->txt, "\001INTL"))
          // The INTL kludge for zone crossing
          if(CFG->useintl and (CFG->useintl == YES or (msg->dest.zone != msg->orig.zone))) {
            sprintf(buf, "\001INTL %u:%u/%u %u:%u/%u%c",
              msg->dest.zone ? msg->dest.zone : AA->Aka().addr.zone,
              msg->dest.net, msg->dest.node,
              msg->orig.zone ? msg->orig.zone : AA->Aka().addr.zone,
              msg->orig.net, msg->orig.node, CR
            );
          }

        if(not strstr(msg->txt, "\001TOPT") or not strstr(msg->txt, "\001FMPT")) {
          // The FMPT and TOPT kludges for point addressing
          if(msg->dest.point and need_topt)
            sprintf(buf+strlen(buf), "\001TOPT %u%c", msg->dest.point, CR);
          if(msg->orig.point and msg->orig.net == msg->oorig.net and msg->orig.node == msg->oorig.node and need_fmpt)
            sprintf(buf+strlen(buf), "\001FMPT %u%c", msg->orig.point, CR);
        }

        uint alen = strlen(buf);
        if(alen) {
          uint mlen = strlen(msg->txt)+1;
          msg->txt = (char*)throw_realloc(msg->txt, alen+mlen);
          memmove(msg->txt+alen, msg->txt, mlen);
          memmove(msg->txt, buf, alen);
        }
      }
      else if(AA->isecho() and not AAorig->isecho()) {

        char* kl = strstr(msg->txt, "\001INTL");
        char* p;
        if(kl and (p = strchr(kl, CR)) != NULL)
          memmove(kl, p, strlen(p)+1);
        kl = strstr(msg->txt, "\001TOPT");
        if(kl and (p = strchr(kl, CR)) != NULL)
          memmove(kl, p, strlen(p)+1);
        kl = strstr(msg->txt, "\001FMPT");
        if(kl and (p = strchr(kl, CR)) != NULL)
          memmove(kl, p, strlen(p)+1);
      }

      // Save the new msg to the destination area
      msgno = msg->msgno;
      AA->SaveMsg(GMSG_NEW|GMSG_NOLSTUPD, msg);

      // Switch back to original area
      AA = AAorig;
    }

    // Delete original msg if moved
    if(mode == MODE_MOVE) {
      msg->msgno = msgno;
      AA->DeleteMsg(msg, DIR_PREV);
      AA->PMrk.Del(msg->msgno);
    }

  } while((do_mode == MODE_MARKED) and ((++mrk) < mrks));

  kbput(Key_Tick);

  // Unlock and close destination area
  AL.SetActiveAreaId(destarea);
  AA->UpdateAreadata();
  AA->Unlock();
  AA->Close();

  // Return to original area and unlock it
  AL.SetActiveAreaId(OrigArea);
  throw_free(AA->adat);
  AA->adat = orig_adat;
  AA->Unlock();

  if(do_mode == MODE_MARKED) {
    if(cmf == MODE_MOVE)
      AA->Mark.ResetAll();
  }
  else {
    if(cmf == MODE_MOVE) {
      AA->Mark.Del(msg->msgno);
      AA->PMrk.Del(msg->msgno);
    }
  }

  w_info(NULL);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void CopyMoveForward() {

  ulong lastread = reader_msg->msgno;

  AA->set_lastread(AA->Msgn.ToReln(lastread, AA->lastread()));

  AA->attr().hex0();
  if(AA->Msgn.Count())
    CmfMsgs(reader_msg);

  AA->set_lastread(AA->Msgn.ToReln(lastread, AA->lastread()));
}


//  ------------------------------------------------------------------

