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
#include <gctype.h>
#include <cstring>
#include <gdefs.h>


//  ------------------------------------------------------------------

#if defined(__EMX__)
#include <sys/nls.h>
#define strupr(s) (char *)_nls_strupr((unsigned char *)(s))
#define strlwr(s) (char *)_nls_strlwr((unsigned char *)(s))
#elif defined(__GNUC__)
#define stricmp strcasecmp
#define strnicmp strncasecmp
char* strupr(char* s);
char* strlwr(char* s);
#endif


//  ------------------------------------------------------------------
//  Function prototypes

bool strblank(const char* str);
int strchg(char* str, char oldch, char newch);
char* stridela(const char* substr, char* str);
int strnicmpw(const char* str1, const char* str2, int len);
const char* striinc(const char* str1, const char* str2);
char* strins(const char* instr, char* str, int st_pos);
char* strischg(char* str, const char* find, const char* replace);
char* strrjust(char* str);
char* strschg(char* str, const char* find, const char* replace);
char* strsetsz(char* str, int newsize);
char* strshl(char* str, int count);
char* strshr(char* str, int count);
char* strsrep(char* str, const char* search, const char* replace);
char* strltrim(char* str);
char* strtrim(char* str);
string& strtrim(string& p);
char* struplow(char* str);

char* longdotstr(long num);                               // Convert a long to a dotted string: xxx.xxx.xxx.xxx
char* longdotstr(char* str, long num);                    // Convert a long to a dotted string: xxx.xxx.xxx.xxx
char* StripQuotes(char* str);

// Safe versions of strcpy, strcat, sequencial strcat...
char* strxcpy(char* d, const char* s, int n);
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

inline char* strbtrim(char* st) { return strtrim(strltrim(st)); }

inline bool streql(const char* str1, const char* str2)          { return not strcmp(str1,str2); }
inline bool strieql(const char* str1, const char* str2)          { return not stricmp(str1,str2); }
inline bool strneql(const char* str1, const char* str2, int n)  { return not strncmp(str1,str2,n); }
inline bool strnieql(const char* str1, const char* str2, int n)  { return not strnicmp(str1,str2,n); }

inline const char* strskip_to(const char* p, char* s) { return p+strcspn(p, s); }
inline char* strskip_to(char* p, char* s) { return p+strcspn(p, s); }

inline const char* strskip_to(const char* p, char c) { while((*p != c) and *p) p++; return p; }
inline char* strskip_to(char* p, char c) { while((*p != c) and *p) p++; return p; }

inline const char* strskip_txt(const char* p) { while(*p and not isspace(*p)) p++; return p; }
inline char* strskip_txt(char* p) { while(*p and not isspace(*p)) p++; return p; }

inline const char* strskip_wht(const char* p) { while(*p and isspace(*p)) p++; return p; }
inline char* strskip_wht(char* p) { while(*p and isspace(*p)) p++; return p; }

inline const char* strskip_digits(const char* p) { return p+strspn(p, "0123456789"); }
inline char* strskip_digits(char* p) { return p+strspn(p, "0123456789"); }

#if defined(_MSC_VER) or (defined(__GNUC__) and not defined(__DJGPP__)) or defined(__WATCOMC__)
inline char * stpcpy(char* dest, const char* src) {
  while ((*dest++ = *src++) != NUL) {}
  return --dest;
}
#endif

#ifndef isoctal
#define isoctal(c) (((c) >= '0') and ((c) <= '7'))
#endif

char* strcvtc(char* s);


//  ------------------------------------------------------------------
//  String tokenizer class

class GTok {

protected:

  const char* separator;
  char* token;

public:

  GTok(char* sep=NULL);

  char* First(char* buf)  { token = strtok(buf,  separator); return token; }
  char* Next()            { token = strtok(NULL, separator); return token; }
  char* Token()           { return token; }
};


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
