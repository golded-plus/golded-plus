//  This may look like C code, but it is really -*- C++ -*-
//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (c) 1994 Tobias Burchhardt
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
//  FastEcho 1.42 beta structures
//  ------------------------------------------------------------------

#ifndef __gs_fech6_h
#define __gs_fech6_h
//  ------------------------------------------------------------------

#include <gdefs.h>
//  ------------------------------------------------------------------

#if defined (GOLD_CANPACK)
    #pragma pack(1)
#endif
//  ------------------------------------------------------------------
//  FASTECHO.CFG = <CONFIG>
//               + <optional extensions>
//               + <CONFIG.NodeCnt * Node>
//               + <CONFIG.AreaCnt * Area>

#define REVISION 6              // current revision
#define MAX_ROUTE 15            // max # of 'vias'
//  ------------------------------------------------------------------
//  Area.board (1-200 = Hudson)

#define NO_BOARD 0x4000u        // JAM/Sq/Passthru etc.
#define AREA_DELETED 0x8000u    // usually never written
//  ------------------------------------------------------------------
//  Area.flags.storage

#define QBBS 0
#define FIDO 1
#define SQUISH 2
#define JAM 3
#define PASSTHRU 7
//  ------------------------------------------------------------------
//  Area.flags.atype

#define AREA_ECHOMAIL 0
#define AREA_NETMAIL 1
#define AREA_LOCAL 2
#define AREA_BADMAILBOARD 3
#define AREA_DUPEBOARD 4
//  ------------------------------------------------------------------
//  Types and other definitions

#define _MAXPATH 56

typedef struct CONFIGURATION6
{
    word  revision;
    dword flags;
    word  NodeCnt, AreaCnt, unused1;
    char  NetMPath[_MAXPATH], MsgBase[_MAXPATH], InBound[_MAXPATH], OutBound[_MAXPATH],
          Unpacker[_MAXPATH], LogFile[_MAXPATH], unused2[448], StatFile[_MAXPATH],
          SwapPath[_MAXPATH], SemaphorePath[_MAXPATH], BBSConfigPath[_MAXPATH],
          DBQueuePath[_MAXPATH], unused3[32], RetearTo[40], SecurePath[_MAXPATH],
          ExtAfter[_MAXPATH - 4], ExtBefore[_MAXPATH - 4];
    byte unused4[480];
    struct
    {
        byte what;
        char object[31];
        word conference;
    } CC[10];
    byte security, loglevel;
    word def_days, def_messages;
    byte unused5[462];
    word autorenum;
    word def_recvdays;
    word openQQQs;
    word compressafter;
    word afixmaxmsglen;
    word compressfree;
    char TempPath[_MAXPATH];
    byte graphics, BBSSoftware;
    char AreaFixHelp[_MAXPATH];
    byte unused6[504];
    word AreaFixFlags;
    byte QuietLevel, Buffers;
    byte FWACnt,                          // # of ForwardAreaFix records,
         GDCnt;                           // # of Group Default records
    struct
    {
        word flags;
        word days[2];
        word msgs[2];
    } rescan_def;
    dword duperecords;
    struct
    {
        byte inb;
        byte outb;
    } arcext;
    word AFixRcptLen;
    word AkaCnt;                          // # of Aka records stored */
    word maxPKT;
    byte sharing, sorting;
    struct
    {
        char  name[36];
        dword resv;
    } sysops[11];
    char AreaFixLog[_MAXPATH];
    char TempInBound[_MAXPATH];
    word maxPKTmsgs;
    word RouteCnt;                        // # of PackRoute records
    word maxPACKratio;
    byte PackerCnt, UnpackerCnt;          // # of Packers and Unpackers records
    byte GroupCnt, OriginCnt;             // # of GroupNames and Origin records
    word mailer;
    char resv[810];
    word AreaRecSize, GrpDefRecSize;      // Size  of  Area  and  GroupDefaults
    // records stored in this file
    word  MaxAreas, MaxNodes;             // Current max values for this config
    word  NodeRecSize;                    // Size of each stored Node record
    dword offset;                         // This is the offset from the current
    // file-pointer to the 1st Node
} CONFIG6;
//  ------------------------------------------------------------------
typedef struct
{
    char name[52];
    word board;                           // 1-200 Hudson, others reserved/special
    word conference;                      // 0 ... CONFIG.MaxAreas-1
    word read_sec, write_sec;
    struct
    {
        word aka    : 8;                  // 0 ... CONFIG.AkaCnt
        word group  : 8;                  // 0 ... CONFIG.GroupCnt
    } info;
    struct
    {
        word storage: 4;
        word atype  : 4;
        word origin : 5;                    // # of origin line
        word resv   : 3;
    } flags;
    struct
    {
        word autoadded  : 1;
        word tinyseen   : 1;
        word cpd        : 1;
        word passive    : 1;
        word keepseen   : 1;
        word mandatory  : 1;
        word keepsysop  : 1;
        word killread   : 1;
        word disablepsv : 1;
        word keepmails  : 1;
        word hide       : 1;
        word nomanual   : 1;
        word umlaut     : 1;
        word resv       : 3;
    } advflags;
    word  resv1;
    dword seenbys;                        // LSB = Aka0, MSB = Aka31
    dword resv2;
    short days;
    short messages;
    short recvdays;
    char  path[_MAXPATH];
    char  desc[52];
} FeArea6;
//  ------------------------------------------------------------------
//  Optional Extensions
typedef struct
{
    word  type;                         // EH_...
    dword offset;                       // length of field excluding header
} ExtensionHeader;

#define EH_AKAS 0x0007                  // CONFIG.AkaCnt * <SysAddress>
typedef struct
{
    ftn_addr main;
    char     domain[28];
    word     pointnet;
    dword    flags;                     // unused
} SysAddress;

#define EH_ORIGINS 0x0008               // CONFIG.OriginCnt * <OriginLines>
typedef struct
{
    char line[62];
} OriginLines;
//  ------------------------------------------------------------------

#if defined (GOLD_CANPACK)
    #pragma pack()
#endif
//  ------------------------------------------------------------------

#endif // ifndef __gs_fech6_h
//  ------------------------------------------------------------------
