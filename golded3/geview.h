
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
//  Message viewer classes.
//  ------------------------------------------------------------------

#include <gwindow.h>


//  ------------------------------------------------------------------

class GMsgHeaderView {

  int alive;

protected:

public:

  Area *area;
  GMsg *msg;
  gwindow window;

  GMsgHeaderView();
  ~GMsgHeaderView();

  // Window setup
  int at_row;
  int at_column;
  int width;
  int height;
  int border_type;
  int border_color;
  int window_color;
  int title_color;

  int highlight_color;
  int from_color;
  int to_color;
  int subject_color;

  void Use(Area *areaptr, GMsg *msgptr);

  void Create();
  void Destroy();

  void Paint();
};


//  ------------------------------------------------------------------

class GMsgBodyView : public Container {

  int alive;

protected:

  // View data
  int visible_width;
  int scrollbar_visible;
  Line dummy_line;
  int can_pagedown;
  int upperline;
  int lowerline;

  void PaintLine(int row, Line *line);
  void UpdateScrollbar();
  void prints(int wrow, int wcol, int atr, const char* str);

public:

  Area *area;
  GMsg *msg;
  gwindow window;

  GMsgBodyView();
  ~GMsgBodyView();

  // Window setup
  int at_row;
  int at_column;
  int width;
  int height;
  int border_type;
  int border_color;
  int window_color;
  int scrollbar_color;

  int highlight_color;

  void Use(Area *areaptr, GMsg *msgptr, int startline=0);

  int UpperLine() { return upperline; }

  void Create();
  void Destroy();

  void Paint();

  int Top(int redraw=false);
  int Bottom();
  int PageUp();
  int PageDown();
  int LineUp();
  int LineDown();
  int Continue();
  int ThumbTrack(int pos);
  int ThumbPosition(int pos);
};


//  ------------------------------------------------------------------

