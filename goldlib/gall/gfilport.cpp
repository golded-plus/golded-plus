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
//  Ports of OS-specific file/path functions.
//  ------------------------------------------------------------------

#include <gfilutil.h>

#if defined(__OS2__)
#define INCL_BASE
#define INCL_ERROR_H
#include <os2.h>
#endif

#if defined(__WIN32__)
#include <windows.h>
#endif

#if defined(__CYGWIN__)
#include <io.h>
#endif

#if defined(__MINGW32__)
#include <sys/locking.h>
#endif

#if defined(__BEOS__)
#include "be_lock.h"
#endif


//  ------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif


//  ------------------------------------------------------------------

#if !defined(__DJGPP__) && ((defined(__BORLANDC__) && defined(__OS2__)) || defined(__UNIX__) || defined(__EMX__))

off_t filelength(int handle)
{

    struct stat s;
    if(fstat(handle, &s) == 0)
        return s.st_size;
    else
        return -1;
}

#endif


//  ------------------------------------------------------------------

#if defined(__OS2__)

int lock(int handle, long ofs, long length)
{
    int rc;
    APIRET apiret;
    FILELOCK urange, lrange;

    lrange.lOffset = ofs;
    lrange.lRange = length;
    urange.lRange = urange.lOffset = 0;

    if((apiret = DosSetFileLocks((HFILE)handle, &urange, &lrange, 0, 0)) != 0)
    {
        if(apiret == ERROR_LOCK_VIOLATION)
        {
            errno = EACCES;
            rc = -1;
        }
        else
            rc = (int)apiret;
    }
    else
        rc = 0;

    return rc;
}

#endif


//  ------------------------------------------------------------------

#if defined(__OS2__)

int unlock(int handle, long ofs, long length)
{
    int rc;
    FILELOCK urange, lrange;
    APIRET apiret;

    urange.lOffset = ofs;
    urange.lRange = length;
    lrange.lRange = lrange.lOffset = 0;

    if((apiret = DosSetFileLocks((HFILE)handle, &urange, &lrange, 0, 0)) != 0)
    {
        if(apiret == ERROR_LOCK_VIOLATION)
        {
            errno = EACCES;
            rc = -1;
        }
        else
            rc = apiret;
    }
    else
        rc = 0;

    return rc;
}

#endif


//  ------------------------------------------------------------------

#if defined(__UNIX__) || defined(__CYGWIN__)

int lock(int handle, long offset, long length)
{

#ifndef __BEOS__
    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = offset;
    fl.l_len = length;

    return fcntl(handle, F_SETLK, &fl);
#else //__BEOS__
    return be_lock(handle);
#endif
}

#endif


//  ------------------------------------------------------------------

#if defined(__UNIX__) || defined(__CYGWIN__)

int unlock(int handle, long offset, long length)
{

#ifndef __BEOS__
    struct flock fl;
    fl.l_type = F_UNLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = offset;
    fl.l_len = length;

    return fcntl(handle, F_SETLK, &fl);
#else //__BEOS__
    return be_unlock(handle);
#endif //__BEOS__
}

#endif


//  ------------------------------------------------------------------

#if defined(__MINGW32__) || defined(_MSC_VER)

int lock(int fh, long offs, long len)
{

    if(LockFile((HANDLE)_get_osfhandle(fh), offs, 0L, len, 0L)) return 0;
    else return -1;
}

#endif


//  ------------------------------------------------------------------

#if defined(__MINGW32__) || defined(_MSC_VER)

int unlock(int fh, long offs, long len)
{

    if(UnlockFile((HANDLE)_get_osfhandle(fh), offs, 0L, len, 0L)) return 0;
    else return -1;
}

#endif


//  ------------------------------------------------------------------

#ifdef __cplusplus
}
#endif


//  ------------------------------------------------------------------

