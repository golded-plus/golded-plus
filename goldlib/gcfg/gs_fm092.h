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
//  Structures for FMail 0.92
//  ------------------------------------------------------------------

#ifndef __gs_fm092_h
#define __gs_fm092_h
//  ------------------------------------------------------------------

#include <gftnall.h>
//  ------------------------------------------------------------------

#if defined (GOLD_CANPACK)
    #pragma pack(1)
#endif
//  ------------------------------------------------------------------

#define MAX_AKAS 11

typedef struct
{
    ftn_addr nodeNum;
    word     fakeNet;
} nodeFakeType;
typedef char         pathType[48];
typedef char         tempStrType[128];
typedef nodeFakeType akaListType[MAX_AKAS];
typedef char         archiverInfo[48];
typedef struct
{
    word useEMS       :  1; /* BIT 0 */
    word :  1;
    word swap         :  1; /* BIT 2 */
    word swapEMS      :  1; /* BIT 3 */
    word swapXMS      :  1; /* BIT 4 */
    word :  1;
    word monochrome   :  1; /* BIT 6 */
    word commentFFD   :  1; /* BIT 7 */
    word PTAreasBBS   :  1; /* BIT 8 */
    word commentFRA   :  1; /* BIT 9 */
    word keepReceipt  :  1; /* BIT 10 */
    word :  1;
    word :  1;
    word :  1;
    word :  1;
    word extraHandles :  1; /* BIT 15 */
} genOptionsType;
typedef struct
{
    word removeNetKludges : 1; /* Bit 0 */
    word : 1;
    word checkPktDest     : 1; /* Bit 2 */
    word areaMgrPwdChange : 1; /* Bit 3 */
    word packetPwdChange  : 1; /* Bit 4 */
    word : 1;
    word : 1;
    word : 1;
    word dupDetection     : 1; /* Bit 8 */
    word /*strictGroups*/ : 1; /* Bit 9 */
    word                  : 1; /* Bit 10 */
    word                  : 1;
    word persNetmail      : 1; /* Bit 12 */
    word : 1;
    word : 1;
    word : 1;                  /* Bit 15 */
} mailOptionsType;
typedef struct
{
    word sortNew      : 1; /* bit  0   */
    word sortSubject  : 1; /* bit  1   */
    word updateChains : 1; /* bit  2   */
    word : 1;              /* bit  3   */
    word _importSeenBy: 1; /* bit  4   */
    word _removePID   : 1; /* bit  5 not USED */
    word removeRe     : 1; /* bit  6   */
    word removeLfSr   : 1; /* bit  7   */
    word scanAlways   : 1; /* bit  8   */
    word scanUpdate   : 1; /* bit  9   */
    word multiLine    : 1; /* bit 10   */
    word : 1;              /* bit 11   */
    word quickToss    : 1; /* bit 12   */
    word : 1;              /* bit 13-14 */
    word : 1;              /* bit 13-14 */
    word sysopImport  : 1; /* bit 15   */
} mbOptionsType;
typedef struct
{
    word active      : 1; /* Bit  0 */
    word tinySeenBy  : 1; /* Bit  1 */
    word security    : 1; /* Bit  2 */
    word : 1;             /* Bit  3 */
    word Private     : 1; /* Bit  4 */
    word impSeenBy   : 1; /* Bit  5 */
    word checkSeenBy : 1; /* Bit  6 */
    word /*compChar*/: 1; /* Bit  7 */
    word local       : 1; /* Bit  8 */
    word : 1;             /* Bit  9 */
    word passThrough : 1; /* Bit 10 */
    word : 1;             /* Bit 11-13 */
    word : 1;             /* Bit 11-13 */
    word : 1;             /* Bit 11-13 */
    word arrivalDate : 1; /* Bit 14 */
    word sysopRead   : 1; /* Bit 15 */
} defaultOptionsType;
typedef struct
{
    char               versionMajor;
    char               versionMinor;
    long               creationDate;
    uint32_t           key;
    uint32_t           reservedKey;
    char               sysopNameOld[32];
    akaListType        akaList;
    nodeFakeType       reservedAka[16 - MAX_AKAS];
    word               netmailBoard[MAX_AKAS];
    word               reservedNet[16 - MAX_AKAS];
    genOptionsType     genOptions;
    mbOptionsType      mbOptions;
    mailOptionsType    mailOptions;
    word               maxPktSize;
    word               logLevel;
    word               mailer;
    word               bbsProgram;
    char               reserved1[86];
    char               sysopName[36];
    word               defaultArc;
    char               reserved2[98];
    word               recBoard;
    word               badBoard;
    word               dupBoard;
    char               topic1[16];
    char               topic2[16];
    pathType           bbsPath;
    pathType           netPath;
    pathType           sentPath;
    pathType           rcvdPath;
    pathType           inPath;
    pathType           outPath;
    pathType           securePath;
    pathType           logName;
    pathType           swapPath;
    pathType           semaphorePath;
    pathType           pmailPath;
    pathType           areaMgrLogName;
    pathType           autoRAPath;
    pathType           autoFolderFdPath;
    pathType           autoAreasBBSPath;
    pathType           autoGoldEdAreasPath;
    archiverInfo       unArc;
    archiverInfo       unZip;
    archiverInfo       unLzh;
    archiverInfo       unPak;
    archiverInfo       unZoo;
    archiverInfo       unArj;
    archiverInfo       reservedArc1;
    archiverInfo       reservedArc2;
    archiverInfo       arc;
    archiverInfo       zip;
    archiverInfo       lzh;
    archiverInfo       pak;
    archiverInfo       zoo;
    archiverInfo       arj;
    archiverInfo       reservedArc3;
    archiverInfo       reservedArc4;
    char               defaultOriginLine[59];
    defaultOptionsType defaultOptions;
    word               defaultDays;
    word               defaultMsgs;
    word               defaultReadSecRA;
    char               defaultReadFlagsRA[4];
    word               defaultWriteSecRA;
    char               defaultWriteFlagsRA[4];
    word               defaultSysopSecRA;
    char               defaultSysopFlagsRA[4];
    char               defaultAttrRA;
    char               reserved3[428];
    char               groupDescr[26][27];
    char               reserved4[20];
    char               attrRA[11];
    word               readSecRA[11];
    char               readFlagsRA[11][4];
    word               writeSecRA[11];
    char               writeFlagsRA[11][4];
    word               sysopSecRA[11];
    char               sysopFlagsRA[11][4];
    word               daysAKA[11];
    word               msgsAKA[11];
    char               descrAKA[11][51];
} configType;

