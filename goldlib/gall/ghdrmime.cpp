//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
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
//  MIME header processing.
//  ------------------------------------------------------------------

#include <gstrall.h>
#include <gstrmail.h>
#include <ghdrmime.h>


//  ------------------------------------------------------------------

const char* mime_crack_encoded_word(const char* encoded_word, char* charset, char* encoding, char* text) {

  if(charset) *charset = NUL;
  if(encoding) *encoding = NUL;
  if(text) *text = NUL;

  const char* ptr = encoded_word;
  if((ptr[0] == '=') and (ptr[1] == '?')) {
    ptr += 2;
    const char* begin = ptr;
    while(*ptr and not is_mime_especial(*ptr))
      ptr++;
    if((ptr-begin) and (*ptr == '?')) {
      ptr++;
      if(charset)
        strxcpy(charset, begin, (uint)(ptr-begin));
      begin = ptr;
      while(*ptr and not is_mime_especial(*ptr))
        ptr++;
      if((ptr-begin) and (*ptr == '?')) {
        ptr++;
        if(encoding)
          strxcpy(encoding, begin, (uint)(ptr-begin));
        begin = ptr;
        while(*ptr and (*ptr != '?'))
          ptr++;
        if(ptr-begin) {
            strxcpy(text, begin, 1+(int)(ptr-begin));
          return ptr + (ptr[0] == '?') + (ptr[1] == '=');
        }
      }
    }
  }
  return NULL;
}


//  ------------------------------------------------------------------
