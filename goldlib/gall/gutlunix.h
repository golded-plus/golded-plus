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

#ifndef __gutlunix_h
#define __gutlunix_h
//  ------------------------------------------------------------------

    #ifdef _POSIX_VDISABLE
#define NULL_VALUE (unsigned)_POSIX_VDISABLE
    #else
#define NULL_VALUE 255
    #endif

    #ifdef __BEOS__
        #include <net/socket.h> // defintions of FD_xx - to avoid problems with code below
    #endif
//  ------------------------------------------------------------------

    #ifndef FD_SET
#define FD_SET(fd, tthis) *(tthis) = 1 << (fd)
#define FD_ZERO(tthis) *(tthis)    = 0
#define FD_ISSET(fd, tthis) (*(tthis) & (1 << fd))
typedef int fd_set;
    #endif
//  ------------------------------------------------------------------

#define GET_TERMIOS(fd, x) tcgetattr(fd, x)
#define SET_TERMIOS(fd, x) tcsetattr(fd, TCSADRAIN, x)
//  ------------------------------------------------------------------

#endif // ifndef __gutlunix_h
//  ------------------------------------------------------------------
