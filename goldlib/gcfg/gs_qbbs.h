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
//  QuickBBS (old and new) configuration data structures
//  ------------------------------------------------------------------

#ifndef __GS_QBBS_H
#define __GS_QBBS_H


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif

//  ------------------------------------------------------------------
//  QuickBBS 2.60, CONFIG.BBS structure

typedef struct
{
    byte  Status;       // 0=Disabled 1=Enabled
    char  RunTime[6];
    byte  ErrorLevel;
    byte  Days;
    byte  Forced;
    char  LastTimeRun[9];
} Q260EvtRecT, *Q260EvtRecP, **EvtRecI;

typedef struct
{
    char  Name[17];
    byte  Typ;          // 0=Standard 1=Net 3=Echo
    byte  Kinds;        // 0=Private & Public 1=Private 2=Public 3=Read-Only
    byte  Combined;
    byte  Aliases;      // 0=no aliases,1=yes,2=ask alias,3=Alias/name
    short  ReadSecLvl;
    byte  ReadFlags[4];
    short  WriteSecLvl;
    byte  WriteFlags[4];
    short  SysopSecLvl;
    byte  SysopFlags[4];
} Q260BrdRecT, *Q260BrdRecP, **Q260BrdRecI;

typedef struct
{
    short        CommPort;
    word        InitBaud;
    word        InitTimes;
    word        AnswerWait;
    char        ModemInitStr[71];
    char        ModemBusyStr[71];
    char        ModemInitResp[41];
    char        ModemBusyResp[41];
    char        Resp300[41];
    char        Resp1200[41];
    char        Resp2400[41];
    char        MenuPath[67];
    char        TextPath[67];
    char        NetPath[67];
    short        MinBaud;
    short        GraphicsBaud;
    short        XferBaud;
    char        LowBaudStart[6];
    char        LowBaudEnd[6];
    char        DownloadStart[6];
    char        DownloadEnd[6];
    char        PagingStart[6];
    char        PagingEnd[6];
    short        MatrixZone;
    short        MatrixNet;
    short        MatrixNode;
    short        AkaNet[5];
    short        AkaNode[5];
    short        NetMailBoard;
    short        DefaultSec;
    short        DefaultCredit;
    byte        DefaultFlags[4];
    char        EditorCmdStr[71];
    char        OriginLine[61];
    char        SysopName[36];
    byte        AutoLogonChar;
    byte        FastLogon;
    byte        ScreenBlanking;
    byte        UseLastRead;
    byte        MonoMode;
    byte        DirectWrite;
    byte        SnowCheck;
    byte        NetEchoExit;
    byte        OneWordNames;
    byte        CheckMail;
    byte        AskHomePhone;
    byte        AskDataPhone;
    byte        GraphicsAvail;
    short        InactiveTimeOut;
    short        LogonTime;
    short        DefFgColor;
    short        DefBgColor;
    short        PasswordTries;
    short        MaxPageTimes;
    short        PageBellLen;
    byte        Use_Xmodem;
    byte        Use_Xmodem1k;
    byte        Use_Ymodem;
    byte        Use_YmodemG;
    byte        Use_Sealink;
    byte        Use_Zmodem;
    byte        Inp_Fields;
    char        QuoteStr[4];
    short        UploadCredit;
    char        LoadingMessage[71];
    char        SelectionPrompt[71];
    word        VersionID;
    char        Resp4800[41];
    char        Resp9600[41];
    short        AkaZone[5];
    short        MatrixPoint;
    short        AkaPoint[5];
    byte        UseAka[200];
    byte        Spare[51];
    Q260EvtRecT Events[30];
    Q260BrdRecT Boards[200];
} Q260CfgRecT, *Q260CfgRecP;


//  ------------------------------------------------------------------
//  QuickBBS 2.76.G2, USERS.BBS structure

typedef struct
{
    char  Name[36];
    char  City[26];
    char  Pwd[16];
    char  DataPhone[13];
    char  HomePhone[13];
    char  LastTime[6];
    char  LastDate[9];
    byte  Attrib;
    byte  Flagsx[4];
    word  Credit;
    word  Pending;
    word  TimesPosted;
    word  HighMsgRead;
    word  SecLvl;
    word  Times;
    word  Ups;
    word  Downs;
    word  UpK;
    word  DownK;
    short  TodayK;
    short  Elapsed;
    short  Len;
    word  CombinedPtr;  // Record number in COMBINED.BBS
    word  AliasPtr;     // Record number in ALIAS.BBS
    long  Birthday;
} Q276UsrRecT, *Q276UsrRecP, **Q276UsrRecI;

//    Attrib:
//
//    Bit 0: Deleted
//    Bit 1: Screen Clear Codes
//    Bit 2: More Prompt
//    Bit 3: ANSI
//    Bit 4: No-Kill
//    Bit 5: Ignore Download Hours
//    Bit 6: ANSI Full Screen Editor
//    Bit 7: Sex (0=male, 1=female)


//  ------------------------------------------------------------------
//  QuickBBS 2.76.G2, MSGCFG.DAT structure

typedef struct
{
    word  Security;
    byte  Flagsx[4];
} Q276SecRecT;

typedef struct
{
    char        Name[41];
    byte        Typ;
    byte        Kinds;
    byte        Combined;
    byte        Aliases;
    byte        Aka;
    char        OriginLine[59];
    byte        AllowDelete;
    word        KeepCnt;
    word        KillRcvd;
    word        KillOld;
    Q276SecRecT ReadSec;
    Q276SecRecT WriteSec;
    Q276SecRecT TemplateSec;
    Q276SecRecT SysopSec;
    short        FileArea;
    byte        Group;
    byte        Spare[9];
} Q276BrdRecT, *Q276BrdRecP, **Q276BrdRecI;


