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
