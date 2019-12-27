
//  ------------------------------------------------------------------
//  GoldED+
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999-2000 Alexander S. Aganichev
//  ------------------------------------------------------------------
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License as
//  published by the Free Software Foundation; either version 2 of the
//  License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston,
//  MA 02111-1307 USA
//  ------------------------------------------------------------------
//  $Id$
//  ------------------------------------------------------------------
//  Language pointer structure.
//  ------------------------------------------------------------------
#ifndef __GELANG_H
#define __GELANG_H
//  ------------------------------------------------------------------
//  Language pointer structure

struct LangGed
{

    char*  SWSun;              // MS_SWSUN
    char*  SWMon;              // MS_SWMON
    char*  SWTue;              // MS_SWTUE
    char*  SWWed;              // MS_SWWED
    char*  SWThu;              // MS_SWTHU
    char*  SWFri;              // MS_SWFRI
    char*  SWSat;              // MS_SWSAT

    char*  LWSunday;           // MS_LWSUNDAY
    char*  LWMonday;           // MS_LWMONDAY
    char*  LWTuesday;          // MS_LWTUESDAY
    char*  LWWednesday;        // MS_LWWEDNESDAY
    char*  LWThursday;         // MS_LWTHURSDAY
    char*  LWFriday;           // MS_LWFRIDAY
    char*  LWSaturday;         // MS_LWSATURDAY

    char*  SMJan;              // MS_SMJAN
    char*  SMFeb;              // MS_SMFEB
    char*  SMMar;              // MS_SMMAR
    char*  SMApr;              // MS_SMAPR
    char*  SMMay;              // MS_SMMAY
    char*  SMJun;              // MS_SMJUN
    char*  SMJul;              // MS_SMJUL
    char*  SMAug;              // MS_SMAUG
    char*  SMSep;              // MS_SMSEP
    char*  SMOct;              // MS_SMOCT
    char*  SMNov;              // MS_SMNOV
    char*  SMDec;              // MS_SMDEC

    char*  LMJanuary;          // MS_LMJANUARY
    char*  LMFebruary;         // MS_LMFEBRUARY
    char*  LMMarch;            // MS_LMMARCH
    char*  LMApril;            // MS_LMAPRIL
    char*  LMMay;              // MS_LMMAY
    char*  LMJune;             // MS_LMJUNE
    char*  LMJuly;             // MS_LMJULY
    char*  LMAugust;           // MS_LMAUGUST
    char*  LMSeptember;        // MS_LMSEPTEMBER
    char*  LMOctober;          // MS_LMOCTOBER
    char*  LMNovember;         // MS_LMNOVEMBER
    char*  LMDecember;         // MS_LMDECEMBER

    char*  Uptime;             // MS_UPTIME

