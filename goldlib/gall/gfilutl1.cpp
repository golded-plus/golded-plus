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
//  File utility functions
//  ------------------------------------------------------------------

#include <gtimall.h>
#include <gstrall.h>
#include <gfilutil.h>
#include <gfile.h>
#include <stdlib.h>

#if defined(__MINGW32__) || defined(_MSC_VER)
    #include <sys/utime.h>
#else
    #include <utime.h>
#endif

#if defined(__OS2__)
    #define INCL_BASE
    #include <os2.h>
#endif

#if defined(__WIN32__)
    #include <windows.h>
#endif

#if defined(__MSDOS__)
    #include <dos.h>
#endif

#if defined(__UNIX__)
    #include <pwd.h>
    #include <sys/types.h>
#endif

//  ------------------------------------------------------------------
//  Adds the directory-delimiter character into end of string ('\\' in DOS-based, '/' in unix-based OS)
// Replace wrong directory-delimiter character with good.

char* AddBackslash(char* p)
{

    if(p != NULL)
    {
        if(*p)
        {
            strchg(p, GOLD_WRONG_SLASH_CHR, GOLD_SLASH_CHR);
            if(p[strlen(p)-1] != GOLD_SLASH_CHR)
                strcat(p, GOLD_SLASH_STR);
        }
        else
            strcpy(p, GOLD_SLASH_STR);
    }

    return p;
}


//  ------------------------------------------------------------------
//  Remove one trailing directory-delimiter character ('\\' in DOS-based, '/' in unix-based OS)

char* StripBackslash(char* p)
{

    if(p == NULL) return p;

    int x = strlen(p) - 1;

    if(*p and isslash(p[x]))
        p[x] = NUL;

    return p;
}


//  ------------------------------------------------------------------
//  Get size of file

long GetFilesize(const char* file)
{

    if(file == NULL) return 0;

    struct stat info;
    if(stat(file, &info) == 0)
        return info.st_size;
    return -1;
}


//  ------------------------------------------------------------------
//  Convert time returned with stat to FFTime

time32_t gfixstattime(time32_t st_time)
{
#if (defined(__MINGW32__) && !defined(__MSVCRT__)) || defined(__CYGWIN__)
    struct tm f;
    ggmtime(&f, &st_time);
#else
    struct tm f;
    glocaltime(&f, &st_time);
#endif
    FFTime t;
    t.ft_year  = f.tm_year - 80;
    t.ft_month = f.tm_mon + 1;
    t.ft_day   = f.tm_mday;
    t.ft_hour  = f.tm_hour;
    t.ft_min   = f.tm_min;
    t.ft_tsec  = f.tm_sec / 2;
#if (defined(__MINGW32__) && !defined(__MSVCRT__)) || defined(__CYGWIN__)
    union
    {
        DWORD t;
        struct
        {
            WORD wFatTime;
            WORD wFatDate;
        } d;
    } ft;
    ft.t = (DWORD)t.number();
    FILETIME FileTime, LocalFileTime;
    DosDateTimeToFileTime(ft.d.wFatDate, ft.d.wFatTime, &FileTime);
    FileTimeToLocalFileTime(&FileTime, &LocalFileTime);
    SYSTEMTIME SystemTime;
    FileTimeToSystemTime(&LocalFileTime, &SystemTime);
    t.ft_year = SystemTime.wYear - 1980;
    t.ft_month = SystemTime.wMonth;
    t.ft_day = SystemTime.wDay;
    t.ft_hour = SystemTime.wHour;
    t.ft_min = SystemTime.wMinute;
    t.ft_tsec = SystemTime.wSecond / 2;
#endif
    return t.number();
}


//  ------------------------------------------------------------------
//  Get timestamp of file

time32_t GetFiletime(const char* file)
{

    struct stat st;

    if(file == NULL) return 0;

    if(stat(file, &st) == 0)
    {
#if defined(__MINGW32__)
        if(st.st_mode & S_IFDIR)
            return 0;
#endif
        return gfixstattime(st.st_mtime);
    }
    return 0;
}


//  ------------------------------------------------------------------
//  Get size of open file

long fsize(FILE* fp)
{
    if(fp == NULL) return 0;
    return filelength(fileno(fp));
}


//  ------------------------------------------------------------------
//  Check if a pathname is a directory

