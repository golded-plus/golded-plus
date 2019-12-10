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
#if __VISUAL_C_NOT_LESS(14, 0)
//#if defined(_MSC_VER) && (_MSC_VER >= 1400)
#include <time.h>
#endif

#include "gstrall.h"
//  ------------------------------------------------------------------
typedef long Clock;
//  ------------------------------------------------------------------

#if defined (GOLD_CANPACK)
#pragma pack(1)
#endif
//  ------------------------------------------------------------------
//  DOS "findfirst" timestamp
struct gfiletime
{
    unsigned     ft_tsec  : 5;// Second / 2
    unsigned     ft_min   : 6;// Minutes
    unsigned     ft_hour  : 5;// Hours
    unsigned     ft_day   : 5;// Days
    unsigned     ft_month : 4; // Months
    unsigned     ft_year  : 7;// Year - 80
    const char * c_str(char * buf);

dword number()
{
    return *(dword *)this;
}
};

typedef gfiletime FFTime;
//  ------------------------------------------------------------------
//  Opus DOS-style file timestamp
struct gopustime
{
    unsigned     ft_day   : 5;// Days
    unsigned     ft_month : 4; // Months
    unsigned     ft_year  : 7;// Year - 80
    unsigned     ft_tsec  : 5;// Second / 2
    unsigned     ft_min   : 6;// Minutes
    unsigned     ft_hour  : 5;// Hours
    const char * c_str(char * buf);

dword number()
{
    return *(dword *)this;
}
};

typedef gopustime FTime;
//  ------------------------------------------------------------------

#if defined (GOLD_CANPACK)
#pragma pack()
#endif
//  ------------------------------------------------------------------
//  Externs for strftimei()
extern char * __gsweekday[];
extern char * __glweekday[];
extern char * __gsmonth[];
extern char * __glmonth[];
extern char * __gampm[];
extern char * gsweekday[];
extern char * glweekday[];
extern char * gsmonth[];
extern char * glmonth[];
extern char * gampm[];
extern const char * gmonths[];
//  ------------------------------------------------------------------
//  Prototypes
inline void ggmtime(struct tm * _tm, const time32_t * timep)
{
    const time_t temp(*timep);

#if __VISUAL_C_NOT_LESS(14, 0)

//#if defined(_MSC_VER) && (_MSC_VER >= 1400)
    if(0 != gmtime_s(_tm, &temp))
    {
        const time_t zero(0);
        gmtime_s(_tm, &zero);
    }

#else
    struct tm * time = gmtime(&temp);
#if defined (__WIN32__)

    if(NULL == time)
    {
        const time_t zero(0);
        time = gmtime(&zero);
    }

#endif
    *_tm = *time;
#endif
} // ggmtime

inline void glocaltime(struct tm * _tm, const time32_t * timep)
{
    const time_t temp(*timep);

#if __VISUAL_C_NOT_LESS(14, 0)

//#if defined(_MSC_VER) && (_MSC_VER >= 1400)
    if(0 != localtime_s(_tm, &temp))
    {
        const time_t zero(0);
        localtime_s(_tm, &zero);
    }

#else
    struct tm * time = localtime(&temp);
#if defined (__WIN32__)

    if(NULL == time)
    {
        const time_t zero(0);
        time = localtime(&zero);
    }

#endif
    *_tm = *time;
#endif
} // glocaltime

inline void gctime(TCHAR * buffer, size_t sizeInChars, const time32_t * timep)
{
    const time_t temp(*timep);

#if __VISUAL_C_NOT_LESS(14, 0)

//#if defined(_MSC_VER) && (_MSC_VER >= 1400)
    if(0 != _tctime_s(buffer, sizeInChars, &temp))
    {
        const time_t zero(0);
        _tctime_s(buffer, sizeInChars, &zero);
    }

#else
    const char * time = ctime(&temp);
#if defined (__WIN32__)

    if(NULL == time)
    {
        const time_t zero(0);
        time = _tctime(&zero);
    }

#endif
    strxcpy(buffer, time, sizeInChars);
#endif
} // gctime

inline time32_t gtime(time32_t * timep)
{
    time32_t temp = (time32_t)time(NULL);

    return timep ? *timep = temp : temp;
}

inline time32_t gmktime(struct tm * timep)
{
    return (time32_t)mktime(timep);
}

#if defined (__OS2__)
inline void usleep(int duration)
{
    DosSleep(duration);
}

#elif (defined (__MINGW32__) && __GNUC_LESS(3, 4)) || defined (_MSC_VER)
//#elif (defined(__MINGW32__) && __GNUC__*100+__GNUC_MINOR__ < 304) || defined(_MSC_VER)
inline void usleep(long duration)
{
    Sleep(duration);
}

#endif

#ifndef CLK_TCK
#define CLK_TCK CLOCKS_PER_SEC
#endif

#ifdef __UNIX__
inline Clock gclock()
{
    struct tms z;

    return Clock(times(&z) * 10 / sysconf(_SC_CLK_TCK));
}

#else
inline Clock gclock()
{
    return Clock(clock() * 10 / CLK_TCK);
}

#endif

int str2mon(const char * ptr) __attribute__ ((const));
int tzoffset();

//  In differ to ANSI C strftime() the function strftimei() supports reloading of field names
// (loaded from language file).
//  Return NULL if output truncated, otherwize return s.
char * strftimei(char * s, size_t maxsize, const char * fmt, const struct tm * t); // __attribute__

// ((format
// (strftime,
// 3, 0)));
FTime TimeToFTime(time32_t __time) __attribute__ ((const));
time32_t FTimeToTime(FTime * __ftime, struct tm * __tm = NULL);
time32_t FidoTimeToUnix(char * __fidotime);
char * FTimeToStr(char * buf, FTime & t);
char * TimeToStr(char * buf, time32_t t);

//  ------------------------------------------------------------------
long YMD2JDN(unsigned yr, unsigned mo, unsigned day) __attribute__ ((const));
void JDN2YMD(long scalar, unsigned * yr, unsigned * mo, unsigned * day);

//  ------------------------------------------------------------------

#endif // ifndef __gtimall_h
//  ------------------------------------------------------------------
