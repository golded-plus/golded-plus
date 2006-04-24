//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999-2000 Alexander S. Aganichev
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
//  Based on CXL by Mike Smedley.
//  ------------------------------------------------------------------
//  Lets user pick from an array of strings.
//  ------------------------------------------------------------------

#include <gctype.h>
#include <cstdlib>
#include <cstring>
#include <gkbdcode.h>
#include <gmoubase.h>
#include <gtimall.h>
#include <gwinall.h>

extern char m_title[];


//  ------------------------------------------------------------------

int wpickstr_tag = false;


//  ------------------------------------------------------------------
//  define record that will hold pick window info

struct r_t {
  int   numelems;
  int   lastelem;
  int   curr;
  int   first;
  int   last;
  int   strsperline;
  int   strsperwin;
  int   maxstrlen;
  int   wwidth;
  int   wheight;
  int   fillspaces;
  int   gapspaces;
  int   xtraspaces;
  vattr winattr;
  vattr barattr;
  int   scrollbar;
};


//  ------------------------------------------------------------------
//  this function will display the mouse
//  cursor if mouse cursor mode is on

static void show_mouse_cursor_pck() {

  #ifdef GOLD_MOUSE
  if(gmou.FreeCursor()) {
    gmou.ShowCursor();
    gmou.SetCursor(0,0xFFFF,((LGREY_|_LGREY)<<8));
  }
  #endif
}


//  ------------------------------------------------------------------
//  this function will hide the mouse
//  cursor if mouse cursor mode is on

static void hide_mouse_cursor_pck() {

  #ifdef GOLD_MOUSE
  if(gmou.FreeCursor())
    gmou.HideCursor();
  #endif
}


//  ------------------------------------------------------------------
//  this function updates the current item by either
//  displaying or erasing the selection bar on it

static void update_curr(char* strarr[], r_t* r, int bar) {

  // calculate row and column string will be displayed
  // at, then print out the string character-by-character

  int crow = (r->curr-r->first)/r->strsperline;
  int temp = r->curr-((crow*r->strsperline)+r->first);
  int ccol = (temp*r->maxstrlen)+((temp+1)*(r->gapspaces))+r->xtraspaces;

  wgotoxy(crow,ccol);

  hide_mouse_cursor_pck();

  wprintns(crow, ccol, bar ? r->barattr : r->winattr, strarr[r->curr], r->maxstrlen);

  show_mouse_cursor_pck();
}


//  ------------------------------------------------------------------

static void update_line(char* strarr[], r_t* r, int wrow, int upcurr) {

  int nomore = false;
  int ccol = r->gapspaces + r->xtraspaces;
  int celem = (wrow*r->strsperline) + r->first;

  if(celem >= r->numelems)
    nomore = true;

  for(int j=0; j<r->strsperline; j++) {
    int ccolbeg = ccol;
    wprintns(wrow, ccolbeg, (upcurr and r->curr==celem) ? r->barattr : r->winattr, nomore ? "" : strarr[celem], r->maxstrlen);
    ccol += r->maxstrlen;
    if(++celem >= r->numelems)
      nomore = true;
    ccol += r->gapspaces;
  }
}


//  ------------------------------------------------------------------
//  this function will update all items in the window

static void update_window(char* strarr[], r_t* r) {

  hide_mouse_cursor_pck();
  for(int crow=0; crow<r->wheight; crow++)
    update_line(strarr, r, crow, 1);
  show_mouse_cursor_pck();
}


//  ------------------------------------------------------------------
//  this function will find the element number of the
//  first element on the same line as the given element

static int e_begline(r_t* r, int elem) {

  return (elem/r->strsperline) * r->strsperline;
}


//  ------------------------------------------------------------------
//  this function will find the first element in a
//  window, using a given last element as input

static int e_begwin(r_t* r, int lelem) {

  int beg = e_begline(r,lelem) + r->strsperline - r->strsperwin;
  if(beg < 0)
    beg = 0;

  return beg;
}


//  ------------------------------------------------------------------
//  this function will find the element number of the
//  last element on the same line as the given element

static int e_endline(r_t* r, int elem) {

  int end = (((elem/r->strsperline)+1)*r->strsperline)-1;
  if(end>=r->numelems)
    end = r->lastelem;

  return end;
}


//  ------------------------------------------------------------------
//  this function will find the last element in a
//  window, using a given first element as input

static int e_endwin(r_t* r, int felem) {

  int end = felem + r->strsperwin - 1;
  if(end > (r->lastelem))
    end = r->lastelem;

  return end;
}


//  ------------------------------------------------------------------

