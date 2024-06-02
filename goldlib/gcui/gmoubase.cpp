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
//  Mousing: class GMou member functions implementation.
//  ------------------------------------------------------------------

#include <gmemall.h>
#include <gvidall.h>
#include <gmoubase.h>
//  ------------------------------------------------------------------

#ifdef GOLD_MOUSE
//  ------------------------------------------------------------------
GMou gmou;
//  ------------------------------------------------------------------
GMou::GMou()
{
    detected = false;
    Reset();
}

//  ------------------------------------------------------------------
GMou::~GMou()
{
    Reset();
}

//  ------------------------------------------------------------------
void GMou::SetLevel(int __level)
{
    if(detected)
    {
        level = __level;

        if(level < GMOU_LEVEL_NONE)
        {
            level = GMOU_LEVEL_NONE;
        }
        else if(level > GMOU_LEVEL_FULL)
        {
            level = GMOU_LEVEL_FULL;
        }
    }
    else
    {
        level = GMOU_LEVEL_NONE;
    }
}

//  ------------------------------------------------------------------
void GMou::Reset()
{
    level      = GMOU_LEVEL_NONE;
    hidden     = 1;
    hit.button = 0;
    hit.count  = 0;
    hit.row    = 0;
    hit.column = 0;

    if(detected)
    {
        Init();
    }
}

//  ------------------------------------------------------------------
//  ------------------------------------------------------------------
//  DOS MOUSING - BEGIN
//  ------------------------------------------------------------------
//  ------------------------------------------------------------------

#if defined (__MSDOS__)
//  ------------------------------------------------------------------
void GMou::Init()
{
    i86 cpu;
    cpu.ax(MSMOUSE_RESET_DRIVER);
    cpu.genint(MSMOUSE_INT);

    if(cpu.ax())
    {
        detected = true;
    }
}

//  ------------------------------------------------------------------
void GMou::ClearEvents()
{
    if(level)
    {
        i86 cpu;
        cpu.ax(MSMOUSE_GET_BUTTON_PRESS);
        cpu.bx(GMOU_LEFT_BUTTON);
        cpu.genint(MSMOUSE_INT);
        cpu.ax(MSMOUSE_GET_BUTTON_PRESS);
        cpu.bx(GMOU_RIGHT_BUTTON);
        cpu.genint(MSMOUSE_INT);
        cpu.ax(MSMOUSE_GET_BUTTON_RELEASE);
        cpu.bx(GMOU_LEFT_BUTTON);
        cpu.genint(MSMOUSE_INT);
        cpu.ax(MSMOUSE_GET_BUTTON_RELEASE);
        cpu.bx(GMOU_RIGHT_BUTTON);
        cpu.genint(MSMOUSE_INT);
    }
}

//  ------------------------------------------------------------------
void GMou::GetStatus()
{
    if(level)
    {
        i86 cpu;
        cpu.ax(MSMOUSE_GET_BUTTONS_AND_POSITION);
        cpu.genint(MSMOUSE_INT);
        hit.button = cpu.bx();
        hit.count  = 0;
        hit.column = cpu.cx() / gvid->curr.screen.cwidth;
        hit.row    = cpu.dx() / gvid->curr.screen.cheight;
    }
}

//  ------------------------------------------------------------------
void GMou::GetPress(int __button)
{
    if(level)
    {
        i86 cpu;
        cpu.ax(MSMOUSE_GET_BUTTON_PRESS);
        cpu.bx((word)__button);
        cpu.genint(MSMOUSE_INT);
        hit.button = cpu.ax();
        hit.count  = cpu.bx();
        hit.column = cpu.cx() / gvid->curr.screen.cwidth;
        hit.row    = cpu.dx() / gvid->curr.screen.cheight;
    }
}

//  ------------------------------------------------------------------
void GMou::GetRelease(int __button)
{
    if(level)
    {
        i86 cpu;
        cpu.ax(MSMOUSE_GET_BUTTON_RELEASE);
        cpu.bx((word)__button);
        cpu.genint(MSMOUSE_INT);
        hit.button = cpu.ax();
        hit.count  = cpu.bx();
        hit.column = cpu.cx() / gvid->curr.screen.cwidth;
        hit.row    = cpu.dx() / gvid->curr.screen.cheight;
    }
}

