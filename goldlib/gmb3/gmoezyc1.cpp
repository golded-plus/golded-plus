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
//  Ezycom msgbase handling
//  ------------------------------------------------------------------

#include <cstdlib>
#include <gdbgerr.h>
#include <gmemdbg.h>
#include <gdbgtrk.h>
#include <gstrall.h>
#include <glog.h>
#include <gmoezyc.h>


//  ------------------------------------------------------------------

EzycWide* ezycomwide = NULL;
EzycData* ezycomdata = NULL;
int       ezycomdatano = 0;


//  ------------------------------------------------------------------

void EzycomArea::data_open() {

  wide = ezycomwide;
  data = ezycomdata + (ezycomdatano++);
  data->fhhdr = data->fhtxt = data->fhnow = -1;
  data->omode = O_RDONLY;
  data->smode = SH_DENYNO;
  data->timesposted = 0;
}


//  ------------------------------------------------------------------

void EzycomArea::data_close() {

  ezycomdatano--;
}


//  ------------------------------------------------------------------

char* EzycomArea::ret_mess_xxx(char* __path, byte __type) {

  if(wide->ver >= 110) {
    sprintf(__path, "%sAREA%u\\M%c%05u.BBS",
      wide->msgbasepath,
      ((board()-1) / 100) + 1,
      (__type == 1) ? 'H' : 'T',
      board()
    );
  }
  else {
    sprintf(__path, "%sAREA%u\\MSG%c%03u.BBS",
      wide->msgbasepath,
      ((board()-1) / 100) + 1,
      (__type == 1) ? 'H' : 'T',
      (board() > 999) ? board()/10 : board()
    );
  }
  return __path;
}


//  ------------------------------------------------------------------

char* EzycomArea::ret_mess_area(char* __path) {

  sprintf(__path, "%sAREA%u",
    wide->msgbasepath,
    ((board()-1) / 100) + 1
  );
  return __path;
}


//  ------------------------------------------------------------------

