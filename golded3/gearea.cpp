
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
//  Arealister and other area-specific code
//  ------------------------------------------------------------------

#include <golded.h>
#include <gutlos.h>


//  ------------------------------------------------------------------

extern int arealistnumgrps;

bool in_arealist = false;

GPickArealist* PickArealist;

Echo area_maybe;
int areaswithgroupid = 0;
uint* areanumbers = NULL;

//            111111111122222222223333333333444444444455555555556666666666777777777
//  0123456789012345678901234567890123456789012345678901234567890123456789012345678
// 旼Area횯escription컴컴컴컴컴컴컴컴컴컴컴컴횺sgs컴횼ew컴EchoID컴컴컴컴컴컴컴Grp커
// �   1> NET FidoNet Z3/4/5/6                 118+    3* NET.FIDOZX            A �

// AREALISTFORMAT "AM D CPUN E G"

int area_pos      = -1;
int area_width    = 4;
int marked_pos    = -1;
int marked_width  = 1;
const char marked_char   = MMRK_MARK;
int desc_pos      = -1;
int desc_width    = -1;
int count_pos     = -1;
int count_width   = 6;
int pmark_pos     = -1;
int pmark_width   = 1;
const char pmark_char    = '+';
int unread_pos    = -1;
int unread_width  = 6;
int changed_pos   = -1;
int changed_width = 1;
const char changed_char  = '*';
int echoid_pos    = -1;
int echoid_width  = 0;
int groupid_pos   = -1;
int groupid_width = 0;


//  ------------------------------------------------------------------

GPickArealist::GPickArealist() {

  areawin1 = 0;
  areawin2 = 0;
  tempwin1 = 0;
  tempwin2 = 0;

  pmails   = 0;
  pmareas  = 0;
  pmscan   = false;

  area_fuzidx = 0;

}


//  ------------------------------------------------------------------

void GPickArealist::AreasRenumber() {

  for(int n=0,a=1,t=AL.size(); n<t; n++)
    areanumbers[n] = AL[n]->isseparator() ? 0 : a++;
}


//  ------------------------------------------------------------------

bool GPickArealist::is_selectable(uint idx) {

  Area* area = AL.AreaNoToPtr(idx);
  return (not area->isseparator());

}


//  ------------------------------------------------------------------

void GPickArealist::do_delayed() {

  Area* area = AL.AreaNoToPtr(index);

  const size_t buflen=strlen(title)+strlen(area_maybe);
  const size_t tmplen=(buflen>MAXCOL?buflen:MAXCOL);
  char * buf = new char[buflen+3];
  buf[buflen] = '\0';
  buf[buflen+1] = '\xFF';
  buf[buflen+2] = '\0';
  char * tmp = new char[tmplen+3];
  tmp[tmplen] = '\0';
  tmp[tmplen+1] = '\xFF';
  tmp[tmplen+2] = '\0';

  update_statuslinef("%s: %u %s, %u %s, %u %s", "", area->echoid(), area->Msgn.Count(),
                     (area->Msgn.Count() == 1 ? LNG->msg : LNG->msgs), area->unread,
                     LNG->unread, area->PMrk.Count(), LNG->personal);

  strcpy(stpcpy(buf, title), area_maybe);
  strsetsz(strcpy(tmp, buf), MAXCOL);
  wwprintstr(tempwin, 0, 0, wattr, tmp);

  if(CFG->switches.get(arealistpagebar))
    wscrollbar(W_VERT, maximum_index+1, maximum_index, index);

  if (buf[buflen] || buf[buflen+1]!='\xFF' || buf[buflen+2])
  {
    LOG.errpointer(__FILE__,__LINE__);
    LOG.printf("! Buffer overflow: buf in GPickArealist::do_delayed(), 8 or 9 lines above");
    PointerErrorExit();
  }
  if (tmp[tmplen] || tmp[tmplen+1]!='\xFF' || tmp[tmplen+2])
  {
    LOG.errpointer(__FILE__,__LINE__);
    LOG.printf("! Buffer overflow: tmp in GPickArealist::do_delayed(), 15 or 16 lines above");
    PointerErrorExit();
  }
  delete buf;
  delete tmp;
}


