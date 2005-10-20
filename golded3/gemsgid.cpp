
//  ------------------------------------------------------------------
//  GoldED+
//  Copyright (C) 2003 Alexander Reznikov, 2:4600/220@fidonet,
//                                         <homebrewer@yandex.ru>
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
//  Generate MSGID.
//  ------------------------------------------------------------------

#include <golded.h>
#include <genmsgid.h>
//  ------------------------------------------------------------------

static uint32_t msgcount = 0;

//  ------------------------------------------------------------------

uint32_t getClassicMsgId() {

  return gtime(NULL)+(msgcount++);
}

//  ------------------------------------------------------------------

uint32_t getMsgId() {

  uint32_t id;
  char *err;

  if((CFG->seqmsgid == YES) or ((CFG->seqmsgid == MAYBE) and (*CFG->seqdir != NUL))) {
    id = GenMsgIdEx(CFG->seqdir, CFG->seqoutrun, getClassicMsgId, &err);
    if(err) {
      LOG.printf("* Warning, SeqMsgId failed: %s, fall to classic algorythm!", err);
    }
    return id;
  }
  return getClassicMsgId();
}

//  ------------------------------------------------------------------