void EzycomArea::raw_close() {

  GFTRK("EzycomRawClose");

  if(data->fhhdr != -1)  ::close(data->fhhdr);  data->fhhdr = -1;
  if(data->fhtxt != -1)  ::close(data->fhtxt);  data->fhtxt = -1;
  if(data->fhnow != -1)  ::close(data->fhnow);  data->fhnow = -1;
  if(data->omode == O_WRONLY)
    remove(AddPath(wide->msgbasepath, "EZYMSG.NOW"));

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

int EzycomArea::test_open(const char* __file, int __mode, int __share) {

  GFTRK("EzycomTestOpen");

  int _fh;
  long _tries = 0;
  Path _path;
  strcpy(_path, __file);

  do {

    _fh = ::sopen(_path, __mode, __share, S_STDRW);
    if(_fh == -1) {

      // Tell the world
      if(PopupLocked(++_tries, false, _path) == false) {
        WideLog->ErrOpen();
        raw_close();
        WideLog->printf("! A Ezycom msgbase file could not be opened.");
        WideLog->printf(": %s.", _path);
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

int EzycomArea::raw_open() {

  GFTRK("EzycomRawOpen");

  int _tryagain = 0;

  do {

    int _sopen_access = data->omode | O_BINARY;
    int _sopen_permit = 0;

    if(not fexist(ret_mess_xxx(data->ezyfile,1))) {
      _sopen_access |= O_CREAT;
      _sopen_permit = S_STDRW;
    }

    ret_mess_xxx(data->ezyfile, 1);
    data->fhhdr = ::sopen(data->ezyfile, _sopen_access, data->smode, _sopen_permit);
    if(data->fhhdr != -1) {
      ret_mess_xxx(data->ezyfile, 2);
      data->fhtxt = ::sopen(data->ezyfile, _sopen_access, data->smode, _sopen_permit);
      if(data->fhtxt != -1) {
        if(data->omode == O_WRONLY) {

          // Create semaphore file
          byte _sema = 0;
          data->fhnow = ::sopen(AddPath(wide->msgbasepath, "EZYMSG.NOW"), O_WRONLY|O_CREAT|O_TRUNC|O_BINARY, WideSharemode, S_STDRW);
          write(data->fhnow, &_sema, 1);  // Write some dummy data
        }

        GFTRK(NULL);
        return true;
      }
    }
    raw_close();
    if(fexist(AddPath(wide->msgbasepath, "EZYMSG.NOW"))) {
      errno = EACCES;
      break;
    }
    Path _path;
    if(not is_dir(ret_mess_area(_path))) {
      mkdir(ret_mess_area(_path), S_IWUSR);
      _tryagain++;
    }
  } while(_tryagain == 1);

  GFTRK(NULL);

  return false;
}


//  ------------------------------------------------------------------

void EzycomArea::test_raw_open(int __fileline) {

  GFTRK("EzycomTestRawOpen");

  int _isopen;
  long _tries = 0;

  do {

    _isopen = raw_open();

    if(not _isopen) {

      // Tell the world
      if((errno != EACCES) or PopupLocked(++_tries, false, data->ezyfile) == false) {

        // User requested to exit
        WideLog->erropen(__FILE__, __fileline);
        WideLog->printf("! A Ezycom msgbase file could not be opened.");
        WideLog->printf(": %s.", data->ezyfile);
        WideLog->ErrOSInfo();
        OpenErrorExit();
      }
    }
  } while(not _isopen);

  // Remove the popup window
  if(_tries)
    PopupLocked(0, 0, NULL);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void EzycomExit() {

  if(ezycomwide)
    delete ezycomwide->user;
  throw_xrelease(ezycomwide);
  throw_xrelease(ezycomdata);
}


//  ------------------------------------------------------------------

void EzycomInit(const char* msgbasepath, const char* userbasepath, int userno) {

  ezycomdata = (EzycData*)throw_calloc(3, sizeof(EzycData));
  ezycomwide = (EzycWide*)throw_calloc(1, sizeof(EzycWide));

  ezycomwide->msgbasepath = msgbasepath;
  ezycomwide->userbasepath = userbasepath;
  ezycomwide->userno = userno;

  Path _path;
  *_path = NUL;
  char* _ptr = getenv("EZY");
  if(_ptr and *_ptr) {
    _ptr = strcpy(_path, _ptr);
    char* _ptr2 = strchr(_ptr, ' ');
    if(_ptr2)
      *_ptr2 = NUL;
    AddBackslash(_path);
  }
  const char* _file = "";
  _ptr = getenv("TASK");
  if(_ptr and *_ptr) {
    char _tmp[20];
    sprintf(_tmp, "CONFIG.%u", atoi(_ptr));
    _file = AddPath(_path, _tmp);
  }
  if(not fexist(_file))
    _file = AddPath(_path, "CONFIG.EZY");

  ezycomwide->ver = 102;
  ezycomwide->maxmess = EZYC_MAXMESS102;
  int _fh = ::sopen(_file, O_RDONLY|O_BINARY, WideSharemode, S_STDRD);
  if(_fh != -1) {
    char _verstr[9];
    read(_fh, _verstr, 9);
    close(_fh);
    strp2c(_verstr);
    if(strnicmp(_verstr, "1.10", 4) >= 0) {
      ezycomwide->ver = 110;
      ezycomwide->maxmess = EZYC_MAXMESS110;
    }
  }

  ezycomwide->user = new EzycomUser;
  throw_new(ezycomwide->user);

  const char* _username = WideUsername[0];
  ezycomwide->user->ver = ezycomwide->ver;
  if(ezycomwide->userno == -1) {
    ezycomwide->user->fh = ::sopen(AddPath(ezycomwide->userbasepath, "USERS.BBS"), O_RDWR|O_CREAT|O_BINARY, WideSharemode, S_STDRW);
    if(ezycomwide->user->fh != -1) {
      ezycomwide->user->extfh = ::sopen(AddPath(ezycomwide->userbasepath, "USERSEXT.BBS"), O_RDWR|O_CREAT|O_BINARY, WideSharemode, S_STDRW);
      if(ezycomwide->user->extfh != -1) {
        ezycomwide->user->find(_username);
        if(not ezycomwide->user->found) {
          WideLog->printf("* User \"%s\" not found in %sUSERS.BBS.", _username, ezycomwide->userbasepath);
          ezycomwide->user->add(_username);
          WideLog->printf("* Now added with user number %u.", ezycomwide->user->index);
        }
        close(ezycomwide->user->extfh);
      }
      close(ezycomwide->user->fh);
    }
    ezycomwide->userno = ezycomwide->user->index;
  }
}


//  ------------------------------------------------------------------

void EzycomArea::open() {

  GFTRK("EzycomOpen");

  isopen++;
  if(isopen > 2) {
    WideLog->ErrTest();
    WideLog->printf("! Trying to open a Ezycom msgbase more than twice.");
    WideLog->printf(": %s, board %u.", echoid(), board());
    WideLog->printf("+ Info: This indicates a serious bug.");
    WideLog->printf("+ Advice: Report to the Author immediately.");
    TestErrorExit();
  }
  if(isopen == 1) {
    data_open();
    test_raw_open(__LINE__);
    scan();
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void EzycomArea::save_lastread() {

  GFTRK("EzycomSaveLastread");

  int _fh = test_open(AddPath(wide->userbasepath, "LASTCOMB.BBS"), O_RDWR|O_CREAT|O_BINARY, SH_DENYNO);
  if(_fh != -1) {
    word _lastread = (word)(Msgn->CvtReln(lastread)+1);
    lseekset(_fh, (long)wide->userno * ((long)wide->maxmess / 16) * (long)sizeof(EzycLast) +
      (((board() - 1) / 16) * sizeof(EzycLast) + sizeof(word)) +
      (board()-1) % 16 * sizeof(word)
    );
    write(_fh, &_lastread, sizeof(word));
    ::close(_fh);
  }

  if(data->timesposted) {
    wide->user->extfh = ::sopen(AddPath(wide->userbasepath, "USERSEXT.BBS"), O_RDWR|O_BINARY, SH_DENYNO, S_STDRW);
    if(wide->user->extfh != -1) {
      wide->user->moveto(wide->userno);
      wide->user->inctimesposted(data->timesposted);
      data->timesposted = 0;
      ::close(wide->user->extfh);
    }
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void EzycomArea::close() {

  GFTRK("EzycomClose");

  if(isopen) {
    if(isopen == 1) {
      save_lastread();
      raw_close();
      Msgn->Reset();
      data_close();
    }
    isopen--;
  }
  else {
    WideLog->ErrTest();
    WideLog->printf("! Trying to close an already closed Ezycom msgbase.");
    WideLog->printf(": %s, board %u.", echoid(), board());
    WideLog->printf("+ Info: This indicates a potentially serious bug.");
    WideLog->printf("+ Advice: Report to the Author immediately.");
    TestErrorExit();
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void EzycomArea::suspend() {

  GFTRK("EzycomSuspend");

  save_lastread();
  raw_close();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void EzycomArea::resume() {

  GFTRK("EzycomResume");
  if(not raw_open()) {
    Path _path;
    WideLog->ErrOpen();
    WideLog->printf("! A Ezycom msgbase file could not be opened.");
    WideLog->printf(": %s.", ret_mess_xxx(_path, 1));
    WideLog->ErrOSInfo();
    OpenErrorExit();
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------
