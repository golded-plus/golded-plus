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
//  JAM msgbase implementation, save/delete.
//  ------------------------------------------------------------------

#include <gmemdbg.h>
#include <gdbgtrk.h>
#include <gdbgerr.h>
#include <gcrcall.h>
#include <gstrall.h>
#include <gtxtpara.h>
#include <gutlmisc.h>
#include <gmojamm.h>


//  ------------------------------------------------------------------

void JamArea::lock() {

  GFTRK("JamArea::lock");

  if(NOT data->islocked AND WideCanLock) {

    long _tries = 0;

    // Try to get the lock
    while(::lock(data->fhjhr, 0, 1) == -1) {

      // Tell the world
      if(PopupLocked(++_tries, true, path()) == false) {
        WideLog->ErrLock();
        raw_close();
        WideLog->printf("! A JAM msgbase file could not be locked.");
        WideLog->printf(": %s.JHR.", path());
        WideLog->ErrOSInfo();
        LockErrorExit();
      }
    }

    // Remove the popup window
    if(_tries)
      PopupLocked(0, 0, NULL);

    data->islocked = true;
  }

  // Load fresh copy of the header info
  lseekset(data->fhjhr, 0);
  read(data->fhjhr, &data->hdrinfo, sizeof(JamHdrInfo));

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void JamArea::unlock() {

  GFTRK("JamArea::unlock");

  if(data->islocked) {
    ::unlock(data->fhjhr, 0, 1);
    data->islocked = false;
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void JamArea::add_subfield(JamHdr& __hdr, byte*& __subfield, word __loid, word __hiid, char* __data) {

  uint _datlen = strlen(__data);
  __subfield = (byte*)throw_realloc(__subfield, (uint)__hdr.subfieldlen+sizeof(JamSubFieldHdr)+_datlen);
  JamSubField* _subfieldptr = (JamSubField*)(__subfield + (uint)__hdr.subfieldlen);
  _subfieldptr->loid = __loid;
  _subfieldptr->hiid = __hiid;
  _subfieldptr->datlen = _datlen;
  memcpy(_subfieldptr->buffer, __data, _datlen);
  __hdr.subfieldlen += sizeof(JamSubFieldHdr) + _datlen;
}


//  ------------------------------------------------------------------

void JamArea::save_message(int __mode, gmsg* __msg, JamHdr& __hdr) {

  int _was_locked = data->islocked;
  if(NOT _was_locked)
    lock();

  // Reset header
  memset(&__hdr, 0, sizeof(JamHdr));

  // Init paragraph class
  GParagraph _para;
  if(WideDispsoftcr)
    _para.softcr = SOFTCR;

  // Set default msg text offset and length
  __hdr.offset = __msg->txtstart;
  __hdr.txtlen = __msg->txtlength;

  // Get message text size
  char* _txtcpy = NULL;
  if(__msg->txt AND (__mode & GMSG_TXT)) {

    // Work on a copy of the original msg text
    _txtcpy = throw_strdup(__msg->txt);

    // Convert message text to a paragraph list
    _para.ConvertText(_txtcpy, strlen(_txtcpy));

    // Mark control lines and calculate pure message text size
    __hdr.txtlen = _para.CheckCtrlLines();
  }

  // Determine where to write the message text
  if((__mode & GMSG_NEW) or (__hdr.txtlen > __msg->txtlength))
    __hdr.offset = filelength(data->fhjdt);

  byte* _subfield = NULL;
  char _buf[1024];
  __hdr.subfieldlen = __msg->jam.subfieldlen;
  
  if(__mode & GMSG_TXT) {

    // Build the subfields
    __hdr.subfieldlen = 0;

    // Process addressing kludges
    int _line = 0;
    GParaData* _pdptr = _para.paraidx;
    while(_line < _para.lines) {
      if(_pdptr->control > CTRL_KLUDGE) {
        word fmpt, topt;
        char buf1[201], buf2[201];
        switch(_pdptr->control) {
          case CTRL_INTL:
            fmpt = __msg->orig.point;
            topt = __msg->dest.point;
            sscanf(_pdptr->text+6, "%s %s", buf1, buf2);
            __msg->dest.set(buf1);
            __msg->orig.set(buf2);
            __msg->orig.point = fmpt;
            __msg->dest.point = topt;
            break;
          case CTRL_FMPT:
            __msg->orig.point = atow(_pdptr->text+6);
            break;
          case CTRL_TOPT:
            __msg->dest.point = atow(_pdptr->text+6);
            break;
          case CTRL_MSGID:
            __msg->msgid.reset(_pdptr->text+8, __msg->odom);
            break;
        }
      }
      _pdptr++;
      _line++;
    }

    if(*__msg->by)
      add_subfield(__hdr, _subfield, JAMSUB_SENDERNAME, 0, __msg->by);

    if(__msg->orig.net) {
      __msg->orig.make_string(_buf, __msg->odom);
      add_subfield(__hdr, _subfield, JAMSUB_OADDRESS, 0, _buf);
    }

    if(*__msg->to)
      add_subfield(__hdr, _subfield, JAMSUB_RECEIVERNAME, 0, __msg->to);

    if(__msg->dest.net) {
      __msg->dest.make_string(_buf, __msg->ddom);
      add_subfield(__hdr, _subfield, JAMSUB_DADDRESS, 0, _buf);
    }

    if(*__msg->re)
      add_subfield(__hdr, _subfield, JAMSUB_SUBJECT, 0, __msg->re);

    // Convert kludges
    _line = 0;
    _pdptr = _para.paraidx;
    while(_line < _para.lines) {
      if(_pdptr->control > CTRL_KLUDGE) {
        uint _offset = 0;
        word _loid = 0;
        switch(_pdptr->control) {
          case CTRL_INTL:
          case CTRL_FMPT:
          case CTRL_TOPT:
            // Strip these. Data is stored in the address subfields
            break;
          case CTRL_MSGID:
            _loid = JAMSUB_MSGID;
            _offset = 8;
            strxcpy(__msg->msgids, _pdptr->text+_offset, sizeof(__msg->msgids));
            break;
          case CTRL_REPLY:
            _loid = JAMSUB_REPLYID;
            _offset = 8;
            strxcpy(__msg->replys, _pdptr->text+_offset, sizeof(__msg->replys));
            break;
          case CTRL_PID:
            _loid = JAMSUB_PID;
            _offset = 6;
            break;
          case CTRL_VIA:
            _loid = JAMSUB_TRACE;
            _offset = 6;
            break;
          case CTRL_SEENBY:
            _loid = JAMSUB_SEENBY2D;
            _offset = 9;
            break;
          case CTRL_SEENBY1:
            _loid = JAMSUB_SEENBY2D;
            _offset = 10;
            break;
          case CTRL_PATH:
            _loid = JAMSUB_PATH2D;
            _offset = 7;
            break;
          case CTRL_FLAGS:
            _loid = JAMSUB_FLAGS;
            _offset = 7;
            break;
          default:
            _loid = JAMSUB_FTSKLUDGE;
            _offset = 1;
        }
        if(_offset)
          add_subfield(__hdr, _subfield, _loid, 0, _pdptr->text+_offset);
      }
      _pdptr++;
      _line++;
    }
  }

  JamIndex _idx;
  bool was_deleted = false;
  if(__mode & GMSG_NEW) {
    __msg->msgno = data->hdrinfo.basemsgnum + (filelength(data->fhjdx)/sizeof(JamIndex));
    __hdr.messagenumber = __msg->msgno;
    _idx.hdroffset = filelength(data->fhjhr);
  }
  else {
    __hdr.messagenumber = __msg->msgno;
    lseekset(data->fhjdx, __hdr.messagenumber-data->hdrinfo.basemsgnum, sizeof(JamIndex));
    read(data->fhjdx, &_idx, sizeof(JamIndex));
    if(_idx.hdroffset != 0xFFFFFFFFL) {
      lseekset(data->fhjhr, _idx.hdroffset);
      JamHdr oldhdr;
      read(data->fhjhr, &oldhdr, sizeof(JamHdr));
      was_deleted = oldhdr.attribute & JAMATTR_DELETED ? true : false;
      if(oldhdr.subfieldlen != __hdr.subfieldlen) {
        oldhdr.attribute |= JAMATTR_DELETED;
        oldhdr.txtlen = 0;
        lseekset(data->fhjhr, _idx.hdroffset);
        write(data->fhjhr, &oldhdr, sizeof(JamHdr));
        _idx.hdroffset = filelength(data->fhjhr);
      }
    }
  }

  if(_idx.hdroffset != 0xFFFFFFFFL) {

    memcpy(__hdr.signature, JAM_SIGNATURE, 4);
    __hdr.revision = JAM_REVISION;

    __hdr.replyto   = __msg->link.to();
    __hdr.reply1st  = __msg->link.first();
    __hdr.replynext = __msg->link.next();

    __hdr.cost      = __msg->cost;
    __hdr.timesread = __msg->timesread;
    __hdr.passwordcrc = 0xFFFFFFFFL;

    __hdr.msgidcrc = strCrc32(strlwr(strcpy(_buf, __msg->msgids)), NO, CRC32_MASK_CCITT);
    __hdr.replycrc = strCrc32(strlwr(strcpy(_buf, __msg->replys)), NO, CRC32_MASK_CCITT);
    _idx.usercrc   = strCrc32(strlwr(strcpy(_buf, __msg->to)),     NO, CRC32_MASK_CCITT);

    __hdr.datewritten   = __msg->written;
    __hdr.datereceived  = __msg->received;
    __hdr.dateprocessed = __msg->arrived;

    __hdr.attribute |= __msg->attr.loc() ? JAMATTR_LOCAL       : 0;
    __hdr.attribute |= __msg->attr.trs() ? JAMATTR_INTRANSIT   : 0;
    __hdr.attribute |= __msg->attr.pvt() ? JAMATTR_PRIVATE     : 0;
    __hdr.attribute |= __msg->attr.rcv() ? JAMATTR_READ        : 0;
    __hdr.attribute |= __msg->attr.snt() ? JAMATTR_SENT        : 0;
    __hdr.attribute |= __msg->attr.k_s() ? JAMATTR_KILLSENT    : 0;
    __hdr.attribute |= __msg->attr.a_s() ? JAMATTR_ARCHIVESENT : 0;
    __hdr.attribute |= __msg->attr.hld() ? JAMATTR_HOLD        : 0;
    __hdr.attribute |= __msg->attr.cra() ? JAMATTR_CRASH       : 0;
    __hdr.attribute |= __msg->attr.imm() ? JAMATTR_IMMEDIATE   : 0;
    __hdr.attribute |= __msg->attr.dir() ? JAMATTR_DIRECT      : 0;
    __hdr.attribute |= __msg->attr.zon() ? JAMATTR_GATE        : 0;
    __hdr.attribute |= __msg->attr.frq() ? JAMATTR_FILEREQUEST : 0;
    __hdr.attribute |= __msg->attr.att() ? JAMATTR_FILEATTACH  : 0;
    __hdr.attribute |= __msg->attr.tfs() ? JAMATTR_TRUNCFILE   : 0;
    __hdr.attribute |= __msg->attr.kfs() ? JAMATTR_KILLFILE    : 0;
    __hdr.attribute |= __msg->attr.rrq() ? JAMATTR_RECEIPTREQ  : 0;
    __hdr.attribute |= __msg->attr.cfm() ? JAMATTR_CONFIRMREQ  : 0;
    __hdr.attribute |= __msg->attr.orp() ? JAMATTR_ORPHAN      : 0;
    __hdr.attribute |= __msg->attr.lok() ? JAMATTR_LOCKED      : 0;
    __hdr.attribute |= __msg->attr.del() ? JAMATTR_DELETED     : 0;

    if(isnet())
      __hdr.attribute |= JAMATTR_TYPENET;
    else if(isecho())
      __hdr.attribute |= JAMATTR_TYPEECHO;
    else
      __hdr.attribute |= JAMATTR_TYPELOCAL;

    lseekset(data->fhjhr, _idx.hdroffset);
    write(data->fhjhr, &__hdr, sizeof(JamHdr));

    // Only write subfields if the msg body is written
    if(__mode & GMSG_TXT)
      write(data->fhjhr, _subfield, (uint)__hdr.subfieldlen);

    // Delete msg if requested
    if(__mode & GMSG_DELETE) {
      if(NOT was_deleted)
        data->hdrinfo.activemsgs--;
      __hdr.attribute |= JAMATTR_DELETED;
      _idx.usercrc = 0xFFFFFFFFL;
      if(wide->harddelete)
        _idx.hdroffset = 0xFFFFFFFFL;
    }

    // Write msg index
    lseekset(data->fhjdx, __hdr.messagenumber-data->hdrinfo.basemsgnum, sizeof(JamIndex));
    write(data->fhjdx, &_idx, sizeof(JamIndex));

    // Free subfield buffer
    throw_release(_subfield);

    // Update the header info
    if(__mode & GMSG_NEW)
      data->hdrinfo.activemsgs++;
    data->hdrinfo.modcounter++;
    lseekset(data->fhjhr, 0);
    write(data->fhjhr, &data->hdrinfo, sizeof(JamHdrInfo));

    // If message text is used
    if(__mode & GMSG_TXT) {

      char* _txt = (char*)throw_malloc((uint)(__hdr.txtlen+256));

      // Copy text paragraphs, excluding kludges
      int _line = 0;
      char* _tptr = _txt;
      GParaData* _pdptr = _para.paraidx;
      while(_line < _para.lines) {
        if(_pdptr->control < CTRL_KLUDGE) {
          memcpy(_tptr, _pdptr->text, _pdptr->length);
          _tptr += _pdptr->length;
          *_tptr++ = CR;
        }
        _pdptr++;
        _line++;
      }
      *_tptr = NUL;

      // Seek to start position of the message text
      lseekset(data->fhjdt, __hdr.offset);

      // Write the message text
      write(data->fhjdt, _txt, (uint)__hdr.txtlen);

      // Release the memory we have used
      throw_free(_txtcpy);
      throw_free(_txt);
    }

    // Update internals if new
    if(__mode & GMSG_NEW) {

      // Count our msgs
      data->timesposted++;

      // Update internal array
      Msgn->Append(__msg->msgno);
    }
  }
  else {
    scan();
  }

  if(NOT _was_locked) {
    lseekset(data->fhjhr, 0);
    write(data->fhjhr, &data->hdrinfo, sizeof(JamHdrInfo));
    unlock();
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void JamArea::save_hdr(int __mode, gmsg* __msg) {

  GFTRK("JamArea::save_hdr");

  JamHdr _hdr;
  save_message(__mode | GMSG_HDR, __msg, _hdr);
}


//  ------------------------------------------------------------------

void JamArea::save_msg(int __mode, gmsg* __msg) {

  GFTRK("JamArea::save_msg");

  JamHdr _hdr;
  save_message(__mode | GMSG_HDRTXT, __msg, _hdr);
}


//  ------------------------------------------------------------------

void JamArea::del_msg(gmsg* __msg) {

  GFTRK("JamArea::del_msg");

  JamHdr _hdr;
  save_message(GMSG_HDR | GMSG_DELETE, __msg, _hdr);
}


//  ------------------------------------------------------------------

void JamArea::new_msgno(gmsg* __msg) {

  GFTRK("JamArea::new_msgno");

  __msg->msgno = data->hdrinfo.basemsgnum + (filelength(data->fhjdx)/sizeof(JamIndex));

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void JamArea::update_timesread(gmsg* msg) {

  GFTRK("JamArea::update_timesread");

  lock();

  lseekset(data->fhjdx, msg->msgno-data->hdrinfo.basemsgnum, sizeof(JamIndex));

  JamIndex idx;
  read(data->fhjdx, &idx, sizeof(JamIndex));

  JamHdr hdr;
  lseekset(data->fhjhr, idx.hdroffset);
  read(data->fhjhr, &hdr, sizeof(JamHdr));

  hdr.timesread = msg->timesread;

  lseekset(data->fhjhr, idx.hdroffset);
  write(data->fhjhr, &hdr, sizeof(JamHdr));

  data->hdrinfo.modcounter++;
  lseekset(data->fhjhr, 0);
  write(data->fhjhr, &data->hdrinfo, sizeof(JamHdrInfo));

  unlock();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

