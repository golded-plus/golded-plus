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

#ifndef __gfilbase_h
#define __gfilbase_h


//  ------------------------------------------------------------------

#include <gfilutil.h>


//  ------------------------------------------------------------------
//  An invalid path/filename

#define GFILE_INVALID "::INVALID::"


//  ------------------------------------------------------------------
//  Stream/Unix-style file I/O class

class gfile {

public:

  //  ----------------------------------------------------------------
  //  Internal variables

  int   fh;       // File handle for Unix I/O
  FILE* fp;       // File pointer for ANSI stream I/O


  //  --------------------------------------------------------------
  //  State variables

  int status;     // Contains last errno value


  //  --------------------------------------------------------------
  //  Handy utility functions

  int okay();    // Returns non-zero if no errors were detected
  int isopen();  // true if the file is open


  //  --------------------------------------------------------------
  //  Constructors and destructor

  gfile();              // Bare constructor
  gfile(int __fh);      // Construct from Unix file handle
  gfile(FILE* __fp);    // Construct from ANSI stream pointer
  gfile(const char* __path, int __access, int __shflag=SH_DENYNO, int __mode=S_IREAD|S_IWRITE);
  gfile(const char* __path, const char* __mode, int __shflag=SH_DENYNO);

  ~gfile();             // Destructor (closes file)

  operator bool() { return isopen() ? true : false; }


  //  --------------------------------------------------------------
  //  Set file handle or stream pointer

  int     setfh(int __fh);
  FILE*   setfp(FILE* __fp);


  //  --------------------------------------------------------------
  //  UNIX-style raw I/O

  int     open        (const char* __path, int __access, int __shflag=SH_DENYNO, int __mode=S_IREAD|S_IWRITE);
  int     open        (const char* __path, int __access, char* __fmode, int __shflag=SH_DENYNO, int __mode=S_IREAD|S_IWRITE);
  int     close       ();

  int     read        (void* __ptr, size_t __len);
  int     write       (void* __ptr, size_t __len);

  long    tell        ();
  long    lseek       (long __offset, int __direction);
  long    lseek       (long __record, long __recordsize, int __direction);
  long    lseekset    (long __record, long __recordsize=1);

  long    filelength  ();

  int     chsize      (long __size);

  int     lock        (long __offset, long __len);
  int     unlock      (long __offset, long __len);

  int     getftime    (dword* __ftime);


  //  --------------------------------------------------------------
  //  ANSI-style streaming buffered I/O

  FILE*   fopen   (const char* __path, const char* __mode, int __shflag=SH_DENYNO);
  FILE*   fopen   (const std::string& __path, const char* __mode, int __shflag=SH_DENYNO);
  FILE*   fdopen  (char* __mode);
  int     fclose  ();

  size_t  fread   (void* __ptr, size_t __size, size_t __items=1);
  size_t  fwrite  (const void* __ptr, size_t __size, size_t __items=1);

  int     fgetc   ();
  int     fputc   (int __ch);

  char*   fgets   (char* __str, size_t __len);
  int     fputs   (const char* __str);

  int     printf  (const char* __format, ...) __attribute__ ((format (printf, 2, 3)));

  int     fflush  ();

  long    ftell   ();
  int     fseek   (long __offset, int __direction);
  int     fseek   (long __record, long __recordsize, int __direction);
  int     fseekset(long __record, long __recordsize=1);

  void    rewind  ();

  int     setvbuf (char* __buf=NULL, int __type=_IOFBF, size_t __size=8192);
  int     setvbuf (size_t __size) { return setvbuf(NULL, _IOFBF, __size); }

  int     feof_   ();
  int     ferror_ ();


  //  ----------------------------------------------------------------

  #ifdef __GOLDWARE_HAS_BOOL
  gfile& operator>> (bool& i);
  #endif
  gfile& operator>> (unsigned char& i);
  gfile& operator>> (unsigned short& i);
  gfile& operator>> (unsigned int& i);
  gfile& operator>> (unsigned long& i);
  gfile& operator>> (char& i)        { return operator>>((unsigned char&)i); }
  gfile& operator>> (signed char& i) { return operator>>((unsigned char&)i); }
  gfile& operator>> (short& i)       { return operator>>((unsigned short&)i); }
  gfile& operator>> (int& i)         { return operator>>((unsigned int&)i); }
  gfile& operator>> (long& i)        { return operator>>((unsigned long&)i); }

  #ifdef __GOLDWARE_HAS_BOOL
  gfile& operator<< (bool o);
  #endif
  gfile& operator<< (unsigned char o);
  gfile& operator<< (unsigned short o);
  gfile& operator<< (unsigned int o);
  gfile& operator<< (unsigned long o);
  gfile& operator<< (char o)        { return operator<<((unsigned char)o); }
  gfile& operator<< (signed char o) { return operator<<((unsigned char)o); }
  gfile& operator<< (short o)       { return operator<<((unsigned short)o); }
  gfile& operator<< (int o)         { return operator<<((unsigned int)o); }
  gfile& operator<< (long o)        { return operator<<((unsigned long)o); }

};


//  ------------------------------------------------------------------
//  Inline implementations


//  ------------------------------------------------------------------

inline long gfile::lseek(long __record, long __recordsize, int __direction) {

  return lseek(__record*__recordsize, __direction);
}


//  ------------------------------------------------------------------

inline long gfile::lseekset(long __record, long __recordsize) {

  return lseek(__record*__recordsize, SEEK_SET);
}


//  ------------------------------------------------------------------

inline int gfile::ferror_() {

  return ferror(fp);
}


//  ------------------------------------------------------------------

inline int gfile::fseek(long __record, long __recordsize, int __direction) {

  return fseek(__record*__recordsize, __direction);
}


//  ------------------------------------------------------------------

inline int gfile::fseekset(long __record, long __recordsize) {

  return fseek(__record*__recordsize, SEEK_SET);
}


//  ------------------------------------------------------------------

inline void gfile::rewind() {

  ::rewind(fp);
}


//  ------------------------------------------------------------------

inline int gfile::feof_() {

  return feof(fp);
}


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
