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


//  ------------------------------------------------------------------

gfile::gfile() {

  fh = -1;
  fp = NULL;
  status = EBADF;
}


//  ------------------------------------------------------------------

gfile::gfile(int __fh) {

  fh = __fh;
  fp = NULL;
  status = 0;
}


//  ------------------------------------------------------------------

gfile::gfile(FILE* __fp) {

  fh = -1;
  fp = __fp;
  status = 0;
}


//  ------------------------------------------------------------------

gfile::gfile(const char* __path, int __access, int __shflag, int __mode) {

  open(__path, __access, __shflag, __mode);
}


//  ------------------------------------------------------------------

gfile::gfile(const char* __path, const char* __mode, int __shflag) {

  fopen(__path, __mode, __shflag);
}


//  ------------------------------------------------------------------

gfile::~gfile() {

  if(fp != NULL)
    fclose();
  if(fh != -1)
    close();
}


//  ------------------------------------------------------------------

int gfile::okay() {

  return !status;
}

//  ------------------------------------------------------------------

int gfile::isopen() {

  if((fh != -1) or (fp != NULL))
    return true;
  return false;
}

//  ------------------------------------------------------------------

int gfile::setfh(int __fh) {

  fh = __fh;
  status = 0;
  return fh;
}

//  ------------------------------------------------------------------

FILE* gfile::setfp(FILE* __fp) {

  fp = __fp;
  status = 0;
  return fp;
}

//  ------------------------------------------------------------------

int gfile::open(const char* __path, int __access, int __shflag, int __mode) {

  fh = ::sopen(__path, __access, __shflag, __mode);
  status = (fh == -1) ? errno : 0;
  return fh;
}


//  ------------------------------------------------------------------

int gfile::open(const char* __path, int __access, char* __fmode, int __shflag, int __mode) {

  open(__path, __access, __shflag, __mode);
  fdopen(__fmode);
  return fh;
}

//  ------------------------------------------------------------------

int gfile::close() {

  if(fp)
    return fclose();

  int _ret = ::close(fh);
  status = _ret ? errno : 0;
  fh = -1;
  return _ret;
}

//  ------------------------------------------------------------------

int gfile::read(void* __ptr, size_t __len) {

  int _ret = ::read(fh, __ptr, (unsigned)__len);
  status = (_ret == -1) ? errno : 0;
  return _ret;
}

//  ------------------------------------------------------------------

int gfile::write(void* __ptr, size_t __len) {

  int _ret = ::write(fh, __ptr, (unsigned)__len);
  status = (_ret == -1) ? errno : 0;
  return _ret;
}

//  ------------------------------------------------------------------

long gfile::tell() {

  long _ret = ::tell(fh);
  status = (_ret == -1) ? errno : 0;
  return _ret;
}

//  ------------------------------------------------------------------

long gfile::lseek(long __offset, int __direction) {

  long _ret = ::lseek(fh, __offset, __direction);
  status = (_ret == -1) ? errno : 0;
  return _ret;
}

//  ------------------------------------------------------------------

long gfile::filelength() {

  long _ret = ::filelength(fh);
  status = (_ret == -1) ? errno : 0;
  return _ret;
}

//  ------------------------------------------------------------------

int gfile::chsize(long __size) {

  int _ret = ::chsize(fh, __size);
  status = _ret ? errno : 0;
  return _ret;
}

//  ------------------------------------------------------------------

int gfile::lock(long __offset, long __len) {

  int _ret = ::lock(fh, __offset, __len);
  status = _ret ? errno : 0;
  return _ret;
}

//  ------------------------------------------------------------------

int gfile::unlock(long __offset, long __len) {

  int _ret = ::unlock(fh, __offset, __len);
  status = _ret ? errno : 0;
  return _ret;
}

//  ------------------------------------------------------------------

int gfile::eof() {

  int _ret = ::eof(fh);
  status = (_ret == -1) ? errno : 0;
  return _ret;
}


//  ------------------------------------------------------------------

int gfile::getftime(dword* __ftime) {

  struct stat s;
  if(fp)
    fflush();
  int rv = fstat (fh, &s);
  status = (rv) ? errno : 0;
  if(rv == 0)
    *__ftime = gfixstattime(s.st_mtime);
  else
    __ftime = 0;
  return rv;
}                                            


//  ------------------------------------------------------------------

FILE* gfile::fopen(const string& __path, const char* __mode, int __shflag) {

  return fopen(__path.c_str(), __mode, __shflag);
}


//  ------------------------------------------------------------------

