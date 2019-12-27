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
//  Ezycom msgbase.
//  ------------------------------------------------------------------


//  ------------------------------------------------------------------
//  Only include once!

#ifndef __GMEZYC_H
#define __GMEZYC_H


//  ------------------------------------------------------------------

#include <gmoarea.h>
#include <gusrezyc.h>

#if defined(GOLD_CANPACK)
    #pragma pack(1)
#endif


//  ------------------------------------------------------------------
//  Ezycom constants

#define EZYC_MAXNAME      36
#define EZYC_MAXSUBJ      73
#define EZYC_MAXMESS102 1024
#define EZYC_MAXMESS110 1536
#define EZYC_USERSBUF    100

#define EZYC_USERDELETED 0x0001


//  ------------------------------------------------------------------

const byte EZYC_MSGATTR_DELETED  = 0x01;
const byte EZYC_MSGATTR_NETPEND  = 0x02;
const byte EZYC_MSGATTR_RESERVED = 0x04;
const byte EZYC_MSGATTR_PRIVATE  = 0x08;
const byte EZYC_MSGATTR_RECEIVED = 0x10;
const byte EZYC_MSGATTR_ECHOPEND = 0x20;
const byte EZYC_MSGATTR_LOCAL    = 0x40;
const byte EZYC_MSGATTR_NOKILL   = 0x80;

const byte EZYC_NETATTR_KILLSENT = 0x01;
const byte EZYC_NETATTR_SENT     = 0x02;
const byte EZYC_NETATTR_ATTACH   = 0x04;
const byte EZYC_NETATTR_CRASH    = 0x08;
const byte EZYC_NETATTR_FREQ     = 0x10;
const byte EZYC_NETATTR_RREQ     = 0x20;
const byte EZYC_NETATTR_AREQ     = 0x40;
const byte EZYC_NETATTR_RREC     = 0x80;

const byte EZYC_EXTATTR_RSVD1    = 0x01;
const byte EZYC_EXTATTR_RSVD2    = 0x02;
const byte EZYC_EXTATTR_RSVD3    = 0x04;
const byte EZYC_EXTATTR_RSVD4    = 0x08;
const byte EZYC_EXTATTR_RSVD5    = 0x10;
const byte EZYC_EXTATTR_RSVD6    = 0x20;
const byte EZYC_EXTATTR_RSVD7    = 0x40;
const byte EZYC_EXTATTR_SEEN     = 0x80;


//  ------------------------------------------------------------------
//  Ezycom message header

struct EzycHdr
{

    word    replyto;
    word    reply1st;

    uint32_t   startposition;
    uint32_t   messagelength;

    Addr    destnet;
    Addr    orignet;

    word    cost;           // redefine to replynext?

    byte    msgattr;
    byte    netattr;
    byte    extattr;

    FTime   posttimedate;
    FTime   recvtimedate;

    char    whoto[EZYC_MAXNAME];
    char    whofrom[EZYC_MAXNAME];
    char    subject[EZYC_MAXSUBJ];
};


//  ------------------------------------------------------------------
//  Ezycom LASTCOMB.BBS record structure

struct EzycLast
{
    word combinedinfo;
    word lastreadinfo[16];
};


//  ------------------------------------------------------------------
//  Ezycom MSGFAST.BBS record structure

struct EzycFast
{
    dword whoto;       // 32 bit CRC on WhoTo in MSGHxxx.BBS
    word msgboard;
    word msgnumber;
};


//  ------------------------------------------------------------------
//  Ezycom MSGDLTD.BBS record

struct EzycDeleted
{
    word msgboard;
    word msgnumber;
};


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
    #pragma pack()
#endif


//  ------------------------------------------------------------------

struct EzycData
{
    int fhhdr;
    int fhtxt;
    int fhnow;
    int omode;
    int smode;
    int timesposted;
    int islocked;
    Path ezyfile;
};


//  ------------------------------------------------------------------

struct EzycWide
{
    int         ver;
    EzycomUser* user;
    int         userno;
    int         maxmess;
    const char* msgbasepath;
    const char* userbasepath;
};


//  ------------------------------------------------------------------

class EzycomArea : public gmo_area
{

protected:

    EzycWide* wide;
    EzycData* data;

    void data_open();
    void data_close();

    char* ret_mess_xxx(char* __path, byte __type);
    char* ret_mess_area(char* __path);

    int raw_open();

    void test_raw_open(int __fileline);
    void raw_close();
    int test_open(const char* __file, int __mode, int __share);

    void save_lastread();

    void raw_scan(int __keep_index);

    int load_message(int __mode, gmsg* __msg, EzycHdr& __hdr);
    void save_message(int __mode, gmsg* __msg, EzycHdr& __hdr);


public:

    EzycomArea()
    {
        wide = NULL;
        data = NULL;
    }
    virtual ~EzycomArea() {}

    virtual bool havereceivedstamp() const
    {
        return false;
    }
    virtual bool requiresoftterm() const
    {
        return true;
    }

    //  ----------------------------------------------------------------
    //  Messagebase member functions

    void open();
    void close();

    void suspend();
    void resume();

    void lock();
    void unlock();

    void scan();
    void scan_area();
    void scan_area_pm();

    int load_hdr(gmsg* msg);
    int load_msg(gmsg* msg);

    void save_hdr(int mode, gmsg* msg);
    void save_msg(int mode, gmsg* msg);

    void del_msg(gmsg* msg);

    void new_msgno(gmsg* msg);
    char* user_lookup(char* lookfor);
    int renumber();

    void update_timesread(gmsg* msg);

    Line* make_dump_msg(Line*& lin, gmsg* msg, char* lng_head);
};


//  ------------------------------------------------------------------

extern EzycWide* ezycomwide;
extern EzycData* ezycomdata;
extern int       ezycomdatano;


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------