//  ------------------------------------------------------------------
void GMou::SetCursor(int __curtype, int __smask, int __cmask)
{
    if(level)
    {
        i86 cpu;
        cpu.ax(MSMOUSE_SET_TEXT_CURSOR);
        cpu.bx((word)__curtype);
        cpu.cx((word)__smask);
        cpu.dx((word)__cmask);
        cpu.genint(MSMOUSE_INT);
    }
}

//  ------------------------------------------------------------------
void GMou::SetPosition(int __row, int __col)
{
    if(level)
    {
        i86 cpu;
        cpu.ax(MSMOUSE_SET_CURSOR_POSITION);
        cpu.cx((word)(__col * gvid->curr.screen.cwidth));
        cpu.dx((word)(__row * gvid->curr.screen.cheight));
        cpu.genint(MSMOUSE_INT);
    }
}

//  ------------------------------------------------------------------
void GMou::HideCursor()
{
    if(level)
    {
        if(not hidden)
        {
            i86 cpu;
            cpu.ax(MSMOUSE_HIDE_CURSOR);
            cpu.genint(MSMOUSE_INT);
            hidden = true;
        }
    }
}

//  ------------------------------------------------------------------
void GMou::ShowCursor()
{
    if(level)
    {
        i86 cpu;
        cpu.ax(MSMOUSE_SHOW_CURSOR);
        cpu.genint(MSMOUSE_INT);
        hidden = false;
    }
}

//  ------------------------------------------------------------------
//  ------------------------------------------------------------------
//  DOS MOUSING - END
//  ------------------------------------------------------------------
//  ------------------------------------------------------------------

#endif // if defined (__MSDOS__)
//  ------------------------------------------------------------------
//  ------------------------------------------------------------------
//  OS/2 MOUSING - BEGIN
//  ------------------------------------------------------------------
//  ------------------------------------------------------------------

#if defined (__OS2__)
//  ------------------------------------------------------------------
void GMou::Init()
{
#if 0

    if(MouOpen(NULL, &hmou) == 0)
    {
        USHORT _mask = MOUSE_BN1_DOWN | MOUSE_BN2_DOWN | MOUSE_BN3_DOWN;
        MouSetEventMask(&_mask, hmou);
        detected = true;
    }

#endif
}

//  ------------------------------------------------------------------
void GMou::ClearEvents()
{
    if(level)
    {
        MouFlushQue(hmou);
    }
}

//  ------------------------------------------------------------------
void GMou::GetStatus()
{
    if(level)
    {
        PTRLOC _pos;
        MouGetPtrPos(&_pos, hmou);
        hit.button = 0;
        hit.count  = 0;
        hit.column = _pos.col;
        hit.row    = _pos.row;
        MOUEVENTINFO _event;
        USHORT _wait = MOU_NOWAIT;

        if(MouReadEventQue(&_event, &_wait, hmou) == 0)
        {
            if(_event.fs & (MOUSE_BN1_DOWN | MOUSE_MOTION_WITH_BN1_DOWN))
            {
                hit.button |= GMOU_LEFT_PRESSED;
            }

            if(_event.fs & (MOUSE_BN2_DOWN | MOUSE_MOTION_WITH_BN2_DOWN))
            {
                hit.button |= GMOU_RIGHT_PRESSED;
            }

            if(_event.fs & (MOUSE_BN3_DOWN | MOUSE_MOTION_WITH_BN3_DOWN))
            {
                hit.button |= GMOU_MIDDLE_PRESSED;
            }
        }
    }
} // GMou::GetStatus