    char*  ImportFile;         // ST_IMPORTFILE
    char*  ImportWhichFile;    // WT_IMPORTWHICHFILE
    char*  ImportTitle;        // WT_IMPORTPICK
    char*  ImportTxt;          // WT_IMPORTTXT
    char*  ImportTxtText;      // MI_IMPORTTXTTEXT
    char*  ImportTxtQuote;     // MI_IMPORTTXTQUOTE
    char*  ImportTxtUue;       // MI_IMPORTTXTUUE
    char*  ImportTxtMime;      // MI_IMPORTTXTMIME
    char*  ImportTxtClip;      // MI_IMPORTTXTCLIP
    char*  ImportTxtXlat;      // MI_IMPORTTXTXLAT
    char*  ImportTxtQuit;      // MI_IMPORTTXTQUIT
    char*  ImportStatus;       // ST_IMPORTSTATUS
    char*  ExportFile;         // ST_EXPORTFILE
    char*  ExportWhatFile;     // WT_EXPORTWHATFILE
    char*  ExportStatus;       // ST_EXPORTSTATUS
    char*  EditStatus;         // ST_EDITSTATUS
    char*  SelectFiles;        // ST_SELECTFILES
    char*  SelectedFiles;      // WL_SELECTEDFILES
    char*  SelectedBytes;      // WL_SELECTEDBYTES
    char*  TotalFiles;         // WL_TOTALFILES
    char*  TotalBytes;         // WL_TOTALBYTES
    char*  ScanningDirectory;  // WL_SCANNINGDIRECTORY
    char*  NoFilesFound;       // WL_NOFILESFOUND
    char*  FilesPressKey;      // ST_FILESPRESSKEY
    char*  Area;               // WT_AREA
    char*  Description;        // WT_DESCRIPTION
    char*  Msgs;               // WT_MSGS
    char*  Last;               // WT_LAST
    char*  EchoID;             // WT_ECHOID
    char*  unread;             // ST_UNREAD
    char*  Scan_Areas;         // WT_SCANAREAS
    char*  Scan_All;           // MI_SCANALL
    char*  Scan_Marked;        // MI_SCANMARKED
    char*  Scan_Current;       // MI_SCANCURRENT
    char*  Scan_Matching;      // MI_SCANMATCHING
    char*  Scan_Unscanned;     // MI_SCANUNSCANNED
    char*  No_Scan;            // MI_NOSCAN
    char*  Heat_Areas;         // WT_HEATAREAS
    char*  Heat_All;           // MI_HEATALL
    char*  Heat_Marked;        // MI_HEATMARKED
    char*  Heat_Current;       // MI_HEATCURRENT
    char*  No_Heat;            // MI_NOHEAT
    char*  Zap_Areas;          // WT_ZAPAREAS
    char*  Zap_All;            // MI_ZAPALL
    char*  Zap_Marked;         // MI_ZAPMARKED
    char*  Zap_Current;        // MI_ZAPCURRENT
    char*  No_Zap;             // MI_NOZAP
    char*  DOS_Shell;          // MS_DOS_SHELL
    char*  ScanningArea;       // IL_SCANNINGAREA
    char*  SearchingFor;       // IL_SEARCHINGFOR
    char*  ReadingMsg;         // ST_READINGMSG
    char*  Outofmem;           // ER_OUTOFMEM
    char*  Hexdumphead;        // MS_HEXDUMPHEAD
    char*  Hexdumptext;        // MS_HEXDUMPTEXT
    char*  Renumbering;        // ST_RENUMBERING
    char*  Locked;             // ST_LOCKED
    char*  Renumbered;         // ST_RENUMBERED
    char*  BlankMsg;           // WL_BLANKMSG
    char*  Wait;               // WL_WAIT
    char*  GenHexdump;         // IL_GENHEXDUMP
    char*  ProcessCC;          // ST_PROCESSCC
    char*  StatusCC;           // ST_STATUSCC
    char*  DestinationCC;      // ST_DESTINATIONCC
    char*  ListCC;             // MS_LISTCC
    char*  DelOrig;            // WT_DELORIG
    char*  DelOrigYes;         // MI_DELORIGYES
    char*  DelOrigNo;          // MI_DELORIGNO
    char*  DropMsg;            // WT_DROPMSG
    char*  DropMsgYes;         // MI_DROPMSGYES
    char*  DropMsgNo;          // MI_DROPMSGNO
    char*  ZoneGate;           // WT_ZONEGATE
    char*  ZoneGateYes;        // MI_ZONEGATEYES
    char*  ZoneGateNo;         // MI_ZONEGATENO
    char*  Quotepct;           // ST_QUOTEPCT
    char*  SaveMsg;            // WT_SAVEMSG
    char*  YesGreat;           // MI_YESGREAT
    char*  Kickit;             // MI_KICKIT
    char*  Continue;           // MI_CONTINUE
    char*  Rot13;              // MI_ROT13
    char*  AttribS;            // MI_ATTRS
    char*  OriginS;            // MI_ORIGS
    char*  View;               // MI_VIEW
    char*  SelectDestNode;     // ST_SELECTDESTNODE
    char*  AttachFiles;        // WT_ATTACHFILES
    char*  UpdreqFiles;        // WT_UPDREQFILES
    char*  EditHeader;         // ST_EDITHEADER
    char*  MsgOptions;         // WT_EDITING
    char*  InternalEd;         // MI_INTERNALED
    char*  ExternalEd;         // MI_EXTERNALED
    char*  SaveNoEdit;         // MI_SAVEMESSAGE
    char*  AttrO;              // MI_ATTRO
    char*  Template;           // MI_TEMPLATE
    char*  Origin;             // MI_ORIGIN
    char*  QuitMessage;        // MI_QUITMESSAGE
    char*  Templates;          // ST_TEMPLATES
    char*  ChangeTemplate;     // WT_CHANGETEMPLATES
    char*  CarbonCopy;         // WT_CARBONCOPY
    char*  CCProcess;          // MI_CCPROCESS
    char*  CCIgnore;           // MI_CCIGNORE
    char*  CCAttribs;          // MI_CCATTRIBS
    char*  CCListFmt;          // MI_CCLISTFMT
    char*  CCList;             // WT_CCLIST
    char*  CCListKeep;         // MI_CCLISTKEEP
    char*  CCListNames;        // MI_CCLISTNAMES
    char*  CCListVisible;      // MI_CCLISTVISIBLE
    char*  CCListHidden;       // MI_CCLISTHIDDEN
    char*  CCListRemove;       // MI_CCLISTREMOVE
    char*  AttrTitle;          // WT_ATTRTITLE
    char*  AttrPvt;            // MI_ATTR01
    char*  AttrRcv;            // MI_ATTR02
    char*  AttrSnt;            // MI_ATTR03
    char*  AttrCrs;            // MI_ATTR04
    char*  AttrHld;            // MI_ATTR05
    char*  AttrFil;            // MI_ATTR06
    char*  AttrFrq;            // MI_ATTR07
    char*  AttrUpd;            // MI_ATTR08
    char*  AttrKS;             // MI_ATTR09
    char*  AttrKfs;            // MI_ATTR10
    char*  AttrTfs;            // MI_ATTR11
    char*  AttrDir;            // MI_ATTR12
    char*  AttrImm;            // MI_ATTR13
    char*  AttrLok;            // MI_ATTR14
    char*  AttrZap;            // MI_ATTR15
    char*  From;               // HD_FROM
    char*  To;                 // HD_TO
    char*  Subj;               // HD_SUBJ
    char*  File;               // HD_FILE
    char*  EmptyMsg;           // MS_EMPTYMSG
    char*  AutoAttachMsg;      // MS_AUTOATTACHMSG
    char*  AutoRequestMsg;     // MS_AUTOREQUESTMSG
    char*  AutoUpdreqMsg;      // MS_AUTOUPDREQMSG
    char*  File_Attach;        // WT_FILEATTACH
    char*  File_Request;       // WT_FILEREQUEST
    char*  File_Updreq;        // WT_FILEUPDREQ
    char*  FileAttaching;      // ST_FILEATTACHING
    char*  FileRequesting;     // ST_FILEREQUESTING
    char*  FileUpdreqing;      // ST_FILEUPDREQING
    char*  ReadMarked;         // ST_READMARKED
    char*  ReadAll;            // ST_READALL
    char*  NoRenum;            // ST_NOQRENUM
    char*  HidingTwit;         // MS_HIDINGTWIT
    char*  Via;                // HD_VIA
    char*  ChangeAttrs;        // WT_CHANGEATTRS
    char*  HeaderText;         // WT_HEADERTEXT
    char*  HeaderOnly;         // WT_HEADERONLY
    char*  NewArea;            // WT_NEWAREA
    char*  ReplyArea;          // WT_REPLYAREA
    char*  CopyArea;           // WT_COPYAREA
    char*  MoveArea;           // WT_MOVEAREA
    char*  ForwardArea;        // WT_FORWARDAREA
    char*  FreqArea;           // WT_FREQAREA
    char*  FreqMenuTitle;      // WT_FREQMENUTITLE
    char*  FreqStat;           // ST_FREQSTAT
    char*  FreqInfoNoFiles;    // IL_FREQINFONOFILES
    char*  Copy;               // WT_COPY
    char*  Move;               // WT_MOVE
    char*  Copying;            // WT_COPYING
    char*  Moving;             // WT_MOVING
    char*  CopyingMsg;         // ST_COPYINGMSG
    char*  MovingMsg;          // ST_MOVINGMSG
    char*  Delete;             // WT_DELETE
    char*  Deleting;           // WT_DELETING
    char*  DeletingMsg;        // ST_DELETINGMSG
    char*  Write;              // WT_WRITE
    char*  WriteMsgs;          // WT_WRITEMSGS
    char*  Writing;            // WT_WRITING
    char*  WritingMsg;         // ST_WRITINGMSG
    char*  WritingFile;        // WT_WRITINGFILE
    char*  WritingPRN;         // WT_WRITINGPRN
    char*  ReadOnlyWarn;       // IL_READONLYWARN
    char*  IsReadOnly;         // WT_ISREADONLY
    char*  ReadOnlyYes;        // MI_READONLYYES
    char*  ReadOnlyNo;         // MI_READONLYNO
    char*  ChangeWarn;         // IL_CHANGEWARN
    char*  Change;             // WT_CHANGE
    char*  ChangeYes;          // MI_CHANGEYES
    char*  ChangeNo;           // MI_CHANGENO
    char*  DeleteThis;         // WT_DELETETHIS
    char*  DeleteYes;          // MI_DELETEYES
    char*  DeleteNo;           // MI_DELETENO
    char*  DeleteNoAsk;        // MI_DELETENOASK
    char*  GotoNext;           // WT_GOTONEXT
    char*  GotoNextYes;        // MI_GOTONEXTYES
    char*  GotoNextNo;         // MI_GOTONEXTNO
    char*  GotoNextNew;        // MI_GOTONEXTNEW
    char*  Forward;            // WT_FORWARD
    char*  ForwardYes;         // MI_FORWARDYES
    char*  ForwardNo;          // MI_FORWARDNO
    char*  Msg;                // WT_MSG
    char*  MsgReal;            // WT_MSGREAL
    char*  FromL;              // WT_FROML
    char*  ToL;                // WT_TOL
    char*  SubjL;              // WT_SUBJL
    char*  MsgLister;          // ST_MSGLISTER
    char*  CopyMoveForward;    // ST_COPYMOVEFORWARD
    char*  SelectAction;       // WT_SELECTACTION
    char*  ForwardMessage;     // MI_FORWARDMESSAGE
    char*  MoveMessage;        // MI_MOVEMESSAGE
    char*  CopyMessage;        // MI_COPYMESSAGE
    char*  ToggleSent;         // MI_TOGGLESENT
    char*  QuitCMF;            // MI_QUITCMF
    char*  are;                // ST_ARE
    char*  is;                 // ST_IS
    char*  marked;             // ST_MARKED
    char*  MarkedMsg;          // MI_MARKEDMSG
    char*  CurrentMsg;         // MI_CURRENTMSG
    char*  QuitMsgs;           // MI_QUITMSGS
    char*  WriteMsgsTo;        // ST_WRITEMSGSTO
    char*  WriteTo;            // WT_WRITETO
    char*  Diskfile;           // MI_DISKFILE
    char*  Printer;            // MI_PRINTER
    char*  QuitWrite;          // MI_QUITWRITE
    char*  MarkingOptions;     // ST_MARKINGOPTIONS
    char*  MarkWhat;           // WT_MARKWHAT
    char*  Yourmail;           // MI_YOURMAIL
    char*  FromToSubj;         // MI_FROMTOSUBJ
    char*  TextHdr;            // MI_TEXTHDR
    char*  Thread;             // MI_THREAD
    char*  AsRead;             // MI_ASREAD
    char*  NewMsgs;            // MI_NEWMSGS
    char*  OldMsgs;            // MI_OLDMSGS
    char*  AllMsgs;            // MI_ALLMSGS
    char*  Unmark;             // MI_UNMARK
    char*  Range;              // MI_RANGE
    char*  Markstoggle;        // MI_MARKSTOGGLE
    char*  QuitMarks;          // MI_QUITMARKS
    char*  EnterMarkString;    // WT_ENTERMARKSTRING
    char*  SearchingMsg;       // ST_SEARCHINGMSG
    char*  UserlistName;       // WT_USERLISTNAME
    char*  GenUserlist;        // IL_GENUSERLIST
    char*  FileExists;         // WT_FILEEXISTS
    char*  Append;             // MI_APPEND
    char*  OverWrite;          // MI_OVERWRITE
    char*  QuitExist;          // MI_QUITEXIST
    char*  WarnUnsent;         // IL_WARNUNSENT
    char*  WarnLocked;         // IL_WARNLOCKED
    char*  ChangeOrigin;       // ST_CHANGEORIGIN
    char*  Origins;            // WT_ORIGINS
    char*  ChangeUsername;     // ST_CHANGEUSERNAME
    char*  Usernames;          // WT_USERNAMES
    char*  ChangeAka;          // ST_CHANGEAKA
    char*  Akas;               // WT_AKAS
    char*  Lookup;             // WT_LOOKUP
    char*  Phone;              // WL_PHONE
    char*  QuitGoldED;         // WT_QUITGOLDED
    char*  QuitYes;            // MI_QUITYES
    char*  QuitNo;             // MI_QUITNO
    char*  EditCmd;            // MS_EDITCMD
    char*  NoOrigDefined;      // IL_NOORIGDEFINED
    char*  NoUserDefined;      // IL_NOUSERDEFINED
    char*  NoAkaDefined;       // IL_NOAKADEFINED
    char*  NoTplDefined;       // IL_NOTPLDEFINED
    char*  NoThreadlist;       // IL_NOTHREADLIST
    char*  SkippingTwit;       // MS_SKIPPINGTWIT
    char*  SkippingDeleted;    // MS_SKIPPINGDELETED
    char*  KillingTwit;        // MS_KILLINGTWIT
    char*  WritingCfg;         // IL_WRITINGCFG
    char*  CouldNotOpen;       // IL_COULDNOTOPEN
    char*  Prompt;             // MS_PROMPT
    char*  UnfinishedMsg;      // IL_UNFINISHEDMSG
    char*  LoadUnfinished;     // ST_LOADUNFINISHED
    char*  ReadingEcholist;    // IL_READINGECHOLIST
    char*  ReadingAddrMacros;  // IL_READINGADDRMACROS
    char*  CheckingNodelists;  // IL_CHECKINGNODELISTS
    char*  Crossposting;       // ST_CROSSPOSTING
    char*  TwitBlanked;        // IL_TWITBLANKED
    char*  TwitSkipped;        // IL_TWITSKIPPED
    char*  TwitIgnoreSkip;     // IL_TWITIGNORESKIP
    char*  TwitDisplayed;      // IL_TWITDISPLAYED
    char*  TwitKilled;         // IL_TWITKILLED
    char*  StylecodesNo;       // IL_STYLECODESNO
    char*  StylecodesYes;      // IL_STYLECODESYES
    char*  StylecodesHide;     // IL_STYLECODESHIDE
    char*  GenCfmReceipt;      // ST_GENCFMRECEIPT
    char*  New;                // WT_NEW
    char*  LookupInfo;         // ST_LOOKUPINFO
    char*  DateTimeFmt;        // MS_DATETIMEFMT
    char*  DateFmt;            // MS_DATEFMT
    char*  TimeFmt;            // MS_TIMEFMT
    char*  Originallyin;       // MS_ORIGINALLYIN
    char*  Crosspostedin;      // MS_CROSSPOSTEDIN
    char*  CCTo;               // MS_CCTO
    char*  StatusLineHelp;     // ST_STATUSLINEHELP
    char*  of;                 // HD_OF
    char*  RobotMsg;           // MS_ROBOTMSG
    char*  StatusLineTimeFmt;  // ST_STATUSLINETIMEFMT

