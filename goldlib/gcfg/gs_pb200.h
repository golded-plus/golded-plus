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
/*
                     旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커
                     � ProBoard v2.00 File Structures �
                     읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸


   This document describes the file structures used by ProBoard version 2.00.
   For information on the structures of the RemoteAccess compatible files, refer
   to the RemoteAccess structures document. It can be obtained from any of the
   ProBoard distribution sites.

   -------------------------------------------------------------------------------

   The data-structures are C-structures, this means:

   - Strings are stored as an array of characters, terminated by a 0.
   - A date is stored as 3 bytes (day,month,year)
   - A time is stored as 3 bytes (hour,min,sec)

   The 'accessflags' are stored in a long integer, with bit 31 being flag 'A',
   bit 30 flag 'B' , ... Bit 5 is flag '1', bit 4 is flag '2', etc...
   I know this doesn't sound logical, but we had some good reasons for doing
   this <grin>.

   The combined boards are stored in 25 bytes (200 bits). One bit per message
   area.
 */
typedef uint32_t accessflags;
typedef uint8_t  combinedboards[125];
typedef uint8_t  pb_bool;
typedef uint8_t  pb_Time[3];
typedef uint8_t  Date[3];
typedef uint8_t  TimeFrame[7][6];
/*
   컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
   USERSPB.BBS
   컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

   This file is tied to the files USERS.BBS and USERSXI.BBS. The records are
   stored in the same order as the records in USERS.BBS. The name of the user
   is duplicated in this file, so it is possible to fix the userfile after using
   a third-party userfile packer/sorter, which does not know about USERSPB.BBS.

 */
struct UsersPbBBS
{
    char           name[36];
    char           country[26];
    char           state[26];
    char           faxPhone[16];
    char           passWord[16];
    char           language[9];
    Date           lastPwdChange;
    Date           lastNewFilesCheck;
    short          logLevel;
    short          tbTimeBalance;
    short          tbKbBalance;
    short          tbTimeWithdrawn;
    short          tbKbWithdrawn;
    uint           tbTimeDeposited;
    uint           tbKbDeposited;
    uint           tbTimeLoaned;
    uint           tbKbLoaned;
    Date           tbTimePayback;
    Date           tbKbPayback;
    Date           tbLastUsed;
    uint           expLevel;
    accessflags    expFlagsOn;
    accessflags    expFlagsOff;
    uint32_t       uFlags;
    combinedboards mailCheckBoards;
    uint32_t       totalTimeUsed;
    uint           qwkMaxMsgsPerArea;
    uint           qwkMaxMsgs;
    byte           qwkArchiver;
    byte           ripFont;
    byte           checkMail;
    byte           checkNewFiles;
    byte           extra[398];
};

/* These are the extra user-flags for 'uFlags': */

#define RA_UFLAG_NOIBM 1
#define RA_UFLAG_NOTOPS 2
#define RA_UFLAG_AVTPLUS 4
#define RA_UFLAG_ATTEN 8
#define RA_UFLAG_NORIP 16
#define RA_UFLAG_MULTILOGIN 32
#define RA_UFLAG_FREECHAT 64
#define RA_UFLAG_LOCALONLY 128
/*
   컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
   CONFIG.PRO
   컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
 */
