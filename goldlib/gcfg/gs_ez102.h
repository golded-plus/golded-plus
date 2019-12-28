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

#ifndef __gs_ez102_h
#define __gs_ez102_h


//  ------------------------------------------------------------------

#include <gftnall.h>


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
    #pragma pack(1)
#endif


//  ------------------------------------------------------------------
//  EzyCom structures converted to C format V1.02 9/9/92
//  by Peter Davies and Ron Clark
//  Based on original C Structures for Ezycom V1.00 by Colin Berg
//
//  These structures may ONLY be used in accordance with those
//  agreements laid out in the Official Ezycom Structures.
//  Peter Davies or Ron Clark takes no responsibility whatsoever
//  for the correctness of these structures.

#define EZYVER                "1.02"
#define VERSIONHIGH           1
#define VERSIONLOW            2
#define MAXFREE               685
#define USERFREE              4
#define CONSTANTFREESPACE     89
#define CONSTANTFILEFREESPACE 816
#define MAXNODES              128
#define MAXAKA                16
#define MAXMESS               1024
#define MAXMESSALL            1024
#define MAXBAUDREC            11

typedef char AskType;         // 0=yes, 1=no, 2=ask
typedef char MsgType;         // 0=localmail, 1=netmail, 2=echomail, 3=passthru, 4=waitthru, 5=allmail
typedef char MsgKindsType;    // 0=public, 1=private, 2=private or public
typedef char UserString[36];
typedef uint8_t EzFlagType[4];


//  ------------------------------------------------------------------
//  CONFIG.<EZY or NODE> structure
//
//  unknownarea = 0 - kill messages
//                1 - make a new echomail area
//                2 - make a new passthru area
//
//  tossattr2 = bit 0 - [ reserved ]
//                  1 - [ reserved ]
//                  2 - kill bad archives
//                  3 - arcmail 0.6 compat
//
//  autodetect = bit 0 - Auto Detect ANSI
//                   1 - Ansi Detect for New User
//                   2 - Auto Detect EMSI
//                   3 - Auto Detect EMSI & for newuser
//
//  tossattr = bit 0 : [reserved]
//                 1 : [reserved]
//                 2 : [reserved]
//                 3 : dupe detection
//                 4 : delete netmail on import
//                 5 : keep echoarea node receipts
//                 6 : allow message rescan
//                 7 : [reserved]
//
//  multitasker = 0 = do not detect or use any multitasker
//                1 = auto-detect
//                2 = desqview (or compatiable)
//                3 = double dos
//                4 = OS.2
//                5 = multidos plus
//                6 = Taskview
//                7 = Topview
//                8 = PC MOS
//                9..255 [reserved]
//
//  printerport = 0 = LPT1
//                1 = LPT2
//                2 = LPT3
//                3 = COM1
//                4 = COM2
//                5 = COM3
//                6 = COM4

