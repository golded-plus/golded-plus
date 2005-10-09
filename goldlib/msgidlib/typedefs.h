/* Completely rewritten */

#ifndef __TYPEDEFS_H__
#define __TYPEDEFS_H__

#ifdef _MSC_VER
#include <windows.h>

typedef UCHAR      uint8_t;
typedef signed char int8_t;
typedef WORD      uint16_t;
typedef short      int16_t;
typedef  INT       int32_t;
typedef UINT      uint32_t;

#else
#include <stdint.h>
#endif

typedef unsigned bit;

typedef uint8_t byte;

typedef int8_t sbyte;

typedef uint16_t word;
typedef int16_t sword;

typedef uint32_t dword;
typedef int32_t sdword;

typedef uint16_t ushort;
typedef int16_t  sshort;

#endif
