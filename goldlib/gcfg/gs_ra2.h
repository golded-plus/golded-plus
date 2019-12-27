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
//  RemoteAccess 2.xx configuration data structures.
//  ------------------------------------------------------------------

#ifndef __GS_RA2_H
#define __GS_RA2_H


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif

//  ------------------------------------------------------------------
//
//  C++ structures for RemoteAccess 2.00 GAMMA.
//
//  Based on the document STRUCT.200 which is (C) Copyright Andrew
//  Milner & Continental Software, 1993. All rights reserved.
//
//  ------------------------------------------------------------------

typedef char      Char;
typedef uint8_t   Byte;
typedef uint8_t   Boolean;
typedef  int16_t  Integer;
typedef uint16_t  Word;
typedef  int32_t  LongInt;

typedef Byte       FlagType[4];
typedef Char       ra_Time[6];
typedef Char       Date[9];
typedef Char       LongDate[10];
typedef Byte       ByteArray32[32];

typedef Byte AskType;
typedef Byte VideoType;
typedef Byte MsgType;
typedef Byte MsgKindsType;
typedef Byte OrphanType;

enum _AskType      { Yes, No, Ask, Only };
enum _VideoType    { Auto, Short, Long };
enum _MsgType      { LocalMail, NetMail, EchoMail };
enum _MsgKindsType { Both, Private, Public, ROnly, NoReply };
enum _OrphanType   { Ignore, Create, Kill };

struct NetAddress
{
    Word             Zone,
                     Net,
                     Node,
                     Point;
};

typedef Char       MSGTOIDXrecord[36];

struct USERSIDXrecord
{
    LongInt          NameCRC32,
                     HandleCRC32;
};

typedef Word       COMBINEDrecord[200];

struct USERSrecord
{
    MSGTOIDXrecord   Name;
    Char             Location[26];
    Char             Organisation[51];
    Char             Address1[51];
    Char             Address2[51];
    Char             Address3[51];
    Char             Handle[36];
    Char             Comment[81];
    LongInt          PasswordCRC;
    Char             DataPhone[16];
    Char             VoicePhone[16];
    ra_Time             LastTime;
    Date             LastDate;

    Byte             Attribute,

                     /* Bit 0 : Deleted
                            1 : Clear screen
                            2 : More prompt
                            3 : ANSI
                            4 : No-kill
                            5 : Xfer priority
                            6 : Full screen msg editor
                            7 : Quiet mode */

                     Attribute2;

    /* Bit 0 : Hot-keys
           1 : AVT/0
           2 : Full screen message viewer
           3 : Hidden from userlist
           4 : Page priority
           5 : No echomail in mailbox scan
           6 : Guest account
           7 : Post bill enabled */

    FlagType         Flagsx;
    LongInt          Credit,
                     Pending;
    Word             MsgsPosted,
                     Security;
    LongInt          LastRead,
                     NoCalls,
                     Uploads,
                     Downloads,
                     UploadsK,
                     DownloadsK,
                     TodayK;
    Integer          Elapsed;
    Word             ScreenLength;
    Byte             LastPwdChange;
    Word             Group;
    COMBINEDrecord   CombinedInfo;
    Date             FirstDate,
                     BirthDate,
                     SubDate;
    Byte             ScreenWidth,
                     Language,
                     DateFormat;
    Char             ForwardTo[36];
    Word             MsgArea,
                     FileArea;
    Char             DefaultProtocol;
    Word             FileGroup;
    Byte             LastDOBCheck;
    Byte             Sex;
    LongInt          XIrecord;
    Word             MsgGroup;
    Byte             FreeSpace[48];
};

struct USERSXIrecord
{
    Byte             FreeSpace[200];
};

struct MESSAGErecord
{
    Word             AreaNum;   // NEW in 2.50
    Word             Unused;
    Char             Name[41];
    MsgType          Typ;
    MsgKindsType     MsgKinds;
    Byte             Attribute;

    /* Bit 0 : Enable EchoInfo
           1 : Combined access
           2 : File attaches
           3 : Allow aliases
           4 : Use SoftCRs as Characters
           5 : Force handle
           6 : Allow deletes
           7 : Is a JAM area */

    Byte             DaysKill,      /* Kill older than 'x' days */
                     RecvKill;      /* Kill recv msgs, recv for more than 'x' days */
    Word             CountKill;

    Word             ReadSecurity;
    FlagType         ReadFlags,
                     ReadNotFlags;

