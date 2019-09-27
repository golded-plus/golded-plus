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
//  GoldED nodelist indexing.
//  ------------------------------------------------------------------

#include <gcrcall.h>
#include <gfilutil.h>
#include <gmemdbg.h>
#include <gstrall.h>
#include <gftnnlge.h>
#include <stdlib.h>

//  ------------------------------------------------------------------

static char* make_goldnode_name(char* inp) {

  if(*inp == NUL)
    return inp;

  char buf[100];
  char* p;
  char* q;

  // Convert underlines to spaces
  p = inp;
  while(*p)
    if(*(p++) == '_')
      *(p-1) = ' ';

  // Strip leading spaces
  p = inp;
  while(isspace(*p))
    p++;
  q = &inp[strlen(p)-1];

  // Strip trailing spaces
  while(isspace(*q))
    *q-- = NUL;

  // Search for last space or point
  while(*q != ' ' and *q != '.' and q > p)
    q--;

  // If last char is a point, find last space instead
  if(*(q+1) == 0)
    while(*q != ' ' and q > p)
      q--;

  // Exchange last name and first name(s)
  if(p != q) {
    strcpy(stpcpy(buf, q+1), ", ");
    *(q+(*q == '.' ? 1 : 0)) = NUL;
    strcat(buf, p);
    strcpy(inp, buf);
  }
  struplow(inp);

  return inp;
}


//  ------------------------------------------------------------------

void ftn_golded_nodelist_index::fetchdata() {

  int currfileno = (int)((current.pos >> 24) & 0xFF);

  // currfileno == 0xFF: Entry is taken from a userlist, no nodelist available
  if(currfileno != lastfileno) {
    if(fhx != -1)
      ::close(fhx);

    if(currfileno != 0xff) {
      fhx = ::sopen(nodelist[currfileno].filename, O_RDONLY|O_BINARY, SH_DENYNO, S_STDRD);
      lastfileno = currfileno;
    } else {
      fhx = -1;
      lastfileno = -1;
    }
  }

  strunrevname(data.name, current.name);
  *data.status = NUL;
  *data.system = NUL;
  *data.location = NUL;
  *data.phone = NUL;
  *data.flags = NUL;
  *data.baud = NUL;

  if(fhx != -1) {

    char buf[256];
    buf[255] = NUL;

    lseekset(fhx, current.pos & 0x00FFFFFFL);
    read(fhx, buf, 255);
    if(*buf != ';') {

/*
      char* end = strchr(buf, '\r');
      if(end)
        *end = NUL;
*/
      strtok(buf,"\r\n");

      data.unpack(buf);
    }
  }

  data.addr = current.addr;
  data.addr.make_string(data.address);
}


//  ------------------------------------------------------------------

void ftn_golded_nodelist_index::getnode() {

  #ifdef DEBUG
  printf("<%05ld> ", node);
  #endif

  long seekto = node - 1L;
  if(not namebrowse) {
    if(index32) {
      lseekset(fha, seekto*(long)sizeof(dword));
      dword nseekto;
      read(fha, &nseekto, sizeof(dword));
      seekto = nseekto;
    }
    else {
      lseekset(fha, seekto*(long)sizeof(word));
      word nseekto;
      read(fha, &nseekto, sizeof(word));
      seekto = nseekto;
    }
  }
  lseekset(fhn, seekto*(long)sizeof(_GEIdx));
  read(fhn, &current, sizeof(_GEIdx));
}


//  ------------------------------------------------------------------

int ftn_golded_nodelist_index::namecmp() const {

  #ifdef DEBUG
  printf("[%s] [%s] ", searchname, current.name);
  #endif

  const char* a = searchname;
  const char* b = current.name;
  int n = 1;
  int d;
  while(1) {
    d = g_tolower(*a) - g_tolower(*b);
    if((d != 0) or (*a == NUL) or (*b == NUL))
      break;
    a++;
    b++;
    n++;
  }
  return d != 0 ? (d > 0 ? n : -n) : 0;
}


//  ------------------------------------------------------------------

int ftn_golded_nodelist_index::addrcmp() const {

  #ifdef DEBUG
  printf("[%d:%d/%d.%d] [%d:%d/%d.%d] ",
    searchaddr.zone,  searchaddr.net,  searchaddr.node,  searchaddr.point,
    current.addr.zone, current.addr.net, current.addr.node, current.addr.point
  );
  #endif

  return searchaddr.compare(current.addr);
}


//  ------------------------------------------------------------------

bool ftn_golded_nodelist_index::searchfirst() {

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
      diff = namebrowse ? namecmp() : addrcmp();
      #ifdef DEBUG
      printf("(%d)\n", diff);
      #endif
      if(diff < 0)
        right = mid - 1;
      else if(diff > 0)
        left = mid + 1;
      else {
        exactmatch = true;
        fetchdata();
        return true;
      }
    } while(left < right);

    if(left < maxnode) {
      node = left + 1;
      getnode();
      prevdiff = diff;
      diff = namebrowse ? namecmp() : addrcmp();
      #ifdef DEBUG
      printf("(%d)\n", diff);
      #endif
      if(diff == 0) {
        exactmatch = true;
        fetchdata();
        return true;
      }
    }

    if(absolute(prevdiff) > absolute(diff))
      previous();
    else {
      prevdiff = diff;
      if(next()) {
        diff = namebrowse ? namecmp() : addrcmp();
        #ifdef DEBUG
        printf("(%d)\n", diff);
        #endif
        if(absolute(prevdiff) >= absolute(diff))
          previous();
      }
      else
        fetchdata();
    }
  }

  exactmatch = false;
  return false;
}


