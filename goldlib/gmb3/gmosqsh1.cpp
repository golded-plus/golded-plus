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

#include <gdbgerr.h>
#include <gmemdbg.h>
#include <gdbgtrk.h>
#include <gstrall.h>
#include <gmosqsh.h>

              
//  ------------------------------------------------------------------

SqshData* squishdata = NULL;
SqshWide* squishwide = NULL;
int       squishdatano = 0;


//  ------------------------------------------------------------------

void SquishArea::data_open() {

  wide = squishwide;
  data = squishdata + (squishdatano++);
  data->islocked = false;
  data->softlock = false;
  data->fhsqd = data->fhsqi = -1;
  data->idx = NULL;
}


//  ------------------------------------------------------------------

void SquishArea::data_close() {

  squishdatano--;
}


//  ------------------------------------------------------------------

void SquishExit() {

  if(squishwide)
    delete squishwide->user;
  throw_release(squishwide);
  throw_release(squishdata);
}


//  ------------------------------------------------------------------

void SquishInit(const char* userpath, int userno, int direct, int recycle, int squishscan) {

  squishdata = (SqshData*)throw_calloc(2, sizeof(SqshData));
  squishwide = (SqshWide*)throw_calloc(1, sizeof(SqshWide));

  squishwide->userpath = userpath;
  squishwide->userno = userno;
  squishwide->direct = direct;
  squishwide->recycle = recycle;
  squishwide->squishscan = squishscan;

  squishwide->user = new MaximusUser;
  throw_new(squishwide->user);

  Path userfile;
  strxcpy(userfile, userpath, sizeof(Path));
  AddPath(userfile, "USER.BBS");
  const char* _username = WideUsername[0];
  if(squishwide->userno == -1) {
    squishwide->user->fh = ::sopen(userfile, O_RDWR|O_CREAT|O_BINARY, WideSharemode, S_STDRW);
    if(squishwide->user->fh != -1) {
      squishwide->user->find(_username);
      if(NOT squishwide->user->found) {
        WideLog->printf("* User \"%s\" not found in %s.", _username, userfile);
        squishwide->user->add(_username);
        WideLog->printf("* Now added with user number %u.", squishwide->user->index);
      }
      close(squishwide->user->fh);
    }
    squishwide->userno = squishwide->user->index;
  }
}


//  ------------------------------------------------------------------

void SquishArea::raw_close() {

  GFTRK("SquishRawClose");

  if(data->fhsqi != -1)  ::close(data->fhsqi);  data->fhsqi = -1;
  if(data->fhsqd != -1)  ::close(data->fhsqd);  data->fhsqd = -1;

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

int SquishArea::test_open(const char* __file) {

  GFTRK("SquishTestOpen");

  int _fh;
  long _tries = 0;

  do {

    _fh = ::sopen(__file, O_RDWR|O_BINARY|O_CREAT, WideSharemode, S_STDRW);
    if(_fh == -1) {

      // Tell the world
      if(PopupLocked(++_tries, false, __file) == false) {

        // User requested to exit
        WideLog->ErrOpen();
        raw_close();
        WideLog->printf("! A Squish msgbase file could not be opened.");
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


//  ------------------------------------------------------------------

void SquishArea::raw_open() {

  GFTRK("SquishRawOpen");

  data->fhsqd = test_open(AddPath(path(), ".sqd"));
  data->fhsqi = test_open(AddPath(path(), ".sqi"));

  GFTRK(NULL);
}


//  ------------------------------------------------------------------
//  Open the Squish message base

void SquishArea::open() {

  GFTRK("SquishOpen");

  isopen++;
  if(isopen > 2) {
    WideLog->ErrTest();
    WideLog->printf("! Trying to open a Squish msgbase more than twice.");
    WideLog->printf(": %s, %s.", echoid(), path());
    WideLog->printf("+ Info: This indicates a serious bug.");
    WideLog->printf("+ Advice: Report to the Author immediately.");
    TestErrorExit();
  }
  if(isopen == 1) {
    data_open();
    raw_open();
    refresh();
    scan();
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void SquishArea::save_lastread() {

  GFTRK("SquishSaveLastread");

  int _fh = ::sopen(AddPath(path(), ".sql"), O_RDWR|O_CREAT|O_BINARY, WideSharemode, S_STDRW);
  if(_fh != -1) {
    lseekset(_fh, wide->userno, sizeof(dword));
    dword _lastread = Msgn->CvtReln(lastread);
    write(_fh, &_lastread, sizeof(dword));
    ::close(_fh);
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void SquishArea::close() {

  GFTRK("SquishClose");

  if(isopen) {
    if(isopen == 1) {
      save_lastread();
      raw_close();
      Msgn->Reset();
      throw_xrelease(data->idx);
      data_close();
    }
    isopen--;
  }
  else {
    WideLog->ErrTest();
    WideLog->printf("! Trying to close an already closed Squish msgbase.");
    WideLog->printf(": %s, %s.", echoid(), path());
    WideLog->printf("+ Info: This indicates a potentially serious bug.");
    WideLog->printf("+ Advice: Report to the Author immediately.");
    TestErrorExit();
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void SquishArea::suspend() {

  GFTRK("SquishSuspend");

  save_lastread();
  raw_close();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void SquishArea::resume() {

  GFTRK("SquishResume");

  raw_open();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------
