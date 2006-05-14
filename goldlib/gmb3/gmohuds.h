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
//  Hudson.
//  ------------------------------------------------------------------


//  ------------------------------------------------------------------
//  Only include once!

#ifndef __GMHUDS_H
#define __GMHUDS_H


//  ------------------------------------------------------------------
//  Required

#include <gmoarea.h>
#include <gusrhuds.h>
#include <gusrra2.h>
#include <gusrgold.h>

//  ------------------------------------------------------------------

#include <limits.h>
#include <stdlib.h>
#include <gdbgerr.h>
#include <stdlib.h>
#include <gmemdbg.h>
#include <gdbgtrk.h>
#include <gstrall.h>
#include <gmemall.h>
#include <gutlmisc.h>


//  ------------------------------------------------------------------

#define HUDS_DELETEDMSGNO   0xFFFFu
#define HUDS_MAXBOARD       200
#define HUDS_EXT            ".bbs"
#define HUDS_NAME           "Hudson"

#define GOLD_DELETEDMSGNO   0xFFFFFFFFL
#define GOLD_MAXBOARD       500
#define GOLD_EXT            ".dat"
#define GOLD_NAME           "Goldbase"


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif


//  ------------------------------------------------------------------
//  Hudson Message Attributes

#define HUDS_DELETED    0x01
#define HUDS_NETTRANS   0x02
#define HUDS_NETMSG     0x04
#define HUDS_PVT        0x08
#define HUDS_RECEIVED   0x10
#define HUDS_ECHOTRANS  0x20
#define HUDS_LOCAL      0x40
#define HUDS_GROUPMSG   0x80

#define HUDS_KILLSENT   0x01
#define HUDS_SENT       0x02
#define HUDS_ATTACH     0x04
#define HUDS_CRASH      0x08
#define HUDS_RETRECREQ  0x10
#define HUDS_AUDITREQ   0x20
#define HUDS_RETREC     0x40
#define HUDS_FREQ       0x80


//  ------------------------------------------------------------------
//  Hudson Message Header (MSGHDR.BBS)

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t>
struct _HudsHdr {
  msgn_t  msgno;
  msgn_t  replyto;
  msgn_t  reply1st;
  word    timesread;
  rec_t   startrec;
  word    numrecs;
  word    destnet;
  word    destnode;
  word    orignet;
  word    orignode;
  byte    destzone;
  byte    origzone;
  word    cost;
  attr_t  msgattr;
  attr_t  netattr;
  board_t board;
  char    time[6];
  char    date[9];
  char    to[36];
  char    by[36];
  char    re[73];
} __attribute__ ((packed));


//  ------------------------------------------------------------------
//  Hudson Messagebase Information (MSGINFO.BBS)

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t>
struct _HudsInfo {
  msgn_t low;
  msgn_t high;
  msgn_t total;
  last_t active;
} __attribute__ ((packed));


//  ------------------------------------------------------------------
//  Hudson Message Header Index (MSGIDX.BBS)

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t>
struct _HudsIdx {
  msgn_t  msgno;
  board_t board;
} __attribute__((packed));


//  ------------------------------------------------------------------
//  Hudson Message Name Index (MSGTOIDX.BBS)

struct HudsToIdx {
  char  name[36];
} __attribute__((packed));


//  ------------------------------------------------------------------

typedef word HudsLast[HUDS_MAXBOARD]
#ifndef __DJGPP__
 __attribute__((packed))
#endif
;
typedef word GoldLast[GOLD_MAXBOARD]
#ifndef __DJGPP__
 __attribute__((packed))
#endif
;


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack()
#endif


//  ------------------------------------------------------------------

struct HudsScan {
  uint count;
  uint active;
  uint32_t lastread;
  uint lastreadreln;
  uint32_t lastreadfound;
  uint32_t firstmsgno;
  uint32_t lastmsgno;
  uint pmcount;
};


//  ------------------------------------------------------------------


