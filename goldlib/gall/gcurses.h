//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1989 Free Software Foundation
//  Copyright (C) 2000 Alexander S. Aganichev
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
//  This file is derived from the GNU C++ Library.
//  Written by Eric Newton (newton@rocky.oswego.edu)
//  ------------------------------------------------------------------

#ifndef __gcurses_h
#define __gcurses_h

//  ------------------------------------------------------------------

// Even many system which mostly have C++-ready header files,
// do not have C++-ready curses.h.
extern "C" {
#ifdef __FreeBSD__
#include <ncurses/ncurses.h>
#else
#include <curses.h>
#endif
}


//  ------------------------------------------------------------------

// "Convert" macros to inlines, if needed.
#ifdef addch
inline int (addch)(char ch)  { return addch(ch); }
#undef addch
#endif
#ifdef addstr
// The (char*) cast is to hack around missing const's
inline int (addstr)(const char * str)  { return addstr((char*)str); }
#undef addstr
#endif
#ifdef clear
inline int (clear)()  { return clear(); }
#undef clear
#endif
#ifdef clearok
inline int (clearok)(WINDOW* win, bool bf)  { return clearok(win, bf); }
#undef clearok
#else
extern "C" int clearok(WINDOW*, bool);
#endif
#ifdef clrtobot
inline int (clrtobot)()  { return clrtobot(); }
#undef clrtobot
#endif
#ifdef clrtoeol
inline int (clrtoeol)()  { return clrtoeol(); }
#undef clrtoeol
#endif
#ifdef delch
inline int (delch)()  { return delch(); }
#undef delch
#endif
#ifdef deleteln
inline int (deleteln)()  { return deleteln(); }
#undef deleteln
#endif
#ifdef erase
inline int (erase)()  { return erase(); }
#undef erase
#endif
#ifdef flushok
inline int (flushok)(WINDOW* _win, int _bf)  { return flushok(_win, _bf); }
#undef flushok
#else
#define _no_flushok
#endif
#ifdef getch
inline int (getch)()  { return getch(); }
#undef getch
#endif
#ifdef getstr
inline int (getstr)(char *_str)  { return getstr(_str); }
#undef getstr
#endif
#ifdef getyx
inline void (getyx)(WINDOW* win, int& y, int& x) { getyx(win, y, x); }
#undef getyx
#endif
#ifdef inch
inline chtype (inch)()  { return inch(); }
#undef inch
#endif
#ifdef insch
inline int (insch)(char c)  { return insch(c); }
#undef insch
#endif
#ifdef insertln
inline int (insertln)()  { return insertln(); }
#undef insertln
#endif
#ifdef leaveok
inline int (leaveok)(WINDOW* win, bool bf)  { return leaveok(win, bf); }
#undef leaveok
#else
extern "C" int leaveok(WINDOW* win, bool bf);
#endif
#ifdef move
inline int (move)(int x, int y)  { return move(x, y); }
#undef move
#endif
#ifdef refresh
inline int (rfresh)()  { return refresh(); }
#undef refresh
#endif
#ifdef scrollok
inline int (scrollok)(WINDOW* win, bool bf)  { return scrollok(win, bf); }
#undef scrollok
#else
#ifndef hpux
extern "C" int scrollok(WINDOW*, bool);
#else
extern "C" int scrollok(WINDOW*, char);
#endif
#endif
#ifdef standend
inline int (standend)()  { return standend(); }
#undef standend
#endif
#ifdef standout
inline int (standout)()  { return standout(); }
#undef standout
#endif
#ifdef wstandend
inline int (wstandend)(WINDOW *win)  { return wstandend(win); }
#undef wstandend
#endif
#ifdef wstandout
inline int (wstandout)(WINDOW *win)  { return wstandout(win); }
#undef wstandout
#endif
#ifdef winch
inline chtype (winch)(WINDOW* win) { return winch(win); }
#undef winch
#endif

// deal with conflicting macros in ncurses.h  which is SYSV based
#ifdef box
inline int _G_box(WINDOW* win, chtype v, chtype h) {return box(win, v, h); }
#undef box
inline int box(WINDOW* win, chtype v, chtype h) {return _G_box(win, v, h); }
#endif
#ifdef scroll
inline int (scroll)(WINDOW* win) { return scroll(win); }
#undef scroll
#endif
#ifdef touchwin
inline int (touchwin)(WINDOW* win) { return touchwin(win); }
#undef touchwin
#endif

#ifdef mvwaddch
inline int (mvwaddch)(WINDOW *win, int y, int x, char ch)
{ return mvwaddch(win, y, x, ch); }
#undef mvwaddch
#endif
#ifdef mvwaddstr
inline int (mvwaddstr)(WINDOW *win, int y, int x, const char * str)
{ return mvwaddstr(win, y, x, (char*)str); }
#undef mvwaddstr
#endif
#ifdef mvwdelch
inline int (mvwdelch)(WINDOW *win, int y, int x) { return mvwdelch(win, y, x);}
#undef mvwdelch
#endif
#ifdef mvwgetch
inline int (mvwgetch)(WINDOW *win, int y, int x) { return mvwgetch(win, y, x);}
#undef mvwgetch
#endif
#ifdef mvwgetstr
inline int (mvwgetstr)(WINDOW *win, int y, int x, char *str)
{return mvwgetstr(win,y,x, str);}
#undef mvwgetstr
#endif
#ifdef mvwinch
inline chtype (mvwinch)(WINDOW *win, int y, int x) { return mvwinch(win, y, x);}
#undef mvwinch
#endif
#ifdef mvwinsch
inline int (mvwinsch)(WINDOW *win, int y, int x, char c)
{ return mvwinsch(win, y, x, c); }
#undef mvwinsch
#endif

#ifdef mvaddch
inline int (mvaddch)(int y, int x, char ch)
{ return mvaddch(y, x, ch); }
#undef mvaddch
#endif
#ifdef mvaddstr
inline int (mvaddstr)(int y, int x, const char * str)
{ return mvaddstr(y, x, (char*)str); }
#undef mvaddstr
#endif
#ifdef mvdelch
inline int (mvdelch)(int y, int x) { return mvdelch(y, x);}
#undef mvdelch
#endif
#ifdef mvgetch
inline int (mvgetch)(int y, int x) { return mvgetch(y, x);}
#undef mvgetch
#endif
#ifdef mvgetstr
inline int (mvgetstr)(int y, int x, char *str) {return mvgetstr(y, x, str);}
#undef mvgetstr
#endif
#ifdef mvinch
inline chtype (mvinch)(int y, int x) { return mvinch(y, x);}
#undef mvinch
#endif
#ifdef mvinsch
inline int (mvinsch)(int y, int x, char c)
{ return mvinsch(y, x, c); }
#undef mvinsch
#endif

extern int curses_initialized;

//  ------------------------------------------------------------------

#endif // __gcurses_h

//  ------------------------------------------------------------------