typedef struct CONFIGRECORD
{
    char            version[9],
                    deflanguage[9],
                    freespace50[68],
                    logpath[61],
                    textpath[61],
                    menupath[61],
                    mnurampath[61],
                    netmailpath[61],
                    nodelistpath[61],
                    msgpath[61],
                    filepath[61],
                    freespace51[61],
                    bipath[61],
                    temppath[61],
                    userbasepath[61],
                    avatarpath[61],
                    ascpath[61],
                    asclowpath[61],
                    filemaint[61],
                    fileattachpath[61],
                    soundpath[61],
                    fastindexpath[61],
                    systempwd[16],     // Password to Logon System
                    sysoppwd[16],      // Password to Keyboard
                    newuserpwd[16],    // Password for Newuser
                    newtopmenu[9],     // NewUser TopMenu
                    freespace52[4],
                    inboundmail[61],
                    outboundmail[61],
                    uploadpath[61],
                    swapfile[61],
                    multipath[61],
                    brackets[3],
                    inactivitytime,
                    minmesscheck,
                    maxlogintime,
                    /* unused */    freespace53,
                    answerdelay;
    char            shellswap,
                    highbit,
                    disppass,
                    asklocalpass,
                    fastlogon,
                    sysopremote,
                    printerlog,
                    phone1ask,
                    colourask,
                    aliasask,
                    dobask,
                    phoneforce,
                    direct_video,
                    snow_check;
    char            freespace54;
    char            screen_blank;
    char            oneword;
    AskType         checkmail,
                    checkfile,
                    ansiask,
                    fullscreenask,
                    clearask,
                    moreask,
                    avatarask,
                    extendask,
                    usdateask;
    char            phone2ask,
                    phoneformat[15],
                    nameprompt[61],
                    pwdprompt[61],
                    shellprompt[61],
                    shell2prompt[61],
                    enterprompt[61],
                    chatprompt[61],
                    listprompt[61];
    char            f7keylinetop[80],
                    f7keylinebot[80],
                    freespace55[84],
                    chat2prompt[61],
                    screenlengthprompt[61],
                    screenclearprompt[61],
                    locationprompt[61],
                    freeprompt[61],
                    loadprompt[61],
                    avatarprompt[61],
                    aliasprompt[61];
    word            security,
                    logonsecurity;
    EzFlagType      flags;
    char            minpasslength,
                    /* constant */  dispfwind,         // Status Bar Colour
                    /* constant */  dispbwind,         // Status Bar Colour
                    /* constant */  disppopupf,        // Popup Forground
                    /* constant */  disppopupborder,   // Popup Border
                    /* constant */  disppopupb,        // Popup Background
                    /* constant */  dispf;             // Foreground Colour
    char            freespace56,
                    comport,
                    passlogons,
                    doblogon,
                    printerport,
                    passtries;
    char            topmenu[9];
    char            freespace100[4];
    word            watchmess,
                    /* constant */  netmailcredit,
                    ansiminbaud,
                    /* unused */    freespace57,
                    slowbaud,
                    minloginbaud,
                    lowsecuritystart,
                    lowsecurityend,
                    slowstart,
                    slowend;
    char            quotestring[6],
                    freespace58,
                    offhook;
    word            forcecrashmail,
                    optioncrashmail,
                    netmailfileattach;
    char            popuphighlight,
                    freespace59,
                    maxpages,
                    maxpagefiles,
                    pagelength;
    word            pagestart[7],
                    /* constant */  pagemessboard,     // Message Board for Paging (0=Not In Use)
                    localfattachsec,
                    sectouploadmess,
                    sectoupdateusers,
                    readsecnewecho,
                    writesecnewecho,
                    sysopsecnewecho;
    word            secreplyvianetmail;
    AskType         netmailkillsent;
    char            swaponarchive;

    char            freespace60[9];

    char            popuptext;
    word            pageend[7];
    char            freespace61[22];
    word            incomingcallstart,
                    incomingcallend,

                    /* unused */    freespace62;

    char            altf[10][61],
                    ctrlf[10][41],
                    /* unused */    freespace63[4];
    word            fp_credit;         // Newuser Filepoints
    char            ks_per_fp,         // Number of Kilobytes per FP
                    fp_upload,         // Filepoints Upload Credit
                    rego_warn_1,
                    rego_warn_2;
    char            freespace64[2];
    word            min_space_1;
    char            swapbimodem,
                    modembusy,         // Toggle DTR or ATH1
                    scrheight,         // 43/50 line mode
                    msgtmptype,        // True  = MSGTMP False = MSGTMP.<node>
                    swapupload;
    char            phonelogon,
                    carrierdetect;     // Carrier Detect (Default=$80)
    char            newfileshighlight;
    char            max_descrip,
                    min_descrip;
    word            requestreceipt;
    char            ushowdate;
    char            ufilesizek;
    char            uuploader,
                    udownloadcount,
                    /* unused */    freespace4,
                    ushowsecurity,
                    sshowdate;
    char            sfilesizek;
    char            suploader,
                    sdownloadcount,
                    /* unused */    freespace5,
                    sshowsecurity,
                    ushowtime,
                    ushowfp,
                    sshowtime,
                    sshowfp;
    word            fp_percent;        // Download Filepoints Credit
    char            autodetect;
    char            dispsecurityfile,
                    askforpagereason,
                    delincompletefiles;
    char            freespace65;
    char            swaponfeditview;
    char            freespace6,
                    secfileschar,
                    passchar;
    char            localinactivity;
    char            conversiononmaster;
    char            leftbracket[2],
                    rightbracket[2];
    word            ignorefp;          // Min Security to Ignore FPs
    char            menuminage;        // Minimum Age for Age Checks
    char            freespace66;
    word            modemeff[MAXBAUDREC],
                    modembaud[MAXBAUDREC];
    char            modemconnect[MAXBAUDREC][16];
    char            freespace67[10];
    word            configattr;        // bit 0: Move Local Uploads, other reserved
    char            usercol1_2,
                    usercol3_4,
                    usercol5_6,
                    usercol7_8,
                    userbkcol,
                    newusercol2,
                    chstatcol,
                    getentercol;
    char            usdateforsysop,
                    ezyovrpath[61];
    UserString      sysfree3;
    char            ovrems;
    char            swapezy,
                    filesecpath[61];
    char            freespace28;
    char            multitasker;
    word            maxbaud;           // longint in 1.02
    char            lockedport;
    word            filereqsec;
    char            autoanswer,
                    initresponse[11],
                    ringstring[11];
    char            inittries,
                    initstring1[61],
                    initstring2[61],
                    busystring[21],
                    answerstring[21],
                    mailerstring[61];
    word            modemstart,
                    modemend;
    char            modemdelay;
    char            sendbreak;
    char            externaleditor[61],
                    defaultorigin[51],
                    connectfax[16];
    char            freespace22[16];
    word            uploadcredit;
    char            sysfree4[36];
    char            shownewfileschar;
    word
    /* unused */    zonea[16],
    neta[16],
    nodea[16],
    pointa[16],
    /* unused */    freespace24[16];
    char            freespace9[16][21],
                    nocarrierstring[21];
    UserString      guestaccount;
    char            freespace[MAXFREE];
} CONFIGRECORD;


