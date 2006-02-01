/*  This include file is used in C and C++ sources.
    Don't use C++ specific code here please.
*/
/*
  ------------------------------------------------------------------
  The Goldware Library
  Copyright (C) 1990-1999 Odinn Sorensen
  Copyright (C) 1999-2000 Alexander S. Aganichev
  ------------------------------------------------------------------
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public
  License along with this program; if not, write to the Free
  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
  MA 02111-1307, USA
  ------------------------------------------------------------------
  $Id$
  ------------------------------------------------------------------
  Basic definitions and types.
  ------------------------------------------------------------------
*/
#ifndef __goldall_h
#define __goldall_h

/*  ------------------------------------------------------------------ */
#ifdef __cplusplus
#include <cstddef>
#endif
#include <gcmpall.h>
#ifdef _MSC_VER
#include <windows.h>
#include <limits.h>
#elif defined(__MINGW32__) || defined(__CYGWIN__)
#include <stdint.h>
#else
#endif

/*  ------------------------------------------------------------------
// Disable some MS Visual C warnings */

#if defined(_MSC_VER)
/*
// C4786: 'identifier' : identifier was truncated to 'number'
//        characters in the debug information
//
// C4065: switch statement contains 'default' but no 'case' labels
//
// C4200: nonstandard extension used : zero-sized array in struct/union
*/
#pragma warning(disable: 4200 4786 4065)
#endif

/*  ------------------------------------------------------------------
//  Define portability and shorthand notation */

/* GCC after 2.95.x have "and", "not", and "or" predefined */
#if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ < 96)
#ifndef and
#define not      !
#define and      &&
#define or       ||
#endif
#endif

#ifndef true
#define true  1
#define false 0
#endif

#define NO     0
#define YES    1
#define ALWAYS 2
#define ASK    2
#define GAUTO  3
#define MAYBE  4

#define NUL ((char)'\x00')    /* Common ASCII control codes */
#define BEL '\x07'
#define BS  '\x08'
#define HT  '\x09'
#define LF  '\x0A'
#define FF  '\x0C'
#define CR  '\x0D'
#define ESC '\x1B'

#ifdef __APPLE__
#define __UNIX__
#endif

#ifdef __UNIX__
#define NL "\r\n"
#else
#define NL "\n"
#endif

/*  ------------------------------------------------------------------
//  Spellchecker using dynamic library load */
#if !defined(GCFG_NOSPELLDLL) && !defined(__WIN32__)
#undef GCFG_NOSPELLDLL
#endif

/*  ------------------------------------------------------------------
//  Special character constants */

#define CTRL_A '\x01'   /* FidoNet kludge line char  */
#define SOFTCR '\x8D'   /* "Soft" carriage-return    */


/*  ------------------------------------------------------------------
//  Supplements for the built-in types   */

#ifdef _MSC_VER
#if (UCHAR_MAX == 0xFF)
typedef   signed char    int8_t;
typedef unsigned char   uint8_t;
#else
#error Dont know how to define 8 bit integers
#endif
#if (USHRT_MAX == 0xFFFF)
typedef   signed short   int16_t;
typedef unsigned short  uint16_t;
#else
#error Dont know how to define 16 bit integers
#endif
#if (UINT_MAX == 0xFFFFFFFF)
typedef   signed int     int32_t;
typedef unsigned int    uint32_t;
#else
#error Dont know how to define 32 bit integers
#endif
#endif  /*#ifdef _MSC_VER */

#if defined(__GNUC__) && !defined(__MINGW32__) && !defined(__CYGWIN__)
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

#if !defined(__APPLE__)
#if !defined(__SUNOS__)
typedef   signed char   int8_t;
#endif
typedef   signed short  int16_t;
typedef   signed int    int32_t;
#endif
#endif

typedef uint8_t   byte;
typedef uint16_t  word;
typedef uint32_t  dword;

#if !defined(__APPLE__)
typedef unsigned int uint;
#endif

typedef uint8_t   bits;
typedef uint32_t  time32_t;   /* 32-bit time_t type */

/*  ------------------------------------------------------------------  */

#ifdef __cplusplus


/*  ------------------------------------------------------------------   */

#if defined(__GOLD_GUI__)
#define STD_PRINT(out) {  \
  std::strstream str;     \
  str << out;             \
  GUI_Print(str);         \
  }
#define STD_PRINTNL(out) {  \
  std::strstream str;       \
  str << out << NL;         \
  GUI_Print(str);           \
  }
#else
#define STD_PRINT(out) std::cout << out;
#define STD_PRINTNL(out) std::cout << out << NL;
#endif


/*  ------------------------------------------------------------------   */
/*  Common function-pointer types                                        */

typedef void (*VfvCP)();
typedef int (*IfvCP)();
typedef int (*IfcpCP)(char*);


/*  ------------------------------------------------------------------   */
/*  Function pointer for stdlib qsort(), bsearch() compare functions     */

typedef int (*StdCmpCP)(const void*, const void*);


/*  ------------------------------------------------------------------   */
/*  Utility templates                                                    */

template <class T> inline bool in_range(T a, T b, T c)   { return (a >= b) and (a <= c); }
template <class T> inline    T absolute(T a)             { return a < 0 ? -a : a; }
template <class T> inline  int compare_two(T a, T b)     { return a < b ? -1 : a > b ? 1 : 0; }
template <class T> inline    T minimum_of_two(T a, T b)  { return (a < b) ? a : b; }
template <class T> inline    T maximum_of_two(T a, T b)  { return (a > b) ? a : b; }
template <class T> inline  int zero_or_one(T e)          { return e ? 1 : 0; }
template <class T> inline bool make_bool(T a)            { return !!a; }
template <class T> inline bool make_bool_not(T a)        { return !a; }


/*  ------------------------------------------------------------------
//  Handy macro for safe casting.           Public domain by Bob Stout
//  ------------------------------------------------------------------
//
//  Example of CAST macro at work
//
//  union {
//    char  ch[4];
//    int   i[2];
//  } my_union;
//
//  long longvar;
//
//  longvar = (long)my_union;         // Illegal cast
//  longvar = CAST(long, my_union);   // Legal cast
//
//  ------------------------------------------------------------------ */

#define CAST(new_type,old_object) (*((new_type *)&(old_object)))


/*  ------------------------------------------------------------------ */
/*  Get size of structure member                                       */

#define sizeofmember(__struct, __member)  sizeof(((__struct*)0)->__member)


/*  ------------------------------------------------------------------ */
/*  Legacy defines                                                     */

#define RngV in_range
#define AbsV absolute
#define CmpV compare_two
#define MinV minimum_of_two
#define MaxV maximum_of_two

/*  ------------------------------------------------------------------ */

#endif  /*#ifdef __cplusplus*/

/*  ------------------------------------------------------------------ */

#endif

/*  ------------------------------------------------------------------ */
