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
//  PCBoard Structures.
//  ------------------------------------------------------------------

#ifndef __GS_PCB_H
#define __GS_PCB_H


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
    #pragma pack(1)
#endif

//  ------------------------------------------------------------------
//  PCBoard Conference Information (CNAMES.@@@ file)

struct PcbConf
{

    char name[14];                  // Conference name
    byte publicconf;                // New user default registration
    byte autorejoin;                // Auto-rejoin for this conference
    byte viewmembers;               // Allow viewing conference members
    byte privuplds;                 // Make uploads private
    byte privmsgs;                  // Make all messages private
    byte echomail;                  // Is this an Echo-Mail conference
    short reqseclevel;               // Security required to join non-public conf.
    short addsec;                    // Additional security level added after join
    short addtime;                   // Additional time added after join
    char msgblocks;                 // Number of message base index blocks
    char msgfile[32];               // Name and location of messages file
    char usermenu[32];              // Name and location of Users Menu
    char sysopmenu[32];             // Name and location of Sysops Menu
    char newsfile[32];              // Name and location of Conference News
    char pubupldsort;               // Sort type for public upload DIR file
    char uplddir[29];               // Name and location of public upload DIR file
    char pubupldloc[26];            // Public upload location
    char prvupldsort;               // Sort type for public upload DIR file
    char privdir[29];               // Name and location of private upload DIR file
    char prvupldloc[26];            // Private upload location
    char drsmenu[29];               // Name and location of conference doors menu
    char drsfile[33];               // Name and location of DOORS.LST file
    char bltmenu[29];               // Name and location of conference blt menu
    char bltnameloc[33];            // Name and location of BLT.LST file
    char scrmenu[29];               // Name and location of conference scripts menu
    char scrnameloc[33];            // Name and location of SCRIPT.LST
    char dirmenu[29];               // Name and location of directories menu
    char dirnameloc[33];            // Name and location of DIR.LST file
    char pthnameloc[33];            // Name and location of DLPATH.LST file
};


//  ------------------------------------------------------------------
//  PCBoard Additional Conference Information (CNAMES.ADD file)

struct PcbAddConf
{

    byte forceecho;                 // Turn off echo question, force all msgs to echo
    byte readonly;                  // Do not allow ANY msgs to be entered in conf
    byte noprivatemsgs;             // Do not allow PRIVATE msgs to be entered
    char retreceiptlevel;           // Level required to request return receipts
    byte recordorigin;              // Record ORIGIN in messages
    byte promptforrouting;          // Prompt user for ROUTING information
    byte allowaliases;              // Allow aliases to be used
    byte showintroonra;             // Show the Conf INTRO in the middle of R A scan
    char reqleveltoenter;           // Security Level required to enter messages
    char password[13];              // Password reqd to join if private
    char intro[32];                 // Name/location of conference INTRO file
    char attachloc[32];             // Location for file attachment storage
    char regflags[4];               // RXS flags for automatic conf registration
    char attachlevel;               // Level required to attach a message
    char carbonlimit;               // Max number of names in carbon list
    char cmdlst[32];                // Name/location of CMD.LST use instead of default
    byte oldindex;                  // Maintain old MSGS indexes?
    byte longtonames;               // Allow long TO: names to be entered
    char carbonlevel;               // Level required to enter @LIST@ messages
    char conftype;                  // One byte code for conference type
    long exportptr;                 // A pointer to the last message number exported
    long chargetime;                // Amount to charge for time spent in conference
    long chargemsgread;             // Amount to charge for each message read
    long chargemsgwrite;            // Amount to charge for each message written
    char reserved[64];              // Reserved for future growth
    char name2[48];                 // Conf name #2
};


//  ------------------------------------------------------------------
//  Conference types in CNAMES.ADD

const char PCBCONFTYPE_NORMAL     = 0;  // Normal PCBoard Conference
const char PCBCONFTYPE_EMAIL      = 1;  // Internet Email (Import & Export)
const char PCBCONFTYPE_USENETJUNK = 2;  // Internet/Usenet Junk (Import Only)
const char PCBCONFTYPE_USENETMOD  = 3;  // Usenet Moderated Newsgroup (Export Public as Email to Moderator / Private as Email to TO:)
const char PCBCONFTYPE_USENETPUB  = 4;  // Usenet Public Newsgroup (Export Public as Articles / Private as Email to TO:)
const char PCBCONFTYPE_FIDOCONF   = 5;  // Fido Conference


//  ------------------------------------------------------------------

struct PcbFidoArea
{

    word  pcbconference;            // PCBoard conference number
    char  areaname[60];             // FIDO area tag
    char  messages[66];             // Path to conference message file
    char  defaultaka[25];           // Aka for conference
    char  origin[70];               // Origin for conference
    char  highascii;                // S R C N
    char  reserved[19];             // Reserved space
};


//  ------------------------------------------------------------------

struct PcbFidoArchivers
{

