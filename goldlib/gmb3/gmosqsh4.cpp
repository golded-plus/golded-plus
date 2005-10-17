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

#include <gdbgerr.h>
#include <gmemdbg.h>
#include <gdbgtrk.h>
#include <gstrall.h>
#include <gcrcall.h>

#include <gmosqsh.h>

              
//  ------------------------------------------------------------------

void SquishArea::lock() {

  GFTRK("SquishLock");

  if(not data->islocked) {
    if(WideCanLock) {
      long _tries = 0;
      while(::lock(data->fhsqd, 0, 1) == -1) {
        if(PopupLocked(++_tries, true, real_path()) == false) {
          WideLog->ErrLock();
          raw_close();
          WideLog->printf("! A Squish msgbase file could not be locked.");
          WideLog->printf(": %s.sqd.", real_path());
          WideLog->ErrOSInfo();
          LockErrorExit();
        }
      }
      if(_tries)
        PopupLocked(0, 0, NULL);
    }
    refresh();
    data->islocked = true;
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void SquishArea::unlock() {

  GFTRK("SquishUnlock");

  if(WideCanLock and data->islocked)
    ::unlock(data->fhsqd, 0, 1);
  lseekset(data->fhsqd, 0);
  write(data->fhsqd, &data->base, sizeof(SqshBase));
  lseekset(data->fhsqi, 0);
  write(data->fhsqi, data->idx, (uint)(data->base.totalmsgs*sizeof(SqshIdx)));
  chsize(data->fhsqi, data->base.totalmsgs*sizeof(SqshIdx));
  data->islocked = false;

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void SquishArea::read_frm(dword __offset, SqshFrm* __frm) {

  lseekset(data->fhsqd, __offset);
  read(data->fhsqd, __frm, sizeof(SqshFrm));
}


//  ------------------------------------------------------------------

void SquishArea::write_frm(dword __offset, SqshFrm* __frm) {

  lseekset(data->fhsqd, __offset);
  write(data->fhsqd, __frm, sizeof(SqshFrm));
}


//  ------------------------------------------------------------------

void SquishArea::upd_frm_next(dword __offset, SqshFrm* __frm, dword __next) {

  if(__offset != SQFRAME_NULL) {
    read_frm(__offset, __frm);
    __frm->next = __next;
    write_frm(__offset, __frm);
  }
}


//  ------------------------------------------------------------------

void SquishArea::upd_frm_prev(dword __offset, SqshFrm* __frm, dword __prev) {

  if(__offset != SQFRAME_NULL) {
    read_frm(__offset, __frm);
    __frm->prev = __prev;
    write_frm(__offset, __frm);
  }
}


//  ------------------------------------------------------------------

void SquishArea::add_to_free_chain(dword __delframe, SqshFrm* __delfrm) {

  SqshBase& _base = data->base;

  // Update free frame chain
  if((_base.firstfreeframe == SQFRAME_NULL) or (_base.lastfreeframe == SQFRAME_NULL)) {

    // No other free frames, so create new chain
    _base.firstfreeframe = _base.lastfreeframe = __delframe;
    __delfrm->prev = __delfrm->next = SQFRAME_NULL;
  }
  else {

    // Insert this frame into the chain
    __delfrm->next = SQFRAME_NULL;
    __delfrm->prev = _base.lastfreeframe;
    SqshFrm _lastfrm;
    upd_frm_next(_base.lastfreeframe, &_lastfrm, __delframe);
    _base.lastfreeframe = __delframe;
  }

  // Write the deleted frame
  __delfrm->type = SQFRAME_FREE;
  //WideLog->printf("- Deleted frame 0x%08X of length %u.", __delframe, __delfrm->length);
  write_frm(__delframe, __delfrm);
}


//  ------------------------------------------------------------------

void SquishArea::delete_msg(uint __reln) {

  GFTRK("SquishDeleteMsg");

  int _was_locked = data->islocked;
  if(not _was_locked)
    lock();

  // Setup some local variables for speed
  SqshIdx* _idx = data->idx;
  SqshBase& _base = data->base;

  // Load the frame to be deleted
  SqshFrm _delfrm;
  dword _delframe = _idx[__reln].offset;
  read_frm(_delframe, &_delfrm);

  // Chain the previous and next frames together
  SqshFrm _tmpfrm;
  upd_frm_next(_delfrm.prev, &_tmpfrm, _delfrm.next);
  upd_frm_prev(_delfrm.next, &_tmpfrm, _delfrm.prev);

  // Update base data if the first or last frame was deleted
  if(_delframe == _base.firstframe)
    _base.firstframe = _delfrm.next;
  if(_delframe == _base.lastframe)
    _base.lastframe = _delfrm.prev;

  // Add the deleted msg to the free frame chain
  add_to_free_chain(_delframe, &_delfrm);

  // Remove deleted message number from the indexes
  Msgn->DelReln(__reln+1);
  dword _tomove = _base.totalmsgs - __reln - 1;
  memmove(_idx+__reln, _idx+__reln+1, (uint)(_tomove*sizeof(SqshIdx)));

  // Update base data
  _base.highestmsg--;
  _base.totalmsgs--;

  // Update area data
  if(lastread)
    lastread--;
  
  if(not _was_locked)
    unlock();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void SquishArea::init_frm(SqshFrm* __frm) {

  memset(__frm, 0, sizeof(SqshFrm));
  __frm->type = SQFRAME_NORMAL;
  __frm->id = SQFRAMEID;
}


//  ------------------------------------------------------------------
//  Copy the text itself to a buffer, or count its length if out==NULL

uint CopyToBuf(char* p, char* out, char** end) {

  if(out)
    *out++ = CTRL_A;

  uint len = 1;

  while((*p==CR) or (*p==LF) or (not WideDispsoftcr and *p==SOFTCR))
    p++;

  while((*p==CTRL_A) or (strncmp(p, "AREA:", 5)==0)) {

    // Skip over the first ^A
    if(*p == CTRL_A)
      p++;

    while(*p and (*p != CR) and (*p != LF) and (WideDispsoftcr or *p!=SOFTCR)) {
      if(out)
        *out++ = *p;
      len++;
      p++;
    }

    if(out)
      *out++ = CTRL_A;

    len++;

    while((*p==LF) or (not WideDispsoftcr and *p==SOFTCR))
      p++;
    if(*p == CR)
      p++;
    while((*p==LF) or (not WideDispsoftcr and *p==SOFTCR))
      p++;
  }

  // Nul-term the string
  if(out)
    *out = NUL;

  len++;

  // Make sure to leave no trailing CTRL_A's.
  if(out and (out[-1]==CTRL_A))
    out[-1] = NUL;
  

  // Now store the new end location of the kludge lines
  if(end)
    *end = p;
  
  return len;
}


//  ------------------------------------------------------------------

char* CopyToControlBuf(char* txt, char** newtext, uint* length) {

  // Figure out how int32_t the control info is
  uint ctlsize = CopyToBuf(txt, NULL, NULL);

  // Allocate memory for it
  char* cbuf = (char*)throw_calloc(1, ctlsize+20);

  // Now copy the text itself
  char* end;
  CopyToBuf(txt, cbuf, &end);

  if(length)
    *length -= (uint)(end-txt);
  if(newtext)
    *newtext = end;

  return cbuf;
}


//  ------------------------------------------------------------------

void SquishArea::excess_frm(dword __lastframe, dword __newframe, SqshFrm* __newfrm, dword __totsize) {

  // Is the excess length large enough for a frame and message header?
  dword _excesslength = __newfrm->length - __totsize;
  if(_excesslength >= sizeof(SqshFrm)) {

    // Calculate frame offset of the excess frame
    dword _exframe = __newframe + __totsize + sizeof(SqshFrm);

    // Adjust base data if this becomes the last free frame
    if(__lastframe == data->base.lastfreeframe)
      data->base.lastfreeframe = _exframe;

    // Setup the excess frame and write it
    SqshFrm _exfrm;
    init_frm(&_exfrm);
    _exfrm.type = SQFRAME_FREE;
    _exfrm.next = __newfrm->next;
    _exfrm.prev = __newframe;
    __newfrm->next = _exframe;
    __newfrm->length = __totsize;
    _exfrm.length = _excesslength - sizeof(SqshFrm);
    write_frm(_exframe, &_exfrm);
    SqshFrm _tmpfrm;
    upd_frm_prev(_exfrm.next, &_tmpfrm, _exframe);
    //WideLog->printf("- Created excess free frame 0x%08X of length %u.", _exframe, _exfrm.length);
  }
}


//  ------------------------------------------------------------------

uint SquishArea::find_msgn(uint32_t __tagn) {

  if(data->idx) {

    register SqshIdx* tag = data->idx;
    register uint tags = (uint)data->base.totalmsgs;

    if(__tagn and tags and (__tagn > tag[tags-1].msgno))
      return 0;

    if(tags and __tagn) {

      register int32_t _mid;
      register int32_t _left = 0;
      register int32_t _right = tags;

      do {
        _mid = (_left+_right)/2;
        if(__tagn < tag[(uint)_mid].msgno)
          _right = _mid - 1;
        else if(__tagn > tag[(uint)_mid].msgno)
          _left = _mid + 1;
        else
          return (uint)(_mid + 1);
      } while(_left < _right);

      if(__tagn == tag[(uint)_left].msgno)
        return (uint)(_left + 1);
    }
  }

  return 0;
}


//  ------------------------------------------------------------------

void SquishArea::save_message(int __mode, gmsg* __msg) {

  SqshHdr __hdr;
  int _was_locked = data->islocked;
  if(not _was_locked)
    lock();

  // If not new, does the message still exist?
  if((__mode & GMSG_NEW) or find_msgn(__msg->msgno)) {

    uint _reln = (__mode & GMSG_NEW) ? 0 : (Msgn->ToReln(__msg->msgno) - 1);

    // Reset header
    memset(&__hdr, 0, sizeof(SqshHdr));

    // Convert attributes
    __hdr.attr |= MSGUID;
    __hdr.attr |= __msg->attr.pvt() ? MSGPRIVATE : 0;
    __hdr.attr |= __msg->attr.cra() ? MSGCRASH   : 0;
    __hdr.attr |= __msg->attr.rcv() ? MSGREAD    : 0;
    __hdr.attr |= __msg->attr.snt() ? MSGSENT    : 0;
    __hdr.attr |= __msg->attr.att() ? MSGFILE    : 0;
    __hdr.attr |= __msg->attr.trs() ? MSGFWD     : 0;
    __hdr.attr |= __msg->attr.orp() ? MSGORPHAN  : 0;
    __hdr.attr |= __msg->attr.k_s() ? MSGKILL    : 0;
    __hdr.attr |= __msg->attr.loc() ? MSGLOCAL   : 0;
    __hdr.attr |= __msg->attr.hld() ? MSGHOLD    : 0;
    __hdr.attr |= __msg->attr.rsv() ? MSGXX2     : 0;
    __hdr.attr |= __msg->attr.frq() ? MSGFRQ     : 0;
    __hdr.attr |= __msg->attr.rrq() ? MSGRRQ     : 0;
    __hdr.attr |= __msg->attr.rrc() ? MSGCPT     : 0;
    __hdr.attr |= __msg->attr.arq() ? MSGARQ     : 0;
    __hdr.attr |= __msg->attr.urq() ? MSGURQ     : 0;
    __hdr.attr |= __msg->attr.prn() ? MSGPRINTED : 0;
    __hdr.attr |= __msg->attr.lok() ? MSGLOK     : 0;
    __hdr.attr |= __msg->timesread  ? MSGSEEN    : 0;

    if(__msg->attr.scn() and not __msg->attr.uns())
      __hdr.attr |= MSGSCANNED;

    if(__msg->attr.dir() and wide->direct)
      __hdr.attr |= MSGCRASH | MSGHOLD;

    memcpy(__hdr.from, __msg->by, 36);
    memcpy(__hdr.to,   __msg->to, 36);
    memcpy(__hdr.subj, __msg->re, 72);

    __hdr.orig.zone  = __msg->oorig.zone;
    __hdr.orig.net   = __msg->oorig.net;
    __hdr.orig.node  = __msg->oorig.node;
    __hdr.orig.point = __msg->oorig.point;

    __hdr.dest.zone  = __msg->odest.zone;
    __hdr.dest.net   = __msg->odest.net;
    __hdr.dest.node  = __msg->odest.node;
    __hdr.dest.point = __msg->odest.point;

    __hdr.replyto    = __msg->link.to();
    __hdr.replies[0] = __msg->link.first();
    for(int r=1; r<=8; r++)
      __hdr.replies[r] = __msg->link.list(r-1);
    __hdr.umsgid = (__mode & GMSG_NEW) ? data->base.nextmsgno : __msg->msgno;

    __hdr.date_written = TimeToFTime(__msg->written);
    __hdr.date_arrived = TimeToFTime(__msg->arrived);
    struct tm* _tm = ggmtime(&__msg->written);
    sprintf(__hdr.ftsc_date, "%02d %3s %02d  %02d:%02d:%02d",
      _tm->tm_mday, gmonths[_tm->tm_mon+1], _tm->tm_year % 100,
      _tm->tm_hour, _tm->tm_min, _tm->tm_sec
    );

    // Setup some local variables for speed
    int _fhsqd = data->fhsqd;
    SqshIdx* _idx = data->idx;
    SqshBase& _base = data->base;
    dword _hash = strHash32(__hdr.to) | ((__hdr.attr & MSGREAD) ? 0x80000000LU : 0);

    // Writing msg text?
    if(__mode & GMSG_TXT) {

      char* _txt = __msg->txt;
      uint _usize = strlen(_txt) + 1;
      char* _ctl = CopyToControlBuf(_txt, &_txt, &_usize);
      dword _txtsize = strlen(_txt) + 1;
      dword _ctlsize = strlen(_ctl) + 1;
      dword _totsize = sizeof(SqshHdr) + _ctlsize + _txtsize;

      SqshFrm _oldfrm, _newfrm;
      dword _newframe = SQFRAME_NULL;
      dword _oldframe = _idx ? _idx[_reln].offset : _base.endframe;
      if(not (__mode & GMSG_NEW)) {

        // Get the original frame and see if there is still room for the msg
        read_frm(_oldframe, &_oldfrm);
        if(_oldfrm.length >= _totsize) {
          _newframe = _oldframe;
          _newfrm = _oldfrm;
        }
      }

      // It's a new message or the changed message doesn't fit
      if(_newframe == SQFRAME_NULL) {

        // If there is a max msgs limit and are we writing a new
        // msg, delete msgs to (hopefully) make room for this msg
        if(_base.maxmsgs and (__mode & GMSG_NEW))
          while(_base.maxmsgs <= _base.totalmsgs)
            delete_msg((uint)_base.protmsgs);

        // Locate a free frame, if possible
        _newframe = _base.firstfreeframe;
        //WideLog->printf("- Looking for a frame of at least length %u.", _totsize);
        while(1) {

          // At end of free frames?
          if(_newframe == SQFRAME_NULL) {
            _newframe = _base.endframe;
            init_frm(&_newfrm);
            //WideLog->printf("- Allocated new frame 0x%08X of length %u.", _newframe, _totsize);
            break;
          }

          // Is this frame large enough in itself?
          read_frm(_newframe, &_newfrm);
          //WideLog->printf("- Found free frame 0x%08X of length %u.", _newframe, _newfrm.length);
          if(_newfrm.length >= _totsize) {

            // Create excess frame if possible
            if(wide->recycle == SQUISHRECYCLE_YES) {
              excess_frm(_newframe, _newframe, &_newfrm, _totsize);
              //WideLog->printf("- Frame was large enough (%u bytes wasted).", _newfrm.length - _totsize);
            }
            break;
          }

          // If two frames are adjacent, try to merge them to make more room
          if(wide->recycle and (wide->recycle != SQUISHRECYCLE_MSGAPI2)) {
            dword _lastframe = SQFRAME_NULL;
            while((_newfrm.next == (_newframe+_newfrm.length+sizeof(SqshFrm))) and (_newfrm.length < _totsize)) {
              SqshFrm _lastfrm;
              read_frm(_newfrm.next, &_lastfrm);
              _newfrm.length += _lastfrm.length + sizeof(SqshFrm);
              //WideLog->printf("- Merged frames 0x%08X and 0x%08X. New length: %u.", _newframe, _newfrm.next, _newfrm.length);
              _lastframe = _newfrm.next;
              _newfrm.next = _lastfrm.next;
            }

            // Did we get a large enough frame?
            if(_newfrm.length >= _totsize) {

              // Create excess frame if possible
              if(wide->recycle == SQUISHRECYCLE_YES) {
                excess_frm(_lastframe, _newframe, &_newfrm, _totsize);
                //WideLog->printf("- Merged frame was large enough (%u bytes wasted).", _newfrm.length - _totsize);
              }

              // If one of the frames in our chain was the last free frame,
              // set the last free frame to the one we've merged it into,
              // for later a clean up effort.
              if(_lastframe == _base.lastfreeframe)
                _base.lastfreeframe = _newframe;

              // Got a free frame
              break;
            }
          }

          // Go to next free frame and try again
          _newframe = _newfrm.next;
        }

        // If this was the first frame (ie. the first one pointed to by
        // firstfreeframe, which means that the first frame found was int32_t
        // enough to hold the message), then set the free pointer to the
        // start of the new free chain.
        if(_newframe == _base.firstfreeframe)
          _base.firstfreeframe = _newfrm.next;
        if(_newframe == _base.lastfreeframe)
          _base.lastfreeframe = _newfrm.prev;

        // Now update the linked list of free frames, to remove the current
        // frame from the free-frame list, if necessary.  We only need to do
        // this if the current frame wasn't just being appended to the .SQD
        // file, since there would be no links to update in that case.
        if(_newframe != _base.endframe) {
          SqshFrm _tmpfrm;
          upd_frm_next(_newfrm.prev, &_tmpfrm, _newfrm.next);
          upd_frm_prev(_newfrm.next, &_tmpfrm, _newfrm.prev);
        }

        if(__mode & GMSG_NEW) {

          // Link the frame to the last frame
          _newfrm.prev = _base.lastframe;
          _newfrm.next = SQFRAME_NULL;
          SqshFrm _tmpfrm;
          upd_frm_next(_newfrm.prev, &_tmpfrm, _newframe);
          if(_base.firstframe == SQFRAME_NULL)
            _base.firstframe = _newframe;
          _base.lastframe = _newframe;
        }
        else {

          // Rewriting old message
          _newfrm.next = _oldfrm.next;
          _newfrm.prev = _oldfrm.prev;
          add_to_free_chain(_oldframe, &_oldfrm);
          SqshFrm _tmpfrm;
          upd_frm_next(_newfrm.prev, &_tmpfrm, _newframe);
          upd_frm_prev(_newfrm.next, &_tmpfrm, _newframe);
          if(_base.firstframe == _oldframe)
            _base.firstframe = _newframe;
          if(_base.lastframe == _oldframe)
            _base.lastframe = _newframe;
        }

        // Set the frame length only if this is a brand new frame
        if(_newframe == _base.endframe) {
          _newfrm.length = _totsize;
          _base.endframe += sizeof(SqshFrm) + _totsize;
        }
      }

      // Set sizes in the frame
      _newfrm.totsize = _totsize;
      _newfrm.ctlsize = _ctlsize;
      _newfrm.type = SQFRAME_NORMAL;

      // Write frame, header, control info and message text
      write_frm(_newframe, &_newfrm);
      write(_fhsqd, &__hdr, sizeof(SqshHdr));
      write(_fhsqd, _ctl, (uint)_ctlsize);
      write(_fhsqd, _txt, (uint)_txtsize);
      throw_free(_ctl);

      // Update internal arrays if new
      if(__mode & GMSG_NEW) {
        _base.highestmsg++;
        _reln = (uint)(_base.totalmsgs++);
        __msg->msgno = _base.nextmsgno++;
        Msgn->Append(__msg->msgno);
        data->idx = _idx = (SqshIdx*)throw_realloc(data->idx, (uint)(_base.totalmsgs*sizeof(SqshIdx)));
      }

      // Update index
      SqshIdx* _idxp = _idx + _reln;
      _idxp->offset = _newframe;
      _idxp->msgno = __msg->msgno;
      _idxp->hash = _hash;
    }
    else {

      // Just update the header
      _idx[_reln].hash = _hash;
      lseekset(_fhsqd, _idx[_reln].offset+sizeof(SqshFrm));
      write(_fhsqd, &__hdr, sizeof(SqshHdr));
    }

    // Adjust the highwatermark if required
    if(__msg->attr.uns())
      if(_base.highwatermark >= __msg->msgno)
        _base.highwatermark = __msg->msgno - 1;
  }
  else {
    scan();
  }

  if(not _was_locked)
    unlock();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void SquishArea::save_hdr(int __mode, gmsg* __msg) {

  GFTRK("SquishSaveHdr");

  save_message(__mode|GMSG_HDR, __msg);
}


//  ------------------------------------------------------------------

void SquishArea::save_msg(int __mode, gmsg* __msg) {

  GFTRK("SquishSaveMsg");

  save_message(__mode|GMSG_HDRTXT, __msg);
}


//  ------------------------------------------------------------------

void SquishArea::del_msg(gmsg* __msg) {

  GFTRK("SquishDelMsg");

  delete_msg(Msgn->ToReln(__msg->msgno)-1);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void SquishArea::new_msgno(gmsg* __msg) {

  __msg->msgno = data->base.nextmsgno;
}


//  ------------------------------------------------------------------

void SquishArea::update_timesread(gmsg* msg) {

  GFTRK("SquishArea::update_timesread");

  lock();

  uint reln = Msgn->ToReln(msg->msgno) - 1;

  dword frame = data->idx[reln].offset;

  SqshHdr hdr;
  ::lseekset(data->fhsqd, frame+sizeof(SqshFrm));
  ::read(data->fhsqd, &hdr, sizeof(SqshHdr));

  hdr.attr |= msg->timesread ? MSGSEEN : 0;

  ::lseekset(data->fhsqd, frame+sizeof(SqshFrm));
  ::write(data->fhsqd, &hdr, sizeof(SqshHdr));

  unlock();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

