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

#if defined (GOLD_CANPACK)
    #pragma pack(1)
#endif
//  ------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
//
//
//    OPUS 1.1x
//
//
/////////////////////////////////////////////////////////////////////////////
/*-------------------------------------------------------------------------*/
/* System*.DAT structure                                                   */
/*-------------------------------------------------------------------------*/
typedef struct
{
    /*........ Common System Data ....................................*/
    word version;           /* System Record version = 110 = v1.10    */
    word menu;              /* Alternate Menu file extension, 0=MNU   */
    word attrib;            /* Area attributes (see below)            */
    byte fillc1[10];        /* Fill to 16th                           */
    byte barrpath[40];      /* Barricade File path.                   */
    byte fillc2[24];        /* Fill to 16th                           */
    /*........ File System Information ...............................*/
    char filtitle[50];      /* File Area Title (as per old Dir.Bbs)   */
    char filepath[40];      /* Path to the file download sub-dir      */
    char uppath[40];        /* Path to the file upload sub-directory  */
    char listpath[40];      /* Path to FILES.BBS equivilent           */
    byte fillf1[22];        /* Fill to 16th                           */
    byte FilePriv;          /* Min priv for file area                 */
    byte DownPriv;          /* Override priv to download              */
    byte UpPriv;            /* Override priv to upload                */
    byte FileExtPriv;       /* Override priv for outside              */
    byte fillf2[12];        /* Fill to 16th                           */
    long FileLock;          /* Locks for File Area                    */
    long DownLock;          /* Override Locks to download             */
    long UpLock;            /* Override Locks to upload               */
    long FileExtLock;       /* Override Locks for Outside             */
    byte fillf3[32];        /* Fill to 16th                           */
    /*........ Message System Information ............................*/
    char msgtitle[50];      /* Msg  Area Title (as per old Dir.Bbs)   */
    char msgpath[40];       /* Path to messages                       */
    byte fillm1[22];        /* Fill to 16th                           */
    byte MsgPriv;           /* Min priv for msg area                  */
    byte EditPriv;          /* Override priv for Enter/Rep            */
    byte MsgExtPriv;        /* Override priv fot outside              */
    byte fillm2[13];        /* Fill to 16th                           */
    long MsgLock;           /* Locks for Msg Area                     */
    long EditLock;          /* Override Locks for Enter/Rep           */
    long MsgExtLock;        /* Override Locks for Outside             */
    byte fillm3[4];         /* Fill to 16th                           */
    char EchoName[32];      /* If an echo area, its name              */
    /*=================================== Total Record Size   = 512 ==*/
} _systemdat;
//////////////////////////////////////////////////////////////////////////////
//
//
//    OPUS 1.7x
//
//
//////////////////////////////////////////////////////////////////////////////
// INCLUDE OPUS.H ////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*               The Opus Computer-Based Conversation System                */
/*    (c) Copyright 1986-1991, Wynn Wagner III, All Rights Reserved         */
/*                                                                          */
/*                                                                          */
/*                   YOOHOO is a trademark of Wynn Wagner III               */
/*                                                                          */
/*                        YOOHOO-YOOHOO/2U2 & WaZOO are                     */
/*           Copyright 1987, Wynn Wagner III, All Rights Reserved           */
/*                                                                          */
/*                                                                          */
/* This material is available for use by anybody with no strings and        */
/* no guarantees.                                                           */
/*                                                                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/* LEGIBLE SECTION.  Definitions to make "C" look like a real language.     */
/*--------------------------------------------------------------------------*/
#undef log

#ifndef begin
#define begin {
#define end }
#endif
/*--------------------------------------------------------------------------*/
/* TIME STAMP                                                               */
/*--------------------------------------------------------------------------*/
struct _stamp begin uint16_t date;
uint16_t time;
end;
/*--------------------------------------------------------------------------*/
/* FIDONET ADDRESS STRUCTURE                                                */
/*--------------------------------------------------------------------------*/
typedef struct _ADDRESS begin short Zone;
short Net;
short Node;
short Point;
end ADDR;
/*--------------------------------------------------------------------------*/
/* NodeList.Sys                                                             */
/*                                                                          */
/*    NET > 0 and NODE > 0    Normal node                                   */
/*                                                                          */
/*    NET > 0 and NODE <= 0   Host node                                     */
/*                               Net host........node== 0                   */
/*                               Regional host...node==-1                   */
/*                               Country host....node==-2                   */
/*                                                                          */
/*    NET == -1      Nodelist.Sys revision                                  */
/*                                                                          */
/*    NET == -2      Nodelist statement                                     */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/* THE NEWSTYLE NODE LIST                                                   */
/*--------------------------------------------------------------------------*/
struct _node begin short NetNumber;
short NodeNumber;
word MsgFee;           /* Amount charged to user for a message */
byte SystemName[34];   /* node name */
byte PhoneNumber[40];  /* phone number */
byte MiscInfo[30];     /* city and state */
byte Password[8];      /* WARNING: not necessarily null-terminated */
word CallCost;         /* phone company's charge */
word HubNode;          /* node # of this node's hub or 0 if none */
byte BaudRate;         /* baud rate divided by 300 */
byte ModemType;        /* RESERVED for modem type */
word NodeFlags;        /* set of flags (see below) */
word NodeFiller;
end;


#define B_hub 0x0001     /* node is a net hub     0000 0000 0000 0001 */
#define B_host 0x0002    /* node is a net host    0000 0000 0000 0010 */
#define B_region 0x0004  /* node is region coord  0000 0000 0000 0100 */
#define B_zone 0x0008    /* is a zone gateway     0000 0000 0000 1000 */
#define B_CM 0x0010      /* runs continuous mail  0000 0000 0001 0000 */
#define B_res1 0x0020    /* reserved by Opus      0000 0000 0010 0000 */
#define B_res2 0x0040    /* reserved by Opus      0000 0000 0100 0000 */
#define B_res3 0x0080    /* reserved by Opus      0000 0000 1000 0000 */
#define B_res4 0x0100    /* reserved by Opus      0000 0001 0000 0000 */
#define B_res5 0x0200    /* reserved for non-Opus 0000 0010 0000 0000 */
#define B_res6 0x0400    /* reserved for non-Opus 0000 0100 0000 0000 */
#define B_res7 0x0800    /* reserved for non-Opus 0000 1000 0000 0000 */
#define B_point 0x1000   /* node is a point       0001 0000 0000 0000 */
#define B_res9 0x2000    /* reserved for non-Opus 0010 0000 0000 0000 */
#define B_resa 0x4000    /* reserved for non-Opus 0100 0000 0000 0000 */
#define B_resb 0x8000    /* reserved for non-Opus 1000 0000 0000 0000 */
/*--------------------------------------------------------------------------*/
/* Nodelist.Idx                                                             */
/* (File is terminated by EOF)                                              */
/*--------------------------------------------------------------------------*/
struct _ndi begin short node; /* node number  */
short net;                    /* net number   */
end;

