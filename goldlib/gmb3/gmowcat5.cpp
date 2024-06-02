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
//  WildCat! 4.x messagebase engine.
//  ------------------------------------------------------------------
//  ------------------------------------------------------------------

#include <gdbgtrk.h>
#include <gmemall.h>
#include <gstrall.h>

#include <gmowcat.h>
//  ------------------------------------------------------------------
char * WCatArea::user_lookup(char * __lookfor)
{
    NW(__lookfor);
    return NULL;
}

//  ------------------------------------------------------------------
int WCatArea::renumber()
{
    return false;
}

//  ------------------------------------------------------------------
//  Hexdump the current message header
Line * WCatArea::make_dump_msg(Line *& lin, gmsg * msg, char * lng_head)
{
    GFTRK("WCatMakeDump");
    WCatHdr _hdr;
    load_message(GMSG_HDRTXT, msg, _hdr);
    char buf[100];
    Line * line = lin = AddLine(NULL, "Hexdump of WildCat! message header and text");
    AddLineF(line,
             "------------------------------------------------------------------------------");
    line = AddLine(line, "");
    AddLineF(line, "Path       : %s", real_path());
    AddLineF(line, "MagicNumber: %08Xh", _hdr.magicnumber);
    AddLineF(line, "MsgNumber  : %u", _hdr.msgno);
    AddLineF(line, "Orig       : %s", STRNP2C(_hdr.from));
    AddLineF(line, "OrigTitle  : %s", STRNP2C(_hdr.fromtitle));
    AddLineF(line, "OrigUserID : %i", _hdr.fromuserid);
    AddLineF(line, "Dest       : %s", STRNP2C(_hdr.to));
    AddLineF(line, "DestTitle  : %s", STRNP2C(_hdr.totitle));
    AddLineF(line, "DestUserID : %i", _hdr.touserid);
    AddLineF(line, "Subject    : %s", STRNP2C(_hdr.subject));
    AddLineF(line, "Network    : %s", STRNP2C(_hdr.network));
    struct tm _tm;
    ggmtime(&_tm, &msg->written);
    AddLineF(line,
             "MsgTime    : %s (%u, %i)",
             strftimei(buf,
                       100,
                       "%d %b %y  %H:%M:%S",
                       &_tm),
             _hdr.msgdate,
             _hdr.msgtime);
    ggmtime(&_tm, &msg->received);
    AddLineF(line,
             "ReadTime   : %s (%u, %i)",
             strftimei(buf,
                       100,
                       "%d %b %y  %H:%M:%S",
                       &_tm),
             _hdr.readdate,
             _hdr.readtime);
    AddLineF(line, "mFlags     : %u (%04Xh)", _hdr.mflags, _hdr.mflags);
    AddLineF(line, "Reference  : %u", _hdr.reference);
    AddLineF(line,
             "FidoFrom   : %u:%u/%u.%u",
             _hdr.origaddr.zone,
             _hdr.origaddr.net,
             _hdr.origaddr.node,
             _hdr.origaddr.point);
    AddLineF(line,
             "FidoTo     : %u:%u/%u.%u",
             _hdr.origaddr.zone,
             _hdr.destaddr.net,
             _hdr.destaddr.node,
             _hdr.destaddr.point);
    AddLineF(line, "MsgBytes   : %u", _hdr.msgbytes);
    AddLineF(line, "Int.Attach : %s", STRNP2C(_hdr.internalattach));
    AddLineF(line, "Ext.Attach : %s", STRNP2C(_hdr.externalattach));
    AddLineF(line, "PrevUnread : %u", _hdr.prevunread);
    AddLineF(line, "NextUnread : %u", _hdr.nextunread);
    AddLineF(line, "FidoFlags  : %u (%04Xh)", _hdr.fidoflags, _hdr.fidoflags);
    AddLineF(line, "Cost       : %i", _hdr.cost);
    AddLineF(line,
             "Reserved   : %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
             _hdr.reserved[0],
             _hdr.reserved[1],
             _hdr.reserved[2],
             _hdr.reserved[3],
             _hdr.reserved[4],
             _hdr.reserved[5],
             _hdr.reserved[6],
             _hdr.reserved[7],
             _hdr.reserved[8],
             _hdr.reserved[9],
             _hdr.reserved[10],
             _hdr.reserved[11],
             _hdr.reserved[12],
             _hdr.reserved[13],
             _hdr.reserved[14],
             _hdr.reserved[15],
             _hdr.reserved[16],
             _hdr.reserved[17],
             _hdr.reserved[18],
             _hdr.reserved[19]);
    line = AddLine(line, "");
    AddLineF(line, "UserRecno  : %u (%s)", wide->userno, WideUsername[0]);
    line = AddLine(line, "");
    AddLine(line, lng_head);
    line = AddLine(line, "");
    int _count;
    char * _ptr = (char *)&_hdr;

    for(_count = 0; _count < sizeof(WCatHdr); _ptr += 16, _count += 16)
    {
        sprintf(buf, "%04X   ", _count);
        HexDump16(buf + 7, _ptr, 16, HEX_DUMP2);
        line = AddLine(line, buf);
    }
    sprintf(buf, "%04X   ", _count);
    HexDump16(buf + 7, _ptr, 14, HEX_DUMP2);
    line = AddLine(line, buf);
    GFTRK(0);
    return line;
} // WCatArea::make_dump_msg

//  ------------------------------------------------------------------