static void goto_item(r_t* r, char* strarr[], int elem) {

  if(elem<0 or elem>r->lastelem)
    elem = 0;
  int outside = (elem<r->first or elem>r->last) ? YES : NO;
  if(outside == NO)
      update_curr(strarr,r,0);
  r->curr = elem;
  if(outside) {
    r->first = e_begwin(r,r->last = e_endwin(r,e_begline(r,r->curr)));
    update_window(strarr,r);
  }
  else {
    update_curr(strarr,r,1);
  }
}


//  ------------------------------------------------------------------
//  this function determines if the mouse cursor is on a item

#ifdef GOLD_MOUSE
static int mouse_on_item(r_t* r, int mcrow, int mccol) {

  int found  = -1;
  int srow   = gwin.active->srow;
  int scol   = gwin.active->scol;
  int border = gwin.active->border;

  for(int i=r->first; i<=r->last; i++) {
    if(mcrow==(srow+border+((i-r->first)/r->strsperline))) {
      int start = scol+border+r->xtraspaces+r->gapspaces+((i%r->strsperline)*(r->maxstrlen+r->gapspaces));
      int end   = start+r->maxstrlen-1;
      if(mccol>=start and mccol<=end) {
        found = i;
        break;
      }
    }
  }
  if(found==-1 and r->scrollbar and mccol==gwin.active->ecol) {
    if(mcrow==srow+1)
      found=-2;
    else {
      if(mcrow==gwin.active->erow-1)
        found=-3;
    }
  }

  return found;
}
#endif


//  ------------------------------------------------------------------

static void page_down(char* strarr[], r_t* r) {

  if(r->curr != r->last) {
    r->curr = r->last;
    update_window(strarr, r);
  }
  else if(r->last != (r->lastelem)) {
    int i = r->curr-r->first;
    r->last = e_endwin(r,r->last+1);
    r->first = e_begwin(r, r->last);
    if((r->curr = r->first+i) > (r->lastelem))
      r->curr -= r->strsperline;
    update_window(strarr, r);
  }
}


//  ------------------------------------------------------------------

static void page_up(char* strarr[], r_t* r) {

  if(r->curr != r->first) {
    r->curr = r->first;
    update_window(strarr, r);
  }
  else if(r->first) {
    int i = r->curr-r->first;
    r->first = e_begwin(r,r->first-1);
    r->last = e_endwin(r, r->first);
    r->curr = r->first+i;
    update_window(strarr,r);
  }
}


//  ------------------------------------------------------------------

static void scroll_down(char* strarr[], r_t* r, int upcurr) {

  if(r->first) {
    hide_mouse_cursor_pck();
    if(upcurr)
      update_curr(strarr,r,0);
    r->first -= r->strsperline;
    r->last = e_endline(r,r->last-r->strsperline);
    if(upcurr>1)
      r->curr -= r->strsperline;
    if(r->first != e_begline(r,r->last))
      wscroll(1,SDOWN);
    update_line(strarr, r, 0, (upcurr>2) ? 0 : upcurr);
    show_mouse_cursor_pck();
  }
}


//  ------------------------------------------------------------------

static void scroll_up(char* strarr[], r_t* r, int upcurr) {

  if(r->last!=(r->lastelem)) {
    hide_mouse_cursor_pck();
    if(upcurr)
      update_curr(strarr,r,0);
    r->first+=r->strsperline;
    r->last=e_endline(r,r->last+1);
    if(upcurr>1)
      if((r->curr+r->strsperline)<=r->last)
        r->curr+=r->strsperline;
    if(r->first!=e_begline(r,r->last))
      wscroll(1,SUP);
    update_line(strarr,r,r->wheight-1,(upcurr>2)?0:upcurr);
    show_mouse_cursor_pck();
  }
}


//  ------------------------------------------------------------------
//  this function reads the mouse for input

