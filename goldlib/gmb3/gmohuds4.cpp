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
void _HudsWide<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::update_netecho(char* __name, msgn_t __hdridx, int __delete) {

  GFTRK("HudsUpdateNetEcho");

  Path name;
  strcpy(name, AddPath(syspath, __name));
  int _fh = test_open(name, O_CREAT);

  // Get size of the file
  uint _total = (uint)(filelength(_fh)/sizeof(msgn_t));

  // Allocate memory for the scanning index
  msgn_t* _scanidx = (msgn_t*)throw_calloc(_total+5, sizeof(msgn_t));

  // Read the scanning index
  read(_fh, _scanidx, _total*sizeof(msgn_t));

  // Close the file
  ::close(_fh);

  // Search for the specified header index
  uint _pos = 0;
  uint _closest = 0;
  while(_pos < _total) {
    if(_scanidx[_pos] <= __hdridx)
      _closest = _pos;
    if(_scanidx[_pos] == __hdridx)
      break;
    _pos++;
  }

  // Delete or add the header index
  if(__delete) {
    if(_pos < _total) {
      --_total;
      if(_total != _pos)
        memmove(_scanidx+_pos, _scanidx+_pos+1, (_total-_pos)*sizeof(msgn_t));
    }
  }
  else {
    if(_scanidx[_closest] != __hdridx) {
      ++_closest;
      if(_closest != _total)
        memmove(_scanidx+_closest+1, _scanidx+_closest, (_total-_closest+1)*sizeof(msgn_t));
      _scanidx[_closest] = __hdridx;
      ++_total;
    }
  }

  // Write the changed scanning file from scratch
  if(_total) {
    _fh = test_open(__name, O_CREAT|O_TRUNC);
    write(_fh, _scanidx, _total*sizeof(msgn_t));
    ::close(_fh);
  }
  else {
    // If the last header index was removed, delete the file
    remove(__name);
  }

  // Free the memory
  throw_free(_scanidx);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
void _HudsArea<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::save_message(int __mode, gmsg* __msg, HudsHdr& __hdr) {

  // Lock msgbase before doing anything
  int _was_locked = wide->islocked;
  if(not _was_locked)
    lock();

  // Find header index
  msgn_t _hdridx;
  if(__mode & GMSG_NEW) {
    __msg->msgno = wide->msginfo.high + 1;
    _hdridx = (msgn_t)(filelength(wide->fhhdr)/(long)sizeof(HudsHdr));
  }
  else {
    _hdridx = get_hdr_idx(__msg, __FILE__, __LINE__);
  }

  __hdr.board = (board_t)board();
  __hdr.msgno = (msgn_t)__msg->msgno;
  __hdr.replyto = (msgn_t)__msg->link.to();
  __hdr.reply1st = (msgn_t)__msg->link.first();

  struct tm* _tmp = gmtime(&__msg->written);
  strc2p(strftimei(__hdr.date, 9, "%m-%d-%y", _tmp));
  strc2p(strftimei(__hdr.time, 6, "%H:%M", _tmp));

  strc2p(strxcpy(__hdr.to, __msg->to, sizeof(__hdr.to)));
  strc2p(strxcpy(__hdr.by, __msg->by, sizeof(__hdr.by)));
  strc2p(strxcpy(__hdr.re, __msg->re, sizeof(__hdr.re)));

  __hdr.origzone = (byte)__msg->oorig.zone;
  __hdr.orignet  = __msg->oorig.net;
  __hdr.orignode = __msg->oorig.node;

  __hdr.destzone = (byte)__msg->odest.zone;
  __hdr.destnet  = __msg->odest.net;
  __hdr.destnode = __msg->odest.node;

  __hdr.cost      = (word)__msg->cost;
  __hdr.timesread = (word)__msg->timesread;

  // Transfer attributes
  __hdr.msgattr = __hdr.netattr = 0;
  __hdr.msgattr |= (byte)(__msg->attr.del() ? HUDS_DELETED   : 0);
  __hdr.msgattr |= (byte)(__msg->attr.pvt() ? HUDS_PVT       : 0);
  __hdr.msgattr |= (byte)(__msg->attr.rcv() ? HUDS_RECEIVED  : 0);
  __hdr.msgattr |= (byte)(__msg->attr.loc() ? HUDS_LOCAL     : 0);
  __hdr.msgattr |= (byte)(__msg->attr.grp() ? HUDS_GROUPMSG  : 0);
  __hdr.netattr |= (byte)(__msg->attr.k_s() ? HUDS_KILLSENT  : 0);
  __hdr.netattr |= (byte)(__msg->attr.snt() ? HUDS_SENT      : 0);
  __hdr.netattr |= (byte)(__msg->attr.att() ? HUDS_ATTACH    : 0);
  __hdr.netattr |= (byte)(__msg->attr.cra() ? HUDS_CRASH     : 0);
  __hdr.netattr |= (byte)(__msg->attr.rrq() ? HUDS_RETRECREQ : 0);
  __hdr.netattr |= (byte)(__msg->attr.arq() ? HUDS_AUDITREQ  : 0);
  __hdr.netattr |= (byte)(__msg->attr.rrc() ? HUDS_RETREC    : 0);
  __hdr.netattr |= (byte)(__msg->attr.frq() ? HUDS_FREQ      : 0);
  if(isnet())
    __hdr.msgattr |= HUDS_NETMSG;
  if(__msg->attr.uns()) {
    if(isnet())
      __hdr.msgattr |= HUDS_NETTRANS;
    else if(isecho())
      __hdr.msgattr |= HUDS_ECHOTRANS;
  }

  if(__mode & GMSG_DELETE)
    __hdr.msgattr |= HUDS_DELETED;

  __hdr.startrec = (msgn_t)__msg->txtstart;
  __hdr.numrecs = (word)__msg->txtlength;

  // Write to MSGTXT.BBS/DAT
  if(__mode & GMSG_TXT) {

    // If the msg is new or the text is too large to fit
    uint _txtlen = strlen(__msg->txt)+1;
    if((__mode & GMSG_NEW) or (_txtlen > ((long)__msg->txtlength*255L)))
      __hdr.startrec = (msgn_t)(filelength(wide->fhtxt)/256L);

    // Calculate the number of text records to write
    register uint _fullrecs = _txtlen / 255;
    register byte _extrarec = (byte)(_txtlen % 255);
    __hdr.numrecs = (word)(_fullrecs + (_extrarec ? 1 : 0));

    // Seek to the text write position
    lseek(wide->fhtxt, (long)__hdr.startrec*256L, SEEK_SET);

    // Write the message text
    register uint _count = 0;
    register char* _txt = __msg->txt;
    char _txtbuf[256];
    char* _txtptr = _txtbuf + 1;
    *_txtbuf = 255;
    while(_count < _fullrecs) {
      memcpy(_txtptr, _txt, 255);
      write(wide->fhtxt, _txtbuf, 256);
      _txt += 255;
      _count++;
    }
    if(_extrarec) {
      *_txtbuf = _extrarec;
      memset(_txtptr, 0, 255);
      memcpy(_txtptr, _txt, _extrarec);
      write(wide->fhtxt, _txtbuf, 256);
    }
  }

  // Write to MSGHDR.BBS/DAT
  lseek(wide->fhhdr, (long)_hdridx*(long)sizeof(HudsHdr), SEEK_SET);
  write(wide->fhhdr, &__hdr, sizeof(HudsHdr));

  // Write to MSGIDX.BBS/DAT
  if(__mode & GMSG_NEW) {
    wide->msgidxsize += sizeof(HudsIdx);
    wide->msgidxptr = (HudsIdx*)throw_realloc(wide->msgidxptr, (uint)(wide->msgidxsize+sizeof(HudsIdx)));
  }
  HudsIdx* _idxp = wide->msgidxptr + (uint)_hdridx;
  _idxp->board = __hdr.board;
  _idxp->msgno = (msgn_t)((__mode & GMSG_DELETE) ? (__HUDSON ? HUDS_DELETEDMSGNO : GOLD_DELETEDMSGNO) : __hdr.msgno);
  lseek(wide->fhidx, (long)_hdridx*(long)sizeof(HudsIdx), SEEK_SET);
  write(wide->fhidx, _idxp, sizeof(HudsIdx));

  // Write to MSGTOIDX.BBS/DAT
  lseek(wide->fhtoi, (long)_hdridx*(long)sizeof(HudsToIdx), SEEK_SET);
  if(__mode & GMSG_DELETE)
    write(wide->fhtoi, "\xB* Deleted *\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", sizeof(HudsToIdx));
  else if(__hdr.msgattr & HUDS_RECEIVED)
    write(wide->fhtoi, "\xC* Received *\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", sizeof(HudsToIdx));
  else
    write(wide->fhtoi, __hdr.to, sizeof(HudsToIdx));

  // Write to MSGINFO.BBS/DAT
  if(__mode & GMSG_DELETE) {
    wide->msginfo.total--;
    wide->msginfo.active[__hdr.board-1]--;
    if(__hdr.msgno == wide->msginfo.low) {
      register uint _count = 0;
      register msgn_t _total = (msgn_t)(wide->msgidxsize/sizeof(HudsIdx));
      register HudsIdx* _msgidx_ptr = wide->msgidxptr;
      while((_count++) < _total) {
        if(_msgidx_ptr->msgno != (__HUDSON ? HUDS_DELETEDMSGNO : GOLD_DELETEDMSGNO)) {
          wide->msginfo.low = _msgidx_ptr->msgno;
          break;
        }
        _msgidx_ptr++;
      }
    }
    if(__hdr.msgno == wide->msginfo.high) {
      register uint _count = (uint)(wide->msgidxsize/sizeof(HudsIdx));
      register HudsIdx* _msgidx_ptr = wide->msgidxptr + (_count-1);
      while(_count--) {
        if(_msgidx_ptr->msgno != (__HUDSON ? HUDS_DELETEDMSGNO : GOLD_DELETEDMSGNO)) {
          wide->msginfo.high = _msgidx_ptr->msgno;
          break;
        }
        _msgidx_ptr--;
      }
    }
  }
  else if(__mode & GMSG_NEW) {
    wide->msginfo.total++;
    wide->msginfo.active[__hdr.board-1]++;
    if(__hdr.msgno < wide->msginfo.low)
      wide->msginfo.low = __hdr.msgno;
    if(__hdr.msgno > wide->msginfo.high)
      wide->msginfo.high = __hdr.msgno;
  }
  lseek(wide->fhinf, 0, SEEK_SET);
  write(wide->fhinf, &wide->msginfo, sizeof(HudsInfo));

  // Update scanning files
  if((__hdr.msgattr & HUDS_NETTRANS) or (__mode & GMSG_DELETE))
    wide->update_netecho(__HUDSON ? "netmail" HUDS_EXT : "netmail" GOLD_EXT, _hdridx, __hdr.msgattr & HUDS_DELETED);
  if((__hdr.msgattr & HUDS_ECHOTRANS) or (__mode & GMSG_DELETE))
    wide->update_netecho(__HUDSON ? "echomail" HUDS_EXT : "echomail" GOLD_EXT, _hdridx, __hdr.msgattr & HUDS_DELETED);

  if(__mode & GMSG_NEW) {

    // Count our msgs
    wide->timesposted++;

    // Add new msgno to index
    Msgn->Append(__msg->msgno);
  }

  // Unlock msgbase after use
  if(not _was_locked)
    unlock();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
void _HudsArea<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::save_hdr(int __mode, gmsg* __msg) {

  GFTRK("HudsSaveHdr");

  HudsHdr _hdr;
  save_message(__mode|GMSG_HDR, __msg, _hdr);
}


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
void _HudsArea<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::save_msg(int __mode, gmsg* __msg) {

  GFTRK("HudsSaveMsg");

  HudsHdr _hdr;
  save_message(__mode|GMSG_HDRTXT, __msg, _hdr);
}


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
void _HudsArea<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::del_msg(gmsg* __msg) {

  GFTRK("HudsDelMsg");

  HudsHdr _hdr;
  save_message(GMSG_HDR | GMSG_DELETE, __msg, _hdr);
}


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
void _HudsArea<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::new_msgno(gmsg* __msg) {

  GFTRK("HudsNewMsgno");

  __msg->msgno = wide->msginfo.high + 1;

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
void _HudsArea<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::update_timesread(gmsg* msg) {

  GFTRK("HudsArea::update_timesread");

  lock();

  msgn_t hdridx = get_hdr_idx(msg, __FILE__, __LINE__);
  ::lseekset(wide->fhhdr, (long)hdridx*(long)sizeof(HudsHdr));

  HudsHdr hdr;
  ::read(wide->fhhdr, &hdr, sizeof(HudsHdr));

  hdr.timesread = (word)msg->timesread;

  ::lseekset(wide->fhhdr, (long)hdridx*(long)sizeof(HudsHdr));
  ::write(wide->fhhdr, &hdr, sizeof(HudsHdr));

  unlock();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

