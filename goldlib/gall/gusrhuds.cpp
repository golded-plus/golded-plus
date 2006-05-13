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
//  Hudson userfile class implementation.
//  ------------------------------------------------------------------

#include <gfilutil.h>
#include <gmemdbg.h>
#include <gstrall.h>
#include <gusrhuds.h>


//  ------------------------------------------------------------------

HudsonUser::HudsonUser() {
  
  recsize = sizeof(HudsUsers);
  record = new HudsUsers;  throw_new(record);
  recptr = (char*)record;
  name = record->name;
}


//  ------------------------------------------------------------------

HudsonUser::~HudsonUser() {
  
  throw_delete(record);
}


//  ------------------------------------------------------------------

void HudsonUser::inctimesposted(int __times) {
  
  seekread();
  record->timesposted += (word)__times;
  seekwrite();
}


//  ------------------------------------------------------------------

int HudsonUser::isvalid() {

  return not (record->attrib & HUDS_USERDELETED);
}


//  ------------------------------------------------------------------

int HudsonUser::read()
{
  if (gufh != -1)
  {
    ::read(gufh, record, sizeof(HudsUsers));
    STRNP2C(record->name);

    return isvalid();
  }

  return false;
}


//  ------------------------------------------------------------------

uint32_t HudsonUser::lastread() {

  seekread();
  return record->highmsgread;
}


//  ------------------------------------------------------------------

void HudsonUser::lastread(uint32_t __lastread) {
  
  seekread();
  record->highmsgread = (word)__lastread;
  seekwrite();
}


//  ------------------------------------------------------------------

void HudsonUser::recinit(const char* __name) {

  GUser::recinit(__name);
  strc2p(record->name);
}


//  ------------------------------------------------------------------
