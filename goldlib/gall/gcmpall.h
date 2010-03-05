/*  ------------------------------------------------------------------
 *  The Goldware Library
 *  Copyright (C) 1990-1999 Odinn Sorensen
 *  Copyright (C) 1999-2000 Alexander S. Aganichev
 *  ------------------------------------------------------------------
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this program; if not, write to the Free
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 *  MA 02111-1307, USA
 *  ------------------------------------------------------------------
 *  $Id$
 *  ------------------------------------------------------------------
 *  Compiler dependent definitions.
 *  --------------------------------------------------------------- */

#ifndef __gcmpall_h
#define __gcmpall_h


/*  --------------------------------------------------------------- */
/*  Check if type "char" is unsigned or signed                      */

#if '\x80' < 0
#error Goldware Library requires -funsigned-char to operate properly
#endif


/*  --------------------------------------------------------------- */

#if !defined(__MSDOS__)
  #if defined(MSDOS)
    #define __MSDOS__ MSDOS
  #elif defined(__DOS__)
    #define __MSDOS__ __DOS__
  #endif
#endif

#if !defined(__OS2__)
  #if defined(OS2)
    #define __OS2__ OS2
  #elif defined(__EMX__) && !defined(__WIN32__)
    #define __OS2__ __EMX__
  #endif
#endif

#if !defined(__WIN32__)
  #if defined(_WIN32)
    #define __WIN32__ _WIN32
  #elif defined(__NT__)
    #define __WIN32__ __NT__
  #elif defined(WIN32)
    #define __WIN32__ WIN32
  #endif
#endif

#if !defined(__UNIX__)
  #if defined(__linux__)
    #define __UNIX__ __linux__
  #endif
  #if defined(__FreeBSD__)
    #define __UNIX__ __FreeBSD__
  #endif
  #if defined(__OpenBSD__)
    #define __UNIX__ __OpenBSD__
  #endif
  #if defined(__DragonFly__)
    #define __UNIX__ __DragonFly__
  #endif
  #if defined(__BEOS__)
    #define __UNIX__ __BEOS__
  #endif
  #if defined(__QNXNTO__)
    #define __UNIX__ __QNXNTO__
  #endif
  #if defined(__APPLE__)
    #define __UNIX__ __APPLE__
  #endif
#endif

#ifdef __GNUC__
#if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ < 8)
#error You definetly need to upgrade your gcc at least up to 2.8.x
#endif
#endif

#ifdef _MSC_VER
#define __attribute__(A)
#define __inline__ __inline
#define __extension__
#define __MSVCRT__
#endif


/*  --------------------------------------------------------------- */

#if defined(__MSDOS__) || defined(__OS2__) || defined(__WIN32__)
#define __HAVE_DRIVES__
#endif

#if defined(_MSC_VER) || defined(__MINGW32__)
#define __USE_ALLOCA__
#endif

/*  --------------------------------------------------------------- */
/*  System-wide constants                                           */

/* #define GOLD_MOUSE 1 */     /* Enable mouse code */

#define GTHROW_LOG
#define GTHROW_DEBUG
#define GTHROWCHKPTR_ENABLE
#define GFTRK_ENABLE


/*  --------------------------------------------------------------- */

#define GOLD_CANPACK
#define NW(x) x=x


/*  --------------------------------------------------------------- */

#endif
