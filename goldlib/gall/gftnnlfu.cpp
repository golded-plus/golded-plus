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
//  FIDOUSER.LST Indexing.
//  ------------------------------------------------------------------

#include <gfilutil.h>
#include <gstrall.h>
#include <gftnnlfu.h>


//  ------------------------------------------------------------------

bool ftn_fidouser_nodelist_index::getnode() {

  #ifdef DEBUG
  printf("<%05ld> ", node);
  #endif

  lseekset(fh, (node-1L)*62L);
  if(read(fh, nodebuf, 62) == 62) {
    nodebuf[60] = NUL;
    char* ptr = strrchr(nodebuf, ' ');
    if(ptr) {
      data.addr.reset(ptr+1);
      data.addr.make_string(data.address);
      *ptr = NUL;
      strtrim(nodebuf);
      strunrevname(data.name, nodebuf);
      *data.status = NUL;
      *data.system = NUL;
      *data.location = NUL;
      *data.phone = NUL;
      *data.baud = NUL;
      *data.flags = NUL;
      return true;
    }
  }

  return false;
}


//  ------------------------------------------------------------------

int ftn_fidouser_nodelist_index::namecmp() const {

  #ifdef DEBUG
  printf("[%s] [%s] ", searchname, nodebuf);
  #endif

  const char* a = searchname;
  const char* b = nodebuf;
  int n = 1;
  int d;
  while(1) {
    d = tolower(*a) - tolower(*b);
    if((d != 0) or (*a == NUL) or (*b == NUL))
      break;
    a++;
    b++;
    n++;
  }
  return d != 0 ? (d > 0 ? n : -n) : 0;
}


//  ------------------------------------------------------------------

bool ftn_fidouser_nodelist_index::search() {

  if(maxnode) {

    long mid;
    long left = 0;
    long right = maxnode;
    int diff = 0;
    int prevdiff;

    do {
      mid = (left+right)/2;
      node = mid + 1;
      getnode();
      prevdiff = diff;
      diff = namecmp();
      #ifdef DEBUG
      printf("(%d)\n", diff);
      #endif
      if(diff < 0)
        right = mid - 1;
      else if(diff > 0)
        left = mid + 1;
      else {
        exactmatch = true;
        return true;
      }
    } while(left < right);

    if(left < maxnode) {
      node = left + 1;
      getnode();
      prevdiff = diff;
      diff = namecmp();
      #ifdef DEBUG
      printf("(%d)\n", diff);
      #endif
      if(diff == 0) {
        exactmatch = true;
        return true;
      }
    }

    if(absolute(prevdiff) > absolute(diff))
      previous();
    else {
      prevdiff = diff;
      if(next()) {
        diff = namecmp();
        #ifdef DEBUG
        printf("(%d)\n", diff);
        #endif
        if(absolute(prevdiff) >= absolute(diff))
          previous();
      }
    }
  }

  exactmatch = false;
  return false;
}


//  ------------------------------------------------------------------

ftn_fidouser_nodelist_index::ftn_fidouser_nodelist_index() {

  fh = -1;
  isopen = false;
  nodebuf[62] = NUL;
  node = 1;
  namebrowse = true;
}


//  ------------------------------------------------------------------

ftn_fidouser_nodelist_index::~ftn_fidouser_nodelist_index() {

  if(isopen)
    close();
}


//  ------------------------------------------------------------------

bool ftn_fidouser_nodelist_index::open() {

  if(isopen)
    close();

  // Open the list file
  fh = ::sopen(nlpath, O_RDONLY|O_BINARY, SH_DENYNO, S_STDRD);
  if(fh == -1) {
    close();
    return false;
  }

  maxnode = filelength(fh) / 62L;
  getnode();

  isopen = true;

  return true;
}


//  ------------------------------------------------------------------

void ftn_fidouser_nodelist_index::close() {

  if(fh != -1)  ::close(fh);  fh = -1;

  isopen = false;
}


//  ------------------------------------------------------------------

bool ftn_fidouser_nodelist_index::find(const char* lookup_name) {

  namebrowse = true;

  struplow(strrevname(searchname, lookup_name));

  return search();
}


//  ------------------------------------------------------------------

bool ftn_fidouser_nodelist_index::previous() {

  if(node > 1) {
    node--;
    bool retval = getnode();
    compare();
    return retval;
  }

  return false;
}


//  ------------------------------------------------------------------

bool ftn_fidouser_nodelist_index::next() {

  if(node < maxnode) {
    node++;
    bool retval = getnode();
    compare();
    return retval;
  }

  return false;
}


//  ------------------------------------------------------------------

void ftn_fidouser_nodelist_index::first() {

  node = 1;
  getnode();
  compare();
}


//  ------------------------------------------------------------------

void ftn_fidouser_nodelist_index::last() {

  node = maxnode;
  getnode();
  compare();
}


//  ------------------------------------------------------------------

void ftn_fidouser_nodelist_index::push_state() {

  statenode = node;
}


//  ------------------------------------------------------------------

void ftn_fidouser_nodelist_index::pop_state() {

  node = statenode;
  getnode();
  compare();
}


//  ------------------------------------------------------------------

const char* ftn_fidouser_nodelist_index::index_name() const {

  const char* ptr = strrchr(nlpath, '\\');
  return ptr ? ptr+1 : nlpath;
}


//  ------------------------------------------------------------------

const char* ftn_fidouser_nodelist_index::nodelist_name() const {

  return NULL;
}


//  ------------------------------------------------------------------

