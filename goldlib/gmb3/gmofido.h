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
//  Fido/Opus *.MSG
//  ------------------------------------------------------------------


//  ------------------------------------------------------------------
//  Only include once!

#ifndef __GMFIDO_H
#define __GMFIDO_H


//  ------------------------------------------------------------------

#include <gmoarea.h>
#include <gusrmax.h>


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif


//  ------------------------------------------------------------------

const uint FIDO_SCANBUFSIZE = 100;


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
//  Fido style message header (FTS-0001.015 compatible)

struct FidoHdr {
  char  by[36];
  char  to[36];
  char  re[72];
  char  datetime[20];
  word  timesread;
  word  destnode;
  word  orignode;
  word  cost;
  word  orignet;
  word  destnet;
  union {
    struct {
      word  destzone;
      word  origzone;
      word  destpoint;
      word  origpoint;
    } ftsc;
    struct {
      FTime  written;
      FTime  arrived;
    } opus;
  };
  word  replyto;
  word  attr;
  word  reply1st;
};


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack()
#endif


//  ------------------------------------------------------------------

struct FidoData {
  ulong highwatermark;
};


//  ------------------------------------------------------------------

struct FidoWide {
  MaximusUser* user;
  int   userno;
  int   fidohwmarks;
  int   fidonullfix;
  const char* fidolastread;
  const char* squishuserpath;
};


//  ------------------------------------------------------------------

class FidoArea : public gmo_area {

protected:

  FidoWide* wide;
  FidoData* data;

  inline bool isopus() const { return streql(cfg.basetype, "OPUS"); }

  void data_open();
  void data_close();

  char* build_msgname(char* __buf, ulong __msgno);
  int test_open(const char* __file, int __openmode, int __sharemode, int __fail=NO);
  void raw_scan(bool __scanpm);
  int load_message(int __mode, gmsg* __msg, FidoHdr& __hdr);
  void save_lastread();
  void save_message(int __mode, gmsg* __msg, FidoHdr& __hdr);

public:

  FidoArea() { wide = NULL; data = NULL; }
  virtual ~FidoArea() {}

  virtual bool havearrivedstamp() const { return isopus() ? true : false; }
  virtual bool havereceivedstamp() const { return false; }

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

  void set_highwater_mark();
  void reset_highwater_mark();
};


//  ------------------------------------------------------------------

extern FidoWide* fidowide;
extern FidoData* fidodata;
extern int       fidodatano;


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------

