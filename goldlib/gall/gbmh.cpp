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
//  Boyer-Moore-Horspool pattern match.
//  ------------------------------------------------------------------
//  Based on Public Domain version by Thad Smith 7/21/1992, based on a
//  7/92 public domain BMH version by Raymond Gardner.
//  With fixes found by Jeff Dunlop 10/21/93.
//  ------------------------------------------------------------------


#include <climits>
#include <gstrall.h>
#include <gmemdbg.h>
#include <gbmh.h>


//  ------------------------------------------------------------------

gbmh::gbmh() {

  pat = NULL;
}


//  ------------------------------------------------------------------

gbmh::~gbmh() {

  throw_deletearray(pat);
}


//  ------------------------------------------------------------------

void gbmh::init(const char* pattern, bool ignorecase) {

  ignore_case = ignorecase;
  patlen = strlen(pattern);

  pat = new char [patlen+1];
  throw_new(pat);

  // Copy pattern
  strcpy(pat, pattern);
  if(ignore_case)
    strupr(pat);

  // initialize skip array
  int i;
  for(i=0; i<256; i++)
    skip[i] = patlen;
  for(i=0; i<patlen; i++) {
    skip[pat[i]] = patlen - i - 1;
    if(ignore_case)
      skip[tolower(pat[i])] = patlen - i - 1;
  }
  char lastpatchar = pat[patlen - 1];
  skip[lastpatchar] = INT_MAX;
  if(ignore_case)
    skip[tolower(lastpatchar)] = INT_MAX;

  // Horspool's fixed second shift
  skip2 = patlen;
  for(i=0; i<patlen-1; ++i)
    if(pat[i] == lastpatchar)
      skip2 = patlen - i - 1;
}


//  ------------------------------------------------------------------

bool gbmh::find(const char* buffer) {

  int buflen = strlen(buffer);

  int i = patlen - 1 - buflen;
  if(i >= 0)
    return false;

  buffer += buflen;

  while(1) {

    while((i += skip[buffer[i]]) < 0)
      ;

    if(i < (INT_MAX - buflen))
      return false;

    i -= INT_MAX;
    int j = patlen - 1;
    const char* s = buffer + (i - j);

    if(ignore_case) {
      while(--j >= 0 and toupper(s[j]) == pat[j])
        ;
    }
    else {
      while(--j >= 0 and s[j] == pat[j])
        ;
    }

    if(j < 0)
      return true;

    if((i += skip2) >= 0)
      return false;
  }
}


//  ------------------------------------------------------------------
