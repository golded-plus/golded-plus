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
//  PCBoard msgbase handling.
//  ------------------------------------------------------------------


//  ------------------------------------------------------------------

#include <gmemdbg.h>
#include <gdbgtrk.h>
#include <gmemall.h>
#include <gutlmisc.h>

#include <gmopcbd.h>

              
// ------------------------------------------------------------------

char* PcbArea::user_lookup(char* lookfor) {

  NW(lookfor);
  return NULL;
}


// ------------------------------------------------------------------

int PcbArea::renumber() {

  return false;
}


// ------------------------------------------------------------------

Line* PcbArea::make_dump_msg(Line*& lin, gmsg* __msg, char* lng_head) {

  GFTRK("PcbMakeDump");

  // Read lastread records
  byte _mailwaiting = 0;
  dword* dumplr = (dword*)throw_calloc(wide->numareas, sizeof(dword));
  lseekset(wide->fhusr, (long)wide->userno*(long)sizeof(PcbUsers));
  read(wide->fhusr, &wide->usersrec, sizeof(PcbUsers));
  int _maxlr = MinV(wide->numareas, 40);
  int n;
  for(n=0; n<_maxlr; n++)
    dumplr[n] = wide->usersrec.lastmsgread[n];
  long _offset = (wide->usersrec.usersinfrec-1)*wide->usershdr.totalrecsize;
  _offset +=     wide->usershdrsize;
  _offset +=     wide->usershdr.sizeofrec;
  long _offset2 = _offset;
  _offset2 += board() / 8;
  lseekset(wide->fhinf, _offset2);
  read(wide->fhinf, &_mailwaiting, 1);
  byte bitmask = (byte)(1 << (board() % 8));
  _mailwaiting &= bitmask;
  _offset += 2 * wide->confbytelen;
  _offset += 3 * wide->extconflen;
  if(wide->extconflen) {
    lseekset(wide->fhinf, _offset);
    read(wide->fhinf, dumplr+40, (wide->numareas-40)*sizeof(long));
  }

  // Read index record for msg
  PcbIdx _idx;
  lseekset(data->fhidx, (__msg->msgno-data->base.lowmsgno)*(long)sizeof(PcbIdx));
  read(data->fhidx, &_idx, sizeof(PcbIdx));

  // Read message header
  PcbHdr _hdr;
  lseekset(data->fhmsg, AbsV(_idx.offset));
  read(data->fhmsg, &_hdr, sizeof(PcbHdr));
  
  // Read the message text
  uint _msgsize = (_hdr.blocks-1)*128;
  __msg->txt = (char*)throw_realloc(__msg->txt, _msgsize+256);
  memset(__msg->txt, 0, _msgsize+256);
  read(data->fhmsg, __msg->txt, _msgsize);

  // Convert Julian date in the index
  unsigned _year, _month, _day;
  JDN2YMD(_idx.date, &_year, &_month, &_day);

  Line* line = lin =
  AddLine (NULL, "Hexdump of PCBoard message header and text");
  AddLineF(line, "------------------------------------------------------------------------------");
  line = AddLine(line, "");
  AddLineF(line, "Msgbase   : %s", path());
  AddLineF(line, "BoardNo   : %u", board());
  AddLineF(line, "MsgOffset : %li", _idx.offset);
  AddLineF(line, "Status    : \'%c\'", _hdr.status);
  AddLineF(line, "Msgno     : %li  (%08lXh)", B2L(_hdr.msgno), _hdr.msgno);
  AddLineF(line, "Refno     : %li  (%08lXh)", B2L(_hdr.refno), _hdr.refno);
  AddLineF(line, "Blocks    : %i", _hdr.blocks);
  AddLineF(line, "Date/Time : %8.8s %5.5s", _hdr.date, _hdr.time);
  AddLineF(line, "IdxDate   : %02u-%02u-%02u (%u)", _month, _day, _year-1900, _idx.date);
  AddLineF(line, "DestName  : %-25.25s", _hdr.destname);
  AddLineF(line, "ReplyDate : %li", B2L(_hdr.replydate));
  AddLineF(line, "ReplyTime : %5.5s", _hdr.replytime);
  AddLineF(line, "HasReply  : \'%c\'", _hdr.hasreply);
  AddLineF(line, "OrigName  : %-25.25s", _hdr.origname);
  AddLineF(line, "Subject   : %-25.25s", _hdr.subject);
  AddLineF(line, "Password  : %-12.12s", _hdr.password);
  AddLineF(line, "Active    : %u%s", _hdr.activestatus, (_hdr.activestatus == 226) ? " (Deleted)" : "");
  AddLineF(line, "Echoed    : \'%c\'", _hdr.echoed);
  AddLineF(line, "Reserved  : %02Xh %02Xh %02Xh %02Xh ", _hdr.reserved[0], _hdr.reserved[1], _hdr.reserved[2], _hdr.reserved[3]);
  AddLineF(line, "ExtHdrFlg : %02Xh", _hdr.exthdrflags);
  AddLineF(line, "Reserved2 : %02Xh", _hdr.reserved2);
  line = AddLine(line, "");
  AddLineF(line, "Message Base Header:");
  line = AddLine(line, "");
  AddLineF(line, "HighMsgno : %li", data->base.highmsgno);
  AddLineF(line, "LowMsgno  : %li", data->base.lowmsgno);
  AddLineF(line, "Active    : %li", data->base.active);
  AddLineF(line, "Locked    : %6.6s", data->base.locked);

  line = AddLine(line, "");
  AddLineF(line, "Mail Waiting: %s", _mailwaiting ? "Yes" : "No");
  line = AddLine(line, "");
  AddLineF(line, "Lastreads from the USERS file:");
  line = AddLine(line, "");
  for(n=0; n<_maxlr; n++) {
    dword tmplr = B2L(dumplr[n]);
    AddLineF(line, "Board %4u : %10lu / %08lXh / %08lXh", n, tmplr, tmplr, dumplr[n]);
  }

  line = AddLine(line, "");
  AddLineF(line, "Lastreads from the USERS.INF file:");
  line = AddLine(line, "");
  for(; n<wide->numareas; n++)
    AddLineF(line, "Board %4u : %10lu / %08lXh", n, dumplr[n], dumplr[n]);

  line = AddLine(line, "");
  AddLineF(line, lng_head);
  line = AddLine(line, "");

  char _buf[256];
  char* _ptr = (char*)&_hdr;
  for(n=0; n < sizeof(PcbHdr); _ptr+=16,n+=16) {
    sprintf(_buf, "%04X   ", n);
    HexDump16(_buf+7, _ptr, 16, HEX_DUMP2);
    line = AddLine(line, _buf);
  }

  throw_free(dumplr);

  GFTRK(NULL);

  return line;
}


// ------------------------------------------------------------------

