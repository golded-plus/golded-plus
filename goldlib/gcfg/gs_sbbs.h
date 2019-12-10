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
//  SuperBBS configuration data structures
//  ------------------------------------------------------------------

#if defined (GOLD_CANPACK)
#pragma pack(1)
#endif
//  ------------------------------------------------------------------
//  Misc types
typedef char MsgToIdxRecord[36];
typedef byte SbFlagType[4];
//  ------------------------------------------------------------------
//  Structure of SCONFIG.BBS
typedef struct
{
    char SystemPath[67];
    char MsgBasePath[67];
    char TempPath[67];
    char RedirectDevice[13];
    char Resp19200[41];
    char Resp38400[41];
    byte ShowSecurity;
    byte ShowPswdAtLogon;
    long BetaCode;
    byte Unused[200];
    byte Empty1[284];
    byte AllowCat;
    byte Answerboard;
    byte Use7bitChars;
    word BufferSize;
    byte Empty2[45];
    byte EntryColor;
    byte FilesBeforeRatio;
    word MinSpace;
    byte ClearTemp;
    byte Empty3[163];
    byte UseAliasSystem;
    byte Ansilogon;           // 0 = No, 1 = Yes, 2 = Auto detect
    byte ExcludeSysOp;
    byte Empty4[122];
    char AltFkeys[10][61];
    byte Checkfiles;
    byte PasswordBoard;
    byte Alt_J_Swaptoems;
    word MaxTimeInBank;
    byte NoBirthDayAsk;
    byte MaxDescLines;
    byte Empty5[45];
    byte FastMailCheck;
    byte DefaultColors[10];
    byte Empty6[45];
    byte LinesInSystem;
    char CommonDir[80];
    word VersionNumber;
    word FullScrChatBaud;
    byte PressEnterColor;
    byte AreaSelectColor[3];
    byte StatusLineColor;
    byte VirusChkMode;
    char WorkPath[80];
    byte WatchCDatDos;
    byte UseCtrlX;
    byte MsgAfterPageBoard;
    byte SearchAliases;
    byte EchoConvert;
    byte SwapAtPackers;
    byte InternalProtocols;
    // bit 0 : Zmodem
    // bit 1 : Ymodem batch
    // bit 2 : Xmodem
    // bit 3 : Xmodem 1K (Ymodem)
    // bit 4 : Xmodem 1K G
    // bit 5 : Ymodem-g
    char  FirstMenuInSystem[9];
    byte  ColoredQuoteText;
    byte  AddUploadersName;
    word  DaysUntilExpiration;
    char  QWKBBSID[7];
    byte  QWKDefaultBoard;
    short QwkMaxNrToPack;
    char  StatuslineStr[2][101];
    char  Location[41];
    byte  UsPhoneNrFormat;
    char  DlCounterLBracket;
    char  DlCounterRBracket;
    byte  QWKAllowTagLines;   // 0=no, 1=yes, 2=only local areas
    word  LogonSecLvl;
    byte  LogonAge;
    char  LogonPassword[16];
    byte  NoGroupMailCheck;
    byte  NoUserListIntype27;
    word  GroupMsgSecLvl;
    byte  AskSex;
    byte  AskAnsi;            // 0=ask, 1=yes, 2=no
    byte  AskMailCheck;       // 0=ask, 1=yes, 2=no
    byte  AskNewFilesCheck;   // 0=ask, 1=yes, 2=no
    byte  AskFullScrEditor;   // 0=ask, 1=yes, 2=no
    byte  AskFullScrViewer;   // 0=ask, 1=yes, 2=no
    byte  AskPagePause;       // 0=ask, 1=yes, 2=no
    byte  AskScreenClears;    // 0=ask, 1=yes, 2=no
    char  PageStart[7][6];
    char  PageEnd[7][6];
    byte  UseIEMSI;
    byte  CapitalizeCity;
    byte  CapitalizeAlias;
    char  VirusScannerCmd[128];
    byte  NoExtractBeforeScan;
    byte  VirusFoundErrorLvl;
    byte  SysOpsFscrChatClr;
    byte  UsersFscrChatClr;
    byte  ScanForDupePhones;  // 0 = no, 1 = log entry, 2 = don't allow
    byte  NormalLongDesc;
    char  NodelistPath[73];
    word  MinCrashMailSec;
    byte  NoDorinfo1;
    byte  NoTouch;
    byte  ZoneMatching;
    byte  strictdupecheck;
    byte  noextendednodeinfo;
    byte  Birthdayverify;
    byte  realmsgnumbers;
    char  LocalAttachPath[67];
    byte  NoLACheckAtLogon;
    byte  AllowedPackers;
    // Bit 0 ; ZIP
    // Bit 1 ; ARC
    // Bit 2 ; LZH
    // Bit 3 ; ARJ
    byte BoardsFileToCurDir;
    char UnLockPwd[16];
    char Resp7200[41];
    char Resp12000[41];
    char Resp14400[41];
    byte LineEditorQuote;     // 0=intelligent, 1=normal, 2=no
    byte SevenBitOnly;
    byte nofileflagging;
    char ReplyingToStr[81];
    byte FreeFileTimeCheck;   // 0 = NO, 1 = YES, 2 = EVENT
    byte PageReasonToSubj;
    byte AgeGraphStartAge;
    word ETGCLocalAttrib;
    word ETGCRemoteAttrib;       // Not yet used
                                 // BIT 0 ; 320x200x16
                                 // BIT 1 ; 640x200x16
                                 // BIT 2 ; 640x350x16
                                 // BIT 3 ; 640x480x16
                                 // BIT 4 ; 320x200x256
    byte       AskCombinedBoards;
    byte       AskCombMailCheck; // 0=ask, 1=yes, 2=no
    byte       UseNewFilesCheckDate;
    word       BurnOutSecs;
    byte       NoNamesInGroupMail;
    byte       AutoChatCapture;
    byte       AskDefProto;
    byte       AskHotkeys;    // 0=ask, 1=yes, 2=no
    byte       FreeAttaches;
    SbFlagType LogonFlags;
    byte       NoRemoteSysOp;
    byte       FscrReaderColors[12];
    byte       NewsDateCheck;
    byte       NoNameIdx;
    word       AkaZone[15];
    word       AkaNet[15];
    word       AkaNode[15];
    word       AkaPoint[15];
    byte       MinorVersionNumber; // ie. 1 = Gamma-1, 2 = Gamma-2, 100 = official
    byte       Extraspace[2709];
} ExtraConfigT, * ExtraConfigP, ** ExtraConfigI;
//  ------------------------------------------------------------------
//  Structure of SUSERS.BBS
typedef struct
{
    MsgToIdxRecord Name;
    char           Birthday[9];
    word           Attrib;
    byte           Flagsx[4]; // Not yet used
    char           Firsttime[6];
    char           FirstDate[8];
    byte           CombinedBoards[25];
    char           SysOpComment[80];
    char           DefaultProto;
    short          UserRecPtr;
    byte           Colors[10];
    byte           FileListType; // Not yet used
    MsgToIdxRecord Alias;
    long           MinutesUsed;
    char           ViewFileName[13]; // SeeAlso attrib bits 4 - 6
    char           MenuToRun[9];
    word           Timeinbank;
    byte           TodayCalls;
    char           LanguageFileN[9];
    char           Expirationdate[9];
    byte           CurrentFArea;
    byte           CurrentMArea;
    byte           CurrentDoor;
    byte           DefaultPacker;
    //  not bit mapped !
    //   1 = ZIP
    //   2 = ARC
    //   3 = LZH
    //   4 = ARJ
    char LastNewFilesCheck[9];
    byte ExtraSpace[403];
} ExtraUserRecT, * ExtraUserRecP, ** ExtraUserRecI;
//  Attrib;
//
//  1     Bit  0; MailCheck at logon
//  2     Bit  1; Newfiles Check at logon
//  4     Bit  2; No downloadratio
//  8     Bit  3; Female
//  16    Bit  4; Delete file after viewing
//  32    Bit  5; Display file only once
//  64    Bit  6; File viewed
//  128   Bit  7; Fullscreen viewer
//  256   Bit  8; Combined mail check
//  512   Bit  9; No hotkeys
//        Bit 10;
//        Bit 11;
//        Bit 12;
//        Bit 13;
//        Bit 14;
//        Bit 15;
//  ------------------------------------------------------------------
//  Structure of USERS.BBS
typedef struct
{
    MsgToIdxRecord Name;
    char           City[26];
    char           Password[16];
    char           DataPhone[13];
    char           HomePhone[13];
    char           LastTime[6];
    char           LastDate[9];
    byte           Attrib;
    SbFlagType     Flagsx;
    short          Credit;
    short          Pending;
    word           MsgsPosted;
    word           HighMsgRead;
    word           SecLvl;
    word           Times;
    word           Ups;
    word           Downs;
    word           UpK;
    word           DownK;
    short          TodayK;
    short          Elapsed;
    short          Len;
    byte           ExtraSpace1[2];
    byte           Age;
    short          ExtraUserrecPtr;
    byte           ExtraSpace2[3];
} UserRecordT, * UserRecordP, ** UserRecordI;
//  Attrib;
//
//  bit 0; Deleted user
//  bit 1; Screen clear codes
//  bit 2; More prompt
//  bit 3; ANSI graphics
//  bit 4; No-Kill
//  bit 5; Ignore Download Hours
//  bit 6; ANSI editor
//  bit 7; Do not disturb mode ON
//
//
//  ------------------------------------------------------------------
//  Structure of CONFIG.BBS
typedef struct
{
    byte Status;      // 0=Disabled 1=Enabled
    char RunTime[6];
    byte ErrorLevel;
    byte Days;
    byte Forced;
    char LastTimeRun[9];
} EventRecordT, * EventRecordP, ** EventRecordI;
typedef struct
{
    char       Name[17];
    byte       Typ;     // 0=Standard 1=Net 3=Echo
    byte       Kinds;   // 0=Private & Public 1=Private 2=Public 3=Read-Only
    byte       Combined;
    byte       Aliases; // 0=no aliases,1=yes,2=ask alias,3=Alias/name
    short      ReadSecLvl;
    SbFlagType ReadFlags;
    short      WriteSecLvl;
    SbFlagType WriteFlags;
    short      SysopSecLvl;
    SbFlagType SysopFlags;
} BoardRecordT, * BoardRecordP, ** BoardRecordI;
typedef struct
{
    short          CommPort;
    word           InitBaud;
    word           InitTimes;
    word           AnswerWait;
    char           ModemInitStr[71];
    char           ModemBusyStr[71];
    char           ModemInitResp[41];
    char           ModemBusyResp[41];
    char           Resp300[41];
    char           Resp1200[41];
    char           Resp2400[41];
    char           MenuPath[67];
    char           TextPath[67];
    char           NetPath[67];
    short          MinBaud;
    short          GraphicsBaud;
    short          XferBaud;
    char           LowBaudStart[6];
    char           LowBaudEnd[6];
    char           DownloadStart[6];
    char           DownloadEnd[6];
    char           PagingStart[6]; // !! Not used anymore !!
    char           PagingEnd[6]; // !! Not used anymore !!
    short          MatrixZone;
    short          MatrixNet;
    short          MatrixNode;
    short          OldAkaNet[5]; // not used
    short          OldAkaNode[5]; // not used
    short          NetMailBoard;
    short          DefaultSec;
    short          DefaultCredit;
    SbFlagType     DefaultFlags;
    char           EditorCmdStr[71];
    char           OriginLine[61];
    MsgToIdxRecord SysopName;
    byte           AutoLogonChar;
    byte           FastLogon;
    byte           ScreenBlanking;
    byte           UseLastRead;
    byte           MonoMode;
    byte           DirectWrite;
    byte           SnowCheck;
    byte           NetEchoExit;
    byte           OneWordNames;
    byte           CheckMail;
    byte           AskHomePhone;
    byte           AskDataPhone;
    byte           GraphicsAvail;
    short          InactiveTimeOut;
    short          LogonTime;
    short          DefFgColor;
    short          DefBgColor;
    short          PasswordTries;
    short          MaxPageTimes;
    short          PageBellLen;
    byte           Use_Xmodem;
    byte           Use_Xmodem1k;
    byte           Use_Ymodem;
    byte           Use_YmodemG;
    byte           Use_Sealink;
    byte           Use_Zmodem;
    byte           Inp_Fields;
    char           QuoteStr[4];
    short          UploadCredit;
    char           LoadingMessage[71];
    char           SelectionPrompt[71];
    word           VersionID;
    char           Resp4800[41];
    char           Resp9600[41];
    short          OldAkaZone[5]; // not used
    short          MatrixPoint;
    short          OldAkaPoint[5]; // not used
    byte           UseAka[200];
    byte           AskAge; // Not used
    char           SystemName[41];
    long           RegKey;
    byte           EntryfieldColor;
    byte           MenuBorderColor;
    short          SysOpSeclvl;
    byte           AllowDelMsgs;
    EventRecordT   EventRec[30];
    BoardRecordT   UnusedBoards[200];
} ConfigRecordT, * ConfigRecordP, ** ConfigRecordI;
//  ------------------------------------------------------------------
//  Structure of BOARDS.BBS
typedef struct
{
    char       Name[31];
    char       QwkName[13];
    byte       Typ;     // 0=Standard 1=Net 3=Echo
    byte       Kinds;   // 0=Private & Public 1=Private 2=Public 3=Read-Only
    byte       Aliases; // 0=no aliases, 1=yes, 2=ask alias, 3=Alias/name
    short      ReadSecLvl;
    SbFlagType ReadFlags;
    short      WriteSecLvl;
    SbFlagType WriteFlags;
    short      SysopSecLvl;
    SbFlagType SysopFlags;
    char       Group;
    byte       Replystatus; // 0 = normal, 1 = net/normal, 2 = net only, 3 = no replies
    byte       Age;
    byte       Attrib;
    byte       UseAka;
} MsgBoardRecordT, * MsgBoardRecordP, ** MsgBoardRecordI;
//  Attributes ;
//
//  bit 0 ; Combined
//  bit 1 ; Default combined
//  bit 2 ; Allow message deleting
//  bit 3 ; Allow taglines
//  bit 4 ; Use in template system
//  bit 5 ; Convert 8bit -> 7bit
//  bit 6 ; forced ON in the mail check
//  ------------------------------------------------------------------

#if defined (GOLD_CANPACK)
#pragma pack()
#endif
//  ------------------------------------------------------------------