    char*  Replies;            // WT_REPLIES
    char*  SelectReply;        // ST_SELECTREPLY
    char*  WaitOrExit;         // IL_WAITOREXIT
    char*  ReallyExit;         // WT_REALLYEXIT
    char*  TaglineS;           // MI_TAGLINES
    char*  HeaderEdit;         // MI_HEADEREDIT
    char*  ScanGroup;          // MI_SCANGROUP
    char*  QWKPacket;          // WT_QWKPACKET
    char*  ScanQWK;            // MI_SCANQWK
    char*  SoupPacket;         // WT_SOUPPACKET
    char*  ScanSoup;           // MI_SCANSOUP
    char*  ScanImport;         // MI_SCANIMPORT
    char*  ScanExport;         // MI_SCANEXPORT
    char*  ScanPM;             // WT_SCANPM
    char*  Tagline;            // MI_TAGLINE
    char*  Confirm;            // WT_CONFIRM
    char*  ConfirmYes;         // MI_CONFIRMYES
    char*  ConfirmNo;          // MI_CONFIRMNO
    char*  ConfirmInfo;        // IL_CONFIRMINFO
    char*  msg;                // ST_MSG
    char*  msgs;               // ST_MSGS
    char*  personal;           // ST_PERSONAL
    char*  Grp;                // WT_GRP
    char*  FoundPersonal;      // IL_FOUNDPERSONAL
    char*  NoPersonal;         // IL_NOPERSONAL
    char*  EscOrContinue;      // ST_ESCORCONTINUE
    char*  SpellChecker;       // MS_SPELLCHECKER
    char*  Ins;                // WT_INS
    char*  DrawSL;             // WT_DRAWSL
    char*  DrawDL;             // WT_DRAWDL
    char*  FilelistFrom;       // MI_FILELISTFROM
    char*  Initializing;       // ST_INITIALIZING
    char*  Checking;           // ST_CHECKING
    char*  LockShareCap;       // ST_LOCKSHARECAP
    char*  AttrTurnOff;        // WT_ATTRTURNOFF
    char*  Taglines;           // WT_TAGLINES
    char*  ChangeTagline;      // ST_CHANGETAGLINE
    char*  NoTagline;          // IL_NOTAGLINE
    char*  Charsets;           // WT_CHARSETS
    char*  ChangeXlatImp;      // ST_CHANGEXLATIMP
    char*  CharsetAuto;        // MI_CHARSETAUTO
    char*  NoXlatImport;       // IL_NOXLATIMPORT
    char*  n_a;                // WT_N_A
    char*  Written;            // WT_WRITTEN
    char*  Arrived;            // WT_ARRIVED
    char*  Received;           // WT_RECEIVED
    char*  NoNodelist;         // IL_NONODELIST
    char*  NodelistMissing;    // IL_NODELISTMISSING
    char*  NodelistOutdated;   // IL_NODELISTOUTDATED
    char*  Replylinker;        // MS_REPLYLINKER
    char*  EnterMsgno;         // WT_ENTERMSGNO
    char*  WaitUUdecoding;     // IL_WAITUUDECODING
    char*  CompletedUUdecode;  // IL_COMPLETEDUUDECODE
    char*  NotUUencoded;       // IL_NOTUUENCODED
    char*  UUEinvalidpath;     // IL_UUEINVALIDPATH
    char*  TMPinvalidpath;     // IL_TMPINVALIDPATH
    char*  Pathreport;         // IL_PATHREPORT
    char*  ErrorInSoup;        // IL_ERRORINSOUP
    char*  WarnAlreadySent;    // IL_WARNALREADYSENT
    char*  WaitLocked;         // IL_WAITLOCKED
    char*  RetryOrESC;         // ST_RETRYORESC
    char*  RetryLock;          // ST_RETRYLOCK
    char*  RetryOpen;          // ST_RETRYOPEN
    char*  TouchSemaphore;     // WT_TOUCHSEMAPHORE
    char*  WriteToHdrNO;       // MI_WRITETOHDRNO
    char*  WriteToHdrYES;      // MI_WRITETIHDRYES
    char*  WriteToHdrONLY;     // MI_WRITETIHDRONLY
    char*  Clipboard;          // MI_CLIPBOARD
    char*  SelectMarks;        // WT_SELECTMARKS
    char*  SelectMarksEdit;    // WT_SELECTMARKSEDIT
    char*  DropMarksInfo;      // IL_DROPMARKSINFO
    char*  DropMarks;          // WT_DROPMARKS
    char*  DropAll;            // MI_DROPALL
    char*  DropMarked;         // MI_DROPMARKED
    char*  DropCurrent;        // MI_DROPCURRENT
    char*  NoDrop;             // MI_NODROP
    char*  CatchAreas;         // WT_CATCHAREAS
    char*  CatchAll;           // MI_CATCHALL
    char*  CatchMarked;        // MI_CATCHMARKED
    char*  CatchCurrent;       // MI_CATCHCURRENT
    char*  NoCatch;            // MI_NOCATCH
    char*  Crosspost;          // WT_CROSSPOST
    char*  XCProcess;          // MI_XCPROCESS
    char*  XCIgnore;           // MI_XCIGNORE
    char*  XCListFmt;          // MI_XCLISTFMT
    char*  XCList;             // WT_XCLIST
    char*  XCListKeep;         // MI_XCLISTRAW
    char*  XCListNames;        // MI_XCLISTVERBOSE
    char*  XCListVisible;      // MI_XCLISTLINE
    char*  XCListRemove;       // MI_XCLISTREMOVE

