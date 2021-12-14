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
//  Squish msgbase handling and Maximus user functions.
//  ------------------------------------------------------------------

#include <gdbgtrk.h>
#include <gmemall.h>
#include <gutlmisc.h>
#include <gstrall.h>
#include <gmosqsh.h>


//  ------------------------------------------------------------------

char* SquishArea::user_lookup(char* __lookfor)
{
    Path userfile;
    strxcpy(userfile, AddPath(wide->userpath, "USER.BBS"), sizeof(Path));
    wide->user->gufh = ::sopen(userfile, O_RDONLY|O_BINARY, WideSharemode, S_STDRD);
    wide->user->findwild(__lookfor, __lookfor);
    ::close(wide->user->gufh);

    if(wide->user->found)
        return __lookfor;
    else
        return NULL;
}


//  ------------------------------------------------------------------

int SquishArea::renumber()
{

    return false;
}


//  ------------------------------------------------------------------

Line* SquishArea::make_dump_msg(Line*& lin, gmsg* msg, char* lng_head)
{

    GFTRK("SquishMakeDump");

    // Load the base record
    SqshBase _base;
    lseekset(data->fhsqd, 0);
    read(data->fhsqd, &_base, sizeof(SqshBase));

    // Load the index record
    SqshIdx _idx;
    uint _reln = Msgn->ToReln(msg->msgno);
    lseekset(data->fhsqi, _reln-1, sizeof(SqshIdx));
    read(data->fhsqi, &_idx, sizeof(SqshIdx));

    // Load the message frame
    SqshFrm _frm;
    dword _frame = _idx.offset;
    lseekset(data->fhsqd, _frame);
    read(data->fhsqd, &_frm, sizeof(SqshFrm));

    // Load the header and message text
    SqshHdr _hdr;
    load_message(GMSG_HDRTXT, msg, _hdr);

    char buf[100];
    Line* line = lin =
                     AddLine (NULL, "Hexdump of Squish-style message header and text");
    AddLineF(line, "------------------------------------------------------------------------------");
    line = AddLine(line, "");
    AddLineF(line, "Msgbase        : %s", real_path());
    AddLineF(line, "From           : %-36.36s", _hdr.from);
    AddLineF(line, "To             : %-36.36s", _hdr.to);
    AddLineF(line, "Subject        : %-72.72s", _hdr.subj);
    AddLineF(line, "DateTime       : %-20.20s", _hdr.ftsc_date);
    AddLineF(line, "OrigAddr       : %u:%u/%u.%u", _hdr.orig.zone, _hdr.orig.net, _hdr.orig.node, _hdr.orig.point);
    AddLineF(line, "DestAddr       : %u:%u/%u.%u", _hdr.dest.zone, _hdr.dest.net, _hdr.dest.node, _hdr.dest.point);
    AddLineF(line, "Umsgid         : %u", _hdr.umsgid);
    AddLineF(line, "Reply          : %u", _hdr.replyto);
    AddLineF(line, "See            : %u, %u, %u, %u, %u, %u, %u, %u, %u",
             _hdr.replies[0], _hdr.replies[1], _hdr.replies[2],
             _hdr.replies[3], _hdr.replies[4], _hdr.replies[5],
             _hdr.replies[6], _hdr.replies[7], _hdr.replies[8]
            );
    AddLineF(line, "Attr           : %08Xh (%sb)", _hdr.attr, ltob(buf,_hdr.attr,0));
    AddLineF(line, "DateWritten    : %s (%08Xh)", FTimeToStr(buf, _hdr.date_written), *(dword*)&_hdr.date_written);
    AddLineF(line, "DateArrived    : %s (%08Xh)", FTimeToStr(buf, _hdr.date_arrived), *(dword*)&_hdr.date_arrived);
    AddLineF(line, "UTC-Offset     : %u", _hdr.utc_offset);
    AddLineF(line, "UserRecno      : %u (%s)", wide->userno, WideUsername[0]);
    line = AddLine(line, "");
    AddLineF(line, "Message Base Record:");
    line = AddLine(line, "");
    AddLineF(line, "TotalMsgs      : %u", _base.totalmsgs);
    AddLineF(line, "HighestMsg     : %u", _base.highestmsg);
    AddLineF(line, "NextMsgno      : %u", _base.nextmsgno);
    AddLineF(line, "HighWaterMark  : %u", _base.highwatermark);
    AddLineF(line, "FirstFrame     : %08Xh (%u)", _base.firstframe, _base.firstframe);
    AddLineF(line, "LastFrame      : %08Xh (%u)", _base.lastframe, _base.lastframe);
    AddLineF(line, "FirstFreeFrame : %08Xh (%u)", _base.firstfreeframe, _base.firstfreeframe);
    AddLineF(line, "LastFreeFrame  : %08Xh (%u)", _base.lastfreeframe, _base.lastfreeframe);
    AddLineF(line, "EndFrame       : %08Xh (%u)", _base.endframe, _base.endframe);
    AddLineF(line, "Max/Skip/Days  : %u  %u  %u", _base.maxmsgs, _base.protmsgs, _base.daystokeep);
    line = AddLine(line, "");
    AddLineF(line, "Message Index Record:");
    line = AddLine(line, "");
    AddLineF(line, "FrameOffset    : %08Xh (%u)", _idx.offset, _idx.offset);
    AddLineF(line, "MessageNumber  : %08Xh (%u)", _idx.msgno, _idx.msgno);
    AddLineF(line, "HashValue      : %08Xh (%u)", _idx.hash, _idx.hash);
    line = AddLine(line, "");
    AddLineF(line, "Message Frame Record:");
    line = AddLine(line, "");
    AddLineF(line, "Frame-ID       : %08Xh%s", _frm.id, (_frm.id != SQFRAMEID) ? " (error, should be AFAE4453h!)" : "");
    AddLineF(line, "ThisFrame      : %08Xh (%u)", _frame, _frame);
    AddLineF(line, "PrevFrame      : %08Xh (%u)", _frm.prev, _frm.prev);
    AddLineF(line, "NextFrame      : %08Xh (%u)", _frm.next, _frm.next);
    AddLineF(line, "FrameLength    : %u", _frm.length);
    AddLineF(line, "TotalLength    : %u", _frm.totsize);
    AddLineF(line, "CtrlLength     : %u", _frm.ctlsize);
    line = AddLine(line, "");
    AddLineF(line, lng_head);
    line = AddLine(line, "");

    int _count = 0;
    char* _ptr = (char*)&_hdr;
    while(_count < sizeof(SqshHdr))
    {
        sprintf(buf, "%04X   ", _count);
        HexDump16(buf+7, _ptr, 16, HEX_DUMP2);
        line = AddLine(line, buf);
        _count += 16;
        _ptr += 16;
    }
    sprintf(buf, "%04X   ", _count);
    HexDump16(buf+7, _ptr, sizeof(SqshHdr)%16, HEX_DUMP2);
    line = AddLine(line, buf);

    GFTRK(0);

    return line;
}


//  ------------------------------------------------------------------
