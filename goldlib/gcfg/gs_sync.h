//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1994 Digital Dynamics
//  Copyright (C) 2002 Alexander S. Aganichev
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
//  Synchronet BBS Definitions and Structures
//  ------------------------------------------------------------------

#ifndef __gs_sync_h
#define __gs_sync_h

//  ------------------------------------------------------------------

#include <gdefs.h>
#include <gftnall.h>


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif


//  ------------------------------------------------------------------

const int LEN_DIR = 63;         // Maximum length of directory paths
const int LEN_ARSTR = 40;       // Max length of Access Requirement string
const int LEN_GSNAME = 15;      // Group/Lib short name
const int LEN_GLNAME = 40;      // Group/Lib long name
const int LEN_SSNAME = 25;      // Sub/Dir short name
const int LEN_SLNAME = 40;      // Sub/Dir long name
const int LEN_SQWKNAME = 10;


//  ------------------------------------------------------------------

// Bit values for sub[x].misc
const int SUB_NSCAN = (1L<<0);    // Scan this sub-board for new msgs
const int SUB_YSCAN = (1L<<1);    // Scan for new messages to you only
const int SUB_SSCAN = (1L<<2);    // Scan this sub-board for msgs to you
const int SUB_QNET = (1L<<3);     // Sub-board is netted via QWK network
const int SUB_PNET = (1L<<4);     // Sub-board is netted via PostLink
const int SUB_FIDO = (1L<<5);     // Sub-board is netted via FidoNet
const int SUB_PRIV = (1L<<6);     // Allow private posts on sub
const int SUB_PONLY = (1L<<7);    // Private posts only
const int SUB_ANON = (1L<<8);     // Allow anonymous posts on sub
const int SUB_AONLY = (1L<<9);    // Anonymous only
const int SUB_NAME = (1L<<10);    // Must use real names
const int SUB_DEL = (1L<<11);     // Allow users to delete messages
const int SUB_DELLAST = (1L<<12); // Allow users to delete last msg only
const int SUB_FORCED = (1L<<13);  // Sub-board is forced scanning
const int SUB_NOTAG = (1L<<14);   // Don't add tag or origin lines
const int SUB_TOUSER = (1L<<15);  // Prompt for to user on posts
const int SUB_ASCII = (1L<<16);   // ASCII characters only
const int SUB_QUOTE = (1L<<17);   // Allow online quoting
const int SUB_NSDEF = (1L<<18);   // New-Scan on by default
const int SUB_UNUSED = (1L<<19);  // Currently unused
const int SUB_FAST = (1L<<20);    // Fast storage mode
const int SUB_KILL = (1L<<21);    // Kill read messages automatically
const int SUB_KILLP = (1L<<22);   // Kill read pvt messages automatically
const int SUB_SYSPERM =(1L<<23);  // Sysop messages are permament
const int SUB_GATE = (1L<<24);    // Gateway between Network types
const int SUB_LZH = (1L<<25);     // Use LZH compression for msgs
const int SUB_SSDEF = (1L<<26);   // Default ON for Scan for Your msgs
const int SUB_HYPER = (1L<<27);   // Hyper allocation


//  ------------------------------------------------------------------

typedef struct {                // Message group info
  char   lname[LEN_GLNAME+1],   // Short name
         sname[LEN_GSNAME+1];   // Long name
  uchar  ar[LEN_ARSTR+1],       // Access requirements
         unused[96];            // unused
} grp_t;

typedef struct {                // Message sub board info
  ushort grp;                   // Which group this sub belongs to
  char   lname[LEN_SLNAME+1],   // Long name - used for listing
         sname[LEN_SSNAME+1],   // Short name - used for prompts
         qwkname[LEN_SQWKNAME+1], // QWK name
         code[9],               // Eight character code
         data_dir[LEN_DIR+1];   // Data file directory
  uchar  ar[LEN_ARSTR+1],       // Access requirements
         read_ar[LEN_ARSTR+1],  // Read requirements
         post_ar[LEN_ARSTR+1],  // Post requirements
         op_ar[LEN_ARSTR+1];    // Operator requirements
  ulong  misc;                  // Miscellaneous flags
  char   tagline[81],           // Optional QWK net tag line
         origline[51],          // Optional EchoMail origin line
         echomail_sem[LEN_DIR+1], // EchoMail semaphore for this sub
         echopath[LEN_DIR+1];   // EchoMail path
  ftn_addr faddr;               // FidoNet address
  ulong  maxmsgs,               // Max number of messages allowed
         maxcrcs;               // Max number of CRCs to keep
  ushort maxage,                // Max age of messages (in days)
         ptridx;                // Index into pointer file
  uchar  mod_ar[LEN_ARSTR+1];   // Moderated user requirements
  ushort qwkconf;               // QWK configuration
  uchar  unused[53];            // unused
} sub_t;

//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack()
#endif

//  ------------------------------------------------------------------

#endif // __gs_sync_h

//  ------------------------------------------------------------------
