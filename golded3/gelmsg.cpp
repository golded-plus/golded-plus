
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
//  Load message or header. Save header.
//  ------------------------------------------------------------------

#include <golded.h>


//  ------------------------------------------------------------------

void ResetMsg(GMsg* msg) {

  throw_xfree(msg->references);
  throw_xfree(msg->inreplyto);
  throw_xfree(msg->messageid);
  throw_xfree(msg->txt);
  throw_xfree(msg->line);
  Line* line = msg->lin;
  while(line) {
    Line* nextline = line->next;
    throw_xdelete(line);
    line = nextline;
  }
  memset(msg, 0, sizeof(GMsg));
}


//  ------------------------------------------------------------------

int Area::LoadHdr(GMsg* msg, ulong msgno, bool enable_recode) {

  ResetMsg(msg);
  msg->msgno = msgno;
  int retval = area->load_hdr(msg);

  // Don't translate charsets if we don't know charset
  // Currently, it only mime-decodes, so it's okay.
  if(retval and enable_recode) {
    // Use default translation by default
    int table = LoadCharset(NULL, NULL, 1);
    if((table == -1) or not CFG->ignorecharset)
      msg->charsetlevel = LoadCharset(AA->Xlatimport(), CFG->xlatlocalset);
    else
      msg->charsetlevel = LoadCharset(CFG->xlatcharset[table].imp, CFG->xlatcharset[table].exp);

    // Charset translate header fields
    strxmimecpy(msg->realby, msg->realby, msg->charsetlevel, sizeof(INam));
    strxmimecpy(msg->realto, msg->realto, msg->charsetlevel, sizeof(INam));
    strxmimecpy(msg->by    , msg->by    , msg->charsetlevel, sizeof(INam));
    strxmimecpy(msg->to    , msg->to    , msg->charsetlevel, sizeof(INam));

    if(not (msg->attr.frq() or msg->attr.att() or msg->attr.urq()))
      strxmimecpy(msg->re  , msg->re    , msg->charsetlevel, sizeof(ISub), true);
  }
  return retval;
}


//  ------------------------------------------------------------------

int Area::LoadMsg(GMsg* msg, ulong msgno, int margin, int mode) {

  ResetMsg(msg);
  msg->msgno = msgno;
  if(msgno and area->load_msg(msg)) {
    if(mode & (GMSG_COPY|GMSG_MOVE)) {
      if(not ((mode & GMSG_MOVE) and (mode & GMSG_UNS_NOT_RCV)))
        return true;
      if(not (msg->attr.uns() and not msg->attr.rcv()))
        return true;
    }
    else {
      // We can't rely on the destination address unless we are in netmail
      if(not AA->isnet()) {
        msg->dest.reset_fast();
        msg->odest.reset_fast();
      }
    }
    msg->attr.tou0();
    msg->TextToLines(margin);
    return true;
  }
  return false;
}


//  ------------------------------------------------------------------

void Area::SaveHdr(int mode, GMsg* msg) {

  // Translate softcr to configured char
  if(EDIT->SoftCrXlat()) {
    strchg(msg->by, SOFTCR, EDIT->SoftCrXlat());
    strchg(msg->to, SOFTCR, EDIT->SoftCrXlat());
    strchg(msg->realby, SOFTCR, EDIT->SoftCrXlat());
    strchg(msg->realto, SOFTCR, EDIT->SoftCrXlat());
    if(not (msg->attr.frq() or msg->attr.att() or msg->attr.urq()))
      strchg(msg->re, SOFTCR, EDIT->SoftCrXlat());
  }
  area->save_hdr(mode, msg);
  UpdateAreadata();
}


//  ------------------------------------------------------------------

