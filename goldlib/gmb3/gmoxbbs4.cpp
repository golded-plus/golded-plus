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

#include <gdbgerr.h>
#include <gmemdbg.h>
#include <gdbgtrk.h>
#include <gstrall.h>
#include <gcrcall.h>
#include <gutlmisc.h>

#include <gmoxbbs.h>


//  ------------------------------------------------------------------

void XbbsArea::lock() {

  // Not applicable in AdeptXBBS
}


//  ------------------------------------------------------------------

void XbbsArea::unlock() {

  // Not applicable in AdeptXBBS
}


//  ------------------------------------------------------------------

void XbbsArea::lock_file(int handle, int32_t position, int32_t length) {

  GFTRK("XbbsLockFile");

  long tries = 0;
  while(::lock(handle, position, length) == -1) {
    if(PopupLocked(++tries, true, real_path()) == false) {
      WideLog->ErrLock();
      raw_close();
      Path file;
      strcpy(file, real_path());
      if(handle == data->fhdata)
        strcat(file, ".Data");
      else if(handle == data->fhtext)
        strcat(file, ".Text");
      else if(handle == data->fhindex)
        strcat(file, ".Index");
      else if(handle == wide->fhpmi)
        sprintf(file, "%sPersonal_Mail\\%s", wide->path, WideUsername[0]);
      WideLog->printf("! An AdeptXBBS msgbase file could not be locked.");
      WideLog->printf(": %s.", file);
      WideLog->ErrOSInfo();
      LockErrorExit();
    }
  }
  if(tries)
    PopupLocked(0, 0, NULL);

  GFTRK(0);
}


//  ------------------------------------------------------------------

void XbbsArea::unlock_file(int handle, int32_t position, int32_t length) {

  GFTRK("XbbsUnlockFile");

  ::unlock(handle, position, length);

  GFTRK(0);
}


//  ------------------------------------------------------------------

uint16_t XbbsCheckSum(char* str) {

  uint16_t checksum = 0;

  while(*str) {
    checksum ^= (uint16_t)g_toupper(*str++);
    if(checksum & 1)
      checksum = (uint16_t)((checksum >> 1) ^ 0xA000);
    else
      checksum >>= 1;
  }

  return checksum;
}


//  ------------------------------------------------------------------

