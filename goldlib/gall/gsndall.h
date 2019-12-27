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
//  Sound interface class.
//  ------------------------------------------------------------------

#ifndef __gsndall_h
#define __gsndall_h


//  ------------------------------------------------------------------

#include <gsndsapi.h>
#ifdef __OS2__
#define INCL_BASE
#include <os2.h>
#endif
#ifdef __WIN32__
#include <windows.h>
#endif


//  ------------------------------------------------------------------

#ifdef DEBUG
extern int debug;
#endif


//  ------------------------------------------------------------------

class gsnd
{

protected:

#if defined(__MSDOS__)
    int mpx;
#if defined(__DJGPP__) || (defined(__WATCOMC__) && defined(__386__))
    int buffer;
#else
    char* buffer;
#endif
    gsapidata* data;
    uint key_value;
    int call_api(uint al, uint bx=0);
#endif

    int api_open;
    int file_open;

    void free_buffer();

public:

    gsnd();
    ~gsnd();

    // API open/close
    int open_api();
    int close_api();

    // Sound file open/close
    int open(const char* file);
    int close();

    // Sound play and control
    int play(uint sample_rate=0);
    int stop();
    int pause();
    int resume();
    int break_loop(int method=0);
    void speaker(int onoff);

    // Information
    uint get_sample_rate();
    int api_is_open()
    {
        return api_open;
    }
    int is_playing();
};


//  ------------------------------------------------------------------

class gsound
{

protected:

    int installed;

public:

    gsound();
    ~gsound();

    int load(const char* file);
    int unload();
    int play();
    int stop();

    int is_playing();

    int is_installed()
    {
        return installed;
    }
};


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
