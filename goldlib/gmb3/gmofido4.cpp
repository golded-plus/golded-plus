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

#include <sys/types.h>
#include <sys/stat.h>
#if defined (__WATCOMC__) || defined (__MINGW32__) || defined (_MSC_VER)
#include <sys/utime.h>
#else
#include <utime.h>
#endif
#include <gdbgtrk.h>
#include <gstrall.h>
#include <gmofido.h>
//  ------------------------------------------------------------------
void FidoArea::lock()
{
    GFTRK("FidoLock");
    GFTRK(0);
}

//  ------------------------------------------------------------------
void FidoArea::unlock()
{
    GFTRK("FidoUnlock");
    GFTRK(0);
}

//  ------------------------------------------------------------------
void FidoArea::save_message(int __mode, gmsg * __msg, FidoHdr & __hdr)
{
    // Build message filename
    Path _msgfile;

    if(__mode & GMSG_NEW)
    {
        // Get a new message number
        uint _cnt = 0;

        do
        {
            __msg->msgno = Msgn->CvtReln(Msgn->Count()) + (++_cnt);
        }
        while(fexist(build_msgname(_msgfile, __msg->msgno)));

        // Never create a new 1.MSG in echo or local areas.
        if(__msg->msgno == 1)
        {
            if(not isnet())
            {
                __msg->msgno = 2;
                build_msgname(_msgfile, __msg->msgno);
            }
        }

        // Avoid collision with an existing msg
        while(fexist(_msgfile))
        {
            build_msgname(_msgfile, ++__msg->msgno);
            // here we should signal that a rescan is needed
        }
    }
    else
    {
        build_msgname(_msgfile, __msg->msgno);
    }

    // Delete message, if so ordered
    if(__mode & GMSG_DELETE)
    {
        if(remove(_msgfile))
        {
            chmod(_msgfile, S_STDRW);
            remove(_msgfile);
        }

        GFTRK(0);
        return;
    }

    // Get message file attributes
    struct stat st;

    if(stat(_msgfile, &st) == 0)
    {
        bool readonly = not (st.st_mode & S_IWUSR);

        // If the file is read-only, make it read-write-able
        if(readonly)
        {
            chmod(_msgfile, S_STDRW);
        }
    }

    // Determine file open mode
    int _omode = O_RDWR | O_CREAT | O_BINARY;

    if(__mode & GMSG_TXT)
    {
        // Add truncate, in case a changed msg is shorter
        _omode |= O_TRUNC;
    }

    // Open the message file
    int _fh = test_open(_msgfile, _omode, WideSharemode, YES);

    // Get date/time of message file
    if(not (__mode & GMSG_NEW) and not __msg->attr.upd())
    {
        fstat(_fh, &st);
    }

    memset(&__hdr, 0, sizeof(FidoHdr));
    // Transfer attributes
    __hdr.attr |= (word)(__msg->attr.pvt() ? FIDO_PVT : 0);
    __hdr.attr |= (word)(__msg->attr.cra() ? FIDO_CRASH : 0);
    __hdr.attr |= (word)(__msg->attr.rcv() ? FIDO_RECEIVED : 0);
    __hdr.attr |= (word)(__msg->attr.snt() ? FIDO_SENT : 0);
    __hdr.attr |= (word)(__msg->attr.att() ? FIDO_ATTACH : 0);
    __hdr.attr |= (word)(__msg->attr.trs() ? FIDO_TRANSIT : 0);
    __hdr.attr |= (word)(__msg->attr.orp() ? FIDO_ORPHAN : 0);
    __hdr.attr |= (word)(__msg->attr.k_s() ? FIDO_KILLSENT : 0);
    __hdr.attr |= (word)(__msg->attr.loc() ? FIDO_LOCAL : 0);
    __hdr.attr |= (word)(__msg->attr.hld() ? FIDO_HOLD : 0);
    __hdr.attr |= (word)(__msg->attr.rsv() ? FIDO_RESERVED : 0);
    __hdr.attr |= (word)(__msg->attr.frq() ? FIDO_FREQ : 0);
    __hdr.attr |= (word)(__msg->attr.rrq() ? FIDO_RETRECREQ : 0);
    __hdr.attr |= (word)(__msg->attr.rrc() ? FIDO_RETREC : 0);
    __hdr.attr |= (word)(__msg->attr.arq() ? FIDO_AUDITREQ : 0);
    __hdr.attr |= (word)(__msg->attr.urq() ? FIDO_UPDREQ : 0);
    strxcpy(__hdr.to, __msg->to, 36);
    strxcpy(__hdr.by, __msg->by, 36);
    strxcpy(__hdr.re, __msg->re, 72);
    __hdr.ftsc.origzone  = __msg->oorig.zone;
    __hdr.orignet        = __msg->oorig.net;
    __hdr.orignode       = __msg->oorig.node;
    __hdr.ftsc.origpoint = __msg->oorig.point;
    __hdr.ftsc.destzone  = __msg->odest.zone;
    __hdr.destnet        = __msg->odest.net;
    __hdr.destnode       = __msg->odest.node;
    __hdr.ftsc.destpoint = __msg->odest.point;
    __hdr.replyto        = (word)__msg->link.to();
    __hdr.reply1st       = (word)__msg->link.first();
    __hdr.cost           = (word)__msg->cost;
    __hdr.timesread      = (word)__msg->timesread;

    if(isopus())
    {
        __hdr.opus.written = TimeToFTime(__msg->written);
        __hdr.opus.arrived = TimeToFTime(__msg->arrived);
    }

    struct tm _tm;
    ggmtime(&_tm, &__msg->written);
    sprintf(__hdr.datetime,
            "%02d %3s %02d  %02d:%02d:%02d",
            _tm.tm_mday,
            gmonths[_tm.tm_mon + 1],
            _tm.tm_year % 100,
            _tm.tm_hour,
            _tm.tm_min,
            _tm.tm_sec);
    // Write message header
    write(_fh, &__hdr, sizeof(FidoHdr));

    // If message text is used
    if(__mode & GMSG_TXT)
    {
        // Write the message text
        write(_fh, __msg->txt, strlen(__msg->txt) + 1);
    }

    // Close the message file
    ::close(_fh);

    // Reset date/time of message file
    if(not (__mode & GMSG_NEW) and not __msg->attr.upd())
    {
        struct utimbuf t;
        t.actime  = st.st_atime;
        t.modtime = st.st_mtime;
        utime(_msgfile, &t);
    }

    // If the message has locked status, make it read-only
    if(__msg->attr.lok())
    {
        chmod(_msgfile, S_STDRD);
    }

    // Update internal array if the msg is new
    if(__mode & GMSG_NEW)
    {
        // Add the msgno to the index
        Msgn->Append(__msg->msgno);
    }

    GFTRK(0);
} // FidoArea::save_message

