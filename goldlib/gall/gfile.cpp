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
//  File I/O class.
//  ------------------------------------------------------------------

#include <cerrno>
#include <cstdarg>
#include <gfile.h>

#if defined(_MSC_VER) /*&& (_MSC_VER >= 1400)*/

    #define g_popen(comm, mode)         _tpopen(comm, mode)
    #define g_pclose(fp)                _pclose(fp)

    #define g_sopen(fn, of, sh, pm)     _tsopen(fn, of, sh, pm)
    #define g_close(fh)                 _close(fh)
    #define g_read(fh, buf, cnt)        _read(fh, buf, cnt)
    #define g_write(fh, buf, cnt)       _write(fh, buf, cnt)
    #define g_tell(fh)                  _tell(fh)
    #define g_lseek(fh, off, org)       _lseek(fh, off, org)
    #define g_filelength(fh)            _filelength(fh)
    #define g_chsize(fh, size)          _chsize(fh, size)

    #define g_fsopen(fn, of, sh)        _tfsopen(fn, of, sh)
    #define g_fdopen(fp, of)            _tfdopen(fp, of)
    #define g_fileno(fp)                _fileno(fp)

#else

    #define g_popen(comm, mode)         popen(comm, mode)
    #define g_pclose(fp)                pclose(fp)

    #define g_sopen(fn, of, sh, pm)     sopen(fn, of, sh, pm)
    #define g_close(fh)                 close(fh)
    #define g_read(fh, buf, cnt)        read(fh, buf, cnt)
    #define g_write(fh, buf, cnt)       write(fh, buf, cnt)
    #define g_tell(fh)                  tell(fh)
    #define g_lseek(fh, off, org)       lseek(fh, off, org)
    #define g_filelength(fh)            filelength(fh)
    #define g_chsize(fh, size)          chsize(fh, size)

    #define g_fsopen(fn, of, sh)        fsopen(fn, of, sh)
    #define g_fdopen(fp, of)            fdopen(fp, of)
    #define g_fileno(fp)                fileno(fp)

#endif

#define g_lock(fh, off, len)        lock(fh, off, len)
#define g_unlock(fh, off, len)      unlock(fh, off, len)

#define g_fclose(fp)                fclose(fp)
#define g_fread(buf, rsz, cnt, fp)  fread(buf, rsz, cnt, fp)
#define g_fwrite(buf, rsz, cnt, fp) fwrite(buf, rsz, cnt, fp)
#define g_fgetc(fp)                 fgetc(fp)
#define g_fputc(c, fp)              fputc(c, fp)
#define g_fgets(str, cnt, fp)       fgets(str, cnt, fp)
#define g_fputs(str, fp)            fputs(str, fp)
#define g_fflush(fp)                fflush(fp)
#define g_ftell(fp)                 ftell(fp)


//  ------------------------------------------------------------------

gfile::gfile()
{
    fh = -1;
    fp = NULL;
    status = EBADF;
}


//  ------------------------------------------------------------------
/*
gfile::gfile(int __fh)
{
  fh = __fh;
  fp = NULL;
  status = 0;
}
*/

//  ------------------------------------------------------------------
/*
gfile::gfile(FILE* __fp)
{
  fh = -1;
  fp = __fp;
  status = 0;
}
*/

//  ------------------------------------------------------------------

gfile::gfile(const char* __path, int __access, int __shflag, int __mode)
{
    fh = -1;
    fp = NULL;
    if( !( __path && *__path ) )
    {
        status = EINVAL;
        return;
    }
    Open(__path, __access, __shflag, __mode);
}

//  ------------------------------------------------------------------

gfile::gfile(const char* __path, const char* __mode, int __shflag)
{
    fh = -1;
    fp = NULL;
    if( !( __path && *__path ) )
    {
        status = EINVAL;
        return;
    }
    Fopen(__path, __mode, __shflag);
}

//  ------------------------------------------------------------------

gfile::~gfile()
{
    if (fp != NULL) Fclose();
    if (fh != -1) Close();
}

//  ------------------------------------------------------------------

bool gfile::isopen()
{
    if ((fh != -1) or (fp != NULL)) return true;
    return false;
}

