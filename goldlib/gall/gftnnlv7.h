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
//  Version 7 nodelist index.
//  Derived from the Binkley 2.50 source code.
//  V7+ support based on draft #2 by Thomas Waldmann.
//  ------------------------------------------------------------------

#ifndef __gftnnlv7_h
#define __gftnnlv7_h


//  ------------------------------------------------------------------

#include <gftnnl.h>


//  -------------------------------------------------------------------
//  Version 7 Nodelist Index structure. This is a 512-byte record,
//  which is defined by three structures: Record 0 is the Control
//  Record, then some number of Leaf Node (LNode) Records, then the
//  Index Node (INode) Records. This defines an unbalanced binary
//  tree.
//
//  This description is based on Scott Samet's CBTREE.PAS program.
//
//  Used for NODEX.NDX and SYSOP.NDX
//  ------------------------------------------------------------------


#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif

//  ------------------------------------------------------------------
//  Index Node records

struct _V7IndxRef {

  word  indxofs;    // Offset of string into block
  word  indxlen;    // Length of string
  long  indxdata;   // Record number of string
  long  indxptr;    // Block number of lower index
};


//  ------------------------------------------------------------------
//  Leaf Node records

struct _V7LeafRef {

  word  keyofs;   // Offset of string into block
  word  keylen;   // Length of string
  long  keyval;   // Pointer to data block
};


//  ------------------------------------------------------------------
//  Index records

struct _V7Ndx {

  union {

    // Control Record
    struct {
      word        ctlblksize;   // Blocksize of Index Blocks
      long        ctlroot;      // Block number of Root
      long        ctlhiblk;     // Block number of last block
      long        ctlloleaf;    // Block number of first leaf
      long        ctlhileaf;    // Block number of last leaf
      long        ctlfree;      // Head of freelist
      word        ctllvls;      // Number of index levels
      word        ctlparity;    // XOR of above fields
    } ctlblk;

    // Index Node records
    struct {
      long        indxfirst;    // Pointer to next lower level
      long        indxblink;    // Pointer to previous link
      long        indxflink;    // Pointer to next link
      short       indxcnt;      // Count of Items in block
      word        indxstr;      // Offset in block of 1st str
      _V7IndxRef  indxref[20];  // If IndxFirst is not -1, this is INode
    } inodeblk;

    // Leaf Node records
    struct {
      long        indxfirst;    // Pointer to next lower level (is -1 in LNodes)
      long        indxblink;    // Pointer to previous link
      long        indxflink;    // Pointer to next link
      short       indxcnt;      // Count of Items in block
      word        indxstr;      // Offset in block of 1st str
      _V7LeafRef  leafref[30];
    } lnodeblk;

    // Raw index data
    char rawndx[512];

  } ndx;
};


//  ------------------------------------------------------------------
//  OPUS 1.20 Version 7 Nodelist structure.
//  Copyright 1991 Wynn Wagner III and Doug Boone.

struct _V7Data {

  word zone;           // Zone
  word net;            // Net
  word node;           // Node
  word hubnode;        // Point (if nodeflags&V7_B_point) or hubnode number
  word callcost;       // Phone company's charge
  word msgfee;         // Amount charged to user for a message
  word nodeflags;      // Set of flags (see below)
  byte modemtype;      // RESERVED for modem type
  byte phone_len;      // Length of phone number
  byte password_len;   // Length of password
  byte bname_len;      // Length of system name
  byte sname_len;      // Length of sysop name
  byte cname_len;      // Length of location
  byte pack_len;       // Length of packed data
  byte baudrate;       // baud rate divided by 300
};


//  ------------------------------------------------------------------
//  Values for the `_V7Data.nodeflags' field
//  ------------------------------------------------------------------

#define V7_B_Hub    0x0001  // node is a net hub     00000000 00000001
#define V7_B_Host   0x0002  // node is a net host    00000000 00000010
#define V7_B_Region 0x0004  // node is region coord  00000000 00000100
#define V7_B_Zone   0x0008  // is a zone gateway     00000000 00001000
#define V7_B_CM     0x0010  // runs continuous mail  00000000 00010000
#define V7_B_Res1   0x0020  // reserved by Opus      00000000 00100000
#define V7_B_Res2   0x0040  // reserved by Opus      00000000 01000000
#define V7_B_Res3   0x0080  // reserved by Opus      00000000 10000000

#define V7_B_Res4   0x0100  // reserved by Opus      00000001 00000000
#define V7_B_Res5   0x0200  // reserved for non-Opus 00000010 00000000
#define V7_B_Res6   0x0400  // reserved for non-Opus 00000100 00000000
#define V7_B_Res7   0x0800  // reserved for non-Opus 00001000 00000000
#define V7_B_Point  0x1000  // node is a point       00010000 00000000
#define V7_B_Res9   0x2000  // reserved for non-Opus 00100000 00000000
#define V7_B_Res10  0x4000  // reserved for non-Opus 01000000 00000000
#define V7_B_Plus   0x8000  // V7+ field length list 10000000 00000000

#define V7_Hub     0
#define V7_Host    1
#define V7_Region  2
#define V7_Zone    3
#define V7_CM      4
#define V7_Res1    5
#define V7_Res2    6
#define V7_Res3    7

#define V7_Res4    8
#define V7_Res5    9
#define V7_Res6   10
#define V7_Res7   11
#define V7_Point  12
#define V7_Res9   13
#define V7_Res10  14
#define V7_Plus   15


//  ------------------------------------------------------------------

struct _V7Stk {
  long  blockno;
  int   node;
};


//  ------------------------------------------------------------------

struct _V7DTPCtl {
  word size;           // Size of this control structure
  byte Version;        // Version of DTP file (current = 0)
  byte AllFixSize;     // sizeof (_V7DTPAllLnk)
  byte AddFixSize;     // sizeof (_V7DTPNodeLnk)
};

#if defined(GOLD_CANPACK)
#pragma pack()
#endif


//  ------------------------------------------------------------------

class ftn_version7_nodelist_index : public ftn_nodelist_index_base {

  int       nfh;
  int       sfh;
  int       dfh;
  int       xfh;
  int       tfh;

  _V7Ndx    ctl;
  _V7Ndx    block;
  _V7DTPCtl dtpctl;

  int       node;
  int       inode;
  long      blockno;
  bool      use_v7plus;

  char      key[160];
  uint      keylength;

  _V7Stk    state;

  char      searchname[80];
  ftn_addr  searchaddr;

  void      getindexkey();
  void      getleafkey();
  void      getblock();
  void      getleaf();

  const char*     namekey() const;
  const ftn_addr& addrkey() const;

  int       namecmp() const;
  int       addrcmp() const;

  void      fetchdata();

  void      push();
  void      pop();
  bool      prevnode();
  bool      nextnode();
  void      compare()           { exactmatch = not (namebrowse ? namecmp() : addrcmp()); }
  bool      search();

public:

  ftn_version7_nodelist_index();
  virtual ~ftn_version7_nodelist_index();

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
