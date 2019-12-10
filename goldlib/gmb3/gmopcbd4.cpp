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

#include <gdbgerr.h>
#include <gdbgtrk.h>
#include <gstrall.h>
#include <gutlmisc.h>

#include <gmopcbd.h>
// ------------------------------------------------------------------
void PcbArea::lock()
{
    GFTRK("PcbLock");

    if(not data->islocked)
    {
        if(WideCanLock)
        {
            long _tries = 0;

            while(::lock(data->fhmsg, 16, 6) == -1)
            {
                if(PopupLocked(++_tries, true, real_path()) == false)
                {
                    WideLog->ErrLock();
                    raw_close();
                    WideLog->printf("! A PCBoard msgbase file could not be locked.");
                    WideLog->printf(": %s.", real_path());
                    WideLog->ErrOSInfo();
                    LockErrorExit();
                }
            }

            if(_tries)
            {
                PopupLocked(0, 0, NULL);
            }
        }

        lseekset(data->fhmsg, 0);
        read(data->fhmsg, &data->base, sizeof(PcbBase));
        lseekset(data->fhmsg, 0);
        memcpy(data->base.locked, "LOCKED", 6);
        write(data->fhmsg, &data->base, sizeof(PcbBase));
        data->base.highmsgno = B2L(data->base.highmsgno);
        data->base.lowmsgno  = B2L(data->base.lowmsgno);
        data->base.active    = B2L(data->base.active);
        data->islocked       = true;
    }

    GFTRK(0);
} // PcbArea::lock

// ------------------------------------------------------------------
void PcbArea::unlock()
{
    GFTRK("PcbUnlock");

    if(WideCanLock and data->islocked)
    {
        ::unlock(data->fhmsg, 16, 6);
    }

    lseekset(data->fhmsg, 0);
    memset(data->base.locked, ' ', 6);
    data->base.highmsgno = L2B(data->base.highmsgno);
    data->base.lowmsgno  = L2B(data->base.lowmsgno);
    data->base.active    = L2B(data->base.active);
    write(data->fhmsg, &data->base, sizeof(PcbBase));
    data->base.highmsgno = B2L(data->base.highmsgno);
    data->base.lowmsgno  = B2L(data->base.lowmsgno);
    data->base.active    = B2L(data->base.active);
    data->islocked       = false;
    GFTRK(0);
}

