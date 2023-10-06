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
//  Windowing kernel.
//  Based on CXL by Mike Smedley.
//  ------------------------------------------------------------------

#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <gmemdbg.h>
#include <gutlmisc.h>
#include <gwinall.h>
#include <gkbdcode.h>


//  ------------------------------------------------------------------

#define GOLD_INLINE inline
#define GOLD_WCHK


//  ------------------------------------------------------------------
//  Global window data

GWin gwin;


//  ------------------------------------------------------------------
//  Window Class constructor

GWin::GWin()
{

    active     = NULL;          // pointer to active window
    hidden     = NULL;          // pointer to last hidden window
    menu       = NULL;          // pointer to head menu record
    cmenu      = NULL;          // pointer to current menu record
    helptr     = NULL;          // pointer to help info record
    handle     = 0;             // last handle given to a window
    help       = 0;             // pointer to current help category
    werrno     = W_NOERROR;     // error num from last window func
    total      = 0;             // total number of open windows
    mlevel     = 0;             // system variable used in menus
    ilevel     = 0;             // system variable used in menus
    esc        = true;          // check for Esc in input funcions?
    tabwidth   = 8;             // window TTY output tab width
    fillch     = ' ';           // character to fill windows with
    style      = STYLE_NORMAL;  // window opening style
}


//  ------------------------------------------------------------------
//  Window Class destructor

GWin::~GWin()
{

    // No action defined yet
}


//  ------------------------------------------------------------------
//  Displays a string in centered in active window

int wcenters(int wrow, vattr attr, const char* str)
{

    int window_width, string_length;
    int start_column, border;

    // check for active window

    if(!gwin.total)
        return gwin.werrno=W_NOACTIVE;

    // check for valid row

    if(wchkcoord(wrow,0))
        return gwin.werrno=W_INVCOORD;

    // check for window border

    border=gwin.active->border;

    // calculate start column & window width

    start_column = gwin.active->scol+border;
    window_width = (gwin.active->ecol-border)-start_column+1;

    // check length of input string

    string_length=strlen(str);
    if(string_length>window_width)
        return gwin.werrno=W_STRLONG;

    // display the string

    vputs(
        gwin.active->srow+wrow+border,
        ((window_width/2)+start_column)-(string_length/2),
        attr,
        str
    );

    // return normally

    return gwin.werrno=W_NOERROR;
}


//  ------------------------------------------------------------------
//  Smoothly drag a window 1 position in given direction

