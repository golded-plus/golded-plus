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
//  AdeptXBBS structures.
//  ------------------------------------------------------------------


//  ------------------------------------------------------------------
//  Only include once!

#ifndef __GMXBBS_H
#define __GMXBBS_H


//  ------------------------------------------------------------------
//  Required

#include <gmoarea.h>
#include <gusrxbbs.h>


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif


//  ------------------------------------------------------------------
//  Bitmaps for the header.fflags

#define FFLAGS_MSGPRIVATE  0x0001  // private message,
#define FFLAGS_MSGCRASH    0x0002  // accept for forwarding
#define FFLAGS_MSGREAD     0x0004  // read by addressee
#define FFLAGS_MSGSENT     0x0008  // sent OK (remote)
#define FFLAGS_MSGFILE     0x0010  // file attached to msg
#define FFLAGS_MSGFWD      0x0020  // being forwarded
#define FFLAGS_MSGORPHAN   0x0040  // unknown dest node
#define FFLAGS_MSGKILL     0x0080  // kill after mailing
#define FFLAGS_MSGLOCAL    0x0100  // FidoNet vs. local
#define FFLAGS_MSGXX1      0x0200  //
#define FFLAGS_MSGXX2      0x0400  // STRIPPED by FidoNet<tm>
#define FFLAGS_MSGFRQ      0x0800  // file request
#define FFLAGS_MSGRRQ      0x1000  // receipt requested
#define FFLAGS_MSGCPT      0x2000  // is a return receipt
#define FFLAGS_MSGARQ      0x4000  // audit trail requested
#define FFLAGS_MSGURQ      0x8000  // update request


//  ------------------------------------------------------------------
//  Bitmaps for the header.xflags

#define XFLAGS_MSGDELETED  0x0001  // deleted message,
#define XFLAGS_MSGANON     0x0002  // anonymous message
#define XFLAGS_MSGECHO     0x0004  // unmoved echo message
#define XFLAGS_MSGNET      0x0008  // unmoved net message

#define XFLAGS_MSGHOLD     0x0010  // file attached to msg
#define XFLAGS_MSGHOST     0x0020  // being forwarded
#define XFLAGS_MSGSCANNED  0x0040  // Fidonet scanned
#define XFLAGS_MSGKEEP     0x0080  // don't delete

#define XFLAGS_MSGTREATED  0x0100  // soft cr's & lf's removed
#define XFLAGS_MSGPACKED   0x0200  // message LZSS compressed
#define XFLAGS_MSGGSCAN    0x0400  // GroupMail scanned
#define XFLAGS_MSGRSCAN    0x0800  // rfc0822 scanned

#define XFLAGS_MSGARCHIVED 0x4000  // Message stored
#define XFLAGS_MSGTAGGED   0x8000  // used by offline readers


//  ------------------------------------------------------------------
//  AdeptXBBS Message Header (*.Data)

struct XbbsHdr {
  char    majorversion; // Minor revision number of this message style
  char    minorversion; // Minor revision number of this message style
  ushort  structlen;    // The length of this data structure
  char    from[60];     // Who the message is from
  char    to[60];       // Who the message is to
  char    subj[70];     // The subject of the message
  char    date[35];     // Date the message was written
  char    indate[4];    // Import date
  uint32_t   msgnum;       // Current message number
  uint32_t   timesread;    // Number of times the message has been read
  time32_t  timerecv;     // Time user received this message
  uint32_t   length;       // Length of message stored in .Text Data file
  int32_t    start;        // Pointer to starting byte in .Text Data file
  uint32_t   extra1;       // Extra space  // Was going to be for reply
  uint32_t   extra2;       // Extra space  // linking instead came up with
  uint32_t   extra3;       // Extra space  // a better method
  Addr    origaddr;     // Messages origin
  Addr    destaddr;     // Messages destination
  ushort  cost;         // Cost to send this message
  ushort  fflags;       // Fidonet related flags
  ushort  xflags;       // XBBS related flags
  uint32_t   iflags;       // Internet related flags
  uint32_t   oflags;       // Other network related flags
};


//  ------------------------------------------------------------------
//  AdeptXBBS Message Index (*.Index)

struct XbbsIdx {
  ushort  to;             // Checksum of the to field
  ushort  from;           // Checksum of the from field
  ushort  subj;           // Checksum of the subject field
  uint32_t   msgidcrc;       // MSGID 32-bit CRC of address field (starting value 0xFFFFFFFF)
  uint32_t   msgidserialno;  // MSGID Serial number field
  uint32_t   replycrc;       // REPLY 32-bit CRC of address field (starting value 0xFFFFFFFF)
  uint32_t   replyserialno;  // REPLY Serial number field
};


//  ------------------------------------------------------------------
//  AdeptXBBS Personal Mail Index

struct XbbsPmi {
  int32_t  areanumber;
  int32_t  msgnumber;
  char  from[60];
  char  subject[70];
  char  date[20];
};


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack()
#endif


//  ------------------------------------------------------------------

struct XbbsData {
  int      fhdata;
  int      fhindex;
  int      fhlmr;
  int      fhtext;
  int      islocked;
  XbbsIdx* idx;
  uint     idx_size;
};


//  ------------------------------------------------------------------

struct XbbsWide {
  int       fhpmi;
  int       userno;
  XbbsUser* user;
  XbbsPmi*  pmi;
  int       isopen;
  const char* path;
};


//  ------------------------------------------------------------------

class XbbsArea : public gmo_area {

protected:

  XbbsWide* wide;
  XbbsData* data;

  void data_open();
  void data_close();

  void raw_open();
  void raw_close();
  void refresh();
  int load_message(int __mode, gmsg* __msg, XbbsHdr& __hdr);

  void lock_file(int handle, int32_t position, int32_t length);
  void unlock_file(int handle, int32_t position, int32_t length);

  int test_open(const char* __file, int sharemode=-1);
  void save_lastread();
  void raw_scan(int __keep_index, int __scanpm=false);
  void save_message(int __mode, gmsg* __msg, XbbsHdr& __hdr);

  void update_personal_mail(gmsg* __msg, XbbsHdr& __hdr, int __status);

public:

  XbbsArea() { wide = NULL; data = NULL; }
  virtual ~XbbsArea() {}

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

extern XbbsWide* xbbswide;
extern XbbsData* xbbsdata;
extern int       xbbsdatano;


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