//  ------------------------------------------------------------------

bool ftn_golded_nodelist_index::search() {

  if(searchfirst()) {
    // Search backwards until first exact match is found
    while(previous()) {
      if(not exactmatch) {
        next();
        break;
      }
    }
    return true;
  }

  return false;
}


//  ------------------------------------------------------------------

ftn_golded_nodelist_index::ftn_golded_nodelist_index() {

  fha = fhn = fhx = -1;
  nodelist = NULL;
  nodelists = 0;
  lastfileno = -1;
  isopen = false;
}


//  ------------------------------------------------------------------

ftn_golded_nodelist_index::~ftn_golded_nodelist_index() {

  if(isopen)
    close();
}


//  ------------------------------------------------------------------

bool ftn_golded_nodelist_index::open() {

  if(isopen)
    close();

  fha = ::sopen(AddPath(nlpath, "goldnode.gxa"), O_RDONLY|O_BINARY, SH_DENYNO, S_STDRD);
  if(fha == -1) {
    close();
    return false;
  }

  fhn = ::sopen(AddPath(nlpath, "goldnode.gxn"), O_RDONLY|O_BINARY, SH_DENYNO, S_STDRD);
  if(fhn == -1) {
    close();
    return false;
  }

  FILE* fp = fopen(AddPath(nlpath, "goldnode.gxl"), "rt");
  if(fp == NULL) {
    close();
    return false;
  }

  // Read the list index
  char buf[2560];
  nodelists = 0;
  while(fgets(buf, sizeof(buf), fp)) {
    nodelist = (fstamp*)throw_realloc(nodelist, (nodelists+1)*sizeof(fstamp));
    char* key;
    char* val=buf;
    getkeyval(&key, &val);
    strcpy(nodelist[nodelists].filename, key);
    MapPath(nodelist[nodelists].filename);
    nodelist[nodelists].stamp = atol(val);
    nodelists++;
  }
  fclose(fp);

  maxnode = filelength(fha) / sizeof(word);
  if(filelength(fhn) / sizeof(_GEIdx) < (size_t)maxnode) {
    maxnode = filelength(fha) / sizeof(dword);
    index32 = true;
  }
  else
    index32 = false;

  isopen = true;

  return true;
}


//  ------------------------------------------------------------------

void ftn_golded_nodelist_index::close() {

  throw_release(nodelist);
  nodelists = 0;

  lastfileno = -1;

  if(fha != -1)  ::close(fha);  fha = -1;
  if(fhn != -1)  ::close(fhn);  fhn = -1;
  if(fhx != -1)  ::close(fhx);  fhx = -1;

  isopen = false;
}


//  ------------------------------------------------------------------

bool ftn_golded_nodelist_index::find(const char* lookup_name) {

  namebrowse = true;
  make_goldnode_name(strcpy(searchname, lookup_name));
  return search();
}


//  ------------------------------------------------------------------

bool ftn_golded_nodelist_index::find(const ftn_addr& addr) {

  namebrowse = false;
  searchaddr = addr;
  return search();
}


//  ------------------------------------------------------------------

bool ftn_golded_nodelist_index::previous() {

  if(node > 1) {
    node--;
    getnode();
    fetchdata();
    compare();
    return true;
  }

  return false;
}


//  ------------------------------------------------------------------

bool ftn_golded_nodelist_index::next() {

  if(node < maxnode) {
    node++;
    getnode();
    fetchdata();
    compare();
    return true;
  }

  return false;
}


//  ------------------------------------------------------------------

void ftn_golded_nodelist_index::first() {

  node = 1;
  getnode();
  fetchdata();
  compare();
}


//  ------------------------------------------------------------------

void ftn_golded_nodelist_index::last() {

  node = maxnode;
  getnode();
  fetchdata();
  compare();
}


//  ------------------------------------------------------------------

void ftn_golded_nodelist_index::push_state() {

  statenode = node;
}


//  ------------------------------------------------------------------

void ftn_golded_nodelist_index::pop_state() {

  node = statenode;
  getnode();
  fetchdata();
  compare();
}


//  ------------------------------------------------------------------

const char* ftn_golded_nodelist_index::index_name() const {

  #if defined(__UNIX__)
  return namebrowse ? "goldnode.gxn" : "goldnode.gxa";
  #else
  return namebrowse ? "GOLDNODE.GXN" : "GOLDNODE.GXA";
  #endif
}


//  ------------------------------------------------------------------

const char* ftn_golded_nodelist_index::nodelist_name() const {

  if(lastfileno == -1)
    return index_name();

  const char* ptr = strrchr(nodelist[lastfileno].filename, '\\');
  return ptr ? ptr+1 : nodelist[lastfileno].filename;
}


//  ------------------------------------------------------------------
