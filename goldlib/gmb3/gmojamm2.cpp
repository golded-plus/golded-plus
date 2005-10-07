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
//  JAM msgbase implementation, scanning.
//  ------------------------------------------------------------------

#include <cerrno>
#include <gdbgerr.h>
#include <gmemdbg.h>
#include <gdbgtrk.h>
#include <gstrall.h>
#include <gcrcall.h>
#include <gmojamm.h>


//  ------------------------------------------------------------------

void JamArea::data_open() {

  wide = jamwide;
  data = jamdata + (jamdatano++);
  data->fhjhr = data->fhjdt = data->fhjdx = data->fhjlr = data->fhjhw = -1;
  data->islocked = false;
  data->timesposted = 0;
  data->lastpos = 0;
}


//  ------------------------------------------------------------------

void JamArea::data_close() {

  jamdatano--;
}


//  ------------------------------------------------------------------

int JamArea::test_open(const char* file) {

  GFTRK("JamArea::test_open");

  int fh;
  long tries = 0;

  do {

    fh = ::sopen(file, O_RDWR|O_CREAT|O_BINARY, WideSharemode, S_STDRW);
    if(fh == -1) {

      // Tell the world
      if((errno != EACCES) or (PopupLocked(++tries, false, file) == false)) {
        WideLog->ErrOpen();
        raw_close();
        WideLog->printf("! A JAM msgbase file could not be opened.");
        WideLog->printf(": %s.", file);
        WideLog->ErrOSInfo();
        OpenErrorExit();
      }
    }
  } while(fh == -1);

  // Remove the popup window
  if(tries)
    PopupLocked(0, 0, NULL);

  GFTRK(NULL);

  return fh;
}


//  ------------------------------------------------------------------