FILE* gfile::fopen(const char* __path, const char* __mode, int __shflag) {

  fp = ::fsopen(__path, __mode, __shflag);
  status = (fp == NULL) ? errno : 0;
  if(fp)
    fh = fileno(fp);
  return fp;
}

//  ------------------------------------------------------------------

FILE* gfile::fdopen(char* __mode) {

  fp = ::fdopen(fh, __mode);
  status = fp ? 0 : errno;
  if(fp)
    fh = fileno(fp);
  return fp;
}

//  ------------------------------------------------------------------

int gfile::fclose() {

  int _ret = 0;
  if(fp)
    _ret = ::fclose(fp);
  status = _ret ? errno : 0;
  fp = NULL;
  fh = -1;
  return _ret;
}

//  ------------------------------------------------------------------

size_t gfile::fread(void* __ptr, size_t __size, size_t __items) {

  size_t _ret = ::fread(__ptr, __size, __items, fp);
  status = ferror_() ? errno : 0;
  return _ret;
}

//  ------------------------------------------------------------------

size_t gfile::fwrite(const void* __ptr, size_t __size, size_t __items) {

  size_t _ret = ::fwrite(__ptr, __size, __items, fp);
  status = (_ret < __items) ? errno : 0;
  return _ret;
}

//  ------------------------------------------------------------------

int gfile::fgetc() {

  int _ret = ::fgetc(fp);
  status = ferror_() ? errno : 0;
  return _ret;
}

//  ------------------------------------------------------------------

int gfile::fputc(int __ch) {

  int _ret = ::fputc(__ch, fp);
  status = ferror_() ? errno : 0;
  return _ret;
}

//  ------------------------------------------------------------------

char* gfile::fgets(char* __str, size_t __len) {

  char* _ret = ::fgets(__str, __len, fp);
  status = (_ret == NULL) ? errno : 0;
  return _ret;
}

//  ------------------------------------------------------------------

int gfile::fputs(const char* __str) {

  int _ret = ::fputs(__str, fp);
  status = (_ret == EOF) ? errno : 0;
  return _ret;
}

//  ------------------------------------------------------------------

int gfile::printf(const char* __format, ...) {

  va_list _argptr;
  va_start(_argptr, __format);
  int _outcount = ::vfprintf(fp, __format, _argptr);
  va_end(_argptr);
  return _outcount;
}

//  ------------------------------------------------------------------

int gfile::fflush() {

  int _ret = ::fflush(fp);
  status = _ret ? errno : 0;
  return _ret;
}

//  ------------------------------------------------------------------

long gfile::ftell() {

  long _ret = ::ftell(fp);
  status = (_ret == -1) ? errno : 0;
  return _ret;
}

//  ------------------------------------------------------------------

int gfile::fseek(long __offset, int __direction) {

  int _ret = ::fseek(fp, __offset, __direction);
  status = _ret ? errno : 0;
  return _ret;
}

//  ------------------------------------------------------------------

int gfile::setvbuf(char* __buf, int __type, size_t __size) {

  int _ret = ::setvbuf(fp, __buf, __type, __size);
  status = _ret ? errno : 0;
  return _ret;
}


//  ------------------------------------------------------------------

#ifdef __GOLDWARE_HAS_BOOL
gfile& gfile::operator>> (bool& i)             { fread(&i, sizeof(bool)); return *this; }
#endif
gfile& gfile::operator>> (unsigned char& i)    { fread(&i, sizeof(unsigned char)); return *this; }
gfile& gfile::operator>> (unsigned short& i)   { fread(&i, sizeof(unsigned short)); return *this; }
gfile& gfile::operator>> (unsigned int& i)     { unsigned int ii; fread(&ii, sizeof(unsigned int)); i = ii; return *this; }
gfile& gfile::operator>> (unsigned long& i)    { fread(&i, sizeof(unsigned long)); return *this; }

#ifdef __GOLDWARE_HAS_BOOL
gfile& gfile::operator<< (bool o)             { fwrite(&o, sizeof(o)); return *this; }
#endif
gfile& gfile::operator<< (unsigned char o)    { fwrite(&o, sizeof(o)); return *this; }
gfile& gfile::operator<< (unsigned short o)   { fwrite(&o, sizeof(o)); return *this; }
gfile& gfile::operator<< (unsigned int o)     { fwrite(&o, sizeof(o)); return *this; }
gfile& gfile::operator<< (unsigned long o)    { fwrite(&o, sizeof(o)); return *this; }


//  ------------------------------------------------------------------
