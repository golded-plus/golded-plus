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


//  ------------------------------------------------------------------

#include <gmemdbg.h>
#include <gdbgtrk.h>
#include <gstrall.h>

#include <gmoxbbs.h>


//  ------------------------------------------------------------------

int XbbsArea::load_message(int __mode, gmsg* __msg, XbbsHdr& __hdr)
{

    // Setup some local variables for speed
    int _fhdata = data->fhdata;
    uint _reln = Msgn->ToReln(__msg->msgno);

    // Load the message header
    memset(&__hdr, 0, sizeof(XbbsHdr));
    lseekset(_fhdata, (_reln-1)*sizeof(XbbsHdr));
    read(_fhdata, &__hdr, sizeof(XbbsHdr));

    // Convert header

    strcpy(__msg->by, __hdr.from);
    strcpy(__msg->to, __hdr.to);
    strcpy(__msg->re, __hdr.subj);

    __msg->orig.zone  = __msg->oorig.zone  = __hdr.origaddr.zone;
    __msg->orig.net   = __msg->oorig.net   = __hdr.origaddr.net;
    __msg->orig.node  = __msg->oorig.node  = __hdr.origaddr.node;
    __msg->orig.point = __msg->oorig.point = __hdr.origaddr.point;

    __msg->dest.zone  = __msg->odest.zone  = __hdr.destaddr.zone;
    __msg->dest.net   = __msg->odest.net   = __hdr.destaddr.net;
    __msg->dest.node  = __msg->odest.node  = __hdr.destaddr.node;
    __msg->dest.point = __msg->odest.point = __hdr.destaddr.point;

    __msg->written = FidoTimeToUnix(__hdr.date);
    __msg->received = __hdr.timerecv;

    if(__hdr.indate[2])
    {
        struct tm t;
        t.tm_year   = __hdr.indate[0]+89;
        t.tm_mon    = __hdr.indate[1]-1;
        t.tm_mday   = __hdr.indate[2];
        t.tm_hour   = t.tm_min = t.tm_sec = 0;
        t.tm_isdst  = -1;
        time32_t a  = gmktime(&t);
        struct tm tp;
        ggmtime(&tp, &a);
        tp.tm_isdst = -1;
        time32_t b  = gmktime(&tp);
        __msg->arrived = a + a - b;
    }

    __msg->cost      = __hdr.cost;
    __msg->timesread = __hdr.timesread;

    __msg->txtstart  = __hdr.start;
    __msg->txtlength = __hdr.length;

    // Convert fflag attributes
    __msg->attr.pvt(__hdr.fflags & FFLAGS_MSGPRIVATE);
    __msg->attr.cra(__hdr.fflags & FFLAGS_MSGCRASH);
    __msg->attr.rcv(__hdr.fflags & FFLAGS_MSGREAD);
    __msg->attr.snt(__hdr.fflags & FFLAGS_MSGSENT);
    __msg->attr.att(__hdr.fflags & FFLAGS_MSGFILE);
    __msg->attr.trs(__hdr.fflags & FFLAGS_MSGFWD);
    __msg->attr.orp(__hdr.fflags & FFLAGS_MSGORPHAN);
    __msg->attr.k_s(__hdr.fflags & FFLAGS_MSGKILL);
    __msg->attr.loc(__hdr.fflags & FFLAGS_MSGLOCAL);
    __msg->attr.frq(__hdr.fflags & FFLAGS_MSGFRQ);
    __msg->attr.rrq(__hdr.fflags & FFLAGS_MSGRRQ);
    __msg->attr.rrc(__hdr.fflags & FFLAGS_MSGCPT);
    __msg->attr.arq(__hdr.fflags & FFLAGS_MSGARQ);
    __msg->attr.urq(__hdr.fflags & FFLAGS_MSGURQ);

    // Convert xflag attributes
    __msg->attr.del(__hdr.xflags & XFLAGS_MSGDELETED);
    __msg->attr.ano(__hdr.xflags & XFLAGS_MSGANON);
    __msg->attr.ume(__hdr.xflags & XFLAGS_MSGECHO);
    __msg->attr.umn(__hdr.xflags & XFLAGS_MSGNET);
    __msg->attr.fsc(__hdr.xflags & XFLAGS_MSGSCANNED);
    __msg->attr.lok(__hdr.xflags & XFLAGS_MSGKEEP);
    __msg->attr.trt(__hdr.xflags & XFLAGS_MSGTREATED);
    __msg->attr.lzs(__hdr.xflags & XFLAGS_MSGPACKED);
    __msg->attr.gsc(__hdr.xflags & XFLAGS_MSGGSCAN);
    __msg->attr.rsc(__hdr.xflags & XFLAGS_MSGRSCAN);
    __msg->attr.arc(__hdr.xflags & XFLAGS_MSGARCHIVED);
    __msg->attr.tag(__hdr.xflags & XFLAGS_MSGTAGGED);

    // Set the unsent attribute
#if 0
    if(isnet())
        __msg->attr.uns((__msg->attr.loc() and not __msg->attr.snt()) or (__hdr.xflags & XFLAGS_MSGNET));
    else
        __msg->attr.uns(__hdr.xflags & XFLAGS_MSGECHO);
#endif

    if(isnet() or isecho())
        __msg->attr.uns(not (__hdr.xflags & XFLAGS_MSGSCANNED));

    __msg->adeptxbbs.iflags = __hdr.iflags;
    __msg->adeptxbbs.oflags = __hdr.oflags;

    // If message text is requested
    if(__mode & GMSG_TXT)
    {

        // Get length of message text
        uint _txtlen = __hdr.length;

        // Allocate space for the message text
        __msg->txt = (char*)throw_calloc(1, _txtlen+256);

        // Read the message text
        lseekset(data->fhtext, __hdr.start);
        read(data->fhtext, __msg->txt, _txtlen);
    }

    GFTRK(0);

    // Success
    return true;
}


//  ------------------------------------------------------------------

int XbbsArea::load_hdr(gmsg* __msg)
{

    GFTRK("XbbsLoadHdr");

    XbbsHdr _hdr;
    return load_message(GMSG_HDR, __msg, _hdr);
}


//  ------------------------------------------------------------------

int XbbsArea::load_msg(gmsg* __msg)
{

    GFTRK("XbbsLoadMsg");

    XbbsHdr _hdr;
    return load_message(GMSG_HDRTXT, __msg, _hdr);
}


//  ------------------------------------------------------------------