#define MAXEXPAND 30          /* max download files (wildcard expansion)    */
#define CMDLEN 80             /* size of the command typeahead buffer       */
#define CARRYLEN 20           /* LORE editor's carry buffer                 */
#define MAXLEN 85             /* size of primary line-input buffer          */
/*--------------------------------------------------------------------------*/
/* OPUS CONTROL FILE and PARM FILE DECLARATIONS                             */
/*--------------------------------------------------------------------------*/
//#include "prm18.h"
// SUB-INCLUDE PRM18.H ///////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*               The Opus Computer-Based Conversation System                */
/*    (c) Copyright 1986-1991, Wynn Wagner III, All Rights Reserved         */
/*                                                                          */
/*                                                                          */
/*                   YOOHOO is a trademark of Wynn Wagner III               */
/*                                                                          */
/*                        YOOHOO-YOOHOO/2U2 & WaZOO are                     */
/*           Copyright 1987, Wynn Wagner III, All Rights Reserved           */
/*                                                                          */
/*                                                                          */
/* This material is available for use by anybody with no strings and        */
/* no guarantees.                                                           */
/*                                                                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* OPUS 1.20 Message/File area database structures.                         */
/* Copyright 1991 Wynn Wagner III, Doug Boone and John Valentyn.            */
/*                                                                          */
/*--------------------------------------------------------------------------*/

#define  THIS_CTL_VERSION 18        /* PRM structure version number         */

#if      COMPILER
#define CTLSIZE 0x7fff       /* Only used when compiling OPUS_CTL    */
#define OFS short
#elif    UTILS
#define CTLSIZE 1       /* Only used when compiling Utilities*/
#define OFS short
#else
#define CTLSIZE 1
#define OFS char *
#endif


#define  MAX_EXTERN 16              /* max. number of external programs     */
#define  MAXCLASS 12                /* number of possible priv levels       */
#define  ALIAS_CNT 15               /* number of matrix addresses           */

#define  _TWIT 0x10
#define  _DISGRACE 0x30
#define  _LIMITED 0x40
#define  _NORMALL 0x50
#define  _WORTHY 0x60
#define  _PRIVEL 0x70
#define  _FAVORED 0x80
#define  _EXTRA 0x90
#define  _CLERK 0xA0
#define  _ASSTSYSOP 0xB0
#define  _SYSOP 0xD0
#define  _HIDDEN 0xE0

#define  Twit _TWIT
#define  Disgrace _DISGRACE
#define  Limited _LIMITED
#define  Normal _NORMALL
#define  Worthy _WORTHY
#define  Privil _PRIVEL
#define  Favored _FAVORED
#define  Extra _EXTRA
#define  Clerk _CLERK
#define  AsstSysop _ASSTSYSOP
#define  Sysop _SYSOP
#define  Hidden _HIDDEN