//  ------------------------------------------------------------------

void GPickArealist::close_all() {

  if(areawin1)
    wunlink(areawin1);
  if(areawin2)
    wunlink(areawin2);
  if(tempwin1)
    wunlink(tempwin1);
  if(tempwin2)
    wunlink(tempwin2);
}


//  ------------------------------------------------------------------

void GPickArealist::dispbuf(char* buf, int areano) {

  Area* area = AL.AreaNoToPtr(areano);

  memset(buf, ' ', MAXCOL-2);
  buf[MAXCOL-2] = NUL;

  char areabuf[33];
  gsprintf(PRINTF_DECLARE_BUFFER(areabuf), "%u", CFG->switches.get(arealistnos) ? area->board() : areanumbers[areano]);
  int areawidth = strlen(areabuf);

  char markedbuf[2] = { " " };
  *markedbuf = area->ismarked() ? marked_char : ' ';
  int markedwidth = 1;

  char descbuf[100];
  int descwidth = strlen(strcpy(descbuf, area->desc()));

  char countbuf[33];
  if (area->isscanned)
    gsprintf(PRINTF_DECLARE_BUFFER(countbuf), "%u", (uint)area->Msgn.Count());
  else
    strcpy(countbuf,  "-");
  int countwidth = strlen(countbuf);

  char pmarkbuf[2] = { " " };
  *pmarkbuf = area->PMrk.Count() ? pmark_char : ' ';
  int pmarkwidth = 1;

  char unreadbuf[33];
  if (area->isscanned)
    gsprintf(PRINTF_DECLARE_BUFFER(unreadbuf), "%u", (uint)((CFG->arealisttype == AL_TOTNEW) ? area->unread : area->lastread()));
  else
    strcpy(unreadbuf, "-");
  int unreadwidth = strlen(unreadbuf);

  char changedbuf[2] = { " " };
  *changedbuf = area->isunreadchg ? changed_char : ' ';
  int changedwidth = 1;

  char echoidbuf[100];
  int echoidwidth = strlen(strcpy(echoidbuf, area->echoid()));

  char groupidbuf[10] = { "" };
  if (groupid_width)
  {
    if (area->groupid() & 0x8000u)
    {
      if (groupid_width > 2)
        gsprintf(PRINTF_DECLARE_BUFFER(groupidbuf), "%u", area->groupid()&0x7FFF);
    }
    else if (g_isupper(area->groupid()))
      gsprintf(PRINTF_DECLARE_BUFFER(groupidbuf), "%c", (char)area->groupid());
  }
  int groupidwidth = strlen(groupidbuf);

  areawidth    = MinV(areawidth,    area_width);
  markedwidth  = MinV(markedwidth,  marked_width);
  descwidth    = MinV(descwidth,    desc_width);
  countwidth   = MinV(countwidth,   count_width);
  pmarkwidth   = MinV(pmarkwidth,   pmark_width);
  unreadwidth  = MinV(unreadwidth,  unread_width);
  changedwidth = MinV(changedwidth, changed_width);
  echoidwidth  = MinV(echoidwidth,  echoid_width);
  groupidwidth = MinV(groupidwidth, groupid_width);

  memcpy(buf+area_pos+area_width-areawidth,          areabuf,    areawidth);
  memcpy(buf+marked_pos,                             markedbuf,  markedwidth);
  memcpy(buf+desc_pos,                               descbuf,    descwidth);
  memcpy(buf+count_pos+count_width-countwidth,       countbuf,   countwidth);
  memcpy(buf+pmark_pos,                              pmarkbuf,   pmarkwidth);
  memcpy(buf+unread_pos+unread_width-unreadwidth,    unreadbuf,  unreadwidth);
  memcpy(buf+changed_pos,                            changedbuf, changedwidth);
  memcpy(buf+echoid_pos,                             echoidbuf,  echoidwidth);
  memcpy(buf+groupid_pos+groupid_width-groupidwidth, groupidbuf, groupidwidth);
}

