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
//  Maximus derived userfile class.
//  ------------------------------------------------------------------

#ifndef __gusrmax_h
#define __gusrmax_h


//  ------------------------------------------------------------------

#include <gtimall.h>
#include <gusrbase.h>


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif


//  ------------------------------------------------------------------
//  Maximus structure for USER.BBS and LASTUSER.BBS

struct MaxUsers {

  char    name[36];       // Caller's name
  char    city[36];       // Caller's location

  char    alias[21];      // MAX: user's alias (handle)
  char    phone[15];      // MAX: user's phone number

  word    lastread_ptr;   // MAX: a num which points to offset in LASTREAD
                          // file -- Offset of lastread pointer will be
                          // lastread_ptr*sizeof(int).

  word    timeremaining;  // MAX: time left for current call (xtern prog)

  char    pwd[16];        // Password
  word    times;          // Number of previous calls to this system
  byte    help;           // Help level
  byte    rsvd1[2];       // Reserved by Maximus for future use
  byte    video;          // user's video mode (see GRAPH_XXXX)
  byte    nulls;          // Number of Nulls (delays) after <cr>

  byte    bits;           // Bit flags for user (number 1)

  word    rsvd2;          // Reserved by Maximus for future use

  word    bits2;          // Bit flags for user (number 2)

  word    priv;           // Access level
  byte    rsvd3[19];      // Reserved by Maximus for future use
  byte    struct_len;     // len of struct, divided by 20. SEE ABOVE!
  word    time;           // Time on-line so far today

  word    delflag;        // Used to hold baud rate for O)utside command
                          // In USER.BBS, usr.flag uses the constants
                          // UFLAG_xxx, defined earlier in this file.

  byte    rsvd4[8];       // Reserved by Maximus for future use

  byte    width;          // Width of the caller's screen
  byte    len;            // Height of the caller's screen
  word    credit;         // Matrix credit, in cents
  word    debit;          // Current matrix debit, in cents

  word    xp_priv;        // Priv to demote to, when time or minutes run
                          // out.

  FTime   xp_date;        // Bit-mapped date of when user expires.
                          // If zero, then no expiry date.

  dword   xp_mins;        // How many minutes the user has left before
                          // expiring.

  byte    xp_flag;        // Flags for expiry.  See above XFLAG_XXX defs.
  byte    xp_rsvd;

  FTime   ludate;         // Bit-mapped date of user's last call

  dword   xkeys;          // User's keys (all 32 of 'em)
  byte    lang;           // The user's current language #
  byte    def_proto;      // Default file-transfer protocol

  dword   up;             // K-bytes uploaded, all calls
  dword   down;           // K-bytes downloaded, all calls
  dword   downtoday;      // K-bytes downloaded, today

  char    msg[10];        // User's last msg area (string)
  char    files[10];      // User's last file area (string)

  byte    compress;       // Default compression program to use

  byte    rsvd5;
  dword   extra;
};


//  ------------------------------------------------------------------
//  Maximus userfile constants

#define MAXIMUS_USERDELETED     0x0001
#define MAXIMUS_USERPERMANENT   0x0002

#define MAXIMUS_PRIV_TWIT       0xFFFE


//  ------------------------------------------------------------------
//  Maximus userfile class

class MaximusUser : public GUser {

public:

  uint maxindex;
  int firstread;
  MaxUsers* record;
  
  MaximusUser();
  ~MaximusUser();

  int  isvalid();
  int  read();

  void  founduser();
  void  recinit(const char* __name);
};


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack()
#endif


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