struct Config
{
    char        shellmsg[81];    /* Message to show when shelling                */
    char        sysopname[36];   /* Name of sysop                                */
    char        txtpath[61];     /* Path for textfiles                           */
    char        mnupath[61];     /* Path for menu-files                          */
    char        msgpath[61];     /* Path for message base                        */
    char        uploadpath[61];  /* Uploadpath                                   */
    char        editorname[61];  /* Name of external editor                      */
    uint        newuserlevel;    /* Level for new user                           */
    short       newuserloglevel; /* Loglevel for new user                        */
    accessflags newuserflags;  /* New user flags                               */
    short       max_passinput;   /* Maximum attempts for password entry          */
    short       min_passlength;  /* Minimum password length                      */
    short       inactivity_time; /* Inactivity time-out limit                    */
    short       max_sysop_pages; /* Maximum times sysop can be paged             */
    short       pagebell_length; /* Length of page-bell (secs)                   */
    short       mailcheck;       /* Check for mail at logon?                     */
    short       europe;          /* European date format?                        */
    short       numnodes;        /* # nodes                                      */
    pb_bool     allowansi;       /* Allow ANSI?                                  */
    pb_bool     allowavatar;     /* Allow AVATAR?                                */
    short       discrete;        /* Hide sysop activity?                         */
    short       askphone;        /* Ask for phone number?                        */
    short       allowoneword;    /* Allow one-word names                         */
    unsigned    crashlevel;      /* Level needed for crashmail                   */
    accessflags crashflags;    /* Flags needed for crashmail                   */
    uint        attachlevel;     /* Level needed for file attach                 */
    accessflags attachflags;   /* Flags needed for file attach                 */
    short       allowmsgupload;  /* Allow message uploads                        */
    short       allowstacking;   /* Allow command stacking                       */
    pb_Time     page_start;      /* Paging hours start                           */
    pb_Time     page_end;        /* Paging hours end                             */
    short       handshaking;     /* I/O Handshaking                              */
    short       allowalias;      /* Allow alias for login                        */
    short       loglocal;        /* Log local calls                              */
    short       doswap;          /* Allow swapping                               */
    char        originline[61];  /* Origin line                                  */
    char        nodelistdir[61]; /* Nodelist directory                           */
    char        sysopkeys[10][60]; /* Sysop hotkeys                                */
    pb_Time     dl_start;        /* Download hours start                         */
    pb_Time     dl_end;          /* Download hours end                           */
    short       uploadspace;     /* Space needed for uploads                     */
    char        pvtuploadpath[61]; /* Directory for files uploads                  */
    char        quotestring[6];  /* String used for quoting                      */
    pb_bool     fastmode;        /* Use fast mode                                */
    pb_bool     extra_bool_1;
    pb_bool     killsent;        /* Kill netmail after sent                      */
    pb_bool     multiline;       /* Use message base sharing?                    */
    pb_bool     egamode;         /* Use 43/50 line mode                          */
    pb_bool     showuserinfo;    /* Show user info while in EGA mode?            */
    char        pexpath[61];     /* Directory for PEX files                      */
    pb_bool     allowquicklogin; /* Allow quick sysop login?                     */
    pb_bool     suspendmsgtime;  /* Suspend time when writing msgs               */
    short       securityboard;   /* MsgBoard for security messages               */
    pb_bool     pwdmessages;     /* Write security-messages?                     */
    pb_bool     extra_bool_2;
    char        bbsname[36];     /* Name of the BBS                              */
    char        pwdchar;         /* Password character                           */
    short       tb_maxtimedeposit; /* Max time deposit per day (TimeBank)          */
    short       tb_maxkbdeposit; /* Max Kbytes deposit per day (TimeBank)        */
    short       tb_maxtimewithdrawal; /* Max time withdrawal per day (TimeBank)       */
    short       tb_maxkbwithdrawal; /* Max Kbytes withdrawal per day (TimeBank)     */
    short       usage_days;      /* Days to keep usage graphs                    */
    char        systempwd[16];   /* System password                              */
    pb_bool     usesystempwd;    /* Use system password?                         */
    pb_bool     askbirthdate;    /* Ask Birth Date?                              */
    short       binlogdays;      /* # days to log in BINLOG.PB                   */
    pb_bool     binloglocal;     /* Log local calls to BINLOG.PB yes/no          */
    short       pageArea;        /* Area number for page messages                */
    pb_bool     indexfiles;      /* Use file indexing                            */
    pb_bool     checkdupes;      /* Check for dupes                              */
    pb_bool     killdupes;       /* Kill duplocate files                         */
    pb_bool     ignore_ext;      /* Ignore file extensions for dupe checking     */
    char        RIPpath[61];     /* Path for RIP scripts                         */
    char        iconspath[61];   /* Path for RIP icons                           */
    char        location[36];    /* BBS Location (city)                          */
    char        phone[26];       /* BBS Phone #                                  */
    char        QWKid[9];        /* BBS QWK id                                   */
    uint        IObuffersize;    /* I/O buffer size in bytes                     */
    TimeFrame   pagingHours;     /* Paging hours                                 */
    char        defaultLanguage[9]; /* Default language                             */
    pb_bool     addUploaderName; /* Add uploader's name to FILES.BBS             */
    TimeFrame   downloadHours; /* Download hours                               */
    pb_bool     askdataphone;    /* Ask data phone #                             */
    pb_bool     askfaxphone;     /* Ask fax phone #                              */
    pb_bool     askaddress;      /* Ask mailing address                          */
    pb_bool     asksex;          /* Ask sex                                      */
    pb_bool     askdateformat;   /* Ask date format                              */
    pb_bool     askstate;        /* Ask state                                    */
    pb_bool     askcountry;      /* Ask country                                  */
    short       fuzzyRate;       /* Fuzzy search percentage for user editor      */
    pb_bool     hidePassword;    /* Hide password in user editor                 */
    pb_bool     valConfirm;      /* Confirm user validation                      */
    char        extra[272];
};

