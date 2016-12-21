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
//  Hudson derived userfile class.
//  ------------------------------------------------------------------

#ifndef __gusrhuds_h
#define __gusrhuds_h


//  ------------------------------------------------------------------

#include <gusrbase.h>


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif


//  ------------------------------------------------------------------
//  Hudson userfile (USERS.BBS) (QuickBBS 2.80 Gamma-3 structure)
//  sizeof = 158

struct HudsUsers {

  char  name[36];
  char  city[26];
  byte  reservedzero;
  byte  language;
  int32_t  pwdcrc;
  word  pwdchangedate;
  word  expiredate;
  int32_t  unusedspace;
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
  word  highmsgread;
  word  seclvl;
  word  times;
  word  ups;
  word  downs;
  word  upk;
  word  downk;
  short  todayk;
  short  elapsed;
  short  len;
  word  combinedptr;  // record number in COMBINED.*
  word  aliasptr;     // record number in ALIAS.*
  int32_t  birthday;
};


//  ------------------------------------------------------------------
//  Hudson userfile attributes

#define HUDS_USERDELETED  0x0001


//  ------------------------------------------------------------------
//  Hudson userfile class

class HudsonUser : public GUser {

public:

  HudsUsers* record;

  HudsonUser();
  ~HudsonUser();

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