bool is_dir(const TCHAR *path)
{
    if(path == NULL) return false;

    // Check if it's a root path (X:\ оr /)
#if defined(__HAVE_DRIVES__)
    if(g_isalpha(path[0]) and (path[1] == ':') and isslash(path[2]) and (path[3] == NUL))
        return true;  // The root is a directory
# else
    if( isslash(path[0]) and (path[1] == NUL))
        return true;  // The root is a directory
#endif

    Path tmp;
    strxcpy(tmp, path, sizeof(Path));
    StripBackslash(tmp);

    struct stat st;
    if(stat(tmp, &st) == 0)
        return make_bool(st.st_mode & S_IFDIR);
    return false;
}


//  ------------------------------------------------------------------
//  Add path to filename if no path is present

static Path __addpath;

const char* AddPath(const char* path, const char* file)
{

    if( (path == NULL) or (file == NULL) ) return file;

    if(strpbrk(file, "/\\"))
    {
        // Don't add path if the filename already contains one
        return file;
    }
    else if(*path and ((*file == '.') or isslash(path[strlen(path)-1])))
    {
        // Build path+filename if path ends with a slash or backslash
        strxmerge(__addpath, sizeof(Path), path, file, NULL);
        return __addpath;
    }
    else
    {
        // Use filename in path or file
        return *path ? path : file;
    }
}


//  ------------------------------------------------------------------
//  Add path to filename, if no path is set

void MakePathname(char* pathname, const char* path, const char* name)
{

    Path tmpname;

    if( (pathname == NULL) or (path == NULL) or (name == NULL) ) return;

    strxcpy(tmpname, name, sizeof(tmpname));
    strschg_environ(tmpname, sizeof(tmpname));

    if(strblank(tmpname))
    {
        *pathname = NUL;
        return;
    }

    bool have_path = false;

    if(isslash(tmpname[0]))
        have_path = true;
#if defined(__HAVE_DRIVES__)
    // Check if it's a root path (X:\)
    else if(g_isalpha(tmpname[0]) and (tmpname[1] == ':') and isslash(tmpname[2]))
        have_path = true;  // The root is a directory
#endif

    strchg(tmpname, GOLD_WRONG_SLASH_CHR, GOLD_SLASH_CHR);
    if(have_path)
    {
        strxcpy(pathname, tmpname, sizeof(Path));
    }
    else
    {
        Path tmppath;

        strcpy(tmppath, path);
        strbtrim(tmppath);
        strchg(tmppath, GOLD_WRONG_SLASH_CHR, GOLD_SLASH_CHR);
        if(*tmppath)
            AddBackslash(tmppath);
        strxmerge(pathname, sizeof(Path), tmppath, tmpname, NULL);
    }
    strschg_environ(pathname);
}


//  ------------------------------------------------------------------
//  Shareable fopen() for compilers that need it

FILE *fsopen(const char *path, const char *type, int shflag)
{

    FILE* fp=NULL;
    int fh=-1, acc=0, mode=S_STDRD, c, n;

    if( (path == NULL) or (type == NULL) ) return NULL;

    switch(type[0])
    {
    case 'r':
        acc |= O_RDONLY;
        break;
    case 'w':
        acc |= (O_TRUNC|O_CREAT|O_WRONLY);
        mode |= S_STDRW;
        break;
    case 'a':
        acc |= (O_APPEND|O_CREAT|O_WRONLY);
        mode |= S_STDRW;
        break;
    default:
        return NULL;
    }
    n=0;
    do
    {
        c = type[++n];
        switch(c)
        {
        case '+':
            acc &= (~O_RDONLY);
            acc &= (~O_WRONLY);
            acc |= O_RDWR;
            break;
        case 'b':
            acc |= O_BINARY;
            break;
        case 't':
            acc |= O_TEXT;
            break;
        default:
            break;
        }
    }
    while((n < 3) and (c));

    fh = sopen(path, acc, shflag, mode);
    if(fh != -1)
        fp = fdopen(fh, (char*)type);

    return fp;
}


//  ------------------------------------------------------------------
//  Update time of modification for the file 'filename'

void TouchFile(const TCHAR *filename)
{
    if(filename == NULL) return;

    if (not fexist(filename))
    {
        gfile fh(filename, O_WRONLY|O_CREAT|O_TRUNC);
    }
    else
    {
        struct utimbuf ut;
        ut.actime = ut.modtime = time(NULL);
        utime(filename, &ut);
    }
}


//  ------------------------------------------------------------------
//  Copy pathname with enviroment variables substitution and adds directory delimiter char.
//  Copy not more sizeof(Path) characters (__dst should be type "Path" or equvalence, size is GMAXPATH)

char* PathCopy(char* __dst, const char* __src)
{
    if( (__dst == NULL) or (__src == NULL) ) return __dst;
    strschg_environ(strxcpy(__dst, __src, sizeof(Path)), sizeof(Path));
    return AddBackslash(__dst);
}

