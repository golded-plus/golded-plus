
//  ------------------------------------------------------------------
//  GoldED+
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999-2000 Alexander S. Aganichev
//  ------------------------------------------------------------------
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License as
//  published by the Free Software Foundation; either version 2 of the
//  License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston,
//  MA 02111-1307 USA
//  ------------------------------------------------------------------
//  $Id$
//  ------------------------------------------------------------------
//  Area scanning functions.
//  ------------------------------------------------------------------

#include <golded.h>
#include <gmoprot.h>


//  ------------------------------------------------------------------

extern bool in_arealist;
extern GPickArealist* PickArealist;

//  ------------------------------------------------------------------

void Area::UpdateAreadata() {

  if(isscanned) {

    uint _unread = Msgn.Count() - lastread();
    if(not isvalidchg) {
      unread = _unread;
      isvalidchg = true;
    }
    isunreadchg = (bool)(unread != _unread);
    unread = _unread;
    word oldmask = AL.mask;
    AL.mask = 0;
    set_marked(_unread);
    AL.mask = oldmask;
  }
}


//  ------------------------------------------------------------------

void Area::ScanArea() {

  if(cmdlinedebughg)
    LOG.printf("- ScanArea: %s", echoid());

  area->Msgn = &Msgn;
  area->PMrk = &PMrk;

  area->scan_area();

  isscanned = true;

  UpdateAreadata();
}


//  ------------------------------------------------------------------

void Area::ScanAreaPM() {

  if(cmdlinedebughg)
    LOG.printf("- ScanAreaPM: %s", echoid());

  area->Msgn = &Msgn;
  area->PMrk = &PMrk;

  area->scan_area_pm();

  isscanned = true;
  ispmscanned = true;

  UpdateAreadata();
}


//  ------------------------------------------------------------------

int AreaList::SetActiveAreaNo(int __areano) {

  AA = idx[__areano];
  CurrArea = AA->areaid();
  CFG->originno = AA->originno();

  return CurrArea;  // Return the areaid
}


//  ------------------------------------------------------------------

int AreaList::AreaScan(int mode, uint currno, int pmscan, int& pmails, int& pmareas, const char* file) {

  gstrarray bag;
  int groupid = -1;

  // Load scan list into a string bag
  if(mode == SCAN_LIST) {
    gfile fp;
    Path tmp;
    strcpy(tmp, file ? file : ListScan.File());
    char* listfile;
    char* option=tmp;
    getkeyval(&listfile, &option);
    fp.fopen(AddPath(CFG->goldpath, listfile), "rt");
    if(fp.isopen()) {
      char buf[512];
      while(fp.fgets(buf, sizeof(buf))) {
        strbtrim(buf);
        char* val = strtok(buf, ", \t");
        while(val) {
          bag.push_back(val);
          val = strtok(NULL, ", \t");
        }
      }
      fp.fclose();
      if(((*option == '-') or (*option == '/')) and strieql(option+1, "delete"))
        remove(listfile);
    }
  }
  else if(mode == SCAN_GROUP) {
    groupid = idx[currno]->groupid();
  }

  int currid = AreaNoToId(currno);

  int scanned = false;

  // For optimized overlay usage, sort arealist by msgbase type, path, board and echoid
  if(*CFG->areascansort) {
    Sort(CFG->areascansort);
    scanned = true;
  }

  currno = AreaIdToNo(currid);

  #ifndef GMB_NOPCB
  if(AL.msgbases & MT_PCBOARD)    PcbWideOpen();
  #endif
  #ifndef GMB_NOGOLD
  if(AL.msgbases & MT_GOLDBASE)   GoldWideOpen();
  #endif
  #ifndef GMB_NOHUDS
  if(AL.msgbases & MT_HUDSON)     HudsWideOpen();
  #endif
  #ifndef GMB_NOXBBS
  if(AL.msgbases & MT_ADEPTXBBS)  XbbsWideOpen();
  #endif

  for(uint n=0; n<idx.size(); n++) {

    // Check if ESC was pressed to skip the scan
    gkey xch = kbxhit();
    if(xch) {
      xch = kbxget();
      if(xch == Key_Esc)
        break;
      else
        kbput(xch);
    }

    SetActiveAreaNo(n);

    if(not AA->isseparator()) {

      int scanit     = false;

      int dopmscan     = pmscan and AA->pmscan();
      int dopmscanexcl = pmscan and AA->pmscanexcl();
      int dopmscanincl = pmscan and AA->pmscanincl();

      int doscan     = AA->scan()     or dopmscan;
      //if Area is excluded from pm-scanning, scan it instead
      int doscanexcl = AA->scanexcl();
      int doscanincl = AA->scanincl() or dopmscanincl or (dopmscanexcl and doscan);

      if(mode != SCAN_STARTUP and pmscan)
        dopmscan = true;

      switch(mode) {
        case SCAN_STARTUP:
          if(doscan and (not doscanexcl or doscanincl))
            scanit = true;
          break;
        case SCAN_ALL:
          if(not doscanexcl or doscanincl)
            scanit = true;
          break;
        case SCAN_CURRENT:
          scanit = n == currno;
          break;
        case SCAN_MARKED:
          if(AA->ismarked())
            scanit = true;
          break;
        case SCAN_MATCHING:
          if(striinc(area_maybe, AA->echoid()))
            scanit = true;
          break;
        case SCAN_UNSCANNED:
          scanit = not (pmscan ? AA->ispmscanned : AA->isscanned);
          break;
        case SCAN_GROUP:
          scanit = AA->groupid() == groupid;
          break;
        case SCAN_NETMAIL:
          scanit = AA->isnet();
          break;
        case SCAN_LIST:
          {
            gstrarray::iterator i;
            for(i = bag.begin(); i != bag.end(); i++)
              if(strwild(AA->echoid(), i->c_str())) {
                scanit = true;
                break;
              }
          }
          break;
      }

      if(scanit) {
        if(not AA->isopen()) {
          AA->Msgn.Reset();
          AA->Mark.ResetAll();
          AA->PMrk.ResetAll();
        }
        scanned = YES;
        if(not blanked)
          update_statuslinef("%s %s", 1+LNG->ScanningArea, AA->echoid());
        if(dopmscan and (not dopmscanexcl or dopmscanincl)) {
          AA->ScanAreaPM();
          uint count = AA->PMrk.Count();
          if(count) {
            pmails += count;
            pmareas++;
          }
          if(CFG->personalmail & PM_LISTONLY)
            AA->PMrk.Reset();
        }
        else {
          AA->ScanArea();
        }
      }
    }
  }

  #ifndef GMB_NOXBBS
  if(AL.msgbases & MT_ADEPTXBBS)  XbbsWideClose();
  #endif
  #ifndef GMB_NOHUDS
  if(AL.msgbases & MT_HUDSON)     HudsWideClose();
  #endif
  #ifndef GMB_NOGOLD
  if(AL.msgbases & MT_GOLDBASE)   GoldWideClose();
  #endif
  #ifndef GMB_NOPCB
  if(AL.msgbases & MT_PCBOARD)    PcbWideClose();
  #endif

  return scanned;
}


