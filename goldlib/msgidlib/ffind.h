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

#ifndef __FFIND_H__
#define __FFIND_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "hsksupp.h"

#ifdef UNIX
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#endif

#ifdef SASC
#include <stdio.h>
#include <dos.h>
#endif

#if defined(__RSXNT__) || defined(__MINGW32__) || (defined(_MSC_VER) && (_MSC_VER >= 1200))
#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOUSER
#define NOMSG

#ifdef __RSXNT__
                                /* The RSXNT winsock.h conflicts with EMX
                                   io.h. As we do not need sockets anyway, we
                                   just prevent their inclusion. */
#define _WINSOCK_H
#endif

#if defined(__MINGW32__)
/* For HTick compatibility */
#define _WINUSER_H
/* mingw32 warnings */
#define NONAMELESSUNION
#endif

#include <windows.h>
#endif


#define FFIND struct ffind

#if defined(__DJGPP__) || defined(__TURBOC__) 
#include <dir.h>
#endif

#if defined(__WATCOMC__) || defined(_MSC_VER)
#include <dos.h>
#endif


struct ffind
{
    /* this is the public area of the struct */
    char ff_attrib;
    unsigned short ff_ftime;
    unsigned short ff_fdate;
    long ff_fsize;
    char ff_name[256];

    /* now comes the privat area where search handles or similiar are stored */

#if defined(__TURBOC__) || defined(__DJGPP__)
    struct ffblk ffbuf;

#elif defined(__WATCOMC__) || (defined(_MSC_VER) && (_MSC_VER < 1200))
    struct find_t ffbuf;
    unsigned long hdir;   /* directory handle from DosFindFirst */

#elif defined(OS2)
#if defined(__386__) || defined(__FLAT__)
    unsigned long hdir;   /* directory handle from DosFindFirst */
#else
    unsigned short hdir;  /* directory handle from DosFindFirst */
#endif

#elif defined(UNIX)
    DIR *dir;
    char firstbit[FILENAME_MAX];
    char lastbit[FILENAME_MAX];

#elif defined(SASC)
    struct FileInfoBlock info;
    char newfile[FILENAME_MAX];
    char prefix[FILENAME_MAX];

#elif defined(__RSXNT__) || defined(__MINGW32__) || (defined(_MSC_VER) && (_MSC_VER >= 1200))
    WIN32_FIND_DATA InfoBuf;
    HANDLE hDirA;
    char attrib_srch;

#else
#error Unknown compiler!
#endif
};


/*
 * I prefixed the functions below with an additional F in order to
 * prevent name clashes with the Win32 API
 */

FFIND *FFindOpen(const char *filespec, unsigned short attribute);
FFIND *FFindInfo(const char *filespec);
int FFindNext(FFIND * ff);
void FFindClose(FFIND * ff);

#define MSDOS_READONLY  0x01
#define MSDOS_HIDDEN    0x02
#define MSDOS_SYSTEM    0x04
#define MSDOS_VOLUME    0x08
#define MSDOS_SUBDIR    0x10
#define MSDOS_ARCHIVE   0x20
#define MSDOS_RSVD1     0x40
#define MSDOS_RSVD2     0x80

#endif

#ifdef __cplusplus
}
#endif