void XbbsArea::save_message(int __mode, gmsg* __msg, XbbsHdr& __hdr) {

  // Reset header
  memset(&__hdr, 0, sizeof(XbbsHdr));

  if(__mode & GMSG_NEW) {
    data->idx_size++;
    __msg->msgno = data->idx_size;
    Msgn->Append(__msg->msgno);
    data->idx = (XbbsIdx*)throw_realloc(data->idx, data->idx_size*sizeof(XbbsIdx));
  }

  strxcpy(__hdr.from, __msg->by, sizeof(__hdr.from));
  strxcpy(__hdr.to,   __msg->to, sizeof(__hdr.to));
  strxcpy(__hdr.subj, __msg->re, sizeof(__hdr.subj));

  struct tm _tm; ggmtime(&_tm, &__msg->written);
  sprintf(__hdr.date, "%02d %3s %02d  %02d:%02d:%02d",
      _tm.tm_mday, gmonths[_tm.tm_mon + 1], _tm.tm_year % 100,
      _tm.tm_hour, _tm.tm_min, _tm.tm_sec
    );

  if (__msg->arrived) ggmtime(&_tm, &__msg->arrived);

  __hdr.indate[0] = (byte)(_tm.tm_year - 89);
  __hdr.indate[1] = (byte)(_tm.tm_mon + 1);
  __hdr.indate[2] = (byte)(_tm.tm_mday);
  __hdr.indate[3] = 0;

  __hdr.msgnum = __msg->msgno;
  __hdr.timesread = __msg->timesread;
  __hdr.timerecv = __msg->received;
  __hdr.origaddr = __msg->oorig;
  __hdr.destaddr = __msg->odest;
  __hdr.cost = (uint16_t)__msg->cost;

  // Transfer attributes
  __hdr.fflags |= (uint16_t)(__msg->attr.pvt() ? FFLAGS_MSGPRIVATE  : 0);
  __hdr.fflags |= (uint16_t)(__msg->attr.cra() ? FFLAGS_MSGCRASH    : 0);
  __hdr.fflags |= (uint16_t)(__msg->attr.rcv() ? FFLAGS_MSGREAD     : 0);
  __hdr.fflags |= (uint16_t)(__msg->attr.snt() ? FFLAGS_MSGSENT     : 0);
  __hdr.fflags |= (uint16_t)(__msg->attr.att() ? FFLAGS_MSGFILE     : 0);
  __hdr.fflags |= (uint16_t)(__msg->attr.trs() ? FFLAGS_MSGFWD      : 0);
  __hdr.fflags |= (uint16_t)(__msg->attr.orp() ? FFLAGS_MSGORPHAN   : 0);
  __hdr.fflags |= (uint16_t)(__msg->attr.k_s() ? FFLAGS_MSGKILL     : 0);
  __hdr.fflags |= (uint16_t)(__msg->attr.loc() ? FFLAGS_MSGLOCAL    : 0);
  __hdr.fflags |= (uint16_t)(__msg->attr.rsv() ? FFLAGS_MSGXX2      : 0);
  __hdr.fflags |= (uint16_t)(__msg->attr.frq() ? FFLAGS_MSGFRQ      : 0);
  __hdr.fflags |= (uint16_t)(__msg->attr.rrq() ? FFLAGS_MSGRRQ      : 0);
  __hdr.fflags |= (uint16_t)(__msg->attr.rrc() ? FFLAGS_MSGCPT      : 0);
  __hdr.fflags |= (uint16_t)(__msg->attr.arq() ? FFLAGS_MSGARQ      : 0);
  __hdr.fflags |= (uint16_t)(__msg->attr.urq() ? FFLAGS_MSGURQ      : 0);

  __hdr.xflags |= (uint16_t)(__msg->attr.del() ? XFLAGS_MSGDELETED  : 0);
  __hdr.xflags |= (uint16_t)(__msg->attr.ano() ? XFLAGS_MSGANON     : 0);
  __hdr.xflags |= (uint16_t)(__msg->attr.fsc() ? XFLAGS_MSGSCANNED  : 0);
  __hdr.xflags |= (uint16_t)(__msg->attr.lok() ? XFLAGS_MSGKEEP     : 0);
  __hdr.xflags |= (uint16_t)(__msg->attr.trt() ? XFLAGS_MSGTREATED  : 0);
  __hdr.xflags |= (uint16_t)(__msg->attr.lzs() ? XFLAGS_MSGPACKED   : 0);
  __hdr.xflags |= (uint16_t)(__msg->attr.gsc() ? XFLAGS_MSGGSCAN    : 0);
  __hdr.xflags |= (uint16_t)(__msg->attr.rsc() ? XFLAGS_MSGRSCAN    : 0);
  __hdr.xflags |= (uint16_t)(__msg->attr.arc() ? XFLAGS_MSGARCHIVED : 0);
  __hdr.xflags |= (uint16_t)(__msg->attr.tag() ? XFLAGS_MSGTAGGED   : 0);

  if(not (__mode & GMSG_UPDATE)) {
    if(__msg->attr.uns()) {
      __hdr.xflags &= ~(XFLAGS_MSGSCANNED|XFLAGS_MSGGSCAN|XFLAGS_MSGRSCAN);
      if(isnet())
        __hdr.xflags |= XFLAGS_MSGNET;
      else if(isecho())
        __hdr.xflags |= XFLAGS_MSGECHO;
    }
    else {
      if(isnet()) {
        __hdr.fflags |= FFLAGS_MSGSENT;
        __hdr.xflags |= XFLAGS_MSGNET | XFLAGS_MSGSCANNED;
      }
      else if(isecho()) {
        __hdr.fflags |= FFLAGS_MSGSENT;
        __hdr.xflags |= XFLAGS_MSGECHO | XFLAGS_MSGSCANNED;
      }
    }
  }

  __hdr.iflags = __msg->adeptxbbs.iflags;
  __hdr.oflags = __msg->adeptxbbs.oflags;

  __hdr.start  = __msg->txtstart;
  __hdr.length = __msg->txtlength;

  if(__mode & GMSG_TXT) {

    // Write the message text
    uint _size = strlen(__msg->txt) + 1;
    if((__mode & GMSG_NEW) or (_size > __hdr.length))
      __hdr.start = filelength(data->fhtext);
    lseekset(data->fhtext, __hdr.start);
    lock_file(data->fhtext, __hdr.start, 640L*1024L);
    write(data->fhtext, __msg->txt, _size);
    unlock_file(data->fhtext, __hdr.start, 640L*1024L);
    __hdr.length = _size;
  }

  // Write header record
  int32_t position = (__msg->msgno-1L)*sizeof(XbbsHdr);
  lseekset(data->fhdata, position);
  lock_file(data->fhdata, position, sizeof(XbbsHdr));
  write(data->fhdata, &__hdr, sizeof(XbbsHdr));
  unlock_file(data->fhdata, position, sizeof(XbbsHdr));

  // Build and write index record unless we are just updating the header
  if(__mode & GMSG_TXT) {
    XbbsIdx idx;
    char buf[201];
    idx.to   = XbbsCheckSum(__hdr.to);
    idx.from = XbbsCheckSum(__hdr.from);
    idx.subj = XbbsCheckSum(__hdr.subj);
    idx.msgidcrc = idx.msgidserialno = 0;
    idx.replycrc = idx.replyserialno = 0;
    if(*__msg->msgids) {
      strcpy(buf, __msg->msgids);
      char* ptr = strrchr(buf, ' ');
      if(ptr) {
        *ptr++ = NUL;
        idx.msgidcrc = strCrc32(buf, true, CRC32_MASK_CCITT);
        idx.msgidserialno = atoulx(ptr);
      }
    }
    if(*__msg->replys) {
      strcpy(buf, __msg->replys);
      char* ptr = strrchr(buf, ' ');
      if(ptr) {
        *ptr++ = NUL;
        idx.replycrc = strCrc32(buf, true, CRC32_MASK_CCITT);
        idx.replyserialno = atoulx(ptr);
      }
    }
    memcpy(data->idx+__msg->msgno-1, &idx, sizeof(XbbsIdx));
    position = (__msg->msgno-1L)*sizeof(XbbsIdx);
    lseekset(data->fhindex, position);
    lock_file(data->fhindex, position, sizeof(XbbsIdx));
    write(data->fhindex, &idx, sizeof(XbbsIdx));
    unlock_file(data->fhindex, position, sizeof(XbbsIdx));
  }

  if(not (__mode & GMSG_DELETE)) {
    // Add/update personal mail record
    int _addpm = true;
    // Remove it if the msg is being received
    if((__mode & GMSG_UPDATE) and __msg->attr.rcv())
      _addpm = false;
    // Don't touch it if the msg was already received
    if(not (_addpm and __msg->attr.rcv())) {
      // Only do it if the person is on this BBS
      if(wide->user->find(__msg->to))
        update_personal_mail(__msg, __hdr, _addpm);
    }
  }

  GFTRK(0);
}