struct   class_rec begin byte ClassPriv;
byte class_fill;
short max_time;      /* max cume time per day         */
short max_call;      /* max time for one call         */
word max_dl;         /* max dl bytes per day          */
byte ratio_down;
byte ratio_up;
word min_baud;       /* speed needed for logon        */
word min_file_baud;  /* speed needed for file xfer    */
end;
struct _FOSREGS begin word ax;
word bx;
word cx;
word dx;
end;
/*--------------------------------------------------------------------------*/
/* The format of the PRM file, VERSION 18                                   */
/*                                                                          */
/* THIS IS AN EXPLOSIVE STRUCTURE.  IT IS SUBJECT TO CHANGE WITH NO NOTICE. */
/*                                                                          */
/* Offsets to the following item(s) are guaranteed:                         */
/*                                                                          */
/*      byte   version;           OFFSET 0, all versions                    */
/*      byte   task_num;          OFFSET 1, 16+                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/
struct _PRM begin
/*-----------------------------------------------------------*/
/* DATA                                                      */
/*-----------------------------------------------------------*/
byte version;             /* for safety                          STABLE*/
byte task_num;            /* for multi-tasking systems           STABLE*/
ADDR alias[ALIAS_CNT];
byte video;               /* 0=Dos, 1=Fossil 2=IBM                     */
byte testmode;            /* input from keyboard, not modem            */
word carrier_mask;
word handshake_mask;
word max_baud;            /* fastest speed we can use                  */
word com_port;            /* Com1=0, Com2=1, FF=keyboard               */
byte multitasker;     /* flag for DoubleDos (see below)            */
byte mailer_type;     /* 0=Opus, 1=load external, 2=call external  */
byte ModemFlag;           /* (See MODEM FLAG below)                    */
byte LogFlag;             /* (See LOG FLAG below)                      */
byte StyleFlag;           /* (See STYLE FLAG below)                    */
byte FWDflag;             /* Bits to control IN TRANSIT messages       */
byte flags;               /* See "FLAGS" below                         */
byte Flags2;              /* See "FLAGS 2" below                       */
byte edit_exit;           /* ERRORLEVEL to use if Matrix area changed  */
byte exit_val;            /* ERRORLEVEL to use after caller            */
byte crashexit;           /* non-zero= ErrorLevel exit                 */
byte arc_exit;            /* ErrorLevel for after incomming ARCmail    */
byte echo_exit;           /* ERRORLEVEL for after inbound echomail     */
byte UDB_Flags;           /* User data base flags                      */
word min_baud;            /* minimum baud to get on-line               */
word color_baud;          /* min baud for graphics                     */
word date_style;          /* Used for FILES.BBS display                */
byte logon_priv;          /* Access level for new users                */
byte seenby_priv;     /* Min priv to see SEEN_BY line              */
byte ctla_priv;           /* Priv to see CONTROL-A lines in messages   */
byte Interline_Flags;   /* Flags relating to interline systems     */
byte AskPrivs[16];    /* Array of privs. for message attr ask's    */
byte AssumePrivs[16]; /* Array of privs. for message attr assume's */
word logon_time;          /* time to give for logons                   */
word matrix_mask;
word MinNetBaud;          /* minimum baud rate for remote netmail      */
byte exit_mailer;         /* Exit when mailer is running        */
byte num_crashes;         /* Number of tries when sending crash */
struct class_rec Class[MAXCLASS];
struct _FOSREGS FosRegs[10];
word F_Reward;            /* File upload time reward percentage        */
word last_area;           /* Highest msg area presumed to exist        */
word last_farea;          /* Highest file area presumed to exist       */
byte return_secure;   /* 0=ignore LASTUSER, >1 re-read LASTUSER    */
/*  New for Version 17 */
byte xuflags;             /* ExtUser Flags: 1=Alias, 2=UserTel         */
word xlmin;               /* Lead mins for expiry warning              */
char xlday;               /* Lead days for expiry warning              */
byte expriv;              /* Expiry privilege                          */
char totlang;             /* Total languages present (1-6)             */
byte sylno;               /* Def Sysop Language no.  (0-5)             */
byte uslno;               /* Def User Language no.   (0-5)             */
byte relog_exit;          /* Exit after -o relog                       */
word Scrn_Size;           /* Local monitor's size, high byte = len    */
/* New for version 18 */
word default_expire_days; /* Number of days before new users  */
word default_expire_mins; /* Number of days before new users  */
long default_keys;        /* New user's default keys                  */
long default_section;  /* New user's default section               */
word Point_Net;           /* My point net                             */
word Boss_Node;           /* My point net                             */
byte NoPwd_Priv;          /* Priv if you want to allow users without password */
byte Local_Exit;          /* Exit after local messages                */
byte Upload_Exit;         /* Exit after an upload for checking  */
byte ExtMail_Exit;        /* Exit for externmail                */
byte Max_Echo;            /* Longest echomail message to handle 3-60k */
byte WhoPriv;             /* Priv to see who uploaded a file           */
ADDR GUUCP;               /* Where to send gated UUCP messages             */
long making_section;
byte ListSysop;           /* Privilege neeeded to see sysops in net list */
byte rawpriv;             /* Privilege needed to see/download files not listed */
byte uucp_priv;           /* Privilege needed to turn off uucp addresses */
byte Flags3;              /* More flags.                               */
byte Mail_Exit;           /* Exit after mail connect                   */
byte byte_fill;
word PRM_FILL3[18];
/*-----------------------------------------------------------*/
/* OFFSETS                                                   */
/*-----------------------------------------------------------*/
/*-------------------------------------------*/
/* MODEM COMMAND STRINGS                     */
/*-------------------------------------------*/
OFS MDM_Init;         /* modem initialization string               */
OFS MDM_PreDial;      /* modem dial command sent before number     */
OFS MDM_PostDial;     /* modem command sent after dialed number    */
OFS MDM_LookBusy;     /* mdm cmd to take modem off hook            */
/*-------------------------------------------*/
/* PRIMROSE PATHS                            */
/*-------------------------------------------*/
OFS misc_path;        /* path to BBS/GBS files                     */
OFS sys_path;         /* path to SYSTEM?.BBS files                 */
OFS temppath;         /* place to put temporary files              */
OFS net_info;         /* path to NODELIST files                    */
OFS mailpath;         /* place to put received netmail bundles     */
OFS filepath;         /* place to put received netmail files       */
OFS hold_area;        /* path to pending outbound matrix traffic   */
/*-------------------------------------------*/
/* DATA FILE NAMES                           */
/*-------------------------------------------*/
OFS user_file;        /* path/filename of User.Bbs                 */
OFS sched_name;       /* name of file with _sched array            */
OFS langdir;          /* Langauge file dir. (Was Syl in v16)       */
OFS spcldir;          /* Spcl Ann Text dir. (Was Usl in v16)       */
/*-------------------------------------------*/
/* MISCELLANEOUS TEXT                        */
/*-------------------------------------------*/
OFS system_name;      /* board's name                              */
OFS sysop;            /* sysop's name                              */
OFS timeformat;
OFS dateformat;
OFS OFS_Filler1[8];
/*-------------------------------------------*/
/* BBS/GBS SUPPORT FILES                     */
/*-------------------------------------------*/
OFS logo;             /* first file shown to a caller              */
OFS welcome;          /* shown after logon                         */
OFS newuser1;
OFS newuser2;
OFS rookie;
OFS HLP_Editor;       /* Intro to msg editor for novices.          */
OFS HLP_Replace;      /* Explain the Msg.Editor E)dit command      */
OFS HLP_Inquire;      /* Explain the Msg. I)nquire command         */
OFS HLP_Locate;       /* Explain the Files L)ocate command         */
OFS HLP_Contents;     /* Explain the Files C)ontents command       */
OFS HLP_OPed;         /* help file for the full-screen editor      */
OFS OUT_Leaving;      /* Bon Voyage                                */
OFS OUT_Return;       /* Welcome back from O)utside                */
OFS ERR_DayLimit;     /* Sorry, you've been on too long...         */
OFS ERR_TimeWarn;     /* warning about forced hangup               */
OFS ERR_TooSlow;      /* explains minimum logon baud rate          */
OFS ERR_XferBaud;     /* explains minimum file transfer baud rate  */
OFS LIST_MsgAreas;    /* dump file... used instead of Dir.Bbs      */
OFS LIST_FileAreas;   /* dump file... used instead of Dir.Bbs      */
OFS FREQ_MyFiles;     /* file to send when FILES is file requested */
OFS FREQ_OKList;      /* list of files approved for file requests  */
OFS FREQ_About;    /* File Request: ABOUT file                  */
OFS OEC_Quotes;
OFS byebye;        /* file displayed at logoff                  */
OFS local_editor;     /* text editor to use in keyboard mode       */
OFS barricade;
OFS STATUS_dir;    /* Where to find INMAIL/ACTIVE/LASTUSER      */
OFS mailer;        /* full external mailer command              */
OFS common;        /* File with data common to all tasks        */
/*  New for Version 17 */
OFS xdwarn;        /* Date Warning OEC BBS                      */
OFS xtwarn;        /* Time Warning OEC BBS                      */
OFS xdexpd;        /* Expired due-to-Date OEC BBS               */
OFS xtexpd;        /* Expired due-to-Time used OEC BBS          */
OFS lang[12];      /* 12 Language File Root Name pointers       */
OFS badpath;       /* Path for unrecognized echo names         */
/*--------------------------------------------------------------------------*/
/* New for version 18                                                       */
/*--------------------------------------------------------------------------*/
OFS ConfHelp;      /* Help file for newuser configure          */
OFS Mainmenu;      /* ASCII menu at MAIN                       */
OFS Confmenu;      /* ASCII menu at Config                     */
OFS Sectmenu;      /* ASCII menu at Section menu               */
OFS Sysopmenu;     /* ASCII menu at Sysop menu                 */
OFS Echotoss;      /* Where to find EchoToss.Log               */
OFS MDM_Answer;    /* String to force autoanswer               */
OFS BAD_PWD;       /* File shown to users who've forgottent thiers */
OFS charset;       /* File that contains character translation tables */
OFS DL_Log;        /* Who's downloaded what                    */
OFS UL_Log;        /* Who uploaded what                        */
OFS Name_Filter;   /* Old USERNAME.TXT                         */
OFS Bad_Name;      /* What to show user if filter catches them */
OFS CUSTOM1_menu;      /* What to show before CUSTOM1 menu         */
OFS CUSTOM2_menu;      /* What to show before CUSTOM2 menu         */
OFS CUSTOM3_menu;      /* What to show before CUSTOM3 menu         */
OFS CUSTOM4_menu;      /* What to show before CUSTOM4 menu         */
OFS CUSTOM5_menu;      /* What to show before CUSTOM5 menu         */
OFS CUSTOM6_menu;      /* What to show before CUSTOM6 menu         */
OFS EXT_mail_string; /* What we see when UUCP is coming    */
OFS MaybeNew;        /* File shown if didn't find user name       */
OFS Histmenu;        /* ASCII menu at History                     */
OFS My_Question;     /* Sysop configured question             */
OFS HLP_make;        /* How to make a section                     */
OFS MDM_FDial[4];    /* Special dialing instructions          */
OFS Yell;            /* Yell OEC                                      */
OFS HLP_Macro;       /* Help for macro menu                       */
OFS UUCP_list;       /* Address list for UUCP                     */
OFS HLP_IChat;       /* Interline chat help                       */
OFS protocols[MAX_EXTERN];   /* external file protocol programs    */
/*v18*/ OFS OFS_Filler[7];
/*-----------------------------------------------------------*/
/* Log_Name must always be the last offset in this struct    */
/* because Bbs_Init uses that symbol to flag the end of      */
/* the offsets.                                              */
/*-----------------------------------------------------------*/
OFS log_name;         /* name of the log file                      */
/*-----------------------------------------------------------*/
/* Big blob of stuff                                         */
/* It's a sequence of null-terminated character arrays...    */
/* pointed-to by the offsets (above).                        */
/*-----------------------------------------------------------*/
char buf[CTLSIZE];
end;

