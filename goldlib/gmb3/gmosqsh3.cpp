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
//  Squish msgbase handling and Maximus user functions.
//  ------------------------------------------------------------------


//  ------------------------------------------------------------------

#include <gmemdbg.h>
#include <gdbgtrk.h>
#include <gtxtpara.h>
#include <gstrall.h>

#include <gmosqsh.h>

              
//  ------------------------------------------------------------------

int SquishArea::load_message(int __mode, gmsg* __msg, SqshHdr& __hdr) {
  ssize_t rwresult=0;

  // Setup some local variables for speed
  int _fhsqd = data->fhsqd;
  SqshIdx* _idx = data->idx;
  uint _reln = Msgn->ToReln(__msg->msgno);

  if(_reln == 0) {
    GFTRK(0);
    return false;
  }

  // Load the message frame
  SqshFrm _frm;
  memset(&_frm, 0, sizeof(SqshFrm));
  lseekset(_fhsqd, _idx[_reln-1].offset);
  rwresult = read(_fhsqd, &_frm, sizeof(SqshFrm));
  if( rwresult!=sizeof(SqshFrm) ) {
    if( rwresult<0 )
      WideLog->printf("! SquishArea::load_message: data file read error \"%s\"", strerror(errno));
    if( rwresult>=0 )
      WideLog->printf("! SquishArea::load_message: can't read Squish frame");
    WideLog->printf(": Info: Your msgbase is corrupted.");
    WideLog->printf("+ Advice: Run a msgbase index rebuild/recover utility.");
    GFTRK(0);
    return false;
  }

  // Load the message header
  memset(&__hdr, 0, sizeof(SqshHdr));
  rwresult = read(_fhsqd, &__hdr, sizeof(SqshHdr));
  if( rwresult!=sizeof(SqshHdr) ) {
    if( rwresult<0 )
      WideLog->printf("! SquishArea::load_message: data file read error \"%s\"", strerror(errno));
    if( rwresult>=0 )
      WideLog->printf("! SquishArea::load_message: can't read Squish message header");
    WideLog->printf(": Info: Your msgbase is corrupted.");
    WideLog->printf("+ Advice: Run a msgbase index rebuild/recover utility.");
    GFTRK(0);
    return false;
  }

  // Read control info and message text
  if(__mode & GMSG_TXT) {

    if(_frm.length) {

      // Allocate memory for kludges and message text, then read control info
      char* _dest = __msg->txt = (char*)throw_calloc(1, (uint)(1+_frm.ctlsize+_frm.totsize-sizeof(SqshHdr)));
      char* _src = _dest + (uint)_frm.ctlsize;
      *_src = NUL;
      rwresult = read(_fhsqd, _src, (uint)_frm.ctlsize);
      if( rwresult!=_frm.ctlsize ) {
        if( rwresult<0 )
          WideLog->printf("! SquishArea::load_message: data file read error \"%s\"", strerror(errno));
        if( rwresult>=0 )
          WideLog->printf("! SquishArea::load_message: can't read Squish message kludges");
        WideLog->printf(": Info: Your msgbase is corrupted.");
        WideLog->printf("+ Advice: Run a msgbase index rebuild/recover utility.");
        throw_free(__msg->txt);
        GFTRK(0);
        return false;
      }

      // Convert Squish control info to true kludges
      while(*_src and (*_src == CTRL_A) and _src[1]) {

        // Copy the kludge char unless it's the AREA: line
        if(not ((*(dword*)(_src+1) == CHR4_AREA) and strneql(_src+1, "AREA:", 5)))
          *_dest++ = *_src;
        _src++;

        // Copy the rest of the kludge and add a CR termination
        while((*_src != CTRL_A) and *_src)
          *_dest++ = *_src++;
        *_dest++ = CR;
      }

      // Make sure the kludge block is NUL terminated
      *_dest = NUL;

      // Read the message text right after the kludges
      uint _txtlen = (uint)(_frm.totsize - _frm.ctlsize - sizeof(SqshHdr));
      rwresult = read(_fhsqd, _dest, _txtlen);
      if( rwresult!=_txtlen ) {
        if( rwresult<0 )
          WideLog->printf("! SquishArea::load_message: data file read error \"%s\"", strerror(errno));
        if( rwresult>=0 )
          WideLog->printf("! SquishArea::load_message: can't read Squish message text");
        WideLog->printf(": Info: Your msgbase is corrupted.");
        WideLog->printf("+ Advice: Run a msgbase index rebuild/recover utility.");
        throw_free(__msg->txt);
        GFTRK(0);
        return false;
      }

      // Make sure the text is NUL terminated
      _dest[_txtlen] = NUL;
    }
    else {
      __msg->txt = (char*)throw_calloc(1, 1);
    }
  }

  // Convert header

  __msg->link.to_set(__hdr.replyto);
  __msg->link.first_set(__hdr.replies[0]);

  // Convert link.list
  int q = 0;
  int r = __hdr.replies[0] == __hdr.replies[1] ? 2 : 1;
  while (r <= 8)
  {
    if (__hdr.replies[r] and __hdr.replies[r-1] != __hdr.replies[r])
      __msg->link.list_set(q++, __hdr.replies[r]);
    r++;
  }

  strxcpy(__msg->by, __hdr.from, sizeof(__msg->by));
  strxcpy(__msg->to, __hdr.to,   sizeof(__msg->to));
  strxcpy(__msg->re, __hdr.subj, sizeof(__msg->re));

  __msg->orig.zone  = __msg->oorig.zone  = __hdr.orig.zone;
  __msg->orig.net   = __msg->oorig.net   = __hdr.orig.net;
  __msg->orig.node  = __msg->oorig.node  = __hdr.orig.node;
  __msg->orig.point = __msg->oorig.point = __hdr.orig.point;

  __msg->dest.zone  = __msg->odest.zone  = __hdr.dest.zone;
  __msg->dest.net   = __msg->odest.net   = __hdr.dest.net;
  __msg->dest.node  = __msg->odest.node  = __hdr.dest.node;
  __msg->dest.point = __msg->odest.point = __hdr.dest.point;
  
  // Convert date and time
  time32_t _written = FTimeToTime(&__hdr.date_written);
  __msg->written = _written ? _written : FidoTimeToUnix(__hdr.ftsc_date);
  __msg->arrived = FTimeToTime(&__hdr.date_arrived);

  __msg->timesread = (__hdr.attr & MSGSEEN) ? 1 : 0;

  // Convert attributes
  __msg->attr.pvt(__hdr.attr & MSGPRIVATE);
  __msg->attr.cra(__hdr.attr & MSGCRASH);
  __msg->attr.rcv(__hdr.attr & MSGREAD);
  __msg->attr.snt(__hdr.attr & MSGSENT);
  __msg->attr.att(__hdr.attr & MSGFILE);
  __msg->attr.trs(__hdr.attr & MSGFWD);
  __msg->attr.orp(__hdr.attr & MSGORPHAN);
  __msg->attr.k_s(__hdr.attr & MSGKILL);
  __msg->attr.loc(__hdr.attr & MSGLOCAL);
  __msg->attr.hld(__hdr.attr & MSGHOLD);
  __msg->attr.rsv(__hdr.attr & MSGXX2);
  __msg->attr.frq(__hdr.attr & MSGFRQ);
  __msg->attr.rrq(__hdr.attr & MSGRRQ);
  __msg->attr.rrc(__hdr.attr & MSGCPT);
  __msg->attr.arq(__hdr.attr & MSGARQ);
  __msg->attr.urq(__hdr.attr & MSGURQ);
  __msg->attr.scn(__hdr.attr & MSGSCANNED);
  __msg->attr.prn(__hdr.attr & MSGPRINTED);
  __msg->attr.lok(__hdr.attr & MSGLOK);

  if(isnet()) {
    __msg->attr.uns(__msg->attr.loc() and not __msg->attr.snt());
    if(wide->direct and __msg->attr.hld() and __msg->attr.cra()) {
      // Translate hld+cra to dir attribute
      __msg->attr.dir1();
      __msg->attr.cra0();
      __msg->attr.hld0();
    }
  }
  else if(isecho())
    __msg->attr.uns(__msg->attr.loc() and ((__hdr.attr & MSGSCANNED) ? 0 : 1));
  else
    __msg->attr.uns0();

  GFTRK(0);

  return true;
}


//  ------------------------------------------------------------------

int SquishArea::load_hdr(gmsg* __msg) {

  if( __msg == NULL )
  {
    WideLog->printf("! SquishArea::load_hdr() is called with NULL pointer." );
    return false;
  }
  GFTRK("SquishLoadHdr");

  SqshHdr _hdr;
  return load_message(GMSG_HDR, __msg, _hdr);
}


//  ------------------------------------------------------------------

int SquishArea::load_msg(gmsg* __msg) {

  if( __msg == NULL )
  {
    WideLog->printf("! SquishArea::load_msg() is called with NULL pointer." );
    return false;
  }
  GFTRK("SquishLoadMsg");

  SqshHdr _hdr;
  return load_message(GMSG_HDRTXT, __msg, _hdr);
}


//  ------------------------------------------------------------------