//  ------------------------------------------------------------------

void XbbsArea::save_hdr(int __mode, gmsg* __msg) {

  GFTRK("XbbsSaveHdr");

  XbbsHdr _hdr;
  save_message(__mode|GMSG_HDR, __msg, _hdr);
}


//  ------------------------------------------------------------------

void XbbsArea::save_msg(int __mode, gmsg* __msg) {

  GFTRK("XbbsSaveMsg");

  XbbsHdr _hdr;
  save_message(__mode|GMSG_HDRTXT, __msg, _hdr);
}


//  ------------------------------------------------------------------

void XbbsArea::del_msg(gmsg* __msg) {

  GFTRK("XbbsDelMsg");

  XbbsHdr _hdr;
  save_message(GMSG_HDR | GMSG_DELETE, __msg, _hdr);
}


//  ------------------------------------------------------------------

void XbbsArea::new_msgno(gmsg* __msg) {

  GFTRK("XbbsNewMsgno");

  __msg->msgno = data->idx_size + 1;

  GFTRK(0);
}


//  ------------------------------------------------------------------

void XbbsArea::update_personal_mail(gmsg* __msg, XbbsHdr& __hdr, int __addpm) {

  XbbsPmi _pmi;
  _pmi.areanumber = board();
  _pmi.msgnumber = __msg->msgno;
  strxcpy(_pmi.from, __msg->by, sizeof(_pmi.from));
  strxcpy(_pmi.subject, __msg->re, sizeof(_pmi.subject));
  strxcpy(_pmi.date, __hdr.date, sizeof(_pmi.date));

  int _pmino = 0;
  XbbsPmi* _pmiptr = NULL;

  gfile fp;
  Path _pmifile;
  sprintf(_pmifile, "%sPersonal_Mail\\%s", wide->path, __hdr.to);
  int fh = test_open(_pmifile, SH_DENYWR);

  int _pmirecs = (int)(filelength(fh) / sizeof(XbbsPmi));
  XbbsPmi* _pmilist = (XbbsPmi*)throw_calloc((_pmirecs+1), sizeof(XbbsPmi));
  ::read(fh, _pmilist, sizeof(XbbsPmi)*_pmirecs);

  if(_pmilist) {
    _pmiptr = _pmilist;
    for(_pmino=0; _pmino<_pmirecs; _pmino++, _pmiptr++) {
      if(_pmiptr->areanumber == _pmi.areanumber)
        if(_pmiptr->msgnumber == _pmi.msgnumber)
          break;
    }
    if(_pmino == _pmirecs)
      _pmiptr = NULL;
  }

  if(__addpm) {
    if(!_pmiptr) {
      _pmirecs++;
      _pmilist = (XbbsPmi*)throw_realloc(_pmilist, _pmirecs*sizeof(XbbsPmi));
      _pmiptr = _pmilist + _pmirecs - 1;
    }
    memcpy(_pmiptr, &_pmi, sizeof(XbbsPmi));
  }
  else {
    if(_pmiptr) {
      memmove(_pmiptr, _pmiptr+1, (_pmirecs-_pmino-1)*sizeof(XbbsPmi));
      _pmirecs--;
    }
  }

  if(_pmirecs) {
    lseekset(fh, 0);
    ::write(fh, _pmilist, sizeof(XbbsPmi)*_pmirecs);
    if(_pmiptr and not __addpm)
      chsize(fh, sizeof(XbbsPmi)*_pmirecs);
    ::close(fh);
  }
  else {
    ::close(fh);
    remove(_pmifile);
  }

  throw_free(_pmilist);
}


//  ------------------------------------------------------------------

void XbbsArea::update_timesread(gmsg* msg) {

  GFTRK("XbbsArea::update_timesread");

  XbbsHdr hdr;

  int32_t position = (Msgn->ToReln(msg->msgno)-1)*sizeof(XbbsHdr);

  ::lseekset(data->fhdata, position);
  ::read(data->fhdata, &hdr, sizeof(XbbsHdr));

  hdr.timesread = (word)msg->timesread;

  ::lseekset(data->fhdata, position);
  lock_file(data->fhdata, position, sizeof(XbbsHdr));
  ::write(data->fhdata, &hdr, sizeof(XbbsHdr));
  unlock_file(data->fhdata, position, sizeof(XbbsHdr));

  GFTRK(0);
}


//  ------------------------------------------------------------------

