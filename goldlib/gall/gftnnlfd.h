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
//  FrontDoor Nodelist Indexing.
//  Derived from code kindly provided by Scott Dudley.
//  ------------------------------------------------------------------

#ifndef __gftnnlfd_h
#define __gftnnlfd_h


//  ------------------------------------------------------------------

#include <gftnnl.h>


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif

struct _FDHdr {
  word rsvd1;
  byte rsvd2;
  word master_idx;
};


//  ------------------------------------------------------------------
//  The four-byte header at the beginning of each Btree block

struct _FDInf {
  char nodes;
  word index;
  char rsvd[2];
};


//  ------------------------------------------------------------------
//  Layout of first record

struct _FDCtl {
  _FDInf inf;
  _FDHdr hdr;
  char   stuff[246];
  byte   unknown;
  char   nl_ext[3];
};


//  ------------------------------------------------------------------
//  Masks for the nlofs member

const ulong IN_FDNODE  = 0x01000000UL;  // Data in FDNODE.FD
const ulong IN_FDNET   = 0x10000000UL;  // Data in FDNET.PVT
const ulong IN_FDPOINT = 0x20000000UL;  // Data in FDPOINT.PVT


//  ------------------------------------------------------------------

struct _FDGdx {
  dword nlofs;
  word  block_num;
};


//  ------------------------------------------------------------------
//  A FrontDoor USERLIST.FDX record

struct _FDUdx {
  dword nlofs;                     // Index into raw nodelist.
  word  block_num;                 // Block number (used in index recs only)
  word  rsvd1;                     // Unknown
  byte  rsvd2;                     // Unknown - always '\x18'
  char  name[15];                  // Space-padded, uppercase
  byte  zone_hi;                   // MSB of zone
  byte  zone_lo;                   // LSB of zone
  byte  net_hi;                    // MSB of net
  byte  net_lo;                    // LSB of net
  byte  node_hi;                   // MSB of node
  byte  node_lo;                   // LSB of node
  byte  point_hi;                  // MSB of point
  byte  point_lo;                  // LSB of point
  byte  type;                      // See TYPE_XXX
};


//  ------------------------------------------------------------------
//  Structure of a B-Tree data block in USERLIST.FDX. This structure
//  is always 1061 bytes long, and the file length is always a
//  multiple of this

struct _FDUdb {
  _FDInf inf;
  _FDUdx udx[32];
};


//  ------------------------------------------------------------------
//  A FrontDoor NODELIST.FDX record

struct _FDFdx {
  dword nlofs;                     // Index into raw nodelist.
  word  block_num;                 // Block number (used in index recs only)
  word  rsvd1;                     // Unknown
  byte  rsvd2;                     // Unknown - always '\x0e'
  byte  zone_hi;                   // MSB of zone
  byte  zone_lo;                   // LSB of zone
  byte  net_hi;                    // MSB of net
  byte  net_lo;                    // LSB of net
  byte  node_hi;                   // MSB of node
  byte  node_lo;                   // LSB of node
  byte  point_hi;                  // MSB of point
  byte  point_lo;                  // LSB of point
  word  host_route;                // Net# for host routng, or 0 if none
  word  hub_route;                 // Hub# for hub routing, or 0 if none
  byte  type;                      // See TYPE_XXX
  byte  sentinel;                  // Always 0xFF
};


//  ------------------------------------------------------------------
//  Structure of a B-Tree data block in NODELIST.FDX. This structure
//  is always 741 bytes long, and the file length is always a multiple
//  of this

struct _FDFdb {
  _FDInf inf;
  _FDFdx fdx[32];
};


//  ------------------------------------------------------------------
//  B-Tree common data block with union for convenience

struct _FDBlk {
  _FDInf info;
  union {
    _FDUdx name[32];
    _FDFdx addr[32];
  } node;
};


//  ------------------------------------------------------------------
//  The private Nodelist Database (FDNODE.FDA) has the following record
//  format. Note that the char[] fields are in Pascal fashion. The first
//  byte is the length byte. The actual string starts at [1] and the
//  string is not NUL terminated.

// Status
#define ISZC        1
#define ISRC        2
#define ISNC        3
#define ISHUB       4
#define ISPVT       5
#define ISHOLD      6
#define ISDOWN      7
#define ISPOINT     9

