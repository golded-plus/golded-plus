//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1994 Folkert J. Wijnstra
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
//  File structures for FMail 1.0g
//  ------------------------------------------------------------------

#ifndef __gs_fm10g_h
#define __gs_fm10g_h


//  ------------------------------------------------------------------

#include <gftnall.h>


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif


//  ------------------------------------------------------------------

typedef struct {
  char  programName[46];
  word  memRequired;
} archiverInfo;

typedef char pathType[48];

typedef struct {
  ftn_addr    nodeNum;
  word        fakeNet;
} nodeFakeType;


//  ------------------------------------------------------------------

#define DATATYPE_CF    0x0102
#define DATATYPE_NO    0x0202
#define DATATYPE_AD    0x0401
#define DATATYPE_AE    0x0402

typedef struct {
  char   versionString[32]; // Always starts with 'FMail'
  word   revNumber;         // Is now 0x0100
  word   dataType;          // See #defines above
  word   headerSize;
  sdword creationDate;
  sdword lastModified;
  word   totalRecords;
  word   recordSize;
} headerType;


//  ------------------------------------------------------------------
//  The structure below is used by the Areas File and (only partly)
//  by the Config File

typedef struct {
  word active      : 1; /* Bit  0 */
  word tinySeenBy  : 1; /* Bit  1 */
  word security    : 1; /* Bit  2 */
  word             : 1; /* Bit  3 */
  word allowPrivate: 1; /* Bit  4 */
  word impSeenBy   : 1; /* Bit  5 */
  word checkSeenBy : 1; /* Bit  6 */
  word             : 1; /* Bit  7 */
  word local       : 1; /* Bit  8 */
  word disconnected: 1; /* Bit  9 */
  word reserved    : 1; /* Bit 10 */
  word allowAreafix: 1; /* Bit 11 */
  word             : 1; /* Bit 12-13 */
  word             : 1; /* Bit 12-13 */
  word arrivalDate : 1; /* Bit 14 */
  word sysopRead   : 1; /* Bit 15 */
} areaOptionsType;

#define MAX_AKAS      16
#define MAX_NETAKAS   11
#define MAX_USERS     16
#define MAX_UPLREQ    16
#define MAX_MATCH     16

typedef nodeFakeType akaListType[MAX_AKAS];

typedef struct {
  word useEMS       : 1; /* BIT 0 */
  word checkBreak   : 1; /* BIT 1 */
  word swap         : 1; /* BIT 2 */
  word swapEMS      : 1; /* BIT 3 */
  word swapXMS      : 1; /* BIT 4 */
  word              : 1;
  word monochrome   : 1; /* BIT 6 */
  word commentFFD   : 1; /* BIT 7 */
  word PTAreasBBS   : 1; /* BIT 8 */
  word commentFRA   : 1; /* BIT 9 */
  word              : 1; /* BIT 10 */
  word incBDRRA     : 1; /* BIT 11 */
  word              : 1; /* BIT 12 */
  word              : 1;
  word              : 1;
  word _RA2         : 1; /* BIT 15 */
} genOptionsType;

typedef struct {
  word removeNetKludges : 1; /* Bit 0 */
  word                  : 1;
  word checkPktDest     : 1; /* Bit 2 */
  word                  : 1;
  word createSema       : 1; /* Bit 4 */
  word                  : 1;
  word warnNewMail      : 1; /* bit 6 */
  word killBadFAtt      : 1; /* Bit 7 */
  word dupDetection     : 1; /* Bit 8 */
  word ignoreMSGID      : 1; /* Bit 9 */
  word ARCmail060       : 1; /* Bit 10 */
  word extNames         : 1; /* Bit 11 */
  word persNetmail      : 1; /* Bit 12 */
  word privateImport    : 1; /* Bit 13 */
  word keepExpNetmail   : 1; /* Bit 14 */
  word killEmptyNetmail : 1; /* Bit 15 */
} mailOptionsType;

