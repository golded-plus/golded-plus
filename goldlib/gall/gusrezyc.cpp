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
//  Ezycom userfile class implementation.
//  ------------------------------------------------------------------

#include <gfilutil.h>
#include <gmemdbg.h>
#include <gstrall.h>
#include <gusrezyc.h>


//  ------------------------------------------------------------------

EzycomUser::EzycomUser() {

  ver = 102;

  extfh = -1;

  recsize = sizeof(EzycUsers);

  record = new EzycUsers;     throw_new(record);
  extrec = new EzycUsersExt;  throw_new(extrec);
  extrec110 = new EzycUsersExt110;  throw_new(extrec110);

  recptr = (char*)record;
  name = record->name;
}


//  ------------------------------------------------------------------

EzycomUser::~EzycomUser() {
  
  throw_delete(extrec110);
  throw_delete(extrec);
  throw_delete(record);
}


//  ------------------------------------------------------------------

void EzycomUser::inctimesposted(int __times) {

  if(extfh != -1) {

    if(ver >= 110) {
      memset(extrec110, 0, sizeof(EzycUsersExt110));
      lseek(extfh, (long)recno*(long)sizeof(EzycUsersExt110), SEEK_SET);
      ::read(extfh, extrec110, sizeof(EzycUsersExt110));
      extrec110->msgsposted += (word)__times;
      lseek(extfh, (long)recno*(long)sizeof(EzycUsersExt110), SEEK_SET);
      ::write(extfh, extrec110, sizeof(EzycUsersExt110));
    }
    else {
      memset(extrec, 0, sizeof(EzycUsersExt));
      lseek(extfh, (long)recno*(long)sizeof(EzycUsersExt), SEEK_SET);
      ::read(extfh, extrec, sizeof(EzycUsersExt));
      extrec->msgsposted += (word)__times;
      lseek(extfh, (long)recno*(long)sizeof(EzycUsersExt), SEEK_SET);
      ::write(extfh, extrec, sizeof(EzycUsersExt));
    }                        
  }
}


//  ------------------------------------------------------------------

int EzycomUser::isvalid() {

  return not (record->attribute & EZYC_USERDELETED);
}


//  ------------------------------------------------------------------

int EzycomUser::read()
{
  if (gufh != -1)
  {
    ::read(gufh, record, sizeof(EzycUsers));
    STRNP2C(record->name);

    return isvalid();
  }

  return false;
}


//  ------------------------------------------------------------------

void EzycomUser::recinit(const char* __name) {

  GUser::recinit(__name);
  strc2p(record->name);
}


//  ------------------------------------------------------------------

void EzycomUser::add(const char* __name) {

  GUser::add(__name);

  if(extfh != -1) {

    if(ver >= 110) {
      memset(extrec110, 0, sizeof(EzycUsersExt110));
      lseek(extfh, (long)recno*(long)sizeof(EzycUsersExt110), SEEK_SET);
      ::write(extfh, extrec110, sizeof(EzycUsersExt110));
    }
    else {
      memset(extrec, 0, sizeof(EzycUsersExt));
      lseek(extfh, (long)recno*(long)sizeof(EzycUsersExt), SEEK_SET);
      ::write(extfh, extrec, sizeof(EzycUsersExt));
    }
  }
}


//  ------------------------------------------------------------------
