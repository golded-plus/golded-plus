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
//  WildCat! 4.0 Structures.
//  ------------------------------------------------------------------


//  ------------------------------------------------------------------
//  Only include once!

#ifndef __GMWCAT_H
#define __GMWCAT_H


//  ------------------------------------------------------------------

#include <gmoarea.h>
#include <gusrwcat.h>


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif


//  ------------------------------------------------------------------
//  WildCat! 4.0 Magic Numbers

const long MagicHeaderActive   = 0x001A1A1BL;
const long MagicHeaderInactive = 0x011A1A1BL;


//  ------------------------------------------------------------------
//  WildCat! 4.0 Message Attributes (mflags)

const word mfPrivate     = 0x0001;
const word mfReceiveable = 0x0002;
const word mfReceived    = 0x0004;
const word mfReceipt     = 0x0008;
const word mfCarboned    = 0x0010;
const word mfForwarded   = 0x0020;
const word mfEchoFlag    = 0x0040;
const word mfHasReplies  = 0x0100;
const word mfDeleted     = 0x0200;
const word mfTagged      = 0x0400;
const word mfSent        = 0x0800;


//  ------------------------------------------------------------------
//  Fido message attributes

#define FIDO_PVT        0x0001
#define FIDO_CRASH      0x0002
#define FIDO_RECEIVED   0x0004
#define FIDO_SENT       0x0008
#define FIDO_ATTACH     0x0010
#define FIDO_TRANSIT    0x0020
#define FIDO_ORPHAN     0x0040
#define FIDO_KILLSENT   0x0080
#define FIDO_LOCAL      0x0100
#define FIDO_HOLD       0x0200
#define FIDO_RESERVED   0x0400
#define FIDO_FREQ       0x0800
#define FIDO_RETRECREQ  0x1000
#define FIDO_RETREC     0x2000
#define FIDO_AUDITREQ   0x4000
#define FIDO_UPDREQ     0x8000


//  ------------------------------------------------------------------
//  WildCat! 4.0 Message Header

struct WCatHdr {
  long  magicnumber;
  word  msgno;
  char  from[71];
  char  fromtitle[11];
  long  fromuserid;
  char  to[71];
  char  totitle[11];
  long  touserid;
  char  subject[71];
  char  network[9];
  word  msgdate;
  long  msgtime;
  word  readdate;
  long  readtime;
  word  mflags;
  word  reference;
  Addr  origaddr;
  Addr  destaddr;
  word  msgbytes;
  char  internalattach[13];
  char  externalattach[13];
  word  prevunread;
  word  nextunread;
  word  fidoflags;
  long  cost;
  byte  reserved[20];
};

  
//  ------------------------------------------------------------------
//  WildCat! 4.0 Message Index Base Record

struct WCatBase {
  word recsize;
  word active;
  word nextmsgno;
};

  
//  ------------------------------------------------------------------
//  WildCat! 4.0 Message Index Record

struct WCatIdx {
  word msgno;
  long offset;
};


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack()
#endif


//  ------------------------------------------------------------------

struct WCatData {
  int      fhix;
  int      fhdat;
  WCatBase base;
  WCatIdx* idx;
  int      islocked;
};


//  ------------------------------------------------------------------

struct WCatWide {
  int          userno;
//  WildCatUser*  user;
};


//  ------------------------------------------------------------------

class WCatArea : public gmo_area {

protected:

  WCatWide* wide;
  WCatData* data;

  void data_open();
  void data_close();

  int test_open(const char* __file);
  void raw_open();
  void save_lastread();
  void raw_scan(int __keep_index, int __scanpm=false);
  void save_message(int __mode, gmsg* __msg, WCatHdr& __hdr);

  void raw_close();
  void refresh();
  int load_message(int __mode, gmsg* __msg, WCatHdr& __hdr);

public:

  WCatArea() { wide = NULL; data = NULL; }
  virtual ~WCatArea() {}

  virtual bool issoftdelete() const { return true; }
  virtual bool requirehardterm() const { return true; }

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

extern WCatWide* wcatwide;
extern WCatData* wcatdata;
extern int       wcatdatano;


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------