void JamArea::raw_open() {

  GFTRK("JamArea::raw_open");

  Path file;
  sprintf(file, "%s.jhr", real_path());  data->fhjhr = test_open(file);
  sprintf(file, "%s.jdx", real_path());  data->fhjdx = test_open(file);
  sprintf(file, "%s.jlr", real_path());  data->fhjlr = test_open(file);
  if(not just_scanning) {
    sprintf(file, "%s.jdt", real_path());  data->fhjdt = test_open(file);
    if(not jamwide->smapihw) {
      sprintf(file, "%s.cmhw", real_path()); data->fhjhw = ::sopen(file, O_RDWR|O_BINARY, WideSharemode, S_STDRW);
    }
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void JamArea::raw_close() {

  GFTRK("JamArea::raw_close");

  if(data->fhjlr != -1) { ::close(data->fhjlr);  data->fhjlr = -1; }
  if(data->fhjdx != -1) { ::close(data->fhjdx);  data->fhjdx = -1; }
  if(data->fhjdt != -1) { ::close(data->fhjdt);  data->fhjdt = -1; }
  if(data->fhjhr != -1) { ::close(data->fhjhr);  data->fhjhr = -1; }
  if(data->fhjhw != -1) { ::close(data->fhjhw);  data->fhjhw = -1; }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void JamArea::open_area() {

  GFTRK("JamArea::open_area");

  // Open the msgbase files
  raw_open();

  // Read the header info
  memset(&data->hdrinfo, 0, sizeof(JamHdrInfo));
  read(data->fhjhr, &data->hdrinfo, sizeof(JamHdrInfo));

  if(not jamwide->smapihw and (data->fhjhw != -1)) {
    lseek(data->fhjhw, 0, SEEK_SET);
    read(data->fhjhw, &data->highwater, sizeof(int32_t));
  }
  else
    data->highwater = -1;

  // Is the signature invalid?
  if(memcmp(data->hdrinfo.signature, JAM_SIGNATURE, 4)) {

    // Initialize header info
    memcpy(data->hdrinfo.signature, JAM_SIGNATURE, 4);
    time_t a = time(NULL);
    struct tm *tp = gmtime(&a);
    tp->tm_isdst = -1;
    time_t b = mktime(tp);
    data->hdrinfo.datecreated = a + a - b;
    data->hdrinfo.passwordcrc = 0xFFFFFFFFL;
    data->hdrinfo.basemsgnum  = 1;

    // Write header info
    lseekset(data->fhjhr, 0);
    write(data->fhjhr, &data->hdrinfo, sizeof(JamHdrInfo));
  }

  // Adjust base msg number if necessary
  if(data->hdrinfo.basemsgnum == 0)
    data->hdrinfo.basemsgnum = 1;

  // Seek to beginning of the .JLR
  lseekset(data->fhjlr, 0);

  // Search for the userid (not the usercrc?!)
  data->lastpos = 0;
  int founduser = false;
  while(read(data->fhjlr, &data->lastrec, sizeof(JamLast)) == sizeof(JamLast)) {
    if(data->lastrec.usercrc == wide->usercrc) {
      founduser = true;
      break;
    }
    data->lastpos++;
  }

  // If user was not found, init the lastread with our values
  if(not founduser) {
    data->lastrec.usercrc = wide->usercrc;
    data->lastrec.userid  = wide->userid;
    data->lastrec.lastread = 0;
    data->lastrec.highread = 0;
  }

  data->timesposted = 0;

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void JamArea::raw_scan(int __keep_index, int __scanpm) {

  GFTRK("JamRawScan");

  // Open the msgbase if it wasn't already
  int _was_open = isopen;
  if(not _was_open) {
    if(not __keep_index or __scanpm)
      just_scanning = true;
    if(ispacked()) {
      const char* newpath = Unpack(path());
      if(newpath == NULL)
        packed(false);
      set_real_path(newpath ? newpath : path());
    }
    isopen++;
    data_open();
    open_area();
    just_scanning = false;
  }

  // Get some sizes
  int32_t _jdxlen = filelength(data->fhjdx);
  uint _jdxsize  = (uint)_jdxlen;
  uint _jdxtotal = _jdxsize / sizeof(JamIndex);

  // (Re)Allocate message index
  if(__keep_index)
    Msgn->Resize(_jdxtotal);

  // Allocate buffer to hold .JDX data
  JamIndex* _jdxbuf = (JamIndex*)throw_malloc(_jdxsize+1);

  // Read the entire .JDX file into memory
  lseekset(data->fhjdx, 0);
  read(data->fhjdx, _jdxbuf, _jdxsize);

  // Variables for the loop
  uint _active = 0;
  uint _firstmsgno = 0;
  uint _lastmsgno = 0;
  uint _lastreadfound = 0;
  uint _msgno = data->hdrinfo.basemsgnum;
  uint _total = data->hdrinfo.basemsgnum + _jdxtotal;
  uint _lastread = data->lastrec.lastread;
  uint _lastread_reln = 0;
  uint32_t* _msgndxptr = Msgn->tag;
  JamIndex* _jdxptr = _jdxbuf;

  // Fill message index
  while(_msgno < _total) {
    if(_jdxptr->hdroffset != 0xFFFFFFFFL) {
      _active++;
      if(not _firstmsgno)
        _firstmsgno = _msgno;
      if(__keep_index)
        *_msgndxptr++ = _msgno;
      if((_msgno >= _lastread) and (_lastread_reln == 0)) {
        _lastreadfound = _msgno;
        _lastread_reln = (uint)(_active - (_msgno != _lastread ? 1 : 0));
      }
      _lastmsgno = _msgno;
    }
    _jdxptr++;
    _msgno++;
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
    INam uname;
    int umax = (WidePersonalmail & PM_ALLNAMES) ? WideUsernames : 1;
    dword* ucrc = (dword*)throw_calloc(umax, sizeof(dword));
    for(int uc=0; uc<umax; uc++) {
      jamstrlwr(strcpy(uname, WideUsername[uc]));
      ucrc[uc] = strCrc32(uname, NO, CRC32_MASK_CCITT);
    }
    PMrk->ResetAll();
    uint n = lastread + 1;
    uint cnt = Msgn->Count();
    int gotpm = false;
    while(n <= cnt) {
      JamIndex* idx = _jdxbuf + (uint)(Msgn->at(n-1) - data->hdrinfo.basemsgnum);
      for(int u=0; u<umax; u++) {
        if(idx->usercrc == ucrc[u]) {
          gotpm = true;
          break;
        }
      }
      if(gotpm) {
        JamHdr hdr;
        lseekset(data->fhjhr, idx->hdroffset);
        read(data->fhjhr, &hdr, sizeof(JamHdr));
        if(not (hdr.attribute & JAMATTR_READ)) {
          if(not (hdr.attribute & JAMATTR_DELETED)) {
            PMrk->Append(hdr.messagenumber);
          }
        }
        gotpm = false;
      }
      n++;
    }
    throw_free(ucrc);
  }

  if(WideDebug) {
    WideLog->printf("- %s: t:%u, l:%u, fm:%u, hm:%u, lr:%u, u:%u, pm:%i",
      echoid(),
      Msgn->Count(),
      lastread,
      _firstmsgno,
      _lastmsgno,
      _lastread,
      data->lastpos,
      __scanpm ? (int)PMrk->Count() : -1
    );
  }

  // Free the .JDX buffer
  throw_free(_jdxbuf);

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


//  ------------------------------------------------------------------

void JamArea::scan() {

  GFTRK("JamArea::scan");

  raw_scan(true);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void JamArea::scan_area() {

  GFTRK("JamArea::scan_area");

  raw_scan(false);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void JamArea::scan_area_pm() {

  GFTRK("JamArea::scan_area_pm");

  raw_scan(true, true);
  Msgn->Reset();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------
