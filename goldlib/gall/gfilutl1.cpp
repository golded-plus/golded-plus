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
#include <utime.h>

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

char* AddBackslash(char* p) {

  if(*p) {
    strchg(p, GOLD_WRONG_SLASH_CHR, GOLD_SLASH_CHR);
    if(p[strlen(p)-1] != GOLD_SLASH_CHR)
      strcat(p, GOLD_SLASH_STR);
  }
  else
    strcpy(p, GOLD_SLASH_STR);

  return p;
}


//  ------------------------------------------------------------------

char* StripBackslash(char* p) {

  int x = strlen(p) - 1;

  if(*p and isslash(p[x]))
    p[x] = NUL;

  return p;
}


//  ------------------------------------------------------------------
//  Get size of file

long GetFilesize(const char* file) {

  struct stat info;
  if(stat(file, &info) == 0)
    return info.st_size;
  return -1;
}


//  ------------------------------------------------------------------
//  Convert time returned with stat to FFTime

dword gfixstattime(time_t st_time) {

  #if defined(__MINGW32__) or defined(__CYGWIN__)
  struct tm &f = *gmtime(&st_time);
  #else
  struct tm &f = *localtime(&st_time);
  #endif
  FFTime t;
  t.ft_year  = f.tm_year - 80;
  t.ft_month = f.tm_mon + 1;
  t.ft_day   = f.tm_mday;
  t.ft_hour  = f.tm_hour;
  t.ft_min   = f.tm_min;
  t.ft_tsec  = f.tm_sec / 2;
  #if defined(__MINGW32__) or defined(__CYGWIN__)
  union {
    DWORD t;
    struct {
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

dword GetFiletime(const char* file) {

  struct stat st;
  if(stat(file, &st) == 0) {
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

long fsize(FILE* fp) {

  return filelength(fileno(fp));
}


//  ------------------------------------------------------------------
//  Check if a pathname is a directory

int is_dir(const char* path) {

  // Check if it's a root path (X:\)
  #if defined(__HAVE_DRIVES__)
  if(isalpha(path[0]) and (path[1] == ':') and isslash(path[2]) and (path[3] == NUL))
    return true;  // The root is a directory
  #endif

  Path tmp;
  strxcpy(tmp, path, sizeof(Path));
  StripBackslash(tmp);

  struct stat st;
  if(stat(tmp, &st) == 0)
    return (st.st_mode & S_IFDIR) ? true : false;
  return false;
}


//  ------------------------------------------------------------------
//  Add path to filename if no path is present

static Path __addpath;

const char* AddPath(const char* path, const char* file) {

  if(strpbrk(file, "/\\")) {
    // Don't add path if the filename already contains one
    return file;
  }
  else if(*path and ((*file == '.') or isslash(path[strlen(path)-1]))) {
    // Build path+filename if path ends with a slash or backslash
    strxmerge(__addpath, sizeof(Path), path, file, NULL);
    return __addpath;
  }
  else {
    // Use filename in path or file
    return *path ? path : file;
  }
}


//  ------------------------------------------------------------------
//  Add path to filename, if no path is set

void MakePathname(char* pathname, const char* path, const char* name) {

  char newpath[GMAXPATH];
  Path tmpname;
  Path tmppath;
  strcpy(tmpname, name);
  strcpy(tmppath, path);

  if(strblank(tmpname)) {
    *pathname = NUL;
    return;
  }

  strbtrim(tmppath);
  strchg(tmppath, GOLD_WRONG_SLASH_CHR, GOLD_SLASH_CHR);
  strchg(tmpname, GOLD_WRONG_SLASH_CHR, GOLD_SLASH_CHR);
  if(strpbrk(tmpname, GOLD_SLASH_STR))
    strschg_environ(strxcpy(pathname, tmpname, sizeof(Path)));
  else {
    strcpy(newpath, tmppath);
    if(*newpath)
      AddBackslash(newpath);
    strxmerge(pathname, sizeof(Path), newpath, tmpname, NULL);
  }
}


//  ------------------------------------------------------------------
//  Shareable fopen() for compilers that need it

FILE *fsopen(const char *path, const char *type, int shflag) {

  FILE* fp=NULL;
  int fh=-1, acc=0, mode=S_STDRD, c, n;

  switch(type[0]) {
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
  do {
    c = type[++n];
    switch(c) {
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
  } while((n < 3) && (c));

  fh = sopen(path, acc, shflag, mode);
  if(fh != -1)
    fp = fdopen(fh, (char*)type);

  return fp;
}


//  ------------------------------------------------------------------

void TouchFile(const char* filename) {

  if(not fexist(filename))
    close(open(filename, O_WRONLY|O_CREAT|O_TRUNC, S_STDRW));
  else {
    struct utimbuf ut;
    ut.actime = ut.modtime = time(NULL);
    utime(filename, &ut);
  }
}


//  ------------------------------------------------------------------

char* PathCopy(char* __dst, const char* __src) {
  strschg_environ(strxcpy(__dst, __src, sizeof(Path)));
  return AddBackslash(__dst);
}


//  ------------------------------------------------------------------

int TestLockPath(const char* __path) {

  int _canlock = false;

  Path _file;
  strxmerge(_file, sizeof(Path), __path, "GDXXXXXX", NULL);
  mktemp(_file);

  int _fh = sopen(_file, O_RDWR|O_CREAT|O_TRUNC|O_BINARY, SH_DENYNO, S_STDRW);
  if(_fh != -1) {
    if(lock(_fh, 0L, 1L) == -1)
      _canlock = NO;
    else {
      _canlock = YES;
      unlock(_fh, 0L, 1L);
    }
    close(_fh);
    remove(_file);
  }

  return _canlock;
}


//  ------------------------------------------------------------------

const char* CleanFilename(const char* __file) {

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

void WipeFile(const char* file, int options) {

  uint n;
  byte buf[512];

  switch(options) {
    default:
      for(n=0; n<512; n++)
        buf[n] = (byte)(rand() % 256);
  }

  int fh = sopen(file, O_RDWR|O_BINARY, SH_DENYRW, S_STDRW);
  if(fh != -1) {
    uint blocks = (uint)(filelength(fh) / 512L) + 1;
    for(n=0; n<blocks; n++)
      write(fh, buf, 512);
    chsize(fh, 0);
    close(fh);
    remove(file);
  }
}


//  ------------------------------------------------------------------

int strschg_environ(char* s) {

  if(*s == NUL)
    return 0;

  string __s = s;
  int rv = strschg_environ(__s);
  if(rv)
    strxcpy(s, __s.c_str(), sizeof(Path));
  return rv;
}


//  ------------------------------------------------------------------

int gchdir(const char* dir) {

  #if defined(__WIN32__)
  return not SetCurrentDirectory(dir);
  #else
  #if defined(__HAVE_DRIVES__)
  if(dir[1] == ':') {
    #if defined(__EMX__)
    _chdrive(*dir);
    #else
    uint drives;
    _dos_setdrive(toupper(*dir)-'@', &drives);
    #endif
  }
  #endif
  int e = chdir(dir);
  if(e) {
    Path p;
    strcpy(p, dir);
    StripBackslash(p);
    e = chdir(p);
  }
  return e;
  #endif
}


//  ------------------------------------------------------------------

void replaceextension(char *destpath, const char *srcpath, const char *ext) {

  const char *ptr;
  char *ptr2, *slash, *dot;
  ptr2 = slash = dot = destpath;
  ptr = srcpath;
  while(*ptr) {
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

void extractdirname(char *dir, const char *path) {

  const char *p1 = path;
  char *p2, *p3;
  p3 = p2 = dir;
  while(*p1) {
    if(isslash(*p1))
      p2 = p3;
    *p3++ = *p1++;
  }
  if(isslash(*p2))
    ++p2;
  *p2 = NUL;
}


//  ------------------------------------------------------------------

