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
//  Unix keyboard functions. Based on SLang source code.
//  ------------------------------------------------------------------

#ifndef __gkbdunix_h
    #define __gkbdunix_h


    //  ------------------------------------------------------------------

    #include <gdefs.h>


    //  ------------------------------------------------------------------

    const int GKBD_MAX_INPUT_BUFFER_LEN = 1024;


    //  ------------------------------------------------------------------

    extern int gkbd_stdin;


    //  ------------------------------------------------------------------

    int  gkbd_tty_init();
    void gkbd_tty_reset();
    int  gkbd_sys_input_pending(int tsecs=0);
    uint gkbd_sys_getkey();


    //  ------------------------------------------------------------------

    extern uint gkbd_last_key_char;

    uint gkbd_getkey();
    void gkbd_ungetkey_string(char *s, uint n);
    int  gkbd_input_pending(int tsecs=0);
    void gkbd_flush_input();
    void gkbd_keymap_init();
    void gkbd_keymap_reset();
    int  gkbd_define_keysym(char* s, uint keysym);
    uint gkbd_getmappedkey();


    //  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
