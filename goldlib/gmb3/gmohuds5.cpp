//  This may look like C code, but it is really -*- C++ -*-
//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999-2000 Alex. S. Aganichev
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
//  Hudson / Goldbase msgbase handling
//  ------------------------------------------------------------------
//  ------------------------------------------------------------------
template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t,
          bool __HUDSON> char * _HudsArea<msgn_t, rec_t, attr_t, board_t, last_t,
                                          __HUDSON>::user_lookup(char * __lookfor)
{
    wide->user->gufh = wide->fhusr.fh;
    wide->user->findwild(__lookfor, __lookfor);

    if(wide->user->found)
    {
        return __lookfor;
    }
    else
    {
        return NULL;
    }
}

//  ------------------------------------------------------------------
template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t,
          bool __HUDSON> int _HudsArea<msgn_t, rec_t, attr_t, board_t, last_t,
                                       __HUDSON>::renumber()
{
    return false;
}

//  ------------------------------------------------------------------
template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t,
          bool __HUDSON> Line * _HudsArea<msgn_t, rec_t, attr_t, board_t, last_t,
                                          __HUDSON>::make_dump_msg(Line * & lin,
                                                                   gmsg * msg,
                                                                   char * lng_head)
{
    GFTRK("HudsMakeDump");
    HudsHdr _hdr;
    load_message(GMSG_HDRTXT, msg, _hdr);
    strnp2c(_hdr.date, 9);
    strnp2c(_hdr.time, 5);
    char buf[100];
    Line * line = lin = AddLine(NULL,
                                __HUDSON ? "Hexdump of " HUDS_NAME " message header and text" : "Hexdump of " GOLD_NAME " message header and text");
    AddLineF(line,
             "------------------------------------------------------------------------------");
    line = AddLine(line, "");
    AddLineF(line, "Msgbase   : %s", wide->path);
    AddLineF(line, "BoardNo   : %u", board());
    AddLineF(line, "From      : %-35.35s", msg->by);
    AddLineF(line, "To        : %-35.35s", msg->to);
    AddLineF(line, "Subject   : %-72.72s", msg->re);
    AddLineF(line, "DateTime  : %8.8s %5.5s", _hdr.date, _hdr.time);
    AddLineF(line, "OrigAddr  : %u:%u/%u", _hdr.origzone, _hdr.orignet, _hdr.orignode);
    AddLineF(line, "DestAddr  : %u:%u/%u", _hdr.destzone, _hdr.destnet, _hdr.destnode);
    AddLineF(line, "Reply     : %u", (uint32_t)_hdr.replyto);
    AddLineF(line, "See       : %u", (uint32_t)_hdr.reply1st);
    AddLineF(line, "TimesRead : %u", _hdr.timesread);
    AddLineF(line, "Cost      : %u", _hdr.cost);
    AddLineF(line, "MsgAttr   : %02Xh (%sb)", _hdr.msgattr, ltob(buf, _hdr.msgattr, 8));
    AddLineF(line, "NetAttr   : %02Xh (%sb)", _hdr.netattr, ltob(buf, _hdr.netattr, 8));
    AddLineF(line, "Msgno     : %u", (int32_t)_hdr.msgno);
    AddLineF(line, "Board     : %u", _hdr.board);
    AddLineF(line, "StartRec  : %u", (int32_t)_hdr.startrec);
    AddLineF(line, "NumRecs   : %u", (int32_t)_hdr.numrecs);
    AddLineF(line, "UserRecno : %u (%s)", wide->userno, WideUsername[0]);
    line = AddLine(line, "");
    AddLineF(line, "Dump of msginfo%s:", __HUDSON ? HUDS_EXT : GOLD_EXT);
    line = AddLine(line, "");
    AddLineF(line,
             "Total Active  : %u (%Xh)",
             (uint32_t)wide->msginfo.total,
             (uint32_t)wide->msginfo.total);
    AddLineF(line,
             "Low/High Msgno: %u (%Xh)  %u (%Xh)",
             (uint32_t)wide->msginfo.low,
             (uint32_t)wide->msginfo.low,
             (uint32_t)wide->msginfo.high,
             (uint32_t)wide->msginfo.high);

    for(int brd = 0; brd < (__HUDSON ? HUDS_MAXBOARD : GOLD_MAXBOARD); brd += 10)
    {
        AddLineF(line,
                 "Board %03u-%03u : %5u %5u %5u %5u %5u %5u %5u %5u %5u %5u",
                 brd + 1,
                 brd + 10,
                 wide->msginfo.active[brd + 0],
                 wide->msginfo.active[brd + 1],
                 wide->msginfo.active[brd + 2],
                 wide->msginfo.active[brd + 3],
                 wide->msginfo.active[brd + 4],
                 wide->msginfo.active[brd + 5],
                 wide->msginfo.active[brd + 6],
                 wide->msginfo.active[brd + 7],
                 wide->msginfo.active[brd + 8],
                 wide->msginfo.active[brd + 9]);
    }
    line = AddLine(line, "");
    AddLineF(line, "Dump of lastread%s:", __HUDSON ? HUDS_EXT : GOLD_EXT);
    line = AddLine(line, "");

    for(int lrd = 0; lrd < (__HUDSON ? HUDS_MAXBOARD : GOLD_MAXBOARD); lrd += 5)
    {
        AddLineF(line,
                 "Board %03u-%03u : %10u %10u %10u %10u %10u",
                 lrd + 1,
                 lrd + 5,
                 (uint32_t)wide->lastrec[lrd + 0],
                 (uint32_t)wide->lastrec[lrd + 1],
                 (uint32_t)wide->lastrec[lrd + 2],
                 (uint32_t)wide->lastrec[lrd + 3],
                 (uint32_t)wide->lastrec[lrd + 4]);
    }
    line = AddLine(line, "");
    AddLineF(line, lng_head);
    line = AddLine(line, "");
    int _count  = 0;
    char * _ptr = (char *)&_hdr;

    for( ; _count < sizeof(HudsHdr); _ptr += 16, _count += 16)
    {
        gsprintf(PRINTF_DECLARE_BUFFER(buf), "%04X   ", _count);
        HexDump16(buf + 7, _ptr, 16, HEX_DUMP2);
        line = AddLine(line, buf);
    }
    gsprintf(PRINTF_DECLARE_BUFFER(buf), "%04X   ", _count);
    HexDump16(buf + 7, _ptr, sizeof(HudsHdr) % 16, HEX_DUMP2);
    line = AddLine(line, buf);
    GFTRK(0);
    return line;
} // >::make_dump_msg

//  ------------------------------------------------------------------
