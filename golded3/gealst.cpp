
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
//  Arealist functions.
//  ------------------------------------------------------------------

#include <algorithm>
#include <golded.h>


//  ------------------------------------------------------------------

AreaList AL;

int AreaTypeOrder[17] = {
  0,
  1,      // AT_NET
  2,      // AT_EMAIL
  3,      // AT_NET | AT_EMAIL
  4,      // AT_ECHO
  5,
  6,
  7,
  8,      // AT_NEWSGROUP
  9,
  10,
  11,
  12,     // AT_ECHO | AT_NEWSGROUP
  13,
  14,
  15,
  16      // AT_LOCAL
};


//  ------------------------------------------------------------------
//  Areagroups compare

int compare_groups(int ga, int gb)
{
  char *gap, *gbp;

  if((ga > 0xff) || (gb > 0xff))
    return compare_two(ga, gb);
  gap = strchr(CFG->arealistgrouporder, (char)ga);
  gbp = strchr(CFG->arealistgrouporder, (char)gb);
  if(gap == NULL) {
    if(gbp != NULL)
      return 1;
    else
      return compare_two(ga, gb);
  }
  else {
    if(gbp == NULL)
      return -1;
    else
      return compare_two(gap, gbp);
  }
}


//  ------------------------------------------------------------------
//  Arealist compare

extern "C" int AreaListCmp(const Area** __a, const Area** __b) {

  const Area* a = *__a;
  const Area* b = *__b;
  const Area* A = a;
  const Area* B = b;
  int cmp = 0;
  int aunread;
  int bunread;

  bool rev = false;
  char* ptr = AL.sortspec;

  while(*ptr) {
    switch(toupper(*ptr)) {
      case '-':
        rev = true;
        A = b;  B = a;
        break;
      case '+':
        rev = false;
        A = a;  B = b;
        break;
      case 'A':
        if((cmp = A->aka().compare(B->aka())) != 0)
          return cmp;
        break;
      case 'B':
        if((cmp = compare_two(A->board(), B->board())) != 0)
          return cmp;
        break;
      case 'D':
        if((cmp = stricmp(A->desc(), B->desc())) != 0)
          return cmp;
        break;
      case 'E':
        if((cmp = stricmp(A->echoid(), B->echoid())) != 0)
          return cmp;
        break;
      case 'F':
        if(*area_maybe) {
          if(striinc(area_maybe, A->echoid()) and not striinc(area_maybe, B->echoid()))
            return -1;
          if(striinc(area_maybe, B->echoid()) and not striinc(area_maybe, A->echoid()))
            return 1;
        }
        break;
      case 'G':
        {
          int ga = A->groupid();
          int gb = B->groupid();

          char* p = strpbrk(AL.sortspec, "tT");
          bool groupfirst = p > ptr;
          bool ignoretype = p == NULL or groupfirst;

          if(ga == 0 and ((not A->isseparator()^rev) or ignoretype))
            ga = INT_MAX;
          if(gb == 0 and ((not B->isseparator()^rev) or ignoretype))
            gb = INT_MAX;

          if((A->isseparator() or B->isseparator()) and ga == gb) {
            if(groupfirst and (cmp = compare_two(CFG->areatypeorder[A->type()&0xFF], CFG->areatypeorder[B->type()&0xFF])) != 0)
              return cmp;
            if((cmp = compare_two(b->isseparator(), a->isseparator())) != 0)
              return cmp;
          }
          else {
            if((cmp = compare_groups(ga, gb)) != 0)
              return cmp;
          }
        }
        break;
      case 'M':
        if(A->ismarked() and not B->ismarked())
          return -1;
        if(B->ismarked() and not A->ismarked())
          return 1;
        break;
      case 'P':
        if((cmp = compare_two(B->PMrk.Count()?1:0, A->PMrk.Count()?1:0)) != 0)
          return cmp;
        break;
      case 'O':
        if((cmp = compare_two(A->areaid(), B->areaid())) != 0)
          return cmp;
        break;
      case 'T':
        if((cmp = compare_two(CFG->areatypeorder[A->type()&0xFF], CFG->areatypeorder[B->type()&0xFF])) != 0)
          return cmp;
        if((cmp = compare_two(b->isseparator(), a->isseparator())) != 0)
          return cmp;
        break;
      case 'U':
        aunread = A->Msgn.Count() - A->lastread();
        bunread = B->Msgn.Count() - B->lastread();
        if(rev)
          cmp = compare_two(aunread, bunread);
        else
          cmp = compare_two(aunread ? aunread : INT_MAX, bunread ? bunread : INT_MAX);
        if(cmp != 0)
          return cmp;
        break;
      case 'X':
        if((cmp = compare_two(A->msgbase(), A->msgbase())) != 0)
          return cmp;
        break;
      case 'Y':
        aunread = A->Msgn.Count() - A->lastread();
        bunread = B->Msgn.Count() - B->lastread();
        if(aunread and not bunread)
          return -1;
        if(bunread and not aunread)
          return 1;
        break;
      case 'Z':
        if((cmp = stricmp(A->path(), B->path())) != 0)
          return cmp;
        break;
    }
    ptr++;
  }

  return cmp;
}

static bool AreaListCmp2(const Area* a, const Area* b) {

  return AreaListCmp(&a, &b) < 0;
}

