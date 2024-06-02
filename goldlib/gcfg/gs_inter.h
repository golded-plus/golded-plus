//  This may look like C code, but it is really -*- C++ -*-
//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1989-1994 Peter Stewart & InterZone Software, inc.
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
//  FD.SYS definitions for InterMail 2.2x
//  ------------------------------------------------------------------

#ifndef __gs_inter_h
#define __gs_inter_h
//  ------------------------------------------------------------------

#include <gftnall.h>
//  ------------------------------------------------------------------

#if defined (GOLD_CANPACK)
    #pragma pack(1)
#endif
//  ------------------------------------------------------------------
typedef struct
{
    /* The logfile */
    char log[71];
    word loglevel;                    /* See --- Loglevels */
    /* Number manipulation */
    char prefix[31];                  /* Always added */
    char hidden[10][31];              /* Strip these if they are in # */
    char postfix[31];                 /* Always appended */
    /* Miscellaneous flags */
    long flags;                       /* See --- Miscellaneous flags */
    long flags_reserved;              /* Reserved flags */
    word audio;                       /* See --- Audio flags */
    byte synchtimer;                  /* Number of seconds for sync */
    /* Errorlevels */
    byte crashexit;                   /* Mail exit */
    byte bbs300;
    byte bbs1200;
    byte bbs1275;
    byte bbs2400;
    byte bbs4800;
    byte bbs9600;
    byte bbs19200;
    byte bbs38400;
    /* Modem */
    word modembaud;                   /* 30=300, 24=2400, etc. */
    byte modemport;                   /* 1-255 (COM1=1, COM2=2, etc.) */
    byte modemdelay;                  /* 1/10 seconds delay / line sent */
    /* --- Messages */
    char b300msg[16];
    char b1200msg[16];
    char b1275msg[16];
    char b2400msg[16];
    char b4800msg[16];
    char b9600msg[16];
    char b19200msg[16];
    char b38400msg[16];
    char errormsg[16];
    char busymsg[16];
    char carriermsg[16];
    char okmsg[16];
    char ringmsg[16];
    char nodialmsg[16];
    char noanswmsg[16];
    char voicemsg[16];
    /* --- Commands */
    char escapestr[11];
    char offhookstr[11];
    char reconnectstr[11];
    char init1[50];
    char init2[50];
    char init3[50];
    char resetstr[50];
    char downstr[50];
    char hangupstr[11];
    char dialstr[11];
    /* --- Manual answer stuff */
    char modemanswer[11];
    byte answerdelay;
    /* ------ Limited answer start and end times */
    byte begin_hour;
    byte begin_minute;
    byte end_hour;
    byte end_minute;
    /* Calling control */
    byte retrybusy;
    byte retryresend;
    byte retrydelay;
    /* File request control */
    char reqlist[71];                 /* List to scan for reqable dirs    */
    char reqalias[71];                /* Magic filenames                  */
    char reqmessage[71];              /* Appended to FAILED REQUEST message */
    byte reqtype;                     /* Bit field                        */
    byte reqmaxfiles;                 /* Max number of files to send on 1 req*/
    word reqmaxtime;                  /* Maximum number of minutes for req*/
    word reqmaxsize;                  /* Maximum size (in KB) for req     */
    word reqminbaud;                  /* Minimum baudrate for req         */
    byte reqstarthr;                  /* Start time for file requests, can be*/
    byte reqstartmin;                 /* combined with the reqdays field. */
    byte reqendhr;
    byte reqendmin;
    byte reqdays;
    /* File to send when human callers are let thru */
    char bbsname[11];
    char beforebbsbanner[71];
    /* Function keys from mailer menu */
    struct
    {
        char cmd[61];
        char title[26];
        byte behavior;             /* 1-Pause, 2-Process msg base */
    } key[24];
    /* Mailer colors */
    byte color[11];
    /* Number of days to keep entries in history files */
    byte keep_history;
    /* FDServer password, if none given, server is INactive */
    char slavepwd[21];
    /* File displayed to users when system is in event for no callers */
    char ineventfile[71];
    /* File displayed when human callers are seen on mail-only system */
    char mailonlyfile[71];
    /* External programs to run on certain "wake-up" strings */
    struct
    {
        char wakeupstr[40];
        byte errorlevel;
    } externmail[10];
    /* RESERVED FIELD */
    /* --- Limited audio start and end times. If the below four bytes
           are all zero (0), audio is enabled all the time */
    byte audio_begin_hour;
    byte audio_begin_minute;
    byte audio_end_hour;
    byte audio_end_minute;
    /* ---    Minimum cost to process undialable */
    word min_undial_cost;
    char RESERVERAT[886];
    char extrnreq[71];
    char modem_name[61];                /* modem name, for 'modem selection' */
} _mailer;
/* Netmail folder behavior */

