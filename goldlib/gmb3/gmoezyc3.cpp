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
//  ------------------------------------------------------------------

#include <gmemdbg.h>
#include <gdbgtrk.h>
#include <gstrall.h>
#include <gutlmisc.h>

#include <gmoezyc.h>
//  ------------------------------------------------------------------
int EzycomArea::load_message(int __mode, gmsg * __msg, EzycHdr & __hdr)
{
    // Load the header
    __hdr = EzycHdr();
    lseekset(data->fhhdr, __msg->msgno - 1, sizeof(EzycHdr));
    read(data->fhhdr, &__hdr, sizeof(EzycHdr));
    // Convert header data
    __msg->link.to_set(__hdr.replyto);
    __msg->link.first_set(__hdr.reply1st);
    __msg->cost      = __hdr.cost;
    __msg->txtstart  = __hdr.startposition;
    __msg->txtlength = __hdr.messagelength;
    strnp2cc(__msg->by, __hdr.whofrom, EZYC_MAXNAME - 1);
    strnp2cc(__msg->to, __hdr.whoto, EZYC_MAXNAME - 1);
    strnp2cc(__msg->re, __hdr.subject, EZYC_MAXSUBJ - 1);
    __msg->orig.zone  = __msg->oorig.zone = __hdr.orignet.zone;
    __msg->orig.net   = __msg->oorig.net = __hdr.orignet.net;
    __msg->orig.node  = __msg->oorig.node = __hdr.orignet.node;
    __msg->orig.point = __msg->oorig.point = __hdr.orignet.point;
    __msg->dest.zone  = __msg->odest.zone = __hdr.destnet.zone;
    __msg->dest.net   = __msg->odest.net = __hdr.destnet.net;
    __msg->dest.node  = __msg->odest.node = __hdr.destnet.node;
    __msg->dest.point = __msg->odest.point = __hdr.destnet.point;
    // Convert date and time
    SwapWord32((uint32_t *)&__hdr.posttimedate);
    SwapWord32((uint32_t *)&__hdr.recvtimedate);
    __msg->written = FTimeToTime(&__hdr.posttimedate);
    __msg->arrived = FTimeToTime(&__hdr.recvtimedate);
    // Convert attributes
    __msg->attr.del(__hdr.msgattr & EZYC_MSGATTR_DELETED);
    __msg->attr.pvt(__hdr.msgattr & EZYC_MSGATTR_PRIVATE);
    __msg->attr.rcv(__hdr.msgattr & EZYC_MSGATTR_RECEIVED);
    __msg->attr.loc(__hdr.msgattr & EZYC_MSGATTR_LOCAL);
    __msg->attr.lok(__hdr.msgattr & EZYC_MSGATTR_NOKILL);
    __msg->attr.k_s(__hdr.netattr & EZYC_NETATTR_KILLSENT);
    __msg->attr.snt(__hdr.netattr & EZYC_NETATTR_SENT);
    __msg->attr.att(__hdr.netattr & EZYC_NETATTR_ATTACH);
    __msg->attr.cra(__hdr.netattr & EZYC_NETATTR_CRASH);
    __msg->attr.frq(__hdr.netattr & EZYC_NETATTR_FREQ);
    __msg->attr.rrq(__hdr.netattr & EZYC_NETATTR_RREQ);
    __msg->attr.arq(__hdr.netattr & EZYC_NETATTR_AREQ);
    __msg->attr.rrc(__hdr.netattr & EZYC_NETATTR_RREC);
    __msg->attr.uns(((__hdr.msgattr & EZYC_MSGATTR_NETPEND) or (__hdr.msgattr &
                                                                EZYC_MSGATTR_ECHOPEND)) ?
                    1 : 0);
    __msg->ezycom.extattr = __hdr.extattr;
    __msg->timesread      = (__hdr.extattr & EZYC_EXTATTR_SEEN) ? 1 : 0;

    if(__mode & GMSG_TXT)
    {
        __msg->txt = (char *)throw_calloc(1, (uint)(__hdr.messagelength + 256));
        lseekset(data->fhtxt, __hdr.startposition);
        read(data->fhtxt, __msg->txt, (uint)__hdr.messagelength);
    }

    GFTRK(0);
    return true;
} // EzycomArea::load_message

//  ------------------------------------------------------------------
int EzycomArea::load_hdr(gmsg * __msg)
{
    GFTRK("EzycomLoadHdr");
    EzycHdr _hdr;
    return load_message(GMSG_HDR, __msg, _hdr);
}

//  ------------------------------------------------------------------
int EzycomArea::load_msg(gmsg * __msg)
{
    GFTRK("EzycomLoadMsg");
    EzycHdr _hdr;
    return load_message(GMSG_HDRTXT, __msg, _hdr);
}

//  ------------------------------------------------------------------
