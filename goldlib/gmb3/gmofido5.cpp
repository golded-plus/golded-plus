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
//  Fido/Opus/FTSC (*.MSG) type handling
//  ------------------------------------------------------------------

#include <gdbgtrk.h>
#include <gmemall.h>
#include <gstrall.h>
#include <gmoprot.h>
#include <gmofido.h>


//  ------------------------------------------------------------------

char* FidoArea::user_lookup(char* __lookfor) {

  Path userfile;
  strxcpy(userfile, AddPath(fidowide->squishuserpath, "USER.BBS"), sizeof(Path));
  wide->user->fh = ::sopen(userfile, O_RDONLY|O_BINARY, WideSharemode, S_STDRD);
  wide->user->findwild(__lookfor, __lookfor);
  ::close(wide->user->fh);

  if(wide->user->found)
    return __lookfor;
  else
    return NULL;
}


//  ------------------------------------------------------------------

int FidoArea::renumber() {

  GFTRK("FidoRenumber");

  // Save the current lastread
  save_lastread();

  // Rescan the area
  scan();

  // In echo or local, start with 2.MSG so we don't conflict
  // with highwater marks, unless there is already a 1.MSG
  uint _msgno1st = 1;
  if(not isnet() and (Msgn->at(0) != 1))
    _msgno1st++;

  // Renumber *.MSG files
  register uint _count = 0;
  register uint _msgno = _msgno1st;
  while(_count < Msgn->Count()) {

    // Only renumber a msg if different
    if(_msgno != Msgn->at(_count)) {

      Path _oldname, _newname;
      sprintf(_oldname, "%s%u.msg", real_path(), Msgn->at(_count));
      sprintf(_newname, "%s%u.msg", real_path(), _msgno);

      // Get the file attribute of the message
      struct stat st;
      stat(_oldname, &st);
      bool readonly = not (st.st_mode & S_IWUSR);
      if(readonly)
        chmod(_oldname, S_STDRW);

      // Show progress
      char buf[100];
      sprintf(buf,
        "%s: %u.msg -> %u.msg%s",
        "Renumbering",
        Msgn->at(_count),
        _msgno,
        readonly ? " (locked)" : ""
      );
      FidoRenumberProgress(buf);

      // Update the msg
      int _fh = test_open(_oldname, O_RDWR|O_BINARY, SH_DENYWR);
      if(_fh != -1) {

        // Read the header
        FidoHdr _hdr;
        read(_fh, &_hdr, sizeof(FidoHdr));

        // Update the replylinks
        if(_hdr.replyto or _hdr.reply1st) {
          _hdr.replyto  = (word)Msgn->ToReln(_hdr.replyto);
          _hdr.reply1st = (word)Msgn->ToReln(_hdr.reply1st);
          lseekset(_fh, 0);
          write(_fh, &_hdr, sizeof(FidoHdr));
        }
        ::close(_fh);

        // Rename the message file
        rename(_oldname, _newname);
        if(readonly)
          chmod(_oldname, S_STDRD);
      }
    }
    _msgno++;
    _count++;
  }

  // Renumber the personal mail marks
  for(_count=0; _count<PMrk->Tags(); _count++)
    PMrk->at(_count) = Msgn->ToReln(PMrk->at(_count)) + _msgno1st - 1;

  // Renumber the index
  for(_count=0; _count<Msgn->Count(); _count++)
    Msgn->at(_count) = _count + _msgno1st;

  GFTRK(NULL);

  return true;
}


//  ------------------------------------------------------------------
//  Hexdump the current message header

Line* FidoArea::make_dump_msg(Line*& lin, gmsg* msg, char* lng_head) {

  GFTRK("FidoMakeDump");

  FidoHdr _hdr;
  load_message(GMSG_HDRTXT, msg, _hdr);

  char buf[100];
  Line* line = lin =
  AddLine (NULL, "Hexdump of Fido/Opus-style message header and text");
  AddLineF(line, "------------------------------------------------------------------------------");
  line = AddLine(line, "");
  AddLineF(line, "File     : %s%u.msg", real_path(), msg->msgno);
  AddLineF(line, "From     : %-35.35s", _hdr.by);
  AddLineF(line, "To       : %-35.35s", _hdr.to);
  AddLineF(line, "Subject  : %-72.72s", _hdr.re);
  AddLineF(line, "DateTime : %-20.20s", _hdr.datetime);
  if(isopus())
    AddLineF(line, "OrigAddr : %u/%u", _hdr.orignet, _hdr.orignode);
  else
    AddLineF(line, "OrigAddr : %u:%u/%u.%u", _hdr.ftsc.origzone, _hdr.orignet, _hdr.orignode, _hdr.ftsc.origpoint);
  if(isopus())
    AddLineF(line, "DestAddr : %u/%u", _hdr.destnet, _hdr.destnode);
  else
    AddLineF(line, "DestAddr : %u:%u/%u.%u", _hdr.ftsc.destzone, _hdr.destnet, _hdr.destnode, _hdr.ftsc.destpoint);
  AddLineF(line, "Reply    : %u  See : %u", _hdr.replyto, _hdr.reply1st);
  AddLineF(line, "TimesRead: %u  Cost: %u", _hdr.timesread, _hdr.cost);
  AddLineF(line, "Attr     : %u (%04X)", _hdr.attr, _hdr.attr);
  if(isopus()) {
    AddLineF(line, "Written  : %s (%08Xh)", FTimeToStr(buf, _hdr.opus.written), *(uint*)&_hdr.opus.written);
    AddLineF(line, "Arrived  : %s (%08Xh)", FTimeToStr(buf, _hdr.opus.arrived), *(uint*)&_hdr.opus.arrived);
  }
  AddLineF(line, "UserRecno: %u (%s)", wide->userno, WideUsername[0]);
  line = AddLine(line, "");
  AddLineF(line, lng_head);
  line = AddLine(line, "");

  int _count;
  char* _ptr = (char*)&_hdr;
  for(_count=0; _count<190; _ptr+=16,_count+=16) {
    sprintf(buf, "%04X   ", _count);
    HexDump16(buf+7, _ptr, 16, HEX_DUMP2);
    line = AddLine(line, buf);
  }
  sprintf(buf, "%04X   ", _count);
  HexDump16(buf+7, _ptr, 14, HEX_DUMP2);
  line = AddLine(line, buf);

  GFTRK(NULL);

  return line;
}


//  ------------------------------------------------------------------

