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

WCatWide* wcatwide = NULL;
WCatData* wcatdata = NULL;
int       wcatdatano = 0;


//  ------------------------------------------------------------------

void WCatArea::data_open() {

  wide = wcatwide;
  data = wcatdata + (wcatdatano++);
}


//  ------------------------------------------------------------------

void WCatArea::data_close() {

  wcatdatano--;
}


//  ------------------------------------------------------------------

void WCatArea::raw_close() {

  GFTRK("WCatRawClose");

  if(data->fhix != -1)   ::close(data->fhix);   data->fhix = -1;
  if(data->fhdat != -1)  ::close(data->fhdat);  data->fhdat = -1;

  GFTRK(0);
}


//  ------------------------------------------------------------------

int WCatArea::test_open(const char* __file) {

  GFTRK("WCatTestOpen");

  int _fh;
  long _tries = 0;

  do {

    _fh = ::sopen(__file, O_RDWR|O_BINARY|O_CREAT, WideSharemode, S_STDRW);
    if(_fh == -1) {

      // Tell the world
      if((errno != EACCES) or (PopupLocked(++_tries, false, __file) == false)) {

        // User requested to exit
        WideLog->ErrOpen();
        raw_close();
        WideLog->printf("! A WildCat! msgbase file could not be opened.");
        WideLog->printf(": %s.", __file);
        WideLog->ErrOSInfo();
        OpenErrorExit();
      }
    }
  } while(_fh == -1);

  // Remove the popup window
  if(_tries)
    PopupLocked(0, 0, NULL);

  GFTRK(0);

  return _fh;
}


//  ------------------------------------------------------------------

void WCatArea::raw_open() {

  GFTRK("WCatRawOpen");

  data->fhix  = test_open(AddPath(real_path(), ".ix"));
  data->fhdat = test_open(AddPath(real_path(), ".dat"));

  GFTRK(0);
}


//  ------------------------------------------------------------------

void WCatExit() {

  throw_xrelease(wcatwide);
  throw_xrelease(wcatdata);
}


//  ------------------------------------------------------------------

void WCatInit(int userno) {

  wcatdata = (WCatData*)throw_calloc(3, sizeof(WCatData));
  wcatwide = (WCatWide*)throw_calloc(1, sizeof(WCatWide));

  wcatwide->userno = userno;
}


//  ------------------------------------------------------------------

void WCatArea::open() {

  GFTRK("WCatOpen");

  isopen++;
  if(isopen > 2) {
    WideLog->ErrTest();
    WideLog->printf("! Trying to open a WildCat! msgbase more than twice.");
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
    raw_open();
    refresh();
    scan();
  }

  GFTRK(0);
}


//  ------------------------------------------------------------------

void WCatArea::save_lastread() {

  GFTRK("WCatSaveLastread");

  int _fh = ::sopen(AddPath(real_path(), ".lrd"), O_RDWR|O_CREAT|O_BINARY, WideSharemode, S_STDRW);
  if(_fh != -1) {
    word _lastread = (word)Msgn->CvtReln(lastread);
    lseekset(_fh, wcatwide->userno, sizeof(word));
    write(_fh, &_lastread, sizeof(word));
    ::close(_fh);
  }

  GFTRK(0);
}


//  ------------------------------------------------------------------

void WCatArea::close() {

  GFTRK("WCatClose");

  if(isopen) {
    if(isopen == 1) {
      save_lastread();
      raw_close();
      Msgn->Reset();
      throw_release(data->idx);
      data_close();
      if(ispacked()) {
        CleanUnpacked(real_path());
      }
    }
    isopen--;
  }
  else {
    WideLog->ErrTest();
    WideLog->printf("! Trying to close an already closed WildCat! msgbase.");
    WideLog->printf(": %s, %s.", echoid(), path());
    WideLog->printf("+ Info: This indicates a potentially serious bug.");
    WideLog->printf("+ Advice: Report to the Author immediately.");
    TestErrorExit();
  }

  GFTRK(0);
}


//  ------------------------------------------------------------------

void WCatArea::suspend() {

  GFTRK("WCatSuspend");

  save_lastread();

  GFTRK(0);
}


//  ------------------------------------------------------------------

void WCatArea::resume() {

  GFTRK("WCatResume");



  GFTRK(0);
}


//  ------------------------------------------------------------------
