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
//  JAM msgbase implementation, utilities.
//  ------------------------------------------------------------------


//  ------------------------------------------------------------------

#include <gmemdbg.h>
#include <gdbgtrk.h>
#include <gutlmisc.h>

#include <gmojamm.h>


//  ------------------------------------------------------------------

char* JamArea::user_lookup(char* lookfor)
{

    NW(lookfor);
    return NULL;
}


//  ------------------------------------------------------------------

int JamArea::renumber()
{

    return false;
}


//  ------------------------------------------------------------------

Line* JamArea::make_dump_msg(Line*& lin, gmsg* __msg, char* lng_head)
{

    GFTRK("JamArea::make_dump_msg");

    // Read base header
    JamHdrInfo* _base = (JamHdrInfo*)throw_calloc(1, sizeof(JamHdrInfo));
    lseekset(data->fhjhr, 0);
    read(data->fhjhr, _base, sizeof(JamHdrInfo));

    // Read index record for msg
    JamIndex _idx;
    lseekset(data->fhjdx, __msg->msgno-data->hdrinfo.basemsgnum, sizeof(JamIndex));
    read(data->fhjdx, &_idx, sizeof(JamIndex));

    // Allocate space for the header
    JamHdr* _hdr = (JamHdr*)throw_calloc(1, (uint)(sizeof(JamHdr)+17));

    // Read message header
    lseekset(data->fhjhr, _idx.hdroffset);
    read(data->fhjhr, _hdr, sizeof(JamHdr));

    // Allocate space for the subfields
    byte* _subfield = (byte*)throw_calloc(1, (uint)(_hdr->subfieldlen+17));

    // Read the subfields
    read(data->fhjhr, _subfield, (uint)_hdr->subfieldlen);

    // Allocate memory for the raw message text
    throw_free(__msg->txt);
    __msg->txt = (char*)throw_calloc(1, (uint)_hdr->txtlen+16);

    // Read the message text
    lseekset(data->fhjdt, _hdr->offset);
    read(data->fhjdt, __msg->txt, (uint)_hdr->txtlen);

    char buf[100];
    Line* line = lin =
                     AddLine (NULL, "Hexdump of JAM message header, subfields and text");
    AddLineF(line, "------------------------------------------------------------------------------");
    line = AddLine(line, "");
    AddLineF(line, "Msgbase       : %s",     real_path());
    AddLineF(line, "Signature     : %s",     _hdr->signature);
    AddLineF(line, "Revision      : %u",     _hdr->revision);
    AddLineF(line, "ReservedWord  : %u",     _hdr->reservedword);
    AddLineF(line, "SubfieldLen   : %u",    _hdr->subfieldlen);
    AddLineF(line, "TimesRead     : %u",    _hdr->timesread);
    AddLineF(line, "MSGIDcrc      : %08Xh", _hdr->msgidcrc);
    AddLineF(line, "REPLYcrc      : %08Xh", _hdr->replycrc);
    AddLineF(line, "ReplyTo       : %u",    _hdr->replyto);
    AddLineF(line, "Reply1st      : %u",    _hdr->reply1st);
    AddLineF(line, "ReplyNext     : %u",    _hdr->replynext);
    AddLineF(line, "DateWritten   : %s (%08Xh)", TimeToStr(buf, _hdr->datewritten), (int32_t)_hdr->datewritten);
    AddLineF(line, "DateReceived  : %s (%08Xh)", TimeToStr(buf, _hdr->datereceived), (int32_t)_hdr->datereceived);
    AddLineF(line, "DateProcessed : %s (%08Xh)", TimeToStr(buf, _hdr->dateprocessed), (int32_t)_hdr->dateprocessed);
    AddLineF(line, "MessageNumber : %u",    _hdr->messagenumber);
    AddLineF(line, "Attribute     : %08Xh (%sb)", _hdr->attribute,  ltob(buf, _hdr->attribute, 0));
    AddLineF(line, "Attribute2    : %08Xh (%sb)", _hdr->attribute2, ltob(buf, _hdr->attribute2, 0));
    AddLineF(line, "Offset        : %u",    _hdr->offset);
    AddLineF(line, "TxtLen        : %u",    _hdr->txtlen);
    AddLineF(line, "PasswordCRC   : %08Xh", _hdr->passwordcrc);
    AddLineF(line, "Cost          : %u",    _hdr->cost);
    line = AddLine(line, "");
    AddLineF(line, "Index Record:");
    line = AddLine(line, "");
    AddLineF(line, "UserCrc       : %08Xh", _idx.usercrc);
    AddLineF(line, "HeaderOffset  : %08Xh (%u)", _idx.hdroffset, _idx.hdroffset);
    line = AddLine(line, "");
    AddLineF(line, "Lastread Record:");
    line = AddLine(line, "");
    AddLineF(line, "Index         : %u",    data->lastpos);
    AddLineF(line, "UserCrc       : %08Xh", data->lastrec.usercrc);
    AddLineF(line, "UserId        : %08Xh", data->lastrec.userid);
    AddLineF(line, "Lastread      : %u",    data->lastrec.lastread);
    AddLineF(line, "Highread      : %u",    data->lastrec.highread);
    line = AddLine(line, "");
    AddLineF(line, "Base Header:");
    line = AddLine(line, "");
    AddLineF(line, "DateCreated   : %s (%08Xh)", TimeToStr(buf, _base->datecreated), (int32_t)_base->datecreated);
    AddLineF(line, "ModCounter    : %u",    _base->modcounter);
    AddLineF(line, "ActiveMsgs    : %u",    _base->activemsgs);
    AddLineF(line, "PasswordCRC   : %08Xh", _base->passwordcrc);
    AddLineF(line, "BaseMsgNum    : %u",    _base->basemsgnum);
    if(jamwide->smapihw)
        AddLineF(line, "HighWaterMark : %u",    _base->highwatermark);
    else if(data->highwater != -1)
        AddLineF(line, "HighWaterMark : %u",    data->highwater);
    else
        AddLineF(line, "HighWaterMark : unknown");
    line = AddLine(line, "");
    AddLineF(line, "Subfields:");
    line = AddLine(line, "");

    AddHexdump(line, _subfield, (uint)_hdr->subfieldlen);
    line = AddLine(line, "");

    // Process the subfields
    JamSubField* _subfieldptr = (JamSubField*)_subfield;
    uint _subfieldpos = 0;
    while(_subfieldpos < _hdr->subfieldlen)
    {
        _subfieldpos += sizeof(JamSubFieldHdr);
        uint _datlen = (uint)_subfieldptr->datlen;
        if(_subfieldpos > _hdr->subfieldlen)
        {
            AddLineF(line, "Error: SubfieldHdr at pos %u exceeds SubfieldLen (%u)!", (uint)(_subfieldpos-sizeof(JamSubFieldHdr)), _hdr->subfieldlen);
            break;
        }
        if((_subfieldpos + _datlen) > _hdr->subfieldlen)
        {
            _datlen = (uint)(_hdr->subfieldlen - _subfieldpos);
            AddLineF(line, "Error: SubfieldData at pos %u is too int32_t (%u)!  Cut to %u.", _subfieldpos, (uint)_subfieldptr->datlen, _datlen);
        }
        AddLineF(line, "%05u [%3u]: \"%-*.*s\"",
                 _subfieldptr->loid, _datlen,
                 MinV(60, (int)_datlen),
                 MinV(60, (int)_datlen),
                 _subfieldptr->buffer
                );
        _subfieldpos += _datlen;
        _subfieldptr = (JamSubField*)(_subfield + _subfieldpos);
    }

    // Free subfield buffer
    throw_free(_subfield);

    line = AddLine(line, "");
    AddLine(line, lng_head);
    line = AddLine(line, "");

    AddHexdump(line, _hdr, sizeof(JamHdr));

    // Free header and base
    throw_free(_hdr);
    throw_free(_base);

    GFTRK(0);

    return line;
}


//  ------------------------------------------------------------------

