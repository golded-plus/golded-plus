/* config.h.  Generated automatically by configure.  */
/* config.h.in.  Generated automatically from configure.in by autoheader.  */

/* if not defined by make */
#if !defined(VERSION)
#define VERSION "0.5"
#endif
#if !defined(PATCH)
#define PATCH   "15"
#endif

/* Define to `unsigned' if <sys/types.h> doesn't define.  */
/* #undef size_t */

/* Define if you have the ANSI C header files.  */
#define STDC_HEADERS 1

/* Define if you can safely include both <sys/time.h> and <time.h>.  */
#ifndef _MSC_VER
#define TIME_WITH_SYS_TIME 1
#endif

/*
 * If your system is kinda special
 */
/* #undef SYSTEM_DOS */
/* #undef SYSTEM_QUICKWIN */
/* #undef SYSTEM_WINDLL */
/* #undef SYSTEM_OS2 */

/*
 * If your system has stdin/stdout/stderr
 */
#define HAVE_STDIO 1

/*
 * how to declare functions that are exported from the UU library
 */
#define UUEXPORT

/*
 * how to declare functions that are exported from the fptools library
 */
#define TOOLEXPORT

/*
 * define if your compiler supports function prototypes
 */
#define PROTOTYPES 1

/*
 * Replacement functions.
 * #define strerror _FP_strerror
 * #define tempnam  _FP_tempnam
 * if you don't have these functions
 */

/* Define if you have the gettimeofday function.  */
/* #undef HAVE_GETTIMEOFDAY */

/* Define if you have the <errno.h> header file.  */
#define HAVE_ERRNO_H 1

/* Define if you have the <fcntl.h> header file.  */
#define HAVE_FCNTL_H 1

/* Define if you have the <io.h> header file.  */
#ifdef _MSC_VER
#define HAVE_IO_H
#endif

/* Define if you have the <malloc.h> header file.  */
#undef HAVE_MALLOC_H

/* Define if you have the <memory.h> header file.  */
#define HAVE_MEMORY_H 1

/* Define if you have the <stdarg.h> header file.  */
/* #undef HAVE_STDARG_H */

/* Define if you have the <sys/time.h> header file.  */
#ifndef _MSC_VER
#define HAVE_SYS_TIME_H 1
#endif

/* Define if you have the <unistd.h> header file.  */
#ifndef _MSC_VER
#define HAVE_UNISTD_H 1
#endif

/* Define if you have the <varargs.h> header file.  */
/* #undef HAVE_VARARGS_H */
