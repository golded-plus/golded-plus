//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (‘) 1993 Roger Kirchhoff & Stefan Graf
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
//  File structures for xMail v.1.00
//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif

//  ------------------------------------------------------------------

enum MsgBaseType {
  FmtHudson,
  FmtMsg,
  FmtPassThrough,
  FmtSquish,
  FmtJam
};


//  ------------------------------------------------------------------

enum MailTypeDef {
  Echomail,
  Netmail,
  Localmail,
  Newmail
};


//  ------------------------------------------------------------------

struct AddressRec {

  unsigned short  Zone;
  unsigned short  Net;
  unsigned short  Node;
  unsigned short  Point;
  char            Domain[21];
};


//  ------------------------------------------------------------------
//  Structure of AREAS.XM

struct EchoAreaRec {

  char            Name[41];           // Area Name
  char            Descr[61];          // Description
  unsigned char   MailType;           // (MailTypeDef) local, netmail or what
  unsigned char   StoreType;          // (MsgBaseType)
  char            MsgDirectory[61];   // ... and the corresponding directory
  unsigned short  ReadSecurity;       // Read access level
  unsigned short  WriteSecurity;      // Write access level
  char            OriginLine[61];     // Origin line for this echo
  AddressRec      AreaAddress;        // Adress of sender for this echo
  unsigned char   ConferenceNr;       // The conference this echo belongs to. If 0 the echo has NO conference
  unsigned short  maxdays;
  unsigned short  maxrecv;
  unsigned short  maxcount;
  unsigned short  totalimp;
  unsigned short  totalexp;           // Purge info
  long            totalimpL;
  long            totalexpL;          // total # of msgs imported/exported in this echo
  unsigned char   BasePathStrip;      // Don't import PATH line to base
  unsigned char   ExportPathStrip;    // Strip PATH line for downlinks
  unsigned char   BaseSeenStrip;      // Don't import SEEN-BY to base
  unsigned char   ExportSeenStrip;    // Strip SEEN-BY for downlinks
  unsigned short  AbsCosts;
  unsigned short  RelCosts;           // The cost for each mail in this echo
  long            LastActionDate;     // Last date something happened in this echo
  unsigned char   NeverRemove;        // Never remove this echo automatically
  unsigned short  MaxDupes;           // Maximum nr. of Dupes
  unsigned char   SkipPathCheck;
  unsigned char   Dummy[106];         // unused
};


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack()
#endif

//  ------------------------------------------------------------------
