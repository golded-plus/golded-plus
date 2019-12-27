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
//  RemoteAccess 2.x derived userfile class.
//  ------------------------------------------------------------------

#ifndef __gusrra2_h
#define __gusrra2_h


//  ------------------------------------------------------------------

#include <gusrbase.h>


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif


//  ------------------------------------------------------------------
//  RA2 USERS.BBS record structure
//  sizeof = 1016

struct RA2Users
{

    char  name[36];
    char  location[26];
    char  organisation[51];
    char  address1[51];
    char  address2[51];
    char  address3[51];
    char  handle[36];
    char  comment[81];
    int32_t  passwordcrc;
    char  dataphone[16];
    char  voicephone[16];
    char  lasttime[6];
    char  lastdate[9];
    byte  attribute;
    byte  attribute2;
    byte  flagsx[4];
    int32_t  credit;
    int32_t  pending;
    word  msgsposted;
    word  security;
    int32_t  lastread;
    int32_t  nocalls;
    int32_t  uploads;
    int32_t  downloads;
    int32_t  uploadsk;
    int32_t  downloadsk;
    int32_t  todayk;
    short  elapsed;
    word  screenlength;
    byte  lastpwdchange;
    word  group;
    word  combinedinfo[200];
    char  firstdate[9];
    char  birthdate[9];
    char  subdate[9];
    byte  screenwidth;
    byte  language;
    byte  dateformat;
    char  forwardto[36];
    word  msgarea;
    word  filearea;
    char  defaultprotocol;
    word  filegroup;
    byte  lastdobcheck;
    byte  sex;
    int32_t  xirecord;
    word  msggroup;
    byte  freespace[48];
};


//  ------------------------------------------------------------------
//  RA2 userfile attributes

#define RA2_USERDELETED 0x0001


//  ------------------------------------------------------------------
//  RA2 USERSIDX.BBS record structure

struct RA2UsersIdx
{

    dword  namecrc32;
    dword  handlecrc32;
};


//  ------------------------------------------------------------------
//  RA2 USERSXI.BBS record structure

struct RA2UsersXi
{

    byte  freespace[200];
};


//  ------------------------------------------------------------------
//  RemoteAccess 2.x userfile class

class RA2User : public GUser
{
public:

    gfile* xifh;
    gfile* idxfh;

    RA2Users*    record;
    RA2UsersXi*  xirec;
    RA2UsersIdx* idxrec;

    RA2User();
    ~RA2User();

    int  isvalid();
    int  read();

    void     inctimesposted(int __times);
    uint32_t lastread();
    void     lastread(uint32_t __lastread);
    void     recinit(const char* __name);

    void  add(const char* __name);
};


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack()
#endif


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