//  ------------------------------------------------------------------
//  CONSTANT.EZY record structure
//
//  scantossattr = bit 0 : Dupe Detection
//                     1 : Kill Null Netmail
//                     2 : Keep EchoArea Node Receipts
//                     3 : Import Messages To Sysop
//                     4 : Route Mail (Off = Direct)
//                     5 : Kill Bad Archives
//                     6 : ARCMail 0.6 Compatability
//                  7-15 : [Reserved]
//
//  constantattr = bit 0 : Sysop Alias in Chat
//                     1 : Auto Log Chat
//                     2 : Display Full Message To User
//                     3 : Do not delete outbound mail bundles with no .MSG
//                  4-15 : [Reserved]
//
//  unknownarea  =     0 : Kill Messages
//                     1 : Make a New EchoMail Area
//                     2 : Make a New PassThru Area

typedef struct CONSTANTRECORD
{

    char            version[9];
    char            system[41];
    UserString      sysopname,
                    sysopalias;
    char            systemlocation[36];
    char            multiline;
    word            maxmess,           // maximum usable message areas
                    maxfile;           // maximum usable file areas
    word            watchmess,         // watchdog message board
                    pagemessboard,     // paging message board
                    badpwdmsgboard;    // bad pwd message board
    char            mintimeforcall;    // minimum time to register call
    char            freespace2[11];
    word            scantossattr,
                    constantattr;
    word            maxmsgsrescan;     // maximum msgs to rescan (0=disable)
    char            qwkfilename[9];
    word            qwkmaxmail,
                    qwkmsgboard;
    ftn_addr        netaddress[MAXAKA];
    word            netmailboard[MAXAKA];
    char            newareagroup[MAXAKA];
    word            newareastmess[MAXAKA];
    char            quotestring[6];
    char            swaponezymail;
    char            unknownarea;
    char            swaponfeditview,
                    swaponarchive;
    word            minspaceupload;
    char            textinputcolour;
    word            badmsgboard;
    char            freespace[CONSTANTFREESPACE];
} CONSTANTRECORD;


//  ------------------------------------------------------------------
//  MESSAGES.EZY record structure
//
//  attribute = bit 0 : allow aliases
//                  1 : use alias
//                  2 : use alias, ask for aliases
//                  3 : test age as defined in config.xx
//                  4 : combined area access
//                  5 : local file attaches
//                  6 : strip private bit on incoming echomail
//                  7 : security
//
//  attribute2 = bit 0 : show seenby lines
//                   1 : forced mail check
//                   2 : strip forward seenbys
//                3..4 : [reserved]
//                   5 : areafix info visible
//                   6 : initial combined area access
//                   7 : Do Not use in Template
//
//  attribute3 = [Reserved]
//
//  destnode = nodes  1 to  8 - destnode[1]
//             nodes  9 to 16 - destnode[2]
//             nodes 17 to 24 - destnode[3]
//             etc, etc, etc.

typedef struct MESSAGERECORD
{
    char            name[31];
    char            areatag[31];
    char            qwkname[13];
    MsgType         typ;
    MsgKindsType    msgkinds;
    char            attribute,
                    attribute2,
                    attribute3;
    char            dayskill,
                    recvkill;
    word            countkill,
                    kilobytekill,
                    readsecurity;
    EzFlagType      readflags;
    word            writesecurity;
    EzFlagType      writeflags;
    word            sysopsecurity;
    EzFlagType      sysopflags;
    char            originline[51];
    char            originaddress;
    char            seenby[MAXAKA/8];
    char            areagroup,
                    messgroup;
    char            destnodes[MAXNODES/8];
    char            echomailfeed;      // 0=No Uplink
} MESSAGERECORD;


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
    #pragma pack()
#endif


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
