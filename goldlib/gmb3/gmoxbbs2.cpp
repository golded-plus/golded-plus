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

#include <gmemdbg.h>
#include <gdbgtrk.h>
#include <gmoxbbs.h>


//  ------------------------------------------------------------------

void XbbsArea::refresh() {

  GFTRK("XbbsRefresh");

  long indexnum = filelength(data->fhindex) / (long)sizeof(XbbsIdx);

  // Are there any msgs?
  if(indexnum) {

    // Read the index file
    data->idx = (XbbsIdx*)throw_realloc(data->idx, (uint)(indexnum*sizeof(XbbsIdx)));
    lseekset(data->fhindex, 0);
    read(data->fhindex, data->idx, (uint)(indexnum*sizeof(XbbsIdx)));
  }

  data->idx_size = (uint)indexnum;

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void XbbsArea::raw_scan(int __keep_index, int __scanpm) {

  GFTRK("XbbsRawScan");

  XbbsData* _was_data = data;
  if(_was_data == NULL) {
    data = xbbsdata;
    wide = xbbswide;
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
  ulong _lastread = 0;
  int _fh = ::sopen(AddPath(real_path(), ".lmr"), O_RDONLY|O_BINARY, WideSharemode, S_STDRD);
  if(_fh != -1) {
    lseekset(_fh, wide->userno+1, sizeof(ulong));
    read(_fh, &_lastread, sizeof(ulong));
    ::close(_fh);
  }

  // Open AdeptXBBS files for scanning unless they are already open
  if(not _wasopen) {

    data->idx = NULL;
    data->idx_size = 0;

    // Open index file
    data->fhindex = ::sopen(AddPath(real_path(), ".Index"), O_RDONLY|O_BINARY, WideSharemode, S_STDRD);
    if(data->fhindex != -1) {

      // Allocate index buffer and read from file
      refresh();

      // Close index file
      ::close(data->fhindex);
      data->fhindex = -1;
    } 
    if(ispacked()) {
      CleanUnpacked(real_path());
    }
    isopen--;
  }

  register uint _active = 0;
  register uint _lastread_reln = 0;
  register ulong _lastreadfound = 0;

  if(data->idx_size) {

    // (Re)allocate message index
    if(__keep_index)
      Msgn->Resize(data->idx_size);

    // Variables for the loop
    register ulong _msgno;
    register ulong* _msgndxptr = Msgn->tag;
    register dword  _totalmsgs = data->idx_size;
    register ulong _firstmsgno = _totalmsgs ? 1 : 0;
    register ulong _lastmsgno = 0;

    // Fill message index
    while(_active < _totalmsgs) {

      _active++;
      _msgno = _active;
      if(not _firstmsgno)
        _firstmsgno = _msgno;
      if(__keep_index)
        *_msgndxptr++ = _msgno;

      // Get the lastread
      if((_msgno >= _lastread) and (_lastread_reln == 0)) {
        _lastreadfound = _msgno;
        _lastread_reln = (uint)(_active - (_msgno != _lastread ? 1 : 0));
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
    // Not implemented yet
  }

  // Free index buffer if just counting
  if(not __keep_index or __scanpm)
    throw_release(data->idx);

  if(_was_data == NULL) {
    data = NULL;
    wide = NULL;
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void XbbsArea::scan() {

  GFTRK("XbbsScan");

  raw_scan(true);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void XbbsArea::scan_area() {

  GFTRK("XbbsScanArea");

  raw_scan(false);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void XbbsArea::scan_area_pm() {

  GFTRK("XbbsScanArea*M");

  raw_scan(true, true);
  Msgn->Reset();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------
