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

#ifndef __gmnubase_h
#define __gmnubase_h


//  ------------------------------------------------------------------

#include <gwinall.h>


//  ------------------------------------------------------------------
//  Menu items

typedef struct _item_t GMnuItm;


//  ------------------------------------------------------------------
//  Menu class

class GMnu {

protected:

  int   bordertype;
  vattr bordercolor;

  vattr textcolor;
  vattr quickcolor;
  vattr noselcolor;
  vattr barcolor;
  vattr shadowcolor;

  const char*  title;
  int   titlepos;
  vattr titlecolor;

  int   deschdl;
  int   descrow;
  int   desccolumn;
  vattr desccolor;

  int  helpnumber;

  int  beginrow;
  int  begincolumn;
  int  beginwidth;
  int  beginheight;

  VfvCP menuopen;
  int  itemmask;

  int  escape;
  int  overtag;
  int  finaltag;
  int  status;

  int  depth;
  struct {
    int  tag;
    int  type;
    int  winrow;
    int  wincolumn;
    int  winwidth;
    int  winheight;
    int  itemrow;
    int  itemcolumn;
  } stack[10];

public:

  void Init();

  void SetBorder(int type, vattr color);
  void SetColor(vattr text, vattr quick, vattr nosel, vattr bar, vattr shadow = DEFATTR);
  void SetTitle(const char* title, vattr color, int pos=TCENTER);
  void SetTitle(const char* title);
  void SetDesc(int hdl, int row, int col, vattr color);
  void SetPos(int row, int col, int width=0, int height=0);
  void SetEsc(int option);
  void SetHelp(int help);
  void SetMask(int mask);
  void SetTag(int tag);

  void Begin(int type=M_VERT);
  void BeginPullDown(int type=0) { Begin(M_VERT|M_PD|type); }
  void End();
  void Start();

  void Item(int tag, const char* text);
  void Item(int tag, const char* text, int fmask);
  void Item(int tag, const char* text, VfvCP select, int fmask=M_CLOSE);
  void Item(int tag, const char* text, int fmask, VfvCP select, gkey hotkey=0);
  void ItemDesc(const char* text);
  void ItemSep();
  void ItemFuncs(VfvCP before, VfvCP after);

  void SetNextItem(int tag);
  void DisableItem(int tag);
  void EnableItem(int tag);

  GMnuItm* FindItem(int tag);

  int FinalTag() { return finaltag; }
};


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
