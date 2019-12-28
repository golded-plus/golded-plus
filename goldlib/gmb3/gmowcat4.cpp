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

#include <gdbgerr.h>
#include <gmemdbg.h>
#include <gdbgtrk.h>
#include <gstrall.h>

#include <gmowcat.h>


//  ------------------------------------------------------------------

void WCatArea::lock()
{

    GFTRK("WCatLock");

    if(not data->islocked)
    {
        if(WideCanLock)
        {
            long _tries = 0;
            while(::lock(data->fhix, 0, 1) == -1)
            {
                if(PopupLocked(++_tries, true, real_path()) == false)
                {
                    WideLog->ErrLock();
                    raw_close();
                    WideLog->printf("! A WildCat! msgbase file could not be locked.");
                    WideLog->printf(": %s.ix.", real_path());
                    WideLog->ErrOSInfo();
                    LockErrorExit();
                }
            }
            if(_tries)
                PopupLocked(0, 0, NULL);
        }
        refresh();
        data->islocked = true;
    }

    GFTRK(0);
}


//  ------------------------------------------------------------------

void WCatArea::unlock()
{

    GFTRK("WCatUnlock");
    if(WideCanLock and data->islocked)
        ::unlock(data->fhix, 0, 1);
    data->islocked = false;

    GFTRK(0);
}


//  ------------------------------------------------------------------