#define RESTRICTED 0x00000001L
#define EXPORTOK 0x00000004L
#define USEXLATTABLES 0x00000008L
#define EDREADONLY 0x00000020L
/* Echomail flags */

#define SAVEBAD 0x00000001L

typedef struct
{
    char path[65];         /* Path if "board==0", otherwise empty (65) */
    byte ftype;            /* Folder type                              */
    char areatag[39];      /* Echomail area tag                        */
    byte origin;           /* Default origin line, 0-19                */
    char title[41];        /* Title to appear on screen                */
    byte useaka;           /* AKA to use, 0==primary                   */
    word board;            /* QuickBBS/RemoteAccess/WC board number    */
    word upzone;           /* Uplink zone                              */
    word upnet;            /* Uplink net                               */
    word upnode;           /* Uplink node                              */
    word uppoint;          /* Uplink point                             */
    long behave;           /* Behavior, see above                      */
    long hiwater;          /* Highwater mark for echomail              */
    long pwdcrc;           /* CRC32 of password or -1L if unprotected  */
    long userok;           /* Users with initial access                */
    long accflags;         /* access flags, for network environment    */
    char reserved[8];      /* for future expansion                     */
} _eFOLDER;

#define MSGPRIVATE 0x0001
#define MSGCRASH 0x0002
#define MSGREAD 0x0004
#define MSGSENT 0x0008
#define MSGFILE 0x0010
#define MSGTRANSIT 0x0020
#define MSGORPHAN 0x0040
#define MSGKILL 0x0080
#define MSGLOCAL 0x0100
#define MSGHOLD 0x0200
#define MSGUNUSED 0x0400
#define MSGFREQ 0x0800
#define MSGRRREQ 0x1000
#define MSGISRR 0x2000
#define MSGAREQ 0x4000
#define MSGFUPDREQ 0x8000

typedef struct
{
    /* Macro keys */
    char macrokey[24][61];            /* F1-F12, Shift F1-F12 */
    /* Margin, default==60 */
    byte margin;
    /* Default message status */
    word msgbits;
    /* Miscellaneous settings */
    long flags;
    /* Origin lines */
    char origin[20][61];
    /* Editor colors */
    byte color[15];
    /* Netmail folder flags */
    long netfolderflags;
    /* Translation tables IN/OUT */
    byte translate_in[256];
    byte translate_out[256];
    /* Where RemoteAccess/QuickBBS message base files are */
    char qbase[71];
    char WCmain[71];
    /* RESERVED */
    char     RESERVERAT[255];
    long     echoflags;
    _eFOLDER BBSnet;
    _eFOLDER dupes;
    _eFOLDER badecho;
    char     echolog[65];
    char     IMEWork[71];
} _editor;
typedef struct
{
    char     systempath[71];
    char     mailpath[71];
    char     swap_path[71];
    char     semaphore[71];
    char     secfilespath[71];
    char     infilepath[71];
    char     packetpath[71];
    char     nodelistpath[71];
    word     countrycode;
    ftn_addr aka[11];
    /* Timeout value for screen blanker in SECONDS (0-255) */
    long flags;
    byte blackout_time;
    /* User record */
    struct
    {
        char  name[37];
        long  pwdcrc;           /* Crc-32 of user password, -1L No pwd */
        dword flags;
    } user[10];
    /* Protection of exits */
    dword exitpwdcrc;           /* Password for DOS shell, exits, etc. */
    dword exitflags;            /* Which flags should be protected */
    char  filler[760];          /* contains domain stuff */
    char  systemname[50];       /* site_info - system name */
    char  systemloc[40];        /* site_info - location */
    char  systemphone[24];      /* site_info - phone */
    char  fill[6];              /* 6 loose bytes :) */
    char  systemflags[20];      /* site_info - nodelist flags */
    char  systemcountry[26];      /* site_info - country */
    char  serial[14];           /* serial number */
    char  outecho[66];          /* outbound echomail packets */
    char  reserved[10];         /* reserved space */
} _shared;
typedef struct
{
    char     initstring[41];    /* Init string                         */
    word     scrollsize;        /* Max memory to use for buffer (in K) */
    byte     emulation;         /* 0=TTY, 1=ANSI, 2=VT52, 3=VT100      */
    byte     protocol;          /* Index in protocol list              */
    char     shiftkey[12][31];    /* Shift F1-F12 macro settings         */
    char     ctrlkey[12][31];   /* Ctrl  F1-F12 macro settings         */
    char     downloadpath[60];    /* Default download path               */
    char     uploadpath[60];    /* Default upload path                 */
    byte     translate_in[256];   /* Translation table - modem->screen   */
    byte     translate_out[256];  /* Translation table - screen->modem   */
    byte     retrywait;         /* Seconds to wait before next dial..  */
    dword    flags;             /* Behavior, sounds, flashes.. etc.    */
    dword    directorypwd;      /* CRC-32 of password to enter phoneDir*/
    char     editor[60];        /* Invoked with Alt-I                  */
    char     RESERVERAT[796];   /* Reserved space                      */
    ftn_addr newaka[21];
} _terminal;
typedef struct
{
    char port;           /* 0 LPT1, 1 LPT2, 2 LPT3, 3 COM1, 4 COM2 */
    char baud;           /* 0 9600, 1 4800, 2 2400, 3 1200 */
    byte stopbits;       /* 0x00 - 1, 0x01 - 2 */
    byte wordlength;     /* 0x00 - 7, 0x01 - 8 */
    byte parity;         /* 0x00 - Even, 0x01 - Odd, 0x02 - None */
    byte pagelen;
    long behavior;
    char init[71];
    char reset[71];
    char bold_on[31];
    char bold_off[31];
    char pr1RESERVED[62];
    char italic_on[31];
    char italic_off[31];
    byte pagewidth;               /* Width in columns of a page */
    byte leftmargin;              /* Left margin, ie. start printing at column*/
    byte footer;                  /* Footer margin, ie. leave nn lines */
    byte header;                  /* Header margin, ie. skip nn lines */
    byte translate_out[256];      /* Translation table - disk->printer */
    char pr2RESERVED[100];
} _printer;

