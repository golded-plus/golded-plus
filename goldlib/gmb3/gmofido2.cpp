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

#include <cstdlib>
#include <gdbgtrk.h>
#include <gdirposx.h>
#include <gstrall.h>
#include <gmofido.h>


//  ------------------------------------------------------------------

void FidoArea::raw_scan(bool __scanpm) {

  GFTRK("FidoArea::raw_scan");

  int _wasopen = isopen;
  if(not _wasopen) {
    isopen++;
    if(ispacked()) {
      Path tmp;
      strxcpy(tmp, path(), sizeof(Path));
      StripBackslash(tmp);
      const char* newpath = Unpack(tmp);
      if(newpath == NULL)
        packed(false);
      else {
        strcpy(tmp, newpath);
        AddBackslash(tmp);
      }
      set_real_path(newpath ? tmp : path());
    }
  }

  register uint _active = 0;
  register ulong* _msgnoptr = NULL;
  register ulong* _msgndx = Msgn->tag;

  gposixdir d(real_path());
  if(WideDebug)
    WideLog->printf("- %s/*.msg", d.fullpath());
  const gdirentry *de;
  while((de = d.nextentry("*.msg", true)) != NULL) {
    if(WideDebug)
      WideLog->printf("- %s", de->name.c_str());
    register ulong _msgno = (ulong)atol(de->name.c_str());
    if(_msgno) {
      if((_active % FIDO_SCANBUFSIZE) == 0) {
        _msgndx = Msgn->Resize(_active+FIDO_SCANBUFSIZE);
        _msgnoptr = _msgndx + _active;
      }
      *_msgnoptr++ = _msgno;
      _active++;
    }
  }
  
  // Sort the index
  Msgn->SetCount(_active);
  Msgn->Sort();

  // Get the lastread msgno
  word _lastread = 0;
  int _fh = ::sopen(AddPath(real_path(), wide->fidolastread), O_RDONLY|O_BINARY, WideSharemode, S_STDRD);
  if(_fh != -1) {
    lseekset(_fh, wide->userno, sizeof(word));
    read(_fh, &_lastread, sizeof(word));
    ::close(_fh);
  }

  _msgnoptr = _msgndx;

  register uint _count = 1;
  register uint _lastread_reln = 0;
  register ulong _firstmsgno = 0;
  register ulong _lastmsgno = 0;
  register ulong _lastreadfound = 0;
  FidoHdr _hdr;

  if(_active) {

    _firstmsgno = Msgn->at(0);
    _lastmsgno = Msgn->at(_active-1);

    while(1) {

      // Set lastread pointer
      if((*_msgnoptr >= _lastread) and (_lastread_reln == 0)) {
        _lastreadfound = *_msgnoptr;
        _lastread_reln = _count - (*_msgnoptr != _lastread ? 1 : 0);
        break;
      }
      if((++_count) > _active)
        break;
      _msgnoptr++;
    }
  }

  // If the exact lastread was not found
  if(_active and (_lastreadfound != _lastread)) {

    // Higher than highest or lower than lowest?
    if(_lastread > _lastmsgno)
      _lastread_reln = _active;
    else if(_lastread < _firstmsgno)
      _lastread_reln = 0;
  }

  // Read highwater mark
  data->highwatermark = 0;
  if(isecho() and wide->fidohwmarks) {
    _fh = test_open(AddPath(real_path(), "1.msg"), O_RDONLY|O_BINARY, WideSharemode);
    if(_fh != -1) {
      read(_fh, &_hdr, sizeof(FidoHdr));
      data->highwatermark = _hdr.replyto;
      ::close(_fh);
    }
  }

  // Update area data
  Msgn->SetCount(_active);
  lastread = _lastread_reln;
  lastreadentry = _lastreadfound;

  if(__scanpm) {
    char hdr_by[37];
    int umax = (WidePersonalmail & PM_ALLNAMES) ? WideUsernames : 1;
    bool gotpm = false;

    PMrk->ResetAll();
    
    for(uint i = lastread+1; i<=Msgn->Count(); i++) {
      // Build message filename
      Path _msgfile;
      ulong msgno = Msgn->CvtReln(i);
      build_msgname(_msgfile, msgno);

      // Open the message file
      int _fh = test_open(_msgfile, O_RDONLY|O_BINARY, WideSharemode);
      if(_fh >= 0) {
        read(_fh, &_hdr, sizeof(FidoHdr));
        strxcpy(hdr_by, _hdr.to, 36);

        for(int u=0; u<umax; u++) {
          if(strieql(WideUsername[u], hdr_by)) {
            gotpm = true;
            break;
          }
        }
        if(gotpm) {
          if(not(_hdr.attr & FIDO_RECEIVED))
            PMrk->Append(msgno);

          gotpm = false;
        }
        ::close(_fh);
      }
    }
  }
  
  if(WideDebug) {
    WideLog->printf("- %s: t:%u, l:%u, fm:%lu, hm:%lu, lr:%u, u:%u, pm: %i",
      echoid(),
      Msgn->Count(),
      lastread,
      _firstmsgno,
      _lastmsgno,
      _lastread,
      fidowide->userno,
      __scanpm ? (int)PMrk->Count() : -1
    );
  }

  if(not _wasopen) {
    if(ispacked()) {
      CleanUnpacked(real_path());
    }
    isopen--;
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void FidoArea::scan() {

  GFTRK("FidoArea::scan");

  scan_area();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void FidoArea::scan_area() {

  GFTRK("FidoArea::scan_area");

  bool was_open = data ? true : false;
  if(not was_open)
    data_open();
  raw_scan(false);
  if(not was_open)
    data_close();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void FidoArea::scan_area_pm() {

  GFTRK("FidoArea::scan_area_pm");

  bool was_open = data ? true : false;
  if(not was_open)
    data_open();
  raw_scan(true);
  if(not was_open)
    data_close();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void FidoArea::set_highwater_mark() {

  if(isecho()) {
    open();
    gmsg hmsg;
    hmsg.msgno = 1;
    if(load_hdr(&hmsg)) {
      hmsg.link.to_set(Msgn->CvtReln(Msgn->Count()));
      save_hdr(GMSG_UPDATE, &hmsg);
    }
    close();
  }
}


//  ------------------------------------------------------------------

void FidoArea::reset_highwater_mark() {

  if(isecho()) {
    open();
    gmsg hmsg;
    hmsg.msgno = 1;
    if(load_hdr(&hmsg)) {
      hmsg.link.to_set(Msgn->CvtReln(1));
      save_hdr(GMSG_UPDATE, &hmsg);
    }
    close();
  }
}


//  ------------------------------------------------------------------
