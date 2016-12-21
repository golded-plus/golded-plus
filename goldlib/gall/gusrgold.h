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
//  Goldbase derived userfile class.
//  ------------------------------------------------------------------

#ifndef __gusrgold_h
#define __gusrgold_h


//  ------------------------------------------------------------------

#include <gusrbase.h>


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif


//  ------------------------------------------------------------------
//  Goldbase userfile (USERS.DAT) (QuickBBS 2.80 Gamma-3 structure)

struct GoldUsers {

  char  name[36];
  char  city[26];
  byte  reservedzero;
  byte  language;
  int32_t  pwdcrc;
  word  pwdchangedate;
  word  expiredate;
  int32_t  highmsgread;
  byte  extraspace[2];
  char  dataphone[13];
  char  homephone[13];
  char  lasttime[6];
  char  lastdate[9];
  byte  attrib;
  byte  flagsx[4];
  word  credit;
  word  pending;
  word  timesposted;
  word  obsoletefield;
  word  seclvl;
  word  times;
  word  ups;
  word  downs;
  word  upk;
  word  downk;
  short  todayk;
  short  elapsed;
  short  len;
  word  combinedptr;
  word  aliasptr;
  int32_t  birthday;
};


//  ------------------------------------------------------------------
//  Goldbase userfile attributes

#define GOLD_USERDELETED  0x0001


//  ------------------------------------------------------------------
//  Goldbase userfile class

class GoldbaseUser : public GUser {

public:

  GoldUsers* record;

  GoldbaseUser();
  ~GoldbaseUser();

  int  isvalid();
  int  read();
  
  void     inctimesposted(int __times);
  uint32_t lastread();
  void     lastread(uint32_t __lastread);
  void     recinit(const char* __name);
};


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack()
#endif


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