    char*  Addressbook;        // WT_ADDRESSBOOK
    char*  AdvancedSearch;     // WT_ADVANCEDSEARCH
    char*  NoMoreMatches;      // IL_NOMOREMATCHES
    char*  HeaderEditHelp1;    // WT_HEADEREDITHELP_1
    char*  HeaderEditHelp2;    // WT_HEADEREDITHELP_2
    char*  ThreadlistTitle;    // WT_THREADLISTTITLE
    char*  AdvancedMarking;    // WT_ADVANCEDMARKING
    char*  UserStatusline;     // ST_USERSTATUSLINE
    char*  UserHeaderName;     // WT_USERHEADERNAME
    char*  UserHeaderOrg;      // WT_USERHEADERORG
    char*  UserHeaderAka;      // WT_USERHEADERAKA
    char*  UserWait;           // IL_USERWAIT

    char*  ArealistSelections1;// MI_ALSELECTIONS1
    char*  ArealistSelections2;// MI_ALSELECTIONS2
    char*  Decode;             // IL_DECODE
    char*  Preparing;          // MI_PREPARING

    char*  ExternUtil;         // WT_EXTERNUTIL
    char*  SelectExternUtil;   // ST_EXTERNUTIL
    char*  ExecCmdLine;        // WT_EXECCMDLINE
    char*  EnterCmdLine;       // MI_ENTERCMDLINE