//  ------------------------------------------------------------------
//  QuickBBS 2.76.G2, QUICKCFG.DAT structure

typedef struct
{
    char  TranslateFrom[26];
    word  TranslateTo;
} Q276MdmXltRecT;

typedef struct
{
    word            VersionID;
    byte            Node;
    short            CommPort;             // Modem Parameters
    word            InitBaud;
    word            ModemDelay;
    short            InitTimes;
    short            AnswerWait;
    char            ModemInitStr[71];
    char            ModemBusyStr[71];
    char            ModemInitResp[41];
    char            ModemBusyResp[41];
    Q276MdmXltRecT  ModemConnectResp[8];
    word            CBV_CallbackDelay;
    word            CBV_WakeupDelay;
    char            ARQ_String[9];
    byte            ModemSpareInfo[9];
    char            EditorCmdStr[71];     // System Paths
    char            MenuPath[67];
    char            TextPath[67];
    char            NetPath[67];
    char            NodelistPath[67];
    char            MsgPath[67];
    char            SwapPath[67];
    char            OverlayPath[67];
    char            LoadingMessage[71];   // System misc strings
    char            SelectionPrompt[71];
    char            NoMailString[71];
    char            OriginLine[59];
    char            QuoteStr[4];
    long            LowBaudStart;         // User Restrictions
    long            LowBaudEnd;
    long            DownloadStart;
    long            DownloadEnd;
    short            MaxPageTimes;
    short            PageBellLen;
    long            PageStart;
    long            PageEnd;
    short            MinBaud;
    short            GraphicsBaud;
    short            XferBaud;
    short            MatrixZone[11];       // Matrix Information
    short            MatrixNet[11];
    short            MatrixNode[11];
    short            MatrixPoint[11];
    short            NetMailBoard;
    Q276SecRecT     DefaultSec;           // Default Information for New Users
    Q276SecRecT     MinimumSec;
    short            DefaultCredit;
    Q276SecRecT     SysopSecurity;        // Sysop Security Etc.
    char            SysopName[36];
    char            SystemName[41];
    long            RegKey;
    byte            TextFileShells;       // Misc System Parameters
    byte            AltJswap;
    byte            Editorswap;
    byte            AutoLogonChar;
    byte            FastLogon;
    byte            UseLastRead;
    byte            MonoMode;
    byte            DirectWrite;
    byte            SnowCheck;
    byte            NetEchoExit;
    byte            OneWordNames;
    byte            CheckMail;
    byte            AskHomePhone;
    byte            AskDataPhone;
    byte            AskBirthday;
    byte            AskSex;
    byte            Use_Xmodem;
    byte            Use_Xmodem1k;
    byte            Use_Ymodem;
    byte            Use_YmodemG;
    byte            Use_Kermit;
    byte            Use_Zmodem;
    byte            Inp_Fields;
    byte            GraphicsAvail;
    byte            ForceUS_Phone;
    short            InactiveTimeOut;
    short            LogonTime;
    short            DefFgColor;
    short            DefBgColor;
    short            PasswordTries;
    byte            EntFldColor;
    byte            BorderColor;
    byte            WindowColor;
    byte            StatusBarColor;
    short            UploadCredit;
    byte            ScreenBlank;
    char            VerifierInit[36];     // Callback verifier
    char            DialString[16];
    char            DialSuffix[16];
    byte            DupeCheck;
    word            NewUserSec;
    word            MemberSec;
    char            MemberFlags[4][8];
    word            LDcost;
    byte            LDenable;
    byte            ResumeLocal;
    byte            ResumeLD;
    long            LDstart;
    long            LDend;
    byte            ForgotPwdBoard;
    byte            SendATA;
    char            Location[61];
    byte            ArchiveNetMail;
    byte            IEMSI;
    byte            IEMSI_New;
    byte            AutoAnsi;
    byte            MultiNode;
    byte            ExtraSpace[332];
} Q276CfgRecT, *Q276CfgRecP;


//  ------------------------------------------------------------------
//  RemoteAccess 2.00 GAMMA, USERS.BBS structure

// Some Pascal types
typedef char      Char;
typedef uint8_t   Byte;
typedef uint8_t   Boolean;
typedef  int16_t  Integer;
typedef uint16_t  Word;
typedef  int32_t  LongInt;

typedef Byte            FlagType[4];
typedef Char            Time[6];
typedef Char            Date[9];

typedef struct
{

    char  name[36];
    char  location[26];
    char  organisation[51];
    char  address1[51];
    char  address2[51];
    char  address3[51];
    char  handle[36];
    char  comment[81];
    long  passwordcrc;
    char  dataphone[16];
    char  voicephone[16];
    char  lasttime[6];
    char  lastdate[9];
    byte  attribute;
    byte  attribute2;
    byte  flagsx[4];
    long  credit;
    long  pending;
    word  msgsposted;
    word  security;
    long  lastread;
    long  nocalls;
    long  uploads;
    long  downloads;
    long  uploadsk;
    long  downloadsk;
    long  todayk;
    short  elapsed;
    word  screenlength;
    byte  lastpwdchange;
    word  group;
    word  combinedinfo[200];
    char  firstdate[9];
    char  birthdate[9];
    char  subdate[9];
    byte  screenwidth;
    byte  language;
    byte  dateformat;
    char  forwardto[36];
    word  msgarea;
    word  filearea;
    char  defaultprotocol;
    word  filegroup;
    byte  lastdobcheck;
    byte  sex;
    long  xirecord;
    word  msggroup;
    byte  freespace[48];

} RA2UsrRecT, *RA2UsrRecP, **RA2UsrRecI;


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack()
#endif

//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------

