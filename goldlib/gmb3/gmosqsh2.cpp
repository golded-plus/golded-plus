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

#include <vector>
#include <gmemdbg.h>
#include <gdbgerr.h>
#include <gdbgtrk.h>
#include <gstrall.h>
#include <gcrcall.h>
#include <gmosqsh.h>


//  ------------------------------------------------------------------

void SquishArea::refresh() {

  GFTRK("SquishRefresh");

  // Create new base record or read the existing one
  if(data->fhsqd != -1) {
    if(filelength(data->fhsqd) < sizeof(SqshBase)) {
      SqshBase& _base = data->base;
      memset(&_base, 0, sizeof(SqshBase));
      _base.size = sizeof(SqshBase);
      strxcpy(_base.name, real_path(), sizeof(_base.name));
      _base.endframe = _base.size;
      _base.framesize = sizeof(SqshFrm);
      _base.nextmsgno = 2;
      lseekset(data->fhsqd, 0);
      write(data->fhsqd, &data->base, sizeof(SqshBase));
    }
    else {
      lseekset(data->fhsqd, 0);
      read(data->fhsqd, &data->base, sizeof(SqshBase));
    }
  }

  // Are there any msgs?
  if(data->base.totalmsgs) {

    // Read the index file
    data->idx = (SqshIdx*)throw_realloc(data->idx, (uint)(data->base.totalmsgs*sizeof(SqshIdx)));
    lseekset(data->fhsqi, 0);
    read(data->fhsqi, data->idx, (uint)(data->base.totalmsgs*sizeof(SqshIdx)));
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void SquishArea::raw_scan(int __keep_index, int __scanpm) {

  GFTRK("SquishRawScan");

  SqshData* _was_data = data;
  if(_was_data == NULL) {
    data = squishdata;
    wide = squishwide;
  }

  int _wasopen = isopen;
  if(not _wasopen) {
    if(ispacked()) {
      const char* newpath = Unpack(path());
      if(newpath == NULL)
        packed(false);
      set_real_path(newpath ? newpath : path());
    }
    isopen++;
  }

  // Load the lastread
  dword _lastread = 0;
  int _fh = ::sopen(AddPath(real_path(), ".sql"), O_RDONLY|O_BINARY, WideSharemode, S_STDRD);
  if(_fh != -1) {
    lseekset(_fh, wide->userno, sizeof(dword));
    read(_fh, &_lastread, sizeof(dword));
    ::close(_fh);
  }

  // Open Squish files for scanning unless they are already open
  if(not _wasopen) {

    data->idx = NULL;
    data->base.totalmsgs = 0;

    // Open index file
    data->fhsqi = ::sopen(AddPath(real_path(), ".sqi"), O_RDONLY|O_BINARY, WideSharemode, S_STDRD);
    if(data->fhsqi != -1) {

      // Get the number of index records
      data->base.totalmsgs = filelength(data->fhsqi) / sizeof(SqshIdx);

      // If there appear to be only one record, experience shows that
      // there may in fact be zero!  So check the *.SQD base record for
      // the exact number of msgs in this case or if requested.
      if((data->base.totalmsgs == 1) or (wide->squishscan == SQS_API)) {

        // Open, read and close data file
        data->fhsqd = ::sopen(AddPath(real_path(), ".sqd"), O_RDONLY|O_BINARY, WideSharemode, S_STDRD);
        if(data->fhsqd != -1) {
          read(data->fhsqd, &data->base, sizeof(SqshBase));
          ::close(data->fhsqd);
          data->fhsqd = -1;
        }
      }
      else {
        data->fhsqd = -1;
      }

      // Allocate index buffer and read from file
      refresh();

      // Close index file
      ::close(data->fhsqi);
      data->fhsqi = -1;
    }
    if(ispacked()) {
      CleanUnpacked(real_path());
    }
    isopen--;
  }

  register ulong _msgno;
  register SqshIdx* _sqiptr = data->idx;
  register dword  _totalmsgs = data->base.totalmsgs;
  register ulong _firstmsgno = _totalmsgs ? _sqiptr->msgno : 0;
  register ulong _lastmsgno = 0;
  register uint _active = 0;
  register uint _lastread_reln = 0;
  register ulong _lastreadfound = 0;

  if(data->base.totalmsgs) {

    // (Re)allocate message index
    if(__keep_index)
      Msgn->Resize((uint)data->base.totalmsgs);

    register ulong* _msgndxptr = Msgn->tag;

    // Fill message index
    while(_active < _totalmsgs) {

      _active++;
      _msgno = (_sqiptr++)->msgno;
      if(__keep_index)
        *_msgndxptr++ = _msgno;

      // Check for premature end of index (free frames)
      if((_msgno <= _lastmsgno) or (_msgno == 0xFFFFFFFFL)) {
        _active--;
        if((_msgno == _lastmsgno) and (_active == 1)) {
          _lastread_reln = 0;
          _active = 0;
        }
        break;
      }

      // Get the lastread
      if((_msgno >= _lastread) and (_lastread_reln == 0)) {
        _lastreadfound = _msgno;
        _lastread_reln = _active - (_msgno != _lastread ? 1 : 0);
      }

      // Store last message number
      _lastmsgno = _msgno;
    }

    // If the exact lastread was not found
    if(_active and (_lastreadfound != _lastread)) {

      // Higher than highest or lower than lowest?
      if(_lastread > _lastmsgno)
        _lastread_reln = _active;
      else if(_lastread < _firstmsgno)
        _lastread_reln = 0;
    }
  }

  // Update area data
  Msgn->SetCount(_active);
  lastread = _lastread_reln;
  lastreadentry = _lastreadfound;

  // Scan for personal mail
  if(__scanpm) {
    int umax = (WidePersonalmail & PM_ALLNAMES) ? WideUsernames : 1;
    std::vector<dword> uhash;
    for(int uh=0; uh<umax; uh++)
      uhash.push_back(strHash32(WideUsername[uh]));
    PMrk->Reset();
    register uint n = lastread + 1;
    register uint cnt = Msgn->Count();
    register int gotpm = false;
    while(n <= cnt) {
      SqshIdx* idx = data->idx + (n-1);
      for(int u=0; u<umax; u++) {
        if((idx->hash & 0x80000000LU) == 0) {
          if(idx->hash == uhash[u]) {
            gotpm = true;
            break;
          }
        }
      }
      if(gotpm) {
        PMrk->Append(Msgn->at(n-1));
        gotpm = false;
      }
      n++;
    }
  }

  if(WideDebug) {
    WideLog->printf("- %s: t:%u, l:%u, fm:%lu, hm:%lu, lr:%lu, u:%u, pm:%i",
      echoid(),
      Msgn->Count(),
      lastread,
      _firstmsgno,
      _lastmsgno,
      _lastread,
      wide->userno,
      __scanpm ? (int)PMrk->Count() : -1
    );
  }

  // Free index buffer if just counting
  if(not __keep_index or __scanpm)
    throw_xrelease(data->idx);

  if(_was_data == NULL) {
    data = NULL;
    wide = NULL;
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void SquishArea::scan() {

  GFTRK("SquishScan");

  raw_scan(true);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void SquishArea::scan_area() {

  GFTRK("SquishScanArea");

  raw_scan(false);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void SquishArea::scan_area_pm() {

  GFTRK("SquishScanAreaPM");

  raw_scan(true, true);
  Msgn->Reset();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------