    char  archivers[4][66];           // Stores the name of the archiver executable
    char  archiver_switches[4][80];   // Stores the name of the archiver switches
    char  unarchivers[4][66];         // Stores the name of the unarchiver executable
    char  unarchiver_switches[4][80]; // Stores the name of the unarchiver switches
};


//  ------------------------------------------------------------------

struct PcbFidoDirectories
{

    char  incoming_packets[66];  // Incoming directory
    char  outgoing_packets[66];  // outbound directory
    char  outgoing_msg[66];      // *.MSG    directory
    char  bad_packets[66];       // BAD      directory
    char  nodelist_path[66];     // nodelist directory
    char  work_directory[66];    // work     directory
};


//  ------------------------------------------------------------------

struct PcbFidoEmsiData
{

    char  BBS_Name[60];      // BBS name
    char  City[30];          // City
    char  Sysop[30];         // Sysop's name
    char  Phone[50];         // BBS phone number
    char  Baud[10];          // MAX baud
    char  Flags[50];         // Fido FLAGS
};


//  ------------------------------------------------------------------

struct PcbFidoAddress
{

    char  nodestr[25];    // Full fido address in the form Z:N/N.P
    word  this_zone;      // FIDO zone
    word  this_net;       // FIDO net
    word  this_node;      // FIDO node
    word  this_point;     // FIDO point
    char  reserved[10];   // Reserved space
};


//  ------------------------------------------------------------------

struct PcbFidoNode
{

    char  nodestr[25];       // Full fido address in the form Z:N/N.P
    word  zone;              // FIDO zone
    word  net;               // FIDO net
    word  node;              // FIDO node
    word  point;             // FIDO point
    word  pkt_type;          // FIDO packet type 1=stoneage 2=2+
    short  archiver_index;    // index to archiver 0=ZIP,1=ARJ,2=ARC,3=LZH
    char  reserved[10];      // Reserved space
};


//  ------------------------------------------------------------------

struct PcbFidoTranslate
{

    char  in[30];          // What to traslate
    char  out[30];         // What to translate to
    char  reserved[10];    // reserved space
};


//  ------------------------------------------------------------------

struct PcbFidoNodelist
{

    char  basename[80];  // Nodelist name (no extension)
    char  diffname[9];   // Diff file name (no extension)
    short  last_compile;  // Last compiled version number
    char  reserved[10];  // reserved space
};


//  ------------------------------------------------------------------

struct PcbFidoFreqPath
{

    char  str[60];        // fully qualified path to FREQ files
    char  reserved[20];   // reserved space.
};


//  ------------------------------------------------------------------

struct PcbFidoFreqInfo
{

    word  stime;      // Session max time
    word  dtime;      // Daily max time
    long  sbytes;     // Session bytes
    long  dbytes;     // Daily Bytes
    char  listed;     // Allowed Nodes
    word  baud;       // Minimum allowed baud
};


//  ------------------------------------------------------------------

struct PcbFidoFreqMagic
{

    char  magic_name[20];   // Magic name
    char  real_name[50];    // Associated file name
};


//  ------------------------------------------------------------------

struct PcbDirectories
{
    char incoming_packets[66];      // Incoming directory
    char outgoing_packets[66];      // Outbound directory
    char outgoing_msg[66];          // *.MSG    directory
    char bad_packets[66];           // BAD      directory
    char nodelist_path[66];         // Nodelist directory
    char work_directory[66];        // Work     directory
    char passthrough[66];           // Passthru files
    char securemail[66];            // Secure netmail
    char messages[66];              // response Messages
};


//  ------------------------------------------------------------------

struct PcbArchivers
{
    char archivers[4][66];           // Stores the name of the archiver executable
    char archiver_switches[4][80];   // Stores the name of the archiver switches
    char unarchivers[4][66];         // Stores the name of the unarchiver executable
    char unarchiver_switches[4][80]; // Stores the name of the unarchiver switches
};


//  ------------------------------------------------------------------

struct PcbEmsiData
{
    char bbs_name[60];      // BBS name
    char city[30];          // City
    char sysop[30];         // Sysop's name
    char phone[50];         // BBS phone number
    char baud[10];          // MAX baud
    char flags[50];         // Fido FLAGS
};


//  ------------------------------------------------------------------

struct PcbAreasDat
{
    word confno;
    char areatag[60];
    word aka;
    word origin;
    char high_ascii;
    word last_activity;
    char allow_private;
    char allow_attach;
    byte reserved[10];
};


//  ------------------------------------------------------------------

struct PcbAkasDat
{
    ftn_addr  addr;
    char  primary_address;
    char  in_seenby;
    char  present_in_handshake;
    char  up_downlink;
    char  conf_range[70];
    char  reserved[10];
};


//  ------------------------------------------------------------------

struct PcbOriginsDat
{
    char  origin[70];
    char  conf_range[70];
    char  reserved[10];
};


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
    #pragma pack()
#endif

//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------