// Capability flags
#define CMflag      0x00000002L
#define MOflag      0x00000004L
#define HSTflag     0x00000008L
#define H96flag     0x00000010L
#define PEPflag     0x00000020L
#define MAXflag     0x00000040L
#define XXflag      0x00000080L
#define XBflag      0x00000100L
#define XRflag      0x00000200L
#define XPflag      0x00000400L
#define XWflag      0x00000800L
#define MNPflag     0x00001000L
#define HST14flag   0x00002000L
#define V32flag     0x00004000L
#define V33flag     0x00008000L
#define V34flag     0x00010000L
#define V42flag     0x00020000L
#define XCflag      0x00040000L
#define XAflag      0x00080000L
#define V42bflag    0x00100000L
#define V32bflag    0x00200000L
#define HST16flag   0x00400000L
#define LOflag      0x00800000L
#define ZYXflag     0x01000000L
#define UISDNAflag  0x02000000L
#define UISDNBflag  0x04000000L
#define UISDNCflag  0x08000000L
#define FAXflag     0x10000000L

// MaxBaud field
#define ISBAUD300        2
#define ISBAUD1200       4
#define ISBAUD2400       5
#define ISBAUD4800       6
#define ISBAUD7200      10
#define ISBAUD9600       7
#define ISBAUD12000     11
#define ISBAUD14400     12
#define ISBAUD16800     13
#define ISBAUD19200     14
#define ISBAUD38400     15
#define ISBAUD57600     16
#define ISBAUD64000     17
#define ISBAUD76800     18
#define ISBAUD115200    19

// Record structure
struct _FDFdn {
  long erased;            // Used to signal erased status
  byte status;            // Zone, host, hub, etc.
  word node;              // Network address
  word net;               //
  word zone;              //
  word point;             //
  word routnode;          // Default routing within zone
  word routnet;           //
  word cost;              // Cost per minute for system
  long capability;        // Capability flags
  byte maxbaud;           // Maximum baud rate
  char name[31];          // Name of system
  char telephone[41];     // Raw telephone number
  char location[41];      // Location of system
  char user[37];          // SysOp name
  char selecttag[4];      // Group field
};


//  ------------------------------------------------------------------
//

struct _FDStk {
  uint blockno;
  uint block_num;
  int  maxnodes;
  int  node;
};


//  ------------------------------------------------------------------

struct _FDstate {
  uint   depth;
  _FDStk stack[8];
  int    blockno;
  int    node;
};

#if defined(GOLD_CANPACK)
#pragma pack()
#endif


//  ------------------------------------------------------------------

class ftn_frontdoor_nodelist_index : public ftn_nodelist_index_base {

  bool          is_intermail;

  int           fdfd;
  int           nfd;
  int           pfd;
  int           ppfd;
  int           ufd;
  int           xfd;

  _FDCtl        ctl;

  _FDBlk        block;
  uint          blocksize;
  uint          blockno;
  uint          maxblockno;
  uint          lastblockno;
  int           node;

  _FDStk        stack[8];
  uint          depth;

  _FDstate      state;

  char          nodelist[13];
  long          nodelistoffset;

  char          searchname[16];
  ftn_addr      searchaddr;

  const _FDUdx& namerec() const;
  const _FDFdx& addrrec() const;
  const _FDGdx& noderec() const;
  void          getaddr(ftn_addr& addr) const;
  void          getblock();
  int           namecmp() const;
  int           addrcmp() const;
  void          getstatus(char* status, int type) const;
  void          fetchdata();
  void          getnodedata();
  void          push();
  void          pop();
  bool          prevnode();
  bool          nextnode();
  void          compare()               { exactmatch = not (namebrowse ? namecmp() : addrcmp()); }
  bool          search();

  #ifdef DEBUG
  void          printnode() const;
  #endif

public:

  ftn_frontdoor_nodelist_index();
  virtual ~ftn_frontdoor_nodelist_index();

  bool can_browse_name() const     { return true; }
  bool can_browse_address() const  { return true; }

  bool open();
  void close();

  bool find(const char* name);
  bool find(const ftn_addr& addr);

  bool previous();
  bool next();

  void first();
  void last();

  void push_state();
  void pop_state();

  const char* index_name() const;
  const char* nodelist_name() const;

};


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
