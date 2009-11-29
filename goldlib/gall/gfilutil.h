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
//  File/disk handling functions.
//  ------------------------------------------------------------------

#ifndef __gfilutil_h
#define __gfilutil_h


//  ------------------------------------------------------------------

#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifndef _MSC_VER
#include <unistd.h>
#else
#include <cstdlib>
#include <direct.h>
#endif
#include <cerrno>
#include <cstdio>
#include <string>
#include <gshare.h>
#if !defined(__UNIX__)
#include <io.h>
#endif
#include <gdefs.h>


//  ------------------------------------------------------------------M

#if defined(__UNIX__)
#if !defined(__BEOS__) && !defined(__QNXNTO__)
#define O_TEXT 0
#define O_BINARY 0
#endif
#endif

#ifndef S_IWUSR
#define S_IWUSR S_IWRITE
#define S_IRUSR S_IREAD
#endif

#if defined(__UNIX__)
#define S_STDRW (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)
#define S_STDRD (S_IRUSR|S_IRGRP|S_IROTH)
#else
#define S_STDRW (S_IRUSR|S_IWUSR)
#define S_STDRD S_IRUSR
#endif

#ifndef S_ISDIR
#define S_ISDIR(st_mode) (make_bool((st_mode)&_S_IFDIR))
#endif

#ifndef S_ISREG
#define S_ISREG(st_mode) (make_bool((st_mode)&_S_IFREG))
#endif

#ifndef R_OK
#define R_OK 0
#endif

//  ------------------------------------------------------------------

#define GMAXPATH (FILENAME_MAX+1) /* ANSI C */


//  ------------------------------------------------------------------
//  Standardized split/merge defines

#define GMAXDRIVE   265
#define GMAXDIR     256
#define GMAXFILE    256
#define GMAXEXT     256
#define GMAXFILEEXT 256


//  ------------------------------------------------------------------
//  Misc. defines

#if defined(__MSDOS__) || defined(__OS2__) || defined(__WIN32__)
  #define GOLD_SLASH_CHR                    '\\'              // Backslash
  #define GOLD_SLASH_STR                    "\\"
  #define GOLD_WRONG_SLASH_CHR              '/'               // Fwrdslash
  #define GOLD_WRONG_SLASH_STR              "/"
  #define GOLD_SHELL_ENV                    "COMSPEC"
#else
  #define GOLD_SLASH_CHR                    '/'               // Fwrdslash
  #define GOLD_SLASH_STR                    "/"
  #define GOLD_WRONG_SLASH_CHR              '\\'              // Backslash
  #define GOLD_WRONG_SLASH_STR              "\\"
  #define GOLD_SHELL_ENV                    "SHELL"
#endif

inline bool isslash(char c) { return (c == GOLD_SLASH_CHR) or (c == GOLD_WRONG_SLASH_CHR); }


//  ------------------------------------------------------------------
//  Path typedefs

typedef char Path[GMAXPATH];


//  ------------------------------------------------------------------
//  Structure for filetime stamp checking

struct Stamp {
  dword ft;  // Timestamp
  Path fn;   // Filename
  byte fc;   // Filecheck
};


//  ------------------------------------------------------------------
//  Prototypes

#if !defined(__GNUC__) || defined(__MINGW32__)
#define mkdir(path,unused) mkdir(path)
#endif

#ifdef __EMX__
#define getcwd _getcwd2
#define chdir _chdir2
#endif

//  Shareable fopen() for compilers that need it
FILE* fsopen(const char* path, const char* type, int shflag);
inline FILE* fsopen(const std::string& path, const char* type, int shflag) { return fsopen(path.c_str(), type, shflag); }

//  ------------------------------------------------------------------
//  Check if a pathname is a directory
bool is_dir(const TCHAR *path);
inline bool is_dir(const std::string &path) { return is_dir(path.c_str()); }

#if defined(_taccess_s)
inline bool fexist(const TCHAR *filename) { return *filename ? (0 == (_taccess_s(filename, R_OK)) && !is_dir(filename)) : false; }
#else
inline bool fexist(const TCHAR *filename) { return *filename ? (0 == (access(filename, R_OK)) && !is_dir(filename)) : false; }
#endif
inline bool fexist(const std::string& filename) { return fexist(filename.c_str()); }

//  Convert time returned with stat to FFTime
time32_t gfixstattime(time32_t st_time);

