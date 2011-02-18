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
//  GCUI: Golded+ Character-oriented User Interface.
//  Functions for line drawing.
//  Based on CXL by Mike Smedley.
//  ------------------------------------------------------------------
//  whline() draws a horizontal text line in active window
//  wvline() draws a vertical text line in active window
//  ------------------------------------------------------------------

#include <gwinall.h>


//  ------------------------------------------------------------------

#define HORZ  0
#define VERT  1


//  ------------------------------------------------------------------

#define ULC   _box_table(bt, 0)    // upper left corner
#define UHL   _box_table(bt, 1)    // upper horizontal line
#define URC   _box_table(bt, 2)    // upper right corner
#define LVL   _box_table(bt, 3)    // left vertical line
#define RVL   _box_table(bt, 4)    // right vertical line
#define LLC   _box_table(bt, 5)    // lower left corner
#define LHL   _box_table(bt, 6)    // lower horizontal line
#define LRC   _box_table(bt, 7)    // lower right corner
#define MJ    _box_table(bt, 8)    // middle junction
#define LVJ   _box_table(bt, 9)    // left vertical junction
#define RVJ   _box_table(bt, 10)   // right vertical junction
#define UHJ   _box_table(bt, 11)   // upper horizontal junction
#define LHJ   _box_table(bt, 12)   // lower horizontal junction


//  ------------------------------------------------------------------

static int disp_char(int wrow,int wcol, vattr attr,int btype,vchar ch,int direc) {

  attr = attr|ACSET;

  // see if next to a border, if so, connect to it
  if(gwin.active->border) {

    // abbreviate pointer
    const int bt = btype;

    // calculate effective row and column
    int row = gwin.active->srow+gwin.active->border+wrow;
    int col = gwin.active->scol+gwin.active->border+wcol;

    // see if this is a horizontal or vertical line
    if(direc==HORZ) {

      // make sure that the box type characters match
      if(LVL==_box_table(gwin.active->btype, 3)) {

        // check left border
        if(col==(gwin.active->scol+1)) {
          vputc(row,gwin.active->scol,attr,LVJ);
          ch=UHL;
        }

        // check right border
        if(col==(gwin.active->ecol-1)) {
          vputc(row,gwin.active->ecol,attr,RVJ);
          ch=UHL;
        }
      }
    }
    else {

      // make sure that the box type characters match
      if(UHL==_box_table(gwin.active->btype, 1)) {

        // check top border
        if(row==(gwin.active->srow+1)) {
          vputc(gwin.active->srow,col,attr,UHJ);
          ch=LVL;
        }

        // check bottom border
        if(row==(gwin.active->erow-1)) {
          vputc(gwin.active->erow,col,attr,LHJ);
          ch=LVL;
        }
      }
    }
  }

  // display character
  if(wprintc(wrow,wcol,attr,ch))
    return gwin.werrno;

  // return normally
  return 0;
} /* disp_char() */


//  ------------------------------------------------------------------

static inline int isupvert(int btype, vchar ch) {

  const int bt = btype;
  return (ch==LVL or ch==UHJ or ch==ULC or ch==URC or ch==LVJ or ch==RVJ or ch==MJ) ? YES : NO;
}


//  ------------------------------------------------------------------

static inline int isdownvert(int btype, vchar ch) {

  const int bt = btype;
  return (ch==LVL or ch==LHJ or ch==LLC or ch==LRC or ch==LVJ or ch==RVJ or ch==MJ) ? YES : NO;
}


//  ------------------------------------------------------------------

static inline int islefthorz(int btype, vchar ch) {

  const int bt = btype;
  return (ch==UHL or ch==LVJ or ch==LLC or ch==ULC or ch==UHJ or ch==LHJ or ch==MJ) ? YES : NO;
}


//  ------------------------------------------------------------------

static inline int isrighthorz(int btype, vchar ch) {

  const int bt = btype;
  return (ch==UHL or ch==RVJ or ch==LRC or ch==URC or ch==UHJ or ch==LHJ or ch==MJ) ? YES : NO;
}


//  ------------------------------------------------------------------

int whline(int wsrow, int wscol, int count, int btype, vattr attr) {

  register int bt;
  int row,col,up,down;
  vchar ch;

  row=wsrow;
  col=wscol;

  // abbreviate pointer
  bt = btype;

  if(count) {

    // see if a left junction or corner is needed
    up   = isupvert  (btype,wgetc(row-1,col));
    down = isdownvert(btype,wgetc(row+1,col));
    if(up and down)
      ch=LVJ;
    else if(up)
      ch=LLC;
    else if(down)
      ch=ULC;
    else
      ch=UHL;

    // display leftmost character
    if(disp_char(row,col,attr,btype,ch,HORZ))
      return gwin.werrno;
    col++;
    count--;
  }

  // do while not last character
  while(count>1) {

    // see if a middle junction is needed
    up   = isupvert  (btype,wgetc(row-1,col));
    down = isdownvert(btype,wgetc(row+1,col));
    if(up and down)
      ch=MJ;
    else if(up)
      ch=LHJ;
    else if(down)
      ch=UHJ;
    else
      ch=UHL;

    // display middle character
    if(disp_char(row,col,attr,btype,ch,HORZ))
      return gwin.werrno;
    col++;
    count--;
  }

  if(count) {

    // see if a right junction or corner is needed
    up   = isupvert  (btype,wgetc(row-1,col));
    down = isdownvert(btype,wgetc(row+1,col));
    if(up and down)
      ch=RVJ;
    else if(up)
      ch=LRC;
    else if(down)
      ch=URC;
    else
      ch=UHL;

    // display rightmost character
    if(disp_char(row,col,attr,btype,ch,HORZ))
      return gwin.werrno;
  }

  // return normally
  return gwin.werrno=W_NOERROR;
} /* whline() */


//  ------------------------------------------------------------------

int wvline(int wsrow, int wscol, int count, int btype, vattr attr) {

  register int bt;
  int row,col,left,right;
  vchar ch;

  row=wsrow;
  col=wscol;

  // abbreviate pointer
  bt = btype;

  if(count) {

    // see if a top junction or corner is needed
    left  = islefthorz (btype,wgetc(row,col-1));
    right = isrighthorz(btype,wgetc(row,col+1));
    if(left and right)
      ch=UHJ;
    else if(left)
      ch=URC;
    else if(right)
      ch=ULC;
    else
      ch=LVL;

    // display uppermost character
    if(disp_char(row,col,attr,btype,ch,VERT))
      return gwin.werrno;
    row++;
    count--;
  }

  // do while not last character
  while(count>1) {
    left  = islefthorz (btype,wgetc(row,col-1));
    right = isrighthorz(btype,wgetc(row,col+1));
    if(left and right)
      ch=MJ;
    else if(left)
      ch=RVJ;
    else if(right)
      ch=LVJ;
    else
      ch=LVL;

    // display middle character
    if(disp_char(row,col,attr,btype,ch,VERT))
      return gwin.werrno;
    row++;
    count--;
  }

  if(count) {

    // see if a bottom junction or corner is needed
    left  = islefthorz (btype,wgetc(row,col-1));
    right = isrighthorz(btype,wgetc(row,col+1));
    if(left and right)
      ch=LHJ;
    else if(left)
      ch=LRC;
    else if(right)
      ch=LLC;
    else
      ch=LVL;

    // display bottommost character
    if(disp_char(row,col,attr,btype,ch,VERT))
      return gwin.werrno;
  }

  // return normally
  return gwin.werrno=W_NOERROR;
} /* wvline() */

//  ------------------------------------------------------------------
