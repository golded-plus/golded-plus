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

// header file converted from Pascal to C by damWARE, 08.06.98.

// WaterGate Developers Kit for release 0.93
// database structures

// include this file from one of your source files with $I structs.pas

// this file describes the structures of WaterGate's database. There is
// no code in header, but just the declarations of the structures. There
// _is_ a whealth of explanation in this file though!

/* History:

  950903 0.19     - Initial release.
  950918 0.19.p2  - The config structure in the original release was a
                    complete mess and unusable.
  951030 0.20     - Added administrator fields.
  951203 0.21     - New list server user type, longer time zone string,
                    export AKA in user record.
  960214 0.90     - Config: Log9->LogAreaFix and Log10->LogMapApply.
                    UUCPForwardRecord: reduced path length by 1 and
                    added Group letter.
  960530 0.91     - AreaBase: Added fields for decoding files.
                    Config: Log11..Log14 are now used.
                    New fields for config: CopyHeaders_FU, MailGrade,
                    NewsGrade, TimeSlicing, NetmailDecode,
                    NetmailDecodePath, PrivateDecode, PrivateDecodPath.
  961014 0.92     - Config: Log15 and Log16 are now used, Added RAR
                    support, more system node numbers, more copy header
                    entries, more areafix/newsfix forward entries,
                    remove unused fields.
                    ListServer: Added Access and DefaultAccess.
                    UserBase: Added BAG and SMTP variant record entry.
                    AreaBase: Seen-BY akas now support 100 bits.
  960706 0.93     - New groups format and lots of other changes.
                    AreaBase 04->07: Changed IsInGroups, Added Hidden.
                    Updated groups explanation.
                    UserBase 05->08: Removed unused longint. Changed
                    groups, added envelope header.
                    ListServer 03->05: Added re-confirmation fields,
                    added MLAddress, added confirm interval.
                    Config 14->22: Changed Default_F and Default_U,
                    removed ConversionTable, Added SmtpForward,
                    MaxXPostNewsGroups, WildCatMSGLockDir, OpusDateFormat,
                    MaxWildCatMsgLen, KillAreafixMsgs, Color set.
*/

// I have reduced all the types and constants we use, to make it easier
// for you to read the structures. The names used end with _F if it is
// Fidonet related, and with _U if it is UUCP/Usenet related. _B is BAG,
// _S is SMTP and _P is POP3.

#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif

/*
**==========================================================================
**                  ABOUT THE DATABASES
**==========================================================================
*/

// WaterGate uses six binary database files and one ascii configuration
// file. This source file only covers the six binary databases. Each of
// the database files starts of with a 26 byte long header, which is
// basically a string that is written to disk, including the length byte
// at the first position. The string is thus declared as a STRING[25].
// Each database has its own header, as defined below. The last two
// digits are a modification counter, which is increased by one very
// time the database structures change. You have to check this header
// and it has to match exactly, or else you should not access the
// databases with the structures in this file!

const char AreaBaseHeader[26]     = "WtrGate AreaBase 07";
//const char UserBaseHeader[26]     = "WtrGate UserBase 08";
//const char SubscrBaseHeader[26]   = "WtrGate SubscrBase 01";
//const char GrpDescrBaseHeader[26] = "WtrGate GroupDescr 03";
//const char ListServerHeader[26]   = "WtrGate ListServer 05";
const char ConfigHeader[26]       = "WtrGate Config 22";

// Directly after this string, the first record starts. All record
// numbering is 1..n, with the special value 65535 meaning a NIL pointer
// record, mostly used to indicate free positions or the end of a list
// of record numbers.

const word NILRecordNr = 0xFFFF;

// to find out how many records are present in a database, you use the
// following formule:
//
// RecCount = ((FileSize (Database)-26) DIV SizeOf (Record));
//
// This formula can be used for each of the databases. Simply substitute
// "Record" with the correct structure name and "Database" with the
// actual file variable. It is best to declare the file variable as a
// FILE and to open the database with a Reset (Database,1), as below:
//
// VAR AreaFile : FILE;
//     RecCount : WORD;
//
// BEGIN
//      Assign (AreaFile,'AREABASE.TDB');
//      Reset (AreaFile,1);
//      BlockRead (AreaFile,Header,26);
//      RecCount:=(FileSize (AreaFile)-26) DIV SizeOf (AreaBaseRecord));
//      Close (AreaFile);
// END
//
// To read and write a record, you substract 1 from the record number,
// multiply it with the record size and add 26, then seek to that
// position in the file and read or write the record, like this:
//
// VAR AreaRec   : AreaBaseRecord;
//     AreaRecNr : WORD;
//     Position  : LONGINT;
//
// BEGIN
//      Position:=26+(AreaRecNr-1)*SizeOf (AreaRecord);
//      Seek (AreaFile,Position);
//      BlockRead (AreaFile,AreaRec,SizeOf (AreaBaseRecord));
// END;

