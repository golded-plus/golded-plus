
//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999-2000 Alexander S. Aganichev
//  ------------------------------------------------------------------
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License as
//  published by the Free Software Foundation; either version 2 of the
//  License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//  ------------------------------------------------------------------
//  $Id$
//  ------------------------------------------------------------------
//  Based on source from the CXL library by Mike Smedley.
//  String manipulation.
//  ------------------------------------------------------------------

#include <gctype.h>
#include <cstdio>
#include <stdarg.h>

// -------------------------------------------------------------------
// snprintf() and vsnprintf()

#if  defined(_MSC_VER) && _MSC_VER<1900
/* It is need a workaround for implementation "speciality" in snprintf() and vsnprintf() from Microsoft. */
int snprintf( char *buffer, size_t sizeOfBuffer, const char *format, ... )
{
    va_list argptr;
    va_start(argptr, format);
    int r = _vsnprintf( buffer, sizeOfBuffer, format, argptr );
    if( r == -1 || r >= sizeOfBuffer )
        buffer[sizeOfBuffer-1] = '\0';
    va_end(argptr);
    return r;
}
# define HAVE_SNPRINTF 1
int vsnprintf( char *buffer, size_t sizeOfBuffer, const char *format, va_list argptr )
{
    int r = _vsnprintf( buffer, sizeOfBuffer, format, argptr );
    if( r == -1 || r >= sizeOfBuffer )
        buffer[sizeOfBuffer-1] = '\0';
    return r;
}
# define HAVE_VSNPRINTF 1
#endif

#ifndef HAVE_SNPRINTF
    #if defined(HAVE__SNPRINTF)
        #define snprintf _snprintf
        #define HAVE_SNPRINTF 1
    #endif
#endif
#ifndef HAVE_VSNPRINTF
    #if defined(HAVE__VSNPRINTF)
        #define vsnprintf _vsnprintf
        #define HAVE_VSNPRINTF 1
    #endif
#endif

#if !defined(HAVE_SNPRINTF) || !defined(HAVE_VSNPRINTF)
    #include "snprintf.c"
    #if !defined(HAVE__SNPRINTF)
        #define HAVE_SNPRINTF 1
    #endif
    #if !defined(HAVE__VSNPRINTF)
        #define HAVE_VSNPRINTF 1
    #endif
#endif

//--------------------------------------------------------------------

#include <gstrall.h>
#include <glog.h>
#include <gdbgerr.h>

extern glog LOG;


//  ------------------------------------------------------------------
//  Determines if a string is blank

bool strblank(const char* str)
{

    const char* p;

    for(p = str; *p; p++)
        if(not isspace(*p))
            return false;

    return true;
}


//  ------------------------------------------------------------------
//  Changes all occurrences of one character to another

size_t strchg(char *str, char oldch, char newch)
{
    size_t count = 0;

    for (char *p = str; *p; p++)
    {
        if (oldch == *p)
        {
            *p = newch;
            count++;
        }
    }

    return count;
}


//  ------------------------------------------------------------------
//  Changes all occurrences of one character to another

size_t strchg(std::string &str, char oldch, char newch)
{
    size_t count = 0;

    std::string::iterator it = str.begin();
    std::string::iterator end = str.end();
    for (; it != end; it++)
    {
        if (oldch == *it)
        {
            *it = newch;
            count++;
        }
    }

    return count;
}


//  ------------------------------------------------------------------
//  Deletes a substring from within a string

static char* strdel(const char* substr, char* str)
{

    char* dest = strstr(str, substr);
    if(!dest)
        return NULL;
    char* src = dest + strlen(substr);
//  strcpy(dest, src);  /* Source and destination overlap. This is rigth. */
    memmove(dest, src, strlen(src)+1);

    return str;
}


//  ------------------------------------------------------------------
//  Deletes a substring from within a string, ignores case

static char* stridel(const char* substr, char* str)
{

    char* dest = (char*)striinc(substr, str);
    if(!dest)
        return NULL;
    char* src = dest + strlen(substr);
//  strcpy(dest, src);
    memmove(dest, src, strlen(src)+1);  /* Source and destination overlaps. */

    return str;
}


//  ------------------------------------------------------------------
//  Deletes all occurrences of one string inside another

char* stridela(const char* substr, char* str)
{

    int count = 0;
    char* p = str;

    while((p = (char*)striinc(substr, p)) != NULL)
    {
        stridel(substr, p);
        count++;
    }

    if(count)
        return str;
    return NULL;
}


//  ------------------------------------------------------------------
//  Compare two strings, allowing wildcards

