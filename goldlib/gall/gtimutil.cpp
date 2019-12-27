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

#include <cstdarg>
#include <cstdio>
#include <gmemall.h>
#include <gstrall.h>
#include <gtimall.h>
#include <gutlmisc.h>
#include <gutlmtsk.h>

#ifdef __OS2__
    #define INCL_BASE
    #include <os2.h>
#endif

#ifdef __WIN32__
    #include <windows.h>
#endif


//  ------------------------------------------------------------------

const char* gmonths[] =
{
    "ERR",
    "Jan", "Feb", "Mar",
    "Apr", "May", "Jun",
    "Jul", "Aug", "Sep",
    "Oct", "Nov", "Dec",
    "ERR"
};


//  ------------------------------------------------------------------
//  Returns current timezone offset based on TZ environment variable.

int tzoffset()
{
    time32_t t1 = gtime(NULL);
    struct tm tp;
    ggmtime(&tp, &t1);
    tp.tm_isdst = -1;
    time32_t t2 = gmktime(&tp);
    int dt = (int)(t1 - t2);
    return (dt / 3600) * 100 + (dt % 3600) / 60;
}


//  ------------------------------------------------------------------

char* __gampm[2] =
{
    "AM", "PM"
};

char* __gsweekday[7] =
{
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};

