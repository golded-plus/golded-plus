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
//  Time utility functions.
//  ------------------------------------------------------------------

#ifndef __gtimall_h
#define __gtimall_h


//  ------------------------------------------------------------------

#include <ctime>
#include <cstddef>
#include <gdefs.h>
#ifdef __UNIX__
#include <unistd.h>
#include <sys/times.h>
#endif
#ifdef __OS2__
#define INCL_BASE
#include <os2.h>
#endif
#ifdef __WIN32__
#include <windows.h>
#endif


//  ------------------------------------------------------------------

typedef long Clock;


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif


//  ------------------------------------------------------------------
//  DOS "findfirst" timestamp

struct gfiletime {
  unsigned ft_tsec  : 5;   // Second / 2
  unsigned ft_min   : 6;   // Minutes
  unsigned ft_hour  : 5;   // Hours
  unsigned ft_day   : 5;   // Days
  unsigned ft_month : 4;   // Months
  unsigned ft_year  : 7;   // Year - 80

  const char* c_str(char* buf);
  dword number() { return *(dword*)this; }
};

typedef gfiletime FFTime;


//  ------------------------------------------------------------------
//  Opus DOS-style file timestamp

struct gopustime {
  unsigned ft_day   : 5;   // Days
  unsigned ft_month : 4;   // Months
  unsigned ft_year  : 7;   // Year - 80
  unsigned ft_tsec  : 5;   // Second / 2
  unsigned ft_min   : 6;   // Minutes
  unsigned ft_hour  : 5;   // Hours

  const char* c_str(char* buf);
  dword number() { return *(dword*)this; }
};

typedef gopustime FTime;


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack()
#endif


//  ------------------------------------------------------------------
//  Externs for strftimei()

extern char* __gsweekday[];
extern char* __glweekday[];
extern char* __gsmonth[];
extern char* __glmonth[];
extern char* __gampm[];

extern char* gsweekday[];
extern char* glweekday[];
extern char* gsmonth[];
extern char* glmonth[];
extern char* gampm[];

extern const char* gmonths[];


//  ------------------------------------------------------------------
//  Prototypes

#ifdef __WIN32__
extern struct tm dummy_struct_tm;
inline struct tm* ggmtime(time_t* arg) {
  struct tm* a = gmtime(arg);
  return (a != NULL) ? a : &dummy_struct_tm;
}
inline struct tm* glocaltime(time_t* arg) {
  struct tm* a = localtime(arg);
  return (a != NULL) ? a : &dummy_struct_tm;
}
#define gmtime(arg)    ggmtime(arg)
#define localtime(arg) glocaltime(arg)
#endif

#if defined(__OS2__)
inline void usleep(long duration) { DosSleep(duration); }
#elif defined(__MINGW32__) || defined(_MSC_VER)
inline void usleep(long duration) { Sleep(duration); }
#endif

#ifndef CLK_TCK
#define CLK_TCK CLOCKS_PER_SEC
#endif

#ifdef __UNIX__
inline Clock gclock() { struct tms z; return Clock(times(&z)*10/sysconf(_SC_CLK_TCK)); }
#else
inline Clock gclock() { return Clock(clock()*10/CLK_TCK); }
#endif

int str2mon(const char* ptr) __attribute__ ((const));
int tzoffset();

char* strftimei(char* s, size_t maxsize, const char* fmt, const struct tm* t); // __attribute__ ((format (strftime, 3, 0)));

FTime  TimeToFTime(time_t __time) __attribute__ ((const));
time_t FTimeToTime(FTime* __ftime, struct tm* __tm=NULL);

time_t FidoTimeToUnix(char* __fidotime);

char* FTimeToStr(char* buf, FTime &t);
char* TimeToStr(char* buf, time_t t);


//  ------------------------------------------------------------------

long YMD2JDN(unsigned yr, unsigned mo, unsigned day) __attribute__ ((const));
void JDN2YMD(long scalar, unsigned *yr, unsigned *mo, unsigned *day);


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
