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
void _HudsWide<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::raw_close()
{
  GFTRK("HudsRawClose");

  fhtxt.Close();
  fhhdr.Close();
  fhidx.Close();
  fhinf.Close();
  fhlrd.Close();
  fhtoi.Close();
  fhusr.Close();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
void _HudsWide<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::test_open(gfile &__file, char* __fname, int __oaccess)
{
  GFTRK("HudsTestOpen");

  long _tries = 0;
  __oaccess |= O_RDWR|O_BINARY;

  do
  {
    Path _testfn;
    strcpy(_testfn, AddPath(path, __fname));
    int _omode = (__oaccess & O_CREAT) ? S_STDRW : S_STDRD;

    __file.Open(_testfn, __oaccess, WideSharemode, _omode);
    if (!__file.isopen())
    {
      if (errno == ENOENT)
      {
        if (_tries == 0)
        {
          __oaccess |= O_CREAT;
          _tries++;
          continue;
        }
      }

      // Request the other program to unlock
      if (errno != ENOENT)
        TouchFile(AddPath(path, "mbunlock.now"));

      // Tell the world
      if (PopupLocked(++_tries, false, _testfn) == false)
      {
        // User requested to exit
        WideLog->ErrOpen();
        raw_close();
        WideLog->printf("! A %s msgbase file could not be opened.", __HUDSON ? HUDS_NAME : GOLD_NAME);
        WideLog->printf(": %s.", _testfn);
        WideLog->ErrOSInfo();
        OpenErrorExit();
      }
    }
  }
  while (!__file.isopen());

  // Remove the popup window
  if (_tries)
    PopupLocked(0, 0, NULL);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
void _HudsWide<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::raw_open(int __oaccess, int __all) {

  GFTRK("HudsRawOpen");

  test_open(fhidx, __HUDSON ? "msgidx" HUDS_EXT : "msgidx" GOLD_EXT, __oaccess);
  test_open(fhinf, __HUDSON ? "msginfo" HUDS_EXT : "msginfo" GOLD_EXT, __oaccess);
  test_open(fhlrd, __HUDSON ? "lastread" HUDS_EXT : "lastread" GOLD_EXT, __oaccess);
  test_open(fhtoi, __HUDSON ? "msgtoidx" HUDS_EXT : "msgtoidx" GOLD_EXT, __oaccess);
  if (__all)
  {
    test_open(fhhdr, __HUDSON ? "msghdr" HUDS_EXT : "msghdr" GOLD_EXT, __oaccess);
    test_open(fhtxt, __HUDSON ? "msgtxt" HUDS_EXT : "msgtxt" GOLD_EXT, __oaccess);
    test_open(fhusr, __HUDSON ? "users" HUDS_EXT : "users" GOLD_EXT, __oaccess);
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
void _HudsWide<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::raw_open_scan() {

  GFTRK("HudsRawOpenScan");

  raw_open(0, false);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
void _HudsWide<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::refresh() {

  GFTRK("HudsRefresh");

  // (Re)Allocate memory to hold the complete MSGIDX.BBS/DAT
  msgidxsize = fhidx.FileLength();
  msgidxptr = (HudsIdx*)throw_realloc(msgidxptr, (uint)(msgidxsize+sizeof(HudsIdx)));

  // Load MSGIDX.BBS/DAT
  fhidx.LseekSet(0);
  fhidx.Read(msgidxptr, (uint)msgidxsize);

  // Load MSGINFO.BBS/DAT
  fhinf.LseekSet(0);
  fhinf.Read(&msginfo, sizeof(HudsInfo));

  // Load LASTREAD.BBS/DAT
  fhlrd.LseekSet(userno*sizeof(HudsLast));
  fhlrd.Read(lastrec, sizeof(HudsLast));

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
void _HudsWide<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::open() {

  GFTRK("HudsWideOpen");

  isopen = 1;
  iswideopen = true;
  ispmscanned = false;
  iswidescanned = false;
  raw_open_scan();
  lock();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
void _HudsWide<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::close() {

  GFTRK("HudsWideClose");

  unlock();
  raw_close();
  throw_release(msgidxptr);
  throw_release(pmscan);
  throw_release(scn);
  iswideopen = false;
  isopen = 0;

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
void _HudsWide<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::lock() {

  GFTRK("HudsLock");

  if(not islocked and WideCanLock) {

    long _tries = 0;

    // Try to get the lock
    while (fhinf.Lock(sizeof(HudsInfo)+1, 1) == -1)
    {
      // Tell the world
      if(PopupLocked(++_tries, true, AddPath(path, __HUDSON ? "msginfo" HUDS_EXT : "msginfo" GOLD_EXT)) == false) {

        // User requested to exit
        WideLog->ErrLock();
        raw_close();
        WideLog->printf("! A %s msgbase file could not be locked.", __HUDSON ? HUDS_NAME : GOLD_NAME);
        WideLog->printf(": %smsginfo%s.", path, __HUDSON ? HUDS_EXT : GOLD_EXT);
        WideLog->ErrOSInfo();
        LockErrorExit();
      }

      // Request the other program to unlock
      TouchFile(AddPath(path, "mbunlock.now"));
    }

    // Remove the popup window
    if(_tries)
      PopupLocked(0, 0, NULL);

    // We got the lock
    islocked = true;
  }

  // Refresh msgbase data
  refresh();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
void _HudsWide<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::unlock() {

  GFTRK("HudsUnlock");

  if (islocked and WideCanLock)
  {
    fhinf.Unlock(sizeof(HudsInfo)+1, 1);
    islocked = false;
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
void _HudsArea<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::lock() {

  wide->lock();
}


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
void _HudsArea<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::unlock() {

  wide->unlock();
}


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
void _HudsWide<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::scan() {
    
  GFTRK("HudsWideScan");

  iswidescanned = true;

  // Alloc scan array
  throw_free(scn);
  scn = (HudsScan*)throw_calloc((__HUDSON ? HUDS_MAXBOARD : GOLD_MAXBOARD), sizeof(HudsScan));

  // Board and scan array pointer
  register int _board;
  register HudsScan* _scan = scn;

  // Init wide scan array
  _board = 0;
  while(_board < (__HUDSON ? HUDS_MAXBOARD : GOLD_MAXBOARD)) {
    _scan->active = msginfo.active[_board];
    _scan->lastread = lastrec[_board];
    _board++;
    _scan++;
  }

  // Setup MSGIDX pointer and counters
  register HudsIdx* _msgidx_ptr = msgidxptr;
  register uint    _msgidx_count = 0;
  register uint    _msgidx_total = (uint)(msgidxsize/sizeof(HudsIdx));
  int         _invalidboards = 0;

  // Scan the index
  while(_msgidx_count < _msgidx_total) {

    // Is the msg not deleted?
    if(_msgidx_ptr->msgno != (__HUDSON ? HUDS_DELETEDMSGNO : GOLD_DELETEDMSGNO)) {

      register int _idxboard = _msgidx_ptr->board;
      if(_idxboard and (_idxboard <= (__HUDSON ? HUDS_MAXBOARD : GOLD_MAXBOARD))) {

        _scan = scn + (_idxboard - 1);

        // Get message number
        _scan->lastmsgno = _msgidx_ptr->msgno;
        _scan->count++;

        // Set first message number
        if(not _scan->firstmsgno)
          _scan->firstmsgno = _scan->lastmsgno;

        // Set lastread pointer
        if((_scan->lastmsgno >= _scan->lastread) and (_scan->lastreadreln == 0)) {
          _scan->lastreadfound = _scan->lastmsgno;
          _scan->lastreadreln = _scan->count - (_scan->lastmsgno != _scan->lastread ? 1 : 0);
        }
      }
      else {
        _invalidboards++;
      }
    }

    // Go to next record
    _msgidx_count++;
    _msgidx_ptr++;
  }

  if(_invalidboards) {
    WideLog->printf("! Found %u msgs with an invalid board number (0 or >%u).", _invalidboards, (__HUDSON ? HUDS_MAXBOARD : GOLD_MAXBOARD));
    WideLog->printf("! In the %s msgbase at %s.", __HUDSON ? HUDS_NAME : GOLD_NAME, path);
    WideLog->printf(": Info: Your msgbase may be partially corrupted.");
    WideLog->printf("+ Advice: Run a msgbase index rebuild/recover utility.");
  }

  // Check/fix boards
  _board = 0;
  _scan = scn;
  while(_board < (__HUDSON ? HUDS_MAXBOARD : GOLD_MAXBOARD)) {

    // Check/fix lastreads
    if(_scan->count and (_scan->lastreadfound != _scan->lastread)) {
      if(_scan->lastread > _scan->lastmsgno)
        _scan->lastreadreln = _scan->count;
      else if(_scan->lastread < _scan->firstmsgno)
        _scan->lastreadreln = 0;
    }

    if(WideDebug) {
      WideLog->printf("- b:%u: t:%u, l:%u, fm:%u, hm:%u, lr:%u, u:%u",
        _board,
        _scan->count,
        _scan->lastreadreln,
        _scan->firstmsgno,
        _scan->lastmsgno,
        _scan->lastread,
        userno
      );
    }

    // Check active message count and log it if different
    if(_scan->active != _scan->count) {
      WideLog->printf("! Counted %u active msgs in %s board %u.", _scan->count, __HUDSON ? HUDS_NAME : GOLD_NAME, _board+1);
      WideLog->printf("! According to msginfo%s there should be %u active msgs.", __HUDSON ? HUDS_EXT : GOLD_EXT, _scan->active);
      WideLog->printf(": Info: A program did not update msginfo%s correctly.", __HUDSON ? HUDS_EXT : GOLD_EXT);
      WideLog->printf("+ Advice: Run a msgbase index rebuild utility.");
    }

    _board++;
    _scan++;
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
void _HudsArea<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::raw_scan(int __keep_index) {
    
  GFTRK("HudsRawScan");

  if(!wide)
    wide = (HudsWide*) (__HUDSON ? (void *)hudsonwide : (void *)goldbasewide);

  if(wide->iswideopen and not wide->iswidescanned)
    wide->scan();

  // Get wide scan data if any
  if(wide->scn) {

    // Update area data
    register HudsScan* _scan = wide->scn + (board() - 1);
    Msgn->SetCount(_scan->count);
    lastread = _scan->lastreadreln;
    lastreadentry = _scan->lastreadfound;
  }
  else {

    // Open the msgbase if it wasn't already
    int _was_open = wide->isopen;
    if(not _was_open)
      wide->open();

    // Get the number of active msgs in the area
    register board_t _board = (board_t)board();
    register uint _active = wide->msginfo.active[_board-1];
    register msgn_t _lastread = wide->lastrec[_board-1];

    // Setup pointers and counts
    register uint    _msg_count = 0;
    register HudsIdx* _msgidx_ptr = wide->msgidxptr;
    register uint    _msgidx_count = 0;
    register uint    _msgidx_total = (uint)(wide->msgidxsize/sizeof(HudsIdx));
    register uint    _lastread_reln = 0;

    // (Re)Allocate index
    if(__keep_index)
      Msgn->Resize(_active);

    // Index pointers
    uint32_t* _msgno_ptr = Msgn->tag;

    // Fill index
    uint _firstmsgno = 0;
    uint _lastmsgno = 0;
    uint _lastreadfound = 0;
    while(_msgidx_count < _msgidx_total) {

      // Is it our board and is the msg not deleted?
      if((_msgidx_ptr->board == _board) and (_msgidx_ptr->msgno != (__HUDSON ? HUDS_DELETEDMSGNO : GOLD_DELETEDMSGNO))) {

        // Get message number
        _lastmsgno = _msgidx_ptr->msgno;
        _msg_count++;

        // Set first message number
        if(not _firstmsgno)
          _firstmsgno = _lastmsgno;

        // Transfer data to the index
        if(__keep_index)
          *_msgno_ptr++ = _lastmsgno;

        // Set lastread pointer
        if((_lastmsgno >= _lastread) and (_lastread_reln == 0)) {
          _lastreadfound = _lastmsgno;
          _lastread_reln = _msg_count - (_lastmsgno != _lastread ? 1 : 0);
        }

        // Break loop as soon as we have all we need
        if(_msg_count == _active)
          break;
      }

      // Go to next record
      _msgidx_count++;
      _msgidx_ptr++;
    }

    // If the exact lastread was not found
    if(_msg_count and (_lastreadfound != _lastread)) {

      // Higher than highest or lower than lowest?
      if(_lastread > _lastmsgno)
        _lastread_reln = _msg_count;
      else if(_lastread < _firstmsgno)
        _lastread_reln = 0;
    }

    // Check active message count and log it if different
    if(_active != _msg_count) {
      WideLog->printf("! Counted %u active msgs in %s board %u (%s).", _msg_count, __HUDSON ? HUDS_NAME : GOLD_NAME, board(), echoid());
      WideLog->printf("! According to msginfo%s there should be %u active msgs.", __HUDSON ? HUDS_EXT : GOLD_EXT, _active);
      WideLog->printf(": Info: A program did not update msginfo%s correctly.", __HUDSON ? HUDS_EXT : GOLD_EXT);
      WideLog->printf("+ Advice: Run a msgbase index rebuild utility.");
    }

    // Update area data
    Msgn->SetCount(_msg_count);
    lastread = _lastread_reln;
    lastreadentry = _lastreadfound;

    if(WideDebug) {
      WideLog->printf("- b:%u: t:%u, l:%u, fm:%u, hm:%u, lr:%u, u:%u",
        board(),
        Msgn->Count(),
        lastread,
        _firstmsgno,
        _lastmsgno,
        _lastread,
        wide->userno
      );
    }

    // Close the msgbase again if we opened it in here
    if(not _was_open)
      wide->close();
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
void _HudsArea<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::scan() {

  GFTRK("HudsScan");

  raw_scan(true);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
void _HudsArea<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::scan_area() {

  GFTRK("HudsScanArea");

  raw_scan(false);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

#define HudsIdxCmp(a,b) ((a.board-b.board != 0) ? a.board-b.board : ((int)(a.msgno-b.msgno)))


//  ------------------------------------------------------------------

#define TOIDXBUFSZ 100u
#define PMSCANBUFSZ 50u

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
void _HudsWide<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::realloc_pm_scan() {

  pmscan = (HudsIdx*)throw_realloc(pmscan, (pmscantotal+PMSCANBUFSZ)*sizeof(HudsIdx));
}


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
void _HudsWide<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::scan_pm() {
    
  GFTRK("HudsWideScanPM");

  if(not iswidescanned)
    scan();

  ispmscanned = true;

  HudsToIdx* toidxbuf = (HudsToIdx*)throw_calloc(TOIDXBUFSZ, sizeof(HudsToIdx));
  uint totrecs = (uint)(fhtoi.FileLength() / sizeof(HudsToIdx));
  HudsIdx* idxptr = msgidxptr;
  throw_release(pmscan);
  pmscantotal = 0;
  int invalidboards = 0;
  int gotpm, idxboard, umax, u;
  uint getrecs, rec;
  HudsToIdx* toidx;

  do {

    rec = 0;
    gotpm = false;
    getrecs = MinV(TOIDXBUFSZ, totrecs);
    fhtoi.Read(toidxbuf, getrecs*sizeof(HudsToIdx));

    for(toidx=toidxbuf; rec<getrecs; idxptr++,toidx++,rec++) {

      // Skip deleted msgs
      if(idxptr->msgno == (__HUDSON ? HUDS_DELETEDMSGNO : GOLD_DELETEDMSGNO))
        continue;

      // Skip msgs in invalid boards
      idxboard = idxptr->board;
      if(not (idxboard and (idxboard <= (__HUDSON ? HUDS_MAXBOARD : GOLD_MAXBOARD)))) {
        invalidboards++;
        continue;
      }

      // If after lastread
      if(idxptr->msgno <= scn[idxboard-1].lastreadfound)
        continue;

      // Convert name string
      strnp2c(toidx->name, sizeof(HudsToIdx)-1);

      // Is it a PM?
      umax = (WidePersonalmail & PM_ALLNAMES) ? WideUsernames : 1;
      for(u=0; u<umax; u++) {
        if(strieql(toidx->name, WideUsername[u])) {
          gotpm = true;
          break;
        }
      }

      if(gotpm) {
        if((pmscantotal % PMSCANBUFSZ) == 0)
          realloc_pm_scan();
        pmscan[pmscantotal++] = *idxptr;
        scn[idxboard-1].pmcount++;
        gotpm = false;
      }
    }
    totrecs -= getrecs;
  } while(totrecs);

  throw_free(toidxbuf);
  pmscan = (HudsIdx*)throw_realloc(pmscan, (pmscantotal+1)*sizeof(HudsIdx));

  // Sort all pm records in board/msgno order
  for(uint k=pmscantotal >> 1; k; k >>= 1)
    for(uint i=k; i < pmscantotal; i++)
      for(uint j=i-k; (j >= 0) and HudsIdxCmp(pmscan[j], pmscan[j+k]) > 0; j-=k) {
        HudsIdx e = pmscan[j];
        pmscan[j] = pmscan[j+k];
        pmscan[j+k] = e;
      }

  if(invalidboards) {
    WideLog->printf("! Found %u msgs with an invalid board number (0 or >%u).", invalidboards, (__HUDSON ? HUDS_MAXBOARD : GOLD_MAXBOARD));
    WideLog->printf("! In the %s msgbase at %s.", __HUDSON ? HUDS_NAME : GOLD_NAME, path);
    WideLog->printf(": Info: Your msgbase may be partially corrupted.");
    WideLog->printf("+ Advice: Run a msgbase index rebuild/recover utility.");
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
void _HudsArea<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON>::scan_area_pm() {

  GFTRK("HudsScanAreaPM");

  PMrk->ResetAll();

  if(!wide)
    wide = (HudsWide*) (__HUDSON ? (void *)hudsonwide : (void *)goldbasewide);

  if(wide->iswideopen and not wide->ispmscanned)
    wide->scan_pm();

  // Get wide scan data if any
  if(wide->scn) {

    register board_t _board = (board_t)board();
    register HudsScan* _scan = wide->scn + (_board - 1);

    if(wide->pmscan) {
      if(_scan->pmcount) {

        register HudsIdx* pmscanptr = wide->pmscan;
        while(pmscanptr->board != _board)
          pmscanptr++;

        register uint n = 0;
        register uint cnt = _scan->pmcount;
        while(n < cnt) {
          PMrk->Append(pmscanptr->msgno);
          pmscanptr++;
          n++;
        }
      }
    }

    // Update area data
    Msgn->SetCount(_scan->count);
    lastread = _scan->lastreadreln;
    lastreadentry = _scan->lastreadfound;

    if(WideDebug) {
      WideLog->printf("- b:%u: t:%u, l:%u, fm:%u, hm:%u, lr:%u, u:%u, pm:%i",
        _board,
        _scan->count,
        _scan->lastreadreln,
        _scan->firstmsgno,
        _scan->lastmsgno,
        _scan->lastread,
        wide->userno,
        PMrk->Count()
      );
    }

  }
  else {
    if(WideDebug)
      WideLog->printf("- Oops!  Fell into empty bracket.");
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