#define ASK_PHONE 0x01              /* Used with xuflags                  */
#define ASK_ALIAS 0x02              /* Used with xuflags                  */
#define ASK_REAL 0x04               /* Used with xuflags                  */
#define ASK_CITY 0x08               /* Used with xuflags                  */
#define CAN_ALIAS 0x10              /* Allow ALIAS' in some situations    */
#define ASK_BDAY 0x20               /* Ask user for birthday              */
#define ASK_ADDRESS 0x40            /* Ask user for street address        */
/* Means no privilege level is set and/or applicable At LOGON,     */
/* for example, it means system is for PRE-REGISTERED users only.  */
#define  NO_PRIV 0xFF

struct _common_data
{
    char Opus[16];         /* "OPUS-CBCS 1.20"                               */
    long QuotePosition;    /* Quote file position                            */
    long CallerCount;      /* Number of callers                              */
    long Caller_ID;        /* ID of current newest user                      */
    long MSG_ID;           /* Last used MSGID                                */
    word FILLER[15];       /* RESERVED - do not use                          */
    word MAX_task;         /* Highest task number                            */
};

/* Explanation of _common_data:

   Opus will EXPECT to find "OPUS-CBCS 1.20" at the beginning of this file.
   If it does not find it, Opus will assume the file belongs to someone
   else and will ignore the file.

   File ctl.common is optional. It should be used only by systems that
   multitask. Its purpose is to allow separate schedule files for each
   task. If the file does not exist, Last quote pointer and Caller count
   are read from and updated in the scheduler.

   On the other hand, if this file exists the quote pointer is only updated
   in this file. Caller Count is updated separately both in the scheduler
   and this file, to allow separate statistics for each task, plus total
   statistics.

   MAX_task is informative only and should be set by the sysop. Its purpose
   is to help external intertask programs figure out how many tasks there
   exist. Its purpose is defeated if task numbers are not consecutive.

   More data may be added in the future. If so, either the FILLER field
   will be used, or the data will be appended. Existing definitions will
   remain it their respective places. Any FILLER bytes will be set to 0.
 */
/*--------------------------------------------------------------------------*/
/* MATRIX_CHANGED flags                                                     */
/*--------------------------------------------------------------------------*/

#define     MATRIX_CHANGED 0x0001     /* Changed matrix area. Scan          */
#define     LOCAL_CHANGED 0x0002      /* Changed Local area.                */
#define     GOT_BUNDLE 0x0100       /* Received a PKT file              */
#define     GOT_ARCMAIL 0x0200      /* Received an archived file        */
#define     GOT_REQUESTS 0x0400     /* Got file requests to handle      */
#define     GOT_ECHOMAIL 0x1000     /* Received Echomail                */
/*--------------------------------------------------------------------------*/
/* INTERLINE FLAGS                                                          */
/*--------------------------------------------------------------------------*/
#define     CAN_CHAT 0x01           /* Multiline systems can do chats     */
// END OF SUB-INCLUDE PRM18.H ////////////////////////////////////////////////////
/*--------------------------------------------------------------------------*/
/* Multitaskers (possible values for `ctl.multitasker'                      */
/*              NOTE: 0 means no multitasker in use                         */
/*--------------------------------------------------------------------------*/
#define DoubleDOS 1
#define DesqView 2
#define TopView 3
#define TaskView 3
#define LAN 0x80
/*--------------------------------------------------------------------------*/
/* Matrix mask                                                              */
/* Undefined bits are reserved by Opus  See EVENT.H                         */
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/* FLAGS                                                                    */
/*--------------------------------------------------------------------------*/
#define LOGECHO 0x01       /* log echomail areas                  0000 0001 */
#define STEADY 0x02        /* never change baud rate              0000 0010 */
#define ECHOSCAN 0x04      /* automatically scan echo's           0000 0100 */
#define ECHO_GUARD 0x08    /* no toss un-pwd echo arc             0000 1000 */
#define LEAVE_ESCN 0x10    /* Leave echotoss.log after Scanning   0001 0000 */
#define DO_MSGID 0x20      /* Put MSGID lines into echomail       0010 0000 */
#define F_NOCRASH 0x40     /* Don't accept crash mail             0100 0000 */
#define F_UNPK 0x80        /* Unpack incomming arcmail            1000 0000 */
/*--------------------------------------------------------------------------*/
/* FLAGS 2                                                                  */
/*--------------------------------------------------------------------------*/
#define NO_USRLIST 0x01    /* no user list (enter msg)            0000 0001 */
#define WATCHDOG 0x02      /* Reboot during outside               0000 0010 */
#define F2_TOSS 0x04       /* Toss incomming echomail             0000 0100 */
#define F2_LASTREAD 0x08   /* Uses LASTREAD for Sysop             0000 1000 */
#define F2_DOSFILE 0x10    /* Close stdAUX and stdPRN             0001 0000 */
#define F2_PRIMARY 0x20    /* MultiTasker Primary task            0010 0000 */
#define F2_SECOND 0x40     /* MultiTasker Secondary task          0100 0000 */
#define F2_ECON 0x80       /* Econo Echo handling                 1000 0000 */
/*--------------------------------------------------------------------------*/
/* FLAGS 3                                                                  */
/*--------------------------------------------------------------------------*/
#define F3STEADY 0x01       /* Can do ARQ steady. &B2                    */
#define F3GMENU 0x02        /* Will let users do graphics menus          */
#define F3LOGOFF 0x04       /* Collect logoff messages                   */
#define F3NOALIAS 0x08      /* Don't put alias' in SEEN-BY               */
#define F3HINAMES 0x10      /* Allow users with hi-bit names             */
/*--------------------------------------------------------------------------*/
/* FWDflags (bits to control IN TRANSIT messages)                           */
/*--------------------------------------------------------------------------*/
#define FWD_NONE 0x01      /* No IN TRANSIT netmail               0000 0001 */
#define FWD_PWD 0x02       /* IN TRANSIT by pwd only              0000 0010 */
#define FWD_TONET 0x04     /* Accept IN TRANSIT only to my net    0000 0100 */
#define FWD_NOBITS 0x08    /* No IN TRANSIT if extended bits used 0000 1000 */
#define FWD_HOST 0x10       /* Send mail to host if can't find.          */
#define VERSION6 0x40   /* Use Version 6 nodelist                        */
#define IMAPOINT 0x80   /* This is a point                              */
/*--------------------------------------------------------------------------*/
/* LOG FLAGS                                                                */
/*--------------------------------------------------------------------------*/
#define LOG_TERSE 0x01     /* brief log                           0000 0001 */
#define LOG_wordY 0x02     /* wordy log                           0000 0010 */
#define LOG_TRACE 0x04     /* very wordy log                      0000 0100 */
/*--------------------------------------------------------------------------*/
/* MODEM FLAGS                                                              */
/*--------------------------------------------------------------------------*/
#define M_SNOOP 0x01       /* sysop display on                    0000 0001 */
#define MOUSE 0x02         /* sysop mouse on                      0000 0010 */
#define M_BRKCLEAR 0x04    /* Use BREAK to clear outbound buffer  0000 0100 */
#define SLOW_MODEM 0x08     /* Slow modem and fast computer                 */
/*--------------------------------------------------------------------------*/
/* STYLE FLAGS                                                              */
/*--------------------------------------------------------------------------*/
#define SF_PATH 0x01       /* use path, not Dir.Bbs               0000 0001 */
#define SF_KILL 0x02       /* Kill rec'd pvt messages             0000 0010 */
#define SF_ASKKILL 0x04    /* ASK about killing msgs              0000 0100 */
#define SF_EUROPE 0x08      /* European date strings */
#define SF_TIME 0x10    /* Show time remaining on menu         0001 0000 */
#define SF_NOFLAG 0x20  /* Don't allow user config at logon    0010 0000 */
#define SF_HANDHOLD 0x40    /* Track user's errors and add help    0100 0000 */
#define SF_NOHIGH 0x80  /* Track user's errors and add help    0100 0000 */


