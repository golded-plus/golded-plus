//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999-2000 Alex. S. Aganichev
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
//  Hudson / Goldbase msgbase handling
//  ------------------------------------------------------------------


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
void _HudsArea<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::data_open() {

  wide = (HudsWide*) (__HUDSON ? (void *)hudsonwide : (void *)goldbasewide);
}


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
void _HudsArea<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::data_close() {

}


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
void _HudsWide<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::exit() {

  if(__HUDSON) {
    delete hudsonwide->user;
    throw_release(hudsonwide->msgidxptr);
  }
  else {
    delete goldbasewide->user;
    throw_release(goldbasewide->msgidxptr);
  }
}


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
void _HudsWide<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::init() {

  GFTRK("HudsInit");

  if(WideDebug)
    WideLog->printf("- Begin init for %s.", path);

  fhusr = -1;
  fhtxt = fhhdr = fhidx = -1;
  fhinf = fhlrd = fhtoi = -1;
  isopen = 0;
  islocked = false;
  timesposted = 0;
  msgidxsize = 0;
  msgidxptr = NULL;
  pmscan = NULL;
  scn = NULL;

  // Open complete msgbase, create if none exists
  if(not fexist(AddPath(path, __HUDSON ? "msghdr" HUDS_EXT : "msghdr" GOLD_EXT))) {
    WideLog->printf("* Creating new msgbase at %s", path);
    raw_open(O_CREAT);
    if(filelength(fhinf) == 0) {
      memset(&msginfo, 0, sizeof(HudsInfo));
      write(fhinf, &msginfo, sizeof(HudsInfo));
    }
    if(filelength(fhlrd) == 0) {
      memset(lastrec, 0, sizeof(HudsLast));
      write(fhlrd, lastrec, sizeof(HudsLast));
    }
  }
  else {
    raw_open();
  }

  // Lock msgbase to ensure data integrity
  lock();

  // Check if MSGTXT.BBS is approaching dangerous size
  if(__HUDSON and (filelength(fhtxt) > sizewarn))
    HudsSizewarn();

  // Check for mismatch between the header and the index files
  long _hdrsize = filelength(fhhdr)/(long)sizeof(HudsHdr);
  long _idxsize = filelength(fhidx)/(long)sizeof(HudsIdx);
  long _toisize = filelength(fhtoi)/(long)sizeof(HudsToIdx);
  if((_hdrsize != _idxsize) or (_hdrsize != _toisize)) {
    raw_close();
    HGWarnRebuild();
    WideLog->ErrIndex();
    WideLog->printf("! The %s msgbase files do not have the same number of records.", __HUDSON ? HUDS_NAME : GOLD_NAME);
    WideLog->printf(": %smsghdr%s   (%lu records).", path, __HUDSON ? HUDS_EXT : GOLD_EXT, _hdrsize);
    WideLog->printf(": %smsgidx%s   (%lu records).", path, __HUDSON ? HUDS_EXT : GOLD_EXT, _idxsize);
    WideLog->printf(": %smsgtoidx%s (%lu records).", path, __HUDSON ? HUDS_EXT : GOLD_EXT, _toisize);
    WideLog->printf("+ Advice: You need to run a msgbase index rebuild utility.");
    IndexErrorExit();
  }

  // Detect USERS.BBS format unless user has configured it
  if(__HUDSON and (ra2usersbbs == 0)) {

    // Get size of USERS.BBS
    long len = filelength(fhusr);

    // Does size match Hudson format?
    int hudsmatch = (len % sizeof(HudsUsers)) == 0;
    if(hudsmatch)
      ra2usersbbs = 1;

    // Does size match RA2 format?
    int ra2match = (len % sizeof(RA2Users)) == 0;
    if(ra2match)
      ra2usersbbs = 2;

    // If it matches both of them
    if(hudsmatch and ra2match) {

      // Check version in CONFIG.RA to make sure
      Path rapath, file;
      char* ptr = getenv("RA");
      if(ptr)
        AddBackslash(strcpy(rapath, ptr));
      MakePathname(file, rapath, "messages.ra");
      int fh = ::sopen(file, O_RDONLY|O_BINARY, WideSharemode, S_STDRD);
      if(fh != -1) {
        word VersionID = 0;
        read(fh, &VersionID, sizeof(word));
        if(VersionID >= 0x200)
          ra2usersbbs = 2;
        else
          ra2usersbbs = 1;
        ::close(fh);
      }
    }

    // If it does not match either of them
    if(not hudsmatch and not ra2match) {
      WideLog->ErrIndex();
      WideLog->printf("! The users.bbs file has an incorrect size.");
      WideLog->printf(": %susers.bbs, %lu bytes.", path, len);
      WideLog->printf(": Should be %lu bytes if it's in RA2 format.", (len/(long)sizeof(RA2Users))*(long)sizeof(RA2Users));
      WideLog->printf(": Should be %lu bytes if it's in Hudson format.", (len/(long)sizeof(HudsUsers))*(long)sizeof(HudsUsers));
      if(ra2usersbbs)
        WideLog->printf(": It appears to be in %s format.", (ra2usersbbs == 2) ? "RA2" : "Hudson");
      WideLog->printf("+ Advice: Run a userbase packing utility.");
      IndexErrorExit();
    }
  }

  if(__HUDSON) {
    fhuix = fhuxi = -1;
    if(ra2usersbbs == 2) {
      if(WideDebug)
        WideLog->printf("- Using a RA2 format userbase.");
      user = new RA2User;
    }
    else {
      if(WideDebug)
        WideLog->printf("- Using a Hudson format userbase.");
      user = new HudsonUser;
    }
  }
  else
    user = new GoldbaseUser;
  throw_new(user);

  // Open RA2 files
  if(__HUDSON and (ra2usersbbs == 2)) {
    RA2User* _user2 = (RA2User*)user;
    _user2->idxfh = fhuix = test_open("usersidx.bbs", O_CREAT);
    _user2->xifh = fhuxi = test_open("usersxi.bbs", O_CREAT);
  }

  // Find user
  const char* _username = WideUsername[0];
  if(userno == -1) {
    user->fh = fhusr;
    user->find(_username);
    if(not user->found) {
      WideLog->printf("* User \"%s\" not found in %susers%s.", _username, path, __HUDSON ? HUDS_EXT : GOLD_EXT);
      user->add(_username);
      WideLog->printf("* Now added with user number %u.", user->index);
    }
    userno = user->index;
  }

  if(WideDebug)
    WideLog->printf("- Using user record number %u.", userno);

  // Close RA2 files
  if(__HUDSON and (ra2usersbbs == 2)) {
    ::close(fhuix);
    ::close(fhuxi);
  }

  // Unlock and close
  unlock();
  raw_close();

  if(WideDebug)
    WideLog->printf("- End init for %s.", path);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
void _HudsWide<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::save_lastread(board_t board, msgn_t msgno) {

  GFTRK("HudsSaveLast");

  // Update lastread record
  msgn_t _lastread = lastrec[board-1] = msgno;
  lseekset(fhlrd, (long)userno*(long)sizeof(HudsLast));
  write(fhlrd, lastrec, sizeof(HudsLast));

  // Update user record
  user->fh = fhusr;
  user->moveto(userno);
  if(user->lastread() < _lastread)
    user->lastread(_lastread);
  if(timesposted) {
    user->inctimesposted(timesposted);
    timesposted = 0;
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
void _HudsArea<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::open() {

  GFTRK("HudsOpen");

  data_open();
  wide->isopen++;
  if(wide->isopen == 1) {
    wide->raw_open();
    wide->refresh();
    wide->timesposted = 0;
  }
  if(wide->isopen > 2) {
    WideLog->ErrTest();
    goto error;
  }

  isopen++;
  if(isopen > 2) {
    WideLog->ErrTest();
    goto error;
  }

  scan();
  goto done;

error:
  WideLog->printf("! Trying to open a %s msgbase more than twice.", __HUDSON ? HUDS_NAME : GOLD_NAME);
  WideLog->printf(": %s, board %u.", echoid(), board());
  WideLog->printf("+ Info: This indicates a serious bug.");
  WideLog->printf("+ Advice: Report to the Author immediately.");
  TestErrorExit();

done:
  GFTRK(NULL);
}


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
void _HudsArea<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::close() {

  GFTRK("HudsClose");

  if(isopen) {
    if(isopen == 1) {
      wide->save_lastread((board_t)board(), (msgn_t)Msgn->CvtReln(lastread));
      Msgn->Reset();
    }
    isopen--;
  }
  else {
    WideLog->ErrTest();
    goto error;
  }

  if(wide->isopen) {
    if(wide->isopen == 1) {
      if(wide->islocked)
        wide->unlock();
      wide->raw_close();
      throw_release(wide->msgidxptr);
    }
    wide->isopen--;
  }
  else {
    WideLog->ErrTest();
    goto error;
  }
  goto done;

error:
  WideLog->printf("! Trying to close an already closed %s msgbase.", __HUDSON ? HUDS_NAME : GOLD_NAME);
  WideLog->printf(": %s, board %u.", echoid(), board());
  WideLog->printf("+ Info: This indicates a potentially serious bug.");
  WideLog->printf("+ Advice: Report to the Author immediately.");
  TestErrorExit();

done:
  data_close();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
void _HudsArea<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::suspend() {

  GFTRK("HudsSuspend");

  wide->save_lastread((board_t)board(), (msgn_t)Msgn->CvtReln(lastread));
  wide->raw_close();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
void _HudsArea<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::resume() {

  GFTRK("HudsResume");

  wide->raw_open();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------