    char*  PeekURLMenuTitle;   // WT_PEEKURL
    char*  PeekURLStat;        // ST_PEEKURL
    char*  PeekInfoNoURLs;     // IL_PEEKINFONOURLS
    char*  PeekInfoNoHandler;  // IL_PEEKINFONOHANDLER

    char*  MMMain;             // MI_MMMAIN
    char*  MMShell;            // MI_MMSHELL
    char*  MMQuit;             // MI_MMQUIT
    char*  MMEdit;             // MI_MMEDIT
    char*  MMNew;              // MI_MMNEWMSG
    char*  MMQuote;            // MI_MMQUOTEREPLY
    char*  MMDirQuote;         // MI_MMDIRQUOTEREPLY
    char*  MMComment;          // MI_MMCMTREPLY
    char*  MMOtherAreaReply;   // MI_MMOTHERAREA
    char*  MMOtherArea;        // WT_MMOTHERAREA
    char*  MMOQuote;           // MI_MMOQUOTEREPLY
    char*  MMODirQuote;        // MI_MMODIRQUOTEREPLY
    char*  MMOComment;         // MI_MMOCMTREPLY
    char*  MMChange;           // MI_MMCHANGE
    char*  MMOrigin;           // MI_MMORIGIN
    char*  MMUsername;         // MI_MMUSERNAME
    char*  MMAka;              // MI_MMAKA
    char*  MMAttrs;            // MI_MMATTRIBUTES
    char*  MMTemplate;         // MI_MMTEMPLATE
    char*  MMUtil;             // MI_MMUTIL
    char*  MMCopyMoveForward;  // MI_MMCOPYMOVEFORWARD
    char*  MMList;             // MI_MMLIST
    char*  MMWrite;            // MI_MMWRITE
    char*  MMFreq;             // MI_MMFREQ

    char*  EndLanguage;        // LAST_CRC

};


//  ------------------------------------------------------------------

#endif
