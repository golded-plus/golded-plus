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
//  Keyboard functions.
//  ------------------------------------------------------------------

#include <gkbdcode.h>
#include <gkbdbase.h>
#include <gmemdbg.h>
#include <gwinall.h>
#include <gutlmtsk.h>
#ifdef GOLD_MOUSE
#include <gmoubase.h>
#endif


//  ------------------------------------------------------------------
//  Event sources

const int GEVT_KEYBOARD = 0;
const int GEVT_BUFFER   = 1;
const int GEVT_MOUSE    = 2;
const int GEVT_TICK     = 3;


//  ------------------------------------------------------------------

void gkbdtickpressreset() {

  gkbd.tickpress = gclock();
}


//  ------------------------------------------------------------------

void gkbdtickvaluereset() {

  gkbd.tickvalue = gclock();
}


//  ------------------------------------------------------------------

gkey kbmhit() {

  gkey k;

  // Check for keypress in internal buffer or keyboard
  if(gkbd.kbuf) {
    gkbd.source = GEVT_BUFFER;
    k = gkbd.kbuf->xch;
  }
  else {
    gkbd.source = GEVT_KEYBOARD;
    k = kbxhit();
  }

  return k;
}


//  ------------------------------------------------------------------

static void kbd_call_func(VfvCP func) {

  int row, col;
  bool hidden = vcurhidden();
  vposget(&row,&col);
  _menu_t* menu = gwin.cmenu;
  (*func)();
  gwin.cmenu = menu;
  vposset(row,col);
  if(hidden)
    vcurhide();
  else
    vcurshow();
}


//  ------------------------------------------------------------------

static _item_t* find_hotkey(_menu_t* wmenu, gkey xch) {

  _item_t* witem;
  _item_t* item;

  // do while more items in this menu
  for(witem=wmenu->item; witem!=NULL; witem=witem->prev) {

    // if hot key matches keypress, return its item address
    if(witem->hotkey==xch and (!(witem->fmask&M_NOSEL)) and witem->select!=NULL)
      return witem;

    // if current item has a child menu, process it
    if(witem->child!=NULL) {
      item = find_hotkey((_menu_t*)witem->child, xch);
      if(item!=NULL)
        return item;
    }
  }

  // return address of item found
  return witem;
}


//  ------------------------------------------------------------------

static void makeextkey(gkey xshift, gkey& xkey) {

  switch(xkey) {
    case Key_Home:
    case Key_C_Home:
      if(xshift & ALT)
        xkey = Key_A_HomeG;
      if(xshift & (LSHIFT | RSHIFT))
        KCodScn(xkey) |= 0x80;
      break;

    case Key_End:
    case Key_C_End:
      if(xshift & ALT)
        xkey = Key_A_EndG;
      if(xshift & (LSHIFT | RSHIFT))
        KCodScn(xkey) |= 0x80;
      break;

    case Key_Up:
      if(xshift & ALT)
        xkey = Key_A_UpG;
      else if(xshift & GCTRL)
        xkey = Key_C_Up;
      if(xshift & (LSHIFT | RSHIFT))
        KCodScn(xkey) |= 0x80;
      break;

    case Key_Dwn:
      if(xshift & ALT)
        xkey = Key_A_DwnG;
      else if(xshift & GCTRL)
        xkey = Key_C_Dwn;
      if(xshift & (LSHIFT | RSHIFT))
        KCodScn(xkey) |= 0x80;
      break;

    case Key_Lft:
      if(xshift & ALT)
        xkey = Key_A_LftG;
      if(xshift & (LSHIFT | RSHIFT))
        KCodScn(xkey) |= 0x80;
      break;

    case Key_Rgt:
      if(xshift & ALT)
        xkey = Key_A_RgtG;
      if(xshift & (LSHIFT | RSHIFT))
        KCodScn(xkey) |= 0x80;
      break;

    case Key_PgUp:
      if(xshift & ALT)
        xkey = Key_A_PgUpG;
      if(xshift & (LSHIFT | RSHIFT))
        KCodScn(xkey) |= 0x80;
      break;

    case Key_PgDn:
      if(xshift & ALT)
        xkey = Key_A_PgDnG;
      if(xshift & (LSHIFT | RSHIFT))
        KCodScn(xkey) |= 0x80;
      break;

    case Key_Ins:
      if(xshift & ALT)
        xkey = Key_A_InsG;
      else if(xshift & GCTRL)
        xkey = Key_C_Ins;
      if(xshift & (LSHIFT | RSHIFT))
        KCodScn(xkey) |= 0x80;
      break;

    case Key_Del:
      if(xshift & ALT)
        xkey = Key_A_DelG;
      else if(xshift & GCTRL)
        xkey = Key_C_Del;
      if(xshift & (LSHIFT | RSHIFT))
        KCodScn(xkey) |= 0x80;
      break;
  }
}


