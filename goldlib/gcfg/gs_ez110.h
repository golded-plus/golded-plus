//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1992-1994 Peter Davies
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

#ifndef __gs_ez110_h
#define __gs_ez110_h


//  ------------------------------------------------------------------

#include <gftnall.h>


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif


//  ------------------------------------------------------------------
//  EzyCom structures converted to C format V1.10 6/3/94
//  by Peter Davies and Ron Clark
//
//  These structures may ONLY be used in accordance with those
//  agreements laid out in the Official Ezycom Structures.
//  Peter Davies or Ron Clark takes no responsibility whatsoever
//  for the correctness of these structures.

#define EZYVER                "1.10"
#define VERSIONHIGH           1
#define VERSIONLOW            10
#define MAXFREE               1238
#define USERFREE              125
#define CONSTANTFREESPACE     89
#define CONSTANTFILEFREESPACE 537
#define MAXNODES              128
#define MAXAKA                32
#define MAXMESS               1536
#define MAXMESSALL            1536
#define MAXBAUDREC            11

typedef byte AskType;          // 0=yes,1=no,2=ask default yes,3=ask default no
typedef byte MsgType;          // 0=local,1=net,2=echo,3=passthru,4=waitthru,5=allmail
typedef byte MsgKindsType;     // 0=public,1=private,2=both
typedef byte ShowFileSizeType; // 0=nosize,1=bytes,2=kilobytes
typedef char UserString[36];
typedef byte Str8[9];
typedef byte EzFlagType[4];

typedef struct SECURITYTYPE {

  word       security;
  EzFlagType onflags,
             offflags;
} SECURITYTYPE;


//  ------------------------------------------------------------------
//  CONFIG.EZY Configuration Record
//
//  printerport =
//                0 : LPT1
//                1 : LPT2
//                2 : LPT3
//                3 : COM1
//                4 : COM2
//                5 : COM3
//                6 : COM4
//
//  autodetect = bit
//                0 : Auto Detect ANSI
//                1 : ANSI Detect for NewUser
//                2 : Auto Detect IEMSI
//                3 : IEMSI Detect for NewUser
//                4 : Auto Detect RIP
//              5-7 : [Reserved]
//
//  configattr = bit
//                0 : Move Local Uploads
//             1-15 : [Reserved]
//
//  multitasker =
//                0 = Do Not Detect or Use Any MultiTasker
//                1 = Auto-Detect
//                2 = Desqview
//                3 = Double DOS
//                4 = OS/2
//                5 = MultiDOS Plus
//                6 = Taskview
//                7 = Topview
//                8 = Windows Enhanced Mode
//           9..255 = [Reserved]
//
//  shownewfileschar
//                0 - No
//                1 - ASCII Only
//                2 - Always

