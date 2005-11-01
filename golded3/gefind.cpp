
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
//  Search functions.
//  ------------------------------------------------------------------

#include <golded.h>
#include <gesrch.h>

#if defined(__USE_ALLOCA__)
#include <malloc.h>
#endif


//  ------------------------------------------------------------------

void SearchExit() { }


//  ------------------------------------------------------------------

void FindAll(GMsg* msg, int& topline, int& keyok) {

  if(AA->Msgn.Count()) {
    topline = 0;
    AA->attr().hex0();

    if (AA->get_findfirst())
    {
      strcpy(CFG->searchfor, AA->Searchfor());
      if(edit_string(CFG->searchfor, sizeof(INam), LNG->HeaderText, H_FindString)) {
        AA->SetSearchfor(CFG->searchfor);
        if(FindString(msg, AA->Searchfor(), GFIND_HDRTXT))
          keyok = true;
        AA->set_findfirst(false);
      }
    }
    else if(FindString(msg, AA->Searchfor(), GFIND_HDRTXT))
      keyok = true;
  }
}


//  ------------------------------------------------------------------

void FindHdr(GMsg* msg, int& topline, int& keyok) {

  if(AA->Msgn.Count()) {
    topline = 0;
    AA->attr().hex0();

    if (AA->get_findfirst())
    {
      strcpy(CFG->searchfor, AA->Searchfor());
      if(edit_string(CFG->searchfor, sizeof(INam), LNG->HeaderOnly, H_FindString)) {
        AA->SetSearchfor(CFG->searchfor);
        if(FindString(msg, AA->Searchfor(), GFIND_HDR))
          keyok = true;
        AA->set_findfirst(false);
      }
    }
    else if(FindString(msg, AA->Searchfor(), GFIND_HDR))
      keyok = true;
  }
}


//  ------------------------------------------------------------------

bool SearchHighlight(const Line*, int, int, int) {

  return false;
}


//  ------------------------------------------------------------------

bool FindString(GMsg* msg, const char* prompt, int what) {

  GFTRK("FindString");

  static uint32_t lastfound = 0;
  bool result = false;
  bool breakloop = true;

  golded_search_manager srchmgr;
  srchmgr.prepare_from_string(prompt, what);

  uint last = AA->lastread();

  lastfound = AA->Msgn.CvtReln(AA->lastread());
  w_progress(MODE_NEW, C_INFOW, AA->lastread(), AA->Msgn.Count(), LNG->AdvancedSearch);

  uint32_t tmpmsgno;

  int margin = CFG->dispmargin-(int)CFG->switches.get(disppagebar);

  do {

    update_statuslinef(LNG->ReadingMsg, AA->lastread(), AA->Msgn.Count());
    w_progress(MODE_UPDATE, C_INFOW, AA->lastread(), AA->Msgn.Count(), LNG->AdvancedSearch);

    bool success = false;
    if(AA->LoadMsg(msg, AA->Msgn.CvtReln(AA->lastread()), margin))
    {
      if (CFG->latin2local)
      {
        Latin2Local(msg->by);
        Latin2Local(msg->to);

        if (!msg->attr.frq() && !msg->attr.att() && !msg->attr.urq())
          Latin2Local(msg->re);

        for (Line *ln = msg->lin; ln; ln = ln->next)
        {
#if defined(__USE_ALLOCA__)
          char *temp = (char *)alloca(ln->txt.length()+1);
#else
          __extension__ char temp[ln->txt.length()+1];
#endif
          strcpy(temp, ln->txt.c_str());
          Latin2Local(temp);
          ln->txt = temp;
        }
      }

      // If hit, search again current mail without shortcircuit evaluation
      success = srchmgr.search(msg, false, true);
      if(success)
        srchmgr.search(msg, false, false);

      if(((srchmgr.reverse ? !success : success) && (lastfound != msg->msgno))
         || (success && AA->get_findfirst()))
      {
        bool istwitto, istwitsubj;
        if(MsgIsTwit(msg, istwitto, istwitsubj) != TWIT_SKIP) {
          HandleGEvent(EVTT_SEARCHSUCCESS);
          lastfound = msg->msgno;
          AA->set_lastread(AA->Msgn.ToReln(msg->msgno));
          result = true;
          breakloop = false;
          break;
        }
      }
    }

    if(kbxhit() and (kbxget() == Key_Esc)) {
      breakloop = false;
      break;
    }

    tmpmsgno = msg->msgno;
    switch(srchmgr.direction) {
      case DIR_PREV:
        if(AA->Msgn.ToReln(msg->msgno) > 1)
          msg->msgno = AA->Msgn.CvtReln(AA->Msgn.ToReln(msg->msgno)-1);
        else
          msg->msgno = 0;
        break;
      case DIR_NEXT:
        if(AA->Msgn.ToReln(msg->msgno) < AA->Msgn.Count())
          msg->msgno = AA->Msgn.CvtReln(AA->Msgn.ToReln(msg->msgno)+1);
        else
          msg->msgno = 0;
        break;
    }
    if(msg->msgno != 0)
      AA->set_lastread(AA->Msgn.ToReln(msg->msgno));

  } while((msg->msgno != 0) and (msg->msgno != tmpmsgno));

  if(not result) {
    HandleGEvent(EVTT_SEARCHFAILED);
    AA->set_lastread(last);
  }

  w_progress(MODE_QUIT, 0, 0, 0, NULL);

  if(breakloop)
  HandleGEvent(EVTT_BREAKLOOP);

  if(not result) {
    w_info(LNG->NoMoreMatches);
    waitkeyt(5000);
    w_info(NULL);
  }

  GFTRK(NULL);
  return result;
}


//  ------------------------------------------------------------------

