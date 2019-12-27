//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 2000 Alexander S. Aganichev
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
//  CRC-16, CRC-32 and Hashing.
//  ------------------------------------------------------------------

#ifndef __gcrcall_h
#define __gcrcall_h


//  ------------------------------------------------------------------

#include <gdefs.h>


//  ------------------------------------------------------------------
//  Declare the CRC tables

extern word __crc16_table[];
extern dword __crc32_table[];

//  ------------------------------------------------------------------
//  Generate/update a CRC-16 or CRC-32 value
//
//  These inline functions were derived from various #define macro
//  implementations. Some of those implementations masked the right
//  shifts to ensure correct zero fill. But this is C++, where zero
//  fill is guaranteed for unsigned operands, and besides we use
//  prototyped unsigned parameters anyway, so we have no problem here.

inline word updCrc16(byte ch, word crc)
{
    return (word)(__crc16_table[crc >> 8] ^ ((crc << 8) & 0xFFFF) ^ ch);
}
inline word updCrc16c(byte ch, word crc)
{
    return (word)(__crc16_table[(crc >> 8) ^ ch] ^ ((crc << 8) & 0xFFFF));
}
inline dword updCrc32(byte ch, dword crc)
{
    return (dword)(__crc32_table[(crc & 0xFF) ^ ch] ^ (crc >> 8));
}


//  ------------------------------------------------------------------
//  Define CRC masks in the "normal" and "CCITT" variants

const word CRC16_MASK_NORMAL = 0;
const word CRC16_MASK_CCITT  = 0xFFFFU;
const dword CRC32_MASK_NORMAL = 0;
const dword CRC32_MASK_CCITT  = 0xFFFFFFFFUL;


//  ------------------------------------------------------------------
//  Prototypes

word  strCrc16(const char* s,         bool nocase=true, word mask=CRC16_MASK_NORMAL);
word  strCrc16c(const char* s,        bool nocase=true, word mask=CRC16_MASK_NORMAL);
dword strCrc32(const char* s,         bool nocase=true, dword mask=CRC32_MASK_NORMAL);

dword strHash32(const char* s,        bool nocase=true);

word  memCrc16(const void* m, long l, bool nocase=true, word mask=CRC16_MASK_NORMAL);
dword memCrc32(const void* m, long l, bool nocase=true, dword mask=CRC32_MASK_NORMAL);
inline dword memCrc32(dword crc, const void* m, long l, bool nocase=true, dword mask=CRC32_MASK_NORMAL)
{
    return memCrc32(m, l, nocase, crc ^ mask) ^ mask;
}

//  ------------------------------------------------------------------
//  Get keyword/value pairs and crc

void getkeyval(char** key, char** val);
void getkeyvaleql(char** key, char** val, bool eql);
word getkeyvalcrc(char** key, char** val);


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
