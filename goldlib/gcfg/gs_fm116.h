//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1996 Folkert J. Wijnstra
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
//  File structures for FMail 1.16
//  ------------------------------------------------------------------

#ifndef __gs_fm116_h
#define __gs_fm116_h


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
//  File header structure

#define DATATYPE_CF    0x0102 // not used yet
#define DATATYPE_NO    0x0202 // node file
#define DATATYPE_AD    0x0401 // area file for echo mail defaults
#define DATATYPE_AE    0x0402 // area file for echo mail

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
  word _reserved   : 1; /* Bit 10 */
  word allowAreafix: 1; /* Bit 11 */
  word             : 2; /* Bit 12-13 */
  word arrivalDate : 1; /* Bit 14 */
  word sysopRead   : 1; /* Bit 15 */
} areaOptionsType;


/* ********** FMAIL.CFG ********** */

#define MAX_AKAS      32
#define MAX_AKAS_F    64
#define MAX_AKAS_OLD  16
#define MAX_NA_OLD    11
#define MAX_NETAKAS   32
#define MAX_NETAKAS_F 64
#define MAX_USERS     16
#define MAX_UPLREQ    32
#define MAX_MATCH     16           // not used yet

typedef nodeFakeType _akaListType[MAX_AKAS_OLD];
typedef nodeFakeType akaListType[MAX_AKAS_F];

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
  word              : 2;
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
  word              : 2; /* Bit 2-3 */
  word autoDiscArea : 1; /* Bit  4 */
  word autoDiscDel  : 1; /* Bit  5 has temp. no effect, rec is always deleted */
  word              : 3; /* Bit 6-8 */
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
  word               :  3;
  word unconditional :  1; /* BIT 4 */
  word               : 11;
} uplOptType;

typedef struct {
  char userName[36];
  byte reserved[28];
} userType;

