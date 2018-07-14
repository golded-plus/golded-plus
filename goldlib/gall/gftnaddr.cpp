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
//  FidoNet Technology Network - 4D address class.
//  ------------------------------------------------------------------

#include <cstdio>
#include <gftnall.h>
#include <gutlmisc.h>
#include <gstrall.h>


//  ------------------------------------------------------------------

void ftn_addr::set(uint zn, uint nt, uint nd, uint pt) {

  zone  = (word)zn;
  net   = (word)nt;
  node  = (word)nd;
  point = (word)pt;
}


//  ------------------------------------------------------------------

void ftn_addr::set(const ftn_addr& a) {

  zone  = a.zone;
  net   = a.net;
  node  = a.node;
  point = a.point;
}


//  ------------------------------------------------------------------

void ftn_addr::set(const void* a) {

  zone = ((ftn_addr*)a)->zone;
  net = ((ftn_addr*)a)->net;
  node = ((ftn_addr*)a)->node;
  point = ((ftn_addr*)a)->point;
}


//  ------------------------------------------------------------------

void ftn_addr::set_all(uint part) {

  set_fast(part, part, part, part);
}


//  ------------------------------------------------------------------

void ftn_addr::reset() {

  reset_fast();
}


//  ------------------------------------------------------------------

char* ftn_addr::reset(const char* str, char* dom, int domsizelimit) {

  reset_fast();
  return set(str, dom, domsizelimit);
}


//  ------------------------------------------------------------------

void ftn_addr::reset(const std::string& str) {

  reset(str.c_str());
}


//  ------------------------------------------------------------------

void ftn_addr::reset(const std::string& str, std::string& dom, int domsizelimit) {

  ftn_domain doms;
  reset(str.c_str(), doms, domsizelimit);
  dom = doms;
}


//  ------------------------------------------------------------------

static bool ftn_getaddrpart(word& part, const char* s) {

  if(*s == '*') {
    part = ftn::wildcard_all;
    return true;
  }
  else if(*s == '?') {
    part = ftn::wildcard_first;
    return true;
  }
  else if(isdigit(*s)) {
    part = atow(s);
    return true;
  }
  return false;
}


//  ------------------------------------------------------------------

const char* ftn_addr::set(const std::string& str) {

  return set(str.c_str());
}


//  ------------------------------------------------------------------

const char* ftn_addr::set(const std::string& str, std::string& dom, int domsizelimit) {

  ftn_domain doms;
  set(str.c_str(), doms, domsizelimit);
  dom = doms;
  return dom.c_str();
}


//  ------------------------------------------------------------------

char* ftn_addr::set(const char* str, char* dom, int domsizelimit) {

  const char* ptr = str;
  const char* domptr;
  bool gotzone = false;
  bool gotnet = false;
  bool gotnode = false;
  bool gotpoint = false;
  bool gotdomain = false;
  static char* nulchar = "";

  if(*str == '*')
    set_all(ftn::wildcard_all);

  while(*ptr and (' ' < *ptr)) {

    switch(*ptr) {

      case '<':
        return nulchar;

      case '{': /*}*/
        while((*ptr != /*{*/ '}') and *ptr)
          ptr++;
        //if(*ptr == /*{*/ '}')
        //  ptr++;
        break;

      case '#':
        domptr = str;
        str = ++ptr;
        if(dom) {
          int domsize = (int)(ptr-domptr);
          strxcpy(dom, domptr, minimum_of_two(domsize, domsizelimit));
          gotdomain = true;
        }
        break;

      case ':':
        gotzone = ftn_getaddrpart(zone, str);
        str = ++ptr;
        if(*str)
          gotnet = ftn_getaddrpart(net, str);
        break;

      case '/':
        if(not gotnet)
          gotnet = ftn_getaddrpart(net, str);
        str = ++ptr;
        if(*str)
          gotnode = ftn_getaddrpart(node, str);
        break;

      case '.':
        if((not gotnode) and (*str != '.'))
          gotnode = ftn_getaddrpart(node, str);
        str = ++ptr;
        if(*str)
          ftn_getaddrpart(point, str);
        gotpoint = true;
        break;

      case '@':
        if(not (gotzone or gotnet or gotnode or gotpoint))
          gotnode = ftn_getaddrpart(node, str);
        domptr = str = ptr++;
        //while(isalnum(*ptr))
        while(*ptr > ' ')
          ptr++;
        if(dom) {
          int domsize = (int)(ptr-domptr);
          strxcpy(dom, domptr+1, minimum_of_two(domsize, domsizelimit));
          gotdomain = true;
        }
        break;

      default:
        ptr++;
    }
  }

  if(not (gotzone or gotnet or gotnode or gotpoint))
    gotnode = ftn_getaddrpart(node, str);

  if(gotzone and not gotnet)
    net = 0;

  if(gotzone and not gotnode) {
    node = (word)((net == ftn::wildcard_all) ? ftn::wildcard_all : 0);
    gotnode = true;
  }

  if(gotnode and not gotpoint)
    point = (word)((node == ftn::wildcard_all) ? ftn::wildcard_all : 0);

  if(dom and gotzone and not gotdomain)
    *dom = NUL;

  return dom ? dom : nulchar;
}