static gkey read_mouse(char* strarr[], r_t* r) {

  #ifdef GOLD_MOUSE
  // if free-floating mouse cursor support is on
  if(gmou.FreeCursor()) {

    // clear mouse button queue
    gmou.ClearEvents();

    // loop until a key is pressed
    while(!kbxhit() and gkbd.kbuf==NULL) {

      // call the keyboard loop function, if defined
      //if(gkbd.kbloop!=NULL)
        //(*gkbd.kbloop)();

      // see if the right button (Esc) was pressed
      gmou.GetRightRelease();
      if(gmou.Count())
        return Key_Esc;

      // see where mouse cursor is at - if it is on a scroll bar
      // or menu item, and the left button is pressed, then scroll
      // menu or select item
      gmou.GetStatus();
      int i = mouse_on_item(r,gmou.Row(),gmou.Column());
      switch(i) {
        case -1:
          gmou.ClearEvents();
          break;
        case -2:
          if(gmou.LeftButton()) {
            scroll_down(strarr,r,3);
            if(gvid->isbios())
              usleep(50);
            gmou.ClearEvents();
          }
          break;
        case -3:
          if(gmou.LeftButton()) {
            scroll_up(strarr,r,3);
            if(gvid->isbios())
              usleep(50);
            gmou.ClearEvents();
          }
          break;
        default:
          gmou.GetLeftRelease();
          if(gmou.Count()) {
            r->curr = i;
            return Key_Ent;
          }
      }
    }
  }
  #endif

  // return zero - it means a key was pressed
  return 0;
}


//  ------------------------------------------------------------------