//  ------------------------------------------------------------------

int gfile::setfh(int __fh)
{
    fh = __fh;
    status = 0;
    return fh;
}

//  ------------------------------------------------------------------

FILE* gfile::setfp(FILE* __fp)
{
    fp = __fp;
    status = 0;
    return fp;
}

//  ------------------------------------------------------------------

int gfile::Open(const char* __path, int __access, int __shflag, int __mode)
{
    if( !( __path && *__path ) )
    {
        status = EINVAL;
        return -1;
    }

#if defined(_tsopen_s)
    status = _tsopen_s(&fh, __path, __access, __shflag, __mode);
    return fh;
#else
    fh = g_sopen(__path, __access, __shflag, __mode);
    status = (fh == -1) ? errno : 0;
    return fh;
#endif
}


//  ------------------------------------------------------------------

int gfile::Open(const char* __path, int __access, const char* __fmode, int __shflag, int __mode)
{
    if( !(__path && *__path &&  __fmode && *__fmode) )
    {
        status = EINVAL;
        return -1;
    }
    Open(__path, __access, __shflag, __mode);
    Fdopen(__fmode);
    return fh;
}

//  ------------------------------------------------------------------

int gfile::Close()
{
    if (fp) return Fclose();
    if (fh==-1)
    {
        status = 0;
        return 0;
    }
    int _ret = g_close(fh);
    status = _ret ? errno : 0;
    fh = -1;
    return _ret;
}

//  ------------------------------------------------------------------

int gfile::Read(void* __ptr, size_t __len)
{
    if( !__ptr )
    {
        status = EINVAL;
        return -1;
    }
    int _ret = g_read(fh, __ptr, unsigned(__len));
    status = (_ret == -1) ? errno : 0;
    return _ret;
}

//  ------------------------------------------------------------------

int gfile::Write(const void* __ptr, size_t __len)
{
    if( !__ptr )
    {
        status = EINVAL;
        return -1;
    }
    int _ret = g_write(fh, __ptr, unsigned(__len));
    status = (_ret == -1) ? errno : 0;
    return _ret;
}

//  ------------------------------------------------------------------

long gfile::Tell()
{
    long _ret = g_tell(fh);
    status = (_ret == -1) ? errno : 0;
    return _ret;
}

//  ------------------------------------------------------------------

long gfile::Lseek(long __offset, int __direction)
{
    long _ret = g_lseek(fh, __offset, __direction);
    status = (_ret == -1) ? errno : 0;
    return _ret;
}

//  ------------------------------------------------------------------

long gfile::FileLength()
{
    long _ret = g_filelength(fh);
    status = (_ret == -1) ? errno : 0;
    return _ret;
}

//  ------------------------------------------------------------------

int gfile::ChSize(long __size)
{
    int _ret = g_chsize(fh, __size);
    status = _ret ? errno : 0;
    return _ret;
}

//  ------------------------------------------------------------------

int gfile::Lock(long __offset, long __len)
{
    int _ret = g_lock(fh, __offset, __len);
    status = _ret ? errno : 0;
    return _ret;
}

//  ------------------------------------------------------------------

int gfile::Unlock(long __offset, long __len)
{
    int _ret = g_unlock(fh, __offset, __len);
    status = _ret ? errno : 0;
    return _ret;
}

//  ------------------------------------------------------------------

int gfile::GetFTime(time32_t *__ftime)
{
    struct stat s;
    if (fp) Fflush();
    int rv = fstat(fh, &s);
    status = (rv) ? errno : 0;
    if (rv == 0) *__ftime = gfixstattime(time32_t(s.st_mtime));
    else __ftime = 0;
    return rv;
}


//  ------------------------------------------------------------------

FILE* gfile::Fopen(const char* __path, const char* __mode, int __shflag)
{
    if( !(__path && *__path && __mode && *__mode) )
    {
        status = EINVAL;
        return NULL;
    }
    fp = g_fsopen(__path, __mode, __shflag);
    status = (fp == NULL) ? errno : 0;
    if (fp) fh = g_fileno(fp);
    return fp;
}


//  ------------------------------------------------------------------