//  ------------------------------------------------------------------

void GPickArealist::center() {

  uint room, toproom, botroom;

  toproom = index;
  botroom = maximum_index - index;
  if(toproom > maximum_position/2) {
    if(botroom > maximum_position/2)
      room = maximum_position/2;
    else if(botroom)
      room = maximum_position - botroom;
    else
      room = maximum_position;
  }
  else
    room = toproom;

  position = room;
  display_page();
}


//  ------------------------------------------------------------------

void GPickArealist::jump_to() {

  uint n = index+1;
  bool found = false;
  // Search for next marked area, wrapping 'round if needed
  for(; n<=maximum_index; n++) {
    if(not AL[n]->isseparator() and AL[n]->ismarked()) {
      found = true;
      break;
    }
  }

  if(not found) {
    for(n = minimum_index; n <= index; n++) {
      if(not AL[n]->isseparator() and AL[n]->ismarked())
        break;
    }
  }

  if(n > maximum_index)
    n = minimum_index;

  while(AL[n]->isseparator()) {
    if((++n) > maximum_index)
      n = minimum_index;
  }

  index = n;
  center();
}


//  ------------------------------------------------------------------

void GPickArealist::jumpmatch() {

  uint n = index+1;
  bool found = false;
  // Search for next marked area, wrapping 'round if needed
  for(; n<=maximum_index; n++) {
    if(not AL[n]->isseparator() and (strnieql(area_maybe, AL[n]->echoid(), area_fuzidx) or striinc(area_maybe, AL[n]->echoid()))) {
      found = true;
      break;
    }
  }

  if(not found) {
    for(n = minimum_index; n <= index; n++) {
      if(not AL[n]->isseparator() and (strnieql(area_maybe, AL[n]->echoid(), area_fuzidx) or striinc(area_maybe, AL[n]->echoid())))
        break;
    }
  }

  if(n > maximum_index)
    n = minimum_index;

  while(AL[n]->isseparator()) {
    if((++n) > maximum_index)
      n = minimum_index;
  }

  index = n;
  center();

}


//  ------------------------------------------------------------------

void GPickArealist::open() {

  int active=NO;

  #if defined(GUTLOS_FUNCS)
  g_set_ostitle_name("Arealist",0);
  #endif

  if(ypos) {
    if(tempwin1) {
      tempwin = tempwin1;
      areawin = areawin1;
      active = YES;
    }
  }
  else {
    if(tempwin2) {
      tempwin = tempwin2;
      areawin = areawin2;
      active = YES;
    }
  }

  if(active) {
    wunhide(tempwin);
    wprints(0,0, wattr, title);
    wunhide(areawin);
  }
  else {
    tempwin = wopen_(ypos, xpos, 1, MAXCOL, 5, battr, wattr, sbattr);
    wprints(0,0, wattr, title);
    areawin = wopen_(ypos+1, xpos, ylen+2, MAXCOL, btype, battr, wattr, sbattr);

    if(ypos) {
      tempwin1 = tempwin;
      areawin1 = areawin;
    }
    else {
      tempwin2 = tempwin;
      areawin2 = areawin;
    }

    if(area_width)
      wmessage(LNG->Area, TP_BORD, 1+area_pos+(marked_pos==area_pos+area_width?1:0), tattr);
    if(desc_width)
      wmessage(LNG->Description, TP_BORD, 1+desc_pos, tattr);
    if(count_width)
      wmessage(LNG->Msgs, TP_BORD, 1+count_pos+count_width-strlen(LNG->Msgs), tattr);
    if(unread_width) {
      if(CFG->arealisttype == AL_TOTLST)
        wmessage(LNG->Last, TP_BORD, 1+unread_pos+unread_width-strlen(LNG->Last), tattr);
      else if(CFG->arealisttype == AL_TOTNEW)
        wmessage(LNG->New, TP_BORD, 1+unread_pos+unread_width-strlen(LNG->New), tattr);
    }
    if(echoid_width)
      wmessage(LNG->EchoID, TP_BORD, 1+echoid_pos, tattr);
    if(groupid_width) {
      char grpbuf[40];
      if(groupid_width > 2)
        strcpy(grpbuf, LNG->Grp);
      else {
        *grpbuf = *(LNG->Grp);
        grpbuf[1] = NUL;
      }
      wmessage(grpbuf, TP_BORD, 1+groupid_pos, tattr);
    }

  }
  if(CFG->switches.get(areaautonext) and ypos == 0 and index >= minimum_index and not AL[index]->ismarked()) {
    jump_to();
  }
  else
    center();
}