typedef struct {
  ftn_addr    node;
  char        program[9];
  byte        password[17];
  char        fileName[13];
  byte        fileType;
  dword       groups;
  byte        originAka;
  uplOptType  options;
  byte        reserved[9];
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
  byte            versionMajor;
  byte            versionMinor;
  sdword          creationDate;
  dword           key;
  dword           reservedKey;
  dword           relKey1;
  dword           relKey2;
  byte            reserved1[22];
  mgrOptionsType  mgrOptions;
  _akaListType    _akaList;
  word            _netmailBoard[MAX_NA_OLD];
  word            _reservedNet[16-MAX_NA_OLD];
  genOptionsType  genOptions;
  mbOptionsType   mbOptions;
  mailOptionsType mailOptions;
  word            maxPktSize;
  word            kDupRecs;
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
  byte            reserved2[68];
  word            colorSet;
  char            sysopName[36];
  word            defaultArc;
  word            _adiscDaysNode;
  word            _adiscDaysPoint;
  word            _adiscSizeNode;
  word            _adiscSizePoint;
  byte            reserved3[16];
  byte            tearType;
  char            tearLine[25];
  pathType        summaryLogName;
  word            recBoard;
  word            badBoard;
  word            dupBoard;
  byte            topic1[16];
  byte            topic2[16];
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
  pathType        autoFMail102Path;
  byte            reserved4[35];
  areaOptionsType _optionsAKA[MAX_NA_OLD];
  byte            _groupsQBBS[MAX_NA_OLD];
  word            _templateSecQBBS[MAX_NA_OLD];
  byte            _templateFlagsQBBS[MAX_NA_OLD][4];
  byte            _attr2RA[MAX_NA_OLD];
  byte            _aliasesQBBS[MAX_NA_OLD];
  word            _groupRA[MAX_NA_OLD];
  word            _altGroupRA[MAX_NA_OLD][3];
  byte            _qwkName[MAX_NA_OLD][13];
  word            _minAgeSBBS[MAX_NA_OLD];
  word            _daysRcvdAKA[MAX_NA_OLD];
  byte            _replyStatSBBS[MAX_NA_OLD];
  word            _attrSBBS[MAX_NA_OLD];
  byte            groupDescr[26][27];
  byte            reserved5[9];
  byte            _msgKindsRA[MAX_NA_OLD];
  byte            _attrRA[MAX_NA_OLD];
  word            _readSecRA[MAX_NA_OLD];
  byte            _readFlagsRA[MAX_NA_OLD][4];
  word            _writeSecRA[MAX_NA_OLD];
  byte            _writeFlagsRA[MAX_NA_OLD][4];
  word            _sysopSecRA[MAX_NA_OLD];
  byte            _sysopFlagsRA[MAX_NA_OLD][4];
  word            _daysAKA[MAX_NA_OLD];
  word            _msgsAKA[MAX_NA_OLD];
  byte            _descrAKA[MAX_NA_OLD][51];
  userType        users[MAX_USERS];
  akaMatchType    akaMatch[MAX_MATCH];     // not used yet
  byte            reserved6[1040-10*MAX_MATCH];
  pathType        sentEchoPath;
  archiverInfo    preUnarc;
  archiverInfo    postUnarc;
  archiverInfo    preArc;
  archiverInfo    postArc;
  archiverInfo    unUc2;
  archiverInfo    unRar;
  archiverInfo    resUnpack[6];
  archiverInfo    uc2;
  archiverInfo    rar;
  archiverInfo    resPack[6];
  uplinkReqType   uplinkReq[MAX_UPLREQ+32];
  archiverInfo    unArc32;
  archiverInfo    unZip32;
  archiverInfo    unLzh32;
  archiverInfo    unPak32;
  archiverInfo    unZoo32;
  archiverInfo    unArj32;
  archiverInfo    unSqz32;
  archiverInfo    unUc232;
  archiverInfo    unRar32;
  archiverInfo    GUS32;
  archiverInfo    resUnpack32[6];
  archiverInfo    preUnarc32;
  archiverInfo    postUnarc32;
  archiverInfo    arc32;
  archiverInfo    zip32;
  archiverInfo    lzh32;
  archiverInfo    pak32;
  archiverInfo    zoo32;
  archiverInfo    arj32;
  archiverInfo    sqz32;
  archiverInfo    uc232;
  archiverInfo    rar32;
  archiverInfo    customArc32;
  archiverInfo    resPack32[6];
  archiverInfo    preArc32;
  archiverInfo    postArc32;
  byte            descrAKA[MAX_NETAKAS][51];
  byte            qwkName[MAX_NETAKAS][13];
  areaOptionsType optionsAKA[MAX_NETAKAS];
  byte            msgKindsRA[MAX_NETAKAS];
  word            daysAKA[MAX_NETAKAS];
  word            msgsAKA[MAX_NETAKAS];
  byte            groupsQBBS[MAX_NETAKAS];
  byte            attrRA[MAX_NETAKAS];
  byte            attr2RA[MAX_NETAKAS];
  word            attrSBBS[MAX_NETAKAS];
  byte            aliasesQBBS[MAX_NETAKAS];
  word            groupRA[MAX_NETAKAS];
  word            altGroupRA[MAX_NETAKAS][3];
  word            minAgeSBBS[MAX_NETAKAS];
  word            daysRcvdAKA[MAX_NETAKAS];
  byte            replyStatSBBS[MAX_NETAKAS];
  word            readSecRA[MAX_NETAKAS];
  byte            readFlagsRA[MAX_NETAKAS][8];
  word            writeSecRA[MAX_NETAKAS];
  byte            writeFlagsRA[MAX_NETAKAS][8];
  word            sysopSecRA[MAX_NETAKAS];
  byte            sysopFlagsRA[MAX_NETAKAS][8];
  word            templateSecQBBS[MAX_NETAKAS];
  byte            templateFlagsQBBS[MAX_NETAKAS][8];
  byte            reserved7[512];
  word            netmailBoard[MAX_NETAKAS_F];
  akaListType     akaList;
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
  word tossedTo     : 1;  /* BIT 0 */
  word              : 15; /* BIT 1-15 */
} areaStatType;

typedef struct {

  word            signature; // contains "AE" for echo areas in FMAIL.AR and
			      // "AD" for default settings in FMAIL.ARD
  word            writeLevel;
  areaNameType    areaName;
  char            comment[COMMENT_LEN];
  areaOptionsType options;
  word            boardNumRA;
  byte            msgBaseType;
  char            msgBasePath[MB_PATH_LEN];
  word            board;
  char            originLine[ORGLINE_LEN];
  word            address;
  dword           group;
  word            _alsoSeenBy; // obsolete: see the 32-bit alsoSeenBy below
  word            msgs;
  word            days;
  word            daysRcvd;

  ftn_addr        exp[MAX_FORWARD];

  word            readSecRA;
  byte            flagsRdRA[4];
  byte            flagsRdNotRA[4];
  word            writeSecRA;
  byte            flagsWrRA[4];
  byte            flagsWrNotRA[4];
  word            sysopSecRA;
  byte            flagsSysRA[4];
  byte            flagsSysNotRA[4];
  word            templateSecQBBS;
  byte            flagsTemplateQBBS[4];
  byte            _internalUse;
  word            netReplyBoardRA;
  byte            boardTypeRA;
  byte            attrRA;
  byte            attr2RA;
  word            groupRA;
  word            altGroupRA[3];
  byte            msgKindsRA;
  byte            qwkName[13];
  word            minAgeSBBS;
  word            attrSBBS;
  byte            replyStatSBBS;
  byte            groupsQBBS;
  byte            aliasesQBBS;
  dword           lastMsgTossDat;
  dword           lastMsgScanDat;
  dword           alsoSeenBy;
  areaStatType    stat;
  byte            reserved[180];
} rawEchoType116;


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack()
#endif


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
