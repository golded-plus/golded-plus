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
//  Decoder/encoders.
//  ------------------------------------------------------------------

#ifdef __GNUG__
#pragma implementation "gutlcode.h"
#endif


//  ------------------------------------------------------------------

#include <gstrall.h>
#include <gutlmisc.h>
#include <gutlcode.h>


//  ------------------------------------------------------------------

uucode_engine::uucode_engine() {

  initialized = false;
}


//  ------------------------------------------------------------------

void uucode_engine::initialize() {

  // Set up the default translation table.
  int i, j;
  for(i=0; i<' '; i++)
    table.ch[i] = -1;
  for(i=' ',j=0; i<' '+64; i++,j++)
    table.ch[i] = j;
  for(i=' '+64; i<256; i++)
    table.ch[i] = -1;
  table.ch['`' & 0xff] = table.ch[' ' & 0xff];  // A common mutation
  table.ch['~' & 0xff] = table.ch['^' & 0xff];  // Another common mutation
  table.blank = ' ';

  // Set up the line length table, to avoid computing lotsa * and / ...
  table.len[0] = 1;
  for(i=1,j=5; i<=60; i+=3, j+=4)
    table.len[i] = (table.len[i+1] = (table.len[i+2] = j));

  initialized = true;
  complete = false;
}


//  ------------------------------------------------------------------

char* uucode_engine::decode(char* outputbuffer, const char* inputbuffer) {

  char buf[80];
  char* o = outputbuffer;
  int len = strlen(strcpy(buf, inputbuffer));

  if(not initialized)
    initialize();

  if(strnieql(buf, "begin ", 6)) {
    return o;
  }
  else if(strieql(buf, "end")) {
    initialized = false;
    complete = true;
    return o;
  }
  else if(strieql(buf, "table")) {
    defining_table = true;
    for(int c=0; c<256; c++)
      table.ch[c] = -1;
    table_index = 0;
    return o;
  }

  if(defining_table) {
    char* p = buf + len - 1;
    *p = ' ';
    while(*p == ' ')
      *p-- = 0;
    p = buf;
    char c;
    while((c = *p) != 0) {
      if(table_index == 0)
        table.blank = c;
      table.ch[c & 0xff] = table_index++;
      if(table_index >= 64) {
        defining_table = false;
        break;
      }
      p++;
    }
    return o;
  }

  // Get the binary line length.
  int n = table.ch[*buf & 0xff];
  if(n > 0) {

    // Pad with blanks.
    int c = len;
    char* i = buf + c;
    int rlen = table.len[n];
    while(c < rlen) {
      *i++ = table.blank;
      c++;
    }

    // Output a group of 3 bytes (4 input characters).
    i = buf + 1;
    while(n > 0) {
      *(o++) = (char)((table.ch[*i & 0xff] << 2) | (table.ch[i[1] & 0xff] >> 4));
      n--;
      if(n) {
        *(o++) = (char)((table.ch[i[1] & 0xff] << 4) | (table.ch[i[2] & 0xff] >> 2));
        n--;
      }
      if(n) {
        *(o++) = (char)((table.ch[i[2] & 0xff] << 6) | table.ch[i[3] & 0xff]);
        n--;
      }
      i += 4;
    }
  }

  *o = NUL;

  return o;
}


//  ------------------------------------------------------------------

char* quoted_printable_engine::decode(char* outputbuffer, const char* inputbuffer) {

  char c;
  char* o = outputbuffer;
  const char* i = inputbuffer;

  while((c = *i) != NUL) {
    if(c == '=') {
      char c1 = i[1];
      char c2 = i[2];
      if(isxdigit(c1) and isxdigit(c2)) {
        // Decode the character
        c = (char)((xtoi(c1) << 4) | xtoi(c2));
        i += 2;
      }
    }
    //else if(c == '_') {
    //  c = ' ';
    //}
    *o++ = c;
    i++;
  }

  *o = NUL;

  return o;
}


//  ------------------------------------------------------------------

char* base64_engine::decode(char* outputbuffer, const char* inputbuffer) {

  char* o = outputbuffer;
  const char* i = inputbuffer;

  int shift = 0;
  uint32_t accum = 0;
  while(*i) {
    char c = *i;
    uint value = (uint)-1;
    if((c >= 'A') and (c <= 'Z'))       value = c - 'A';
    else if((c >= 'a') and (c <= 'z'))  value = 26 + c - 'a';
    else if((c >= '0') and (c <= '9'))  value = 52 + c - '0';
    else if(c == '+')                   value = 62;
    else if(c == '/')                   value = 63;
    else if(c == '=')                   value = (uint)-2;
    if(value >= 64)
      break;
    else {
      accum <<= 6;
      shift += 6;
      accum |= value;
      if(shift >= 8) {
        shift -= 8;
        value = (uint)(accum >> shift);
        c = (byte)(value & 0xFF);
        *o++ = c;
      }
    }
    i++;
  }

  *o = NUL;

  return o;
}


//  ------------------------------------------------------------------

char base64_engine::table[64] = {
  'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P',
  'Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f',
  'g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v',
  'w','x','y','z','0','1','2','3','4','5','6','7','8','9','+','/'
};


//  ------------------------------------------------------------------

char* base64_engine::encode(char* outputbuffer, const char* inputbuffer, uint length, bool padding) {

  uint l = length;
  char* o = outputbuffer;
  const char* i = inputbuffer;

  while(l > 0) {

    char c1 = i[0];
    char c2 = l > 1 ? i[1] : '\0';
    char c3 = l > 2 ? i[2] : '\0';
    int pads = l > 2 ? 0 : (3 - l);

    o[0] = table[c1 >> 2];
    o[1] = table[((c1 & 0x03) << 4) | ((c2 & 0xF0) >> 4)];
    if(pads == 2) {
      if(padding) {
        o[2] = '=';
        o[3] = '=';
      }
      else {
        o[2] = NUL;
        o[3] = NUL;
      }
    }
    else if(pads == 1) {
      o[2] = table[((c2 & 0x0F) << 2) | ((c3 & 0xC0) >> 6)];
      if(padding) {
        o[3] = '=';
      }
      else {
        o[3] = NUL;
      }
    }
    else {
      o[2] = table[((c2 & 0x0F) << 2) | ((c3 & 0xC0) >> 6)];
      o[3] = table[c3 & 0x3F];
    }
    o += 4;
    if(l < 3)
      break;
    l -= 3;
    i += 3;
  }

  *o = NUL;

  return o;
}


//  ------------------------------------------------------------------
