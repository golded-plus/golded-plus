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

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cerrno>
#include <cstdio>
#include <string>
#include <gshare.h>
#if !defined(__UNIX__)
#include <io.h>
#endif


//  ------------------------------------------------------------------M

#if defined(__UNIX__)
#define O_TEXT 0
#define O_BINARY 0
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

inline bool isslash(char c) { return (c == '\\') || (c == '/'); }


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

FILE* fsopen(const char* path, const char* type, int shflag);
inline FILE* fsopen(const std::string& path, const char* type, int shflag) { return fsopen(path.c_str(), type, shflag); }

int is_dir(const char* path);
inline int is_dir(const std::string& path) { return is_dir(path.c_str()); }

inline bool fexist(const char* filename) { return *filename ? ((access(filename, 0) == 0) and not is_dir(filename)) : false; }
inline bool fexist(const std::string& filename) { return fexist(filename.c_str()); }

dword gfixstattime(time_t st_time);

dword GetFiletime(const char* file);
inline dword GetFiletime(const std::string& file) { return GetFiletime(file.c_str()); }

inline long FiletimeCmp(const char* file1, const char* file2) { return long(GetFiletime(file1) - GetFiletime(file2)); }
inline long FiletimeCmp(const std::string& file1, const std::string& file2) { return FiletimeCmp(file1.c_str(), file2.c_str()); }

long fsize(FILE* fp);
long GetFilesize(const char* file);

const char* AddPath(const char* path, const char* file);
inline const char* AddPath(const std::string& path, const char* file) { return AddPath(path.c_str(), file); }
inline const char* AddPath(const std::string& path, const std::string& file) { return AddPath(path.c_str(), file.c_str()); }

void MakePathname(char* pathname, const char* path, const char* name);
void MakePathname(std::string& pathname, const std::string& path, const std::string& name);

char* AddBackslash(char* p);
std::string& AddBackslash(std::string& p);
char* StripBackslash(char* p);

char* PathCopy(char* dst, const char* src);
void PathCopy(std::string& dst, const char* src);

void TouchFile(const char* __filename);

int TestLockPath(const char* __path);
void WipeFile(const char* file, int options);

const char* CleanFilename(const char* __file);

int strschg_environ(char* s);
int strschg_environ(std::string& s);

char* MapPath(char* map, bool reverse = false);
inline char* ReMapPath(char* map) { return MapPath(map, true); };

inline long lseekset(int fh, long offset) { return lseek(fh, offset, SEEK_SET); }
inline long lseekset(int fh, long record, long recordsize) { return lseek(fh, record*recordsize, SEEK_SET); }

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

void replaceextension(char *destpath, const char *srcpath, const char *ext);
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
inline long tell(int fh) { return lseek(fh, 0, SEEK_CUR); }
#endif

#if !defined(__MINGW32__) && !defined(__EMX__) && defined(__GNUC__)

inline int eof(int h) {
  return tell(h) > filelength(h);
}

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

bool maketruepath(std::string &dirname);


//  ------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