//  Get timestamp of file
time32_t GetFiletime(const char* file);
inline time32_t GetFiletime(const std::string& file) { return GetFiletime(file.c_str()); }

inline long FiletimeCmp(const char* file1, const char* file2) { return long(GetFiletime(file1) - GetFiletime(file2)); }
inline long FiletimeCmp(const std::string& file1, const std::string& file2) { return FiletimeCmp(file1.c_str(), file2.c_str()); }

//  Get size of open file
long fsize(FILE* fp);
//  Get size of file
long GetFilesize(const char* file);

//  Add path to filename if no path is present. Uses static string and don't chech size of 'path', be careful!
const char* AddPath(const char* path, const char* file);
inline const char* AddPath(const std::string& path, const char* file) { return AddPath(path.c_str(), file); }
inline const char* AddPath(const std::string& path, const std::string& file) { return AddPath(path.c_str(), file.c_str()); }

//  Add path to filename, if no path is set. Don't chech size of 'path', be careful!
void MakePathname(char* pathname, const char* path, const char* name);
void MakePathname(std::string& pathname, const std::string& path, const std::string& name);

//  Adds the directory-delimiter character into end of string ('\\' in DOS-based, '/' in unix-based OS)
char* AddBackslash(char* p);
std::string& AddBackslash(std::string& p);

//  Remove one trailing directory-delimiter character ('\\' in DOS-based, '/' in unix-based OS)
char* StripBackslash(char* p);

//  Copy pathname with enviroment variables substitution and adds directory delimiter char.
//  Copy not more sizeof(Path) characters (__dst should be type "Path" or equvalence, size is GMAXPATH)
char* PathCopy(char* dst, const char* src);
void PathCopy(std::string& dst, const char* src);

//  Update time of modification for the file 'filename'
void TouchFile(const TCHAR *filename);

//  Test filesystem for file locks feature
int TestLockPath(const char* __path);

//  Fill file with garbage (random byte values).
void WipeFile(const char* file, int options);

//  Return filename without path. (Return pointer to filename part of filepath.)
const char* CleanFilename(const char* __file);

//  DOS-style enviroment variables substitution in string.
int strschg_environ(char* s);
int strschg_environ(std::string& s);

char* MapPath(char* map, bool reverse = false); // gcarea.cpp
inline char* ReMapPath(char* map) { return MapPath(map, true); }

inline long lseekset(int fh, long offset) { return lseek(fh, offset, SEEK_SET); }
inline long lseekset(int fh, long record, long recordsize) { return lseek(fh, record*recordsize, SEEK_SET); }

//  OS-independent change directory
int gchdir(const char* dir);


//  ------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif


//  ------------------------------------------------------------------

#if (defined(__BORLANDC__) && defined(__OS2__)) || defined(__UNIX__) || defined(__EMX__)
long filelength(int fh);
#endif


//  ------------------------------------------------------------------

//  Replace file suffix with specified in 'ext'
void replaceextension(char *destpath, const char *srcpath, const char *ext);
//  Write to 'dir' dirname of the 'path'
void extractdirname(char *dir, const char *path);


//  ------------------------------------------------------------------

#if defined(_MSC_VER)
int lock(int handle, long offset, long length);
int unlock(int handle, long offset, long length);
#endif


//  ------------------------------------------------------------------

#if defined(__DJGPP__)
#undef sopen
#endif

#if !defined(__DJGPP__) && defined(__GNUC__)
int lock(int handle, long offset, long length);
int unlock(int handle, long offset, long length);
#if !defined(__QNXNTO__) && !defined(__MINGW32__)
inline long tell(int fh) { return lseek(fh, 0, SEEK_CUR); }
#endif
#endif

#if !defined(sopen) && !defined(__MINGW32__) && !defined(__EMX__) && !defined(__QNXNTO__) && defined(__GNUC__)
inline int sopen(const char* path, int access, int shflag, int mode) {
#ifdef __UNIX__
  shflag = 0;
#endif
  return open(path, access|shflag, mode);
}
#endif

#if defined(__UNIX__) || defined(__CYGWIN__)
inline int chsize(int handle, long size) { return ftruncate(handle, size); }
#endif


//  ------------------------------------------------------------------
//  Change dirname to real full pathname

bool maketruepath(std::string &dirname);


//  ------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
