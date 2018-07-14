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

#ifndef __gfile_h
#define __gfile_h


//  ------------------------------------------------------------------

#include <gfilutil.h>


//  ------------------------------------------------------------------
//  An invalid path/filename

#define GFILE_INVALID "::INVALID::"


//  ------------------------------------------------------------------
//  Stream/Unix-style file I/O class

class gfile
{
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

  int okay() { return (0 == status); }
  bool isopen();  // true if the file is open
//  operator bool() { return isopen(); }


  //  --------------------------------------------------------------
  //  Constructors and destructor

  gfile();              // Bare constructor
//  gfile(int __fh);      // Construct from Unix file handle
//  gfile(FILE* __fp);    // Construct from ANSI stream pointer
  gfile(const char* __path, int __access, int __shflag=SH_DENYNO, int __mode=S_STDRW);
  gfile(const char* __path, const char* __mode, int __shflag=SH_DENYNO);

  ~gfile();             // Destructor (closes file)


  //  --------------------------------------------------------------
  //  Set file handle or stream pointer

  int     setfh(int __fh);
  FILE*   setfp(FILE* __fp);


  //  --------------------------------------------------------------
  //  UNIX-style raw I/O

  int     Open        (const char* __path, int __access, int __shflag=SH_DENYNO, int __mode=S_STDRW);
  int     Open        (const char* __path, int __access, const char* __fmode, int __shflag=SH_DENYNO, int __mode=S_STDRW);
  int     Close       ();

  int     Read        (void* __ptr, size_t __len);
  int     Write       (const void* __ptr, size_t __len);

  long    Tell        ();
  long    Lseek       (long __offset, int __direction);
  long    Lseek       (long __record, long __recordsize, int __direction) { return Lseek(__record*__recordsize, __direction); }
  long    LseekSet    (long __record, long __recordsize = 1) { return Lseek(__record*__recordsize, SEEK_SET); }

  long    FileLength  ();

  int     ChSize      (long __size);

  int     Lock        (long __offset, long __len);
  int     Unlock      (long __offset, long __len);

  int     GetFTime    (dword* __ftime);


  //  --------------------------------------------------------------
  //  ANSI-style streaming buffered I/O

  FILE*   Fopen   (const char* __path, const char* __mode, int __shflag=SH_DENYNO);
  FILE*   Fopen   (const std::string& __path, const char* __mode, int __shflag=SH_DENYNO) { return Fopen(__path.c_str(), __mode, __shflag); }
  FILE*   Fdopen  (const char* __mode);
  int     Fclose  ();

  FILE*   Popen   (const char* __path, const char* __mode);
  FILE*   Popen   (const std::string& __path, const char* __mode) { return Popen(__path.c_str(), __mode); }
  int     Pclose  ();

  size_t  Fread   (void* __ptr, size_t __size, size_t __items=1);
  size_t  Fwrite  (const void* __ptr, size_t __size, size_t __items=1);

  int     Fgetc   ();
  int     Fputc   (int __ch);

  char*   Fgets   (char* __str, size_t __len);
  int     Fputs   (const char* __str);

  int     Printf  (const char* __format, ...) __attribute__ ((format (printf, 2, 3)));

  int     Fflush  ();

  long    Ftell   ();
  int     Fseek   (long __offset, int __direction);
  int     Fseek   (long __record, long __recordsize, int __direction) { return Fseek(__record*__recordsize, __direction); }
  int     FseekSet(long __record, long __recordsize = 1) { return Fseek(__record*__recordsize, SEEK_SET); }

  void    Rewind  () { rewind(fp); }

  int     SetvBuf (char* __buf=NULL, int __type=_IOFBF, size_t __size=8192);
  int     SetvBuf (size_t __size) { return SetvBuf(NULL, _IOFBF, __size); }

  int     feof_   () { return feof(fp); }
  int     ferror_ () { return ferror(fp); }


  //  ----------------------------------------------------------------

  #ifdef __GOLDWARE_HAS_BOOL
  gfile& operator>> (bool& i)     { Fread(&i, sizeof(bool)); return *this; }
  #endif
  gfile& operator>> (uint8_t& i)  { Fread(&i, sizeof(uint8_t)); return *this; }
  gfile& operator>> (uint16_t& i) { Fread(&i, sizeof(uint16_t)); return *this; }
  gfile& operator>> (uint32_t& i) { Fread(&i, sizeof(uint32_t)); return *this; }
  #if !defined(__CYGWIN__)
  gfile& operator>> (unsigned long& i) { Fread(&i, sizeof(unsigned long)); return *this; }
  #endif

