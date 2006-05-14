//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
//  ------------------------------------------------------------------
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Library General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Library General Public License for more details.
//
//  You should have received a copy of the GNU Library General Public
//  License along with this program; if not, write to the Free
//  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
//  MA 02111-1307, USA
//  ------------------------------------------------------------------
//  $Id$
//  ------------------------------------------------------------------
//  Ezycom msgbase handling
//  ------------------------------------------------------------------

#include <gdbgtrk.h>
#include <gmemall.h>
#include <gutlmisc.h>
#include <gmoezyc.h>


//  ------------------------------------------------------------------

char* EzycomArea::user_lookup(char* __lookfor)
{
  wide->user->gufh = ::sopen(AddPath(wide->userbasepath, "USERS.BBS"), O_RDWR|O_BINARY, WideSharemode, S_STDRW);
  if (wide->user->gufh)
  {
    wide->user->findwild(__lookfor, __lookfor);
    ::close(wide->user->gufh);
  }

  if(wide->user->found)
    return __lookfor;
  else
    return NULL;
}


//  ------------------------------------------------------------------

int EzycomArea::renumber() {

  return false;
}


//  ------------------------------------------------------------------

Line* EzycomArea::make_dump_msg(Line*& lin, gmsg* msg, char* lng_head) {

  GFTRK("EzycomMakeDump");

  EzycHdr _hdr;
  load_message(GMSG_HDRTXT, msg, _hdr);

  char buf[100];
  Line* line = lin =
  AddLine (NULL, "Hexdump of Ezycom-style message header and text");
  AddLineF(line, "------------------------------------------------------------------------------");
  line = AddLine(line, "");
  AddLineF(line, "Msgbase  : %s", wide->msgbasepath);
  AddLineF(line, "BoardNo  : %u", board());
  AddLineF(line, "From     : %-35.35s", msg->by);
  AddLineF(line, "To       : %-35.35s", msg->to);
  AddLineF(line, "Subject  : %-72.72s", msg->re);
  AddLineF(line, "Orig     : %u:%u/%u.%u", _hdr.orignet.zone, _hdr.orignet.net, _hdr.orignet.node, _hdr.orignet.point);
  AddLineF(line, "Dest     : %u:%u/%u.%u", _hdr.destnet.zone, _hdr.destnet.net, _hdr.destnet.node, _hdr.destnet.point);
  AddLineF(line, "Reply    : %u  See : %u", _hdr.replyto, _hdr.reply1st);
  AddLineF(line, "Cost     : %u", _hdr.cost);
  AddLineF(line, "MsgAttr  : %02Xh (%sb)", _hdr.msgattr, ltob(buf, _hdr.msgattr, 8));
  AddLineF(line, "NetAttr  : %02Xh (%sb)", _hdr.netattr, ltob(buf, _hdr.netattr, 8));
  AddLineF(line, "ExtAttr  : %02Xh (%sb)", _hdr.extattr, ltob(buf, _hdr.extattr, 8));
  AddLineF(line, "PostDate : %s (%08Xh)", FTimeToStr(buf, _hdr.posttimedate), *(dword*)&_hdr.posttimedate);
  AddLineF(line, "RecvDate : %s (%08Xh)", FTimeToStr(buf, _hdr.recvtimedate), *(dword*)&_hdr.recvtimedate);
  AddLineF(line, "StartPos : %u", _hdr.startposition);
  AddLineF(line, "MsgLength: %u", _hdr.messagelength);
  AddLineF(line, "UserRecno: %u (%s)", wide->userno, WideUsername[0]);
  line = AddLine(line, "");
  AddLineF(line, lng_head);
  line = AddLine(line, "");

  uint _count = 0;
  char* _ptr = (char*)&_hdr;
  while(_count < sizeof(EzycHdr)) {
    sprintf(buf, "%04X   ", _count);
    HexDump16(buf+7, _ptr, 16, HEX_DUMP2);
    line = AddLine(line, buf);
    _count += 16;
    _ptr += 16;
  }
  sprintf(buf, "%04X   ", _count);
  HexDump16(buf+7, _ptr, sizeof(EzycHdr)%16, HEX_DUMP2);
  line = AddLine(line, buf);

  GFTRK(0);

  return line;
}


//  ------------------------------------------------------------------