/*
   컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
   TIMELOG.PRO
   컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
 */
struct TimeLog
{
    short numdays;         /* Number of days active                         */
    Date  lastdate;        /* Last update                                   */
    long  hours[24];       /* # minutes per hour usage (obsolete)           */
    long  days[7];         /* # minutes per day usage  (obsolete)           */
    long  weeks[53];       /* # minutes per week usage (obsolete)           */
    long  totalcalls;      /* Total calls to system                         */
};

/*
   컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
   FILECFG.PRO
   컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
 */
struct FileCfg
{
    char    name[80];    /* Name of file area                                */
    char    listpath[80]; /* Path for file-list                               */
    char    filepath[80]; /* Path for files                                   */
    uint    level;       /* Level needed for access                          */
    long    flags;       /* Flags needed for access                          */
    char    type;        /* 1 = CDROM File listing type                      */
    short   maxfiles;    /* Max files per day in this area downloadable      */
    short   maxkb;       /* Max Kbytes per day in this area                  */
    pb_bool notops;      /* Set to 1 -> should not be listed in TOPFILES.A?? */
    pb_bool free;        /* Set to 1 -> free area                            */
    byte    groups[4];   /* Groups belonging to                              */
    pb_bool allGroups;   /* Belongs to all groups                            */
    byte    minAge;      /* Minimum age                                      */
    long    flagsNot;    /* Access flags not allowed                         */
    byte    extra[3];
};

/*
   컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
   MSGAREAS.PB
   컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
 */
struct MsgAreas
{
    short       areanum;       /* # of message area (1-10000)                 */
    char        name[31];      /* Name of message areas                       */
    byte        msgtype;       /* Type of messages                            */
    byte        msgkind;       /* Kind of message area                        */
    byte        msgbasetype;   /* 0 = hudson , 1 = squish , 2 = *.MSG         */
    char        path[80];      /* Path/Directory for Squish / *.MSG           */
    byte        flags;         /* Alias allowed/forced/prohibited             */
    uint        readlevel;     /* Minimum level needed to read msgs           */
    accessflags readflags;     /* flags needed to read msgs                   */
    uint        writelevel;    /* Minimum level needed to write msgs          */
    accessflags writeflags;    /* flags needed to write msgs                  */
    uint        sysoplevel;    /* Minimum level needed to change msgs         */
    accessflags sysopflags;    /* flags needed to change msgs                 */
    char        origin[62];    /* Origin line                                 */
    short       aka;           /* AKA                                         */
    short       rcv_kill_days; /* Kill received after xx days                 */
    short       msg_kill_days; /* Kill after xx days                          */
    short       max_msgs;      /* Max # msgs                                  */
    char        sysop[36];     /* Area Sysop                                  */
    short       replyboard;    /* Reply area # (0=here)                       */
    char        echotag[21];   /* Echomail Tag Name                           */
    char        qwktag[13];    /* QWK Area Name                               */
    byte        groups[4];     /* Groups belonging to                         */
    pb_bool     allGroups;     /* Belongs to all groups                       */
    byte        minAge;        /* Minimum age for this area                   */
    long        readFlagsNot;  /* flags not allowed for reading               */
    long        writeFlagsNot; /* flags not allowed for writing               */
    long        sysopFlagsNot; /* flags not allowed for sysop access          */
    byte        extra[5];
};

#define MSGTYPE_BOTH 0     /* Private/Public */
#define MSGTYPE_PVT 1      /* Private Only   */
#define MSGTYPE_PUBLIC 2   /* Public Only    */
#define MSGTYPE_TOALL 3    /* To All         */