    Word             WriteSecurity;
    FlagType         WriteFlags,
                     WriteNotFlags;

    Word             SysopSecurity;
    FlagType         SysopFlags,
                     SysopNotFlags;

    Char             OriginLine[61];
    Byte             AkaAddress;

    Byte             Age;

    Char             JAMbase[61];
    Word             Group;
    Word             AltGroup[3];

    Byte             Attribute2;

    /* Bit 0 : Include in all groups */

    Byte             FreeSpace2[9];
};

struct GROUPrecord
{
    Word             AreaNum;
    Char             Name[41];
    Word             Security;
    FlagType         Flagsx,
                     NotFlagsMask;
    Byte             FreeSpace[100];
};

struct CONFrecord
{
    Char             Name[9];
    Char             Parent[9];
    Char             Desc[71];
    Byte             Attr;

    /* Bit 0 : Private
           1 : Unlisted
           2 : Global
           3 : Permanent
           4 : Use handles
                         */

    Char             Moderator[36];
    Char             Language[21];
    Char             Password[16];
    Word             Security;
    FlagType         Flagsx;
    Byte             NumNodes;
    Byte             Active[250];
    Boolean          Child[250];
    FlagType         NotFlagsMask;
    Byte             FreeSpace[96];
};

struct ARCrecord
{
    Char             Extension[4];
    Char             UnpackCmd[61];
    Char             PackCmd[61];
};

struct CONFIGrecord
{
    Word             VersionID;
    Byte             xCommPort;
    LongInt          xBaud;
    Byte             xInitTries;
    Char             xInitStr[71];
    Char             xBusyStr[71];
    Char             xInitResp[41];
    Char             xBusyResp[41];
    Char             xConnect300[41];
    Char             xConnect1200[41];
    Char             xConnect2400[41];
    Char             xConnect4800[41];
    Char             xConnect9600[41];
    Char             xConnect19k[41];
    Char             xConnect38k[41];
    Boolean          xAnswerPhone;
    Char             xRing[21];
    Char             xAnswerStr[21];
    Boolean          xFlushBuffer;
    Integer          xModemDelay;

    Word             MinimumBaud;
    Word             GraphicsBaud,
                     TransferBaud;
    ra_Time             SlowBaudTimeStart,
                        SlowBaudTimeEnd,
                        DownloadTimeStart,
                        DownloadTimeEnd;

    ra_Time             PageStart[7];
    ra_Time             PageEnd[7];

    Char             SeriNum[23];
    Char             CustNum[23];
    Byte             FreeSpace1[24];
    Word             PwdExpiry;

    Char             MenuPath[61];
    Char             TextPath[61];
    Char             AttachPath[61];
    Char             NodelistPath[61];
    Char             MsgBasePath[61];
    Char             SysPath[61];
    Char             ExternalEdCmd[61];

    NetAddress       Address[10];
    Char             SystemName[31];

    Word             NewSecurity;
    Word             NewCredit;
    FlagType         NewFlags;

    Char             OriginLine[61];
    Char             QuoteString[16];
    Char             Sysop[36];
    Char             LogFileName[61];
    Boolean          FastLogon,
                     AllowSysRem,
                     MonoMode,
                     StrictPwdChecking,
                     DirectWrite,
                     SnowCheck;
    Integer          CreditFactor;

    Word             UserTimeOut,
                     LogonTime,
                     PasswordTries,
                     MaxPage,
                     PageLength;
    Boolean          CheckForMultiLogon,
                     ExcludeSysopFromList,
                     OneWordNames;
    AskType          CheckMail;
    Boolean          AskVoicePhone,
                     AskDataPhone,
                     DoFullMailCheck,
                     AllowFileShells,
                     FixUploadDates,
                     FreezeChat;
    AskType          ANSI,              /* ANSI: Yes, no, or ask new users     */
                     ClearScreen,       /* Clear:        "                     */
                     MorePrompt;        /* More:         "                     */
    Boolean          UploadMsgs;
    AskType          KillSent;          /* Kill/Sent     "                     */

    Word             CrashAskSec;       /* Min sec# to ask 'Crash Mail ?'      */
    FlagType         CrashAskFlags;
    Word             CrashSec;          /* Min sec# to always send crash mail. */
    FlagType         CrashFlags;
    Word             FAttachSec;        /*        "    ask 'File Attach ?'     */
    FlagType         FAttachFlags;

