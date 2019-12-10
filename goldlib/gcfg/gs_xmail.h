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

#if defined (GOLD_CANPACK)
#pragma pack(1)
#endif
//  ------------------------------------------------------------------
enum MsgBaseType
{
    FmtHudson, FmtMsg, FmtPassThrough, FmtSquish, FmtJam
};

//  ------------------------------------------------------------------
enum MailTypeDef
{
    Echomail, Netmail, Localmail, Newmail
};

//  ------------------------------------------------------------------
struct AddressRec
{
    uint16_t Zone;
    uint16_t Net;
    uint16_t Node;
    uint16_t Point;
    char     Domain[21];
};

//  ------------------------------------------------------------------
//  Structure of AREAS.XM
struct EchoAreaRec
{
    char       Name[41];          // Area Name
    char       Descr[61];         // Description
    uint8_t    MailType;          // (MailTypeDef) local, netmail or what
    uint8_t    StoreType;         // (MsgBaseType)
    char       MsgDirectory[61];  // ... and the corresponding directory
    uint16_t   ReadSecurity;      // Read access level
    uint16_t   WriteSecurity;     // Write access level
    char       OriginLine[61];    // Origin line for this echo
    AddressRec AreaAddress;       // Adress of sender for this echo
    uint8_t    ConferenceNr;      // The conference this echo belongs to. If 0 the echo has NO
                                  // conference
    uint16_t   maxdays;
    uint16_t   maxrecv;
    uint16_t   maxcount;
    uint16_t   totalimp;
    uint16_t   totalexp;          // Purge info
    int32_t    totalimpL;
    int32_t    totalexpL;         // total # of msgs imported/exported in this echo
    uint8_t    BasePathStrip;     // Don't import PATH line to base
    uint8_t    ExportPathStrip;   // Strip PATH line for downlinks
    uint8_t    BaseSeenStrip;     // Don't import SEEN-BY to base
    uint8_t    ExportSeenStrip;   // Strip SEEN-BY for downlinks
    uint16_t   AbsCosts;
    uint16_t   RelCosts;          // The cost for each mail in this echo
    int32_t    LastActionDate;    // Last date something happened in this echo
    uint8_t    NeverRemove;       // Never remove this echo automatically
    uint16_t   MaxDupes;          // Maximum nr. of Dupes
    uint8_t    SkipPathCheck;
    uint8_t    Dummy[106];        // unused
};

//  ------------------------------------------------------------------

#if defined (GOLD_CANPACK)
#pragma pack()
#endif
//  ------------------------------------------------------------------