template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
struct _HudsWide {

#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif
  // <class msgn_t, class rec_t, class attr_t, class board_t, class last_t>
  typedef _HudsHdr<msgn_t, rec_t, attr_t, board_t, last_t> HudsHdr;
  typedef _HudsInfo<msgn_t, rec_t, attr_t, board_t, last_t> HudsInfo;
  typedef _HudsIdx<msgn_t, rec_t, attr_t, board_t, last_t> HudsIdx;
#if defined(GOLD_CANPACK)
#pragma pack()
#endif

  gfile    fhtxt;
  gfile    fhhdr;
  gfile    fhidx;
  gfile    fhinf;
  gfile    fhlrd;
  gfile    fhtoi;
  gfile    fhusr;
  gfile    fhuix;
  gfile    fhuxi;
  int      isopen;
  int      islocked;
  int      timesposted;
  int32_t     msgidxsize;
  HudsIdx* msgidxptr;
  HudsInfo msginfo;
  last_t   lastrec;
  GUser*   user;
  int      userno;
  HudsScan* scn;
  HudsIdx* pmscan;
  uint     pmscantotal;
  int      iswideopen;
  int      ispmscanned;
  int      iswidescanned;
  const char* path;
  const char* syspath;
  int32_t sizewarn;
  int  ra2usersbbs;

  void init();
  void exit();

  void open();
  void close();

  void lock();
  void unlock();

  void scan();

  void raw_open_scan();
  void scan_pm();

  void realloc_pm_scan();

  void update_netecho(char* __name, msgn_t __hdridx, int __delete);

  void test_open(gfile &__file, char* __fname, int __oaccess);
  void raw_open(int __oaccess=0, int __all=true);
  void raw_close();
  void refresh();
  void save_lastread(board_t board, msgn_t msgno);
};


//  ------------------------------------------------------------------

template <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
class _HudsArea : public gmo_area {

  // <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
  typedef _HudsWide<msgn_t, rec_t, attr_t, board_t, last_t, __HUDSON> HudsWide;

#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif
  // <class msgn_t, class rec_t, class attr_t, class board_t, class last_t>
  typedef _HudsHdr<msgn_t, rec_t, attr_t, board_t, last_t> HudsHdr;
  typedef _HudsInfo<msgn_t, rec_t, attr_t, board_t, last_t> HudsInfo;
  typedef _HudsIdx<msgn_t, rec_t, attr_t, board_t, last_t> HudsIdx;
#if defined(GOLD_CANPACK)
#pragma pack()
#endif

protected:

  HudsWide* wide;

  void data_open();
  void data_close();

  void raw_scan(int __keep_index);

  msgn_t get_hdr_idx(gmsg* __msg, char* __file__, int __line__);

  int load_message(int __mode, gmsg* __msg, HudsHdr& __hdr);
  void save_message(int __mode, gmsg* __msg, HudsHdr& __hdr);

public:

  _HudsArea() { wide = NULL; }
  virtual ~_HudsArea() {}

  virtual bool havearrivedstamp() const { return false; }
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
};


//  ------------------------------------------------------------------

// <class msgn_t, class rec_t, class attr_t, class board_t, class last_t, bool __HUDSON>
typedef _HudsWide<word, word, byte, byte, HudsLast, true> HudsWide;
typedef _HudsWide<dword, int32_t, word, word, GoldLast, false> GoldWide;
typedef _HudsArea<word, word, byte, byte, HudsLast, true> HudsArea;
typedef _HudsArea<dword, int32_t, word, word, GoldLast, false> GoldArea;


//  ------------------------------------------------------------------

extern HudsWide* hudsonwide;
extern GoldWide* goldbasewide;


//  ------------------------------------------------------------------

#include <gmohuds1.cpp>
#include <gmohuds2.cpp>
#include <gmohuds3.cpp>
#include <gmohuds4.cpp>
#include <gmohuds5.cpp>


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------

