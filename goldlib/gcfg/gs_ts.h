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
//  TosScan Areafile struct. Reverse engineered spec!
//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif

//  ------------------------------------------------------------------
//
//  The TosScan AREAFILE.FD
//
//  Original hack by Bjarne Maschoreck 2:231/50.
//  Later modified by Odinn Sorensen 2:231/77
//
//  ------------------------------------------------------------------
//
//  The first 4 bytes are ignored (probably CRC32 of total file).
//
//  The rest are records of fixed-length 650 bytes.
//  Strings are NUL-terminated.
//  Some fields are indexes into tables in FD.SYS.
//  Conferences are not removed when deleted, a flag is set instead.
//  The same record format is used for TosScan/QBBS and TosScan/MSG.
//
//  ------------------------------------------------------------------


//  ------------------------------------------------------------------

typedef byte bit;


//  ------------------------------------------------------------------
//  Structure of the records

typedef struct {
  char  echoname[51];   /* Echomail conference name */
  char  path[51];       /* Path, if .MSG format otherwise NUL'ed */
  bit   seenbys  : 1;   /* 1 = Import Seen-By 0 = No Seen-By   */
  bit   security : 1;   /* 1 = Security On    0 = Security Off */
  bit   passthru : 1;   /* 1 = Passthru       0 = Not Passthru */
  bit   visible  : 1;   /* 1 = Visible        0 = Not visible  */
  bit   deleted  : 1;   /* 1 = Deleted        0 = In Use       */
  bit   cpp      : 1;   /* 1 = CPP On         0 = CPP Off      */
  bit   tiny     : 1;   /* 1 = Tiny           0 = Not Tiny     */
  bit   Private  : 1;   /* 1 = Private        0 = Not Private  */
  byte  originno;       /* FrontDoor origin number? Seems to follow origno */
  ftn_addr feeders[60];    /* List of feeds. Sorted descending, NUL-padded */
  char  fill2[4];       /* Unknown */
  byte  groupid;        /* Group-relation. (A-Z, uppercase) */
  byte  board;          /* Board number, if QBBS format - otherwise zero */
  bit   akano  : 4;     /* Index in FD.SYS - Aka number */
  bit   origno : 4;     /* Index in FD.SYS - Origin number */
  char  fill3;          /* Unknown */
  short  maxmsgs;        /* Max msgs in area (max 20000) */
  short  dayskeep;       /* Day to keep before deleting (max 365) */
  char  fill4[4];       /* Unknown */
  char  desc[40];       /* Area description for AreaMgr */
  char  fill5[10];      /* Unknown */
} TS_Areafile;


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack()
#endif

//  ------------------------------------------------------------------