//  ------------------------------------------------------------------

void GPickArealist::close() {

  whide();
  whide();
}

//  ------------------------------------------------------------------

void GPickArealist::precursor() {

  *area_maybe = 0;
  area_fuzidx = 0;
}


//  ------------------------------------------------------------------

void GPickArealist::print_line(uint idx, uint pos, bool isbar) {

  vchar vbuf[256];
  char buf[256];

  if(AL[idx]->isseparator()) {
    Area* area = AL.AreaNoToPtr(idx);

    int sep_pos = (desc_pos != -1) ? desc_pos : echoid_pos;

    {for(int c = 0; c < sep_pos; c++)
      vbuf[c] = _box_table(btype, 1);}
    vbuf[sep_pos] = NUL;
    wprintvs(pos, 0, battr|ACSET, vbuf);
    wprints(pos, sep_pos, tattr, area->desc());

    int l = strlen(area->desc());
    int n = MAXCOL-2-sep_pos-l;

    {for(int c = 0; c < n; c++)
      vbuf[c] = _box_table(btype,1);}
    vbuf[n] = NUL;
    wprintvs(pos, sep_pos+l, battr|ACSET, vbuf);
  }
  else {
    dispbuf(buf, idx);
    wprints(pos, 0, isbar ? sattr : wattr, buf);
    if(AL[idx]->ismarked())
      wprintc(pos, marked_pos, isbar ? sattr : hattr, marked_char);
  }

}


//  ------------------------------------------------------------------

void GPickArealist::AreaCatchUp(uint n) {

  // Do not do catch up if there's active area
  if(AA->isopen())
    return;

  GMenuAreaCatchup MenuAreaCatchup;
  GMsg* msg = (GMsg*)throw_calloc(1, sizeof(GMsg));

  int mode = MenuAreaCatchup.Run();

  if(mode != SCAN_QUIT) {
    for(AL.item = AL.idx.begin(); AL.item != AL.idx.end(); AL.item++) {

      if((mode == SCAN_MARKED and (*AL.item)->ismarked()) or mode == SCAN_ALL or (mode == SCAN_CURRENT and (*AL.item) == AL[n])) {

        if((*AL.item)->isseparator())
          continue;

        update_statuslinef("%s ...", "", (*AL.item)->echoid());

        AA = (*AL.item);
        AA->Open();

        if(CFG->switches.get(highlightunread) and CFG->switches.get(areacatchupread)) {
          w_info(LNG->Wait);
          for(uint i=AA->lastread()+1; i <= AA->Msgn.Count(); i++) {
            AA->LoadHdr(msg, AA->Msgn.CvtReln(i), false);
            if(msg->timesread++ == 0)
              AA->UpdateTimesread(msg);
          }
          w_info(NULL);
        }

        AA->set_lastread(AA->Msgn.Count());
        AA->isvalidchg = false;
        AA->Close();
      }
    }
  }
  throw_free(msg);
}


//  ------------------------------------------------------------------

