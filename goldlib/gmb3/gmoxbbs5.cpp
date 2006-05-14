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
//  AdeptXBBS messagebase engine.
//  ------------------------------------------------------------------

#include <gdbgtrk.h>
#include <gmemall.h>
#include <gmoxbbs.h>


//  ------------------------------------------------------------------

char* XbbsArea::user_lookup(char* __lookfor)
{
  wide->user->gufh = ::sopen(AddPath(wide->path, "Users"), O_RDWR|O_BINARY, WideSharemode, S_STDRD);
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

int XbbsArea::renumber() {

  return false;
}


//  ------------------------------------------------------------------
//  Hexdump the current message header

Line* XbbsArea::make_dump_msg(Line*& lin, gmsg* msg, char* lng_head) {

  GFTRK("XbbsMakeDump");

  XbbsHdr hdr;
  load_message(GMSG_HDRTXT, msg, hdr);
  XbbsIdx* idx = data->idx + (msg->msgno - 1);

  char buf[100];
  Line* line = lin =
  AddLine (NULL, "Hexdump of AdeptXBBS message header and text");
  AddLineF(line, "------------------------------------------------------------------------------");
  line = AddLine(line, "");
  AddLineF(line, "Path       : %s", real_path());
  AddLineF(line, "UserRecno  : %u (%s)", wide->userno, WideUsername[0]);
  line = AddLine(line, "");
  AddLineF(line, "Header Record:");
  line = AddLine(line, "");
  AddLineF(line, "Version    : %u.%u", hdr.majorversion, hdr.minorversion);
  AddLineF(line, "StructLen  : %u", hdr.structlen);
  AddLineF(line, "From       : %s", hdr.from);
  AddLineF(line, "To         : %s", hdr.to);
  AddLineF(line, "Subj       : %s", hdr.subj);
  AddLineF(line, "Date       : %s", hdr.date);
  AddLineF(line, "InDate     : %u-%u-%u", hdr.indate[0]+1989, hdr.indate[1], hdr.indate[2]);
  AddLineF(line, "MsgNum     : %u", hdr.msgnum);
  AddLineF(line, "TimesRead  : %u", hdr.timesread);
  AddLineF(line, "TimeRecv   : %s (%08Xh)", TimeToStr(buf, hdr.timerecv), (int32_t)hdr.timerecv);
  AddLineF(line, "Length     : %u", hdr.length);
  AddLineF(line, "Start      : %u", hdr.start);
  AddLineF(line, "Extra1,2,3 : %u, %u, %u", hdr.extra1, hdr.extra2, hdr.extra3);
  AddLineF(line, "OrigAddr   : %u:%u/%u.%u", hdr.origaddr.zone, hdr.origaddr.net, hdr.origaddr.node, hdr.origaddr.point);
  AddLineF(line, "DestAddr   : %u:%u/%u.%u", hdr.origaddr.zone, hdr.destaddr.net, hdr.destaddr.node, hdr.destaddr.point);
  AddLineF(line, "Cost       : %u", hdr.cost);
  AddLineF(line, "FFlags     : %u (%04Xh)", hdr.fflags, hdr.fflags);
  AddLineF(line, "XFlags     : %u (%04Xh)", hdr.xflags, hdr.xflags);
  AddLineF(line, "IFlags     : %u (%08Xh)", hdr.iflags, hdr.iflags);
  AddLineF(line, "OFlags     : %u (%08Xh)", hdr.oflags, hdr.oflags);
  line = AddLine(line, "");
  AddLineF(line, "Index Record:");
  line = AddLine(line, "");
  AddLineF(line, "CheckSumTo      : %04Xh (%u)", idx->to,   idx->to);
  AddLineF(line, "CheckSumFrom    : %04Xh (%u)", idx->from, idx->from);
  AddLineF(line, "CheckSumSubj    : %04Xh (%u)", idx->subj, idx->subj);
  AddLineF(line, "MsgidCrc        : %08Xh (%u)", idx->msgidcrc,      idx->msgidcrc);
  AddLineF(line, "MsgidSerialNo   : %08Xh (%u)", idx->msgidserialno, idx->msgidserialno);
  AddLineF(line, "ReplyidCrc      : %08Xh (%u)", idx->replycrc,      idx->replycrc);
  AddLineF(line, "ReplyidSerialNo : %08Xh (%u)", idx->replyserialno, idx->replyserialno);
  line = AddLine(line, "");
  AddLineF(line, lng_head);
  line = AddLine(line, "");

  int _count;
  char* _ptr = (char*)&hdr;
  for(_count=0; _count<sizeof(XbbsHdr); _ptr+=16,_count+=16) {
    sprintf(buf, "%04X   ", _count);
    HexDump16(buf+7, _ptr, 16, HEX_DUMP2);
    line = AddLine(line, buf);
  }
  sprintf(buf, "%04X   ", _count);
  HexDump16(buf+7, _ptr, 14, HEX_DUMP2);
  line = AddLine(line, buf);

  GFTRK(0);

  return line;
}


//  ------------------------------------------------------------------