int strnicmpw(const char* str1, const char* str2, int len)
{

    int cmp = 0;

    for(int n=0; n<len; n++)
    {
        // Single char match?
        if(str1[n] == '?')
            continue;
        // Matches rest of string?
        else if(str1[n] == '*')
            return 0;
        // Compare chars
        else if((cmp = compare_two(g_toupper(str1[n]), g_toupper(str2[n]))) != 0)
            return cmp;
    }

    return cmp;
}


//  ------------------------------------------------------------------
//  Determines if string1 is included in string2

const char* striinc(const char* str1, const char* str2)
{

    int max = strlen(str1);

    for(const char* p=str2; *p; p++)
        if(!strnicmp(str1,p,max))
            return p;

    return NULL;                         // string1 not found in string2
}


//  ------------------------------------------------------------------
//  Inserts one string into another

char* strins(const char* instr, char* str, int st_pos)
{

    int i, leninstr;

    // get length of string to insert
    leninstr = strlen(instr);

    // shift affected portion of str text to the right
    for(i=strlen(str); i>=st_pos; i--)
        *(str+leninstr+i) = *(str+i);

    // insert instr text
    for(i=0; i<leninstr; i++)
        *(str+st_pos+i) = *(instr+i);

    // return address of modified string
    return str;
}


//  ------------------------------------------------------------------
//  String search and replace, case insensitive

char* strisrep(char* str, const char* search, const char* replace)
{

    char* p;

    if((p = (char*)striinc(search,str)) != NULL)
    {
        stridel(search, str);
        strins(replace, str, (int)(p-str));
        p = str;
    }

    return p;
}


//  ------------------------------------------------------------------
//  Changes all occurrences of one string to another

char* strischg(char* str, const char* find, const char* replace)
{

    int count = 0;
    char* p = str;

    int len = strlen(replace);
    while((p = (char*)striinc(find, p)) != NULL)
    {
        strisrep(p, find, replace);
        p += len;
        count++;
    }

    if(count)
        return str;
    return NULL;
}


//  ------------------------------------------------------------------
//  Changes all occurrences of one string to another

void strischg(std::string &str, const char* find, const char* replace)
{
    size_t lenf = strlen(find);
    size_t lenr = strlen(replace);
    size_t length = str.length();

    for (size_t pos = 0; pos < length; )
    {
        if (strnieql(&(str.c_str()[pos]), find, lenf))
        {
            str.replace(pos, lenf, replace, lenr);
            pos += lenr;
            length = str.length();
        }
        else
            pos++;
    }
}


//  ------------------------------------------------------------------
//  Takes a long number and makes a string with the form x.xxx.xxx.xxx

char* longdotstr(long num)
{

    // 1234567890123
    // 4.294.967.296

    static char buf[15];
    return longdotstr(buf, num);
}


//  ------------------------------------------------------------------
//  Takes a long number and makes a string with the form x.xxx.xxx.xxx

char* longdotstr(char* str, long num)
{

    char tmp[20], pos=0;

    char* out = str;
    char* ptr = tmp;

    do
    {
        if(pos == 3 or pos == 6 or pos == 9)
            *ptr++ = '.';
        pos++;
        *ptr++ = (char)(num % 10 + '0');
    }
    while((num = num/10) > 0);

    while(ptr-- > tmp)
        *str++ = *ptr;
    *str = NUL;

    return out;
}


//  ------------------------------------------------------------------

char* strp2c(char* str)
{

    int len = *str;

    memmove(str, str+1, len);       // Copy data part
    str[len] = NUL;                 // Set length
    return str;
}


//  ------------------------------------------------------------------

char* strnp2c(char* str, int n)
{

    int len = (n < *str) ? n : *str;

    memmove(str, str+1, len);     // Copy data part
    str[len] = NUL;               // Set length
    return str;
}


//  ------------------------------------------------------------------

char* strnp2cc(char* dest, const char* str, int n)
{

    int len = (n < *str) ? n : *str;

    memcpy(dest, str+1, len);           // Copy data part
    dest[len] = NUL;                    // Set length
    return dest;
}


//  ------------------------------------------------------------------

char* strc2p(char* str)
{

    char len = (char)strlen(str);

    memmove(str+1, str, len);       // Copy data part
    *str = len;                     // Set length
    return str;
}


//  ------------------------------------------------------------------
//  Strip the quotes off a quoted string ("" or '')

char* StripQuotes(char* str)
{

    int len;

    switch(*str)
    {
    case '\'':
    case '\"':
        len = strlen(str);
        switch(*(str+len-1))
        {
        case '\'':
        case '\"':
            memmove(str, str+1, len);
            str[len-2] = NUL;
        }
    }
    return str;
}