int wdrag(int direction)
{

    int   srow, scol, erow, ecol, fill_row, fill_col, i;
    int   nsrow, nscol, nerow, necol;
    vattr shad_attr = DEFATTR;
    int   chars_per_line, lines_per_win;
    int   vert_movement, horz_movement;
    vsavebuf* win_image;
    vsavebuf* wp;
    vatch* p;
    vatch* src;
    vatch* dest;

    // check for active window
    if(!gwin.total)
        return(gwin.werrno=W_NOACTIVE);

    // get window coordinates
    srow = gwin.active->srow;
    scol = gwin.active->scol;
    erow = gwin.active->erow;
    ecol = gwin.active->ecol;

    // calculate lines-per-window and characters-per-line
    lines_per_win  = erow - srow + 1;
    chars_per_line = ecol - scol + 1;

    // determine facts about direction of move
    vert_movement=horz_movement=0;
    switch(direction)
    {
    case D_DOWN:
        vert_movement=1;
        lines_per_win--;
        break;
    case D_UP:
        vert_movement=-1;
        lines_per_win--;
        break;
    case D_LEFT:
        horz_movement=-1;
        chars_per_line--;
        break;
    case D_RIGHT:
    default:
        horz_movement=1;
        chars_per_line--;
    }

    // calculate new window coordinates
    nsrow = srow + vert_movement;
    nscol = scol + horz_movement;
    nerow = erow + vert_movement;
    necol = ecol + horz_movement;

    // if window has shadow, close it before the move
    if(gwin.active->wsbuf!=NULL)
    {
        shad_attr = gwin.active->wsattr;
        wshadoff();
    }

    // save the current window image
    win_image = vsave(srow,scol,erow,ecol);
    if(win_image==NULL)
        return(gwin.werrno=W_ALLOCERR);

    // save the area where the window will relocate to
    wp = vsave(nsrow,nscol,nerow,necol);
    if(wp==NULL)
    {
        throw_xfree(win_image);
        return(gwin.werrno=W_ALLOCERR);
    }

    // restore window to new coordinates
    vrestore(win_image, nsrow, nscol, nerow, necol);
    throw_xfree(win_image);

    // start buffer positions past coordinates
    src  = gwin.active->wbuf->data;
    dest = wp->data;

    if(direction==D_DOWN)
        src  += chars_per_line;
    if(direction==D_UP)
        dest += chars_per_line;

    // do the transfer of buffer contents
    for(i=0; i < lines_per_win; i++)
    {

        if(direction==D_LEFT)
            dest++;
        if(direction==D_RIGHT)
            src++;

        // move 1 line
        memmove(dest, src, chars_per_line*sizeof(vatch));
        src  += chars_per_line;
        dest += chars_per_line;

        if(direction==D_LEFT)
            src++;
        if(direction==D_RIGHT)
            dest++;
    }

    // erase the trail that was left-over
    p = gwin.active->wbuf->data;
    if(vert_movement)
    {
        if(direction==D_DOWN)
            fill_row=srow;
        else
        {
            p += (lines_per_win * chars_per_line);
            fill_row=erow;
        }

        vputx(fill_row, scol, vgattr(*p), vgchar(*p), ecol-scol+1);
    }
    else
    {
        if(direction==D_RIGHT)
            fill_col=scol;
        else
        {
            p += chars_per_line;
            fill_col=ecol;
        }

        for(i=srow; i<=erow; i++,p+=chars_per_line+1)
            vputc(i, fill_col, vgattr(*p), vgchar(*p));
    }

    // free old window buffer
    throw_xfree(gwin.active->wbuf);

    // update window record
    gwin.active->wbuf   = wp;
    gwin.active->row    = gwin.active->row    - gwin.active->srow + nsrow;
    gwin.active->column = gwin.active->column - gwin.active->scol + nscol;
    gwin.active->srow   = nsrow;
    gwin.active->scol   = nscol;
    gwin.active->erow   = nerow;
    gwin.active->ecol   = necol;

    // if window has shadow, redraw it
    if(shad_attr != DEFATTR)
        wshadow(shad_attr);

    // reset cursor position
    vposset(gwin.active->row,gwin.active->column);

    // return normally
    return(gwin.werrno=W_NOERROR);
}


//  ------------------------------------------------------------------
//  Slides active window to a new location

int wslide(int nsrow, int nscol)
{

    vattr shattr = DEFATTR;
    int   err = 0;

    // check for active windows
    if(!gwin.total)
        return gwin.werrno=W_NOACTIVE;

    // check for valid coordinates
    if(nsrow<0 or nscol<0)
        return gwin.werrno=W_INVCOORD;

    // if window has shadow, close it before the move
    if(gwin.active->wsbuf!=NULL)
    {
        shattr = gwin.active->wsattr;
        wshadoff();
    }

    // slide it on over
    while(gwin.active->scol>nscol and !err) err = wdrag(D_LEFT);
    while(gwin.active->scol<nscol and !err) err = wdrag(D_RIGHT);
    while(gwin.active->srow>nsrow and !err) err = wdrag(D_UP);
    while(gwin.active->srow<nsrow and !err) err = wdrag(D_DOWN);

    // test for error
    if(gwin.werrno)
        return gwin.werrno;

    // if window has shadow, redraw it
    if(shattr != DEFATTR)
        wshadow(shattr);

    // return normally
    return gwin.werrno=W_NOERROR;
}


//  ------------------------------------------------------------------
