//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999 Alexander S. Aganichev
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
//  Multitasking.
//  ------------------------------------------------------------------

#include <cstdlib>
#include <gmemall.h>
#include <gutlmtsk.h>

#ifdef __OS2__
#define INCL_BASE
#include <os2.h>
#endif

#ifdef __WIN32__
#include <windows.h>
#endif

#ifdef __GNUC__
#include <unistd.h>
#endif

#ifdef __BEOS__
#include <OS.h> // sz: snooze() declaration ...
#endif

//  ------------------------------------------------------------------
//  Global multitasker data

GMTsk gmtsk;


//  ------------------------------------------------------------------

GMTsk::GMTsk()
{

    detected = GMTSK_NONE;
    name = "";
#if defined(__DJGPP__)
    detected = GMTSK_DOS;
    name = "DPMI32";
#elif defined(__UNIX__)
    detected = GMTSK_LINUX;
    name = "UNIX";
#else
    if(desqview())
        return;
    if(win32())
        return;
    if(os2())
        return;
    if(windows())
        return;
    dosint28();
#endif
}


//  ------------------------------------------------------------------

int GMTsk::os2()
{

#if defined(__OS2__)
    detected = GMTSK_OS2;
    name = "OS/2";
#elif !defined(__GNUC__) && !defined(_MSC_VER)
    if(_osmajor >= 10)
    {
        detected = GMTSK_OS2;
        name = "OS/2";
    }
#endif
    return detected;
}


//  ------------------------------------------------------------------

int GMTsk::win32()
{

#if defined(__WIN32__)
#if defined(__BORLANDC__)
    if(GetProcAddress(GetModuleHandle("kernel32.dll"), "Borland32"))
    {
        name = "DOS (32RTM.EXE)";
        detected = GMTSK_DOS;
        return detected;
    }
#endif
    detected = GMTSK_W32;
    name = "Win32";
#endif
    return detected;
}


//  ------------------------------------------------------------------

int GMTsk::windows()
{

#if defined(__MSDOS__) && !defined(__DJGPP__)
    i86 cpu;
    cpu.ax(0x352F);
    cpu.genint(0x21);
    if(cpu.es() != 0)
    {
        cpu.ax(0x1600);
        cpu.genint(0x2F);
        if(cpu.ax() & 0x007F)
        {
            detected = GMTSK_WINDOWS;
            name = "Windows";
        }
    }
#endif
    return detected;
}


//  ------------------------------------------------------------------

int GMTsk::desqview()
{

#if defined(__MSDOS__) && !defined(__DJGPP__)
    i86 cpu;
    cpu.cx(0x4445);
    cpu.dx(0x5351);
    cpu.ax(0x2B01);
    cpu.genint(0x21);
    if(cpu.al() != 0xFF)
    {
        if(cpu.bx())
        {
            detected = GMTSK_DESQVIEW;
            name = "DESQview";
        }
    }
#endif
    return detected;
}


//  ------------------------------------------------------------------

int GMTsk::dosint28()
{

#if defined(__MSDOS__) && !defined(__DJGPP__)
    detected = GMTSK_DOS;
    name = "DOS";
#endif

    return detected;
}


//  ------------------------------------------------------------------

void GMTsk::timeslice()
{

#if defined(__DJGPP__)
    __dpmi_yield();
#else
#if defined(__MSDOS__)
    i86 cpu;
#endif

    switch(detected)
    {
#if defined(__UNIX__)
    case GMTSK_LINUX:
#if defined(__BEOS__)
        snooze(5000);
#else
        usleep(5000);
#endif
        break;
#endif
#if defined(__WIN32__)
    case GMTSK_W32:
        Sleep(5);
        break;
    case GMTSK_DOS:
        Sleep(5);
        break;
#endif
#if defined(__MSDOS__) || defined(__OS2__)
    case GMTSK_OS2:
#if defined(__OS2__)
        DosSleep(5);
        break;
#endif
        // Drop through if this is a DOS version
#if defined(__MSDOS__)
    case GMTSK_WINDOWS:
        cpu.ax(0x1680);
        cpu.genint(0x2F);
        break;
    case GMTSK_DESQVIEW:
        cpu.ax(0x1000);
        cpu.genint(0x15);
        break;
    case GMTSK_DOS:
        cpu.genint(0x28);
        break;
#endif
#endif
    }
#endif
}


//  ------------------------------------------------------------------
