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
//  QFront 1.13.
//  ------------------------------------------------------------------

#ifndef __GS_QFRNT_H
#define __GS_QFRNT_H


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif

//  ------------------------------------------------------------------
//  Typedefs

typedef unsigned char   _boolean;
typedef unsigned char   byte;
typedef unsigned short  word;


//  ------------------------------------------------------------------
//  Constants

const int MaxOrigins = 35;


//  ------------------------------------------------------------------
//  File   : QORIGIN.DAT
//  Stores : Information about each of your origin lines.
//

struct OriginLineRecord {

  char OriginLine[MaxOrigins][61];
  byte Extra[50];
};


//  ------------------------------------------------------------------
//  File   : QECHOS.DAT
//  Stores : Information about each EchoMail area you have configured in your
//           Area Manager.
//  Notes  : This is VERY IMPORTANT.  Each EchoMail area has an associated
//           "tag".  The tag MUST be unique among ALL EchoMain areas, and the
//           tag must be between 1 and MaxConferences.  QFront uses the tag
//           as a bit number in the SelectedConfs field in the
//           EchoMailAssignmentRecord record to know which areas each node has
//           selected.  Failure to set the tag properly will result in
//           duplicate selected conferences for all of your nodes.  You must
//           not add any more than MaxConferences records to this file.
//

struct EchoMailConferenceRecord {

  _boolean PassThru;

  word    ConfNum;
  word    ConfTag;
  char    AreaName[81];

  long    HighMsgPointer;

  word    AreaLevel;

  _boolean CheckForDupes;
  _boolean ImportSeenByLines;
  _boolean ImportPathLines;
  _boolean KeepPrivate;

  _boolean Deleted;

  byte    OriginLine;
  byte    Group;

  _boolean ScanPrivate;
  _boolean Mandatory;
  _boolean ForcePrivate;

  byte    Extra[45];
};


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack()
#endif

//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------

