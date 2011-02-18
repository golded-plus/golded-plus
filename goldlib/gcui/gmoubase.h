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
//  GCUI: Golded+ Character-oriented User Interface.
//  Mousing (declarations).
//  ------------------------------------------------------------------

#ifndef __gmoubase_h
#define __gmoubase_h


//  ------------------------------------------------------------------

#include <gdefs.h>


//  ------------------------------------------------------------------

#ifdef GOLD_MOUSE

//  ------------------------------------------------------------------
//  Mouse constants

const int GMOU_LEVEL_NONE = 0;   // No mouse support
const int GMOU_LEVEL_KEYS = 1;   // Mouse movement emulates arrow keys
const int GMOU_LEVEL_CURS = 2;   // Free-moving mouse cursor where supported
const int GMOU_LEVEL_FULL = 3;   // Full mouse support (MS_KEYS | MS_CURS)

const int GMOU_EVENT_MAX = 10;

const int GMOU_LEFT_BUTTON = 0x00;
const int GMOU_RIGHT_BUTTON = 0x01;

const int GMOU_LEFT_PRESSED = 0x01;
const int GMOU_RIGHT_PRESSED = 0x02;
const int GMOU_MIDDLE_PRESSED = 0x04;

const int GMOU_MOVED = 0x01;

const int GMOU_PRESSED_LEFT    = 0x02;
const int GMOU_RELEASED_LEFT   = 0x04;
const int GMOU_PRESSED_RIGHT   = 0x08;
const int GMOU_RELEASED_RIGHT  = 0x10;
const int GMOU_PRESSED_MIDDLE  = 0x20;
const int GMOU_RELEASED_MIDDLE = 0x40;

const int GMOU_EVENT_LEFT     = GMOU_PRESSED_LEFT   | GMOU_RELEASED_LEFT;
const int GMOU_EVENT_RIGHT    = GMOU_PRESSED_RIGHT  | GMOU_RELEASED_RIGHT;
const int GMOU_EVENT_MIDDLE   = GMOU_PRESSED_MIDDLE | GMOU_RELEASED_MIDDLE;


//  ------------------------------------------------------------------
//  MS MOUSE interrupt

const word MSMOUSE_INT = 0x33;


//  ------------------------------------------------------------------
//  MS MOUSE functions

const word MSMOUSE_RESET_DRIVER             = 0x00;
const word MSMOUSE_SHOW_CURSOR              = 0x01;
const word MSMOUSE_HIDE_CURSOR              = 0x02;
const word MSMOUSE_GET_BUTTONS_AND_POSITION = 0x03;
const word MSMOUSE_SET_CURSOR_POSITION      = 0x04;
const word MSMOUSE_GET_BUTTON_PRESS         = 0x05;
const word MSMOUSE_GET_BUTTON_RELEASE       = 0x06;
const word MSMOUSE_SET_HORIZONTAL_RANGE     = 0x07;
const word MSMOUSE_SET_VERTIAL_RANGE        = 0x08;
const word MSMOUSE_SET_GRAPHICS_CURSOR      = 0x09;
const word MSMOUSE_SET_TEXT_CURSOR          = 0x0A;


//  ------------------------------------------------------------------
//  Mouse class

class GMou {

  #if 0
  #ifdef __OS2__
  HMOU hmou;
  #endif
  #endif

public:

  int      detected;   // true if a mouse driver was detected
  int      level;      // Mouse support level (GMOU_LEVEL_xxxx)
  int      hidden;     // Depth of cursor hides

  struct {
    int    button;
    int    count;
    int    row;
    int    column;
  } hit;

  GMou();
  ~GMou();

  void SetLevel(int __level);

  void Reset();
  void Init();

  void ClearEvents();

  void GetStatus();
  void GetPress(int __button);
  void GetRelease(int __button);

  void SetCursor(int __curtype, int __smask, int __cmask);

  void SetPosition(int __row, int __col);

  void HideCursor();
  void ShowCursor();

  int Hidden() { return make_bool(hidden); }

  int Enabled() { return level > GMOU_LEVEL_NONE; }

  int FreeCursor() { return level & GMOU_LEVEL_CURS; }
  int KeysEmulate() { return level & GMOU_LEVEL_KEYS; }

  int Button() { return hit.button; }
  int LeftButton() { return hit.button & GMOU_LEFT_PRESSED; }
  int RightButton() { return hit.button & GMOU_RIGHT_PRESSED; }
  int MiddleButton() { return hit.button & GMOU_MIDDLE_PRESSED; }
  int Count() { return hit.count; }
  int Row() { return hit.row; }
  int Column() { return hit.column; }

  void GetLeftPress() { GetPress(GMOU_LEFT_BUTTON); }
  void GetRightPress() { GetPress(GMOU_RIGHT_BUTTON); }

  void GetLeftRelease() { GetRelease(GMOU_LEFT_BUTTON); }
  void GetRightRelease() { GetRelease(GMOU_RIGHT_BUTTON); }
};

extern GMou gmou;


//  ------------------------------------------------------------------

#endif // GOLD_MOUSE

//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------