//  ------------------------------------------------------------------
void FidoArea::save_hdr(int __mode, gmsg * __msg)
{
    GFTRK("FidoSaveHdr");
    FidoHdr _hdr;
    save_message(__mode | GMSG_HDR, __msg, _hdr);
}

//  ------------------------------------------------------------------
void FidoArea::save_msg(int __mode, gmsg * __msg)
{
    GFTRK("FidoSaveMsg");
    FidoHdr _hdr;
    save_message(__mode | GMSG_HDRTXT, __msg, _hdr);
}

//  ------------------------------------------------------------------
void FidoArea::del_msg(gmsg * __msg)
{
    GFTRK("FidoDelMsg");
    FidoHdr _hdr;
    save_message(GMSG_HDR | GMSG_DELETE, __msg, _hdr);
}

//  ------------------------------------------------------------------
void FidoArea::new_msgno(gmsg * __msg)
{
    GFTRK("FidoNewMsgno");
    uint n = 0;
    Path _buf;

    do
    {
        __msg->msgno = Msgn->CvtReln(Msgn->Count()) + (++n);
    }
    while(fexist(build_msgname(_buf, __msg->msgno)));
    GFTRK(0);
}

//  ------------------------------------------------------------------
void FidoArea::update_timesread(gmsg * msg)
{
    GFTRK("FidoArea::update_timesread");
    Path filename;
    build_msgname(filename, msg->msgno);
    // If it has Lok attribute set, remove READ_ONLY attribute
    // and later add it again
    struct stat st;
    bool readonly = false;

    if(stat(filename, &st) == 0)
    {
        readonly = not (st.st_mode & S_IWUSR);

        if(readonly)
        {
            chmod(filename, S_STDRW);
        }
    }

    int fh = test_open(filename, O_RDWR | O_BINARY, WideSharemode);

    if(fh != -1)
    {
        FidoHdr hdr;
        ::read(fh, &hdr, sizeof(FidoHdr));
        hdr.timesread = (word)msg->timesread;
        ::lseekset(fh, 0);
        ::write(fh, &hdr, sizeof(FidoHdr));
        ::close(fh);
    }

    // If file was read_only before, make it again read_only
    if(readonly)
    {
        chmod(filename, S_STDRD);
    }

    GFTRK(0);
} // FidoArea::update_timesread

//  ------------------------------------------------------------------