void GPickArealist::AreaDropMsgMarks(uint n) {

  GMenuAreaDropMarks MenuAreaDropMarks;

  uint nummarks = 0;

  for(AL.item = AL.idx.begin(); AL.item != AL.idx.end(); AL.item++)
    nummarks += (*AL.item)->Mark.Count();

  char buf[256];
  gsprintf(PRINTF_DECLARE_BUFFER(buf), LNG->DropMarksInfo, longdotstr(nummarks));

  w_info(buf);
  int mode = MenuAreaDropMarks.Run();
  w_info(NULL);

  if(mode != SCAN_QUIT) {
    for(AL.item = AL.idx.begin(); AL.item != AL.idx.end(); AL.item++) {
      if((mode == SCAN_MARKED and (*AL.item)->ismarked()) or mode == SCAN_ALL or (mode == SCAN_CURRENT and (*AL.item) == AL[n])) {

        if((*AL.item)->isseparator())
          continue;

        (*AL.item)->Mark.ResetAll();
      }
    }
  }
}


//  ------------------------------------------------------------------

bool GPickArealist::handle_key() {

  uint n;
  uint x;
  const char* adesc;
  char buf[256], tmp[256];

  int mode, changed, currno;

  gkey kk;

  if(key < KK_Commands) {
    key = key_tolower(key);
    if((key == Key_Esc) and esc_abort)
      key = KK_AreaAbort;
    else {
      kk = SearchKey(key, AreaKey, AreaKeys);
      if(kk)
        key = kk;
    }
  }


  switch(key) {

    case KK_AreaDropMsgMarks:
      AreaDropMsgMarks(index);
      break;

    case KK_AreaSelectMarks:
      AL.Select_Mask();
      break;

    case KK_AreaAskExit:
      {
        GMenuQuit MenuQuit;
        aborted = gkbd.quitall = (MenuQuit.Run()!=0);
        if(gkbd.quitall) {
          precursor();
          return false;
        }
      }
      break;

    case KK_AreaAbort:
      aborted = true;
      // Drop Through

    case KK_AreaSelect:
      if(AL[index]->isseparator()) {
        if(not PlayMacro(key, KT_A))
          SayBibi();
        break;
      }
      precursor();
      return false;

    case KK_AreaQuitNow:
      aborted = gkbd.quitall = true;
      precursor();
      return false;

    case KK_AreaToggle:
      AL[index]->set_marked(not AL[index]->ismarked());
      display_bar();
      precursor();
      cursor_down();
      break;

    case KK_AreaMark:
      AL[index]->set_marked(true);
      display_bar();
      precursor();
      cursor_down();
      break;

    case KK_AreaUnmark:
      AL[index]->set_marked(false);
      display_bar();
      precursor();
      cursor_down();
      break;

    case KK_AreaBoardnos:
      CFG->switches.set(arealistnos, not CFG->switches.get(arealistnos));
      update();
      break;

    case KK_AreaCatchUp:
      AreaCatchUp(index);
      update();
      break;

    case KK_AreaJumpNextMatch:
      jumpmatch();
      break;

    case KK_AreaJump:
      {
        precursor();
        jump_to();
      }
      break;

    case KK_AreaDosShell:
      DosShell();
      break;

    case KK_AreaGotoPrev:   precursor(); cursor_up();    break;
    case KK_AreaGotoNext:   precursor(); cursor_down();  break;
    case KK_AreaGotoFirst:  precursor(); cursor_first(); break;
    case KK_AreaGotoLast:   precursor(); cursor_last();  break;

    case KK_AreaSoundkill:
      HandleGEvent(EVTT_STOPVOICE);
      break;

    case KK_AreaTouchNetscan:
      TouchNetscan();
      break;

    case KK_AreaHeat:
      {
        GMenuAreaHeat MenuAreaHeat;
        mode = MenuAreaHeat.Run();
        if(mode != SCAN_QUIT) {
          for(AL.item = AL.idx.begin(); AL.item != AL.idx.end(); AL.item++) {
            if((mode == SCAN_MARKED and (*AL.item)->ismarked()) or mode == SCAN_ALL or (mode == SCAN_CURRENT and (*AL.item) == AL[index-1])) {
              update_statuslinef("%s %s", "", 1+LNG->ScanningArea, (*AL.item)->echoid());
              (*AL.item)->SetHighwaterMark();
            }
          }
        }
      }
      break;

    case KK_AreaZap:
      {
        GMenuAreaZap MenuAreaZap;
        mode = MenuAreaZap.Run();
        if(mode != SCAN_QUIT) {
          for(AL.item = AL.idx.begin(); AL.item != AL.idx.end(); AL.item++) {
            if((mode == SCAN_MARKED and (*AL.item)->ismarked()) or mode == SCAN_ALL or (mode == SCAN_CURRENT and (*AL.item) == AL[index-1])) {
              update_statuslinef("%s %s", "", 1+LNG->ScanningArea, (*AL.item)->echoid());
              (*AL.item)->ResetHighwaterMark();
            }
          }
        }
      }
      break;

    case KK_AreaScanPM:
      pmscan = true;
      pmails = 0;
      pmareas = 0;
      // DROP THROUGH //

    case KK_AreaScan:
      {
        currno = index;
        GMenuAreaScan MenuAreaScan;
        mode = MenuAreaScan.Run(pmscan);
        if(mode != SCAN_QUIT) {

          w_info(LNG->Wait);

          strcpy(stecho, AL[index]->echoid());

          if(mode == SCAN_IMPORTQWK)
            changed = ImportQWK();
          else if(mode == SCAN_EXPORTQWK)
            changed = ExportQWK();
          else if(mode == SCAN_IMPORTSOUP)
            changed = ImportSOUP();
          else if(mode == SCAN_EXPORTSOUP)
            changed = ExportSOUP();
          else {
            if(cmdlinedebughg)
              LOG.printf("- AreaScan from arealist");
            changed = AL.AreaScan(mode, currno, pmscan, pmails, pmareas);
          }

          w_info();

          AL.SetActiveAreaNo(currno);
          if(changed) {
            AL.Sort();
            AreasRenumber();
            index = AL.AreaEchoToNo(stecho);
            center();
          }

          if(pmscan) {
            if(pmails) {
              w_infof(LNG->FoundPersonal,
                pmails, (pmails==1?"":"s"),
                pmareas, (pmareas==1?"":"s")
              );
            }
            else {
              w_info(LNG->NoPersonal);
            }
            waitkeyt(10000);
            w_info();
          }
        }
        pmscan = false;
      }
      break;

    case Key_Tick:
      CheckTick(KK_AreaQuitNow);
      break;

    case KK_AreaUndefine:
      break;

    case KK_AreaWriteGoldlast:
      w_info(LNG->Wait);
      AL.WriteGoldLast();
      w_info();
      break;

    default:
      if(key < KK_Macro) {
        n = g_toupper(key & 0xFF);
        if((area_fuzidx < area_maxfuz) or (key == Key_BS)) {

          // Incremental search in the echoids
          if(not iscntrl(n) or (key == Key_BS)) {
            if(n == ' ')
              n = '_';
            if(key != Key_BS)
              area_maybe[area_fuzidx++] = (char)n;
            else if(area_fuzidx)
              area_fuzidx--;
            area_maybe[area_fuzidx] = NUL;
            strcpy(stpcpy(buf, title), area_maybe);
            strsetsz(strcpy(tmp, buf), MAXCOL);
            wwprintstr(tempwin, 0, 0, wattr, tmp);
            if(area_fuzidx) {

              x = atoi(area_maybe);
              if(x) {
                if(CFG->switches.get(arealistnos) and (isdigit(area_maybe[1]) or (area_maybe[1] == NUL))) {
                  for(n=0; n<AL.size(); n++) {
                    if(AL[n]->board() == x) {
                      x = n;
                      goto RedrawAreas;
                    }
                  }
                  x = 0;  // No match found
                }
                else {
                  for(n=0; n<area_fuzidx; n++) {
                    if(not isdigit(area_maybe[n])) {
                      x = 0;
                      break;
                    }
                  }
                }
              }

              if(x >= 1 and x<=AL.size()) {
                {
                  for(int a=0, at=AL.size(); a<at; a++) {
                    if(areanumbers[a] == x) {
                      x = a;
                      goto RedrawAreas;
                    }
                  }
                }
                x = (uint)-1;

              RedrawAreas:

                if(x == (uint)-1)
                  x = index;
                while(AL[x]->isseparator()) {
                  x++;
                  if(x > maximum_index)
                    x = minimum_index;
                }
                if(x != index)
                  display_line();
                #define _topidx (index-position)                    // Shorthand..
                #define _botidx (index+(maximum_position-position)) // Shorthand..
                if((_topidx <= x) and (x <= _botidx)) {
                  position += (x-index);
                  index = x;
                  display_page();    /////////// NEW
                }
                else {
                  index = x;
                  center();
                }
                break;
              }
              else {
                // Regular search
                for(n=0; n<AL.size(); n++) {
                  adesc = AL[n]->echoid();
                  if(strnicmp(area_maybe, adesc, area_fuzidx) == 0) {
                    AL.Sort();
                    AreasRenumber();
                    x = AL.AreaEchoToNo(adesc);
                    goto RedrawAreas;
                  }
                }
                // Search inside
                for(n=0; n<AL.size(); n++) {
                  adesc = AL[n]->echoid();
                  if(striinc(area_maybe, adesc)) {
                    AL.Sort();
                    AreasRenumber();
                    x = AL.AreaEchoToNo(adesc);
                    goto RedrawAreas;
                  }
                }
              }
            }
            else {
              adesc = AL[index]->echoid();
              AL.Sort();
              AreasRenumber();
              index = AL.AreaEchoToNo(adesc);
              center();
            }
            break;
          }
        }
      }
      if(not PlayMacro(key, KT_A))
        SayBibi();
  }

  return true;
}