//  ------------------------------------------------------------------
//  Right justifies a string

char* strrjust(char* str)
{

    char* p;
    char* q;

    for(p=str; *p; p++)
        ;   // find end of string
    p--;
    for(q=p; isspace(*q) and q>=str; q--)
        ;   // find last non-space character
    if(p != q)
    {
        while(q >= str)
        {
            *p-- = *q;
            *q-- = ' ';
        }
    }
    return str;
}


//  ------------------------------------------------------------------
//  Changes all occurrences of one string to another

char* strschg(char* str, const char* find, const char* replace)
{

    int count = 0;
    char* p = str;

    int len = strlen(replace);
    while((p = strstr(p, find)) != NULL)
    {
        strsrep(p, find, replace);
        p += len;
        count++;
    }

    if(count)
        return str;
    return NULL;
}


//  ------------------------------------------------------------------
//  Adjusts the size of a string
//  NOTE Buffer shall be at least one byte longer than newsize!

char* strsetsz(char* str, int newsize)
{

    int len = strlen(str);
    if(newsize < len)
        *(str+newsize) = NUL;
    else
    {
        memset(str+len, ' ', newsize-len);
        *(str+newsize) = NUL;
    }

    return str;
}


//  ------------------------------------------------------------------
//  Shifts a string left

char* strshl(char* str, int count)
{

    int i, j;

    if(*str)
    {
        for(j=0; j<count; j++)
        {
            for(i=0; *(str+i); i++)
                *(str+i) = *(str+i+1);
            *(str+i-1) = ' ';
        }
    }

    return str;
}


//  ------------------------------------------------------------------
//  Shifts a string right

char* strshr(char* str, int count)
{

    int i, j, len;

    if(*str)
    {
        len = strlen(str)-1;
        for(j=0; j<count; j++)
        {
            for(i=len; i>0; i--)
                *(str+i) = *(str+i-1);
            *(str) = ' ';
        }
    }

    return str;
}


//  ------------------------------------------------------------------
//  String search and replace, case sensitive

char* strsrep(char* str, const char* search, const char* replace)
{

    char* p;

    if((p = strstr(str, search)) != NULL)
    {
        strdel(search, str);
        strins(replace, str, (int)(p-str));
        p = str;
    }

    return p;
}


//  ------------------------------------------------------------------
//  Trims trailing spaces off of a string

char* strtrim(char* p)
{

    int i;
    for(i = strlen(p) - 1; (i >= 0) and (isspace(p[i]) or iscntrl(p[i])); i--) {}
    p[i + 1] = NUL;
    return p;
}


std::string &strtrim(std::string &str)
{
    if (!str.empty())
    {
        std::string::iterator begin = str.begin();
        std::string::iterator trail = str.end();

        while (trail != begin)
        {
            --trail;
            if (not isspace(*trail) and not iscntrl(*trail))
            {
                ++trail;
                break;
            }
        }

        str.erase(trail, str.end());
    }

    return str;
}


//  ------------------------------------------------------------------
//  Trims leading spaces off of a string

char* strltrim(char* str)
{
    char* p;
    char* q;

    p = q = str;
    while(*p and (isspace(*p) or iscntrl(*p)))
        p++;

    if(p != q)
    {
        while(*p)
            *q++ = *p++;
        *q = NUL;
    }

    return str;
}

//  ------------------------------------------------------------------

std::string &strltrim(std::string &str)
{
    if (!str.empty())
    {
        std::string::iterator begin = str.begin();
        std::string::iterator end = str.end();
        std::string::iterator it = begin;

        for (; (it != end) && isspace(*it); it++) { /**/ }
        if (it != begin) str.erase(begin, it);
    }

    return str;
}

//  ------------------------------------------------------------------

const char* strlword(const char* str, const char *separator)
{

    char buf[256];
    static char left[40];

    *left = NUL;
    if(*str)
    {
        strxcpy(buf, str, sizeof(buf));
        if(strtok(buf, (separator == NULL) ? " \t\n\r" : separator) != NULL)
        {
            strxcpy(left, buf, sizeof(left));
        }
    }
    return left;
}


//  ------------------------------------------------------------------

const char* strrword(const char* str, const char *separator)
{

    char* ptr;
    char* ptr2;
    char buf[256];
    static char right[40];

    *right = NUL;
    if(*str)
    {
        strxcpy(buf, str, sizeof(buf));
        if(separator == NULL)
        {
            separator = " \t\n\r";
        }
        ptr = strtok(buf, separator);
        ptr2 = ptr;
        while(ptr != NULL)
        {
            ptr2 = ptr;
            ptr = strtok(NULL, separator);
        }
        if(ptr2)
        {
            strxcpy(right, ptr2, sizeof(right));
        }
    }
    return right;
}