//  ------------------------------------------------------------------

bool ftn_addr::match(const ftn_addr& mask) const {

  if(not ((mask.zone == ftn::wildcard_all) or (mask.zone == zone)))
    return false;
  if(not ((mask.net == ftn::wildcard_all) or (mask.net == net)))
    return false;
  if(not ((mask.node == ftn::wildcard_all) or (mask.node == node)))
    return false;
  if(not ((mask.point == ftn::wildcard_all) or (mask.point == point)))
    return false;

  return true;
}


//  ------------------------------------------------------------------

int ftn_addr::compare(const ftn_addr& other) const {

  int n = 1;
  int d = compare_two(zone, other.zone);
  if(d == 0) {
    n++;
    d = compare_two(net, other.net);
    if(d == 0) {
      n++;
      d = compare_two(node, other.node);
      if(d == 0) {
        n++;
        d = compare_two(point, other.point);
      }
    }
  }

  return d != 0 ? (d > 0 ? n : -n) : 0;
}


//  ------------------------------------------------------------------

std::string& ftn_addr::make_string(std::string& str) const {

  char buf[200];
  make_string(buf);
  str = buf;
  return str;
}


//  ------------------------------------------------------------------

std::string& ftn_addr::make_string(std::string& str, const std::string& dom, int domfmt) const {

  char buf[200];
  make_string(buf, dom.c_str(), domfmt);
  str = buf;
  return str;
}


//  ------------------------------------------------------------------

char* ftn_addr::make_string(char* str, const char* dom, int domfmt) const {

  if(net == 0) {
    *str = NUL;
    return str;
  }

  char buf[20];
  char* ptr = str;

  *ptr = NUL;

  if(domfmt == ftn::domain_first) {
    if(dom and *dom) {
      ptr = stpcpy(ptr, dom);
      *ptr++ = '#';
    }
  }

  if(zone) {
    if(zone == ftn::wildcard_all)
      ptr = stpcpy(ptr, "*");
    else if(zone == ftn::wildcard_first)
      ptr = stpcpy(ptr, "?");
    else {
      sprintf(buf, "%u", (uint)zone);
      ptr = stpcpy(ptr, buf);
    }
    *ptr++ = ':';
  }

  if(net == ftn::wildcard_all)
    ptr = stpcpy(ptr, "*");
  else if(net == ftn::wildcard_first)
    ptr = stpcpy(ptr, "?");
  else {
    sprintf(buf, "%u", (uint)net);
    ptr = stpcpy(ptr, buf);
  }
  *ptr++ = '/';

  if(node == ftn::wildcard_all)
    ptr = stpcpy(ptr, "*");
  else if(node == ftn::wildcard_first)
    ptr = stpcpy(ptr, "?");
  else {
    sprintf(buf, "%u", (uint)node);
    ptr = stpcpy(ptr, buf);
  }

  if(point) {
    *ptr++ = '.';
    if(point == ftn::wildcard_all)
      ptr = stpcpy(ptr, "*");
    else if(point == ftn::wildcard_first)
      ptr = stpcpy(ptr, "?");
    else {
      sprintf(buf, "%u", (uint)point);
      ptr = stpcpy(ptr, buf);
    }
  }

  if(domfmt == ftn::domain_last) {
    if(dom and *dom) {
      *ptr++ = '@';
      ptr = stpcpy(ptr, dom);
    }
  }

  *ptr = NUL;

  return str;
}


//  ------------------------------------------------------------------
