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

#include <gmemdbg.h>
#include <gdbgtrk.h>
#include <gstrall.h>

#include <gmowcat.h>
//  ------------------------------------------------------------------
int WCatArea::load_message(int __mode, gmsg * __msg, WCatHdr & __hdr)
{
    // Setup some local variables for speed
    int _fhdat     = data->fhdat;
    WCatIdx * _idx = data->idx;
    uint _reln     = Msgn->ToReln(__msg->msgno);

    // Load the message header
    memset(&__hdr, 0, sizeof(WCatHdr));
    lseekset(_fhdat, _idx[_reln - 1].offset);
    read(_fhdat, &__hdr, sizeof(WCatHdr));
    // Convert header
    __msg->timesread = 1;
    __msg->link.to_set(__hdr.reference);
    struplow(strnp2cc(__msg->by, __hdr.from, 70));
    struplow(strnp2cc(__msg->to, __hdr.to, 70));
    strnp2cc(__msg->re, __hdr.subject, 70);
    strnp2cc(__msg->wildcat.from_title, __hdr.fromtitle, 10);
    strnp2cc(__msg->wildcat.to_title, __hdr.totitle, 10);
    strnp2cc(__msg->wildcat.network, __hdr.network, 8);
    strnp2cc(__msg->wildcat.internal_attach, __hdr.internalattach, 12);
    strnp2cc(__msg->wildcat.external_attach, __hdr.externalattach, 12);
    memcpy(__msg->wildcat.reserved, __hdr.reserved, 20);
    __msg->wildcat.from_userid = __hdr.fromuserid;
    __msg->wildcat.to_userid   = __hdr.touserid;
    __msg->wildcat.next_unread = __hdr.nextunread;
    __msg->wildcat.prev_unread = __hdr.prevunread;
    __msg->cost       = (uint)__hdr.cost;
    __msg->orig.zone  = __msg->oorig.zone = __hdr.origaddr.zone;
    __msg->orig.net   = __msg->oorig.net = __hdr.origaddr.net;
    __msg->orig.node  = __msg->oorig.node = __hdr.origaddr.node;
    __msg->orig.point = __msg->oorig.point = __hdr.origaddr.point;
    __msg->dest.zone  = __msg->odest.zone = __hdr.destaddr.zone;
    __msg->dest.net   = __msg->odest.net = __hdr.destaddr.net;
    __msg->dest.node  = __msg->odest.node = __hdr.destaddr.node;
    __msg->dest.point = __msg->odest.point = __hdr.destaddr.point;
    // Convert date and time
    struct tm _tm;
    int32_t _time;
    unsigned _year, _month, _day, _hour, _minute, _second;
    __msg->written = __msg->arrived = __msg->received = 0;

    if(__hdr.msgdate and __hdr.msgtime)
    {
        JDN2YMD(__hdr.msgdate + 1, &_year, &_month, &_day);
        _time        = __hdr.msgtime - 1;
        _hour        = (unsigned)(_time / 3600L);
        _minute      = (unsigned)((_time % 3600L) / 60L);
        _second      = (unsigned)(_time - ((_hour * 3600L) + _minute * 60L));
        _tm.tm_year  = _year - 1900;
        _tm.tm_mon   = _month - 1;
        _tm.tm_mday  = _day;
        _tm.tm_hour  = _hour;
        _tm.tm_min   = _minute;
        _tm.tm_sec   = _second;
        _tm.tm_isdst = -1;
        time32_t a = gmktime(&_tm);
        struct tm tp;
        ggmtime(&tp, &a);
        tp.tm_isdst = -1;
        time32_t b = gmktime(&tp);
        __msg->written = a + a - b;
    }

    if(__hdr.readdate and __hdr.readtime)
    {
        JDN2YMD(__hdr.readdate + 1, &_year, &_month, &_day);
        _time        = __hdr.readtime - 1;
        _hour        = (unsigned)(_time / 3600L);
        _minute      = (unsigned)((_time % 3600L) / 60L);
        _second      = (unsigned)(_time - ((_hour * 3600L) + _minute * 60L));
        _tm.tm_year  = _year - 1900;
        _tm.tm_mon   = _month - 1;
        _tm.tm_mday  = _day;
        _tm.tm_hour  = _hour;
        _tm.tm_min   = _minute;
        _tm.tm_sec   = _second;
        _tm.tm_isdst = -1;
        time32_t a = gmktime(&_tm);
        struct tm tp;
        ggmtime(&tp, &a);
        tp.tm_isdst = -1;
        time32_t b = gmktime(&tp);
        __msg->received = a + a - b;
    }

    // Convert fidoflag attributes
    __msg->attr.pvt(__hdr.fidoflags & FIDO_PVT);
    __msg->attr.cra(__hdr.fidoflags & FIDO_CRASH);
    __msg->attr.rcv(__hdr.fidoflags & FIDO_RECEIVED);
    __msg->attr.snt(__hdr.fidoflags & FIDO_SENT);
    __msg->attr.att(__hdr.fidoflags & FIDO_ATTACH);
    __msg->attr.trs(__hdr.fidoflags & FIDO_TRANSIT);
    __msg->attr.orp(__hdr.fidoflags & FIDO_ORPHAN);
    __msg->attr.k_s(__hdr.fidoflags & FIDO_KILLSENT);
    __msg->attr.loc(__hdr.fidoflags & FIDO_LOCAL);
    __msg->attr.hld(__hdr.fidoflags & FIDO_HOLD);
    __msg->attr.rsv(__hdr.fidoflags & FIDO_RESERVED);
    __msg->attr.frq(__hdr.fidoflags & FIDO_FREQ);
    __msg->attr.rrq(__hdr.fidoflags & FIDO_RETRECREQ);
    __msg->attr.rrc(__hdr.fidoflags & FIDO_RETREC);
    __msg->attr.arq(__hdr.fidoflags & FIDO_AUDITREQ);
    __msg->attr.urq(__hdr.fidoflags & FIDO_UPDREQ);
    // Convert mflag attributes
    __msg->attr.pvt(__hdr.mflags & mfPrivate);
    __msg->attr.rab(__hdr.mflags & mfReceiveable);
    __msg->attr.rcv(__hdr.mflags & mfReceived);
    __msg->attr.rrc(__hdr.mflags & mfReceipt);
    __msg->attr.car(__hdr.mflags & mfCarboned);
    __msg->attr.fwd(__hdr.mflags & mfForwarded);
    __msg->attr.efl(__hdr.mflags & mfEchoFlag);
    __msg->attr.hrp(__hdr.mflags & mfHasReplies);
    __msg->attr.del(__hdr.mflags & mfDeleted);
    __msg->attr.tag(__hdr.mflags & mfTagged);
    __msg->attr.snt(__hdr.mflags & mfSent);
    // Set the unsent attribute
    __msg->attr.uns(__msg->attr.loc() and not __msg->attr.snt());
    __msg->txtlength = __hdr.msgbytes;

    // If message text is requested
    if(__mode & GMSG_TXT)
    {
        // Get length of message text
        uint _txtlen = __hdr.msgbytes;
        // Allocate space for the message text
        __msg->txt = (char *)throw_calloc(1, _txtlen + 256);
        // Read the message text
        read(_fhdat, __msg->txt, _txtlen);
        // Convert kludge char from NUL to CTRL-A
        char * p = __msg->txt;

        for(int n = 0; n < _txtlen; n++, p++)
        {
            if(*p == '\0')
            {
                *p = CTRL_A;
            }
        }
    }

    GFTRK(0);
    // Success
    return true;
} // WCatArea::load_message

//  ------------------------------------------------------------------
int WCatArea::load_hdr(gmsg * __msg)
{
    GFTRK("WCatLoadHdr");
    WCatHdr _hdr;
    return load_message(GMSG_HDR, __msg, _hdr);
}

//  ------------------------------------------------------------------
int WCatArea::load_msg(gmsg * __msg)
{
    GFTRK("WCatLoadMsg");
    WCatHdr _hdr;
    return load_message(GMSG_HDRTXT, __msg, _hdr);
}

//  ------------------------------------------------------------------
