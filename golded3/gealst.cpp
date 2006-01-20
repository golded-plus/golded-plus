
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

#if defined(_MSC_VER)
    /* C4786: 'identifier' : identifier was truncated to 'number'
          characters in the debug information
    */
  #pragma warning(disable: 4786)
#endif
#include <algorithm>
#include <golded.h>

#if defined(__USE_ALLOCA__)
#include <malloc.h>
#endif


//  ------------------------------------------------------------------

AreaList AL;

int AreaTypeOrder[17] = {
  0,
  1,      // GMB_NET
  2,      // GMB_EMAIL
  3,      // GMB_NET | GMB_EMAIL
  4,      // GMB_ECHO
  5,
  6,
  7,
  8,      // GMB_NEWSGROUP
  9,
  10,
  11,
  12,     // GMB_ECHO | GMB_NEWSGROUP
  13,
  14,
  15,
  16      // GMB_LOCAL
};


//  ------------------------------------------------------------------
//  Areagroups compare

int compare_groups(int _ga, int _gb)
{
  register int ga = _ga ? _ga : INT_MAX;
  register int gb = _gb ? _gb : INT_MAX;

  const char *gap = NULL;
  const char *gbp = NULL;

  const char *g;

  for(g = CFG->arealistgrouporder; *g != NUL;) {

    int gr = getgroup(g);

    if(gr == ga)
      gap = g;

    if(gr == gb)
      gbp = g;

    if(*g == '#') {
      do {
        g++;
      } while(isdigit(*g));
    }
    else
      g++;
  }

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

  bool sepfirst = false;

  bool rev = false;
  char* ptr = AL.sortspec;

  while(*ptr) {
    switch(*ptr) {
      case '-':
        rev = true;
        A = b; B = a;
        break;
      case '+':
        rev = false;
        A = a; B = b;
        break;
      case 'A':
      case 'a':
        sepfirst = true;
        if((cmp = A->aka().compare(B->aka())) != 0)
          return cmp;
        break;
      case 'B':
      case 'b':
        if(A->isseparator() and B->isseparator())
          break;
        else if(A->isseparator()) {
          if(B->board())
            return sepfirst?-1:1;
        }
        else if(B->isseparator()) {
          if(A->board())
            return sepfirst?1:-1;
        }
        else if((cmp = compare_two(A->board(), B->board())) != 0)
          return cmp;
        break;
      case 'D':
      case 'd':
        if((cmp = stricmp(A->desc(), B->desc())) != 0)
          return cmp;
        break;
      case 'E':
      case 'e':
        if((cmp = stricmp(A->echoid(), B->echoid())) != 0)
          return cmp;
        break;
      case 'F':
      case 'f':
        if(*area_maybe) {
          bool amay = make_bool(striinc(area_maybe, A->echoid()));
          bool bmay = make_bool(striinc(area_maybe, B->echoid()));

          if((cmp = compare_two(bmay, amay)) != 0)
            return cmp;
        }
        break;
      case 'G':
      case 'g':
        sepfirst = true;
        if((cmp = compare_groups(A->groupid(), B->groupid())) != 0)
          return cmp;
        break;
      case 'M':
      case 'm':
        if(A->isseparator() and B->isseparator())
          break;
        else if(A->isseparator()) {
          if(B->ismarked())
            return sepfirst?-1:1;
        }
        else if(B->isseparator()) {
          if(A->ismarked())
            return sepfirst?1:-1;
        }
        else if((cmp = compare_two(B->ismarked(), A->ismarked())) != 0)
          return cmp;
        break;
      case 'P':
        if(A->isseparator() and B->isseparator())
          break;
        else if(A->isseparator()) {
          if(B->PMrk.Count())
            return sepfirst?-1:1;
        }
        else if(B->isseparator()) {
          if(A->PMrk.Count())
            return sepfirst?1:-1;
        }
        else {
          register int aunread = A->PMrk.Count();
          register int bunread = B->PMrk.Count();

          aunread = (rev or aunread) ? aunread : INT_MAX;
          bunread = (rev or bunread) ? bunread : INT_MAX;
          if((cmp = compare_two(aunread, bunread)) != 0)
            return cmp;
        }
        break;
      case 'p':
        if(A->isseparator() and B->isseparator())
          break;
        else if(A->isseparator()) {
          if(B->PMrk.Count())
            return sepfirst?-1:1;
        }
        else if(B->isseparator()) {
          if(A->PMrk.Count())
            return sepfirst?1:-1;
        }
        else if((cmp = compare_two(B->PMrk.Count()?1:0, A->PMrk.Count()?1:0)) != 0)
          return cmp;
        break;
      case 'O':
      case 'o':
        if((cmp = compare_two(A->areaid(), B->areaid())) != 0)
          return cmp;
        break;
      case 'T':
      case 't':
        sepfirst = true;
        if((cmp = compare_two(CFG->areatypeorder[A->type()&0xFF], CFG->areatypeorder[B->type()&0xFF])) != 0)
          return cmp;
        break;
      case 'U':
        if(A->isseparator() and B->isseparator())
          break;
        else if(A->isseparator()) {
          if(B->unread)
            return sepfirst?-1:1;
        }
        else if(B->isseparator()) {
          if(A->unread)
            return sepfirst?1:-1;
        }
        else {
          register int aunread = A->unread;
          register int bunread = B->unread;

          aunread = (rev or aunread) ? aunread : INT_MAX;
          bunread = (rev or bunread) ? bunread : INT_MAX;
          if((cmp = compare_two(aunread, bunread)) != 0)
            return cmp;
        }
        break;
      case 'u':
        if(A->isseparator() and B->isseparator())
          break;
        else if(A->isseparator()) {
          if(B->unread)
            return sepfirst?-1:1;
        }
        else if(B->isseparator()) {
          if(A->unread)
            return sepfirst?1:-1;
        }
        else if((cmp = compare_two(B->unread?1:0, A->unread?1:0)) != 0)
          return cmp;
        break;
      case 'X':
      case 'x':
        if(A->isseparator() and B->isseparator())
          break;
        else if(A->isseparator()) {
          return sepfirst?-1:1;
        }
        else if(B->isseparator()) {
          return sepfirst?1:-1;
        }
        else if((cmp = strcmp(A->basetype(), B->basetype())) != 0)
          return cmp;
        break;
      case 'Y':
      case 'y':
        if(A->isseparator() and B->isseparator())
          break;
        else if(A->isseparator()) {
          if(B->isnewmail())
            return sepfirst?-1:1;
        }
        else if(B->isseparator()) {
          if(A->isnewmail())
            return sepfirst?1:-1;
        }
        else if((cmp = compare_two(B->isnewmail(), A->isnewmail())) != 0)
          return cmp;
        break;
      case 'Z':
      case 'z':
        if(A->isseparator() and B->isseparator())
          break;
        else if(A->isseparator())
          return sepfirst?-1:1;
        else if(B->isseparator())
          return sepfirst?1:-1;
        else if((cmp = stricmp(A->path(), B->path())) != 0)
          return cmp;
        break;
      case 'S':
      case 's':
        if((cmp = compare_two(B->isseparator(), A->isseparator())) != 0)
          return cmp;
        break;
    }
    ptr++;
  }

  if(cmp == 0) {
    cmp = compare_two(B->isseparator(), A->isseparator());
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
    std::sort(idx.begin()+first, idx.begin()+last, AreaListCmp2);
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

  window.openxy(ypos, xpos, ylen+2, xlen+2, btype, battr, LGREY_|_BLACK);
  window.title(title, tattr);
  window.shadow(C_SHADOW);
  update();
}

void SelMaskPick::close() {

  window.close();

}

void SelMaskPick::print_line(uint idx, uint pos, bool isbar) {

#if defined(__USE_ALLOCA__)
  char *buf = (char*)alloca(DESC_LEN+3);
#else
   __extension__ char buf[DESC_LEN+3];
#endif

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

}


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

