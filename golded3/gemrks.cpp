
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
//  Msg marks handling.
//  ------------------------------------------------------------------

#include <golded.h>
#include <gesrch.h>


//  ------------------------------------------------------------------

extern GMsg* reader_msg;


//  ------------------------------------------------------------------

int NextMarkedmsg(int direction, GMsg* msg) {

  GTag& tag = AA->isreadpm ? AA->PMrk : AA->Mark;

  if(tag.Count()) {
    uint n;
    ulong msgno = 0;
    if(direction == DIR_NEXT) {
      if(msg->msgno != tag[tag.Count()-1]) {
        for(n=0; n<tag.Count(); n++) {
          if(tag[n] > msg->msgno) {
            break;
          }
        }
        if(n >= tag.Count())
          n = tag.Count()-1;
        msgno = n+1;
      }
      else {
        n = tag.Count()-1;
      }
      AA->set_lastread(AA->Msgn.ToReln(tag[n]));
    }
    else {
      if(msg->msgno != tag[0]) {
        n = tag.Count();
        do {
          if(tag[--n] < msg->msgno)
            break;
        } while(n);
        msgno = n+1;
      }
      else {
        n = 0;
      }
      AA->set_lastread(AA->Msgn.ToReln(tag[n]));
    }
    return msgno ? 1 : 0;
  }
  return 0;
}


//  ------------------------------------------------------------------

void MarkMsgs_Unmark() {

  AA->Mark.ResetAll();
  AA->isreadmark = false;
}


//  ------------------------------------------------------------------

void MarkMsgs_Toggle() {

  GTag tmp;

  // Transplant current marks
  tmp.SetCount(AA->Mark.Count());
  tmp.tag = AA->Mark.tag;
  tmp.allocated = AA->Mark.allocated;

  // Reset marks
  AA->Mark.SetCount(0);
  AA->Mark.tag = NULL;
  AA->Mark.allocated = 0;
  AA->isreadmark = false;

  if(tmp.Count()) {
    AA->Mark.Resize(AA->Msgn.Count()-tmp.Count());
    uint m=0, x=0;
    for(uint n = 0; n < AA->Msgn.Count(); n++) {
      if(tmp.tag[x] != AA->Msgn[n])
        AA->Mark[m++] = AA->Msgn[n];
      else
        x++;
    }
  }

  tmp.Reset();
}


//  ------------------------------------------------------------------

void MarkMsgs_All() {

  AA->Mark.Resize(AA->Msgn.Count());
  memcpy(AA->Mark.tag, AA->Msgn.tag, AA->Msgn.Count()*sizeof(ulong));
}


//  ------------------------------------------------------------------

void MarkMsgs_New() {

  uint oldmarks = AA->Mark.Count();
  AA->Mark.Resize(AA->Mark.Count()+(AA->Msgn.Count() - AA->lastread()));
  memcpy(AA->Mark.tag+oldmarks, AA->Msgn.tag+AA->lastread(), (AA->Msgn.Count()-AA->lastread())*sizeof(ulong));
  AA->Mark.Sort();
  AA->Mark.ElimDups();
}


//  ------------------------------------------------------------------

void MarkMsgs_Old() {

  uint oldmarks = AA->Mark.Count();
  AA->Mark.Resize(AA->Mark.Count() + (AA->lastread() - 1));
  memcpy(AA->Mark.tag+oldmarks, AA->Msgn.tag, (AA->lastread()-1)*sizeof(ulong));
  AA->Mark.Sort();
  AA->Mark.ElimDups();
}


//  ------------------------------------------------------------------

void MarkMsgs_Range() {

  uint markstart = AA->Msgn.ToReln(AA->bookmark);
  uint markstop = AA->lastread();
  uint temp = MinV(markstart, markstop);
  uint mrks = (MaxV(markstart, markstop) - MinV(markstart, markstop)) + 1;
  uint oldmarks = AA->Mark.Count();
  AA->Mark.Resize(AA->Mark.Count() + mrks);
  for(uint n=0; n<mrks; n++)
    AA->Mark[oldmarks+n] = AA->Msgn[temp+n-1];
  AA->Mark.Sort();
  AA->Mark.ElimDups();
}


//  ------------------------------------------------------------------

