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

#include <gmemdbg.h>
#include <gdbgtrk.h>
#include <gstrall.h>
#include <gutlmisc.h>

#include <gmopcbd.h>
//  ------------------------------------------------------------------
int PcbArea::load_message(int __mode, gmsg * __msg, PcbHdr & __hdr)
{
    // Read index record for msg
    PcbIdx _idx;

    lseekset(data->fhidx, (__msg->msgno - data->base.lowmsgno) * sizeof(PcbIdx));
    read(data->fhidx, &_idx, sizeof(PcbIdx));
    __msg->txtstart = _idx.offset;

    // Read message header
    lseekset(data->fhmsg, AbsV(_idx.offset));
    read(data->fhmsg, &__hdr, sizeof(PcbHdr));
    __msg->txtlength = (__hdr.blocks - 1) * 128;
    __msg->txtblocks = __hdr.blocks;
    __msg->timesread = 1;
    // Convert attributes
    __msg->pcboard.status = __hdr.status;

    if(not islocal())
    {
        __msg->attr.uns(__hdr.date[5] != '\xC4');
    }

    __msg->attr.pvt((__hdr.status == '*') or (__hdr.status == '+'));
    __msg->attr.rcv((__hdr.status == '+') or (__hdr.status == '-') or (__hdr.status ==
                                                                       '`') or (__hdr.
                                                                                status ==
                                                                                '^') or (
                        __hdr.status == '#'));
    __msg->attr.del(__hdr.activestatus == 226);
    __msg->pcboard.exthdrflags = __hdr.exthdrflags;
    // Convert msgno and replylinks
    __hdr.msgno = B2L(__hdr.msgno);
    __hdr.refno = B2L(__hdr.refno);
    __msg->link.to_set(__hdr.refno);
    // Convert date and time
    int _year, _month, _day, _hour, _minute;
    sscanf(__hdr.date, "%d%*c%d%*c%2d", &_month, &_day, &_year);
    sscanf(__hdr.time, "%d%*c%2d", &_hour, &_minute);
    struct tm _tm;
    _tm.tm_year  = (_year < 80) ? (_year + 100) : _year;
    _tm.tm_mon   = _month - 1;
    _tm.tm_mday  = _day;
    _tm.tm_hour  = _hour;
    _tm.tm_min   = _minute;
    _tm.tm_sec   = 0;
    _tm.tm_isdst = -1;
    time32_t a = gmktime(&_tm);
    struct tm tp;
    ggmtime(&tp, &a);
    tp.tm_isdst = -1;
    time32_t b = gmktime(&tp);
    __msg->written = a + a - b;
    __msg->arrived = 0;
    // Convert names and subject and password
    strtrim(strncpy(__msg->by, __hdr.origname, 25));
    strtrim(strncpy(__msg->to, __hdr.destname, 25));
    strtrim(strncpy(__msg->re, __hdr.subject, 25));
    strtrim(strncpy(__msg->pcboard.password, __hdr.password, 12));
    uint _msgsize = (uint)__msg->txtlength;
    // Allocate memory for the message text
    __msg->txt = (char *)throw_realloc(__msg->txt, _msgsize + 256);
    char * _tmptxt = (char *)throw_calloc(1, _msgsize + 1);
    // Read the message text, trim spaces and translate PCB linefeeds
    read(data->fhmsg, _tmptxt, _msgsize);
    strtrim(_tmptxt);

    if(not wide->foreign)
    {
        strchg(_tmptxt, 0xE3, 0x0D);
    }

    //int _line = 0;
    char _eby[122] = {""};
    char _eto[122] = {""};
    char * _dst    = __msg->txt;
    char * _src    = _tmptxt;

    while(*_src)
    {
        //char* _begline = _src;
        // Check for extended header
        PcbExtHdr * _ehdr = (PcbExtHdr *)_src;

        if(_ehdr->id == 0x40FF)
        {
            if(strnieql(_ehdr->function, "TO     ", 7))
            {
                strncpy(_eto, _ehdr->desc, 60);
            }
            else if(strnieql(_ehdr->function, "TO2    ", 7))
            {
                strncpy(_eto + 60, _ehdr->desc, 60);
            }
            else if(strnieql(_ehdr->function, "FROM   ", 7))
            {
                strncpy(_eby, _ehdr->desc, 60);
            }
            else if(strnieql(_ehdr->function, "FROM2  ", 7))
            {
                strncpy(_eby + 60, _ehdr->desc, 60);
            }
            else if(strnieql(_ehdr->function, "SUBJECT", 7))
            {
                strtrim(strncpy(__msg->re, _ehdr->desc, 60));
            }

            _src += sizeof(PcbExtHdr);
        }
        else
        {
            while(*_src)
            {
                *_dst++ = *_src;

                if(*_src++ == CR)
                {
          #if 0

                    // Get address and attribute from FidoPCB
                    if(isnet())
                    {
                        _line++;

                        if((_line == 1) and (*_begline == '(' /*)*/))
                        {
                            Addr _addr;
                            _addr.ResetFast();
                            char * _ptr   = _begline + 1;
                            int _incoming = false;

                            if(strnieql(_ptr, "FROM:", 5))
                            {
                                _incoming = true;
                                _ptr     += 5;
                            }

                            _addr.set(strskip_wht(_ptr));

                            if(not _incoming and strieql(__msg->by, WideUsername[0]))
                            {
                                __msg->dest  = _addr;
                                __msg->odest = _addr;
                            }
                            else
                            {
                                __msg->orig  = _addr;
                                __msg->oorig = _addr;
                            }
                        }
                        else if((_line == 2) and (*_begline == '(' /*)*/))
                        {
                            _src[-1] = NUL;

                            if(striinc("HOLD", _begline))
                            {
                                __msg->attr.hld1();
                            }

                            if(striinc("IMM", _begline))
                            {
                                __msg->attr.imm1();
                            }

                            if(striinc("CRASH", _begline))
                            {
                                __msg->attr.cra1();
                            }

                            if(striinc("INTL", _begline))
                            {
                                __msg->attr.zon1();
                            }

                            _src[-1] = CR;
                        }
                    }

          #endif // if 0
                    break;
                }
            }
        }
    }
    *_dst = NUL;
    strtrim(_eby);

    if(*_eby)
    {
        strxcpy(__msg->by, _eby, sizeof(__msg->by));
    }

    strtrim(_eto);

    if(*_eto)
    {
        strxcpy(__msg->to, _eto, sizeof(__msg->to));
    }

    if(isnet() and not isemail())
    {
        char * ptr = strchr(__msg->by, '@');

        if(ptr)
        {
            *ptr++ = NUL;
            __msg->orig.reset(ptr);
            __msg->oorig = __msg->orig;
        }

        ptr = strchr(__msg->to, '@');

        if(ptr)
        {
            *ptr++ = NUL;
            char * ptr2 = strskip_txt(ptr);

            if(*ptr2)
            {
                *ptr2++ = NUL;

                while(*ptr2)
                {
                    if(*ptr2 == '+')
                    {
                        // get flags
                        switch(g_toupper(*(++ptr2)))
                        {
                            case 'C':
                                __msg->attr.cra1();
                                break;

                            case 'D':
                                __msg->attr.dir1();
                                break;
                        }
                    }
                    else if(*ptr2 == '[')
                    {
                        // get from-aka address
                        ptr2++;
                        char * ptr3 = ptr2;
                        ptr2 = strskip_to(ptr2, ']');

                        if(*ptr2)
                        {
                            *ptr2++ = NUL;
                            __msg->orig.reset(ptr3);
                            __msg->oorig = __msg->orig;
                        }
                    }

                    ptr2++;
                }
            }

            __msg->dest.reset(ptr);
            __msg->odest = __msg->dest;
        }
    }

    throw_free(_tmptxt);

    if(not (__mode & GMSG_TXT))
    {
        throw_release(__msg->txt);
    }

    GFTRK(0);
    return true;
} // PcbArea::load_message

// ------------------------------------------------------------------
int PcbArea::load_hdr(gmsg * __msg)
{
    GFTRK("PcbLoadHdr");
    PcbHdr _hdr;
    return load_message(GMSG_HDR, __msg, _hdr);
}

// ------------------------------------------------------------------
int PcbArea::load_msg(gmsg * __msg)
{
    GFTRK("PcbLoadMsg");
    PcbHdr _hdr;
    return load_message(GMSG_HDRTXT, __msg, _hdr);
}

// ------------------------------------------------------------------
