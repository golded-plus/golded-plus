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
//  Ezycom derived userfile class.
//  ------------------------------------------------------------------

#ifndef __gusrezyc_h
#define __gusrezyc_h
//  ------------------------------------------------------------------

#include <gusrbase.h>
//  ------------------------------------------------------------------

#if defined (GOLD_CANPACK)
    #pragma pack(1)
#endif
//  ------------------------------------------------------------------
//  Ezycom USERS.BBS record structure
struct EzycUsers
{
    char name[36];
    char alias[36];
    char password[16];
    word security;
    byte attribute;
    byte attribute2;
    byte attribute3;
    byte attribute4;
    byte flagsx[4];
    char dataphone[15];
    char voicephone[15];
};

//  ------------------------------------------------------------------
//  Ezycom userfile attributes

#define EZYC_USERDELETED 0x0001
//  ------------------------------------------------------------------
//  Ezycom USERSEXT.BBS record structure
struct EzycUsersExt
{
    char location[26];
    long lasttimedate;
    word credit;
    word pending;
    word msgsposted;
    word nocalls;
    word uploads;
    word todayk;
    word timeused;
    word downloads;
    long uploadsk;
    long downloadsk;
    byte screenlength;
    byte lastpwdchange;
    word timebanked;
    word ksbanked;
    word filepoints;
    byte qwkcompression;
    byte qwkdaysold;
    byte comment[41];
    byte colour1_2;
    byte colour3_4;
    byte colour5_6;
    byte colour7_8;
    byte bkcolour;
    byte sessionfailures;
    byte topmenu[9];
    word filepointsgiven;
    struct
    {
        word year;
        byte month;
        byte day;
    }     dateofbirth;
    byte  groups[4];
    word  regodate;
    word  firstdate;
    word  lastfiledate;
    char  defprotocol;
    word  timeleft;
    word  filearea;
    word  messarea;
    word  qwkmaxmsgs;
    word  qwkmaxmsgsperarea;
    short todaybankwk;
    char  forwardto[36];
    byte  todaycalls;
    short todaybankwt;
    char  extraspace[4];
};

//  ------------------------------------------------------------------
//  Ezycom USERSEXT.BBS record structure, version 1.10
struct EzycUsersExt110
{
    char location[26];
    long lasttimedate;
    word credit;
    word pending;
    word msgsposted;
    word nocalls;
    word uploads;
    word todayk;
    word timeused;
    word downloads;
    long uploadsk;
    long downloadsk;
    byte screenlength;
    byte lastpwdchange;
    word timebanked;
    word ksbanked;
    word filepoints;
    byte qwkcompression;
    byte qwkdaysold;
    byte comment[41];
    byte colour1_2;
    byte colour3_4;
    byte colour5_6;
    byte colour7_8;
    byte bkcolour;
    byte sessionfailures;
    byte topmenu[9];
    word filepointsgiven;
    struct
    {
        word year;
        byte month;
        byte day;
    }     dateofbirth;
    byte  groups[4];
    word  regodate;
    word  firstdate;
    word  lastfiledate;
    char  defprotocol;
    word  timeleft;
    word  filearea;
    word  messarea;
    word  qwkmaxmsgs;
    word  qwkmaxmsgsperarea;
    short todaybankwk;
    char  forwardto[36];
    byte  todaycalls;
    short todaybankwt;
    byte  language;
    word  endregodate;
    long  tottimeused;
    char  extraspace[125];
};

//  ------------------------------------------------------------------
//  Ezycom userfile class
class EzycomUser : public GUser
{
public: int ver;
    int extfh;
    EzycUsers * record;
    EzycUsersExt * extrec;
    EzycUsersExt110 * extrec110;
    EzycomUser();
    ~EzycomUser();
    int isvalid();
    int read();
    void inctimesposted(int __times);
    void recinit(const char * __name);
    void add(const char * __name);
};
//  ------------------------------------------------------------------

#if defined (GOLD_CANPACK)
    #pragma pack()
#endif
//  ------------------------------------------------------------------

#endif // ifndef __gusrezyc_h
//  ------------------------------------------------------------------
