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
#include <cerrno>
#include <gdbgerr.h>
#include <gmemdbg.h>
#include <gdbgtrk.h>
#include <gstrall.h>
#include <gmofido.h>


//  ------------------------------------------------------------------

FidoWide* fidowide = NULL;
FidoData* fidodata = NULL;
int       fidodatano = 0;


//  ------------------------------------------------------------------

void FidoArea::data_open() {

  wide = fidowide;
  data = fidodata + (fidodatano++);
}


//  ------------------------------------------------------------------

void FidoArea::data_close() {

  fidodatano--;
}


//  ------------------------------------------------------------------

char* FidoArea::build_msgname(char* __buf, ulong __msgno) {

  sprintf(__buf, "%s%lu.msg", real_path(), __msgno);
  return __buf;
}


//  ------------------------------------------------------------------

int FidoArea::test_open(const char* __file, int __openmode, int __sharemode, int __fail) {

  GFTRK("FidoTestOpen");

  int _fh;
  long _tries = 0;

  do {

    _fh = ::sopen(__file, __openmode, __sharemode, S_STDRW);
    if(_fh == -1) {

      if((errno != EACCES) or (PopupLocked(++_tries, false, __file) == false)) {

        // Return instead of halting if requested
        if((errno != EACCES) and not __fail) {
          GFTRK(NULL);
          return _fh;
        }

        // User requested to exit
        WideLog->ErrOpen();
        WideLog->printf("! A Fido message file could not be opened.");
        WideLog->printf(": %s", __file);
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


//  ------------------------------------------------------------------

void FidoExit() {

  if(fidowide)
    delete fidowide->user;
  throw_release(fidowide);
  throw_release(fidodata);
}


//  ------------------------------------------------------------------

void FidoInit(const char* fidolastread, int fidohwmarks, int fidonullfix, int fidouserno, const char* squishuserpath) {

  fidodata = (FidoData*)throw_calloc(3, sizeof(FidoData));
  fidowide = (FidoWide*)throw_calloc(1, sizeof(FidoWide));

  fidowide->fidolastread = fidolastread;
  fidowide->fidohwmarks = fidohwmarks;
  fidowide->fidonullfix = fidonullfix;
  fidowide->userno = fidouserno;
  fidowide->squishuserpath = squishuserpath;

  fidowide->user = new MaximusUser;
  throw_new(fidowide->user);

  const char* _username = WideUsername[0];
  if(fidowide->userno == -1) {
    Path userfile;
    strxcpy(userfile, AddPath(fidowide->squishuserpath, "USER.BBS"), sizeof(Path));
    fidowide->user->fh = ::sopen(userfile, O_RDWR|O_CREAT|O_BINARY, WideSharemode, S_STDRW);
    if(fidowide->user->fh != -1) {
      fidowide->user->find(_username);
      if(not fidowide->user->found) {
        WideLog->printf("* User \"%s\" not found in %s.", _username, userfile);
        fidowide->user->add(_username);
        WideLog->printf("* Now added with user number %u.", fidowide->user->index);
      }
      ::close(fidowide->user->fh);
    }
    fidowide->userno = fidowide->user->index;
  }
}


//  ------------------------------------------------------------------

void FidoArea::open() {

  GFTRK("FidoOpen");

  isopen++;
  if(isopen > 2) {
    WideLog->ErrTest();
    WideLog->printf("! Trying to open a *.MSG msgbase more than twice.");
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
    data_open();
    scan();
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void FidoArea::save_lastread() {

  GFTRK("FidoSaveLastread");

  int _fh = ::sopen(AddPath(real_path(), wide->fidolastread), O_RDWR|O_CREAT|O_BINARY, WideSharemode, S_STDRW);
  if(_fh != -1) {
    word _lastread = (word)Msgn->CvtReln(lastread);
    lseekset(_fh, wide->userno, sizeof(word));
    write(_fh, &_lastread, sizeof(word));
    ::close(_fh);
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void FidoArea::close() {

  GFTRK("FidoClose");

  if(isopen) {
    if(isopen == 1) {
      save_lastread();
      Msgn->Reset();
      data_close();
      if(ispacked()) {
        CleanUnpacked(real_path());
      }
    }
    isopen--;
  }
  else {
    WideLog->ErrTest();
    WideLog->printf("! Trying to close an already closed *.MSG msgbase.");
    WideLog->printf(": %s, %s.", echoid(), path());
    WideLog->printf("+ Info: This indicates a potentially serious bug.");
    WideLog->printf("+ Advice: Report to the Author immediately.");
    TestErrorExit();
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void FidoArea::suspend() {

  GFTRK("FidoSuspend");

  save_lastread();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void FidoArea::resume() {

  GFTRK("FidoResume");

  GFTRK(NULL);
}


//  ------------------------------------------------------------------