/*
**==========================================================================
**                  GLOBAL STRUCTURES
**==========================================================================
*/

// this structure is used to hold an AKA, whereever required

typedef struct {
  word   zone;
  word   net;
  word   node;
  word   point;
  char   domain[26];
} FidoAddrType;


/*
**==========================================================================
**                  GROUP DESCRIPTIONS DATABASE
**==========================================================================
*/

// this database contains 128 records, one for each of the groups. Each
// record hold the description of the group, the system AKA to use,
// which is an index number from 1 to 100 in the array of system AKAs
// in the Configuration Record. And finally it contains the ReadOnly
// boolean, which is set to TRUE to disable posting to any area in this
// group, unless the user and the area are both in read/write group as
// well.

struct {
  char   groupdesc[31];
  byte   originaka;                                         // 1..100
  bool   readonly;
} GroupDescRecord;

// [...]

// new groups storage (0.93):
//
// there are 128 groups, numbered 1 to 128. The AreaBase.IsInGroups
// and UserBase.Groups are 16 byte arrays, where each byte represents
// subscription to one group. Bit 0 in the first byte is group 1, etc.

const word MaxGroups    = 128;                 // A1-5,B1-5,..,Y1-5,Z1-3
const word GroupFlagLen = MaxGroups / 8;

typedef byte GroupFlagType[GroupFlagLen];      // bit mask

// to access to groups, use the GroupFlagType and the following code
// sequence to access the group list.
//
// VAR I,O : BYTE;
//
// I:=1+((NewGroupNr-1) DIV 8);
// O:=(NewGroupNr-1) MOD 8;
// Group[I]:=Group[I] OR (1 SHL O);
//
// The above example adds NewGroupNr (1..128) by settings the correct
// bit in the array. I is an index into the array and O is the bit
// number ("offset", 0..7).
//
// You can use constructions with AND to reset a bit. I have created
// abstract functions to access a GroupFlagType for resetting it,
// adding a single group, removing a single group, testing for
// membership, set overlap tests, etc.


/*
**==========================================================================
**                  AREABASE
**==========================================================================
*/

typedef struct {
  bool   deleted;
  byte   areatype;              // 0=Echo, 1=Netmail, 2=Local, 3=Email
  char   areaname_U[61];        // the two names of this area
  char   areaname_F[61];
  char   comment[61];           // the comment / description
  byte   isingroups[16];        // the groups this area is in. See the group
                                // descriptions database for an explanation.

  word   userlist;              // points to the first subscription base
                                // record. See explanation there.

  byte   originaka;             // the system AKA to use for the origin line,
                                // is an index 1..20 into the array in the
                                // configuration record.

  byte   addseenbyakas[13];     // The system AKAs 1..100 to add to the SEEN-BY
                                // If the bit is set, it is added. [0]bit0 is
                                // AKA 1, [0]bit1 2, etc.

  bool   passive;               // TRUE if this area is passive 
  bool   alwpassive;            // TRUE if this area is allowed to be passive
  byte   originnr;              // origin number to use: 0=custon, 1/2=system
  char   origin[62];            // the custom origin line
  byte   fidomsgstyle;          // the style of this area's message base
                                // 0=None, 1=Msg, 2=Squish, 3=Jam, 4=WildCat

  char   fidomsgpath[80];       // the path to the database
  word   fidomsgage;            // the maximum age of a message, when cleaning
  word   fidomsglimit;          // the max. num of msgs in allowed
  byte   moderated;             // is this area moderated? 0=No, 1=Usenet
  char   moderator[51];         // the e-mail address of the moderator
  bool   decode;                // XX/UU/MIME decode?
  char   decodepath[80];        // path where to store decoded files
  bool   hidden;                // if TRUE then doesn't show up in AreaFix/newsfix
} AreaBaseRecord;

/*
**==========================================================================
**                  USERBASE
**==========================================================================
*/

// the userbase holds all the users that are configured in WtrConf. There
// is one record for each user. The maximum number of records is 65534.

