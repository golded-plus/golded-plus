//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1989-1990 Joaquim H. Homrighausen
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
//  Definitions for FrontDoor 1.99c and above
//  ------------------------------------------------------------------

#ifndef __gs_fd_h
#define __gs_fd_h


//  ------------------------------------------------------------------

#include <gdefs.h>


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif


//  ------------------------------------------------------------------
//  Actually only size of the following structure used

typedef struct {

  // The logfile

  char    log[71];
  word    loglevel;                   // See --- Loglevels

  // Number manipulation

  char    prefix[31];                 // Always added
  char    hidden[10][31];             // Strip these if they are in #
  char    postfix[31];                // Always appended

  // Miscellaneous flags

  long    flags;                      // See --- Miscellaneous flags
  long    flags_reserved;             // Reserved flags
  word    audio;                      // See --- Audio flags
  byte    synchtimer;                 // Number of seconds for sync

  // Errorlevels

  byte    crashexit;                  // Mail exit
  byte    bbs300;
  byte    bbs1200;
  byte    bbs1275;
  byte    bbs2400;
  byte    bbs4800;
  byte    bbs9600;
  byte    bbs19200;
  byte    bbs38400;

  // Modem

  word    modembaud;                  // 30=300, 24=2400, etc.
  byte    modemport;                  // 1-255 (COM1=1, COM2=2, etc.)
  byte    modemdelay;                 // 1/10 seconds delay / line sent

  // Messages

  char    b300msg[16];
  char    b1200msg[16];
  char    b1275msg[16];
  char    b2400msg[16];
  char    b4800msg[16];
  char    b9600msg[16];
  char    b19200msg[16];
  char    b38400msg[16];
  char    errormsg[16];
  char    busymsg[16];
  char    carriermsg[16];
  char    okmsg[16];
  char    ringmsg[16];
  char    nodialmsg[16];
  char    noanswmsg[16];
  char    voicemsg[16];

  // Commands

  char    escapestr[11];
  char    offhookstr[11];
  char    reconnectstr[11];
  char    init1[50];
  char    init2[50];
  char    init3[50];
  char    resetstr[50];
  char    downstr[50];
  char    hangupstr[11];
  char    dialstr[11];

  // Manual answer stuff

  char    modemanswer[11];
  byte    answerdelay;

  // Limited answer start and end times

  byte    begin_hour;
  byte    begin_minute;
  byte    end_hour;
  byte    end_minute;

  // Calling control

  byte    retrybusy;
  byte    retryresend;
  byte    retrydelay;

  // File request control

  char    reqlist[71];            // List to scan for reqable dirs
  char    reqalias[71];           // Magic filenames
  char    reqmessage[71];         // Appended to FAILED REQUEST message
  byte    reqtype;                // Bit field
  byte    reqmaxfiles;            // Max number of files to send on 1 req
  word    reqmaxtime;             // Maximum number of minutes for req
  word    reqmaxsize;             // Maximum size (in KB) for req
  word    reqminbaud;             // Minimum baudrate for req
  byte    reqstarthr;             // Start time for file requests, can be
  byte    reqstartmin;            // combined with the reqdays field.
  byte    reqendhr;
  byte    reqendmin;
  byte    reqdays;

  // File to send when human callers are let thru

  char    bbsname[11];
  char    beforebbsbanner[71];

  // Function keys from mailer menu

  struct {
    char cmd[61];
    char title[26];
    byte behavior;
  }       key[24];

  // Mailer colors

  byte    color[11];

  // Number of days to keep entries in history files

  byte    keep_history;

  // FDServer password, if none given, server is INactive

  char    slavepwd[21];

  // File displayed to users when system is in event for no callers

  char    ineventfile[71];

  // File displayed when human callers are seen on mail-only system

  char    mailonlyfile[71];

  // External programs to run on certain "wake-up" strings

  struct {
    char wakeupstr[40];
    byte errorlevel;
  }       externmail[10];

  // RESERVED FIELD

  // Limited audio start and end times. If the below four bytes
  // are all zero (0), audio is enabled all the time

  byte    audio_begin_hour;
  byte    audio_begin_minute;
  byte    audio_end_hour;
  byte    audio_end_minute;

  // Minimum cost for system to possibly end up in undialable list

  word    min_undial_cost;

  char    RESERVERAT[1018];

} FD_Mailer;


//  ------------------------------------------------------------------