int wpickstr(int srow, int scol, int erow, int ecol, int btype, vattr bordattr, vattr winattr, vattr barattr, char* strarr[], int initelem, VfvCP open) {

  int i, j, maxlen, outside;
  gkey xch;
  char ch;
  char* p;
  r_t r;

  int quickpos = (strarr[0][0] == ' ') ? 1 : 0;

  // go through input array and determine the longest
  // string, and count the number of elements in the array

  maxlen = strlen(m_title);

  for (i = 0; strarr[i] != NULL; i++)
    if ((j = strlen(strarr[i])) > maxlen)
      maxlen = j;

  r.maxstrlen = maxlen;
  r.lastelem = ((r.numelems=i)-1);
  r.winattr = winattr;
  r.barattr = barattr;

  // see if window is to have a border
  int border = (btype==5) ? NO : YES;

  // if ecol == -1 then adjust it to conform to length of longest string
  if(ecol==-1)
    ecol = scol+border+r.maxstrlen+border-1;

  // calculate window area width and height
  r.wwidth  = (ecol-border)-(scol+border)+1;
  r.wheight = (erow-border)-(srow+border)+1;

  // make sure longest string can fit in window
  if(r.maxstrlen > r.wwidth)
    r.maxstrlen = r.wwidth;

  // open window which strings will reside in
  hide_mouse_cursor_pck();
  if(!wopen(srow,scol,erow,ecol,btype,bordattr,winattr,bordattr))
    return -1;

  // if mouse cursor mode is on and window has a border,
  // display scroll indicator arrows on right window border
  #ifdef GOLD_MOUSE
  if(gmou.FreeCursor() and (btype!=5) and ((srow+2) < erow)) {
    vputc(srow+1,ecol,bordattr,(char) 24);
    vputc(erow-1,ecol,bordattr,(char) 25);
    r.scrollbar=true;
  }
  else {
  #endif
    r.scrollbar=false;
  #ifdef GOLD_MOUSE
  }
  #endif
  show_mouse_cursor_pck();

  // if an open function has been specified, then call it
  if(open!=NULL)
    open();

  // if mouse cursor mode is on, then turn on mouse cursor
  show_mouse_cursor_pck();

  // calculate how many strings can fit into 1 window line, number of
  // filler spaces needed per window line, number of spaces per gap in
  // between strings, number of extra spaces to add to first gap, and
  // number of strings that can fit inside the window
  r.strsperline = (r.wwidth-2)/(r.maxstrlen+2);
  if(!r.strsperline)
    r.strsperline++;
  r.fillspaces = r.wwidth-(r.strsperline*r.maxstrlen);
  r.gapspaces = r.fillspaces/(r.strsperline+1);
  r.xtraspaces = (r.fillspaces%(r.strsperline+1))/2;
  r.strsperwin = r.strsperline*r.wheight;

  // initialize first, last, and current elements
  r.curr = r.first = 0;
  r.last = (r.numelems<r.strsperwin) ? (r.lastelem) : (r.strsperwin-1);

  // move selection bar to specified initial element
  goto_item(&r,strarr,initelem);

  // fill window with as many strings as can fit and display bar
  update_window(strarr,&r);

  // loop until an element is selected or Escape is pressed
  for(;;) {

    // get keystroke/mouse movement
    gkbd.inmenu = true;
    xch = read_mouse(strarr,&r);
    if(!xch) {
      xch = getxch();
    }
    gkbd.inmenu=false;

    // test keystroke for a special key
    switch(xch) {

      case Key_Space:
        if(wpickstr_tag) {
          p = strarr[r.curr];
          switch(*p) {
            case ' ':
              *p = (char)wpickstr_tag;
              update_curr(strarr,&r,1);
              break;
            default:
              *p = ' ';
              update_curr(strarr,&r,1);
          }
          kbput(Key_Dwn);
        }
        break;

      case Key_Ins:
      case Key_PlsG:
      case Key_S_Equ:
        if(wpickstr_tag) {
          strarr[r.curr][0] = (char)wpickstr_tag;
          update_curr(strarr,&r,1);
          kbput(Key_Dwn);
        }
        break;

      case Key_Del:
      case Key_Min:
      case Key_MinG:
        if(wpickstr_tag) {
          strarr[r.curr][0] = ' ';
          update_curr(strarr,&r,1);
          kbput(Key_Dwn);
        }
        break;

      case Key_StrG:
      case Key_S_8:
        if(wpickstr_tag) {
          for(i=0; i<r.numelems; i++)
            strarr[i][0] = (char)wpickstr_tag;
          update_window(strarr,&r);
        }
        break;

      case Key_Sls:
        if(wpickstr_tag) {
          for(i=0; i<r.numelems; i++)
            strarr[i][0] = ' ';
          update_window(strarr,&r);
        }
        break;

      case Key_Esc:

        // close window and set error to Escape press code
        if(gwin.esc) {
          hide_mouse_cursor_pck();
          wclose();
          gwin.werrno=W_ESCPRESS;
          return -1;
        }
        break;

      case Key_Ent:

        // close window and return subscript of selected string
        hide_mouse_cursor_pck();
        wclose();
        gwin.werrno=W_NOERROR;
        return(r.curr);

      case Key_Lft:

        // move to the item to the left, scrolling if necessary
        if(r.curr) {
          update_curr(strarr,&r,0);
          if(r.curr==r.first)
            scroll_down(strarr,&r,0);
          r.curr--;
          update_curr(strarr,&r,1);
        }
        break;

      case Key_Rgt:

        // move to the item to the right, scrolling if necessary
        if(r.curr!=r.lastelem) {
          update_curr(strarr,&r,0);
          if(r.curr==r.last)
            scroll_up(strarr,&r,0);
          r.curr++;
          update_curr(strarr,&r,1);
        }
        break;

      case Key_Up:

        // move to the item up, scrolling if necessary
        if((r.curr-r.strsperline)<r.first)
          scroll_down(strarr,&r,2);
        else {
          update_curr(strarr,&r,0);
          r.curr-=r.strsperline;
          update_curr(strarr,&r,1);
        }
        break;

      case Key_Dwn:

        // move to the item down, scrolling if necessary
        if((r.curr+r.strsperline)>r.last)
          scroll_up(strarr,&r,((r.curr+r.strsperline)>r.lastelem) ? 0 : 2);
        else {
          update_curr(strarr,&r,0);
          r.curr+=r.strsperline;
          update_curr(strarr,&r,1);
        }
        break;

      case Key_PgUp:

        // move up 1 page.  adjust position if at 1st element
        page_up(strarr,&r);
        break;

      case Key_PgDn:

        // move down 1 page.  adjust position if at last element
        page_down(strarr,&r);
        break;

      case Key_Home:

        // set position to 1st element
        if(r.curr) {
          if((outside=r.first)==NO)
            update_curr(strarr,&r,0);
          r.first=r.curr=0;
          if(outside) {
            r.last=e_endwin(&r,r.first);
            update_window(strarr,&r);
          }
          else {
            update_curr(strarr,&r,1);
          }
        }
        break;

      case Key_End:

        // set position to last element
        if(r.curr!=r.lastelem) {
          if((outside=(r.last<(r.lastelem))?YES:NO)==NO)
            update_curr(strarr,&r,0);
          r.last=r.curr=r.lastelem;
          if(outside) {
            r.first=e_begwin(&r,r.last);
            update_window(strarr,&r);
          }
          else {
            update_curr(strarr,&r,1);
          }
        }
        break;

      default:

        // if not an extended keypress, then search from current
        // position for the item that begins with the same ASCII
        // character as the keypress.  If not found after current
        // position, search from the beginning for a match
        ch = (char)g_toupper(char(xch & 0xFF));
        if(!ch)
          break;
        for(i=r.curr+1; i<r.numelems; i++)
          if(ch==g_toupper(strarr[i][quickpos]))
            break;
        if(i==r.numelems) {
          for(i=0;i<r.curr;i++)
            if(ch==g_toupper(strarr[i][quickpos]))
              break;
          if(i==r.curr)
            continue;
        }

        // a matching ASCII character was found.  set position
        // to matching element, adjusting window if necessary
        goto_item(&r,strarr,i);
    }
  }
}


//  ------------------------------------------------------------------