//  ------------------------------------------------------------------
void GMou::GetPress(int __button)
{
    NW(__button);
#if 0

    if(level)
    {
        hit.button = 0;
        hit.count  = 0;
        hit.column = 0;
        hit.row    = 0;
        MOUQUEINFO _que;
        MouGetNumQueEl(&_que, hmou);
        hit.count = _que.cEvents;

        if(hit.count)
        {
            MOUEVENTINFO _event;
            USHORT _wait = MOU_WAIT;

            if(MouReadEventQue(&_event, &_wait, hmou) == 0)
            {
                if(_event.fs & (MOUSE_BN1_DOWN | MOUSE_MOTION_WITH_BN1_DOWN))
                {
                    hit.button |= GMOU_LEFT_PRESSED;
                }

                if(_event.fs & (MOUSE_BN2_DOWN | MOUSE_MOTION_WITH_BN2_DOWN))
                {
                    hit.button |= GMOU_RIGHT_PRESSED;
                }

                if(_event.fs & (MOUSE_BN3_DOWN | MOUSE_MOTION_WITH_BN3_DOWN))
                {
                    hit.button |= GMOU_MIDDLE_PRESSED;
                }

                hit.column = _event.col;
                hit.row    = _event.row;
            }
        }
    }

#endif // if 0
} // GMou::GetPress

//  ------------------------------------------------------------------
void GMou::GetRelease(int __button)
{
    NW(__button);
#if 0

    if(level)
    {
        hit.button = 0;
        hit.count  = 0;
        hit.column = 0;
        hit.row    = 0;
        MOUQUEINFO _que;
        MouGetNumQueEl(&_que, hmou);
        hit.count = _que.cEvents;

        if(hit.count)
        {
            MOUEVENTINFO _event;
            USHORT _wait = MOU_WAIT;

            if(MouReadEventQue(&_event, &_wait, hmou) == 0)
            {
                if(_event.fs & (MOUSE_BN1_DOWN | MOUSE_MOTION_WITH_BN1_DOWN))
                {
                    hit.button |= GMOU_LEFT_PRESSED;
                }

                if(_event.fs & (MOUSE_BN2_DOWN | MOUSE_MOTION_WITH_BN2_DOWN))
                {
                    hit.button |= GMOU_RIGHT_PRESSED;
                }

                if(_event.fs & (MOUSE_BN3_DOWN | MOUSE_MOTION_WITH_BN3_DOWN))
                {
                    hit.button |= GMOU_MIDDLE_PRESSED;
                }

                hit.column = _event.col;
                hit.row    = _event.row;
            }
        }
    }

#endif // if 0
} // GMou::GetRelease

//  ------------------------------------------------------------------
void GMou::SetCursor(int __curtype, int __smask, int __cmask)
{
    if(level)
    {
        // Not implemented yet
        NW(__curtype);
        NW(__smask);
        NW(__cmask);
    }
}

//  ------------------------------------------------------------------
void GMou::SetPosition(int __row, int __col)
{
    if(level)
    {
        PTRLOC _pos;
        _pos.row = (USHORT)__row;
        _pos.col = (USHORT)__col;
        MouSetPtrPos(&_pos, hmou);
    }
}

//  ------------------------------------------------------------------
void GMou::HideCursor()
{
    if(level)
    {
        if(not hidden)
        {
            NOPTRRECT _rect;
            _rect.row  = 0;
            _rect.col  = 0;
            _rect.cRow = (USHORT)(gvid->curr.screen.rows - 1);
            _rect.cCol = (USHORT)(gvid->curr.screen.columns - 1);
            MouRemovePtr(&_rect, hmou);
            hidden = true;
        }
    }
}

//  ------------------------------------------------------------------
void GMou::ShowCursor()
{
    if(level)
    {
        MouDrawPtr(hmou);
        hidden = false;
    }
}

//  ------------------------------------------------------------------
//  ------------------------------------------------------------------
//  OS/2 MOUSING - END
//  ------------------------------------------------------------------
//  ------------------------------------------------------------------

#endif // if defined (__OS2__)
//  ------------------------------------------------------------------
//  Dummy definitions when mouse support is not defined
void GMou::Init()
{
}

void GMou::ClearEvents()
{
}

void GMou::GetStatus()
{
}

void GMou::GetPress(int)
{
}

void GMou::GetRelease(int)
{
}

void GMou::SetCursor(int, int, int)
{
}

void GMou::SetPosition(int, int)
{
}

void GMou::HideCursor()
{
}

void GMou::ShowCursor()
{
}

//  ------------------------------------------------------------------

#endif // GOLD_MOUSE
//  ------------------------------------------------------------------
