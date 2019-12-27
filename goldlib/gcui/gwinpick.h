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
//  Pick engine (declarations).
//  ------------------------------------------------------------------

#ifndef __gwinpick_h
#define __gwinpick_h


//  ------------------------------------------------------------------

#include <gkbdbase.h>


//  ------------------------------------------------------------------
//  Constants

const int PICK_DISP = 0;
const int PICK_BAR  = 1;

const int LIST_BOTTOM      = -2;
const int LIST_NEARBOTTOM  = -1;
const int LIST_MIDDLE      = 0;
const int LIST_NEARTOP     = 1;
const int LIST_TOP         = 2;


//  ------------------------------------------------------------------

class gwinpick
{

private:

protected:

    gkey key;
    bool keyok;
    uint new_hoffset;
    uint h_offset;
    bool replylinkfloat;  // in derived classes set this with CFG->replylinkfloat
    // before call display_page() and run_picker()

    void cursor_up();
    void cursor_down();
    void cursor_pageup();
    void cursor_pagedown();
    void cursor_first();
    void cursor_last();
    void cursor_scroll_up();
    void cursor_scroll_down();

    void update_cursor();

    void display_line(bool bar=false);
    void display_page();
    bool default_handle_key();

    inline void update()
    {
        display_page();
    };

public:

    uint  ypos;                                 // Window Starting Row
    uint  xpos;                                 // Window Starting Column
    uint  ylen;                                 // Window Height
    uint  xlen;                                 // Window Width
    int   btype;                                // Window Border Type
    vattr battr;                                // Window Border Color
    vattr wattr;                                // Window Color
    vattr tattr;                                // Window Title Color
    vattr sattr;                                // Window Selection Bar Color
    vattr hattr;                                // Window Highlight Color
    vattr loattr;                               // Window LoAttr Color
    vattr sbattr;                               // Window Scrollbar Color
    const char* title;                          // Window Title
    int   helpcat;                              // Window Help Category
    uint  maximum_index;                        // List Entries - 1
    uint  minimum_index;                        // Minimum index in the list
    uint  maximum_position;                     // Display Pos
    uint  index;                                // List Index
    uint  position;                             // Display Pos
    bool  aborted;                              // True if aborted
    bool  listwrap;                             // True if wrap-around is supported
    int   direction;                            // 1 if next, -1 if previous

    virtual void open();                        // Called after window is opened
    virtual void close();                       // Called after window is closed
    virtual void precursor();                   // Called before any cursor movement
    virtual void do_delayed();                  // Called after a delay
    virtual void print_line(uint idx, uint pos, bool isbar) = 0;
    virtual void scroll(int where);             // Scroll page
    virtual bool handle_key();                  // Handles keypress
    virtual bool is_selectable(uint idx);       // returns true if selectable

    void display_bar()
    {
        display_line(true);
    }
    void center(int listmode);

    int run_picker();

    gwinpick();
    virtual ~gwinpick() { };
};


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
