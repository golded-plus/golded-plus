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
//  WildCat! 4.x messagebase engine.
//  ------------------------------------------------------------------

#include <gdbgerr.h>
#include <gmemdbg.h>
#include <gdbgtrk.h>
#include <gmowcat.h>


//  ------------------------------------------------------------------

void WCatArea::refresh() {

  GFTRK("WCatRefresh");

  // Create new base record or read the existing one
  if(data->fhix != -1) {
    if(filelength(data->fhix) < sizeof(WCatBase)) {
      data->base.recsize = sizeof(WCatIdx);
      data->base.active = 0;
      data->base.nextmsgno = 1;
      lseekset(data->fhix, 0);
      write(data->fhix, &data->base, sizeof(WCatBase));
    }
    else {
      lseekset(data->fhix, 0);
      read(data->fhix, &data->base, sizeof(WCatBase));
    }
  }

  long ixnum = (filelength(data->fhix)-(long)sizeof(WCatBase)) / (long)sizeof(WCatIdx);

  // Are there any msgs?
  if(ixnum) {

    // Read the index file
    data->idx = (WCatIdx*)throw_realloc(data->idx, (uint)(ixnum*sizeof(WCatIdx)));
    lseekset(data->fhix, sizeof(WCatBase));
    read(data->fhix, data->idx, (uint)(ixnum*sizeof(WCatIdx)));
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void WCatArea::raw_scan(int __keep_index, int __scanpm) {

  GFTRK("WCatRawScan");

  WCatData* _was_data = data;
  if(_was_data == NULL) {
    data = wcatdata;
    wide = wcatwide;
  }

  // Load the lastread
  dword _lastread = 0;
  int _fh = ::sopen(AddPath(path(), ".lrd"), O_RDONLY|O_BINARY, WideSharemode, S_STDRD);
  if(_fh != -1) {
    lseekset(_fh, wide->userno, sizeof(dword));
    read(_fh, &_lastread, sizeof(dword));
    ::close(_fh);
  }

  // Open WildCat! files for scanning unless they are already open
  if(not isopen) {

    data->idx = NULL;
    data->base.recsize = 0;
    data->base.active = 0;
    data->base.nextmsgno = 0;

    // Open index file
    data->fhix = ::sopen(AddPath(path(), ".ix"), O_RDONLY|O_BINARY, WideSharemode, S_STDRD);
    if(data->fhix != -1) {

      // Allocate index buffer and read from file
      refresh();

      // Close index file
      ::close(data->fhix);
      data->fhix = -1;
    } 
  }

  register uint _active = 0;
  register uint _lastread_reln = 0;
  register ulong _lastreadfound = 0;

  if(data->base.active) {

    // (Re)allocate message index
    if(__keep_index)
      Msgn->Resize((uint)data->base.active);

    // Variables for the loop
    register ulong _msgno;
    register ulong* _msgndxptr = Msgn->tag;
    register WCatIdx* _ixptr = data->idx;
    register dword  _totalmsgs = data->base.active;
    register ulong _firstmsgno = _totalmsgs ? _ixptr->msgno : 0;
    register ulong _lastmsgno = 0;

    // Fill message index
    while(_active < _totalmsgs) {

      _active++;
      _msgno = (_ixptr++)->msgno;
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

void WCatArea::scan() {

  GFTRK("WCatScan");

  raw_scan(true);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void WCatArea::scan_area() {

  GFTRK("WCatScanArea");

  raw_scan(false);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void WCatArea::scan_area_pm() {

  GFTRK("WCatScanArea*M");

  raw_scan(true, true);
  Msgn->Reset();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------
