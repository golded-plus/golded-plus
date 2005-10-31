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

#include <gcrcall.h>
#include <gdbgerr.h>
#include <gdbgtrk.h>
#include <gstrall.h>
#include <gutlmisc.h>
#include <gmoezyc.h>


//  ------------------------------------------------------------------

void EzycomArea::lock() {
                        
  GFTRK("EzycomLock");

  raw_close();
  data->omode = O_RDWR;
  data->smode = SH_DENYWR;
  test_raw_open(__LINE__);
  data->islocked = true;

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void EzycomArea::unlock() {
                        
  GFTRK("EzycomUnlock");

  raw_close();
  data->omode = O_RDONLY;
  data->smode = SH_DENYNO;
  test_raw_open(__LINE__);
  data->islocked = false;

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void EzycomArea::save_message(int __mode, gmsg* __msg, EzycHdr& __hdr) {

  int _was_locked = data->islocked;
  if(not _was_locked)
    lock();

  if(__mode & GMSG_NEW)
    __msg->msgno = (filelength(data->fhhdr) / sizeof(EzycHdr)) + 1;

  // Reset header
  memset(&__hdr, 0, sizeof(EzycHdr));

  // Convert attributes
  __hdr.msgattr |= (byte)(__msg->attr.del() ? EZYC_MSGATTR_DELETED  : 0);
  __hdr.msgattr |= (byte)(__msg->attr.pvt() ? EZYC_MSGATTR_PRIVATE  : 0);
  __hdr.msgattr |= (byte)(__msg->attr.rcv() ? EZYC_MSGATTR_RECEIVED : 0);
  __hdr.msgattr |= (byte)(__msg->attr.loc() ? EZYC_MSGATTR_LOCAL    : 0);
  __hdr.msgattr |= (byte)(__msg->attr.lok() ? EZYC_MSGATTR_NOKILL   : 0);
  __hdr.netattr |= (byte)(__msg->attr.k_s() ? EZYC_NETATTR_KILLSENT : 0);
  __hdr.netattr |= (byte)(__msg->attr.snt() ? EZYC_NETATTR_SENT     : 0);
  __hdr.netattr |= (byte)(__msg->attr.att() ? EZYC_NETATTR_ATTACH   : 0);
  __hdr.netattr |= (byte)(__msg->attr.cra() ? EZYC_NETATTR_CRASH    : 0);
  __hdr.netattr |= (byte)(__msg->attr.frq() ? EZYC_NETATTR_FREQ     : 0);
  __hdr.netattr |= (byte)(__msg->attr.rrq() ? EZYC_NETATTR_RREQ     : 0);
  __hdr.netattr |= (byte)(__msg->attr.arq() ? EZYC_NETATTR_AREQ     : 0);
  __hdr.netattr |= (byte)(__msg->attr.rrc() ? EZYC_NETATTR_RREC     : 0);
  __hdr.extattr = __msg->ezycom.extattr;
  __hdr.extattr |= (byte)(__msg->timesread  ? EZYC_EXTATTR_SEEN     : 0);

  if(__msg->attr.uns()) {
    if(isnet())
      __hdr.msgattr |= EZYC_MSGATTR_NETPEND;
    else if(isecho())
      __hdr.msgattr |= EZYC_MSGATTR_ECHOPEND;
  }

  // Delete if requested
  if(__mode & GMSG_DELETE)
    __hdr.msgattr |= EZYC_MSGATTR_DELETED;

  // Convert header data

  __hdr.posttimedate = TimeToFTime(__msg->written);
  __hdr.recvtimedate = TimeToFTime(__msg->arrived);

  SwapWord32((uint32_t*)&__hdr.posttimedate);
  SwapWord32((uint32_t*)&__hdr.recvtimedate);

  strc2p(strxcpy(__hdr.whoto,   __msg->to, sizeof(__hdr.whoto)));
  strc2p(strxcpy(__hdr.whofrom, __msg->by, sizeof(__hdr.whofrom)));
  strc2p(strxcpy(__hdr.subject, __msg->re, sizeof(__hdr.subject)));

  __hdr.orignet.zone  = __msg->oorig.zone;
  __hdr.orignet.net   = __msg->oorig.net;
  __hdr.orignet.node  = __msg->oorig.node;
  __hdr.orignet.point = __msg->oorig.point;

  __hdr.destnet.zone  = __msg->odest.zone;
  __hdr.destnet.net   = __msg->odest.net;
  __hdr.destnet.node  = __msg->odest.node;
  __hdr.destnet.point = __msg->odest.point;

  __hdr.replyto = (word)__msg->link.to();
  __hdr.reply1st = (word)__msg->link.first();

  __hdr.cost = (word)__msg->cost;

  __hdr.startposition = __msg->txtstart;
  __hdr.messagelength = __msg->txtlength;

  if(__mode & GMSG_TXT) {

    // Write the message text
    uint _size = strlen(__msg->txt) + 1;
    if((__mode & GMSG_NEW) or (_size > __hdr.messagelength))
      __hdr.startposition = filelength(data->fhtxt);
    lseekset(data->fhtxt, __hdr.startposition);
    write(data->fhtxt, __msg->txt, _size);
    __hdr.messagelength = _size;
  }

  // Write header record
  lseekset(data->fhhdr, __msg->msgno-1, sizeof(EzycHdr));
  write(data->fhhdr, &__hdr, sizeof(EzycHdr));

  int _fh;

  // Update MSGEXPRT.BBS, which tells ezymail/ezynet which areas to scan for mail
  _fh = test_open(AddPath(wide->msgbasepath, "MSGEXPRT.BBS"), O_WRONLY|O_CREAT|O_BINARY, SH_DENYNO);
  if(_fh == -1) {
    WideLog->ErrOpen();
    WideLog->printf("! A Ezycom msgbase file could not be opened.");
    WideLog->printf(": %s.", AddPath(wide->msgbasepath, "MSGEXPRT.BBS"));
    WideLog->ErrOSInfo();
    OpenErrorExit();
  }
  byte _tmp = true;
  lseekset(_fh, board()-1);
  write(_fh, &_tmp, sizeof(byte));
  ::close(_fh);

  // Update MSGFAST.BBS
  EzycFast _msgfast;
  _msgfast.msgboard = (word)board();
  _msgfast.msgnumber = (word)__msg->msgno;
  _msgfast.whoto = CRC32_MASK_CCITT;
  char* _ptr = __msg->to;
  while(*_ptr) {
    _msgfast.whoto = updCrc32((char)g_toupper(*_ptr), _msgfast.whoto);
    _ptr++;
  }
  _fh = test_open(AddPath(wide->msgbasepath, "MSGFAST.BBS"), O_WRONLY|O_CREAT|O_BINARY, SH_DENYWR);
  if(_fh == -1) {
    WideLog->ErrOpen();
    WideLog->printf("! A Ezycom msgbase file could not be opened.");
    WideLog->printf(": %s.", AddPath(wide->msgbasepath, "MSGFAST.BBS"));
    WideLog->ErrOSInfo();
    OpenErrorExit();
  }
  lseek(_fh, 0, SEEK_END);
  write(_fh, &_msgfast, sizeof(EzycFast));
  ::close(_fh);

  // Update MSGCOUNT.BBS
  _fh = test_open(AddPath(wide->msgbasepath, "MSGCOUNT.BBS"), O_WRONLY|O_CREAT|O_BINARY, SH_DENYNO);
  if(_fh == -1) {
    WideLog->ErrOpen();
    WideLog->printf("! A Ezycom msgbase file could not be opened.");
    WideLog->printf(": %s.", AddPath(wide->msgbasepath, "MSGCOUNT.BBS"));
    WideLog->ErrOSInfo();
    OpenErrorExit();
  }

  if(WideCanLock) {

    // Try to get the lock
    long _tries = 0;
    while(::lock(_fh, (board()-1)*2, 2) == -1) {

      // Tell the world
      if(PopupLocked(++_tries, true, AddPath(wide->msgbasepath, "MSGCOUNT.BBS")) == false) {

        // User requested to exit
        WideLog->ErrLock();
        WideLog->printf("! A Ezycom msgbase file could not be locked.");
        WideLog->printf(": %s.", AddPath(wide->msgbasepath, "MSGCOUNT.BBS"));
        WideLog->ErrOSInfo();
        LockErrorExit();
      }
    }

    // Remove the popup window
    if(_tries)
      PopupLocked(0, 0, NULL);
  }

  word _tmpword = (word)(filelength(data->fhhdr) / sizeof(EzycHdr));
  lseekset(_fh, (board()-1)*2);
  write(_fh, &_tmpword, sizeof(word));
  ::unlock(_fh, (board()-1)*2, 2);
  ::close(_fh);

  // Update internals if new
  if(__mode & GMSG_NEW) {

    // Count our msgs
    data->timesposted++;

    // Update internal array
    Msgn->Append(__msg->msgno);
  }

  if(not _was_locked)
    unlock();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void EzycomArea::save_hdr(int __mode, gmsg* __msg) {

  GFTRK("EzycomSaveHdr");

  EzycHdr _hdr;
  save_message(__mode|GMSG_HDR, __msg, _hdr);
}


//  ------------------------------------------------------------------

void EzycomArea::save_msg(int __mode, gmsg* __msg) {

  GFTRK("EzycomSaveMsg");

  EzycHdr _hdr;
  save_message(__mode|GMSG_HDRTXT, __msg, _hdr);
}


//  ------------------------------------------------------------------

void EzycomArea::del_msg(gmsg* __msg) {

  GFTRK("EzycomDelMsg");

  EzycHdr _hdr;
  save_message(GMSG_HDR | GMSG_DELETE, __msg, _hdr);
}


//  ------------------------------------------------------------------

void EzycomArea::new_msgno(gmsg* __msg) {

  GFTRK("EzycomNewMsgno");

  __msg->msgno = (filelength(data->fhhdr) / sizeof(EzycHdr)) + 1;

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void EzycomArea::update_timesread(gmsg* msg) {

  GFTRK("EzycomArea::update_timesread");

  lock();

  EzycHdr hdr;
  ::lseekset(data->fhhdr, msg->msgno-1, sizeof(EzycHdr));
  ::read(data->fhhdr, &hdr, sizeof(EzycHdr));

  hdr.extattr |= (byte)(msg->timesread ? EZYC_EXTATTR_SEEN : 0);

  ::lseekset(data->fhhdr, msg->msgno-1, sizeof(EzycHdr));
  ::write(data->fhhdr, &hdr, sizeof(EzycHdr));

  unlock();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------