//  ------------------------------------------------------------------
//  copy not more n-1 bytes of "s" into "d", insert '\0' into end of string.
//  return d
TCHAR *strxcpy(TCHAR *d, const TCHAR *s, size_t n)
{
#if defined(_tcsncpy_s)
    _tcsncpy_s(d, n, s, _TRUNCATE);
#else
    if (n)
    {
        strncpy(d, s, n-1);
        d[n-1] = NUL;
    }
    else
        *d = NUL;
#endif
    return d;
}


//  ------------------------------------------------------------------

char *strxcat(char *dest, const char *src, size_t max)
{
    while (*dest and (max > 0))
    {
        --max;
        dest++;
    }
    while (*src and (max > 0))
    {
        --max;
        *dest++ = *src++;
    }
    *dest = NUL;
    return dest;
}


//  ------------------------------------------------------------------

char *strxmerge(char *dest, size_t max, ...)
{
    va_list a;
    va_start(a, max);
    for(; max > 0;)
    {
        const char *src = va_arg(a, const char *);
        if (src == NULL)
            break;
        while (*src and (max > 0))
        {
            --max;
            *dest++ = *src++;
        }
    }
    va_end(a);
    *dest = NUL;
    return dest;
}


//  ------------------------------------------------------------------

size_t strxlen(const char* str, size_t max)
{
    const char * end = static_cast<const char *>(memchr(str, '\0', max));
    if (end == NULL)
        return max;
    else
        return end - str;
}

//  ------------------------------------------------------------------

int gsprintf(TCHAR* buffer, size_t sizeOfBuffer, const TCHAR* __file, int __line, const TCHAR* format, ...)
{
    int ret = -1;

    if (!buffer || !format)
    {
        LOG.errpointer(__file, __line);
        LOG.printf( "! Parameter is NULL pointer: gsprintf(%s,%lu,%s,...).",
                    (buffer?"buffer":"NULL"), (unsigned long)sizeOfBuffer, (format?"format":"NULL") );
        PointerErrorExit();
        return -1;
    }
    if (!sizeOfBuffer)
    {
        LOG.errmemory(__file, __line);
        LOG.printf("! Buffer size is 0: gsprintf(buf,0,...).");
        MemoryErrorExit();
        return -1;
    }
    if (!*format)
    {
        LOG.errtest(__file, __line);
        LOG.printf("! Format is empty string: gsprintf(buffer,%lu,"",...).", (unsigned long)sizeOfBuffer);
        return 0;
    }

    {
        va_list argptr;
        va_start(argptr, format);
        /* _vsnprintf_s() may cause exception. Need to test before enabling.
        #   if __VISUAL_C_NOT_LESS(14,00)  // defined HAVE__VSTPRINTF_S // _vsnprintf_s() recommended in MSDN
            ret = _vsnprintf_s(buffer, _TRUNCATE, format, argptr);
            if (ret < 0)
            {
              if (sizeOfBuffer>7) strcpy(buffer," ERROR ");
              else               buffer[sizeOfBuffer-1] = '\0';
              LOG.errtest(__FILE__,__LINE__-5);
              LOG.printf("! gsprintf()(buffer,%i,%s,...): _vsnprintf_s() error: \"%s\".", sizeOfBuffer, format, strerror(errno));
              return -1;
            }
            buffer[sizeOfBuffer-1] = '\0';  // Microsoft implementation don't write final '\0' if buffer full.
        #   elif __VISUAL_C_NOT_LESS(10,00)  // defined HAVE__VSTPRINTF // _vsnprintf() exist in VS6 and deprecated in VS2005
        */
#   if __VISUAL_C_NOT_LESS(10,00)  // defined HAVE__VSTPRINTF // _vsnprintf() exist in VS6 and deprecated in VS2005

        char * b1 = new char[sizeOfBuffer+1];
        const size_t endOfBuffer = sizeOfBuffer-1;
        ret = _vsnprintf(b1, sizeOfBuffer+1, format, argptr);
        if (ret == -1 || ret >= sizeOfBuffer) // Microsoft implementation returns -1 when buffer overflow.
        {
            strncpy(buffer,b1,endOfBuffer);
            buffer[endOfBuffer] = '\0';  // Microsoft implementation don't write final '\0' when buffer full.
            if (b1[sizeOfBuffer] && strlen(buffer)>=endOfBuffer)
            {
                LOG.printf("! %s", gerrinfo("Memory error", __file, __line));
                LOG.printf("! gsprintf(buffer,%i,%s,...): buffer overflow, result in next line:", sizeOfBuffer, format);
                LOG.printf("! %s", buffer);
                if (sizeOfBuffer>17) memcpy(buffer, " ERROR, see log! ", 17);
                else if (sizeOfBuffer>7) memcpy(buffer," ERROR ", 7);
            }
        }
        else if (ret < 0)
        {
            LOG.errtest(__file, __line);
            LOG.printf("! gsprintf()(buffer,%i,%s,...): _vsnprintf() error: \"%s\".", sizeOfBuffer, format, strerror(errno));
            TestErrorExit();
        }
        else
        {
            strncpy(buffer,b1,endOfBuffer);
            buffer[endOfBuffer] = '\0';
        }

#   elif defined HAVE_VSNPRINTF  // C99 and above

        ret = vsnprintf(buffer, sizeOfBuffer, format, argptr);
        if (ret < 0) // Until glibc 2.0.6 vsnprintf() would return -1 when the output was truncated.
        {
            LOG.errtest(__file, __line);
            char * errstring = strerror(errno);
            LOG.printf("! gsprintf(buffer,%i,%s,...): vsnprintf() error: \"%s\".", sizeOfBuffer, format, errstring);
            if ( strcmp(errstring, "Invalid or incomplete multibyte or wide character")==0 )
            {
                LOG.printf("! Possible reason: you don't set locale properly");
            }
            TestErrorExit();
        }
        else if (ret >= sizeOfBuffer)
        {
//            if (sizeOfBuffer>17) strcpy(buffer, " ERROR, see log! ");
//            else if (sizeOfBuffer>7) strcpy(buffer," ERROR ");
//            else buffer[sizeOfBuffer-1] = '\0';
			buffer[sizeOfBuffer-1] = '\0';
            LOG.printf("! %s", gerrinfo("Line trunkated", __file, __line));
            LOG.printf("! gsprintf(buffer,%i,%s,...): line trunkated to buffer size (need %i bytes).", sizeOfBuffer, format, ret);
        }

#   else
#   error Please look C library of your compiler for function like vsnprintf, what do not write more than size bytes into string.
#   endif
        va_end(argptr);
    }

    return ret;
}


