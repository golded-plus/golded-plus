#ifndef __HSKSUPP_H_
#define __HSKSUPP_H_

#include <stdlib.h>
#include <time.h>
#include "typedefs.h"

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

#if defined(__EMX__)
#if !defined(OS2)
#define OS2 1
#endif
#if !defined(__FLAT__)
#define __FLAT__ 1
#endif
#endif

#if defined(__MINGW32__)
/* mingw32 warnings */
#define NONAMELESSUNION

#define sleep(sec) _sleep((sec)*1000l)
#endif

#if defined(SASC) || defined(UNIX)
#define PATH_DELIM  '/'
#else
#define PATH_DELIM  '\\'
#endif

#define eqstr(str1,str2)      (strcmp(str1,str2)==0)

#if !defined(__GNUC__) || defined(__MINGW32__)
#define mymkdir(path) mkdir(path)
#else
#define mymkdir(path) mkdir(path, 0)
#endif

/* fexists.c */
int fexist(const char *filename);
long fsize(const char *filename);
int direxist(const char *directory);
int _createDirectoryTree(const char *pathName);

/* patmat.c */
#ifdef UNIX
int patmat(char *raw, char *pat);
#endif

#endif /* __HSKSUPP_H_ */