  gfile& operator>> (char& i)     { return operator>>((uint8_t&)i); }
#if !defined(__SUNOS__)
  gfile& operator>> (int8_t& i)   { return operator>>((uint8_t&)i); }
#endif
  gfile& operator>> (int16_t& i)  { return operator>>((uint16_t&)i); }
  gfile& operator>> (int32_t& i)  { return operator>>((uint32_t&)i); }
  #if !defined(__CYGWIN__)
  gfile& operator>> (long& i)     { return operator>>((unsigned long&)i); }
  #endif

  #ifdef __GOLDWARE_HAS_BOOL
  gfile& operator<< (bool o)      { Fwrite(&o, sizeof(o)); return *this; }
  #endif
  gfile& operator<< (uint8_t o)   { Fwrite(&o, sizeof(o)); return *this; }
  gfile& operator<< (uint16_t o)  { Fwrite(&o, sizeof(o)); return *this; }
  gfile& operator<< (uint32_t o)  { Fwrite(&o, sizeof(o)); return *this; }
  #if !defined(__CYGWIN__)
  gfile& operator<< (unsigned long o) { Fwrite(&o, sizeof(o)); return *this; }
  #endif

  gfile& operator<< (char o)      { return operator<<((uint8_t )o); }
#if !defined(__SUNOS__)
  gfile& operator<< (int8_t  o)   { return operator<<((uint8_t )o); }
#endif
  gfile& operator<< (int16_t  o)  { return operator<<((uint16_t)o); }
  gfile& operator<< (int32_t o)   { return operator<<((uint32_t)o); }
  #if !defined(__CYGWIN__)
  gfile& operator<< (long o)      { return operator<<((unsigned long)o); }
  #endif
};


//  ------------------------------------------------------------------



// file with ordered data: low byte first
class gnfile: gfile
{
//  int     Printf  (const char* __format, ...) __attribute__ ((format (printf, 2, 3)));


  size_t  FreadItem (void* __ptr, size_t __size) // Read one item size __size bytes
                                   { // please check logic of this function
                                     uint8_t *c = new uint8_t[__size];
                                     c[0]=0;
                                     size_t s = gfile::Fread(c,__size);
                                     uint32_t v=c[0];
                                     for( unsigned n=1; n<__size; n++ ) { 
                                         v += uint32_t(c[n])>>(8*n);
                                     }
                                     delete(c);
                                     memmove(&v,__ptr,__size);
                                     return s;
                                   }

  #ifdef __GOLDWARE_HAS_BOOL
  gnfile& operator>> (bool& i)     { if( sizeof(bool) ==1 ) Fread(&i, 1);
                                     else for( size_t c=0; c<sizeof(bool) ){ ((char*)i)[c] = Fgetc(); };
                                     return *this; }
  #endif
  gnfile& operator>> (uint8_t& i)  { Fread(&i, sizeof(uint8_t)); return *this; }
  gnfile& operator>> (uint16_t& i) { char c[2]; Fread(c, 2);
                                     i = uint16_t(c[0])
                                       + uint16_t((uint16_t(c[1]))>>8);
                                     return *this; }
  gnfile& operator>> (uint32_t& i) { char c[4]; Fread(c, 4);
                                     i = uint32_t(c[0])
                                       + uint32_t((uint32_t(c[1]))>>8)
                                       + uint32_t((uint32_t(c[2]))>>16)
                                       + uint32_t((uint32_t(c[3]))>>24);
                                     return *this;
                                   }
/*
  gnfile& operator>> (uint64_t& i) { char c[8]; Fread(c, 8);
                                     i = uint64_t(c[0])
                                       + uint64_t((uint64_t(c[1]))>>8)
                                       + uint64_t((uint64_t(c[2]))>>16)
                                       + uint64_t((uint64_t(c[3]))>>24)
                                       + uint64_t((uint64_t(c[4]))>>32)
                                       + uint64_t((uint64_t(c[5]))>>40)
                                       + uint64_t((uint64_t(c[6]))>>48)
                                       + uint64_t((uint64_t(c[7]))>>56);
                                     return *this;
                                   }
*/
  #if !defined(__CYGWIN__)
  gnfile& operator>> (unsigned long& i) { // FreadItem(&i, sizeof(unsigned long)); return *this; }
                                     switch(sizeof(unsigned long)) {
                                       case 1: operator>>((uint8_t&)i);
                                       case 2: operator>>((uint16_t&)i);
                                       case 4: operator>>((uint32_t&)i);
                                       // case 8: operator>>((uint64_t&)i);
                                     }
                                     return *this;
                                   }
  #endif

