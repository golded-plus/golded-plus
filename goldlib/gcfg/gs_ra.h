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
//  RemoteAccess configuration data structures.
//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif

//  ------------------------------------------------------------------
//  RA Address structure

typedef struct {
  word  zone;
  word  net;
  word  node;
  word  point;
} _raaddr;


//  ------------------------------------------------------------------
//  RA Area info structure

typedef struct {
  char    name[41];         // 000
  char    type;             // 041
  byte    msgkinds;         // 042
  byte    attribute;        // 043
  byte    dayskill;         // 044
  byte    recvkill;         // 045
  word    countkill;        // 046
  word    readsecurity;     // 048
  byte    readflags[4];     // 050
  word    writesecurity;    // 054
  byte    writeflags[4];    // 056
  word    sysopsecurity;    // 060
  byte    sysopflags[4];    // 062
  char    originline[61];   // 066
  byte    akaaddress;       // 127
} _messagesra;              // 128 Bytes


//  ------------------------------------------------------------------
//  RA Configuration structure

typedef struct {
  word    versionid;
  byte    commport;
  long    baud;
  byte    inittries;
  char    initstr[71],
          busystr[71];
  char    initresp[41],
          busyresp[41],
          connect300[41],
          connect1200[41],
          connect2400[41],
          connect4800[41],
          connect9600[41],
          connect19k[41],
          connect38k[41];
  byte    answerphone;
  char    ring[21],
          answerstr[21];
  byte    flushbuffer;
  short    modemdelay;
  short    minimumbaud,
          graphicsbaud,
          transferbaud;
  char    slowbaudtimestart[6],
          slowbaudtimeend[6],
          downloadtimestart[6],
          downloadtimeend[6],
          pagingtimestart[6],
          pagingtimeend[6];
  char    loadingmsg[71],
          listprompt[71];
  short    pwdexpiry;
  char    menupath[61],
          textpath[61],
          netpath[61],
          nodelistpath[61],
          msgbasepath[61],
          syspath[61],
          externaledcmd[61];
  _raaddr address[10];
  char    systemname[31];
  short    newsecurity;
  short    newcredit;
  byte    newflags[4];
  char    originline[61];
  char    quotestring[16];
  char    sysop[36];
  char    logfilename[61];
  byte    fastlogon,
          allowsysrem,
          monomode,
          strictpwdchecking,
          directwrite,
          snowcheck;
  short    creditfactor;
  short    usertimeout,
          logontime,
          passwordtries,
          maxpage,
          pagelength;
  byte    checkformultilogon,
          excludesysopfromlist,
          onewordnames;
  char    checkmail;
  byte    askvoicephone,
          askdataphone,
          dofullmailcheck,
          allowfileshells,
          fixuploaddates,
          showfiledates;
  short    ansi,
          clearscreen,
          moreprompt,
          uploadmsgs,
          killsent;
  short    crashasksec;
  byte    crashaskflags[4];
  short    crashsec;
  byte    crashflags[4];
  short    fattachsec;
  byte    fattachflags[4],
          normfore,
          normback,
          statfore,
          statback,
          hiback,
          hifore,
          windfore,
          windback,
          exitlocal,
          exit300,
          exit1200,
          exit2400,
          exit4800,
          exit9600,
          exit19k,
          exit38k;
  byte    multiline;
  byte    minpwdlen;
  word    minupspace;
  char    hotkeys;
  byte    borderfore,
          borderback,
          barfore,
          barback,
          logstyle,
          multitasker,
          pwdboard;
  word    buffersize;
  char    fkeys[10][61];
  byte    futureexpansion[1024];
} _configra;


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack()
#endif

//  ------------------------------------------------------------------
