
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
//  Global data from GEREAD & GECTRL

extern GMsg* reader_msg;
extern int _use_fwd;


//  ------------------------------------------------------------------

void Area::DeleteMsg(GMsg* msg, int direction) {

  uint32_t replyto=0, reply1st=0, lread;

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
    //                  ����������Ŀ
    //  Downlink --> Deleted --> Uplink
    //      �������������
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
    if(isnet())
      TouchNetscan(false);

    if(not issoftdelete()) {
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

void Area::DelMsgs(GMsg* msg, bool force)
{
  GFTRK("DelMsgs");
  GMenuDelete MenuDelete;

  if (!force && CFG->arearecyclebin[0])
  {
    int areano = AL.AreaEchoToNo(CFG->arearecyclebin);
    int currno = AL.AreaIdToNo(CurrArea);

    if ((areano != currno) && (areano >= 0))
    {
      CopyMoveForward(true);
      GFTRK(0);
      return;
    }
  }

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
        update_statuslinef(LNG->DeletingMsg, "ST_DELETINGMSG", x+1, Mark.Count());
        w_progress(MODE_UPDATE, C_INFOW, x+1, Mark.Count(), LNG->Deleting);
        uint msgno = Mark[x];
        if(Msgn.ToReln(msgno)) {
          if(LoadHdr(msg, msgno, false)) {
            bool deletethis = false;
            if(delask) {
              if(msg->attr.uns() and not (msg->attr.rcv() or msg->attr.del())) {
                AA->LoadMsg(msg, msgno, CFG->dispmargin-(int)CFG->switches.get(disppagebar));
                w_progress(MODE_QUIT, BLACK_|_BLACK, 0, 0, NULL);
                HeaderView->Use(AA, msg);
                HeaderView->Paint();
                BodyView->Use(AA, msg, topline);
                BodyView->Paint();

                switch(MenuDelete.Run(YES, msg))
                {
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
              w_progress(MODE_QUIT, BLACK_|_BLACK, 0, 0, NULL);
              HeaderView->Use(AA, msg);
              HeaderView->Paint();
              BodyView->Use(AA, msg, topline);
              BodyView->Paint();

              switch (MenuDelete.Run(YES, msg))
              {
                case YES:       // Yes, delete
                  break;
                case NO:        // No, dont delete
                  continue;
                default:        // Delete without asking
                  dellocked = true;
              }
            }
            DeleteMsg(msg, reader_direction);
            PMrk.Del(msg->msgno);
          }
        }
      }
      Unlock();
      if(not escaped) {
        isreadmark = false;
        Mark.ResetAll();
      }
      w_progress(MODE_QUIT, BLACK_|_BLACK, 0, 0, NULL);
    }
  }

  if (n == 0)
  {
    Mark.Del(msg->msgno);
    PMrk.Del(msg->msgno);

    if (Mark.Count() or MenuDelete.Run(NO, msg))
    {
      HandleGEvent(EVTT_MSGDELETING);
      DeleteMsg(msg, reader_direction);
    }
  }

  HandleGEvent(EVTT_BREAKLOOP);

  GFTRK(0);
}


//  ------------------------------------------------------------------

void TwitDeleteMsg(GMsg* msg) {

  GFTRK("TwitDeleteMsg");

  HandleGEvent(EVTT_MSGDELETING);
  AA->Mark.Del(msg->msgno);
  AA->PMrk.Del(msg->msgno);
  AA->DeleteMsg(msg, reader_direction);
  HandleGEvent(EVTT_BREAKLOOP);

  GFTRK(0);
}


//  ------------------------------------------------------------------

void Area::DelMsg(bool force)
{
  if(Msgn.Count())
    DelMsgs(reader_msg, force);
}


//  ------------------------------------------------------------------

void CmfMsgs(GMsg* msg, bool torecycle)
{
  // Select action
  int cmf;

  if (torecycle) cmf = MODE_MOVE;
  else
  {
    GMenuCMF MenuCMF;
    cmf = MenuCMF.Run();
    if(cmf == -1)
      return;
  }

  // Set language strings
  char* pickstr = NULL;
  char* markstr = NULL;
  char* progstr = NULL;
  char* statstr = NULL;
  char* tokenstr = "";
  int loadmode = GMSG_UNS_NOT_RCV;
  switch(cmf) {
    case MODE_COPY:
      GFTRK("CopyMsgs");
      pickstr = LNG->CopyArea;
      markstr = LNG->Copy;
      progstr = LNG->Copying;
      statstr = LNG->CopyingMsg;
      tokenstr = "ST_COPYINGMSG";
      loadmode |= GMSG_COPY;
      break;
    case MODE_MOVE:
      if(AA->attr().r_o()) {
        GMenuReadonly MenuReadonly;
        if(not MenuReadonly.Run())
          return;
      }

      if (torecycle)
      {
        GFTRK("DeleteMsgs");
        pickstr = ">>Delete To Area: ";
        markstr = " Delete ";
        progstr = " Deleting ";
        statstr = "Deleting Msg %u of %u to %s";
      }
      else
      {
        GFTRK("MoveMsgs");
        pickstr = LNG->MoveArea;
        markstr = LNG->Move;
        progstr = LNG->Moving;
        statstr = LNG->MovingMsg;
        tokenstr = "ST_MOVINGMSG";
      }

      loadmode |= GMSG_MOVE;
      break;
    case MODE_FORWARD:
      GFTRK("ForwardMsgs");
      pickstr = LNG->ForwardArea;
      markstr = " Forward ";
      progstr = " Forwarding ";
      statstr = "Forwarding Msg %u of %u to %s";
      break;
    case MODE_UPDATE:
      GFTRK("ToggleSent");
      loadmode |= GMSG_UPDATE;
      break;
  }

  // Do with current or marked msgs?
  int do_mode = MODE_CURRENT;
  if (AA->Mark.Count())
  {
    GMenuDomarks MenuDomarks;
    do_mode = MenuDomarks.Run(markstr);
    if (do_mode == MODE_DONT)
    {
      GFTRK(0);
      return;
    }
  }

  if (cmf == MODE_UPDATE)
  {
    w_info(LNG->Wait);

    uint32_t loadmsgno = msg->msgno;
    uint32_t mrks = AA->Mark.Count();
    uint32_t *mrkp = AA->Mark.tag;
    uint32_t mrk = 0;

    do
    {
      if (do_mode == MODE_MARKED)
        loadmsgno = mrkp[mrk];

      mrk++;

      if (AA->LoadHdr(msg, loadmsgno, false))
      {
        msg->attr.sntX();
        if (msg->attr.snt())
        {
          msg->attr.uns0();
          msg->attr.scn1();
        }
        else
        {
          msg->attr.uns1();
          msg->attr.scn0();
          msg->attr.loc1();
        }

        AA->SaveHdr(GMSG_UPDATE, msg);
      }
    }
    while (mrk < mrks);

    AA->Mark.ResetAll();

    w_info(NULL);
    GFTRK(0);
    return;
  }

  // Pick the destination area
  bool fromrecycle = false;
  int destarea = CurrArea;
  const char* cmfptr;

  if (torecycle)
    cmfptr = CFG->arearecyclebin;
  else
  {
    if (CFG->arearecyclebin[0])
    {
      int areano = AL.AreaEchoToNo(CFG->arearecyclebin);
      int currno = AL.AreaIdToNo(CurrArea);

      if ((areano == currno) && (areano >= 0))
        fromrecycle = true;
    }

    if (fromrecycle)
      cmfptr = msg->areakludgeid;
    else
      cmfptr = (cmf == MODE_FORWARD) ? AA->Areareplyto() : AA->Areacopyto();
  }

  if (cmfptr[0])
  {
    int a = AL.AreaEchoToNo(cmfptr);
    if (a != -1)
      destarea = AL.AreaNoToId(a);
  }

  if (!torecycle && (cmf == MODE_FORWARD ? not AA->Areaforwarddirect() : not AA->Areacopydirect()))
    destarea = AreaPick(pickstr, 6, &destarea);

  if (destarea == -1)
  {
    GFTRK(0);
    return;
  }

  int xlat_table = LoadCharset(NULL, NULL, 1);

  AL.SetActiveAreaId(OrigArea);
  AreaData* orig_adat = AA->adat;
//  AA->adat = (AreaData*)throw_calloc(1, sizeof(AreaData));
//  memcpy(AA->adat, orig_adat, sizeof(AreaData));
  AA->adat = new AreaData;
  *AA->adat = *orig_adat;
  AL.SetActiveAreaId(destarea);
  Area* AAdest = AA;

  AA->RandomizeData();
  if (torecycle) AA->adat->areacopyaddid = true;

  // Is it readonly?
  if(AA->attr().r_o()) {
    GMenuReadonly MenuReadonly;
    if(not MenuReadonly.Run())
    {
      AL.SetActiveAreaId(OrigArea);
//      throw_free(AA->adat);
      delete AA->adat;
      AA->adat = orig_adat;
      GFTRK(0);
      return;
    }
  }

  // Handle a forward
  if (cmf == MODE_FORWARD)
  {
    _use_fwd = orig_adat->usefwd;
    if (_use_fwd == ASK)
    {
      GMenuForward MenuForward;
      _use_fwd = MenuForward.Run();
    }
  }

  // Popup wait window
  w_info(progstr);

  // Open destination area
  AA->Open();
  AA->RandomizeData();

  // Re-activeate original area and lock that too
  AL.SetActiveAreaId(OrigArea);
  AA->Lock();

  // Setup some variables for the loop
  Area* AAorig = AA;
  const char* echoid = AAdest->echoid();
  uint loadmsgno = msg->msgno;
  uint32_t* mrkp = AA->Mark.tag;
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
      update_statuslinef(statstr, tokenstr, mrk+1, mrks, echoid);
      loadmsgno = *mrkp++;
    }

    int mode = 0;
    uint msgno = 0;
    if(AA->LoadMsg(msg, loadmsgno, CFG->dispmargin-(int)CFG->switches.get(disppagebar), loadmode)) {

      // Handle unsent msgs
      mode = cmf;
      if ((loadmode & GMSG_UNS_NOT_RCV) &&
          ((torecycle && CFG->arearecyclebinask) ||
          ((cmf == MODE_MOVE) && msg->attr.uns() && !msg->attr.rcv())))
      {
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
        switch(MenuDelete.Run(YES, msg))
        {
          case YES:  break;
          case NO:
            if (torecycle) continue;
            mode = MODE_COPY;
            break;
          default:   loadmode &= ~GMSG_UNS_NOT_RCV;
        }

        // Put up the wait window again
        w_info(progstr);
      }

      // Switch to destination area
      AA = AAdest;

      if (cmf == MODE_FORWARD)
        MakeMsg(MODE_FORWARD, msg);
      else
      {
      // Change things in the header to match the destination area
      msg->attr.del0();   // Allows deleted msgs to be undeleted
      msg->board = AA->board();
      msg->link.reset();

      // Fake Scn-Attribute if copied to squisharea
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

      if (not need_netmail_kludges &&
          (AAorig->basetype() == "SQUISH") &&
          (AAdest->basetype() == "SQUISH") &&
          AAdest->isnet() && AAorig->isnet())
      {
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

        if (not strstr(msg->txt, "\001INTL"))
          // The INTL kludge for zone crossing
          if (CFG->useintl and (CFG->useintl == YES or (msg->dest.zone != msg->orig.zone)))
          {
            gsprintf(PRINTF_DECLARE_BUFFER(buf), "\001INTL %u:%u/%u %u:%u/%u%c",
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

      } //if (cmf == MODE_FORWARD)

      // Switch back to original area
      AA = AAorig;
    }

    // Delete original msg if moved
    if(mode == MODE_MOVE) {
      msg->msgno = msgno;
      AA->DeleteMsg(msg, torecycle ? reader_direction : DIR_PREV);
      AA->PMrk.Del(msg->msgno);
    }

  } while((do_mode == MODE_MARKED) and ((++mrk) < mrks));

  kbput(Key_Tick);

  // close destination area
  AL.SetActiveAreaId(destarea);
  AA->UpdateAreadata();
  AA->Close();

  // Return to original area and unlock it
  AL.SetActiveAreaId(OrigArea);
//  throw_free(AA->adat);
  delete AA->adat;
  AA->adat = orig_adat;
  AA->Unlock();

  if(xlat_table != -1)
    LoadCharset(CFG->xlatcharset[xlat_table].imp, CFG->xlatcharset[xlat_table].exp);
  else
    LoadCharset("N/A", "N/A");

  if(do_mode == MODE_MARKED)
  {
    if(cmf == MODE_MOVE)
      AA->Mark.ResetAll();
  }

  w_info(NULL);

  GFTRK(0);
}


//  ------------------------------------------------------------------

void CopyMoveForward(bool torecycle)
{
  AA->attr().hex0();
  if(AA->Msgn.Count())
    CmfMsgs(reader_msg, torecycle);
}


//  ------------------------------------------------------------------