  gnfile& operator>> (char& i)     { return operator>>((uint8_t&)i); }
  #if !defined(__SUNOS__)
  gnfile& operator>> (int8_t& i)   { return operator>>((uint8_t&)i); }
  #endif
  gnfile& operator>> (int16_t& i)  { return operator>>((uint16_t&)i); }
  gnfile& operator>> (int32_t& i)  { return operator>>((uint32_t&)i); }
  #if !defined(__CYGWIN__)
  gnfile& operator>> (long& i)     { return operator>>((unsigned long&)i); }
  #endif

  gnfile& operator<< (uint8_t o)   { Fwrite(&o, sizeof(o)); return *this; }
  gnfile& operator<< (uint16_t o)  { uint8_t o8;
                                     o8 = uint8_t(o && 0xff); Fwrite(&o8, sizeof(o8));
                                     o8 = uint8_t((o<<8) && 0xff); Fwrite(&o8, sizeof(o8));
                                     return *this;
                                   }
  gnfile& operator<< (uint32_t o)  { uint8_t o8;
                                     o8 = uint8_t(o && 0xff); Fwrite(&o8, sizeof(o8));
                                     o8 = uint8_t((o<<8) && 0xff); Fwrite(&o8, sizeof(o8));
                                     o8 = uint8_t((o<<16) && 0xff); Fwrite(&o8, sizeof(o8));
                                     o8 = uint8_t((o<<24) && 0xff); Fwrite(&o8, sizeof(o8));
                                     return *this;
                                   }
/*
  gnfile& operator<< (uint64_t o)  { uint8_t o8;
                                     o8 = uint8_t(o && 0xff); Fwrite(&o8, sizeof(o8));
                                     o8 = uint8_t((o<<8) && 0xff); Fwrite(&o8, sizeof(o8));
                                     o8 = uint8_t((o<<16) && 0xff); Fwrite(&o8, sizeof(o8));
                                     o8 = uint8_t((o<<24) && 0xff); Fwrite(&o8, sizeof(o8));
                                     o8 = uint8_t((o<<32) && 0xff); Fwrite(&o8, sizeof(o8));
                                     o8 = uint8_t((o<<40) && 0xff); Fwrite(&o8, sizeof(o8));
                                     o8 = uint8_t((o<<48) && 0xff); Fwrite(&o8, sizeof(o8));
                                     o8 = uint8_t((o<<56) && 0xff); Fwrite(&o8, sizeof(o8));
                                     return *this;
                                   }
*/
  #if !defined(__CYGWIN__)
  gnfile& operator<< (unsigned long o) { switch (sizeof(o)) {
                                           case 1: return operator<<((uint8_t )o); break;
                                           case 2: return operator<<((uint16_t )o); break;
                                           case 4: return operator<<((uint32_t )o); break;
                                           // case 8: return operator<<((uint64_t )o); break;
                                         }
                                         return *this;
                                       }
  #endif
  #ifdef __GOLDWARE_HAS_BOOL
  gnfile& operator<< (bool o)      { switch (sizeof(o)) {
                                           case 1: return operator<<((uint8_t )o); break;
                                           case 2: return operator<<((uint16_t )o); break;
                                           case 4: return operator<<((uint32_t )o); break;
                                           // case 8: return operator<<((uint64_t )o); break;
                                     }
                                     return *this;
                                   }
  #endif

  gnfile& operator<< (char o)      { return operator<<((uint8_t )o); }
  #if !defined(__SUNOS__)
  gnfile& operator<< (int8_t  o)   { return operator<<((uint8_t )o); }
  #endif
  gnfile& operator<< (int16_t  o)  { return operator<<((uint16_t)o); }
  gnfile& operator<< (int32_t o)   { return operator<<((uint32_t)o); }
  #if !defined(__CYGWIN__)
  gnfile& operator<< (long o)      { return operator<<((unsigned long)o); }
  #endif

};

#endif

//  ------------------------------------------------------------------