FILE* gfile::Popen(const char* __path, const char* __mode)
{
    if( !(__path && *__path && __mode && *__mode) )
    {
        status = EINVAL;
        return NULL;
    }
    fp = g_popen(__path, __mode);
    status = (fp == NULL) ? errno : 0;
    if (fp) fh = g_fileno(fp);
    return fp;
}


//  ------------------------------------------------------------------

FILE* gfile::Fdopen(const char* __mode)
{
    if( !(__mode) )
    {
        status = EINVAL;
        return NULL;
    }
    fp = g_fdopen(fh, __mode);
    status = fp ? 0 : errno;
    if (fp) fh = g_fileno(fp);
    return fp;
}


//  ------------------------------------------------------------------

int gfile::Fclose()
{
    int _ret = 0;
    if (fp) _ret = g_fclose(fp);
    status = _ret ? errno : 0;
    fp = NULL;
    fh = -1;
    return _ret;
}


//  ------------------------------------------------------------------

int gfile::Pclose()
{
    int _ret = 0;
    if (fp) _ret = g_pclose(fp);
    status = _ret ? errno : 0;
    fp = NULL;
    fh = -1;
    return _ret;
}

//  ------------------------------------------------------------------

size_t gfile::Fread(void* __ptr, size_t __size, size_t __items)
{
    if( !(__ptr) )
    {
        status = EINVAL;
        return 0;
    }
    size_t _ret = g_fread(__ptr, __size, __items, fp);
    status = ferror_() ? errno : 0;
    return _ret;
}


//  ------------------------------------------------------------------

size_t gfile::Fwrite(const void* __ptr, size_t __size, size_t __items)
{
    if( !(__ptr) )
    {
        status = EINVAL;
        return 0;
    }
    size_t _ret = g_fwrite(__ptr, __size, __items, fp);
    status = (_ret < __items) ? errno : 0;
    return _ret;
}


//  ------------------------------------------------------------------

int gfile::Fgetc()
{
    int _ret = g_fgetc(fp);
    status = ferror_() ? errno : 0;
    return _ret;
}


//  ------------------------------------------------------------------

int gfile::Fputc(int __ch)
{
    int _ret = g_fputc(__ch, fp);
    status = ferror_() ? errno : 0;
    return _ret;
}

//  ------------------------------------------------------------------

char* gfile::Fgets(char* __str, size_t __len)
{
    if( !(__str) )
    {
        status = EINVAL;
        return NULL;
    }
    char* _ret = g_fgets(__str, int(__len), fp);
    status = (_ret == NULL) ? errno : 0;
    return _ret;
}

//  ------------------------------------------------------------------

int gfile::Fputs(const char* __str)
{
    if( !(__str) )
    {
        status = EINVAL;
        return -1;
    }
    int _ret = g_fputs(__str, fp);
    status = (_ret == EOF) ? errno : 0;
    return _ret;
}


//  ------------------------------------------------------------------

int gfile::Printf(const char* __format, ...)
{
    if( !(__format && *__format) )
    {
        status = EINVAL;
        return -1;
    }
    va_list _argptr;
    va_start(_argptr, __format);
    int _outcount = vfprintf(fp, __format, _argptr);
    va_end(_argptr);
    return _outcount;
}

//  ------------------------------------------------------------------

int gfile::Fflush()
{
    int _ret = g_fflush(fp);
    status = _ret ? errno : 0;
    return _ret;
}

//  ------------------------------------------------------------------

long gfile::Ftell()
{
    long _ret = g_ftell(fp);
    status = (_ret == -1) ? errno : 0;
    return _ret;
}

//  ------------------------------------------------------------------

int gfile::Fseek(long __offset, int __direction)
{
    int _ret = ::fseek(fp, __offset, __direction);
    status = _ret ? errno : 0;
    return _ret;
}

//  ------------------------------------------------------------------

int gfile::SetvBuf(char* __buf, int __type, size_t __size)
{
    if( !(__buf && __size) )
    {
        status = EINVAL;
        return -1;
    }
    int _ret = ::setvbuf(fp, __buf, __type, __size);
    status = _ret ? errno : 0;
    return _ret;
}


//  ------------------------------------------------------------------
