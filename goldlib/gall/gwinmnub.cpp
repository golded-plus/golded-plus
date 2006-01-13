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
//  Menu class.
//  ------------------------------------------------------------------

#include <cstdlib>
#include <cstring>
#include <gmnubase.h>


//  ------------------------------------------------------------------

void GMnu::Init() {

  bordertype = 0;
  bordercolor = BLACK|_BLACK;

  textcolor = BLACK|_BLACK;
  quickcolor = BLACK|_BLACK;
  noselcolor = BLACK|_BLACK;
  barcolor = BLACK|_BLACK;
  shadowcolor = DEFATTR;

  title = NULL;
  titlepos = TCENTER;
  titlecolor = BLACK|_BLACK;

  deschdl = -1;
  descrow = 0;
  desccolumn = 0;
  desccolor = BLACK|_BLACK;

  helpnumber = -1;

  beginrow = 0;
  begincolumn = 0;

  menuopen = NULL;
  itemmask = 0;

  escape = YES;
  finaltag = -1;
  status = W_NOERROR;

  depth = -1;
  memset(stack, 0, sizeof(stack));
}


//  ------------------------------------------------------------------

void GMnu::SetBorder(int type, vattr color) {

  bordertype = type;
  bordercolor = color;
}


//  ------------------------------------------------------------------

void GMnu::SetColor(vattr text, vattr quick, vattr nosel, vattr bar, vattr shadow) {

  textcolor = text;
  quickcolor = quick;
  noselcolor = nosel;
  barcolor = bar;
  shadowcolor = shadow;
}


//  ------------------------------------------------------------------

void GMnu::SetTitle(const char* text, vattr color, int pos) {

  title = text;
  titlepos = pos;
  titlecolor = color;
}


//  ------------------------------------------------------------------

void GMnu::SetTitle(const char* text) {

  title = text;
}


//  ------------------------------------------------------------------

void GMnu::SetDesc(int hdl, int row, int col, vattr color) {

  deschdl = hdl;
  descrow = row;
  desccolumn = col;
  desccolor = color;
}


//  ------------------------------------------------------------------

void GMnu::SetPos(int row, int col, int width, int height) {

  beginrow = row;
  begincolumn = col;
  beginwidth = width;
  beginheight = height;
}


//  ------------------------------------------------------------------

void GMnu::SetEsc(int option) {

  escape = option;
}


//  ------------------------------------------------------------------

void GMnu::SetHelp(int help) {

  helpnumber = help;
}


//  ------------------------------------------------------------------

void GMnu::SetMask(int mask) {

  itemmask = mask;
}


//  ------------------------------------------------------------------

void GMnu::SetTag(int tag) {

  stack[depth].tag = tag;
}


//  ------------------------------------------------------------------

void GMnu::Begin(int type) {

  bool was_horz = make_bool(stack[depth].type & M_HORZ);
  depth++;
  stack[depth].tag = -1;
  stack[depth].type = type | M_SAVE;
  stack[depth].winrow = (beginrow != -1) ? beginrow : (was_horz ? stack[depth-1].winrow+1 : stack[depth-1].winrow+stack[depth-1].itemrow);
  stack[depth].wincolumn = (begincolumn != -1) ? begincolumn : (stack[depth-1].wincolumn+(was_horz?0:1));
  stack[depth].itemrow = stack[depth].itemcolumn = 0;
  status = wmenubeg_(
    stack[depth].winrow,
    stack[depth].wincolumn,
    beginheight,
    beginwidth,
    bordertype,
    bordercolor,
    textcolor,
    menuopen,
    stack[depth].type
  );
  wmenutitshad(title, titlepos, titlecolor, shadowcolor);
  beginwidth = beginheight = 0;
  beginrow = begincolumn = -1;
}


//  ------------------------------------------------------------------

void GMnu::End() {

  status = wmenuend(stack[depth].tag, stack[depth].type, (stack[depth].type&M_VERT)?255:0, 0, textcolor, quickcolor, noselcolor, barcolor);
  depth--;
  if((depth >= 0) and (stack[depth].type & M_HORZ))
    stack[depth].wincolumn += stack[depth].winwidth;
  title = NULL;
}


//  ------------------------------------------------------------------

extern int _overtagid;
extern int _finaltagid;

void GMnu::Start() {

  int _prev_escape = wsetesc(escape);
  wmenuget();
  finaltag = _finaltagid;
  overtag = _overtagid;
  wsetesc(_prev_escape);
}


//  ------------------------------------------------------------------

void GMnu::Item(int tag, const char* text) {

  Item(tag, text, itemmask, NULL, 0);
}


//  ------------------------------------------------------------------

void GMnu::Item(int tag, const char* text, int fmask) {

  Item(tag, text, fmask, NULL, 0);
}


//  ------------------------------------------------------------------

void GMnu::Item(int tag, const char* text, VfvCP select, int fmask) {

  Item(tag, text, fmask, select, 0);
}


//  ------------------------------------------------------------------

void GMnu::Item(int tag, const char* text, int fmask, VfvCP select, gkey hotkey) {

  if(stack[depth].tag == -1)
    stack[depth].tag = tag;

  char shortcut = *text;

  for(_item_t* p = gwin.cmenu->item; p; p = p->prev) {
    if(p->schar == shortcut)
      shortcut = NUL;
  }

  status = wmenuitem(stack[depth].itemrow, stack[depth].itemcolumn, text+1, shortcut, tag, fmask, select, hotkey, helpnumber);
  stack[depth].winwidth = strlen(text+1);
  if(stack[depth].type & M_HORZ)
    stack[depth].itemcolumn += stack[depth].winwidth;
  else
    stack[depth].itemrow++;
}


//  ------------------------------------------------------------------

void GMnu::ItemDesc(const char* text) {

  status = wmenuitxt(deschdl, descrow, desccolumn, desccolor, text);
}


//  ------------------------------------------------------------------

void GMnu::ItemSep()
{
  Item(-1, " ", M_SEPAR, NULL, 0);
}


//  ------------------------------------------------------------------

void GMnu::ItemFuncs(VfvCP before, VfvCP after) {

  status = wmenuiba(before, after);
}


//  ------------------------------------------------------------------

void GMnu::SetNextItem(int tag) {

  status = wmenuinext(tag);
}


//  ------------------------------------------------------------------

void GMnu::DisableItem(int tag) {

  status = wmenuidsab(tag);
}


//  ------------------------------------------------------------------

void GMnu::EnableItem(int tag) {

  status = wmenuienab(tag);
}


//  ------------------------------------------------------------------

GMnuItm* GMnu::FindItem(int tag) {

  return wmenuifind(tag);
}


//  ------------------------------------------------------------------