//  ------------------------------------------------------------------

void CheckSemaphores() {

  Path file;
  int scanned = 0;
  int pmareas = 0;
  int pmails = 0;

  if(fexist(CFG->semaphore.qwkimport)) {
    ImportQWK();
    remove(CFG->semaphore.qwkimport);
    scanned++;
  }

  if(fexist(CFG->semaphore.qwkexport)) {
    ExportQWK();
    remove(CFG->semaphore.qwkexport);
  }

  if(fexist(CFG->semaphore.soupimport)) {
    ImportSOUP();
    remove(CFG->semaphore.soupimport);
    scanned++;
  }

  if(fexist(CFG->semaphore.soupexport)) {
    ExportSOUP();
    remove(CFG->semaphore.soupexport);
  }

  if(fexist(CFG->semaphore.exitnow) and in_arealist) {
    gkbd.quitall = YES;
    kbput(KK_AreaAbort);
    remove(CFG->semaphore.exitnow);
  }
  else {

    if(fexist(CFG->semaphore.scanall)) {
      scanned += AL.AreaScan(SCAN_ALL, 0, false, pmails, pmareas);
      remove(CFG->semaphore.scanall);
    }

    if(fexist(CFG->semaphore.scanthis)) {
      sprintf(file, "%s -delete", CFG->semaphore.scanthis);
      scanned += AL.AreaScan(SCAN_LIST, 0, false, pmails, pmareas, file);
    }

    if(fexist(CFG->semaphore.scannetmail)) {
      scanned += AL.AreaScan(SCAN_NETMAIL, 0, false, pmails, pmareas);
      remove(CFG->semaphore.scannetmail);
    }

    if(fexist(CFG->semaphore.pmscanall)) {
      scanned += AL.AreaScan(SCAN_ALL, 0, true, pmails, pmareas);
      remove(CFG->semaphore.pmscanall);
    }

    if(fexist(CFG->semaphore.pmscanthis)) {
      sprintf(file, "%s -delete", CFG->semaphore.pmscanthis);
      scanned += AL.AreaScan(SCAN_LIST, 0, true, pmails, pmareas, file);
    }

    if(fexist(CFG->semaphore.pmscannetmail)) {
      scanned += AL.AreaScan(SCAN_NETMAIL, 0, true, pmails, pmareas);
      remove(CFG->semaphore.pmscannetmail);
    }
  }

  if(scanned) {
    AL.Sort();
    if(in_arealist and not blanked) {
      PickArealist->update();        // update arealist-display
      PickArealist->do_delayed();    // update statusline
    }
    AL.WriteGoldLast();
  }
}


//  ------------------------------------------------------------------