typedef struct {
  word sortNew      : 1; /* bit  0   */
  word sortSubject  : 1; /* bit  1   */
  word updateChains : 1; /* bit  2   */
  word reTear       : 1; /* bit  3   */
  word              : 1; /* bit  4   */
  word              : 1; /* bit  5   */
  word removeRe     : 1; /* bit  6   */
  word removeLfSr   : 1; /* bit  7   */
  word scanAlways   : 1; /* bit  8   */
  word scanUpdate   : 1; /* bit  9   */
  word multiLine    : 1; /* bit 10   */
  word              : 1; /* bit 11   */
  word quickToss    : 1; /* bit 12   */
  word              : 1; /* bit 13   */
  word              : 1; /* bit 14   */
  word sysopImport  : 1; /* bit 15   */
} mbOptionsType;

typedef struct {
  word keepRequest  : 1; /* Bit  0 */
  word keepReceipt  : 1; /* Bit  1 */
  word              : 1; /* Bit  2 */
  word              : 1; /* Bit  3 */
  word autoDiscArea : 1; /* Bit  4 */
  word autoDiscDel  : 1; /* Bit  5 has temp. no effect, rec is always deleted */
  word              : 1; /* Bit  6 */
  word              : 1; /* Bit  7 */
  word              : 1; /* Bit  8 */
  word allowAddAll  : 1; /* Bit  9 */
  word allowActive  : 1; /* Bit 10 */
  word              : 1; /* Bit 11 */
  word allowPassword: 1; /* Bit 12 */
  word allowPktPwd  : 1; /* Bit 13 */
  word allowNotify  : 1; /* Bit 14 */
  word allowCompr   : 1; /* Bit 15 */
} mgrOptionsType;

typedef struct {
  word addPlusPrefix :  1; /* BIT 0 */
  word               :  1;
  word               :  1;
  word               :  1;
  word unconditional :  1; /* BIT 4 */
  word                   : 11;
} uplOptType;

typedef struct {
  char  userName[36];
  char  reserved[28];
} userType;

typedef struct {
  ftn_addr    node;
  char        program[9];
  char        password[17];
  char        fileName[13];
  char        fileType;
  dword       groups;
  char        originAka;
  uplOptType  options;
  char        reserved[9];
} uplinkReqType;

typedef struct {
  word  valid;
  word  zone;
  word  net;
  word  node;
} akaMatchNodeType;

typedef struct {
  akaMatchNodeType amNode;
  word             aka;
} akaMatchType;

