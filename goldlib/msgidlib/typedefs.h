/* Completely rewritten */

#ifndef __TYPEDEFS_H__
#define __TYPEDEFS_H__

#ifndef __goldall_h
#ifdef _MSC_VER
#include <windows.h>

typedef   signed char   int8_t;
typedef unsigned char  uint8_t;
typedef   signed short  int16_t;
typedef unsigned short uint16_t;
typedef   signed long   int32_t;
typedef unsigned long  uint32_t;

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

typedef  int32_t  sdword;
typedef uint32_t  dword;

#endif  //#ifndef __goldall_h

#endif  //#ifndef __TYPEDEFS_H__
