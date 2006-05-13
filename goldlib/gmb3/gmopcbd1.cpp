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

PcbData* pcbdata = NULL;
PcbWide* pcbwide = NULL;
int      pcbdatano = 0;


// ------------------------------------------------------------------

void PcbExit() {

  if(pcbwide) {
    throw_free(pcbwide->lastread);
    delete pcbwide->user;
  }
  throw_xrelease(pcbwide);
  throw_xrelease(pcbdata);
}


// ------------------------------------------------------------------

void PcbInit(const char* path, int userno) {

  pcbdata = (PcbData*)throw_calloc(3, sizeof(PcbData));
  pcbwide = (PcbWide*)throw_calloc(1, sizeof(PcbWide));

  pcbwide->path = path;
  pcbwide->userno = userno;

  pcbwide->fhusr = pcbwide->fhinf = -1;

  pcbwide->user = new PcbUser;
  throw_new(pcbwide->user);

  // Get PCBoard path
  Path _path;
  char* _ptr = getenv("PCBOARD");
  if(_ptr)
    AddBackslash(strcpy(_path, _ptr));
  else
    strcpy(_path, path);

  Path _cnamespath;
  *_cnamespath = NUL;

  // Open PCBOARD.DAT
  const char* _file = AddPath(_path, "PCBOARD.DAT");
  gfile fp(_file, "rt", WideSharemode);
  if (fp.isopen())
  {
    // Get some paths/filenames
    int _line = 0;
    char _buf[256];
    fp.SetvBuf(NULL, _IOFBF, 8192);
    while (fp.Fgets(_buf, sizeof(_buf)))
    {
      _line++;
      if(_line == 28)
        strxcpy(pcbwide->usersidxpath, strbtrim(_buf), sizeof(Path));
      else if(_line == 29)
        strxcpy(pcbwide->users, strbtrim(_buf), sizeof(Path));
      else if(_line == 31)
        strxcpy(_cnamespath, strbtrim(_buf), sizeof(Path));
      else if(_line == 180)
        strxcpy(pcbwide->usersinf, strbtrim(_buf), sizeof(Path));
      else if(_line == 208)
        pcbwide->foreign = atoi(_buf);
    }
    fp.Fclose();

    // Open CNAMES.@@@
    _file = AddPath(_cnamespath, ".@@@");
    fp.Fopen(_file, "rb", WideSharemode);

    // Get board numbers for lastread indexing in the userfiles
    word _recsize = 0;
    fp.SetvBuf(NULL, _IOFBF, 8192);
    fp.Fread(&_recsize, 2);
    if (_recsize)
    {
      PcbConf* _cnames = (PcbConf*)throw_calloc(1, _recsize);
      int _rec = 0;
      pcbwide->numareas = (int)((fp.FileLength()-2)/_recsize);
      pcbwide->confbytelen = (pcbwide->numareas/8) + ((pcbwide->numareas%8) != 0 ? 1 : 0);
      if(pcbwide->confbytelen < 5)
        pcbwide->confbytelen = 5;
      pcbwide->extconflen = pcbwide->confbytelen - 5;
      pcbwide->lastread = (int32_t*)throw_calloc(pcbwide->numareas, sizeof(int32_t));
      while (fp.Fread(_cnames, _recsize) == 1)
      {
        PcbAdjustArea((uint)_rec, _cnames->msgfile);
        _rec++;
      }
      throw_free(_cnames);
    }
    fp.Fclose();

    const char* _username = WideUsername[0];
    pcbwide->user->gufh = ::sopen(AddPath(_path, pcbwide->users), O_RDONLY|O_BINARY, WideSharemode, S_STDRD);
    if (pcbwide->user->gufh != -1)
    {
      pcbwide->user->fhinf = ::sopen(AddPath(_path, pcbwide->usersinf), O_RDONLY|O_BINARY, WideSharemode, S_STDRD);
      if(pcbwide->user->fhinf != -1) {
        ::read(pcbwide->user->fhinf, &pcbwide->usershdr, sizeof(PcbUsersInfHdr));
        pcbwide->usershdrsize = sizeof(PcbUsersInfHdr) + (pcbwide->usershdr.numofapps*sizeof(PcbUsersInfApp));
        pcbwide->user->usershdr = &pcbwide->usershdr;
        pcbwide->user->usershdrsize = pcbwide->usershdrsize;
        if(pcbwide->userno == -1) {
          pcbwide->user->find(_username);
          if(not pcbwide->user->found) {
            pcbwide->userno = 0;
            //WideLog->printf("* User \"%s\" not found in %sUSERS.", _username, _path);
            //pcbwide->user->add(_username);
            //WideLog->printf("* Now added with user number %u.", pcbwide->user->index);
          }
        }
        close(pcbwide->user->fhinf);
      }
      close(pcbwide->user->gufh);
    }
    pcbwide->userno = pcbwide->user->index;
  }
  else {

    WideLog->ErrOpen();
    WideLog->printf("! Configured for PCBoard, but can't find it.");
    WideLog->printf(": %s.", _file);
    WideLog->printf("+ Advice: Add or correct the PCBOARDPATH keyword.");
    WideLog->ErrOSInfo();
    OpenErrorExit();
  }
}