#define SET_FLAG(x, y) ((x) |= (y))
#define CLEAR_FLAG(x, y) ((x) &= (~(y)))
#define isFLAG(x, y) ((x) & (y))
/*--------------------------------------------------------------------------*/
/* The rest of this file contains structure definitions similar to those    */
/* used by Fido<tm> version 11w.  No claim is made that Opus will maintain  */
/* compatibility with these structures beyond Opus version 0.               */
/*                                                                          */
/* The original version of the following items can be found in STRUCT.H, an */
/* include file writte (and probably copyrighted by) Tom Jennings.          */
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/*   THE USER DATABASE                                                      */
/*--------------------------------------------------------------------------*/

#define    MAX_USEC 80   /* Maximum number of areas in "user" section */
struct   _usr
{
    char name[36];       /* Caller's first and last names                 */
    char city[36];       /* Caller's location                             */
    char pwd[16];        /* Password                                      */
    char usrtel[16];     /* User Tel# for ref or future callback .........*/
    char alias[32];      /* User alias if ID is not it ...................*/
    /* 136 bytes */
    word times;          /* Number of previous calls to this system       */
    byte ClassPriv;      /* User Access Privilege                         */
    byte help;           /* Help level (see below)                        */
    byte tabs;           /* 0=transmit <sp> instead of <tab>              */
    byte language;       /* 0=english, 1=french                           */
    word nulls;          /* Number of Nulls (delays) after <cr>           */
    word msg;            /* Last message area visited                     */
    word Bits;           /* SEE BELOW                                     */
    /* 148 bytes */
    long ClassLock;      /* 32 User Keys                                  */
    long ludate;         /* Last UNIX Date on system                      */
    /* 156 bytes */
    short time;          /* Time on-line so-far today                     */
    word  flag;          /* Used to hold User maintainence information    */
    /* -------1 0x01 Reuse record for next new user  */
    /* xxxxxxx0 ---- Reserved reserved for opus Mgr  */
    long  upld;          /* K-bytes uploaded, all calls                   */
    long  dnld;          /* K-bytes downloaded, all calls                 */
    short dnldl;         /* K-bytes downloaded, today                     */
    word  files;         /* Last file area visited                        */
    byte  width;         /* Width of the caller's monitor                 */
    byte  len;           /* Height of the caller's                        */
    word  credit;        /* FidoNet usage credit in cents                 */
    word  debit;         /* FidoNet usage in cents                        */
    char  spcoec[8];     /* Special OECC to show after logon .............*/
    /* Good for clubs, stores, user groups ..........*/
    /* Eg, "Welcome to the dBase User Group" ........*/
    byte saccnt[5];      /* Array of 5 special announcement cntrs ........*/
    /* If any is > 0 than the user is shown .........*/
    /* a corresponding announcement file & the ......*/
    /* counter is decremented till zero .............*/
    byte exflag;         /* Flag of expiration control bits ..............*/
    /* -------- 000 No expiry actions ...............*/
    /* -------1 001 Expire AFTER xdate ..............*/
    /* ------1- 002 Expire when dbmin GE crmin ......*/
    /* -----:-- 004 ... Reserved ...  ...............*/
    /* ----:--- 008 ... Reserved ...  ...............*/
    /* ---1---- 016 Demote user priv to (expriv) ....*/
    /* --1----- 032 Axe user when expired  ..........*/
    /* -:------ 064 ... Reserved ...  ...............*/
    /* :------- 128 ... Reserved ...  ...............*/
    long     xdate;         /* Expiry date (UNIX style for math/futr)........*/
    long     crmin;         /* Total minutes given to user ..................*/
    long     dbmin;         /* Total minutes used  to user ..................*/
    char     u_section[16]; /* String for user current section          */
    char     ulikes[16];    /* String of user interest keywords .............*/
    int32_t  fudate;        /* First time user called system in seconds      */
    uint32_t caller_id;   /* Unique ID for this user. fudate        */
    uint32_t Section;       /* What "section" of the bbs the user is in.    */
    word     menu_mode;     /* What menu the user was at last ...............*/
    word     TwoBits;       /* More user configuration flags                */
    word     After_Externs; /* Any special flags from external?         */
    byte     msecs;         /* Number of message areas in User sections defined  */
    byte     fsecs;         /* Number of file areas in User sections defined     */
    /* terminated!! */
#ifdef OLD
    word lastmsg[MAXLREAD]; /* All the last message read stuff .....*/
#else
    /* New for Opus 1.20 */
    char     Address[32];  /* User's address                        */
    char     My_Ans[64];   /* Reply to prm.My_Question              */
    char     Strfill[32];  /* Filler for more strings               */
    char     bday_mon;             /* User's birthday/month                 */
    char     bday_day;             /* User's birthday/day                   */
    short    bday_year;            /* User's birthday/year                  */
    uint32_t messages_read;  /* Total number of messages read */
    int32_t  messages_sent;  /* Number of messages entered by user    */
    word     my_section[MAX_USEC]; /* This is just a block of area  */
    /* numbers in the user's Section */
    /* Message areas are first, then */
    /* file areas. The first file    */
    /* area is at my_section[msecs]  */
    word big_fill[40];       /* Fill out to 512 bytes                 */
    word Opus_Flags;         /* Stores Opus temporary configuration */
    byte byte_fill;
    byte Default_Protocol;   /* Default transfer protocol           */
    byte User_MMacro[16];    /* Message User's keyboard Macro       */
    byte User_FMacro[16];    /* File User's keyboard Macro          */
    byte User_DMacro[16];    /* Default User's keyboard Macro       */
    byte Sysop_Comment[80];  /* Anything you want to say about 'em  */
#endif
    uint32_t OPUS_id;
    /* The next 7 sets of id and inf data are for    */
    /* external programs to use for auxialliary inf. */
    /* Opus Mgr uses 1st 2 for reamrks about user.   */
    long extern_id[7];            /* LONG ID number for external programs */
    /* Must be registered with OpusInfo ....*/
    byte extern_inf[7][32];       /* 7 32-byte external util data blocks  */
    /* Total record size = 1024b .........................................*/
};

/*--------------------------------------------------------------------------*/
/* USER OPTIONS bit weights (undefined bits are reserved)                   */
/*--------------------------------------------------------------------------*/
#define  FillerBit 0x0001   /* Reserved                                     */
#define  ASKED_LEN 0x0002   /* Asked user for screen length?                */
#define  NO_IBMCHAR 0x0004  /* Cannot receive IBM textmode graphics         */
#define  USE_LORE 0x0008    /* Wants LORE editor instead of OPed            */
#define  MORE_PROMPT 0x0010 /* Wants the page break "MORE?" question        */
#define  ANSI 0x0020        /* Can handle ANSI video                        */
#define  CONFIG_SET 0x0040  /* OPUS logon questions answered                */
#define  FORMFEED 0x0080    /* SET=xmit clearscreen, CLEAR=eat formfeed     */
#define  AVATAR 0x0100      /* Can handle AVATAR (aka "oANSI") video        */
#define  TALKER 0x0200      /* Wants limited graphics for speech            */