void WCatArea::save_message(int __mode, gmsg* __msg, WCatHdr& __hdr)
{

    int _was_locked = data->islocked;
    if(not _was_locked)
        lock();

    memset(&__hdr, 0, sizeof(WCatHdr));

    if(__mode & GMSG_NEW)
    {
        data->base.active++;
        __msg->msgno = data->base.nextmsgno++;
        Msgn->Append(__msg->msgno);
        data->idx = (WCatIdx*)throw_realloc(data->idx, data->base.active*sizeof(WCatIdx));
        __hdr.mflags |= mfReceiveable;
        lseekset(data->fhix, 0);
        write(data->fhix, &data->base, sizeof(WCatBase));
    }
    else if(__mode & GMSG_DELETE)
    {
        __hdr.mflags |= mfDeleted;
    }

    __hdr.magicnumber = MagicHeaderActive;
    __hdr.msgno = (word)__msg->msgno;

    strc2p(strupr(strxcpy(__hdr.from, __msg->by, sizeof(__hdr.from))));
    strc2p(strcpy(__hdr.fromtitle, __msg->wildcat.from_title));
    __hdr.fromuserid = __msg->wildcat.from_userid;

    strc2p(strupr(strxcpy(__hdr.to, __msg->to, sizeof(__hdr.to))));
    strc2p(strcpy(__hdr.totitle, __msg->wildcat.to_title));
    __hdr.touserid = __msg->wildcat.to_userid;

    strc2p(strxcpy(__hdr.subject, __msg->re, sizeof(__hdr.subject)));

    if(not *__msg->wildcat.network and (isnet() or isecho()))
        strcpy(__msg->wildcat.network, "FTSC");
    strc2p(strcpy(__hdr.network, __msg->wildcat.network));

    if (__msg->written)
    {
        struct tm _tm;
        ggmtime(&_tm, &__msg->written);
        __hdr.msgdate = (word)(YMD2JDN(1900+_tm.tm_year, _tm.tm_mon+1, _tm.tm_mday)-1);
        __hdr.msgtime = ((_tm.tm_hour*3600L)+(_tm.tm_min*60L)+_tm.tm_sec)+1;
    }

    if (__msg->received)
    {
        struct tm _tm;
        ggmtime(&_tm, &__msg->received);
        __hdr.readdate = (word)YMD2JDN(1900+_tm.tm_year, _tm.tm_mon+1, _tm.tm_mday);
        __hdr.readtime = ((_tm.tm_hour*3600L)+(_tm.tm_min*60L)+_tm.tm_sec)+1;
    }

    __hdr.mflags |= (word)(__msg->attr.pvt() ? mfPrivate     : 0);
    __hdr.mflags |= (word)(__msg->attr.rab() ? mfReceiveable : 0);
    __hdr.mflags |= (word)(__msg->attr.rcv() ? mfReceived    : 0);
    __hdr.mflags |= (word)(__msg->attr.rrc() ? mfReceipt     : 0);
    __hdr.mflags |= (word)(__msg->attr.car() ? mfCarboned    : 0);
    __hdr.mflags |= (word)(__msg->attr.fwd() ? mfForwarded   : 0);
    __hdr.mflags |= (word)(__msg->attr.efl() ? mfEchoFlag    : 0);
    __hdr.mflags |= (word)(__msg->attr.hrp() ? mfHasReplies  : 0);
    __hdr.mflags |= (word)(__msg->attr.del() ? mfDeleted     : 0);
    __hdr.mflags |= (word)(__msg->attr.tag() ? mfTagged      : 0);
    __hdr.mflags |= (word)(__msg->attr.snt() ? mfSent        : 0);
    if(__msg->link.first())
        __hdr.mflags |= mfHasReplies;

    __hdr.reference = (word)__msg->link.to();

    __hdr.origaddr = __msg->orig;
    __hdr.destaddr = __msg->dest;

    __hdr.msgbytes = (word)__msg->txtlength;

    strc2p(strcpy(__hdr.internalattach, __msg->wildcat.internal_attach));
    strc2p(strcpy(__hdr.externalattach, __msg->wildcat.external_attach));

    __hdr.prevunread = (word)__msg->wildcat.prev_unread;
    __hdr.nextunread = (word)__msg->wildcat.next_unread;

    __hdr.fidoflags |= (word)(__msg->attr.pvt() ? FIDO_PVT       : 0);
    __hdr.fidoflags |= (word)(__msg->attr.cra() ? FIDO_CRASH     : 0);
    __hdr.fidoflags |= (word)(__msg->attr.rcv() ? FIDO_RECEIVED  : 0);
    __hdr.fidoflags |= (word)(__msg->attr.snt() ? FIDO_SENT      : 0);
    __hdr.fidoflags |= (word)(__msg->attr.att() ? FIDO_ATTACH    : 0);
    __hdr.fidoflags |= (word)(__msg->attr.trs() ? FIDO_TRANSIT   : 0);
    __hdr.fidoflags |= (word)(__msg->attr.orp() ? FIDO_ORPHAN    : 0);
    __hdr.fidoflags |= (word)(__msg->attr.k_s() ? FIDO_KILLSENT  : 0);
    __hdr.fidoflags |= (word)(__msg->attr.loc() ? FIDO_LOCAL     : 0);
    __hdr.fidoflags |= (word)(__msg->attr.hld() ? FIDO_HOLD      : 0);
    __hdr.fidoflags |= (word)(__msg->attr.rsv() ? FIDO_RESERVED  : 0);
    __hdr.fidoflags |= (word)(__msg->attr.frq() ? FIDO_FREQ      : 0);
    __hdr.fidoflags |= (word)(__msg->attr.rrq() ? FIDO_RETRECREQ : 0);
    __hdr.fidoflags |= (word)(__msg->attr.rrc() ? FIDO_RETREC    : 0);
    __hdr.fidoflags |= (word)(__msg->attr.arq() ? FIDO_AUDITREQ  : 0);
    __hdr.fidoflags |= (word)(__msg->attr.urq() ? FIDO_UPDREQ    : 0);

    __hdr.cost = __msg->cost;

    memcpy(__hdr.reserved, __msg->wildcat.reserved, 20);

    char* _txt = NULL;
    uint _reln = Msgn->ToReln(__msg->msgno)-1;
    int32_t _datstart = data->idx[_reln].offset;

    if(__mode & GMSG_TXT)
    {

        _txt = throw_strdup(__msg->txt);
        word _size = (word)strlen(_txt);
        __hdr.msgbytes = _size;
        word n = 0;
        char* ptr = _txt;
        while(n < _size)
        {
            if(*ptr == CTRL_A)
                *ptr = NUL;
            ptr++;
            n++;
        }

        if((__mode & GMSG_NEW) or (_size > __hdr.msgbytes))
        {
            if(_size > __hdr.msgbytes)
            {
                uint32_t _magic = MagicHeaderInactive;
                lseekset(data->fhdat, data->idx[_reln].offset);
                write(data->fhdat, &_magic, 4);
            }
            _datstart = filelength(data->fhdat);
            data->idx[_reln].msgno = __hdr.msgno;
            data->idx[_reln].offset = _datstart;
            lseekset(data->fhix, sizeof(WCatBase)+(_reln*sizeof(WCatIdx)));
            write(data->fhix, &data->idx[_reln], sizeof(WCatIdx));
        }
        __hdr.msgbytes = _size;
    }

    lseekset(data->fhdat, _datstart);
    write(data->fhdat, &__hdr, sizeof(WCatHdr));
    if(_txt)
    {
        write(data->fhdat, _txt, __hdr.msgbytes);
        throw_free(_txt);
    }

    if(not _was_locked)
        unlock();

    GFTRK(0);
}


//  ------------------------------------------------------------------

void WCatArea::save_hdr(int __mode, gmsg* __msg)
{

    GFTRK("WCatSaveHdr");

    WCatHdr _hdr;
    save_message(__mode|GMSG_HDR, __msg, _hdr);
}


//  ------------------------------------------------------------------

void WCatArea::save_msg(int __mode, gmsg* __msg)
{

    GFTRK("WCatSaveMsg");

    WCatHdr _hdr;
    save_message(__mode|GMSG_HDRTXT, __msg, _hdr);
}


//  ------------------------------------------------------------------

void WCatArea::del_msg(gmsg* __msg)
{

    GFTRK("WCatDelMsg");

    WCatHdr _hdr;
    save_message(GMSG_HDR | GMSG_DELETE, __msg, _hdr);
}


//  ------------------------------------------------------------------

void WCatArea::new_msgno(gmsg* __msg)
{

    GFTRK("WCatNewMsgno");

    __msg->msgno = data->base.nextmsgno;

    GFTRK(0);
}


//  ------------------------------------------------------------------

void WCatArea::update_timesread(gmsg*)
{

}


//  ------------------------------------------------------------------