typedef struct {
  char            versionMajor;
  char            versionMinor;
  sdword          creationDate;
  dword           key;
  dword           reservedKey;
  dword           relKey1;
  dword           relKey2;
  char            reserved1[22];
  mgrOptionsType  mgrOptions;
  akaListType     akaList;
  word            netmailBoard[MAX_NETAKAS];
  word            reservedNet[16-MAX_NETAKAS];
  genOptionsType  genOptions;
  mbOptionsType   mbOptions;
  mailOptionsType mailOptions;
  word            maxPktSize;
  word            reserved2;
  word            mailer;
  word            bbsProgram;
  word            maxBundleSize;
  word            extraHandles; /* 0-235 */
  word            autoRenumber;
  word            bufSize;
  word            ftBufSize;
  word            allowedNumNetmail;
  word            logInfo;
  word            logStyle;
  char            reserved3[68];
  word            colorSet;
  char            sysopName[36];
  word            defaultArc;
  char            reserved4[24];
  char            tearType;
  char            tearLine[25];
  pathType        summaryLogName;
  word            recBoard;
  word            badBoard;
  word            dupBoard;
  char            topic1[16];
  char            topic2[16];
  pathType        bbsPath;
  pathType        netPath;
  pathType        sentPath;
  pathType        rcvdPath;
  pathType        inPath;
  pathType        outPath;
  pathType        securePath;
  pathType        logName;
  pathType        swapPath;
  pathType        semaphorePath;
  pathType        pmailPath;
  pathType        areaMgrLogName;
  pathType        autoRAPath;
  pathType        autoFolderFdPath;
  pathType        autoAreasBBSPath;
  pathType        autoGoldEdAreasPath;
  archiverInfo    unArc;
  archiverInfo    unZip;
  archiverInfo    unLzh;
  archiverInfo    unPak;
  archiverInfo    unZoo;
  archiverInfo    unArj;
  archiverInfo    unSqz;
  archiverInfo    GUS;
  archiverInfo    arc;
  archiverInfo    zip;
  archiverInfo    lzh;
  archiverInfo    pak;
  archiverInfo    zoo;
  archiverInfo    arj;
  archiverInfo    sqz;
  archiverInfo    customArc;
  char            reserved5[83];
  areaOptionsType optionsAKA[MAX_NETAKAS]; /* 'areaOptionsType' below */
  char            groupsQBBS[MAX_NETAKAS];
  word            templateSecQBBS[MAX_NETAKAS];
  char            templateFlagsQBBS[MAX_NETAKAS][4];
  char            attr2RA[MAX_NETAKAS];
  char            aliasesQBBS[MAX_NETAKAS];
  word            groupRA[MAX_NETAKAS];
  word            altGroupRA[MAX_NETAKAS][3];
  char            qwkNameSBBS[MAX_NETAKAS][13];
  word            minAgeSBBS[MAX_NETAKAS];
  word            daysRcvdAKA[MAX_NETAKAS];
  char            replyStatSBBS[MAX_NETAKAS];
  word            attrSBBS[MAX_NETAKAS];
  char            groupDescr[26][27];
  char            reserved6[9];
  char            msgKindsRA[MAX_NETAKAS];
  char            attrRA[MAX_NETAKAS];
  word            readSecRA[MAX_NETAKAS];
  char            readFlagsRA[MAX_NETAKAS][4];
  word            writeSecRA[MAX_NETAKAS];
  char            writeFlagsRA[MAX_NETAKAS][4];
  word            sysopSecRA[MAX_NETAKAS];
  char            sysopFlagsRA[MAX_NETAKAS][4];
  word            daysAKA[MAX_NETAKAS];
  word            msgsAKA[MAX_NETAKAS];
  char            descrAKA[MAX_NETAKAS][51];
  userType        users[MAX_USERS];
  akaMatchType    akaMatch[MAX_MATCH];
  char            reserved7[2048-10*MAX_MATCH];
  uplinkReqType   uplinkReq[MAX_UPLREQ];
} configType;

#define MAX_FORWARD   64

#define MB_PATH_LEN_OLD   19
#define MB_PATH_LEN       61
#define ECHONAME_LEN_090  25
#define ECHONAME_LEN      51
#define COMMENT_LEN       51
#define ORGLINE_LEN       59

typedef char areaNameType[ECHONAME_LEN];

typedef struct {
  word            signature;
  word            writeLevel;
  areaNameType    areaName;
  char            comment[COMMENT_LEN];
  areaOptionsType options;
  word            boardNumRA;
  char            msgBaseType;
  char            msgBasePath[MB_PATH_LEN];
  word            board;
  char            originLine[ORGLINE_LEN];
  word            address;
  dword           group;
  word            alsoSeenBy;
  word            msgs;
  word            days;
  word            daysRcvd;

  ftn_addr        exp[MAX_FORWARD];

  word            readSecRA;
  char            flagsRdNotRA[4];
  char            flagsRdRA[4];
  word            writeSecRA;
  char            flagsWrNotRA[4];
  char            flagsWrRA[4];
  word            sysopSecRA;
  char            flagsSysRA[4];
  char            flagsSysNotRA[4];
  word            templateSecQBBS;
  char            flagsTemplateQBBS[4];
  char            reserved2;
  word            netReplyBoardRA;
  char            boardTypeRA;
  char            attrRA;
  char            attr2RA;
  word            groupRA;
  word            altGroupRA[3];
  char            msgKindsRA;
  char            qwkName[13];
  word            minAgeSBBS;
  word            attrSBBS;
  char            replyStatSBBS;
  char            groupsQBBS;
  char            aliasesQBBS;
} rawEchoType;


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack()
#endif


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