// ------------------------------------------------------------------
void PcbArea::save_message(int __mode, gmsg * __msg, PcbHdr & __hdr)
{
    // Lock and refresh base
    int _was_locked = data->islocked;

    if(not _was_locked)
    {
        lock();
    }

    // Reset header
    memset(&__hdr, 0, sizeof(PcbHdr));
    // Find msgno and index offset
    PcbIdx _idx;
    uint32_t oldmsgno    = __msg->msgno;
    int32_t oldtxtstart  = __msg->txtstart;
    int32_t oldtxtlength = __msg->txtlength;

    if(__mode & GMSG_TXT)
    {
        if(data->base.lowmsgno == 0)
        {
            data->base.lowmsgno = 1;
        }

        data->base.highmsgno++;
        data->base.active++;
        __msg->msgno = data->base.highmsgno;
        Msgn->Append(__msg->msgno);
    }

    int32_t _idxoffset = (__msg->msgno - data->base.lowmsgno) * sizeof(PcbIdx);
    __hdr.blocks     = (byte)__msg->txtblocks;
    _idx.num         = __msg->msgno;
    _idx.reserved[0] = _idx.reserved[1] = _idx.reserved[2] = 0;
    // Convert msgno and replylinks
    __hdr.msgno = L2B(__msg->msgno);
    __hdr.refno = L2B(__msg->link.to());
    // Convert attributes
    char _status = __msg->pcboard.status;
    int _pvt     = __msg->attr.pvt();
    int _rcv     = __msg->attr.rcv();

    if(_rcv)
    {
        switch(_status)
        {
            case '$':
                __hdr.status = '#';
                break;

            case '!':
                __hdr.status = '#';
                break;

            case '%':
                __hdr.status = '^';
                break;

            case '~':
                __hdr.status = '`';
                break;

            case '*':
                __hdr.status = '+';
                break;

            case ' ':
                __hdr.status = '-';
                break;

            default:
                __hdr.status = _pvt ? '+' : '-';
        } // switch
    }
    else
    {
        if(_status and not _pvt)
        {
            __hdr.status = _status;
        }
        else if(_pvt)
        {
            __hdr.status = '*';
        }
        else
        {
            __hdr.status = ' ';
        }
    }

    _idx.status        = __hdr.status;
    __hdr.activestatus = (__mode & GMSG_DELETE) ? '\xE2' : '\xE1';
    __hdr.echoed       = ((isnet() or isecho()) and not isinternet()) ? 'E' : ' ';
    // Convert dates and times
    char _dtbuf[9];
    struct tm _tm;
    ggmtime(&_tm, &__msg->written);
    memcpy(__hdr.date,
           strftimei(_dtbuf, 9, __msg->attr.uns() ? "%d-%m-%y" : "%d-%m\xC4%y", &_tm),
           8);
    memcpy(__hdr.time, strftimei(_dtbuf, 6, "%H:%M", &_tm), 5);
    _idx.date = (word)YMD2JDN(1900 + _tm.tm_year, _tm.tm_mon + 1, _tm.tm_mday);

    if(__msg->link.first())
    {
        __hdr.hasreply = 'R';
        ggmtime(&_tm, &__msg->pcboard.reply_written);
        int _year = _tm.tm_year % 100;
        __hdr.replydate =
            L2B((10000L * _year) + (100L * (_tm.tm_mon + 1)) + _tm.tm_mday);
        memcpy(__hdr.replytime, strftimei(_dtbuf, 6, "%H:%M", &_tm), 5);
    }
    else
    {
        __hdr.hasreply  = ' ';
        __hdr.replydate = L2B(0);
        memcpy(__hdr.replytime, "     ", 5);
    }

    // Convert names, subject and password
    char tobuf[150];
    char bybuf[150];
    strcpy(tobuf, __msg->to);
    strcpy(bybuf, __msg->by);
    int _tolen = strlen(tobuf);
    int _bylen = strlen(bybuf);
    int _relen = strlen(__msg->re);
    int _pwlen = strlen(__msg->pcboard.password);
    memset(__hdr.destname, ' ', 25);
    memset(__hdr.origname, ' ', 25);
    memset(__hdr.subject, ' ', 25);
    memset(__hdr.password, ' ', 12);
    strncpy(__hdr.destname, tobuf, MinV(_tolen, 25));
    strncpy(__hdr.origname, bybuf, MinV(_bylen, 25));
    strncpy(__hdr.subject, __msg->re, MinV(_relen, 25));
    strncpy(__hdr.password, __msg->pcboard.password, MinV(_pwlen, 12));
    memcpy(_idx.to, __hdr.destname, 25);
    memcpy(_idx.from, __hdr.origname, 25);

    if(isnet())
    {
        char toaddr[40];
        char byaddr[40];
        __msg->dest.make_string(toaddr);
        strcpy(byaddr, " [");
        __msg->orig.make_string(byaddr + 2);
        strcat(byaddr, "]");
        sprintf(tobuf + strlen(tobuf),
                "@%s%s%s%s",
                toaddr,
                __msg->attr.cra() ? " +C" : "",
                __msg->attr.dir() ? " +D" : "",
                byaddr);
        _tolen = strlen(tobuf);
        _bylen = strlen(bybuf);
    }

    // Determine size of the msg text including extended headers
    uint _txtlen = 0;

    if(__mode & GMSG_TXT)
    {
        __msg->txtlength = _txtlen = strlen(__msg->txt);

        if((_tolen > 25) or isnet())
        {
            __msg->txtlength  += sizeof(PcbExtHdr);
            __hdr.exthdrflags |= 0x01;
        }

        if((_bylen > 25) or isnet())
        {
            __msg->txtlength  += sizeof(PcbExtHdr);
            __hdr.exthdrflags |= 0x02;
        }

        if(_relen > 25)
        {
            __msg->txtlength  += sizeof(PcbExtHdr);
            __hdr.exthdrflags |= 0x04;
        }

        // Translate msg text to PCB linefeeds if running non-foreign system
        if(not wide->foreign)
        {
            strchg(__msg->txt, 0x0D, 0xE3);
        }

        // Calculate new number of blocks
        __hdr.blocks =
            (byte)(1 + (__msg->txtlength / 128) + ((__msg->txtlength % 128) ? 1 : 0));
        // Determine where to write the message text
        __msg->txtstart  = filelength(data->fhmsg);
        __msg->txtblocks = __hdr.blocks;
    }

    // Write index
    _idx.offset = (__mode & GMSG_DELETE) ? -__msg->txtstart : __msg->txtstart;
    lseekset(data->fhidx, _idxoffset);
    write(data->fhidx, &_idx, sizeof(PcbIdx));
    // Write header
    lseekset(data->fhmsg, AbsV(_idx.offset));
    write(data->fhmsg, &__hdr, sizeof(PcbHdr));

    // Writing msg text?
    if(__mode & GMSG_TXT)
    {
        uint _txtlenwritten = 0;

        // Write extended headers
        if((_tolen > 25) or isnet())
        {
            PcbExtHdr _ehdr;
            _ehdr.id        = 0x40FF;
            _ehdr.colon     = ':';
            _ehdr.status    = 'N';
            _ehdr.separator = wide->foreign ? '\x0D' : '\xE3';
            memcpy(_ehdr.function, "TO     ", 7);
            memset(_ehdr.desc, ' ', 60);
            memcpy(_ehdr.desc, tobuf, MinV(60, _tolen));
            write(data->fhmsg, &_ehdr, sizeof(PcbExtHdr));
            _txtlenwritten += sizeof(PcbExtHdr);

            if(_tolen > 60)
            {
                memcpy(_ehdr.function, "TO2    ", 7);
                memset(_ehdr.desc, ' ', 60);
                memcpy(_ehdr.desc, tobuf + 60, MinV(60, (_tolen - 60)));
                write(data->fhmsg, &_ehdr, sizeof(PcbExtHdr));
                _txtlenwritten += sizeof(PcbExtHdr);
            }
        }

        if((_bylen > 25) or isnet())
        {
            PcbExtHdr _ehdr;
            _ehdr.id        = 0x40FF;
            _ehdr.colon     = ':';
            _ehdr.status    = 'N';
            _ehdr.separator = wide->foreign ? '\x0D' : '\xE3';
            memcpy(_ehdr.function, "FROM   ", 7);
            memset(_ehdr.desc, ' ', 60);
            memcpy(_ehdr.desc, bybuf, MinV(60, _bylen));
            write(data->fhmsg, &_ehdr, sizeof(PcbExtHdr));
            _txtlenwritten += sizeof(PcbExtHdr);

            if(_bylen > 60)
            {
                memcpy(_ehdr.function, "FROM2  ", 7);
                memset(_ehdr.desc, ' ', 60);
                memcpy(_ehdr.desc, bybuf + 60, MinV(60, (_bylen - 60)));
                write(data->fhmsg, &_ehdr, sizeof(PcbExtHdr));
                _txtlenwritten += sizeof(PcbExtHdr);
            }
        }

        if(_relen > 25)
        {
            PcbExtHdr _ehdr;
            _ehdr.id = 0x40FF;
            memcpy(_ehdr.function, "SUBJECT", 7);
            _ehdr.colon = ':';
            memset(_ehdr.desc, ' ', 60);
            memcpy(_ehdr.desc, __msg->re, MinV(60, _relen));
            _ehdr.status    = 'N';
            _ehdr.separator = wide->foreign ? '\x0D' : '\xE3';
            write(data->fhmsg, &_ehdr, sizeof(PcbExtHdr));
            _txtlenwritten += sizeof(PcbExtHdr);
        }

        // Special handling of netmails
    #if 0

        if(isnet())
        {
            // Write destination address in netmail for FidoPCB
            char _abuf[40], _sbuf[45];
            sprintf(_sbuf,
                    "(%s)%c",
                    __msg->dest.make_string(_abuf),
                    wide->foreign ? '\x0D' : '\xE3');
            uint _sbuflen = strlen(_sbuf);
            write(data->fhmsg, _sbuf, _sbuflen);
            _txtlenwritten += _sbuflen;
            // Write flags
            int _hld = __msg->attr.hld();
            int _imm = __msg->attr.imm();
            int _cra = __msg->attr.cra();
            int _zon = __msg->attr.zon();

            if(_hld or _imm or _cra or _zon)
            {
                strcpy(_sbuf, "(" /*)*/);

                if(_hld)
                {
                    strcat(_sbuf, "HOLD,");
                }

                if(_imm)
                {
                    strcat(_sbuf, "IMM,");
                }

                if(_cra)
                {
                    strcat(_sbuf, "CRASH,");
                }

                if(_zon)
                {
                    strcat(_sbuf, "INTL,");
                }

                _sbuf[strlen(_sbuf)] = /*(*/ ')';
                _sbuflen             = strlen(_sbuf);
                write(data->fhmsg, _sbuf, _sbuflen);
                _txtlenwritten += _sbuflen;
            }
        }

    #endif // if 0
        // Write message text
        write(data->fhmsg, __msg->txt, _txtlen);
        _txtlenwritten += _txtlen;
        // Write remainder of the last message block (if any)
        uint _remainlen = ((__hdr.blocks - 1) * 128) - _txtlenwritten;

        if(_remainlen)
        {
            byte _remainrecord[128];
            memset(_remainrecord, ' ', 128);
            write(data->fhmsg, _remainrecord, _remainlen);
        }

        // Translate back
        if(not wide->foreign)
        {
            strchg(__msg->txt, 0xE3, 0x0D);
        }
    }

    if(not (__mode & GMSG_DELETE))
    {
        // Set the mail waiting flag
        int _status = true;

        // Reset it if the msg is being received
        if((__mode & GMSG_UPDATE) and __msg->attr.rcv())
        {
            _status = false;
        }

        // Don't touch the flag if the msg was already received
        if(not (_status and __msg->attr.rcv()))
        {
            pcbwide->user->update_mail_waiting(__msg->to, board(), _status);
        }
    }

    if((__mode & GMSG_TXT) and (__mode & GMSG_UPDATE))
    {
        __msg->msgno     = oldmsgno;
        __msg->txtstart  = oldtxtstart;
        __msg->txtlength = oldtxtlength;
        del_msg(__msg);
    }

    // Unlock and update base
    if(not _was_locked)
    {
        unlock();
    }

    GFTRK(0);
} // PcbArea::save_message

// ------------------------------------------------------------------
void PcbArea::save_hdr(int __mode, gmsg * __msg)
{
    GFTRK("PcbSaveHdr");
    PcbHdr _hdr;
    save_message(__mode | GMSG_HDR, __msg, _hdr);
}

// ------------------------------------------------------------------
void PcbArea::save_msg(int __mode, gmsg * __msg)
{
    GFTRK("PcbSaveMsg");
    PcbHdr _hdr;
    save_message(__mode | GMSG_HDRTXT, __msg, _hdr);
}

//  ------------------------------------------------------------------
void PcbArea::del_msg(gmsg * __msg)
{
    GFTRK("PcbDelMsg");
    PcbHdr _hdr;
    save_message(GMSG_HDR | GMSG_DELETE, __msg, _hdr);
}

// ------------------------------------------------------------------
void PcbArea::new_msgno(gmsg * msg)
{
    GFTRK("PcbNewMsgno");
    msg->msgno = data->base.highmsgno + 1;
    GFTRK(0);
}

//  ------------------------------------------------------------------
void PcbArea::update_timesread(gmsg *){}
// ------------------------------------------------------------------
