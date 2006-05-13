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
//  RemoteAccess 2.x userfile class implementation.
//  ------------------------------------------------------------------

#include <gfile.h>
#include <gcrcall.h>
#include <gmemdbg.h>
#include <gstrall.h>
#include <gusrra2.h>


//  ------------------------------------------------------------------

RA2User::RA2User()
{
  xifh = idxfh = 0;

  recsize = sizeof(RA2Users);

  record = new RA2Users;     throw_new(record);
  xirec  = new RA2UsersXi;   throw_new(xirec);
  idxrec = new RA2UsersIdx;  throw_new(idxrec);

  recptr = (char*)record;
  name = record->name;
}


//  ------------------------------------------------------------------

RA2User::~RA2User() {
  
  throw_delete(record);
  throw_delete(idxrec);
  throw_delete(xirec);
}


//  ------------------------------------------------------------------

void RA2User::inctimesposted(int __times) {
  
  seekread();
  record->msgsposted += (word)__times;
  seekwrite();
}


//  ------------------------------------------------------------------

int RA2User::isvalid() {

  return not (record->attribute & RA2_USERDELETED);
}


//  ------------------------------------------------------------------

int RA2User::read()
{
  if (gufh != -1)
  {
    ::read(gufh, record, sizeof(RA2Users));
    STRNP2C(record->name);

    return isvalid();
  }

  return false;
}


//  ------------------------------------------------------------------

uint32_t RA2User::lastread() {

  seekread();
  return record->lastread;
}


//  ------------------------------------------------------------------

void RA2User::lastread(uint32_t __lastread) {
  
  seekread();
  record->lastread = (long)__lastread;
  seekwrite();
}


//  ------------------------------------------------------------------

void RA2User::recinit(const char* __name) {

  GUser::recinit(__name);
  strc2p(record->name);
  strcpy(record->handle, __name);
  strc2p(record->handle);
}


//  ------------------------------------------------------------------

void RA2User::add(const char* __name) {

  GUser::add(__name);

  if (idxfh && idxfh->isopen())
  {
    char _namebuf[36];
    strupr(strcpy(_namebuf, __name));
    idxrec->namecrc32 = idxrec->handlecrc32 = strCrc32(_namebuf, NO, CRC32_MASK_CCITT);

    idxfh->LseekSet((long)recno*(long)sizeof(RA2UsersIdx));
    idxfh->Write(idxrec, sizeof(RA2UsersIdx));
  }

  if (xifh && xifh->isopen())
  {
    memset(xirec, 0, sizeof(RA2UsersXi));

    xifh->LseekSet((long)recno*(long)sizeof(RA2UsersXi));
    xifh->Write(xirec, sizeof(RA2UsersXi));
  }
}


//  ------------------------------------------------------------------
