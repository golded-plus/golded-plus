//  hey, emacs, treat this like -*- C++ -*- file

//  ------------------------------------------------------------------
//  The Goldware Library.
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 2000 Alex. S. Aganichev
//  ------------------------------------------------------------------
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License as
//  published by the Free Software Foundation; either version 2 of the
//  License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//  ------------------------------------------------------------------
//  $Id$
//  ------------------------------------------------------------------
//  Group/Random System.
//  ------------------------------------------------------------------

#ifdef __GNUG__
#pragma implementation "gutlgrp.h"
#endif

#include <cstdlib>
#include <gmemdbg.h>
#include <string>
#include <gwildmat.h>
#include <gutlgrp.h>


//  ------------------------------------------------------------------
//  Constructor

Grp::Grp() {

  currgrp = container.end();
  currgrpno = -1;
}


//  ------------------------------------------------------------------
//  Destructors

Grp::~Grp() {

  multimap<int, grp_stock>::iterator i;
  for(currgrp = container.begin(); currgrp != container.end(); currgrp++)
    for(i = currgrp->second.find(GRP_MEMBER); i != currgrp->second.end(); i++) {
      if(i->second.type == TYPE_OBJECT)
        throw_free(i->second.data.object_item);
      else if(i->second.type == TYPE_STRING)
        throw_delete(i->second.data.string_item);
    }
}


//  ------------------------------------------------------------------
//  Add a new group

void Grp::AddGrp(const char* id) {

  string sid(id);
  multimap<int, grp_stock> m;
  m.insert(pair<int, grp_stock>(GRP_MEMBER, sid));
  container.push_back(pair<string, multimap<int, grp_stock> >(sid, m));
  currgrp = container.end()-1;
  currgrpno = container.size()-1;
}


//  ------------------------------------------------------------------
//  Add a new group member

void Grp::AddMbr(const char* id) {
  
  currgrp->second.insert(pair<int, grp_stock>(GRP_MEMBER, string(id)));
}


//  ------------------------------------------------------------------

const char* Grp::SetGrp(const char* id) {

  multimap<int, grp_stock>::iterator i;
  for(currgrp = container.begin(), currgrpno = 0; currgrp != container.end(); currgrp++, currgrpno++)
    for(i = currgrp->second.find(GRP_MEMBER); (i != currgrp->second.end()) and (i->first == GRP_MEMBER); i++)
      if(strwild(id, i->second.data.string_item->c_str()))
        return i->second.data.string_item->c_str();
  currgrpno = -1;
  return NULL;
}


//  ------------------------------------------------------------------

void Grp::AddItm(int __type, bool __data) {

  currgrp->second.insert(pair<int, grp_stock>(__type, __data));
}


//  ------------------------------------------------------------------

void Grp::AddItm(int __type, char __data) {

  currgrp->second.insert(pair<int, grp_stock>(__type, __data));
}


//  ------------------------------------------------------------------

void Grp::AddItm(int __type, int __data) {

  currgrp->second.insert(pair<int, grp_stock>(__type, __data));
}


//  ------------------------------------------------------------------

void Grp::AddItm(int __type, const string& __data) {

  currgrp->second.insert(pair<int, grp_stock>(__type, __data));
}


//  ------------------------------------------------------------------

void Grp::AddItm(int __type, void* __data, int __size) {

  void *data = throw_malloc(__size);
  memcpy(data, __data, __size);
  currgrp->second.insert(pair<int, grp_stock>(__type, data));
}


//  ------------------------------------------------------------------

int Grp::GetItm(int __type, bool& __data, int __no) {

  // Return error if a current group is not set
  if(currgrpno == -1)
    return -1;

  // Pointer to item type index for this group and type
  int rv = currgrp->second.count(__type);

  if(rv) {
    if((__no >= rv) or (__no == -1))
      __no = rand() % rv;
    multimap<int, grp_stock>::iterator i = currgrp->second.find(__type);
    while(__no--) i++;
    __data = i->second.data.bool_item;
  }

  // Return number of items of this type in the group
  return rv;
}


//  ------------------------------------------------------------------

int Grp::GetItm(int __type, char& __data, int __no) {

  // Return error if a current group is not set
  if(currgrpno == -1)
    return -1;

  // Pointer to item type index for this group and type
  int rv = currgrp->second.count(__type);

  if(rv) {
    if((__no >= rv) or (__no == -1))
      __no = rand() % rv;
    multimap<int, grp_stock>::iterator i = currgrp->second.find(__type);
    while(__no--) i++;
    __data = i->second.data.char_item;
  }

  // Return number of items of this type in the group
  return rv;
}


//  ------------------------------------------------------------------

int Grp::GetItm(int __type, int& __data, int __no) {

  // Return error if a current group is not set
  if(currgrpno == -1)
    return -1;

  // Pointer to item type index for this group and type
  int rv = currgrp->second.count(__type);

  if(rv) {
    if((__no >= rv) or (__no == -1))
      __no = rand() % rv;
    multimap<int, grp_stock>::iterator i = currgrp->second.find(__type);
    while(__no--) i++;
    __data = i->second.data.int_item;
  }

  // Return number of items of this type in the group
  return rv;
}


//  ------------------------------------------------------------------

int Grp::GetItm(int __type, string& __data, int __no) {

  // Return error if a current group is not set
  if(currgrpno == -1)
    return -1;

  // Pointer to item type index for this group and type
  int rv = currgrp->second.count(__type);

  if(rv) {
    if((__no >= rv) or (__no == -1))
      __no = rand() % rv;
    multimap<int, grp_stock>::iterator i = currgrp->second.find(__type);
    while(__no--) i++;
    __data = *(i->second.data.string_item);
  }

  // Return number of items of this type in the group
  return rv;
}


//  ------------------------------------------------------------------

int Grp::GetItm(int __type, void* __data, int __size, int __no) {

  // Return error if a current group is not set
  if(currgrpno == -1)
    return -1;

  // Pointer to item type index for this group and type
  int rv = currgrp->second.count(__type);

  if(rv) {
    if((__no >= rv) or (__no == -1))
      __no = rand() % rv;
    multimap<int, grp_stock>::iterator i = currgrp->second.find(__type);
    while(__no--) i++;
    memcpy(__data, i->second.data.object_item, __size);
  }

  // Return number of items of this type in the group
  return rv;
}


//  ------------------------------------------------------------------
