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
//  PCBoard 15.xx derived userfile class.
//  ------------------------------------------------------------------

#ifndef __gusrpcb_h
#define __gusrpcb_h


//  ------------------------------------------------------------------

#include <gusrbase.h>


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif


//  ------------------------------------------------------------------
//  PCBoard Users Record (USERS. file)

struct PcbUsers {

  char  name[25];                 // Full Name
  char  city[24];                 // City
  char  password[12];             // Password
  char  dataphone[13];            // Business / Data Phone Number
  char  voicephone[13];           // Home / Voice Phone Number
  char  lastdateon[6];            // Last Date On (format:  YYMMDD)
  char  lasttimeon[5];            // Last Time On (format HH:MM)
  char  expertmode;               // Expert Mode (Y or N)
  char  protocol;                 // Default Transfer Protocol (A-Z, 0-9)
  byte  bitflags;                 // Bit Flags (see below)
  char  dateoflastdirscan[6];     // Date of Last DIR Scan (most recent file found)
  byte  securitylevel;            // Security Level (0-255)
  short  timesonline;              // Number of Times On
  byte  pagelength;               // Page Length (# lines displayed before prompt)
  short  filesuploaded;            // Number of Files Uploaded
  short  filesdownloaded;          // Number of Files Downloaded
  dword  bytesdownloadedtoday[2];  // Total Bytes Downloaded Today
  char  userscomment[30];         // Comment Field #1 (user comment)
  char  sysopcomment[30];         // Comment Field #2 (sysop comment - user can't see)
  short  elapsedtimeon;            // Elapsed Time On (in minutes)
  char  regexpirationdate[6];     // Registration Expiration Date (YYMMDD)
  byte  expiredregseclevel;       // Expired Registration - Security Level
  byte  lastconferenceinold;      // Last Conference In (used for v14.x compatibility)
  byte  confregflags[5];          // Conference Registration Flags (conf 0-39)
  byte  expiredconfregflags[5];   // Expired Registration Conference Flags (conf 0-39)
  byte  usersconfflags[5];        // User Selected Conference Flags (conf 0-39)
  dword  totalbytesdownloaded[2];  // Total Bytes Downloaded
  dword  totalbytesuploaded[2];    // Total Bytes Uploaded
  char  deleteflag;               // Delete Flag (Y or N)
  dword  lastmsgread[40];          // Last Message Read pointer (conference 0-39)
  long  usersinfrec;              // Record Number of USERS.INF Record
  byte  bitflags2;                // Bit Flags 2 (see below)
  char  reserved[8];              // Reserved (do not use)
  short  lastconferencein;         // Last Conference In (used instead of offset 192)
};


//  ------------------------------------------------------------------
//  PCBoard Users Inf Header Record (in USERS.INF file)

struct PcbUsersInfHdr {

  ushort version;                  // PCBoard Version Number
  ushort numofconf;                // Number of EXTENDED Conferences Allocated in File
  ushort sizeofrec;                // Size of the 'static' PCBoard User Record
  long  sizeofconf;               // Total Size of PCBoard Conference Information
  ushort numofapps;                // Number of Third Party Apps adding onto the record
  long  totalrecsize;             // Total Record Size (PCB and all TPA components)
};


//  ------------------------------------------------------------------
//  PCBoard Users Inf App Record (in USERS.INF file)

struct PcbUsersInfApp {

  char  name[15];                 // Name of Application (NULL terminated)
  ushort version;                  // Version Number
  ushort sizeofrec;                // Size of Application Record information (0-65535)
  ushort sizeofconfrec;            // Size of Conference Record information (0-65535)
  char  keyword[9];               // Keyword to execute Application (NULL terminated)
  long  offset;                   // Offset in User Record where TPA record begins
};


//  ------------------------------------------------------------------
//  PCBoard 15.xx userfile class

class PcbUser : public GUser {

public:

  int fhinf;

  PcbUsers* record;
  PcbUsersInfHdr* usershdr;
  long usershdrsize;

  PcbUser();
  ~PcbUser();

  int  isvalid();
  int  read();

  void  recinit(const char* __name);

  void  add(const char* __name);

  void  update_mail_waiting(const char* __name, int __confno, int __status);
};


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack()
#endif


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