#define  USE_PHONE 0x0400   /* Call Back User Modem                         */
#define  USE_ALIAS 0x0800   /* Enable ALIAS searches, message entry, etc    */

#define  NO_NAME 0x1000    /* User List Name                               */
#define  NO_TIME 0x2000    /* User List Time                               */
#define  NO_CITY 0x4000    /* User List City                               */


#define  CURSOR_CONTROL (ANSI | AVATAR)
/*--------------------------------------------------------------------------*/
/* 2Bits flags (More user configuration choices....                         */
/*--------------------------------------------------------------------------*/
#define  HOTKEYS 0x0001     /* Wants HotKeys at all help levels             */
#define  QMENU 0x0002       /* Wants to return to last menu instead of MAIN */
#define  ASKGRAPH 0x0004    /* Wants to be asked about graphics every logon */
#define  ASCIIMENU 0x0008       /* Wants long tedious graphics menus        */
#define  SHOW_TIME 0x0010       /* Show time remaining at prompts        */
#define  BLOCKCHAT 0x4000       /* User doesn't want to be sociable.     */
#define  INCRITAREA 0x8000      /* Opus is in critical area              */
/*--------------------------------------------------------------------------*/
/* eXpiration flags                                                         */
/*--------------------------------------------------------------------------*/
#define BY_DATE 0x0001          /* Zap users by date                        */
#define BY_TIME 0x0002          /* Zap users when they run out of credit    */
#define DEMOTE 0x0010           /* Demote user instead of deleting them     */
#define AXE 0x0020              /* Delete user                              */
/*--------------------------------------------------------------------------*/
/* User help levels                                                         */
/*--------------------------------------------------------------------------*/
#define  HITECH 0x00       /* Lotus style, top-of-screen, menu bars         */
#define  EXPERT 0x02       /* grizzled veteran, no menus at all             */
#define  REGULAR 0x04      /* experienced user, brief menus                 */
#define  NOVICE 0x06       /* Full menus plus additional hand-holding       */
#define  USING_FTB 0xff    /* caller is using the Full-Tilt-Boogie method   */
/*--------------------------------------------------------------------------*/
/* Flags for After_Externs                                                  */
/*--------------------------------------------------------------------------*/
#define NEW_ECHOS 0x0001        /* New Echomail entered by user             */
#define NEW_MATRIX 0x0002       /* New Matrix entered by user               */
#define NEW_LOCAL 0x0004        /* New Local mail entered by user           */
#define UPLOADED 0x0008         /* User uploaded something                  */
#define DUP_UPLOAD 0x0010       /* User uploaded a duplicate file           */
/*--------------------------------------------------------------------------*/
/* LASTUSER.BBS file structure                                              */
/*--------------------------------------------------------------------------*/
/* Struct of LASTUSE#.BBS file that is written everytime a user logons on   */
/* or when Opus does an OUTSIDE exit.  It contains a complete and exact     */
/* copy of the user record, following by extended data for session control. */
/* If PRM is configured to reload user data upon return from an OUTSIDE     */
/* exit, then Opus reloads user record portion, making it the active copy.  */
/*--------------------------------------------------------------------------*/

#ifdef _TM_DEFINED
struct _lu_file begin struct _usr user; /* Copy of user record                           */
word baud;               /* Current user's baud (0 == keyboard!)          */
word port;               /* Current port #                                */
word task;               /* Active task number                            */
word mins;               /* User's minutes remaining                      */
word msgarea;            /* Which message area user is in                 */
word filearea;           /* Which file area user is in                    */
long timeoff;            /* UNIX GMT of latest time user stays on till    */
struct tm tmoff;         /* MS-C 'tm' struct of above but as local time   */
char laston[25];         /* Orig Last Call Date as ASCIIZ string because  */
/* same field in user rec now is curr logon time */
end;
#endif
/*-------------------------------------------------------------------------*/
/*                                                                         */
/* MESSAGE and FILE AREAS                                                  */
/*                                                                         */
/*-------------------------------------------------------------------------*/
/* See SYSTEM.H */
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* MESSAGES                                                                 */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/* Message header                                                           */
/*--------------------------------------------------------------------------*/
struct opus_msg begin byte from[36];
byte to[36];
byte subj[72];
byte date[20];       /* Obsolete/unused ASCII date information        */
word times;          /* FIDO<tm>: Number of times read                */
word dest;           /* Destination node                              */
word orig;           /* Origination node number                       */
word cost;           /* Unit cost charged to send the message         */
word orig_net;       /* Origination network number                    */
word dest_net;       /* Destination network number                    */
/* A TIMESTAMP is a 32-bit integer in the Unix   */
/* flavor (ie. the number of seconds since       */
/* January 1, 1970).  Timestamps in messages are */
/* always Greenwich Mean Time, never local time. */
struct _stamp date_written;   /* When user wrote the msg              */
struct _stamp date_arrived;   /* When msg arrived on-line             */
word reply;          /* Current msg is a reply to this msg number     */
word attr;           /* Attribute (behavior) of the message           */
word up;             /* Next message in the thread                    */
end;
/*--------------------------------------------------------------------------*/
/* Message attributes                                                       */
/*--------------------------------------------------------------------------*/
#define MSGPRIVATE 0x0001  /* private message         * 0000 0000 0000 0001 */
#define MSGCRASH 0x0002    /* squirtmail              * 0000 0000 0000 0010 */
#define MSGREAD 0x0004     /* read by addressee       * 0000 0000 0000 0100 */
#define MSGSENT 0x0008     /* sent OK (remote)          0000 0000 0000 1000 */
#define MSGFILE 0x0010     /* file attached to msg    * 0000 0000 0001 0000 */
#define MSGFWD 0x0020      /* in transit                0000 0000 0010 0000 */
#define MSGORPHAN 0x0040   /* unknown dest node         0000 0000 0100 0000 */
#define MSGKILL 0x0080     /* kill after bundling     * 0000 0000 1000 0000 */
#define MSGLOCAL 0x0100    /* FidoNet vs. local         0000 0001 0000 0000 */
#define MSGHOLD 0x0200     /* Hold, don't send        * 0000 0010 0000 0000 */
#define MSGXX2 0x0400      /* <reserved>             X? 0000 0100 0000 0000 */
#define MSGFRQ 0x0800      /* file request            * 0000 1000 0000 0000 */
#define MSGRRQ 0x1000      /* receipt requested      X* 0001 0000 0000 0000 */
#define MSGCPT 0x2000      /* is a return receipt    X* 0010 0000 0000 0000 */
#define MSGARQ 0x4000      /* audit trail requested  X* 0100 0000 0000 0000 */
#define MSGURQ 0x8000      /* update request         X* 1000 0000 0000 0000 */
/*-----------------------*/
/* ^                     */
/* |                     */
/*  * = preserved by     */
/*      the network      */
/*  ? = stripped by the  */
/*      net (FTSC spec)  */
/*      but preserved    */
/*      by seadog<tm>    */
/*  X = not used by Opus */
/*-----------------------*/
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* EVENTS                                                                   */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/*  #include <event.h>  */
/*--------------------------------------------------------------------------*/
/* Message bundle header                                                    */
/*--------------------------------------------------------------------------*/

