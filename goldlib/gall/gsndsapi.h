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

#ifndef __g_sapi_h
#define __g_sapi_h


//  ------------------------------------------------------------------

#include <gdefs.h>


//  ------------------------------------------------------------------
//  Goldware Sound API version

#define GSAPI_VERSION 0x0100


//  ------------------------------------------------------------------
//  Goldware Sound API functions

#define GSAPI_INSTALL_CHECK       0x00
#define GSAPI_OPEN_API            0x10
#define GSAPI_CLOSE_API           0x11
#define GSAPI_OPEN_AND_LOAD_FILE  0x12
#define GSAPI_CLOSE_FILE          0x13
#define GSAPI_PLAY                0x14
#define GSAPI_STOP                0x15
#define GSAPI_PAUSE               0x16
#define GSAPI_RESUME              0x17
#define GSAPI_BREAK_LOOP          0x18
#define GSAPI_SPEAKER_ON_OFF      0x19


//  ------------------------------------------------------------------
//  Goldware Sound API data structure

struct gsapidata {
           word driver_version;
           word dsp_version;
           word io_port;
           byte irq_number;
           byte dma_channel;
           word sample_rate;
  volatile word status;
#if defined(__DJGPP__) or (defined(__WATCOMC__) and defined(__386__))
           int buffer_segment;
           int buffer_offset;
#else
           word buffer_segment;
           word buffer_offset;
#endif
           long buffer_length;
           char parameters[80];
};


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
