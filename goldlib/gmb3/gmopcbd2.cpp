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
//  PCBoard msgbase handling.
//  ------------------------------------------------------------------

#include <gdbgerr.h>
#include <gmemdbg.h>
#include <gdbgtrk.h>
#include <gstrall.h>
#include <gutlmisc.h>
#include <gmoprot.h>
#include <gmopcbd.h>

              
//  ------------------------------------------------------------------

void PcbArea::data_open() {

  wide = pcbwide;
  data = pcbdata + (pcbdatano++);
  data->fhmsg = data->fhidx = -1;
  memset(&data->base, 0x20, sizeof(PcbBase));
  data->base.highmsgno = 0;
  data->base.lowmsgno  = 0;
  data->base.active    = 0;
}


// ------------------------------------------------------------------

void PcbArea::data_close() {

  pcbdatano--;
}


//  ------------------------------------------------------------------

int PcbWideTestOpen(char* __file) {

  GFTRK("PcbWideTestOpen");

  int _fh;
  long _tries = 0;

  do {

    _fh = ::sopen(__file, O_RDWR|O_BINARY|O_CREAT, WideSharemode, S_STDRW);
    if(_fh == -1) {

      // Tell the world
      if((errno != EACCES) or (PopupLocked(++_tries, false, __file) == false)) {
        WideLog->ErrOpen();
        PcbWideClose();
        WideLog->printf("! A PCBoard msgbase file could not be opened.");
        WideLog->printf(": %s.", __file);
        WideLog->ErrOSInfo();
        OpenErrorExit();
      }
    }
  } while(_fh == -1);

  // Remove the popup window
  if(_tries)
    PopupLocked(0, 0, NULL);

  GFTRK(NULL);

  return _fh;
}


// ------------------------------------------------------------------

void PcbWideOpen() {

  GFTRK("PcbWideOpen");

  if(not pcbwide->isopen) {

    pcbwide->fhusr = PcbWideTestOpen(pcbwide->users);
    pcbwide->fhinf = PcbWideTestOpen(pcbwide->usersinf);
    pcbwide->user->fh = pcbwide->fhusr;
    pcbwide->user->fhinf = pcbwide->fhinf;

    // Read lastreads from USERS file
    lseekset(pcbwide->fhusr, (long)pcbwide->userno*(long)sizeof(PcbUsers));
    read(pcbwide->fhusr, &pcbwide->usersrec, sizeof(PcbUsers));
    int _maxlr = MinV(pcbwide->numareas, 40);
    for(int n=0; n<_maxlr; n++)
      pcbwide->lastread[n] = B2L(pcbwide->usersrec.lastmsgread[n]);

    // Read lastreads from USERS.INF file
    if(pcbwide->extconflen) {
      long _offset = (pcbwide->usersrec.usersinfrec-1)*pcbwide->usershdr.totalrecsize;
      _offset +=     pcbwide->usershdrsize;
      _offset +=     pcbwide->usershdr.sizeofrec;
      _offset += 2 * pcbwide->confbytelen;
      _offset += 3 * pcbwide->extconflen;
      lseekset(pcbwide->fhinf, _offset);
      read(pcbwide->fhinf, pcbwide->lastread+40, (pcbwide->numareas-40)*sizeof(long));
    }

    pcbwide->isopen = true;
  }

  GFTRK(NULL);
}


// ------------------------------------------------------------------

