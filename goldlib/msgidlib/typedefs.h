/* This is modifyed part of HUSKY project */
/*
 *  SMAPI; Modified Squish MSGAPI
 *
 *  Squish MSGAPI0 is copyright 1991 by Scott J. Dudley.  All rights reserved.
 *  Modifications released to the public domain.
 *
 *  Use of this file is subject to the restrictions contain in the Squish
 *  MSGAPI0 licence agreement.  Please refer to licence.txt for complete
 *  details of the licencing restrictions.  If you do not find the text
 *  of this agreement in licence.txt, or if you do not have this file,
 *  you should contact Scott Dudley at FidoNet node 1:249/106 or Internet
 *  e-mail Scott.Dudley@f106.n249.z1.fidonet.org.
 *
 *  In no event should you proceed to use any of the source files in this
 *  archive without having accepted the terms of the MSGAPI0 licensing
 *  agreement, or such other agreement as you are able to reach with the
 *  author.
 */

#ifndef __TYPEDEFS_H__
#define __TYPEDEFS_H__

#ifdef SASC
#include <dos.h>
#endif

typedef unsigned bit;

#ifndef SASC
typedef unsigned char byte;
#endif

typedef signed char sbyte;

typedef unsigned short word;
typedef signed short sword;

#ifdef __alpha    /* add other 64 bit systems here */
typedef unsigned int dword;
typedef signed   int sdword;
#else             /* 32 and 16 bit machines */
typedef unsigned long dword;
typedef signed long sdword;
#endif

#if !defined(UNIX) && !defined(SASC) && !defined(__MINGW32__)
typedef unsigned short ushort;
#endif

typedef signed short sshort;

#ifndef UNIX
typedef unsigned long ulong;
#endif

typedef signed long slong;

#endif