struct {
  bool   deleted;               // TRUE means deleted
  byte   system;                // 0=Fido, 1=Usenet, 2=Bag supplier
  char   organization[61];      // description of this user
  byte   groups[16];            // the groups this user is allowed in
                                // see the group description database
                                // for an explanation of the storage.

  bool   passive;               // is this user passive?
  char   areafixpwd[21];        // its AreaFix/newsfix password
  bool   allowfrom;             // TRUE = allow special AreaFix commands
  bool   allowcreate;           // TRUE = allow new area creation
  bool   allowsubdomains;       // TRUE if this user is allowed to have sub-domains
  word   arealist;              // pointer to the first subscription base
                                // record, holding the areas this user is
                                // subscribed to. See the subscription
                                // base for an explanation.

  char   uucpname[11];          // the user's UUCP name
  bool   worldreg;              // World Registered UUCP name?
  char   domains[51][6];        // domain names

  word   type;

  union {
    // 0 = FidoNet style user
    struct {
      FidoAddrType  Address;      // AKA
      char   sysop[51];           // full name of the sysop
      char   packetpwd[9];        // password for *.PKT files
      byte   compression;
      dword  maxpktlength;        // max  length of an .PKT files
                                  // before it is cut off and packed

      byte  sendformat;           // the send condition: 0=Normal,
                                  // 1=Hold, 2=Crash, 3=Direct.

      byte  lastarchdow;          // day-of-week used on the last sent
                                  // archive. 0=first, 1..7 = dow.

      byte  lastarchnr;           // last archive number used
      byte  exportaka;            // Export AKA. 0=Auto, 1..100=System AKA
      bool  decodefiles;          // TRUE = decode files for this user?
    } fidostyle;
    // 1 = UUCP style user
    struct {
      byte compress;              // 0=None, 1=Compress, 2=Zip
      bool cunbatch;              // TRUE= add CunBatch to compressed archive
      char mailgrade;
      char newsgrade;
      bool gigot;                 // TRUE = create GIGOT compatible .XQT files
    } uucpstyle;
    // 2 = BAG style user
    struct {
      char bagbacklink[11];       // uucp name of the bag return system
      char bagpath[80];           // Search path for the bag files
    } bagstyle;
    // 3 = SMTP mailer link
    struct {
      char smtpinpath[80];        // where to process .WRK/.TXT files from
      char smtpoutpath[80];       // where to create .WRK/.TXT files
    } smtpstyle;
    // 4 = POP3 mailbox link
    struct {
      char pop3file[80];          // where is the POP3 file stored? }
      char recipient[51];         // single recipient address
      char separator[16];         // separator searched for
      char envelopehdr[26];       // envelope header searched for
    } pop3style;
    // 5 = BBS
    struct {
      char inbound[80];           // search for .PKT files from BBS tosser here
      char outbound[80];          // create .PKT files for BBS tosser here
      byte systemaka;             // 1..100
      word fakezone;              // fake AKA
      word fakenet;
      word fakenode;

      char inboundext[3];         // extension for inbound packets (PKT/OUT)
      bool keepsbp;               // TRUE = keep SEEN-BY and PATH?
      byte reserved1;             // unused
    } bbsstyle;
  };
  
} UserBaseRecord;


/*
**==========================================================================
**                  SUBSCRIPTIONS DATABASE
**==========================================================================
*/

// [...]

/*
**==========================================================================
**                  CONFIGURATION DATABASE
**==========================================================================
*/

// structures for the system configuration database. There is only one
// record in this database. The structures below start with the areafix
// and newsfix forwarding records, which are used in the configuration
// record. I have removed all other references and put them straigt in.

// Since AreaFix and newsfix forwarding have not been re-implemented
// yet, I have not described the structures, until they become certain.

typedef struct {
  FidoAddrType  uplinkaddress;  // uplink AKA
  bool   unconditional;         // allow all new names, or check against file?
  char   arealistpath[51];      // path to the forwarding file
  byte   arealist;              // type of the file. 0=areas.bbs, 1=names list
  char   areamanager[11];       // name of uplink's AreaFix
  char   password[11];          // password for uplink's areafix
  byte   group;                 // group to put new areas in. 0 = Group A1
  bool   addplus;               // Add "+" to connect in front of areaname
} AreafixForwardRecord;

typedef struct {
  char   uucpname[11];          // UUCPname of uplink
  char   arealistpath[50];      // path to the newsgroups listing
  byte   group;                 // group to put new areas in. 0 = Group A1
} UUCPForwardRecord;