void PcbWideClose() {

  GFTRK("PcbWideClose");

  if(pcbwide->isopen) {
    if(pcbwide->fhinf != -1)  ::close(pcbwide->fhinf);  pcbwide->fhinf = -1;
    if(pcbwide->fhusr != -1)  ::close(pcbwide->fhusr);  pcbwide->fhusr = -1;
    pcbwide->isopen = false;
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void PcbArea::raw_close() {

  GFTRK("PcbRawClose");

  if(data->fhidx != -1)  ::close(data->fhidx);  data->fhidx = -1;
  if(data->fhmsg != -1)  ::close(data->fhmsg);  data->fhmsg = -1;

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

int PcbArea::test_open(const char* __file) {

  GFTRK("PcbArea::test_open");

  int _fh;
  long _tries = 0;

  do {

    _fh = ::sopen(__file, O_RDWR|O_BINARY|O_CREAT, WideSharemode, S_STDRW);
    if(_fh == -1) {

      // Tell the world
      if((errno != EACCES) or (PopupLocked(++_tries, false, __file) == false)) {
        WideLog->ErrOpen();
        raw_close();
        PcbWideClose();
        WideLog->printf("! A PCBoard msgbase file could not be opened.");
        WideLog->printf(": %s.", __file);
        WideLog->ErrOSInfo();
        OpenErrorExit();
      }
    }
  } while(_fh == -1);

  // Remove the popup window
  if(_tries)
    PopupLocked(0, 0, NULL);

  GFTRK(NULL);

  return _fh;
}


// ------------------------------------------------------------------

void PcbArea::raw_open() {

  GFTRK("PcbRawOpen");

  if(not just_scanning)
    data->fhmsg = test_open(real_path());
  data->fhidx = test_open(AddPath(real_path(), ".idx"));

  GFTRK(NULL);
}


// ------------------------------------------------------------------

void PcbArea::raw_scan(int __keep_index, int __scanpm) {

  GFTRK("PcbRawScan");

  // Open the msgbase if it wasn't already
  int _was_open = isopen;
  if(not _was_open) {
    if(not __keep_index)
      just_scanning = true;
    if(ispacked()) {
      const char* newpath = Unpack(path());
      if(newpath == NULL)
        packed(false);
      set_real_path(newpath ? newpath : path());
    }
    isopen++;
    data_open();
    raw_open();
    just_scanning = false;
  }

  int _was_wideopen = wide->isopen;
  if(not _was_wideopen)
    PcbWideOpen();

  // Load the base header
  if(__keep_index) {
    lseekset(data->fhmsg, 0);
    read(data->fhmsg, &data->base, sizeof(PcbBase));
    data->base.highmsgno = B2L(data->base.highmsgno);
    data->base.lowmsgno  = B2L(data->base.lowmsgno);
    data->base.active    = B2L(data->base.active);
  }
  
  // Get some sizes
  long _idxlen = filelength(data->fhidx);
  uint _idxsize  = (uint)_idxlen;
  uint _idxtotal = _idxsize / sizeof(PcbIdx);

  // (Re)Allocate message index
  if(__keep_index)
    Msgn->Resize(_idxtotal);

  // Allocate buffer to hold .IDX data
  PcbIdx* _idxbuf = (PcbIdx*)throw_malloc(_idxsize+1);

  // Read the entire .IDX file into memory
  lseekset(data->fhidx, 0);
  read(data->fhidx, _idxbuf, _idxsize);

  // Variables for the loop
  register uint _active = 0;
  register ulong _firstmsgno = 0;
  register ulong _lastmsgno = 0;
  register ulong _lastreadfound = 0;
  register ulong _totalmsgs = _idxtotal;
  register ulong _lastread = wide->lastread[board()];
  register uint _lastread_reln = 0;
  register ulong* _msgndxptr = Msgn->tag;
  register PcbIdx* _idxptr = _idxbuf;

  // Fill message index
  register uint n = 0;
  while((n++) < _totalmsgs) {
    if(_idxptr->offset > 0) {
      _active++;
      _lastmsgno = _idxptr->num;
      if(not _firstmsgno)
        _firstmsgno = _lastmsgno;
      if(__keep_index)
        *_msgndxptr++ = _lastmsgno;
      if((_lastmsgno >= _lastread) and (_lastread_reln == 0)) {
        _lastreadfound = _lastmsgno;
        _lastread_reln = (uint)(_active - (_lastmsgno != _lastread ? 1 : 0));
      }
    }
    _idxptr++;
  }

  // If the exact lastread was not found
  if(_active and (_lastreadfound != _lastread)) {

    // Higher than highest or lower than lowest?
    if(_lastread > _lastmsgno)
      _lastread_reln = _active;
    else if(_lastread < _firstmsgno)
      _lastread_reln = 0;
  }

  // Update area data
  Msgn->SetCount(_active);
  lastread = _lastread_reln;
  lastreadentry = _lastreadfound;

  // Scan for personal mail
  if(__scanpm) {
    int umax = (WidePersonalmail & PM_ALLNAMES) ? WideUsernames : 1;
    PcbName* uname = (PcbName*)throw_calloc(umax, sizeof(PcbName));
    for(int un=0; un<umax; un++)
      strsetsz(strxcpy(uname[un], WideUsername[un], sizeof(PcbName)), 25);
    PMrk->ResetAll();
    register uint n = lastread + 1;
    register uint cnt = Msgn->Count();
    register int gotpm = false;
    while(n <= cnt) {
      PcbIdx* idx = _idxbuf + (uint)(Msgn->at(n-1) - data->base.lowmsgno);
      for(int u=0; u<umax; u++) {
        if(strnieql(idx->to, uname[u], 25)) {
          gotpm = true;
          break;
        }
      }
      if(gotpm) {
        switch(idx->status) {
          case '+':
          case '-':
          case '`':
          case '^':
          case '#':
            break;
          default:
            PMrk->Append(idx->num);
        }
        gotpm = false;
      }
      n++;
    }
    throw_free(uname);
  }

  // Free the .IDX buffer
  throw_free(_idxbuf);

  if(not _was_wideopen)
    PcbWideClose();

  // Close the msgbase again if we opened it in here
  if(not _was_open) {
    raw_close();
    data_close();
    if(ispacked()) {
      CleanUnpacked(real_path());
    }
    isopen--;
  }

  GFTRK(NULL);
}


// ------------------------------------------------------------------

void PcbArea::scan() {

  GFTRK("PcbScan");

  raw_scan(true);

  GFTRK(NULL);
}


// ------------------------------------------------------------------

void PcbArea::scan_area() {

  GFTRK("PcbScanArea");

  raw_scan(false);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void PcbArea::scan_area_pm() {

  GFTRK("PcbScanArea*M");

  raw_scan(true, true);
  Msgn->Reset();

  GFTRK(NULL);
}


// ------------------------------------------------------------------