//  ------------------------------------------------------------------
//  Test filesystem for file locks feature

int TestLockPath(const char* __path)
{

    int _canlock = false;

    Path _file;

    if(__path == NULL) return NO;

    strxmerge(_file, sizeof(Path), __path, "GDXXXXXX", NULL);
    mktemp(_file);

    int _fh = sopen(_file, O_RDWR|O_CREAT|O_TRUNC|O_BINARY, SH_DENYNO, S_STDRW);
    if(_fh != -1)
    {
        if(lock(_fh, 0L, 1L) == -1)
            _canlock = NO;
        else
        {
            _canlock = YES;
            unlock(_fh, 0L, 1L);
        }
        close(_fh);
        remove(_file);
    }

    return _canlock;
}


//  ------------------------------------------------------------------
//  Return filename without path. (Return pointer to filename part of filepath.)

const char* CleanFilename(const char* __file)
{

    static const char* invalidfilename = "<invalid>";
    if((__file == NULL) or (__file == (char*)0xFFFFFFFFL) or (__file == (char*)0xEEEEEEEEL))
        return invalidfilename;

    const char *tmp, *tmp2;
    tmp = tmp2 = __file;
    while(*tmp)
        if(isslash(*tmp++))
            tmp2 = tmp;
    return *tmp2 ? tmp2 : invalidfilename;
}


//  ------------------------------------------------------------------
//  Fill file with garbage (random byte values).

void WipeFile(const char* file, int options)
{

    uint n;
    byte buf[512];

    (void)options;

    if(file == NULL) return;

    for(n=0; n<512; n++)
        buf[n] = (byte)(rand() % 256);

    int fh = sopen(file, O_RDWR|O_BINARY, SH_DENYRW, S_STDRW);
    if(fh != -1)
    {
        uint blocks = (uint)(filelength(fh) / 512L) + 1;
        for(n=0; n<blocks; n++)
            write(fh, buf, 512);
        chsize(fh, 0);
        close(fh);
        remove(file);
    }
}


//  ------------------------------------------------------------------
//  DOS-style enviroment variables substitution in string.

int strschg_environ(char* s)
{

    if(s == NULL) return 0;
    if(*s == NUL)
        return 0;

    std::string __s = s;
    int rv = strschg_environ(__s); // Look in gfilutl2.cpp
    if(rv)
        strcpy(s, __s.c_str());
    return rv;
}

int strschg_environ(char* s, size_t s_size)
{

    if(s == NULL) return 0;
    if(*s == NUL)
        return 0;

    std::string __s = s;
    int rv = strschg_environ(__s); // Look in gfilutl2.cpp
    if(rv)
        strxcpy(s, __s.c_str(), s_size);
    return rv;
}


//  ------------------------------------------------------------------
//  OS-independent change directory

int gchdir(const char* dir)
{

    if(dir == NULL) return 0;
#if defined(__WIN32__)
    return not SetCurrentDirectory(dir);
#else
#if defined(__HAVE_DRIVES__)
    if(dir[1] == ':')
    {
#if defined(__EMX__)
        _chdrive(*dir);
#else
        uint drives;
        _dos_setdrive(g_toupper(*dir)-'@', &drives);
#endif
    }
#endif
    int e = chdir(dir);
    if(e)
    {
        Path p;
        strcpy(p, dir);
        StripBackslash(p);
        e = chdir(p);
    }
    return e;
#endif
}


//  ------------------------------------------------------------------
//  Replace file suffix with specified in 'ext'

void replaceextension(char *destpath, const char *srcpath, const char *ext)
{

    const char *ptr;
    char *ptr2, *slash, *dot;

    if( (destpath == NULL) or (srcpath == NULL) or (ext == NULL) ) return;

    ptr2 = slash = dot = destpath;
    ptr = srcpath;
    while(*ptr)
    {
        if(isslash(*ptr))
            slash = ptr2;
        else if(*ptr == '.')
            dot = ptr2;
        *ptr2++ = *ptr++;
    }
    if(dot-slash > 0)
        ptr2 = dot;
    strcpy(ptr2, ext);
}


//  ------------------------------------------------------------------
//  Write to 'dir' dirname of the 'path'

void extractdirname(char *dir, const char *path)
{

    const char *p1 = path;
    char *p2, *p3;

    if( (dir == NULL) or (path == NULL) ) return;

    p3 = p2 = dir;
    *p3 = NUL;
    while(*p1)
    {
        if(isslash(*p1))
            p2 = p3;
        *p3++ = *p1++;
    }
    if(isslash(*p2))
        ++p2;
    *p2 = NUL;
}


//  ------------------------------------------------------------------
