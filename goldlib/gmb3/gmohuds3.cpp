//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999-2000 Alex. S. Aganichev
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
//  Hudson / Goldbase msgbase handling
//  ------------------------------------------------------------------


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
msgn_t _HudsArea<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::get_hdr_idx(gmsg* __msg, char* __file__, int __line__) {

  GFTRK("HudsGetHdrIdx");

  msgn_t _count = 0;
  msgn_t _total = (msgn_t)(wide->msgidxsize/sizeof(HudsIdx));
  HudsIdx* _msgidx_ptr = wide->msgidxptr;

  uint32_t _msgno = __msg->msgno;
  if(_msgidx_ptr) {
    while(_count <= _total) {
      if(_msgno == _msgidx_ptr->msgno) {
        GFTRK(NULL);
        return _count;
      }
      _msgidx_ptr++;
      _count++;
    }
  }

  // Report error
  uint32_t _lmsgno = lastread ? Msgn->at(lastread-1) : Msgn->at(0);
  uint32_t _lread = wide->lastrec[board()-1];
  uint32_t _active = wide->msginfo.active[board()-1];
  WideLog->errindex(__file__, __line__);
  WideLog->printf("! Failed to locate a msgno in an internal %s index.", __HUDSON ? HUDS_NAME : GOLD_NAME);
  WideLog->printf(": Msgno %u (%Xh) in board %u (%s,%u,%u).", _msgno, _msgno, board(), echoid(), Msgn->Count(), lastread);
  WideLog->printf(": First Msgno       : %u (%Xh).", Msgn->at(0), Msgn->at(0));
  WideLog->printf(": Msgno at lastread : %u (%Xh).", _lmsgno, _lmsgno);
  WideLog->printf(": Real lastread msg : %u (%Xh).", _lread, _lread);
  WideLog->printf(": Real active msgs  : %u (%Xh).", _active, _active);
  if(Msgn->Count() > 2) {
    _lmsgno = Msgn->at(Msgn->Count()-3);
    WideLog->printf(": Highest-2 msgno   : %u (%Xh).", _lmsgno, _lmsgno);
  }
  if(Msgn->Count() > 1) {
    _lmsgno = Msgn->at(Msgn->Count()-2);
    WideLog->printf(": Highest-1 msgno   : %u (%Xh).", _lmsgno, _lmsgno);
  }
  if(Msgn->Count()) {
    _lmsgno = Msgn->at(Msgn->Count()-1);
    WideLog->printf(": Highest msgno     : %u (%Xh).", _lmsgno, _lmsgno);
  }
  if(_msgno == 0xEEEEEEEEL) {
    WideLog->printf("+ Info: This indicates a serious bug.");
    WideLog->printf("+ Advice: Report to the Author immediately.");
  }
  WideLog->printf("+ Advice: Restart or run a msgbase index rebuild utility.");
  IndexErrorExit();

  GFTRK(NULL);

  return (msgn_t)(__HUDSON ? HUDS_DELETEDMSGNO : GOLD_DELETEDMSGNO);
}


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
int _HudsArea<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::load_message(int __mode, gmsg* __msg, HudsHdr& __hdr) {

  if(__msg->msgno == 0) {
    GFTRK(NULL);
    return false;
  }

  // Read header
  msgn_t _hdridx = get_hdr_idx(__msg, __FILE__, __LINE__);
  wide->fhhdr.LseekSet((int32_t)_hdridx*(int32_t)sizeof(HudsHdr));
  wide->fhhdr.Read(&__hdr, sizeof(HudsHdr));

  __msg->msgno = __hdr.msgno;
  __msg->link.to_set(__hdr.replyto);
  __msg->link.first_set(__hdr.reply1st);

  __msg->cost = __hdr.cost;
  __msg->board = __hdr.board;
  __msg->timesread = __hdr.timesread;

  strnp2cc(__msg->by, __hdr.by, 35);
  strnp2cc(__msg->to, __hdr.to, 35);
  strnp2cc(__msg->re, __hdr.re, 72);

  __msg->orig.zone  = __msg->oorig.zone  = __hdr.origzone;
  __msg->orig.net   = __msg->oorig.net   = __hdr.orignet;
  __msg->orig.node  = __msg->oorig.node  = __hdr.orignode;
  __msg->orig.point = __msg->oorig.point = 0;

  __msg->dest.zone  = __msg->odest.zone  = __hdr.destzone;
  __msg->dest.net   = __msg->odest.net   = __hdr.destnet;
  __msg->dest.node  = __msg->odest.node  = __hdr.destnode;
  __msg->dest.point = __msg->odest.point = 0;

  // Convert date and time
  int _year, _month, _day, _hour, _minute;
  sscanf(__hdr.date+1, "%d%*c%d%*c%2d", &_month, &_day, &_year);
  sscanf(__hdr.time+1, "%d%*c%2d", &_hour, &_minute);
  struct tm _tm;
  _tm.tm_year  = (_year < 80) ? (_year+100) : _year;
  _tm.tm_mon   = _month - 1;
  _tm.tm_mday  = _day;
  _tm.tm_hour  = _hour;
  _tm.tm_min   = _minute;
  _tm.tm_sec   = 0;
  _tm.tm_isdst = -1;
  time32_t a   = gmktime(&_tm);
  struct tm tp; ggmtime(&tp, &a);
  tp.tm_isdst  = -1;
  time32_t b   = gmktime(&tp);
  __msg->written = a + a - b;
  __msg->arrived = 0;

  // Convert attributes
  __msg->attr.del(__hdr.msgattr & HUDS_DELETED);
  __msg->attr.pvt(__hdr.msgattr & HUDS_PVT);
  __msg->attr.rcv(__hdr.msgattr & HUDS_RECEIVED);
  __msg->attr.loc(__hdr.msgattr & HUDS_LOCAL);
  __msg->attr.grp(__hdr.msgattr & HUDS_GROUPMSG);
  __msg->attr.k_s(__hdr.netattr & HUDS_KILLSENT);
  __msg->attr.att(__hdr.netattr & HUDS_ATTACH);
  __msg->attr.cra(__hdr.netattr & HUDS_CRASH);
  __msg->attr.rrq(__hdr.netattr & HUDS_RETRECREQ);
  __msg->attr.arq(__hdr.netattr & HUDS_AUDITREQ);
  __msg->attr.rrc(__hdr.netattr & HUDS_RETREC);
  __msg->attr.frq(__hdr.netattr & HUDS_FREQ);
  __msg->attr.uns((__hdr.netattr & HUDS_SENT) ? 0 : ((__hdr.msgattr & (HUDS_NETTRANS|HUDS_ECHOTRANS)) ? 1 : 0));
  __msg->attr.snt(__msg->attr.loc() and not (__msg->attr.uns() or __msg->attr.rcv()));

  __msg->txtstart = __hdr.startrec;
  __msg->txtlength = __hdr.numrecs;

  // If message text is used
  if(__mode & GMSG_TXT) {

    // Get length of message text
    uint _numrecs = __hdr.numrecs;

    // Allocate memory for message text
    __msg->txt = (char*)throw_realloc(__msg->txt, _numrecs*256+256);
    *__msg->txt = NUL;

    // Read message text and convert it to a NUL-terminated C string
    if(_numrecs) {

      // Seek to, and read the raw text
      wide->fhtxt.LseekSet((int32_t)__hdr.startrec*256L);
      wide->fhtxt.Read(__msg->txt, _numrecs*256);

      // Set up loop variables
      char* _src = __msg->txt;
      char* _dst = _src;
      uint _count = 0;

      // Conversion loop
      while(_count++ < _numrecs) {

        // Get block length
        byte _len = *_src++;

        // Move text over the length byte
        memmove(_dst, _src, _len);

        // Move pointers
        _dst += _len;
        _src += _len;
      }

      // NUL-terminate the text
      *_dst = NUL;
    }
  }

  GFTRK(NULL);

  return true;
}


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
int _HudsArea<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::load_hdr(gmsg* __msg) {

  GFTRK("HudsLoadHdr");

  HudsHdr _hdr;
  return load_message(GMSG_HDR, __msg, _hdr);
}


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
int _HudsArea<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::load_msg(gmsg* __msg) {

  GFTRK("HudsLoadMsg");

  HudsHdr _hdr;
  return load_message(GMSG_HDRTXT, __msg, _hdr);
}


//  ------------------------------------------------------------------