#define IM_THISREV 0x0100

struct _ctl
{
    char      fingerprint[5];  /* Must contain "JoHo" */
    word      sysrev;          /* Must contain THISREV above */
    dword     ctlcrc;          /* CRC-32 of struct excluding the 1st 11 bytes */
    _mailer   m;
    _editor   e;
    _shared   s;
    _terminal t;
    _printer  p;
    dword     ctlcrc2;         /* CRC-32 of all the above */
};

//  Constant long bit values

#define RESTRICT 0x00000001L
#define ECHO_INFO 0x00000002L
#define EXPORT_OK 0x00000004L
#define USE_XLAT 0x00000008L
#define PRIVATE 0x00000010L
#define READONLY 0x00000020L
#define F_NETMAIL 0x08000000L
#define BOARDTYPE 0x10000000L
#define DELETED 0x20000000L             /* Never written to disk */
#define LOCAL 0x40000000L
#define ECHOMAIL 0x80000000L

#define F_MSG 0
#define F_HUDSON 1
#define F_WC35 2
#define F_PCB15 3
#define F_JAM 4
//  Folder structure
typedef struct
{
    char path[65];         /* Path if "board==0", otherwise empty (65) */
    byte ftype;            /* Folder type                              */
    char areatag[39];      /* Echomail area tag                        */
    byte origin;           /* Default origin line, 0-19                */
    char title[41];        /* Title to appear on screen                */
    byte useaka;           /* AKA to use, 0==primary                   */
    word board;            /* QuickBBS/RemoteAccess/WC conf number     */
    word upzone;           /* Uplink zone                              */
    word upnet;            /* Uplink net                               */
    word upnode;           /* Uplink node                              */
    word uppoint;          /* Uplink point                             */
    long behave;           /* Behavior, see above                      */
    long hiwater;          /* Highwater mark for echomail              */
    long pwdcrc;           /* CRC32 of password or -1L if unprotected  */
    long userok;           /* Users with initial access                */
    long accflags;         /* access flags, for network environment    */
    long timestamp;        /* Time stamp for detecting msg base updates*/
    char reserved[4];      /* for future expansion                     */
} FOLDER;
//  The following struct was used in IM 2.00-2.25, file name FOLDER.CFG.
typedef struct
{
    char path[65];         /* Path if "board==0", otherwise empty (65) */
    char title[41];        /* Title to appear on screen                */
    byte origin;           /* Default origin line, 0-19                */
    long behave;           /* Behavior, see above                      */
    long pwdcrc;           /* CRC32 of password or -1L if unprotected  */
    long userok;           /* Users with initial access                */
    byte useaka;           /* AKA to use, 0==primary                   */
    word board;            /* QuickBBS/RemoteAccess/WC board number    */
} OLDFOLDER;
//  ------------------------------------------------------------------

#if defined (GOLD_CANPACK)
    #pragma pack()
#endif
//  ------------------------------------------------------------------

#endif // ifndef __gs_inter_h
//  ------------------------------------------------------------------
