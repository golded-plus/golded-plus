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
//  Keyboard functions.
//  ------------------------------------------------------------------

#ifndef __gkbdbase_h
#define __gkbdbase_h


//  ------------------------------------------------------------------

#include <gtimall.h>


//  ------------------------------------------------------------------
//  Simple types

typedef word gkey;


//  ------------------------------------------------------------------
//  Keycode object

inline gkey& KCodKey(gkey &key) { return key; }
inline byte& KCodAsc(gkey &key) { return *(((byte *)&key)+0); }
inline byte& KCodScn(gkey &key) { return *(((byte *)&key)+1); }


//  ------------------------------------------------------------------
//  Definition of kbuf record

struct KBuf {
  KBuf* prev;         // previous record
  KBuf* next;         // next record
  gkey   xch;         // keypress
};


//  ------------------------------------------------------------------
//  Definition of onkey record

struct KBnd {
  KBnd* prev;         // pointer to previous record
  KBnd* next;         // pointer to next record
  gkey  keycode;      // Scan/ASCII code of trap key
  VfvCP func;         // address of onkey function
  gkey  pass;         // key to pass back, 0=don't pass
};


//  ------------------------------------------------------------------
//  Definition of keyboard info record

class GKbd {

public:

  KBuf*  kbuf;           // Pointer to head record in key buffer
  KBnd*  onkey;          // Pointer to head record in onkey list
  KBnd*  curronkey;      // Pointer to current onkey record
  int    inmenu;         // In-menu flag used by menuing functions
  int    source;         // Source of keypress 0=kb, 1=kbuf, 2=mouse
  int    extkbd;         // Extended keyboard 0=none, 1=yes
  int    polling;        // Keyboard polling enabled
  Clock  tickinterval;   // Minimum interval between ticks
  Clock  tickvalue;      // Value from last tick
  VfvCP  tickfunc;       // Function to call when a tick is generated
  Clock  tickpress;      // Tick value at last keypress
  bool   inidle;         // In-idle flag used by tickfunc
  bool   quitall;        // Quit-all flag for menus etc.

  void Init();
  GKbd();
  ~GKbd();
};

extern GKbd gkbd;


//  ------------------------------------------------------------------
//  Keyboard status codes returned from kbstat()

#define RSHIFT      1       // right shift pressed
#define LSHIFT      2       // left shift pressed
#define GCTRL       4       // [Ctrl] pressed
#define ALT         8       // [Alt] pressed
#define SCRLOCK     16      // [Scroll Lock] toggled
#define NUMLOCK     32      // [Num Lock] toggled
#define CAPSLOCK    64      // [Caps Lock] toggled
#define INS         128     // [Ins] toggled


//  ------------------------------------------------------------------

extern gkey scancode_table[];
extern bool right_alt_same_as_left;


//  ------------------------------------------------------------------
//  Function prototypes

KBnd* chgonkey  (KBnd* kblist);
void  clearkeys ();
void  freonkey  ();
int   setonkey  (gkey keycode, VfvCP func, gkey pass);
gkey  getxch    (int __tick=false);
void  kbclear   ();
gkey  kbmhit    ();
gkey  kbxget    (int mode=0);
gkey  kbxhit    ();
int   kbput     (gkey xch);
word  kbput_    (gkey xch);
void  kbputs_   (char* str);
byte  scancode  (gkey ch);
gkey  waitkey   ();
gkey  waitkeyt  (int duration);

gkey  key_tolower(gkey __keycode);

gkey  keyscanxlat(gkey k);

gkey  __kbxget(int __mode=0, long __ticks=0, VfvCP __idlefunc=NULL);

void gkbdtickpressreset();
void gkbdtickvaluereset();


//  ------------------------------------------------------------------
//  Inline functions

inline gkey getxchtick()  { return getxch(true); }
inline void kbdsettickfunc(VfvCP func)  { gkbd.tickfunc = func; }


//  ------------------------------------------------------------------
//  Shorthand definitions of keyboard scancodes

#define  KEY_BRK  0xFFFF     // ^Break return from _KeyHit()/_KeyGet()


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