//  ------------------------------------------------------------------
//  Arealist sort areas

void AreaList::Sort(const char* specs, int first, int last) {

  if(specs)
    strcpy(sortspec, specs);
  else
    strcpy(sortspec, CFG->arealistsort);
  if(last == -1)
    last = idx.size();
  if(*sortspec) {
    sort(idx.begin()+first, idx.begin()+last, AreaListCmp2);
  }
}


//  ------------------------------------------------------------------

int AreaList::AreaEchoToNo(const char* echoid) {

  for(uint n=0; n<idx.size(); n++)
    if(strieql(echoid, idx[n]->echoid()))
      return n;
  return -1;
}


//  ------------------------------------------------------------------

Area* AreaList::AreaEchoToPtr(const char* echoid) {

  for(uint n=0; n<idx.size(); n++)
    if(strieql(echoid, idx[n]->echoid()))
      return idx[n];
  return NULL;
}


//  ------------------------------------------------------------------

int AreaList::AreaNoToId(int __areano) {

  if(in_range(__areano, 0, int(idx.size()-1)))
    return idx[__areano]->areaid();
  else {
    LOG.ErrIndex();
    LOG.printf("! Arealist index out of bounds.");
    LOG.printf(": Tried to access record %i, but the range was only 0 - %u.", __areano, (uint)(idx.size()-1));
    LOG.printf("+ Advice: Report this to the author.");
    IndexErrorExit();
    return 0;
  }
}


//  ------------------------------------------------------------------

Area* AreaList::AreaNoToPtr(int __areano) {

  if(in_range(__areano, 0, int(idx.size()-1)))
    return idx[__areano];
  else {
    LOG.ErrIndex();
    LOG.printf("! Arealist index out of bounds.");
    LOG.printf(": Tried to access record %i, but the range was only 0 - %u.", __areano, (uint)(idx.size()-1));
    LOG.printf("+ Advice: Report this to the author.");
    IndexErrorExit();
    return 0;
  }
}


//  ------------------------------------------------------------------

int AreaList::AreaIdToNo(int __areaid) {

  uint _areano = 0;
  while(_areano < idx.size()) {
    if(idx[_areano]->areaid() == __areaid)
      return _areano;
    _areano++;
  }
  return -1;
}


//  ------------------------------------------------------------------

Area* AreaList::AreaIdToPtr(int __areaid) {

  int _areano = AreaIdToNo(__areaid);
  if(_areano != -1)
    return idx[_areano];
  return NULL;
}


//  ------------------------------------------------------------------

int AreaList::SetActiveAreaId(int __areaid) {

  int _areano = AreaIdToNo(__areaid);
  SetActiveAreaNo(_areano);
  return _areano;   // Return the areano
}


//  ------------------------------------------------------------------

class SelMaskPick : public gwinpick {

  gwindow window;
  int DESC_LEN;

  void open();                        // Called after window is opened
  void close();                       // Called after window is closed
  void print_line(uint idx, uint pos, bool isbar);
  bool handle_key();                  // Handles keypress

public:

  void Run();

};

void SelMaskPick::open() {

  window.openxy(ypos, xpos, ylen+2, xlen+2, btype, battr, 7);
  window.title(title, tattr);
  window.shadow(C_SHADOW);
  update();
}

void SelMaskPick::close() {

  window.close();

}

void SelMaskPick::print_line(uint idx, uint pos, bool isbar) {

  __extension__ char buf[DESC_LEN+3];

  *buf = ' '; strxcpy(buf+1, AL.alistselections[idx], DESC_LEN);
  window.printns(pos, 0, isbar ? sattr : wattr, buf, xlen);
}

bool SelMaskPick::handle_key() {

  switch(key) {
    case Key_Esc:
      aborted = true;

      /// Drop through
    case Key_Ent:
      return false;

    case Key_Tick:
      break;

    default:
      if(key < KK_Commands) {
        kbput(key);
        edit_string(AL.alistselections[index], DESC_LEN-1, LNG->SelectMarksEdit, H_SelectMarks);
        display_bar();
      }
      break;
  }

  return true;
}

void SelMaskPick::Run() {

  DESC_LEN = (sizeof(Desc) > (MAXCOL-6)) ? MAXCOL-6 : sizeof(Desc);

  ypos = (MAXROW-18)/2;
  xpos = (MAXCOL-DESC_LEN-4)/2-1;
  ylen = 16;
  xlen = DESC_LEN+2;
  btype = W_BMENU;
  battr = C_MENUB;
  wattr = C_MENUW;
  tattr = C_MENUT;
  sattr = C_MENUS;
  hattr = C_MENUQ;

  title = LNG->SelectMarks;
  helpcat = H_SelectMarks;
  listwrap = true;

  maximum_index = 15;
  maximum_position = 16;

  index = position = AL.mask;

  run_picker();

  if(not aborted)
    AL.mask = (byte)index;

};


void AreaList::Select_Mask() {

  SelMaskPick& SelMask = *new SelMaskPick;
  throw_new(&SelMask);

  SelMask.Run();

  extern bool in_arealist;
  extern GPickArealist* PickArealist;

  if(in_arealist) {
    PickArealist->update();
    PickArealist->do_delayed();
  }

  delete &SelMask;
}


//  ------------------------------------------------------------------