//  ------------------------------------------------------------------

GTok::GTok(char* sep)
{

    separator = sep ? sep : ", \t";
}


//  ------------------------------------------------------------------

#if defined(__GNUC__) && !defined(__EMX__)

char* strupr(char* s)
{

    char* p = s;
    while(*p)
    {
        *p = g_toupper(*p);
        p++;
    }
    return s;
}

char* strlwr(char* s)
{

    char* p = s;
    while(*p)
    {
        *p = g_tolower(*p);
        p++;
    }
    return s;
}

#endif

void strupr(std::string& s)
{
    for (std::string::iterator it = s.begin(); it !=s.end(); ++it)
    {
        *it = g_toupper(*it);
    }
}

void strlwr(std::string& s)
{
    for (std::string::iterator it = s.begin(); it !=s.end(); ++it)
    {
        *it = g_tolower(*it);
    }
}


//  ------------------------------------------------------------------

void tokenize(gstrarray &array, const TCHAR* str, const TCHAR *delim)
{
    if (delim == NULL) delim = ", \t";

#if defined(_tcstok_s)
    TCHAR *tmp = _strdup(str);
    TCHAR *next_token;
    TCHAR *token = _tcstok_s(tmp, delim, &next_token);
#else
    TCHAR *tmp = strdup(str);
    TCHAR *token = strtok(tmp, delim);
#endif

    while (token)
    {
        array.push_back(token);
#if defined(_tcstok_s)
        token = _tcstok_s(NULL, delim, &next_token);
#else
        token = strtok(NULL, delim);
#endif
    }

    free(tmp);
}


//  ------------------------------------------------------------------

std::string &FormatString(std::string &format, const char *token, const char *replace)
{
    size_t tokenLen = strlen(token);
    size_t pos = format.find(token);

    for (; pos != std::string::npos; pos = format.find(token))
    {
        format.replace(pos, tokenLen, replace);
    }

    return format;
}


//  ------------------------------------------------------------------

std::string &FormatString(std::string &format, const char *token, size_t replace)
{
    char buff[128];
    gsprintf(PRINTF_DECLARE_BUFFER(buff), "%u", replace);
    return FormatString(format, token, buff);
}


//  ------------------------------------------------------------------