#define PKTVER 2                      /* Used for `ver' (below)                     */
struct _pkthdr begin short orig_node; /* originating node               */
short dest_node;                      /* destination node               */
short year;                           /* 0..99  when packet was created */
short month;                          /* 1..12  when packet was created */
short day;                            /* 1..31  when packet was created */
short hour;                           /* 0..23  when packet was created */
short minute;                         /* 0..59  when packet was created */
short second;                         /* 0..59  when packet was created */
short rate;                           /* destination's baud rate        */
short ver;                            /* packet version                 */
short orig_net;                       /* originating network number     */
short dest_net;                       /* destination network number     */
char product;                         /* product type                   */
char x1;                              /* filler (used by some systems)  */
/* ------------------------------ */
/* THE FOLLOWING SECTION IS not   */
/* THE SAME ACROSS SYSTEM LINES:  */
/* ------------------------------ */
byte pktpwd[6];
byte B_fill1[2];
word Orig_Zone;
word Dest_Zone;
byte B_fill2[16];
long B_fill3;
end;
/*--------------------------------------------------------------------------*/
/* WAZOO                                                                    */
/*--------------------------------------------------------------------------*/

#ifndef ACK
#define ACK 0x06
#endif

#ifndef NAK
#define NAK 0x15
#endif

#ifndef ENQ
#define ENQ 0x05
#endif

#ifndef YOOHOO
#define YOOHOO 0x00f1         /* 241 (a prime number, I think) */
#endif

#ifndef  TSYNC
#define  TSYNC 0x00ae
#endif

struct   _Hello begin word signal; /* always 'o'     (0x6f)                   */
word hello_version;        /* currently 1    (0x01)                   */
word product;                      /* product code                            */
word product_maj;                  /* major revision of the product           */
word product_min;                  /* minor revision of the product           */
char my_name[60];                  /* Other end's name                        */
char sysop[20];                    /* sysop's name                            */
ADDR my_address;
byte my_password[8];       /* ONLY 6 CHARACTERS ARE SIGNIFICANT !!!!! */
byte reserved2[8];                 /* reserved by Opus                        */
word capabilities;                 /* see below                               */
byte reserved3[12];        /* available to non-Opus systems by prior  */
/* "approval" of 124/108.                  */
end; /* size 128 bytes */
/*--------------------------------------------------------------------------*/
/* YOOHOO<tm> CAPABILITY VALUES                                             */
/*--------------------------------------------------------------------------*/
#define Y_DIETIFNA 0x0001  /* Can do LoTek            0000 0000 0000 0001 */
#define FTB_USER 0x0002    /* reserved by Opus        0000 0000 0000 0010 */
#define Bit_2 0x0004       /* reserved by Opus        0000 0000 0000 0100 */
#define ZED_ZAPPER 0x0008  /* Can do ZModem/plain     0000 0000 0000 1000 */
#define DOES_IANUS 0x0010  /* Does Opus Janus         0000 0000 0001 0000 */
#define Bit_5 0x0020       /* reserved by Opus        0000 0000 0010 0000 */
#define Bit_6 0x0040       /* reserved by Opus        0000 0000 0100 0000 */
#define Bit_7 0x0080       /* reserved by Opus        0000 0000 1000 0000 */
#define Bit_8 0x0100       /* reserved by Opus        0000 0001 0000 0000 */
#define Bit_9 0x0200       /* reserved by Opus        0000 0010 0000 0000 */
#define Bit_a 0x0400       /* reserved by Opus        0000 0100 0000 0000 */
#define Bit_b 0x0800       /* reserved by Opus        0000 1000 0000 0000 */
#define Bit_c 0x1000       /* reserved by Opus        0001 0000 0000 0000 */
#define Bit_d 0x2000       /* reserved by Opus        0010 0000 0000 0000 */
#define Bit_e 0x4000       /* reserved by Opus        0100 0000 0000 0000 */
#define WZ_FREQ 0x8000     /* accepts file requests   1000 0000 0000 0000 */
/*

   User Indexing Support Items

   Bit setting of (word) UDB_Flags

   & 1 ... Set ON if user name CRC indexing is expected.  The general
           init func should look for User.Idx and check its size for
           a match to the user file size.  Must be nrecs * sizeof(long).

           Or, can simply be forced on and the first func that finds the
           index to be missing or inconsistent will give a short status
           error and turn the bit off, stopping further index attempts.

   & 2 ... Set ON if user file locking is to be attempted which requires
           SHARE.EXE to be loaded.  This is desired as updates to the
           user file index should be controlled as access to 2 separate
           files in a multitasked scenario can span some time and the
           chance of indexing errors is quite high.  When ON, the 1st byte
           of ONLY the user file is locked to mean that the user database
           is being updated.  There should be no need to lock the index
           as the user file is always accessed first.

           It has to be tested to see if the effect of other read accesses
           to the user file (eg, user listing, searching) will fail if
           the file is locked during an update.  I suspect that other
           activities will be transparent to it and ONLY other LOCKING
           attempts will be foiled.  This would allow compatibility with
           other existing R/O accesses to the file.

           If it proves to be safe, we can force this ON and allow the
           the first function wherein it fails to turn it off with a
           single short message.

 */

#define UsrIdx 1                          /* Says, Attempt CRC indexing     */
#define UsrLok 2                          /* Says, Attempt File Locking     */
#define UsrPwd 4

#define HAS_UF(v) (ctl.UDB_Flags & v)     /* Check if Ufile feature used    */
#define STOP_UF(v) ctl.UDB_Flags &= ~v    /* Turn off Ufile feature         */
/* END OF FILE: opus.h */
// INCLUDE SYSTEM.H //////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*               The Opus Computer-Based Conversation System                */
/*    (c) Copyright 1986-1991, Wynn Wagner III, All Rights Reserved         */
/*                                                                          */
/*                                                                          */
/*                   YOOHOO is a trademark of Wynn Wagner III               */
/*                                                                          */
/*                        YOOHOO-YOOHOO/2U2 & WaZOO are                     */
/*           Copyright 1987, Wynn Wagner III, All Rights Reserved           */
/*                                                                          */
/*                                                                          */
/* This material is available for use by anybody with no strings and        */
/* no guarantees.                                                           */
/*                                                                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* OPUS 1.20 Message/File area database structures.                         */
/* Copyright 1991 Wynn Wagner III, Doug Boone and John Valentyn.            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/* Structure for SYSMSG.DAT                                                 */
/* A single-file message area database                                      */
/*--------------------------------------------------------------------------*/
struct  _filesys
{
    char Area_Name[32]; /* Area's primary name */
    word Area_Number;   /* area number */
    word Area_Menu;     /* which menu? */
    word Total_Size;
    byte Area_Priv;     /* Area Privilege */
    byte Down_Priv;     /* Download Privilege */
    byte Up_Priv;
    byte Priv_Up;       /* Uploads from users with Priv>= Priv_Up */
    byte Ratio_Priv;    /* Privilege where u/d ratios start */
    byte New_Priv;
    byte fill_byte1[2];
    /* 48 */
    word Attrib;        /* Area Attribute */
    word Status;        /* Area Status, extended Attribute? */
    long Start_Pos;     /* start of Area in sysfile.dat */
    /* 56 */
    long Area_Lock;     /* Area Lock */
    long Up_Lock;
    long Down_Lock;     /* edit Lock */
    long Priv_Key;      /* Private Key path */
    /* 72 */
    long Section;       /* edit Lock */
    long fill_long[2];
    byte Title_Len;     /* length of Title */
    byte Prefiles_Len;      /* files shown at Area access like Areainfo */
    byte Downpath_Len;  /* length of path */
    byte Uppath_Len;    /* length of path */
    /* 88 */
    byte Barricade_Len; /* Barricade length if used */
    byte Rules_Len;
    byte Privkey_Len;   /* Uploads from users with "key" go here */
    byte PrivUp_Len;    /* Private Upload path */
    byte Menu_Len;      /* file name for this area's ascii menu */
    byte Vol_Len;       /* Length of volume name */
    byte Help_Len;      /* Length of help name for this area */
    byte fill_byte2[5]; /* more filler */
    /* 100 */
    word Other_Len;     /* external programs use this number bytes */
    word Def_Upload;    /* Area number for default uploads */
    word Priv_Upload;   /* Area number for Priv uploads */
    word Key_Upload;    /* Area number for Lock uploads */
    word fill_word[10]; /* more filler to 128 bytes*/
};

