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
//  String bag class.
//  ------------------------------------------------------------------

#include <gmemdbg.h>
#include <gstrall.h>
#include <gstrbags.h>


//  ------------------------------------------------------------------

const int BLOCKSIZE = 4096;


//  ------------------------------------------------------------------

GStrBag::GStrBag() {

  items = 0;
  bagsize = 0;
  bag = NULL;
  blocksize = BLOCKSIZE;
  currno = 0;
}


//  ------------------------------------------------------------------

GStrBag::~GStrBag() {

  Reset();
}


//  ------------------------------------------------------------------

void GStrBag::Reset() {

  if(bag != NULL)
    throw_release(bag);
}


//  ------------------------------------------------------------------

int GStrBag::Add(const char* string) {

  return Add(string, strlen(string)+1);
}


//  ------------------------------------------------------------------

int GStrBag::Add(const void* data, int length) {

  if(items == 0)
    bag = (char*)throw_malloc(blocksize);
  int currsize = bagsize + (items*sizeof(int));
  int currsizeblks = (currsize/blocksize) + 1;
  int newsizeblks = ((currsize+length+sizeof(int))/blocksize) + 1;
  if(newsizeblks != currsizeblks)
    bag = (char*)throw_realloc(bag, newsizeblks*blocksize);
  memmove(bag+bagsize+length, bag+bagsize, items*sizeof(int));
  memcpy(bag+bagsize, data, length);
  ((int*)(bag+bagsize+length))[items] = bagsize;
  bagsize += length;
  return items++;
}


//  ------------------------------------------------------------------

void GStrBag::Change(int index, const char* string) {

  Change(index, string, strlen(string)+1);
}


//  ------------------------------------------------------------------

void GStrBag::Change(int index, const void* data, int length) {

  int oldpos = Pos(index);
  int oldlen = bag ? strlen(bag+oldpos)+1 : 0;
  int lendiff = length - oldlen;
  int oldsize = bagsize+(items*sizeof(int));
  int movesize = oldsize - oldpos - oldlen;
  int currsizeblks = (oldsize/blocksize) + 1;
  int newsizeblks = ((oldsize+lendiff)/blocksize) + 1;
  if(lendiff > 0) {
    if(newsizeblks != currsizeblks)
      bag = (char*)throw_realloc(bag, oldsize+lendiff);
    memmove(bag+oldpos+length, bag+oldpos+oldlen, movesize);
  }
  else if(lendiff < 0) {
    memmove(bag+oldpos+length, bag+oldpos+oldlen, movesize);
    if(newsizeblks != currsizeblks)
      bag = (char*)throw_realloc(bag, oldsize+lendiff);
  }
  memcpy(bag+oldpos, data, length);
  bagsize += lendiff;
  for(int n=index+1; n<items; n++)
    Pos(n) += lendiff;
}


//  ------------------------------------------------------------------

const char* GStrBag::operator[](int index) {

  return Index(index);
}


//  ------------------------------------------------------------------

void GStrSet3::Put(const char* s1, const char* s2, const char* s3) {

  int len1 = strlen(s1 ? s1 : "")+1;
  int len2 = strlen(s2 ? s2 : "")+1;
  int len3 = strlen(s3 ? s3 : "")+1;
  cfg.size = len1 + len2 + len3;
  set = (char *)throw_xmalloc(cfg.size);
  cfg.pos2 = len1;
  cfg.pos3 = len1 + len2;
  memcpy(set, s1 ? s1 : "", len1);
  memcpy(set+cfg.pos2, s2 ? s2 : "", len2);
  memcpy(set+cfg.pos3, s3 ? s3 : "", len3);
}


//  ------------------------------------------------------------------

void GStrSet3::Change(const char* s1, const char* s2, const char* s3) {
  
  char* oldset = set;
  Put(s1, s2, s3);
  throw_xfree(oldset);
}


//  ------------------------------------------------------------------
