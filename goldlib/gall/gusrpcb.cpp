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
//  PCBoard 15.xx userfile class implementation.
//  ------------------------------------------------------------------

#include <gfilutil.h>
#include <gcrcall.h>
#include <gmemdbg.h>
#include <gstrall.h>
#include <gusrpcb.h>


//  ------------------------------------------------------------------

PcbUser::PcbUser() {

  fhinf = -1;
  recsize = sizeof(PcbUsers);
  record = new PcbUsers;     throw_new(record);
  recptr = (char*)record;
  name = record->name;
}


//  ------------------------------------------------------------------

PcbUser::~PcbUser() {
  
  throw_delete(record);
}


//  ------------------------------------------------------------------

int PcbUser::isvalid() {

  return record->deleteflag == 'N';
}


//  ------------------------------------------------------------------

int PcbUser::read() {

  if(fh != -1) {
    ::read(fh, record, sizeof(PcbUsers));
    return isvalid();
  }

  return false;
}


//  ------------------------------------------------------------------

void PcbUser::recinit(const char* __name) {

  GUser::recinit(__name);
}


//  ------------------------------------------------------------------

void PcbUser::add(const char*) {

}


//  ------------------------------------------------------------------

void PcbUser::update_mail_waiting(const char* __name, int __confno, int __status) {

  if(find(__name)) {
    byte bitmask = 0x04;
    if(__status)
      record->bitflags |= bitmask;
    else
      record->bitflags &= (byte)~bitmask;
    seekwrite();
    long _offset = (record->usersinfrec-1)*usershdr->totalrecsize;
    _offset += usershdrsize;
    _offset += usershdr->sizeofrec;
    _offset += __confno / 8;
    lseek(fhinf, _offset, SEEK_SET);
    byte mailwaitingflags = 0;
    ::read(fhinf, &mailwaitingflags, 1);
    bitmask = (byte)(1 << (__confno % 8));
    if(__status)
      mailwaitingflags |= bitmask;
    else
      mailwaitingflags &= (byte)~bitmask;
    lseek(fhinf, _offset, SEEK_SET);
    ::write(fhinf, &mailwaitingflags, 1);
  }
}


//  ------------------------------------------------------------------