typedef struct {
  char   systemdir[80];         // path to the other databases
  char   sysop[51];             // full name of the sysop
  word   dupechecks;            // max. number of dupes to remember (16000 max.)
  bool   dodupechk;             // TRUE = Check for dupes
  byte   maxhandles;            // max. number of outbound files to keep open
  bool   cachetdbs;             // TRUE cache the databases in XMS
  char   toolargepath[80];      // path to the too-large directory for oversized msgs
  char   logfilepath[80];       // path and filename of the logfile and stats file
  FidoAddrType nodenrs[100];    // system AKAs
  word   pointnets[100];        // pointnet for each of the system AKAs
  byte   security_F[2];         // Secure the inbound paths: 0=Yes, 1=No
  char   inbound_F[80][2];      // paths to the inbound directories
  char   outbound_F[80];        // path to the outbound directory
  char   comprprgs_F[80][18];   // Fido compress/decompression programs
  dword  maxfidomsglen;
  dword  maxfidoarclen;
  byte   defgroups_F[16];       // default groups for a new Fido user
  byte   fidosystem;            // 0=Binkley, 1=Frontdoor, 2=dBridge
  bool   fidoacceptto;          // scan the To: field for a e-mail address?
  byte   fidonetmailtype;       // Netmail msgbase type: 0=None, 1=Msg, 2=Squish, 3=Jam
  char   fidonetmailpath[80];   // path to the database
  byte   fidobadareatype;       // Bad msgbase type: 0=None, 1=Msg, 2=Squish, 3=Jam
  char   fidobadpath[80];       // path to the database
  byte   fidodupeareatype;      // Dupe msgbase type: 0=None, 1=Msg, 2=Squish, 3=Jam
  char   fidodupepath[80];      // path to the database
  byte   fidoautocreatetype;    // New created areas type: 0=None, 1=Msg, 2=Squish, 3=Jam
  char   defaultfidomsgpath[80];// default path to use (no filename)
  bool   stripseenby;           // TRUE = strip seen-bys when importing
  bool   autolink;              // TRUE = link after import
  bool   replacetear;           // replace the tearline when exporting?
  word   defnumbtokeep_F;       // default number of msgs to keep for an new area
  word   defdaystokeep_F;       // default number of days to keep msgs for a new area
  byte   fidoarcmailextension;  // 0=Arc(0..9), 1=Hex(0..9,A..F), 2=All(0..9,A..Z)
  byte   uucpgateway;           // Gateway AKA nr: 1..10, as index in system AKAs array
  char   nameseparator;         // character to replace spaces with in user names
                                // when building an e-mail address.

  char   origins[2][62];        // the two system origin lines
  bool   worldwide;             // TRUE = system UUCPname is world-reg
  byte   defgroups_U[16];       // default groups to use for UUCP
  char   spoolbasedir[80];      // path to the spool directories
  dword  maxdatlength;          // Max. length of .DAT files, before compression
  char   smarthost[11];         // UUCPname of your smarthost
  char   uucpname[11];          // system UUCP name
  char   backbone[51];          // domain name of the backbone
  char   organization[60];      // system organization line
  char   domains[6][51];        // system domain names
  char   comprprg_U[4][80];     // installed compression/decompression programs
  byte   privmailtype;          // type of private mail message base:
                                // 0=None, 1=*.MSG, 2=Squish, 3=JAM
  char   privmailpath[80];      // path to the message base
  char   privmailoption[10][73];
  byte   privmailselect[10];    // where to search: 0=From, 1=To, 2=Subj
  byte   reserved1;
  AreafixForwardRecord  areafixforward[50];
  UUCPForwardRecord  uucpforward[50];
  dword  maxsquishmsglen;       // max msg length when importing into Squish base
  dword  maxjammsglen;          // max msg length when importing into JAM base
  byte   defaultcompressor;     // default compression to set in new user records:
                                // 0=ARC, 1=ARJ, 2=LZH, 3=PAK, 4=ZIP, 5=ZOO, 6=RAR
                                // 7=OP1, 8=GUS, 9=PKT.
  bool   packedaddresses;       // TRUE = build small e-mail addresses
  char   rescanflagfile[80];    // path to the rescan flag file
  char   dbridgequeuepath[80];  // path to the d'Bridge dir where to store the queueing files }
  char   areafixname[11];       // name of the fido areafix
  bool   killgatednetmail;      // TRUE = automatically kill gated netmail
  char   newsfixname[11];       // name of the UUCP areafix (newsfix)
  bool   useswapfile;           // TRUE = use swapfile
  char   swapfilepath[80];      // path to the swapfile
  byte   swapfilesize;          // max length of the swapfile in megabytes
  bool   replyfsc35;            // TRUE = create FSC35 reply kludges
  bool   headerfullname;        // TRUE put the full name in the From: field?
  bool   logdebug;              // TRUE = enable all other options
  bool   logspooltossed;        // TRUE = log tossed spool files
  bool   logfidoextract;        // TRUE = log extracted fidonet archives
  bool   logfidotossed;         // TRUE = log tossed fido .PKT files
  bool   logtranslationFU;      // TRUE = log built e-mail addressed
  bool   logcheckfilter;        // TRUE = log accepted / denied new newsgroup names
  bool   logxfix;               // TRUE = log fixed to-address from .X files
  bool   logcopyheaders;        // TRUE = Log headers that were copied from fido netmail/echomail messages
  bool   logillegalheaders;     // TRUE = Log found headers that are not allowed
  bool   logareafix;            // TRUE = Log commands to / replyies from areafix / newsfix
  bool   logmapapply;           // TRUE = Log applied mapping statements
  bool   lognetmailimport;      // TRUE = log all imported netmails
  bool   logrodeny;             // TRUE = log all read-only denied postings in areas
  bool   logpkteachecho;        // TRUE = log each echomail plus length
  bool   loguucpoutbound;       // TRUE = log all created UUCP files
  bool   logsmtpoutbound;       // TRUE = log all created SMTP jobs
  bool   logexportednetmail;    // TRUE =
  bool   log17, log18, log19, log20, log21;
  char   copyheadernames[16][30];// names of the e-mail Headers to search for
                                // without the trailing space!
                                
  byte   copyheaderhow[30];     //  How to store the copied header:
                                // 0=Not, 1=Kludge, 2=Text
                                
  char   gatewayuser[37];       // user name of the gateway user
  bool   bounceunknown;         // TRUE=bounce undeliverable mail
                                // FALSE=Write to netmail

  bool   bouncesmall;           // TRUE=Send only the first approximate 20 lines
  byte   adminaddrtype;         // 0=None, 1=Fido, 2=Usenet
  char   adminfidoname[37];     // Full name of the administrator, for the Fidonet address
  FidoAddrType adminfidoaddr;   // Fidonet address of the administrator
  char   adminuucpaddr[256];    // E-mail address of the administrator
  bool   adminsendlog;          // Send the logfile to the administrator?
  bool   adminsend2, adminsend3, adminsend4, adminsend5, adminsend6, adminsend7,
         adminsend8, adminsend9, adminsend10,adminsend11, adminsend12, adminsend13,
         adminsend14,adminsend15,adminsend16;
  char   timezone[26];          // time zone description
  bool   copyheaders_FU;        // TRUE = Scan and copy headers Fido->Usenet
  char   mailgrade;             // grade letter to use in mail job UUCP filenames
  char   newsgrade;             // grade letter to use in news batch job UUCP filenames
  bool   timeslicing;           // FALSE = Do not detect multitasker
  bool   netmaildecode;         // TRUE = scan and decode files from imported netmail
  char   netmaildecodepath[80]; // where to store the files
  bool   privatedecode;         // TRUE = scan and decode files from imported private scanned messages
  char   privatedecodepath[80]; // where to store the files
  word   maxrmaillinelen;       // max len for rmail line in .X file
  byte   minimumdiskfree;       // min MB's free for disk check
  char   diskfreedrives[27];    // drives to check for disk check
  byte   old;
  byte   rmailcorrect;          // rmail correction to use
  bool   gatemsgid;             // TRUE = gate Message-ID -> MSGID
  bool   forcenobitmask;        // TRUE = don't put "0" in front of UUCP jobs
  char   rescanflagfile2[80];   // editor rescan flag file
  byte   organizationinorigin;  // Gate organization -> origin: no, yes, override
  bool   alwaysmimequoteprint;  // TRUE = always use quoted-printable
  char   smtpforward[51];       // address for dumping mail at ISP
  byte   maxxpostnewsgroups;    // cross-post limit. 255 = allow all
  char   wildcatmsglockdir[80]; // path to MSGLOCK directory for WildCat conference lock
  bool   opusdateformat;        // TRUE = create *.MSG with Opus date fields
  dword  maxwildcatmsglen;      // max len of a single WildCat message
  bool   killareafixmsgs;       // kill *.MSG with areafix after processing?
  byte   colors[21];            // color codes for each of the elements
}  ConfigRecord;

#if defined(GOLD_CANPACK)
#pragma pack()
#endif

//==== END OF FILE ==========================================================
