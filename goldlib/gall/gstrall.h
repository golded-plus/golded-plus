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
//  String manipulation functions.
//  ------------------------------------------------------------------

#ifndef __gstrall_h
#define __gstrall_h


//  ------------------------------------------------------------------
//  Required headers

#include <string>
#include <vector>
#include <stdlib.h>
#include <cstring>
#if defined(HAVE_STDARG_H)
    #include <stdarg.h>
#endif

#include <gctype.h>
#include <gdefs.h>

//  ------------------------------------------------------------------

#if defined(__EMX__)
    //old EMX 0.9d code
    //#include <sys/nls.h>
    //#define strupr(s) (char *)_nls_strupr((uint8_t *)(s))
    //#define strlwr(s) (char *)_nls_strlwr((uint8_t *)(s))
    //old EMX 0.9d code
#elif defined(__GNUC__)
    #if defined(__QNXNTO__) || defined(__SUNOS__)
        #include <strings.h>
    #endif
    #define stricmp strcasecmp
    #define strnicmp strncasecmp
    char* strupr(char* s);
    char* strlwr(char* s);
#endif

//  ------------------------------------------------------------------

#if !defined(HAVE_SNPRINTF)
    #ifdef HAVE_STDARG_H
        int snprintf (char *str,size_t count,const char *fmt,...);
    #else
        int snprintf (va_list);
    #endif
#endif
#if !defined(HAVE_VSNPRINTF)
    int vsnprintf( char *buffer, size_t sizeOfBuffer, const char *format, va_list argptr );
#endif


//  ------------------------------------------------------------------

typedef std::vector<std::string> gstrarray;


//  ------------------------------------------------------------------
//  Function prototypes

bool strblank(const char* str);
size_t strchg(char *str, char oldch, char newch);
size_t strchg(std::string &str, char oldch, char newch);
char* stridela(const char* substr, char* str);
int strnicmpw(const char* str1, const char* str2, int len);
const char* striinc(const char* str1, const char* str2);
char* strins(const char* instr, char* str, int st_pos);
char* strisrep(char* str, const char* search, const char* replace);
char* strischg(char* str, const char* find, const char* replace);
void  strischg(std::string &str, const char* find, const char* replace);
char* strrjust(char* str);
char* strschg(char* str, const char* find, const char* replace);
char* strsetsz(char* str, int newsize);
char* strshl(char* str, int count);
char* strshr(char* str, int count);
char* strsrep(char* str, const char* search, const char* replace);
char* strltrim(char* str);
char* strtrim(char* str);
std::string &strtrim(std::string &str);
std::string &strltrim(std::string &str);
char* struplow(char* str);

char* longdotstr(long num);                               // Convert a long to a dotted string: xxx.xxx.xxx.xxx
char* longdotstr(char* str, long num);                    // Convert a long to a dotted string: xxx.xxx.xxx.xxx
char* StripQuotes(char* str);

// Safe versions of strcpy, strcat, sequencial strcat...
TCHAR *strxcpy(TCHAR *d, const TCHAR *s, size_t n);
// strxcpy: copy not more n-1 bytes of "s" into "d", insert '\0' into end of string.
//          return d
char* strxcat(char *dest, const char *src, size_t max);
char* strxmerge(char *dest, size_t max, ...);

char* strc2p(char* str);
char* strnp2c(char* str, int n);
char* strnp2cc(char* dest, const char* str, int n);
char* strp2c(char* str);

const char* strlword(const char* str, const char *separator=NULL);
const char* strrword(const char* str, const char *separator=NULL);

char* strrevname(char* reversedname, const char* name);
char* strunrevname(char* unreversedname, const char* name);


//  ------------------------------------------------------------------
//  Inliners

#define STRNP2C(str) strnp2c(str, sizeof(str)-1)

inline char* strbtrim(char* st)
{
    return strtrim(strltrim(st));
}

inline bool streql  (const TCHAR *str1, const TCHAR *str2)
{
    return (0 == strcmp  (str1, str2));
}
inline bool strieql (const TCHAR *str1, const TCHAR *str2)
{
    return (0 == stricmp (str1, str2));
}
inline bool strneql (const TCHAR *str1, const TCHAR *str2, int n)
{
    return (0 == strncmp (str1, str2, n));
}
inline bool strnieql(const TCHAR *str1, const TCHAR *str2, int n)
{
    return (0 == strnicmp(str1, str2, n));
}

inline const char* strskip_to(const char* p, char* s)
{
    return p+strcspn(p, s);
}
inline char* strskip_to(char* p, char* s)
{
    return p+strcspn(p, s);
}

inline const char* strskip_to(const char* p, char c)
{
    while((*p != c) and *p) p++;
    return p;
}
inline char* strskip_to(char* p, char c)
{
    while((*p != c) and *p) p++;
    return p;
}

inline const char* strskip_txt(const char* p)
{
    while(*p and not isspace(*p)) p++;
    return p;
}
inline char* strskip_txt(char* p)
{
    while(*p and not isspace(*p)) p++;
    return p;
}

inline const char* strskip_wht(const char* p)
{
    while(*p and isspace(*p)) p++;
    return p;
}
inline char* strskip_wht(char* p)
{
    while(*p and isspace(*p)) p++;
    return p;
}

inline const char* strskip_digits(const char* p)
{
    return p+strspn(p, "0123456789");
}
inline char* strskip_digits(char* p)
{
    return p+strspn(p, "0123456789");
}

#if defined(_MSC_VER) || (defined(__GNUC__) && (defined(__WIN32__) || defined(__SUNOS__))) || defined(__WATCOMC__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
inline char * stpcpy(char* dest, const char* src)
{
    while ((*dest++ = *src++) != NUL) {}
    return --dest;
}
#endif

#ifndef isoctal
    #define isoctal(c) (((c) >= '0') and ((c) <= '7'))
#endif

char* strcvtc(char* s);

//  ------------------------------------------------------------------

#define PRINTF_DECLARE_BUFFER(b) b, ARRAYSIZE(b), __FILE__, __LINE__
int gsprintf(TCHAR* buffer, size_t sizeOfBuffer, const TCHAR* __file, int __line, const TCHAR* format, ...);

//  ------------------------------------------------------------------

std::string &FormatString(std::string &format, const char *token, const char *replace);
std::string &FormatString(std::string &format, const char *token, size_t replace);

//  ------------------------------------------------------------------
//  String tokenizer class

void tokenize(gstrarray &array, const TCHAR* str, const TCHAR *delim = NULL);

class GTok
{
protected:
    const TCHAR *separator;
    TCHAR *token;
#if defined(_tcstok_s)
    TCHAR *next_token;
#endif

public:
    GTok(TCHAR *sep = NULL);

#if defined(_tcstok_s)
    TCHAR *First(TCHAR *buf)
    {
        token = _tcstok_s(buf,  separator, &next_token);
        return token;
    }
    TCHAR *Next()
    {
        token = _tcstok_s(NULL, separator, &next_token);
        return token;
    }
#else
    TCHAR *First(TCHAR *buf)
    {
        token = strtok(buf,  separator);
        return token;
    }
    TCHAR *Next()
    {
        token = strtok(NULL, separator);
        return token;
    }
#endif
    TCHAR *Token()
    {
        return token;
    }
};


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