//  Netmail folder behavior

#define RESTRICTED      0x00000001L
#define EXPORTOK        0x00000004L
#define USEXLATTABLES   0x00000008L
#define EDREADONLY      0x00000020L

#define MSGPRIVATE      0x0001
#define MSGCRASH        0x0002
#define MSGREAD         0x0004
#define MSGSENT         0x0008
#define MSGFILE         0x0010
#define MSGTRANSIT      0x0020
#define MSGORPHAN       0x0040
#define MSGKILL         0x0080
#define MSGLOCAL        0x0100
#define MSGHOLD         0x0200
#define MSGUNUSED       0x0400
#define MSGFREQ         0x0800
#define MSGRRREQ        0x1000
#define MSGISRR         0x2000
#define MSGAREQ         0x4000
#define MSGFUPDREQ      0x8000

typedef struct {

  // Macro keys

  char    macrokey[24][61];

  // Margin, default==60

  byte    margin;

  // Default message status

  word    msgbits;

  // Miscellaneous settings

  long    flags;

  // Origin lines

  char    origin[20][61];

  // Editor colors

  byte    color[15];

  // Netmail folder flags

  long    netfolderflags;

  // Translation tables IN/OUT

  byte    translate_in[256];
  byte    translate_out[256];

  // Where RemoteAccess/QuickBBS message base files are

  char    qbase[71];

  // RESERVED

  char    RESERVERAT[1024];

} FD_Editor;


//  ------------------------------------------------------------------

typedef struct {

  char    systempath[71];
  char    mailpath[71];
  char    swap_path[71];
  char    RESERVED_path_1[71];
  char    RESERVED_path_2[71];
  char    infilepath[71];
  char    packetpath[71];
  char    nodelistpath[71];

  word    countrycode;

  ftn_addr   aka[11];

  // Timeout value for screen blanker in SECONDS (0-255)

  long    flags;
  byte    blackout_time;

  // User record

  struct {
    char  name[37];
    long  pwdcrc; // Crc-32 of user password, -1L No pwd
    dword flags;
  }       user[10];

  char    RESERVED[1024];

} FD_Shared;


//  ------------------------------------------------------------------

//  Constant long bit values

#define FOLDER_RESTRICT     0x00000001L  // Restricted folder
#define FOLDER_ECHOINFO     0x00000002L  // Add Origin: information
#define FOLDER_EXPORTOK     0x00000004L  // OK for user to export from folder
#define FOLDER_USEXLAT      0x00000008L  // Use translation tables
#define FOLDER_PRIVATE      0x00000010L  // Add Private message status
#define FOLDER_READONLY     0x00000020L  // Folder is read-only
#define FOLDER_FORCEHARDCR  0x00000040L  // Force hard CRs at linebreak (C)
#define FOLDER_JAM          0x01000000L  // JAM Message Base folder
#define FOLDER_NOCHECK      0x02000000L  // Exclude from new mail check (C)
#define FOLDER_NETMAIL      0x08000000L  // Netmail-type folder (C)
#define FOLDER_HMB          0x10000000L  // Hudson Message Base folder
#define FOLDER_DELETED      0x20000000L  // Never written to disk
#define FOLDER_LOCAL        0x40000000L  // Local-type folder
#define FOLDER_ECHOMAIL     0x80000000L  // Conference-type folder

//  User access mask

#define FOLDER_USER_1       0x00000001L
#define FOLDER_USER_2       0x00000002L
#define FOLDER_USER_3       0x00000004L
#define FOLDER_USER_4       0x00000008L
#define FOLDER_USER_5       0x00000010L
#define FOLDER_USER_6       0x00000020L
#define FOLDER_USER_7       0x00000040L
#define FOLDER_USER_8       0x00000080L
#define FOLDER_USER_9       0x00000100L
#define FOLDER_USER_10      0x00000200L

typedef struct {

  char    path[65],       // Path if "board==0", otherwise emtpy
          title[41];      // Title to appear on screen
  byte    origin;         // Default origin line, 0-19
  long    behave;         // Behavior, see above
  long    pwdcrc;         // CRC32 of password or -1L if unprotected
  long    userok;         // Users with initial access
  byte    useaka;         // AKA to use, 0==primary
  word    board;          // QuickBBS/RemoteAccess board number

} FD_Folder, *FOLDERPTR;


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack()
#endif


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