char* __glweekday[7] =
{
    "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

char* __gsmonth[12] =
{
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

char* __glmonth[12] =
{
    "January", "February", "March",     "April",   "May",      "June",
    "July",    "August",   "September", "October", "November", "December"
};

char* gampm[2] =
{
    "AM", "PM"
};

char* gsweekday[7] =
{
    NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

char* glweekday[7] =
{
    NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

char* gsmonth[12] =
{
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

char* glmonth[12] =
{
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};


//  ------------------------------------------------------------------
//  written 6 september 1989 by jim nutt
//  released into the public domain by jim nutt
//  modified 21-Oct-89 by Rob Duff

static char buf[26];
static char format[] = "%?";

static int gpow[5] = { 1, 10, 100, 1000, 10000 };


//  ------------------------------------------------------------------
//  static void strfmt(char *str, char *fmt);
//  simple sprintf for strftime
//  each format descriptor is of the form %n
//  where n goes from zero to four
//  0    -- string %s
//  1..4 -- int %?.?d

static void strfmt(char *str, const char *fmt, ...)
{

    int ival, ilen;
    char *sval;
    va_list vp;

    va_start(vp, fmt);
    while(*fmt)
    {
        if(*fmt++ == '%')
        {
            ilen = *fmt++ - '0';
            if(ilen == 0)                 // zero means string arg
            {
                sval = va_arg(vp, char*);
                while(*sval)
                    *str++ = *sval++;
            }
            else                          // always leading zeros
            {
                if(ilen == ('-' - '0'))
                {
                    ilen = *fmt++ - '0';
                    ival = va_arg(vp, int);
                    bool padding = true;
                    while(ilen)
                    {
                        ival %= gpow[ilen];
                        int cval = ival / gpow[ilen-1];
                        if(cval)
                            padding = false;
                        if(--ilen and padding)
                            cval = ' ' - '0';
                        *str++ = (char)('0' + cval);
                    }
                }
                else
                {
                    ival = va_arg(vp, int);
                    while(ilen)
                    {
                        ival %= gpow[ilen--];
                        *str++ = (char)('0' + ival / gpow[ilen]);
                    }
                }
            }
        }
        else  *str++ = fmt[-1];
    }
    *str = '\0';
    va_end(vp);
}


//  ------------------------------------------------------------------
//  In differ to ANSI C strftime this function supports reloading of
//  field names
//  Return NULL if output truncated, otherwize return s.

#define setvar(internal,external,field) \
  char **internal = external; \
  if(internal[field] == NULL) internal = __##external;

char *strftimei(char *s, size_t maxs, const char *f, const struct tm *t)
{
    char *p, *q, *r;

    setvar(aday, gsweekday, t->tm_wday);
    setvar(day, glweekday, t->tm_wday);
    setvar(amonth, gsmonth, t->tm_mon);
    setvar(month, glmonth, t->tm_mon);

    p = s;
    q = s + maxs - 1;
    while((*f != NUL))
    {
        if(*f++ == '%')
        {
            r = buf;
            switch(*f++)
            {

            case '%':
                format[1] = NUL;
                r = format;
                break;

            case 'a':
                r = aday[t->tm_wday];
                break;

            case 'A':
                r = day[t->tm_wday];
                break;

            case 'b':
                r = amonth[t->tm_mon];
                break;

            case 'B':
                r = month[t->tm_mon];
                break;

            case 'C':
                strfmt(r, "%0 %0 %-2 %2:%2:%2 %4",
                       aday[t->tm_wday], amonth[t->tm_mon],
                       t->tm_mday,t->tm_hour, t->tm_min,
                       t->tm_sec, t->tm_year+1900);
                break;

            case 'e':
                strfmt(r, "%-2", t->tm_mday);
                break;

            case 'E':
                sprintf(r, "%u", (uint)t->tm_mday);
                break;

            case 'p':
                r = gampm[(t->tm_hour>11)?1:0];
                break;

            default:
                format[1] = f[-1];
                if(strftime(r, 26, format, t) == 0)
                {
                    buf[0] = '%';   // reconstruct the format
                    buf[1] = f[-1];
                    buf[2] = '\0';
                    if(buf[1] == 0)
                        f--;          // back up if at end of string
                }
                break;
            }
            while(*r)
            {
                if(p == q)
                {
                    *q = '\0';
                    return 0;
                }
                *p++ = *r++;
            }
        }
        else
        {
            if(p == q)
            {
                *q = '\0';
                return 0;
            }
            *p++ = f[-1];
        }
    }
    *p = '\0';
    return s;
}


//  ------------------------------------------------------------------
//  Convert string-month to integer

int str2mon(const char* ptr)
{

    int mon;

    switch(g_toupper(*ptr))
    {
    case 'A':
        if(g_toupper(ptr[1]) == 'P')
            mon = 4;
        else
            mon = 8;
        break;
    case 'D':
        mon = 12;
        break;
    case 'F':
        mon = 2;
        break;
    case 'J':
        if(g_toupper(ptr[1]) == 'A')
            mon = 1;
        else if(g_toupper(ptr[2]) == 'L')
            mon = 7;
        else
            mon = 6;
        break;
    case 'M':
        if(g_toupper(ptr[2]) == 'R')
            mon = 3;
        else
            mon = 5;
        break;
    case 'N':
        mon = 11;
        break;
    case 'O':
        mon = 10;
        break;
    case 'S':
        mon = 9;
        break;
    default:
        mon = 0;
    }

    return mon;
}


//  ------------------------------------------------------------------

time32_t FTimeToTime(FTime* __ftime, struct tm* __tm)
{

    struct tm _tm;
    uint32_t _time = 0;

    if(__tm == NULL)
        __tm = &_tm;

    // Only try to convert a valid date
    if(__ftime->ft_year >= 5)     // FidoNet standards didn't exist before 1985
    {
        if((__ftime->ft_day >= 1) and (__ftime->ft_day <= 31))
        {
            if((__ftime->ft_month >= 1) and (__ftime->ft_month <= 12))
            {

                __tm->tm_year  = __ftime->ft_year + 80;
                __tm->tm_mon   = __ftime->ft_month - 1;
                __tm->tm_mday  = __ftime->ft_day;
                __tm->tm_hour  = __ftime->ft_hour;
                __tm->tm_min   = __ftime->ft_min;
                __tm->tm_sec   = __ftime->ft_tsec * 2;
                __tm->tm_isdst = -1;

                time32_t a  = gmktime(__tm);
                struct tm tp;
                ggmtime(&tp, &a);
                tp.tm_isdst = -1;
                time32_t b  = gmktime(&tp);
                _time = a + a - b;

                if(_time == (uint32_t)0xFFFFFFFFL)
                    _time = 0;
            }
        }
    }

    return _time;
}


//  ------------------------------------------------------------------

FTime TimeToFTime(time32_t __time)
{

    FTime _ft;
    memset(&_ft, 0, sizeof(FTime));

    if (__time)
    {
        struct tm _tmp;
        ggmtime(&_tmp, &__time);
        _ft.ft_year  = (word)(_tmp.tm_year - 80);
        _ft.ft_month = (word)(_tmp.tm_mon + 1);
        _ft.ft_day   = (word)(_tmp.tm_mday);
        _ft.ft_hour  = (word)(_tmp.tm_hour);
        _ft.ft_min   = (word)(_tmp.tm_min);
        _ft.ft_tsec  = (word)(_tmp.tm_sec / 2);
    }

    return _ft;
}


//  ------------------------------------------------------------------

time32_t FidoTimeToUnix(char* ptr)
{

    bool date_ok = false;
    int year=0, month=0, day=0;
    int hour=0, minute=0, second=0;

    ptr = strskip_wht(ptr);
    if(not isdigit(*ptr))
    {
        // Skip past weekday string (SEA format)
        ptr = strskip_wht(strskip_txt(ptr));
    }
    if(*ptr)
    {
        if(isdigit(*ptr))
        {
            day = atoi(ptr);
            ptr = strskip_wht(strskip_txt(ptr));
            if(g_isalpha(*ptr))
            {
                month = str2mon(ptr);
                if(month)
                {
                    ptr = strskip_wht(strskip_txt(ptr));
                    if(isdigit(*ptr))
                    {
                        year = atoi(ptr);
                        ptr = strskip_wht(strskip_txt(ptr));
                        if(isdigit(*ptr))
                        {
                            hour = atoi(ptr);
                            ptr = strskip_digits(ptr);
                            if(*ptr and isdigit(ptr[1]))
                            {
                                minute = atoi(++ptr);
                                date_ok = true;
                                // The seconds part is only in the FTS-1 format
                                ptr = strskip_digits(ptr);
                                if(*ptr and isdigit(ptr[1]))
                                {
                                    second = atoi(++ptr);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Convert datetime to UNIX timestamp
    if(date_ok)
    {
        struct tm t;
        t.tm_year  = (year < 80) ? (year+100) : year;
        t.tm_mon   = month - 1;
        t.tm_mday  = day;
        t.tm_hour  = hour;
        t.tm_min   = minute;
        t.tm_sec   = second;
        t.tm_isdst = -1;
        time32_t a  = gmktime(&t);
        struct tm tp;
        ggmtime(&tp, &a);
        tp.tm_isdst = -1;
        time32_t b  = gmktime(&tp);
        return a + a - b;
    }
    return (uint32_t)-1;
}


//  ------------------------------------------------------------------

char* TimeToStr(char* buf, time32_t t)
{
    struct tm tm;
    ggmtime(&tm, &t);
    return strftimei(buf, 20, "%Y-%m-%d %H:%M:%S", &tm);
}


//  ------------------------------------------------------------------

char* FTimeToStr(char* buf, FTime& t)
{

    sprintf(buf, "%04u-%02u-%02u %02u:%02u:%02u",
            t.ft_year+1980, t.ft_month, t.ft_day,
            t.ft_hour, t.ft_min, t.ft_tsec*2
           );
    return buf;
}


//  ------------------------------------------------------------------

const char* gfiletime::c_str(char* buf)
{

    sprintf(buf, "%04u-%02u-%02u %02u:%02u:%02u",
            ft_year+1980, ft_month, ft_day,
            ft_hour, ft_min, ft_tsec*2
           );
    return buf;
}


//  ------------------------------------------------------------------

const char* gopustime::c_str(char* buf)
{

    sprintf(buf, "%04u-%02u-%02u %02u:%02u:%02u",
            ft_year+1980, ft_month, ft_day,
            ft_hour, ft_min, ft_tsec*2
           );
    return buf;
}


//  ------------------------------------------------------------------