void MarkMsgs_Txt(int item, char* markstring) {

  if(item == TAG_MARKHEADER or item == TAG_MARKTXTHDR) {
    if(not edit_string(markstring, sizeof(INam), LNG->EnterMarkString, H_MarkString))
      return;
  }

  GMsg* msg = (GMsg*)throw_calloc(1, sizeof(GMsg));

  golded_search_manager srchmgr;
  srchmgr.prepare_from_string(markstring, (item == TAG_MARKTXTHDR) ? GFIND_HDRTXT : GFIND_HDR);

  w_progress(MODE_NEW, C_INFOW, 0, AA->Msgn.Count(), LNG->AdvancedMarking);

  uint n;
  int marked=0;
  for(n=AA->lastread(); srchmgr.direction == DIR_NEXT ? (n<=AA->Msgn.Count()) : (n>=1); srchmgr.direction == DIR_NEXT ? n++ : n--) {
    if(kbxhit()) {
      if(kbxget() == Key_Esc) {
        HandleGEvent(EVTT_SEARCHFAILED);
        break;
      }
    }

    update_statuslinef(LNG->SearchingMsg, n, AA->Msgn.Count(), marked);
    w_progress(MODE_UPDATE, C_INFOW, n, AA->Msgn.Count(), NULL);

    if(AA->LoadMsg(msg, AA->Msgn[n-1], CFG->dispmargin-(int)CFG->switches.get(disppagebar))) {

      bool success = srchmgr.search(msg, false, true);

      if(srchmgr.reverse ? not success : success) {
        AA->Mark.Add(msg->msgno);
        update_statuslinef(LNG->SearchingMsg, n, AA->Msgn.Count(), ++marked);
      }
    }
  }

  w_progress(MODE_QUIT, 0, 0, 0, NULL);

  ResetMsg(msg);
  throw_free(msg);
}


//  ------------------------------------------------------------------

static void recursive_mark(GMsg* msg, ulong msgno) {

  int i;
  gmsg_links templink;

  if(AA->Msgn.ToReln(msgno) and AA->LoadHdr(msg, msgno)) {

    templink = msg->link;

    if(templink.first())
      AA->Mark.Add(templink.first());

    for(i = 0; i < templink.list_max(); i++) {
      if(templink.list(i)) {
        AA->Mark.Add(templink.list(i));
      }
    }

    if(templink.first())
      recursive_mark(msg, templink.first());

    for(i = 0; i < templink.list_max(); i++) {
      if(templink.list(i)) {
        recursive_mark(msg, templink.list(i));
      }
    }
  }
}


//  ------------------------------------------------------------------

void MarkMsgs_Thread(GMsg* msg) {

  GMsg* tempmsg = (GMsg*)throw_calloc(1, sizeof(GMsg));
  tempmsg->msgno = msg->msgno;

  w_info(LNG->Wait);

  AA->Mark.Add(msg->msgno);

  ulong msgno = msg->link.to();
  while(AA->Msgn.ToReln(msgno)) {  // Search backwards
    AA->Mark.Add(msgno);

    if(not AA->LoadHdr(tempmsg, msgno))
      tempmsg->link.to_set(0);
    msgno = tempmsg->link.to();
  }

  recursive_mark(tempmsg, tempmsg->msgno);

  w_info(NULL);

  ResetMsg(tempmsg);
  throw_free(tempmsg);
}


//  ------------------------------------------------------------------

void MarkMsgs(GMsg* msg) {

  GFTRK("MarkMsgs");

  static INam markstring;

  GMenuMarkMsgs MenuMarkMsgs;
  int item = MenuMarkMsgs.Run();

  switch(item) {

    // ---------------------------------------------------------------
    case TAG_MARKUNMARK:
      MarkMsgs_Unmark();
      break;

    // ---------------------------------------------------------------
    case TAG_MARKTOGGLE:
      MarkMsgs_Toggle();
      break;

    // ---------------------------------------------------------------
    case TAG_MARKALLMSGS:
      MarkMsgs_All();
      break;

    // ---------------------------------------------------------------
    case TAG_MARKNEWMSGS:
      MarkMsgs_New();
      break;

    // ---------------------------------------------------------------
    case TAG_MARKOLDMSGS:
      MarkMsgs_Old();
      break;

    // ---------------------------------------------------------------
    case TAG_MARKRANGE:
      MarkMsgs_Range();
      break;

    // ---------------------------------------------------------------
    case TAG_MARKYOURMAIL:
      sprintf(markstring, "\"%s\"", AA->Username().name);
      // Drop through!

    // ---------------------------------------------------------------
    case TAG_MARKHEADER:
      // Drop through!

    // ---------------------------------------------------------------
    case TAG_MARKTXTHDR:
      MarkMsgs_Txt(item, markstring);
      break;

    // ---------------------------------------------------------------
    case TAG_MARKTHREAD:
      MarkMsgs_Thread(msg);
      break;
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void MarkingOptions() {

  if(AA->Msgn.Count())
    MarkMsgs(reader_msg);
}


//  ------------------------------------------------------------------