    Byte             NormFore,
                     NormBack,
                     StatFore,
                     StatBack,
                     HiBack,
                     HiFore,
                     WindFore,
                     WindBack,
                     ExitLocal,
                     Exit300,
                     Exit1200,
                     Exit2400,
                     Exit4800,
                     Exit9600,
                     Exit19k,
                     Exit38k;

    Boolean          MultiLine;
    Byte             MinPwdLen;
    Word             MinUpSpace;
    AskType          HotKeys;
    Byte             BorderFore,
                     BorderBack,
                     BarFore,
                     BarBack,
                     LogStyle,
                     MultiTasker,
                     PwdBoard;
    Word             xBufferSize;
    Char             FKeys[10][61];

    Boolean          WhyPage;
    Byte             LeaveMsg;
    Boolean          ShowMissingFiles,
                     xLockModem;
    Byte             FreeSpace2[10];
    Boolean          AllowNetmailReplies;
    Char             LogonPrompt[41];
    AskType          CheckNewFiles;
    Char             ReplyHeader[61];
    Byte             BlankSecs;
    Byte             ProtocolAttrib[6];
    Char             xErrorFreeString[16];
    Byte             xDefaultCombined[25];
    Word             RenumThreshold;
    Char             LeftBracket,
                     RightBracket;
    Boolean          AskForHandle;
    Boolean          AskForBirthDate;

    Word             GroupMailSec;

    Boolean          ConfirmMsgDeletes;

    Byte             FreeSpace4[30];

    Char             TempScanDir[61];
    AskType          ScanNow;
    Byte             xUnknownArcAction,
                     xFailedUnpackAction,
                     FailedScanAction;      /* Bit 0:Mark deleted, 1:Mark unlisted, 2:Mark notavail */
    Word             xUnknownArcArea,
                     xFailedUnpackArea,
                     FailedScanArea;
    Char             ScanCmd[61];
    Boolean          xDeductIfUnknown;

    Byte             NewUserGroup;
    AskType          AVATAR;
    Byte             BadPwdArea;
    Char             Location[41];
    Byte             DoAfterAction;         /* 0 = wait for CR, > 0 = wait for x seconds */
    Char             OldFileLine[41];
    Byte             CRfore,
                     CRback;
    Char             LangHdr[41];
    Boolean          xSendBreak;
    Char             ListPath[61];
    AskType          FullMsgView;
    AskType          EMSI_Enable;
    Boolean          EMSI_NewUser;

    Char             EchoChar[2];
    Char             xConnect7200[41];
    Char             xConnect12000[41];
    Char             xConnect14400[41];
    Byte             Exit7200;
    Byte             Exit12000;
    Byte             Exit14400;
    Char             ChatCommand[61];
    AskType          ExtEd;
    Byte             NewuserLanguage;
    Char             LanguagePrompt[41];
    VideoType        VideoMode;
    Boolean          AutoDetectANSI;
    Boolean          xOffHook;
    Byte             NewUserDateFormat;
    Char             KeyboardPwd[16];
    Boolean          CapLocation;
    Byte             NewuserSub;
    Char             PrinterName[5];
    Byte             HilitePromptFore,
                     HiLitePromptBack;
    Char             xInitStr2[71];
    Boolean          AltJSwap;
    Char             SemPath[61];
    Boolean          AutoChatCapture;

    Char             FileBasePath[61];
    Boolean          NewFileTag;
    Boolean          IgnoreDupeExt;
    Char             TempCDFilePath[61];
    Byte             TagFore,
                     TagBack;
    Char             xConnect16k[41];
    Byte             Exit16k,
                     FilePayback;
    Char             FileLine[201];
    Char             FileMissingLine[201];
    Byte             NewUserULCredit;
    Word             NewUserULCreditK;
    ARCrecord        ArcInfo[10];
    Char             RAMGRAltFKeys[5][61];
    Char             ArcViewCmd[61];
    Char             xConnectFax[41];
    Byte             ExitFax;
    Boolean          UseXMS,
                     UseEMS;
    Byte             CheckDOB;
    AskType          EchoCheck;
    Word             ccSec,
                     ReturnRecSec;
    Boolean          HonourNetReq;
    COMBINEDrecord   DefaultCombined;
    Boolean          AskForSex,
                     AskForAddress;
    AskType          DLdesc;
    Boolean          NewPhoneScan;
    Byte             FutureExpansion[587];
};


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack()
#endif

//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------

