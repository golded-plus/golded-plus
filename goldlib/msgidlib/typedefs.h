/* Completely rewritten */

#ifndef __TYPEDEFS_H__
#define __TYPEDEFS_H__

#ifndef __goldall_h
#ifdef _MSC_VER
#include <windows.h>
#include <limits.h>

#if (UCHAR_MAX == 0xFF)
typedef   signed char    int8_t;
typedef unsigned char   uint8_t;
#else
#error Don't know how to define 8 bit integers
#endif
#if (USHRT_MAX == 0xFFFF)
typedef   signed short   int16_t;
typedef unsigned short  uint16_t;
#else
#error Don't know how to define 16 bit integers
#endif
#if (UINT_MAX == 0xFFFFFFFF)
typedef   signed int     int32_t;
typedef unsigned int    uint32_t;
#else
#error Don't know how to define 32 bit integers
#endif

#else
#include <stdint.h>
#endif  //#ifdef _MSC_VER

typedef uint8_t   bit;

typedef  int8_t   sbyte;
typedef uint8_t   byte;

typedef  int16_t  sword;
typedef uint16_t  word;
typedef  int16_t  sshort;
typedef uint16_t  ushort;

typedef uint32_t  dword;

#endif  //#ifndef __goldall_h

#endif  //#ifndef __TYPEDEFS_H__