#define MSGKIND_LOCAL 0    /* Local          */
#define MSGKIND_NET 1      /* NetMail        */
#define MSGKIND_ECHO 2     /* EchoMail       */
#define MSGKIND_PVTECHO 3  /* Pvt EchoMail   */

#define MSGBASE_HUDSON 0
#define MSGBASE_SQUISH 1
#define MSGBASE_SDM 2
#define MSGBASE_JAM 3
/*
   컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
   ONLINE.PRO
   컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
 */
struct Online
{
    char  name[36];    /* Name of user online                                 */
    char  city[16];    /* City of user online                                 */
    short baud;        /* Baud rate of user                                   */
    short status;      /* 0 -> online , <0 -> not online or unavailable       */
    char  extra[8];
};

/*
   컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
   PROTOCOL.PRO
   컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
 */
struct Protocol
{
    char  name[50];      /* Name of protocol                                  */
    char  key;           /* Hotkey for Protocol                               */
    char  flags;         /* Protocol behavior                                 */
    char  logfile[60];   /* Logfile from protocol                             */
    char  ctlfile[60];   /* Control file (redirection file)                   */
    char  dlcom[128];    /* Download command line                             */
    char  ulcom[128];    /* Upload command line                               */
    char  dlstring[80];  /* String to place in download control file          */
    char  ulstring[80];  /* String to place in upload control file            */
    char  dlkeyword[40]; /* Download keyword to search for in logfile         */
    char  ulkeyword[40]; /* Upload keyword to search for in logfile           */
    short wordnr;          /* File name word nr after keyword (1-..)            */
};

/* Flags: */

#define PROT_BATCH 1      /* Batch protocol        */
#define PROT_ENABLED 2    /* Enabled               */
#define PROT_BOTH 4       /* Full-duplex protocol  */
#define PROT_BIM 8        /* Bimodem-type ctl-file */
/*
   컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
   LIMITS.PRO
   컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
 */
struct Limits
{
    uint  level;           /* Level                                           */
    short timelimit;       /* # minutes per day                               */
    short daily_klimit;    /* Kbytes per day allowed                          */
    short pre_download;    /* # minutes online before download                */
    char  id[6];           /* Usergroup ID                                    */
    uint  free;            /* Free upload in Kb.                              */
    byte  factor;          /* Percentage upload required                      */
    uint  max_download;    /* Max download for this level                     */
    short fallto;          /* Fall to level x when max. reached               */
    short msgfactor;       /* # Kbytes granted per message written            */
    char  extra[5];
};

/*
   컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
   MODEM.PRO
   컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
 */
struct MODEM_PRO
{
    unsigned baud;
    short    port;
    short    quiet;
    short    blanktime;
    char     initstring[70];
    char     busystring[70];
    char     initok[40];
    char     busyok[40];
    char     connect300[40];
    char     connect1200[40];
    char     connect2400[40];
    char     connect4800[40];
    char     connect9600[40];
    char     connect19200[40];
    char     connect38400[40];
    char     ring[40];
    char     answer[40];
    char     connect7200[40];
    char     connect12000[40];
    pb_bool  forceanswer;
    byte     extra1[5];
    short    delay;
    char     connect14400[40];
    char     initstring2[70];
    char     connect16800[40];
    char     connect28800[40];
    char     ext_connect1[40];
    char     ext_connect2[40];
    short    ext_errlevel1;
    short    ext_errlevel2;
    char     extra[36];
};

/*
   컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
   BINLOG.PB
   컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
 */
struct BinLog
{
    Date     date;
    pb_Time  timeIn;
    pb_Time  timeOut;
    char     name[36];
    char     city[26];
    char     country[26];
    long     baud;
    uint     node;
    long     kbDown;
    long     kbUp;
    uint     yells;
    uint     level;
    uint32_t uflags;
    char     extra[81];
};

/*
   컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
   FILESIDX.PB
   컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
 */
struct FilesIdx
{
    char filename[13];        /* Name of the file, with extension       */
    uint area;                /* File area number where file is located */
};

/*
   컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
   FGROUPS.PB / MGROUPS.PB
   컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
 */
struct xGroups
{
    char name[80];
    uint level;
    long flags;
    long flagsNot;
    byte extra[10];
};

//  ------------------------------------------------------------------

#if defined (GOLD_CANPACK)
    #pragma pack()
#endif
//  ------------------------------------------------------------------