/*  Title */
/*  Prefiles */
/*  Downpath */
/*  Barricade */
/*  Rules */
/*  Menu */
/*  Help */
/*  Upload */
/*  Priv Upload */
/*  Key Upload */
/* File area help file */
/*  Volume */
/*  "Other" field. Reserved for external programs */
/*--------------------------------------------------------------------------*/
/* Structure for SYSMSG.DAT                                                 */
/* A single-file message Area database                                      */
/*--------------------------------------------------------------------------*/
struct  _msgsys
{
    char Area_Name[32]; /* Area's primary name */
    char Echo_Name[32]; /* Area's echo name */
    word Area_Number;   /* Area number */
    word Area_Menu;     /* which menu? */
    word Total_Size;    /* Total size of this Area in SYSMSG.DAT */
    byte Area_Priv;     /* Area Privilege */
    byte Edit_Priv;     /* Edit Privilege */
    byte Private_Priv;  /* Privilege to read Private messages*/
    byte Upload_Priv;
    byte fill_0[3];
    byte Translate;     /* Which character set to use 1, 2.... */
    /* 80 */
    long Area_Lock;     /* Area Lock */
    long Edit_Lock;     /* Edit Lock */
    long Private_Lock;  /* Lock to read Private messages*/
    long Upload_Lock;
    word Attrib;        /* Area Attribute */
    word Status;        /* Area Status, extended Attribute? */
    long Start_Pos;     /* start of Area in sysmsg.dat */
    long Section;
    byte Max_Lines;     /* Maximum number of lines in messages */
    byte fill_1[3];
    /* 112 */
    word Zone;
    word Net;
    word Node;
    word Point;
    byte Path_Len;          /* length of Path */
    byte Title_Len;         /* length of Title */
    byte Barricade_Len;     /* Barricade length if used */
    byte Origin_Len;        /* non-default Origin line */
    byte Domain_Len;        /* Domain (if used) length */
    byte Menu_Len;          /* file name for this area's ascii menu */
    byte Vol_Len;           /* Length of volume name */
    byte Help_Len;          /* Length of help name for this area */
    /* 144 */
    byte Scan_Len;          /* number of boards that Area is Scanned to */
    byte Scan_Pos;          /* where Opus is in the Scan right now */
    word Other_Len;         /* external programs use this number bytes */
    word Extern_Flags;
    word word_fill[5];
    /* 160 */
};

/*--------------------------------------------------------------------------*/
/* Then some strings/structures follow as needed                            */
/*--------------------------------------------------------------------------*/
/* Path string  */
/* Title string */
/* Barricade string */
/* Origin string */
/* Domain string */
/* Menu string */
/* Message area help file */
/* Volume ID */
/* aScan[Scan_Len] echoScan addresses */
/*  "Other" field. Reserved for external programs */
/*--------------------------------------------------------------------------*/
/* end of strings                                                           */
/*--------------------------------------------------------------------------*/
struct  _ascan          /* structure of address for echoScanning */
{
    word Net;
    word Node;
};

/*--------------------------------------------------------------------------*/
/* LMR is used in LREAD.DAT to store the user's last message read counter   */
/*--------------------------------------------------------------------------*/
struct  _lmr
{
    uint32_t user_id;
    word     high_msg;
    word     last_msg;
};

/*--------------------------------------------------------------------------*/
/* LF is used to store the last time a user has accessed each area          */
/*--------------------------------------------------------------------------*/
struct  _lf
{
    uint32_t user_id;
    uint32_t last_timestamp;
};

/*--------------------------------------------------------------------------*/
/* File Area attributes (limit or describe the behavior of an area)         */
/*--------------------------------------------------------------------------*/

#define    F_DUPE 0x0001        /* Allow duplicate uploads in this area      */
#define    F_FREE 0x0002        /* Downloads from this area area all 0K      */
/*--------------------------------------------------------------------------*/
/* Area attributes (limit or describe the behavior of an area)              */
/*--------------------------------------------------------------------------*/
#define  SYSMAIL 0x01      /* is a mail area                                */
#define  P_REP 0x02        /* Net mail private echomail back                */
#define  NOPUBLIC 0x04     /* OPUS: Disallow public messages                */
#define  NOPRIVATE 0x08    /* OPUS: Disallow private messages               */
#define  ANON_OK 0x10      /* OPUS: Enable anonymous messages               */
#define  _ECHOMAIL 0x20     /* OPUS: Set=Echomail Clear=Not Echomail         */
#define  USEALIAS 0x40     /* OPUS: Use user's alias in this area if.....   */
//#define  PASSTHROUGH   0x80     /* OPUS: Allow high-bit characters in echo area  */
#define  INBOUND 0x0100         /* Inbound Only area */
/*--------------------------------------------------------------------------*/
/* msgsys.Extern_Flags                                                      */
/* Opus will not use these flags, set them or anything. The ones that are   */
/* marked as RESERVED will be defined in the future for "standard" uses.    */
/* The ones marked as EXTERN will have no "standard" meaning so conflicting */
/* programs MAY have conflicting uses.                                      */
/*--------------------------------------------------------------------------*/

#define RENUMBER 0x0001     /* This area gets renumbered                 */
#define MAILCHECK 0x0002    /* The mail check program will do this area  */
#define RESERVED2 0x0004    /* Reserved for Opus-defined flags           */
#define RESERVED3 0x0008    /* Reserved for Opus-defined flags           */
#define RESERVED4 0x0010    /* Reserved for Opus-defined flags           */
#define RESERVED5 0x0020    /* Reserved for Opus-defined flags           */
#define RESERVED6 0x0040    /* Reserved for Opus-defined flags           */
#define RESERVED7 0x0080    /* Reserved for Opus-defined flags           */
#define EXTERN1 0x0100      /* Undefined! Meaning varies for each program*/
#define EXTERN2 0x0200      /* Undefined! Meaning varies for each program*/
#define EXTERN3 0x0400      /* Undefined! Meaning varies for each program*/
#define EXTERN4 0x0800      /* Undefined! Meaning varies for each program*/
#define EXTERN5 0x1000      /* Undefined! Meaning varies for each program*/
#define EXTERN6 0x2000      /* Undefined! Meaning varies for each program*/
#define EXTERN7 0x4000      /* Undefined! Meaning varies for each program*/
#define EXTERN8 0x8000      /* Undefined! Meaning varies for each program*/
//  ------------------------------------------------------------------

#if defined (GOLD_CANPACK)
    #pragma pack()
#endif
//  ------------------------------------------------------------------