typedef struct CONFIGRECORD {
  Str8         version,
               deflanguage;
  byte         nocarrierstring[21];
  UserString   guestaccount;
  char         freespace01[11],        // unused
               logpath[61],
               textpath[61],
               menupath[61],
               mnurampath[61],
               netmailpath[61],
               nodelistpath[61],
               msgpath[61],
               filepath[61],
               freespace02[61],        // unused
               binkleyoutpath[61],
               temppath[61],
               userbasepath[61],
               avatarpath[61],
               ascpath[61],
               asclowpath[61],
               filemaint[61],
               fileattachpath[61],
               soundpath[61],
               fastindexpath[61],
               systempwd[16],          // Password to Logon System
               sysoppwd[16],           // Password to Keyboard
               newuserpwd[16];         // Password for Newuser
  Str8         newtopmenu;             // NewUser TopMenu
  byte         newusergroups[4],
               inboundmail[61],
               outboundmail[61],
               uploadpath[61],
               swapfile[61],
               multipath[61],
               brackets[3],
               inactivitytime,
               minmesscheck,
               maxlogintime,
               freespace03,            // unused
               answerdelay,            // BOOLEAN
               shellswap,              //    "
               highbit,                //    "
               disppass,               //    "
               asklocalpass,           //    "
               fastlogon,              //    "
               sysopremote,            //    "
               printerlog,             //    "
               phone1ask,              //    "
               colourask,              //    "
               aliasask,               //    "
               dobask,                 //    "
               phoneforce,             //    "
               direct_video,           //    "
               snow_check,             //    "
               freespace04,            // unused
               screen_blank,
               oneword;                // BOOLEAN
  AskType      checkmail,
               checkfile,
               ansiask,
               fullscreenask,
               clearask,
               moreask,
               avatarask,
               extendask,
               usdateask;
  byte         phone2ask,              // BOOLEAN
               phoneformat[15],
               freespace04a[61],
               freespace04b[61],
               shellprompt[61],
               shell2prompt[61],
               enterprompt[61],
               chatprompt[61],
               listprompt[61],
               f7keylinetop[80],
               f7keylinebot[80],
               freespace05[84],        // unused
               chat2prompt[61],
               freespace05a[61],
               freespace05b[61],
               freespace05c[61],
               freespace05d[61],
               loadprompt[61],
               freespace05e[61],
               freespace05f[61];
  word         security,
               logonsecurity;
  EzFlagType   flags;
  byte         minpasslength,
               dispfwind,              //  Status Bar Colour  constant
               dispbwind,              //  Status Bar Colour     "
               disppopupf,             //  Popup Forground       "
               disppopupborder,        //  Popup Border          "
               disppopupb,             //  Popup Background      "
               dispf,                  //  Foreground Colour     "
               freespace06,            // Unused
               comport,
               passlogons,
               doblogon,
               printerport,
               passtries,
               topmenu[9],
               freespace07[4];         // Unused
  word         watchmess,              // constant
               netmailcredit;          // constant
  long         ansiminbaud;
  word         slowbaud,
               minloginbaud,
               lowsecuritystart,
               lowsecurityend,
               slowstart,
               slowend;
  byte         quotestring[6],
               freespace09,            // Unused
               offhook;                // BOOLEAN
  word         forcecrashmail,
               optioncrashmail,
               netmailfileattach;
  byte         popuphighlight,         // Popup Highlight Colour (constant)
               freespace10,            // Unused
               maxpages,
               maxpagefiles,
               pagelength;
  word         pagestart[7],
               freespace50,            // Unused
               localfattachsec,
               sectouploadmess,
               sectoupdateusers,
               readsecnewecho,
               writesecnewecho,
               sysopsecnewecho,
               secreplyvianetmail;
  AskType      netmailkillsent;
  byte         swaponarchive,
               freespace11[9],         // Unused
               popuptext;              // Popup Text Colour (constant)
  word         pageend[7];
  byte         freespace12[22];        // Unused
  word         incomingcallstart,
               incomingcallend,
               fp_upload;              // File Points Upload Credit
  byte         altf[10][61],
               ctrlf[10][41],
               freespace13[4];
  word         fp_credit;              // Newuser Filepoints
  byte         ks_per_fp,              // Number of Kilobytes per FP
               freespace14,            // unused
               rego_warn_1,
               rego_warn_2,
               freespace15[2];         // Unused
  word         min_space_1;            // constant
  byte         swapbimodem,            // boolean
               modembusy,              // boolean  (* Toggle DTR or ATH1 *)
               scrheight,              // boolean  (* 43/50 line mode *)
               msgtmptype,             // boolean  (True=MSGTMP False=MSGTMP.<node>)
               swapupload,             // boolean
               phonelogon,
               carrierdetect,          // (* Carrier Detect (Default=$80) *)
               newfileshighlight,      // boolean;
               max_descrip,
               min_descrip;
  word         requestreceipt;
  byte         ushowdate;              // boolean
  ShowFileSizeType ufilesizek;
  byte         uuploader,              // boolean
               udownloadcount,         // boolean
               freespace16,            // unused
               ushowsecurity,          // boolean
               sshowdate;              // boolean
  ShowFileSizeType sfilesizek;
  byte         suploader,              // boolean
               sdownloadcount,         // boolean
               freespace17,            // Unused
               sshowsecurity,          // boolean
               ushowtime,              // boolean
               ushowfp,                // boolean
               sshowtime,              // boolean
               sshowfp;                // boolean
  word         fp_percent;             // Download Filepoints Credit
  byte         autodetect,
               dispsecurityfile,       // boolean
               askforpagereason,       // boolean
               delincompletefiles,     // boolean
               freespace18,            // unused
               swaponfeditview,        // constant
               freespace19,            // unused
               secfileschar,
               passchar;
  byte         localinactivity,        // boolean
               freespace20,            // unused
               leftbracket[2],
               rightbracket[2];
  word         ignorefp;               // Min Security to Ignore FPs
  byte         menuminage,             // Minimum Age for Age Checks
               freespace21;            // unused
  word         modemeff[MAXBAUDREC],
               modembaud[MAXBAUDREC];
  byte         modemconnect[MAXBAUDREC][16],
               freespace22[10];        // unused
  word         configattr;
  byte         usercol1_2,
               usercol3_4,
               usercol5_6,
               usercol7_8,
               userbkcol,
               newusercol2,
               chstatcol,
               getentercol,
               usdateforsysop,         // boolean
               ezyovrpath[61],
               freespace23[36],
               ovrems,                 // boolean;
               swapezy,
               filesecpath[61],
               freespace24,            // unused
               multitasker;
  word         oldmaxbaud;
  byte         lockedport;
  word         filereqsec;
  byte         autoanswer,             //  boolean
               initresponse[11],
               ringstring[11],
               inittries,
               initstring1[61],
               initstring2[61],
               busystring[21],
               answerstring[21],
               mailerstring[61];
  word         modemstart,
               modemend;
  byte         modemdelay,
               sendbreak,              // boolean;
               externaleditor[61];
  char         defaultorigin[51],
               connectfax[16];
  long         maxbaud;
  byte         freespace25[12];
  word         uploadcredit;           //  Upload Credit Percentage
  byte         freespace26[36],
               shownewfileschar;
  byte         freespace[MAXFREE];
} CONFIGRECORD;