//  ------------------------------------------------------------------

extern int blanked;

gkey getxch(int __tick) {

  gkey k;

  while(1) {

    // Keyboard polling loop
    if(gkbd.polling) {
      while(not kbmhit()) {
        Clock thistick = gclock();
        long tickdiff = thistick - gkbd.tickvalue;
        if(tickdiff < 0) {
          gkbd.tickvalue = thistick;
          tickdiff = gkbd.tickinterval + 1;
        }
        if(tickdiff > gkbd.tickinterval) {
          gkbd.tickvalue = thistick;
          if(gkbd.tickfunc) {
            gkbd.inidle = true;
            (*gkbd.tickfunc)();
            gkbd.inidle = false;
          }
          if(__tick)
            kbput(Key_Tick);
        }
        if(gmtsk.detected)
          gmtsk.timeslice();
      }
    }

    // Get key from internal buffer or keyboard
    if(gkbd.kbuf) {
      gkbd.source = GEVT_BUFFER;
      k = gkbd.kbuf->xch;
      KBuf* _kbuf = gkbd.kbuf->next;
      throw_free(gkbd.kbuf);
      gkbd.kbuf = _kbuf;
      if(gkbd.kbuf)
        gkbd.kbuf->prev = NULL;
    }
    else {
      gkbd.source = GEVT_KEYBOARD;
      k = kbxget();
      gkey s = kbxget(2);   // Read shift status
      if(not gkbd.extkbd) {
        if(s & (LSHIFT|RSHIFT|GCTRL|ALT))
          makeextkey(s,k);
      }
    }

    // Note time of keypress unless it's a tick
    if(k != Key_Tick)
      gkbdtickpressreset();

    // Search through onkey linked list for a
    // matching defined onkey. If one is found,
    // then save the current environment, call the
    // onkey's function, and restore the environment.

    if(not blanked) {
      KBnd* _onkey = gkbd.onkey;
      while(_onkey) {
        if(_onkey->keycode == k) {
          gkbd.curronkey = _onkey;
          kbd_call_func(_onkey->func);
          #ifdef GOLD_MOUSE
          if(gkbd.inmenu and gmou.FreeCursor())
            return 0;
          #endif
          break;
        }
        _onkey = _onkey->prev;
      }
      if(_onkey) {
        if(not _onkey->pass or (_onkey->pass >= 0xFE00))
          k = 0;
        else
          k = _onkey->pass;
      }
      else {

        // Search for a menu hot key. If one is found,
        // then save the current environment, call the
        // hotkey's function, and restore the environment.

        if(gwin.menu) {
          _item_t* item = find_hotkey(gwin.menu,k);
          if(item) {
            gwin.menu->hotkey = true;
            kbd_call_func(item->select);
            #ifdef GOLD_MOUSE
            if(gkbd.inmenu and gmou.FreeCursor())
              return 0;
            #endif
            k = 0;
          }
        }
      }
    }

    // If we still have a keycode, exit the loop
    if(k)
      break;
  }

  // Return keycode
  return k;
}


//  ------------------------------------------------------------------
