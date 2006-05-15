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
//  JAM msgbase implementation, load.
//  ------------------------------------------------------------------


//  ------------------------------------------------------------------

#include <stdlib.h>

#include <gmemdbg.h>
#include <gdbgtrk.h>
#include <gstrall.h>

#include <gmojamm.h>


//  ------------------------------------------------------------------

int JamArea::load_message(int __mode, gmsg* __msg, JamHdr& __hdr) {

  // Read index record for msg
  JamIndex _idx;
  memset(&_idx, 0, sizeof(JamIndex));
  lseekset(data->fhjdx, __msg->msgno-data->hdrinfo.basemsgnum, sizeof(JamIndex));
  read(data->fhjdx, &_idx, sizeof(JamIndex));

  if(_idx.hdroffset == 0xFFFFFFFFL) {
    GFTRK(0);
    return false;
  }

  // Read message header
  memset(&__hdr, 0, sizeof(JamHdr));
  lseekset(data->fhjhr, _idx.hdroffset);
  read(data->fhjhr, &__hdr, sizeof(JamHdr));

  if(strncmp(__hdr.signature, "JAM", 4) != 0) {
    WideLog->printf("! Invalid signature found in %s (msgno %d).", path(), __msg->msgno);
    WideLog->printf(": Info: Your msgbase is corrupted.");
    WideLog->printf("+ Advice: Run a msgbase index rebuild/recover utility.");

    GFTRK(0);
    return false;
  }

  __msg->link.to_set(__hdr.replyto);
  __msg->link.first_set(__hdr.reply1st);
  __msg->link.next_set(__hdr.replynext);

  __msg->cost = (uint)__hdr.cost;
  __msg->timesread = (uint)__hdr.timesread;

  __msg->written = __hdr.datewritten;
  __msg->arrived = __hdr.dateprocessed;
  __msg->received = __hdr.datereceived;

  __msg->attr.loc(__hdr.attribute & JAMATTR_LOCAL);
  __msg->attr.trs(__hdr.attribute & JAMATTR_INTRANSIT);
  __msg->attr.pvt(__hdr.attribute & JAMATTR_PRIVATE);
  __msg->attr.rcv(__hdr.attribute & JAMATTR_READ);
  __msg->attr.snt(__hdr.attribute & JAMATTR_SENT);
  __msg->attr.k_s(__hdr.attribute & JAMATTR_KILLSENT);
  __msg->attr.a_s(__hdr.attribute & JAMATTR_ARCHIVESENT);
  __msg->attr.hld(__hdr.attribute & JAMATTR_HOLD);
  __msg->attr.cra(__hdr.attribute & JAMATTR_CRASH);
  __msg->attr.imm(__hdr.attribute & JAMATTR_IMMEDIATE);
  __msg->attr.dir(__hdr.attribute & JAMATTR_DIRECT);
  __msg->attr.zon(__hdr.attribute & JAMATTR_GATE);
  __msg->attr.frq(__hdr.attribute & JAMATTR_FILEREQUEST);
  __msg->attr.att(__hdr.attribute & JAMATTR_FILEATTACH);
  __msg->attr.tfs(__hdr.attribute & JAMATTR_TRUNCFILE);
  __msg->attr.kfs(__hdr.attribute & JAMATTR_KILLFILE);
  __msg->attr.rrq(__hdr.attribute & JAMATTR_RECEIPTREQ);
  __msg->attr.cfm(__hdr.attribute & JAMATTR_CONFIRMREQ);
  __msg->attr.orp(__hdr.attribute & JAMATTR_ORPHAN);
  __msg->attr.lok(__hdr.attribute & JAMATTR_LOCKED);
  __msg->attr.del(__hdr.attribute & JAMATTR_DELETED);

  if(isnet() or isecho())
    __msg->attr.uns(__msg->attr.loc() and not __msg->attr.snt());
  else
    __msg->attr.uns0();

  __msg->txtstart = __hdr.offset;
  __msg->txtlength = __hdr.txtlen;

  // Allocate space for the subfields
  __msg->jam.subfieldlen = __hdr.subfieldlen;
  byte* _subfield = (byte*)throw_malloc((uint)(__hdr.subfieldlen+1));

  // Allocate space for kludge versions of the subfields
  char* _kludges = (char*)throw_malloc((uint)(__hdr.subfieldlen*2)+1);
  *_kludges = NUL;
  
  // Allocate space for seenby/paths
  char* _kludges2 = (char*)throw_malloc((uint)(__hdr.subfieldlen*2)+1);
  *_kludges2 = NUL;

  // Read the subfields
  read(data->fhjhr, _subfield, (uint)__hdr.subfieldlen);

  // Pointer to the subfields
  JamSubField* _subfieldptr = (JamSubField*)_subfield;

  // Process the subfields
  int _got_oaddr = false;
  int _got_daddr = false;
  uint _subfieldpos = 0;

  while(_subfieldpos < __hdr.subfieldlen) {

    _subfieldpos += sizeof(JamSubFieldHdr);
    uint _datlen = (uint)_subfieldptr->datlen;

    if(_subfieldpos > __hdr.subfieldlen)
      break;

    if((_subfieldpos + _datlen) > __hdr.subfieldlen)
      _datlen = (uint)(__hdr.subfieldlen - _subfieldpos);

    char* _buf = _subfieldptr->buffer;
    char _bufendchar = _buf[_datlen];
    _buf[_datlen] = NUL;

    char* _ptr;

    switch(_subfieldptr->loid) {

      case JAMSUB_OADDRESS:
        if(not _got_oaddr) {
          _got_oaddr = true;
          __msg->oorig.set(_buf, __msg->odom);
          __msg->orig = __msg->oorig;
          if(_got_daddr) {
            add_intl_topt_fmpt:
            if(isnet()) {
              sprintf(_kludges+strlen(_kludges), "\001INTL %u:%u/%u %u:%u/%u\r",
                __msg->dest.zone, __msg->dest.net, __msg->dest.node,
                __msg->orig.zone, __msg->orig.net, __msg->orig.node
              );
              if(__msg->dest.point)
                sprintf(_kludges+strlen(_kludges), "\001TOPT %u\r", __msg->dest.point);
              if(__msg->orig.point)
                sprintf(_kludges+strlen(_kludges), "\001FMPT %u\r", __msg->orig.point);
            }
          }
        }
        break;

      case JAMSUB_DADDRESS:
        if(not _got_daddr) {
          _got_daddr = true;
          __msg->odest.set(_buf, __msg->ddom);
          __msg->dest = __msg->odest;
          if(_got_oaddr)
            goto add_intl_topt_fmpt;
        }
        break;

      case JAMSUB_SENDERNAME:
        strxcpy(__msg->by, _buf, sizeof(__msg->by));
        break;

      case JAMSUB_RECEIVERNAME:
        strxcpy(__msg->to, _buf, sizeof(__msg->by));
        break;

      case JAMSUB_MSGID:
        sprintf(_kludges+strlen(_kludges), "\001MSGID: %s\r", _buf);
        strxcpy(__msg->msgids, _buf, sizeof(__msg->msgids));
        __msg->msgid.reset_fast();
        if(atoi(__msg->msgids)) {
          __msg->msgid.set(__msg->msgids, __msg->odom);
          if(__msg->msgid.net and (__msg->orig.net == 0))
            __msg->orig = __msg->msgid;
        }
        break;

      case JAMSUB_REPLYID:
        sprintf(_kludges+strlen(_kludges), "\001REPLY: %s\r", _buf);
        strxcpy(__msg->replys, _buf, sizeof(__msg->replys));
        break;

      case JAMSUB_SUBJECT:
        strxcpy(__msg->re, _buf, sizeof(__msg->re));
        break;

      case JAMSUB_PID:
        sprintf(_kludges+strlen(_kludges), "\001PID: %s\r", _buf);
        strxcpy(__msg->pid, _buf, sizeof(__msg->pid));
        break;

      case JAMSUB_TRACE:
        sprintf(_kludges2+strlen(_kludges2), "\001Via %s\r", _buf);
        // Not processed
        break;

      case JAMSUB_ENCLOSEDFILE:
        sprintf(_kludges+strlen(_kludges), "\001ENCLFILE: %s\r", _buf);
        // Not processed
        break;

      case JAMSUB_ENCLOSEDFILEWALIAS:
        sprintf(_kludges+strlen(_kludges), "\001ENCLFILEWALIAS: %s\r", _buf);
        // Not processed
        break;

      case JAMSUB_ENCLOSEDFREQ:
        sprintf(_kludges+strlen(_kludges), "\001ENCLFREQ: %s\r", _buf);
        // Not processed
        break;

      case JAMSUB_ENCLOSEDFILEWCARD:
        sprintf(_kludges+strlen(_kludges), "\001ENCLFILEWCARD: %s\r", _buf);
        // Not processed
        break;

      case JAMSUB_ENCLOSEDINDIRECFILE:
        sprintf(_kludges+strlen(_kludges), "\001ENCLINDIRFILE: %s\r", _buf);
        // Not processed
        break;

      case JAMSUB_EMBINDAT:
        // Not processed
        break;

      case JAMSUB_FTSKLUDGE:
        {
          char *_kludgesx;
          if(strneql(_buf, "Via ", 4) or strneql(_buf, "Recd", 4) or strneql(_buf, "Forwarded", 9))
            _kludgesx = _kludges2;
          else
            _kludgesx = _kludges;
          sprintf(_kludgesx+strlen(_kludgesx), "\001%s\r", _buf);
        }
        // Not processed
        break;

      case JAMSUB_SEENBY2D:
        _ptr = _buf;
        while(strlen(_ptr) > 70) {
          char* _tmp = _ptr;
          _ptr += 70;
          while(*_ptr != ' ')
            _ptr--;
          *_ptr++ = NUL;
          sprintf(_kludges2+strlen(_kludges2), "SEEN-BY: %s\r", _tmp);
        }
        sprintf(_kludges2+strlen(_kludges2), "SEEN-BY: %s\r", _ptr);
        // Not processed
        break;

        //           1         2         3         4         5         6         7         8
        //  12345678901234567890123456789012345678901234567890123456789012345678901234567890
        //  SEEN-BY: 123/456 123/456 123/456 123/456 123/456 123/456 123/456 123/456 23/456 123/456
        //           12345678901234567890123456789012345678901234567890123456789012345678901234567890
        //                    1         2         3         4         5         6         7         8

      case JAMSUB_PATH2D:
        _ptr = _buf;
        while(strlen(_ptr) > 72) {
          char* _tmp = _ptr;
          _ptr += 72;
          while(*_ptr != ' ')
            _ptr--;
          *_ptr++ = NUL;
          sprintf(_kludges2+strlen(_kludges2), "\001PATH: %s\r", _tmp);
        }
        sprintf(_kludges2+strlen(_kludges2), "\001PATH: %s\r", _ptr);
        // Not processed
        break;

      case JAMSUB_FLAGS:
        sprintf(_kludges+strlen(_kludges), "\001FLAGS %s\r", _buf);
        GetAttribstr(&__msg->attr, _buf);
        break;

      case JAMSUB_TZUTCINFO:
        sprintf(_kludges+strlen(_kludges), "\001TZUTC: %s\r", _buf);
        // Not processed
        break;
    }

    _buf[_datlen] = _bufendchar;

    _subfieldpos += _datlen;
    _subfieldptr = (JamSubField*)(_subfield + _subfieldpos);
  }

  // Free subfield buffer
  throw_free(_subfield);

  // Get reply numbers in chain
  if (wide->lookreplies and __msg->link.first())
  {
    int r = 0;
    uint32_t m = __msg->link.first();
    while (m)
    {
      JamHdr _rhdr;
      memset(&_rhdr, 0, sizeof(JamHdr));
      lseekset(data->fhjdx, m-data->hdrinfo.basemsgnum, sizeof(JamIndex));
      read(data->fhjdx, &_idx, sizeof(JamIndex));
      lseekset(data->fhjhr, _idx.hdroffset);
      read(data->fhjhr, &_rhdr, sizeof(JamHdr));
      m = _rhdr.replynext;
      if (m) __msg->link.list_set(r++, m);
    }
  }

  // If message text is used
  if(__mode & GMSG_TXT) {

    // Get size of kludges
    uint _kludgelen1 = strlen(_kludges);
    uint _kludgelen2 = strlen(_kludges2);
    uint _kludgelen = _kludgelen1 + _kludgelen2;
    uint32_t _msgsize = __hdr.txtlen;

    // Allocate memory for the message text
    __msg->txt = (char*)throw_realloc(_kludges, (uint)(_msgsize+_kludgelen+256));

    // Read the message text
    lseekset(data->fhjdt, __hdr.offset);
    read(data->fhjdt, __msg->txt+_kludgelen1, (uint)_msgsize);

    // Is there a CR at the end?
    {
      char* ptr = __msg->txt+_kludgelen1+_msgsize-1;
      if(*ptr != '\r') {
        if(*ptr) {
          ptr++;
          _msgsize++;
        }
        *ptr = '\r';
      }
    }

    memcpy(__msg->txt+_kludgelen1+(uint)_msgsize, _kludges2, _kludgelen2);
    __msg->txt[(uint)_msgsize+_kludgelen] = NUL;
  }
  else {

    // Free kludge buffer
    throw_free(_kludges);
  }

  throw_free(_kludges2);

  GFTRK(0);

  return true;
}


//  ------------------------------------------------------------------

int JamArea::load_hdr(gmsg* __msg) {

  GFTRK("JamArea::load_hdr");

  JamHdr _hdr;
  return load_message(GMSG_HDR, __msg, _hdr);
}


//  ------------------------------------------------------------------

int JamArea::load_msg(gmsg* __msg) {

  GFTRK("JamArea::load_msg");

  JamHdr _hdr;
  return load_message(GMSG_HDRTXT, __msg, _hdr);
}


//  ------------------------------------------------------------------