//  ------------------------------------------------------------------
//  CONSTANT.EZY Constant Configuration Information
//
//  The four following definitions
//     constantrecord
//     compressrecord
//     domaintype
//     constantfilefreespacetype
//  make up the complete CONSTANT.EZY file
//  As shown in "constantfilerecord"
//
//  scantossattr = bit
//               0 : Dupe Detection
//               1 : Kill Null Netmail
//               2 : Keep EchoArea Node Receipts
//               3 : Import Messages to Sysop
//               4 : Binkley Support
//               5 : Kill Bad Archives
//               6 : ArcMail 0.6 Compatability
//               7 : Binkley 5D Support
//            8-15 : [Reserved]
//
//  constantattr = bit
//               0 : Sysop Alias in Chat
//               1 : Auto Log Chat
//               2 : Display Full Message to User
//               3 : Do not delete outbound mail bundles with no .MSG
//               4 : On means do not use real name kludge line
//               5 : User can write messages to user of same name
//               6 : Users receive QWK messages that they posted
//               7 : Show Sysop Online
//               8 : Make Binkley Crash, Hold, etc
//               9 : Show Colour in File Areas
//           10-15 : [Reserved]
//
//  unknownarea =
//               0 : Kill Messages
//               1 : Make a New EchoMail Area
//               2 : Make a New PassThru Area

typedef struct CONSTANTRECORD {
  byte         version[9],
               system[41];
  UserString   sysopname,
               sysopalias;
  byte         systemlocation[36],
               multiline;              // boolean, multiline operation
  word         maxmess,                //  maximum usable message areas
               maxfile,                //  maximum usable file areas
               watchmess,              //  watchdog message area
               pagemessboard,          //  paging message board
               badpwdmsgboard;         //  bad logon message board
  byte         mintimeforcall,         //  minimum time to register call today
               freespace2[11];
  word         scantossattr,           //  ezymail scan/toss info
               constantattr,
               maxmsgsrescan;          //  Maximum msgs to rescan (0=disable)
  Str8         qwkfilename;            //  Unique QWK Mail filename
  word         qwkmaxmail,             //  Maximum Msgs for QWK archive
               qwkmsgboard;            //  Bad QWK Message Board
  ftn_addr     oldnetaddress[16];
  word         oldnetmailboard[16];
  byte         oldnewareagroup[16];
  word         oldnewareastmess[16];   //  New area start msg board
  byte         quotestring[6],         //  quote messsage string
               swaponezymail,          //  ezymail swapping information
               unknownarea,            //  unknown new area tag action
               swaponfeditview,        //  FEdit swapping information
               swaponarchive;          //  Ezymaint swapping information
  word         minspaceupload;         //  minimum space to upload
  byte         textinputcolour;        //  default text input colour
  word         badmsgboard;            //  Bad echomail msg board
  ftn_addr     netaddress[MAXAKA];
  word         netmailboard[MAXAKA];
  char         newareagroup[MAXAKA];
  word         newareastmess[MAXAKA];  //  New area start msg board
  byte         domain[21][MAXAKA];
  byte         freespace[CONSTANTFREESPACE];
} CONSTANTRECORD;


//  ------------------------------------------------------------------
//  <systempath>MESSAGES.EZY: Used by Ezycom to store message areas
//
//  attribute = bit
//               0 : Allow Aliases
//               1 : Use Alias
//               2 : Use Alias, Ask for Aliases
//               3 : Test Age (use config age)
//               4 : Combined Area Access
//               5 : Local File attaches
//               6 : Keep Private Bit on Incoming EchoMail
//               7 : Security *)
//
//  attribute2 = bit
//               0 : Show Seenby Lines
//               1 : Forced Mail Check
//               2 : Tiny Seenbys
//             3-4 : [Reserved]
//               5 : Areafix Info Visible
//               6 : Initial Combined Area Access
//               7 : Do Not Use in Template *)
//
//  attribute3 = bit
//             0-7 : [Reserved]

typedef struct MESSAGERECORD {
  char         name[31],
               areatag[76],
               qwkname[13];
  MsgType      typ;
  MsgKindsType msgkinds;
  byte         attribute,
               attribute2,
               attribute3,
               dayskill,
               recvkill;
  word         countkill,
               kilobytekill;
  SECURITYTYPE readsecurity,
               writesecurity,
               sysopsecurity;
  byte         minimumage;
  char         originline[51];
  byte         originaddress,
               seenby[MAXAKA/8],
               areagroup,
               messgroup,
               altgroups[3],
               echomailfeed,           // 0=No Uplink
               destnodes[MAXNODES/8],  // Nodes  1 to  8 - DestNode[1]
                                       // Nodes  9 to 16 - DestNode[2]
                                       // Nodes 17 to 24 - DestNode[3]
               freespace[32];
} MESSAGERECORD;


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack()
#endif


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