#define ECHONAME_LEN_OLD 25
#define ECHONAME_LEN 51
#define COMMENT_LEN 51
#define ORGLINE_LEN 59
#define MAX_FORWARD 64

typedef struct
{
    word active      : 1; /* Bit  0 */
    word tinySeenBy  : 1; /* Bit  1 */
    word security    : 1; /* Bit  2 */
    word : 1;             /* Bit  3 */
    word Private     : 1; /* Bit  4 */
    word impSeenBy   : 1; /* Bit  5 */
    word checkSeenBy : 1; /* Bit  6 */
    word /*compChar*/: 1; /* Bit  7 */
    word local       : 1; /* Bit  8 */
    word : 1;             /* Bit  9 */
    word reserved    : 1; /* Bit 10 */ /* default pass-thru bit */
    word : 1;             /* Bit 11-13 */
    word : 1;             /* Bit 11-13 */
    word : 1;             /* Bit 11-13 */
    word arrivalDate : 1; /* Bit 14 */
    word sysopRead   : 1; /* Bit 15 */
} areaOptionsType;
typedef struct
{
    char            areaNameOld[ECHONAME_LEN_OLD];
    char            comment[COMMENT_LEN];
    long            group;
    word            board;
    word            address;
    word            alsoSeenBy;
    char            reserved1[8];
    areaOptionsType options;
    word            outStatus;
    word            days;
    word            msgs;
    char            reserved2[4];
    word            readSecRA;
    char            flagsRdRA[4];
    word            writeSecRA;
    char            flagsWrRA[4];
    word            sysopSecRA;
    char            flagsSysRA[4];
    char            attrRA;
    char            msgKindsRA;
    word            attrSBBS;
    char            replyStatSBBS;
    char            areaName[ECHONAME_LEN];
    char            reserved3[17];
    char            originLine[ORGLINE_LEN];
    ftn_addr        exp[MAX_FORWARD];
} rawEchoType;
//  ------------------------------------------------------------------

#if defined (GOLD_CANPACK)
    #pragma pack()
#endif
//  ------------------------------------------------------------------

#endif // ifndef __gs_fm092_h
//  ------------------------------------------------------------------
