/* This is handmade generic config.h */

#if !defined WINDOWS32 && defined _WIN32
#define WINDOWS32     1
#endif

#define HAVE_DIRENT_H 1
#define STDC_HEADERS  1
#ifndef _MSC_VER
#define HAVE_UNISTD_H 1
#endif
#define HAVE_STRING_H 1
#define HAVE_MEMORY_H 1
#define HAVE_STRCOLL  1
#ifdef _MSC_VER
#define HAVE_MALLOC_H 1
#endif

#define __P(protos)   protos