// ------------------------------------------------------------------

void PcbArea::open() {

  GFTRK("PcbOpen");

  isopen++;
  if(isopen > 2) {
    WideLog->ErrTest();
    WideLog->printf("! Trying to open a PCBoard msgbase more than twice.");
    WideLog->printf(": %s, %s.", echoid(), path());
    WideLog->printf("+ Info: This indicates a serious bug.");
    WideLog->printf("+ Advice: Report to the Author immediately.");
    TestErrorExit();
  }
  if(isopen == 1) {
    if(ispacked()) {
      isopen--;
      const char* newpath = Unpack(path());
      if(newpath == NULL)
        packed(false);
      set_real_path(newpath ? newpath : path());
      isopen++;
    }
    PcbWideOpen();
    data_open();
    raw_open();
    scan();
  }

  GFTRK(NULL);
}


// ------------------------------------------------------------------

void PcbArea::save_lastread() {

  GFTRK("PcbSaveLastread");

  dword tmplr = Msgn->CvtReln(lastread);
  if(board() < 40) {
    // Write lastreads to USERS file
    lseekset(wide->fhusr, wide->userno*sizeof(PcbUsers));
    read(wide->fhusr, &wide->usersrec, sizeof(PcbUsers));
    wide->usersrec.lastmsgread[board()] = L2B(tmplr);
    lseekset(wide->fhusr, wide->userno*sizeof(PcbUsers));
    write(wide->fhusr, &wide->usersrec, sizeof(PcbUsers));
  }
  else {
    // Write lastreads to USERS.INF file
    if(wide->extconflen) {
      int32_t _offset = (wide->usersrec.usersinfrec-1)*wide->usershdr.totalrecsize;
      _offset +=     wide->usershdrsize;
      _offset +=     wide->usershdr.sizeofrec;
      _offset += 2 * wide->confbytelen;
      _offset += 3 * wide->extconflen;
      _offset += sizeof(dword) * (board() - 40);
      lseekset(wide->fhinf, _offset);
      write(wide->fhinf, &tmplr, sizeof(dword));
    }
  }

  GFTRK(NULL);
}


// ------------------------------------------------------------------

void PcbArea::close() {

  GFTRK("PcbClose");

  if(isopen) {
    if(isopen == 1) {
      save_lastread();
      raw_close();
      Msgn->Reset();
      data_close();
      PcbWideClose();
      if(ispacked()) {
        CleanUnpacked(real_path());
      }
    }
    isopen--;
  }
  else {
    WideLog->ErrTest();
    WideLog->printf("! Trying to close an already closed PCBoard msgbase.");
    WideLog->printf(": %s, %s.", echoid(), path());
    WideLog->printf("+ Info: This indicates a potentially serious bug.");
    WideLog->printf("+ Advice: Report to the Author immediately.");
    TestErrorExit();
  }

  GFTRK(NULL);
}


// ------------------------------------------------------------------

void PcbArea::suspend() {

  GFTRK("PcbSuspend");

  save_lastread();
  raw_close();
  PcbWideClose();

  GFTRK(NULL);
}


// ------------------------------------------------------------------

void PcbArea::resume() {

  GFTRK("PcbResume");

  PcbWideOpen();
  raw_open();

  GFTRK(NULL);
}


// ------------------------------------------------------------------
