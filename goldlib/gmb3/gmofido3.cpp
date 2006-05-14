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
#include <gmemdbg.h>
#include <gdbgtrk.h>
#include <gstrall.h>
#include <gmofido.h>


//  ------------------------------------------------------------------

int FidoArea::load_message(int __mode, gmsg* __msg, FidoHdr& __hdr) {

  // Build message filename
  Path _msgfile;
  build_msgname(_msgfile, __msg->msgno);

  // Open the message file
  int _fh = test_open(_msgfile, O_RDONLY|O_BINARY, WideSharemode);
  if(_fh == -1) {
    GFTRK(0);
    return false;
  }

  // Get file info
  struct stat st;
  stat(_msgfile, &st);

  // Read header
  memset(&__hdr, 0, sizeof(FidoHdr));
  read(_fh, &__hdr, sizeof(FidoHdr));

  __msg->link.to_set(__hdr.replyto);
  __msg->link.first_set(__hdr.reply1st);

  __msg->cost      = __hdr.cost;
  __msg->timesread = __hdr.timesread;

  strxcpy(__msg->by, __hdr.by, 36);
  strxcpy(__msg->to, __hdr.to, 36);
  strxcpy(__msg->re, __hdr.re, 72);

  if(isopus()) {
    __msg->orig.zone  = __msg->oorig.zone  = 0;
    __msg->orig.net   = __msg->oorig.net   = __hdr.orignet;
    __msg->orig.node  = __msg->oorig.node  = __hdr.orignode;
    __msg->orig.point = __msg->oorig.point = 0;

    __msg->dest.zone  = __msg->odest.zone  = 0;
    __msg->dest.net   = __msg->odest.net   = __hdr.destnet;
    __msg->dest.node  = __msg->odest.node  = __hdr.destnode;
    __msg->dest.point = __msg->odest.point = 0;

    // Convert datetime
    __msg->written = FTimeToTime(&__hdr.opus.written);
    __msg->arrived = FTimeToTime(&__hdr.opus.arrived);
  }
  else {
    __msg->orig.zone  = __msg->oorig.zone  = __hdr.ftsc.origzone;
    __msg->orig.net   = __msg->oorig.net   = __hdr.orignet;
    __msg->orig.node  = __msg->oorig.node  = __hdr.orignode;
    __msg->orig.point = __msg->oorig.point = __hdr.ftsc.origpoint;

    __msg->dest.zone  = __msg->odest.zone  = __hdr.ftsc.destzone;
    __msg->dest.net   = __msg->odest.net   = __hdr.destnet;
    __msg->dest.node  = __msg->odest.node  = __hdr.destnode;
    __msg->dest.point = __msg->odest.point = __hdr.ftsc.destpoint;

    __msg->written = __msg->arrived = 0;
  }

  __msg->written = __msg->written ? __msg->written : FidoTimeToUnix(__hdr.datetime);

  if (__msg->arrived == 0)
  {
    time32_t a  = gtime(NULL);
    struct tm tp; ggmtime(&tp, &a);
    tp.tm_isdst = -1;
    time32_t b  = gmktime(&tp);
    __msg->arrived = a + a - b;
  }

  // Transfer attributes
  __msg->attr.pvt(__hdr.attr & FIDO_PVT);
  __msg->attr.cra(__hdr.attr & FIDO_CRASH);
  __msg->attr.rcv(__hdr.attr & FIDO_RECEIVED);
  __msg->attr.snt(__hdr.attr & FIDO_SENT);
  __msg->attr.att(__hdr.attr & FIDO_ATTACH);
  __msg->attr.trs(__hdr.attr & FIDO_TRANSIT);
  __msg->attr.orp(__hdr.attr & FIDO_ORPHAN);
  __msg->attr.k_s(__hdr.attr & FIDO_KILLSENT);
  __msg->attr.loc(__hdr.attr & FIDO_LOCAL);
  __msg->attr.hld(__hdr.attr & FIDO_HOLD);
  __msg->attr.rsv(__hdr.attr & FIDO_RESERVED);
  __msg->attr.frq(__hdr.attr & FIDO_FREQ);
  __msg->attr.rrq(__hdr.attr & FIDO_RETRECREQ);
  __msg->attr.rrc(__hdr.attr & FIDO_RETREC);
  __msg->attr.arq(__hdr.attr & FIDO_AUDITREQ);
  __msg->attr.urq(__hdr.attr & FIDO_UPDREQ);
  __msg->attr.lok(not (st.st_mode & S_IWUSR));

  // Set the unsent attribute
  if(isnet() or (isecho() and not wide->fidohwmarks))
    __msg->attr.uns((__hdr.attr & FIDO_LOCAL) and not (__hdr.attr & FIDO_SENT));
  else if(isecho() and wide->fidohwmarks) {
    __msg->attr.uns(data->highwatermark <  __msg->msgno);
    __msg->attr.snt(data->highwatermark >= __msg->msgno);
  }
  else
    __msg->attr.uns0();

  // If message text is used
  if(__mode & GMSG_TXT) {

    // Get length of message text
    size_t _fillen = filelength(_fh);
    uint _txtlen = (uint) ((_fillen >= sizeof(FidoHdr)) ? (_fillen - sizeof(FidoHdr)) : 0);

    // Allocate space for the message text
    __msg->txt = (char*)throw_calloc(1, _txtlen+256);

    // Read the message text
    read(_fh, __msg->txt, (uint)_txtlen);

    // Fix msgs handled by brain-dead programs
    register char* _ptr = __msg->txt;
    if(!*_ptr)
      if(_txtlen)
        *_ptr = LF;  // Quick fix

    // Fix complete msg
    if(wide->fidonullfix) {
      for(uint _count=0; _count<_txtlen; _count++,_ptr++)
        if(!*_ptr)
          *_ptr = LF;
    }
  }

  // Close the message file
  ::close(_fh);

  GFTRK(0);

  // Success
  return true;
}


//  ------------------------------------------------------------------

int FidoArea::load_hdr(gmsg* __msg) {

  GFTRK("FidoLoadHdr");

  FidoHdr _hdr;
  return load_message(GMSG_HDR, __msg, _hdr);
}


//  ------------------------------------------------------------------

int FidoArea::load_msg(gmsg* __msg) {

  GFTRK("FidoLoadMsg");

  FidoHdr _hdr;
  return load_message(GMSG_HDRTXT, __msg, _hdr);
}


//  ------------------------------------------------------------------