//  ------------------------------------------------------------------

int GPickArealist::Run(const char* _title, int wpos, int& idx) {

  HandleGEvent(EVTT_AREALIST);

  xpos     = 0;                             // Window starting coloumn
  ypos     = wpos;                          // Window starting row
  xlen     = MAXCOL-2;                      // Window ending coloumn
  ylen     = MAXROW-wpos-4;                 // Window ending row
  title    = _title;                        // Window title
  btype    = W_BAREA;                       // Window border type
  battr    = C_AREAB;                       // Window border attributes
  wattr    = C_AREAW;                       // Window Color
  tattr    = C_AREAT;                       // Window Title Color
  sattr    = C_AREAS;                       // Window Selection Bar Color
  hattr    = C_AREAQ;                       // Window Highlight Color
  sbattr   = C_AREAPB;                      // Window Scrollbar Color
  helpcat  = H_Area;                        // Window Help Category

  maximum_index    = AL.size() - 1;             // List Entries - 1
  maximum_position = MinV((size_t)ylen-1, AL.size()-1); // Display Pos
  index            = AL.AreaIdToNo(idx);        // List Index
  listwrap         = CFG->switches.get(displistwrap);  // True if wrap-around is supported
  esc_abort        = (wpos!=0);
  area_maxfuz      = MinV(sizeof(Echo), MAXCOL-strlen(title)-1);

  goldmark = ' ';

  areanumbers = (uint*)throw_calloc(AL.size(), sizeof(uint));
  AreasRenumber();

  run_picker();

  throw_release(areanumbers);

  if(not aborted)
    return AL[index]->areaid();

  idx = AL[index]->areaid();

  return -1;
}


//  ------------------------------------------------------------------

int AreaPick(char* title, int wpos, int* idx) {

  GPickArealist p;
  int new_area;

  if(gkbd.quitall)
    return -1;

  PickArealist = &p;

  in_arealist = true;
  new_area = p.Run(title, wpos, *idx);
  in_arealist = false;

  PickArealist = NULL;

  return new_area;
}


//  ------------------------------------------------------------------

