//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1991-1995 Gerard J. van der Land
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
//  System data file definitions for GEcho 1.20.b10+
//  ------------------------------------------------------------------

#ifndef __gs_ge120_h
#define __gs_ge120_h


//  ------------------------------------------------------------------

#include <gftnall.h>


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif


//  ------------------------------------------------------------------

#define GE_THISREV 0x0002  // System file revision level
#define GE_MAJOR   1       // GEcho major revision version
#define GE_MINOR   20      // GEcho minor revision version

#define AKAS               32  // Main + AKAs
#define OLDAKAS            11  // Not used
#define OLDUPLINKS         10  // Not used
#define OLDGROUPS          26  // Not used
#define USERS              10  // User names
#define MAXAREAS        10000  // Area records
#define MAXCONNECTIONS    500  // Connections per area
#define MAXGROUPS         256  // Group records
#define MAXNODES         5000  // Node records
#define MAXVIAS            60  // Pack "Via" records
#define MAXROUTES         640  // Pack "Routed node" records

#define GROUPBYTES  ((MAXGROUPS + 7) / 8)


//  ------------------------------------------------------------------
//  Data types

typedef char GROUPS[GROUPBYTES];

/* --- Log levels */

#define LOG_INBOUND     0x0001  /* Inbound activities */
#define LOG_OUTBOUND    0x0002  /* Outbound activities */
#define LOG_PACKETS     0x0004  /* Inbound packet info */
#define LOG_UNEXPECT    0x0008  /* Extended packet info */
#define LOG_AREAMGR     0x0010  /* Unexpected passwords */
#define LOG_EXTPKTINFO  0x0040  /* AreaMgr messages */
#define LOG_NETEXPORT   0x0100  /* Exporting of netmail */
#define LOG_NETIMPORT   0x0200  /* Importing of netmail */
#define LOG_NETPACK     0x0400  /* Packing of netmail */
#define LOG_NETMOVED    0x0800  /* Moving Sent/Rcvd mail */
#define LOG_STATISTICS  0x2000  /* GEcho's statistics */
#define LOG_MBUTIL      0x4000  /* MBUTIL's activities */
#define LOG_DEBUG       0x8000  /* DEBUG: All of the above */


/* --- Log styles */

#define LOG_FD       0  /* FrontDoor */
#define LOG_BINK     1  /* BinkleyTerm */
#define LOG_QUICK    2  /* QuickBBS */
#define LOG_DBRIDGE  3  /* D'Bridge */


/* --- Setup option bits */

#define NOKILLNULL 0x0001  /* Don't kill null netmail messages while tossing */
#define RESCANOK   0x0002  /* Allow %RESCAN */
#define KEEPREQS   0x0004  /* Keep AreaMgr requests */
#define NONODEADD  0x0008  /* Don't automatically add NodeMgr records */
#define USEHMBBUF  0x0020  /* Use Hudson buffers */
#define KEEPNET    0x0040  /* Don't use Kill/Sent on exported netmail */
#define KEEPMGR    0x0080  /* Don't use Kill/Sent on MGR receipts */
#define NORRQS     0x0100  /* Ignore Return receipt Requests */
#define KILLDUPES  0x0200  /* Kill duplicate messages */
#define DOS32BIT   0x0400  /* Run 32-bit DOS version on 386+ machines */
#define NOCRSTRIP  0x0800  /* Don't strip Soft-CRs */
#define REMOVEJUNK 0x1000  /* Remove "Re:" junk from JAM subjects */
#define NOAUTODISC 0x2000  /* Don't automatically disconnect empty PT areas */
#define NOCHECKEND 0x4000  /* Don't check for valid end of archives */
#define SETPVT     0x8000  /* Set Pvt on imported netmail messages */


/* --- Extra option bits */

#define NOCHKDEST  0x0001  /* Don't check packet destination */
#define AUTOCREAT  0x0002  /* Automatically create message bases */
#define PAUSEOK    0x0004  /* Allow %PAUSE */
#define NOTIFYOK   0x0008  /* Allow %NOTIFY OFF */
#define ADDALLOK   0x0010  /* Allow +* */
#define PWDOK      0x0020  /* Allow %PWD */
#define PKTPWDOK   0x0040  /* Allow %PKTPWD */
#define NOBADPKTS  0x0080  /* Don't notify sysop about BAD/DST/LOC packets */
#define PKTPRGONCE 0x0100  /* Run PKT program only before the first PKT */
#define CREATEBUSY 0x0200  /* Create busy flags */
#define COMPRESSOK 0x0400  /* Allow %COMPRESS */
#define FROMOK     0x0800  /* Allow %FROM */
#define REDIR2NUL  0x1000  /* Redirect output of external utilities to NUL */
#define NOEXPAND   0x2000  /* Don't expand filenames of file attaches */
#define LOCALEXPT  0x4000  /* Export netmail to our own AKA */
#define OPUSDATES  0x8000  /* Use Opus style binary date/time stamps */


/* --- Compression types */

#define PR_ARC  0  /* Compressed mail files created by ARC or PKPAK */
#define PR_ARJ  1  /* Compressed mail files created by ARJ */
#define PR_LZH  2  /* Compressed mail files created by LHA */
#define PR_PAK  3  /* Compressed mail files created by PAK */
#define PR_ZIP  4  /* Compressed mail files created by PKZIP */
#define PR_ZOO  5  /* Compressed mail files created by ZOO */
#define PR_SQZ  6  /* Compressed mail files created by SQZ */
#define PR_UC2  7  /* Compressed mail files created by UC II */
#define PR_RAR  8  /* For compressed mail files created by RAR */
#define PR_PKT 10  /* Uncompressed PKT files */


/* --- Locking method */

#define LOCK_OFF    0  /* Deny Write (Exclusive) */
#define LOCK_RA101  1  /* RemoteAccess 1.01 (SHARE) */
#define LOCK_RA111  2  /* RemoteAccess 1.11 (SHARE) */


/* --- Semaphore mode */

#define SEMAPHORE_OFF  0  /* Don't use semaphores */
#define SEMAPHORE_FD   1  /* FrontDoor 2.1x */
#define SEMAPHORE_IM   2  /* InterMail 2.2x */
#define SEMAPHORE_DB   3  /* D'Bridge 1.5x */
#define SEMAPHORE_BT   4  /* BinkleyTerm 2.5x */
#define SEMAPHORE_MD   5  /* MainDoor */


/* --- Check user name */

#define CHECK_NOT        0  /* Don't check if user name exists */
#define CHECK_USERFILE   1  /* User file (USERS.BBS) */
#define CHECK_USERINDEX  2  /* User index (USERSIDX.BBS / NAMEIDX.BBS) */


/* --- Mailer type */

#define MAILER_FD  0  /* FrontDoor */
#define MAILER_DB  1  /* D'Bridge */
#define MAILER_BT  2  /* BinkleyTerm */


/* --- BBS type */

#define BBS_RA111     0  /* RemoteAccess 1.1x */
#define BBS_RA200     1  /* RemoteAccess 2.xx */
#define BBS_QUICK275  2  /* QuickBBS 2.7x */
#define BBS_SBBS116   3  /* SuperBBS 1.16 */


/* --- Change tear line */

#define TEAR_NO       0  /* No */
#define TEAR_DEFAULT  1  /* Replace default */
#define TEAR_CUSTOM   2  /* Replace custom */
#define TEAR_EMPTY    3  /* Replace empty */
#define TEAR_REMOVE   4  /* Remove */


/* --- PCBoard options */

#define PCB_145COMPAT  0x01  /* PCBoard 14.5 compatibility */


/* --- Wildcat! options */

#define WC_NONOTIFY  0x01  /* Do not notify users of new mail */


typedef struct {
  ftn_addr address;      /* Uplink address */
  char    areafix[9];    /* AreaFix program */
  char    password[17];  /* AreaFix password */
  char    filename[13];  /* "Forward List" filename */
  byte    unused[6];
  byte    options;       /* See --- Uplink options bits */
  byte    filetype;      /* 0 = Random, 1 = "<areaname> <description>" */
  dword   groups;        /* Nodes must have one of these groups */
  byte    origin;        /* Origin AKA */
} OLDUPLINK;

typedef struct {
  word zone;
  word net;
  byte aka;
} AKAMATCH;

typedef struct {
  byte bg_char;
  byte headerframe;
  byte headertext;
  byte background;
  byte bottomline;
  byte bottomtext;
  byte bottomkey;
  byte errorframe;
  byte errortext;
  byte helpframe;
  byte helptitle;
  byte helptext;
  byte helpfound;
  byte winframe;
  byte wintitle;
  byte winline;
  byte wintext;
  byte winkey;
  byte windata;
  byte winselect;
  byte inputdata;
  byte exportonly;
  byte importonly;
  byte lockedout;
} COLORSET;


/* --- SETUP.GE structure */

typedef struct {
  word sysrev;              /* Must contain GE_THISREV */
  word options;             /* Options bits, see --- Setup option bits */
  word autorenum;           /* Auto renumber value */
  word maxpktsize;          /* Maximum packet size, 0 = unlimited */
  byte logstyle;            /* See --- Log styles */
  byte oldnetmailboard;     /* Netmail board, must be zero now */
  byte oldbadboard;         /* Where bad echomail is stored (0 = path) */
  byte olddupboard;         /* Where duplicates are stored (0 = path) */
  byte recoveryboard;       /* Recovery board (1-200, 0 = delete) */
  byte filebuffer;          /* Size (in KB) of MBU file I/O buffer */
  byte days;                /* Days to keep old mail around */
  byte swapping;            /* Swapping method */
  byte compr_default;       /* Default compresion type */
  byte pmcolor[15];         /* Not used */
  ftn_addr oldaka[OLDAKAS]; /* Main address and AKAs */
  word oldpointnet[OLDAKAS];/* Pointnets for all addresses */
  dword gekey;              /* GEcho registration key */
  dword mbukey;             /* MBUTIL registration key */
  char geregto[51];         /* Text used to generate the GEcho key */
  char mburegto[51];        /* Text used to generate the MBUTIL key */
  char username[USERS][36]; /* User names */
  char hmbpath[53];         /* Hudson message base path */
  char mailpath[53];        /* Netmail path */
  char inbound_path[53];    /* Where incoming compressed mail is stored */
  char outbound_path[53];   /* Where outgoing compressed mail is stored */
  char echotoss_file[65];   /* The ECHOTOSS.LOG used for Squish areas */
  char nodepath[53];        /* Not used */
  char areasfile[65];       /* AREAS.BBS style file */
  char logfile[65];         /* GEcho/MBUTIL log file */
  char mgrlogfile[65];      /* AreaMgr log file */
  char swap_path[53];       /* Swap path */
  char tear_line[31];       /* Tearline to be placed by MBUTIL Export */
  char originline[20][61];  /* Origin lines */
  char compr_prog[10][13];       /* Compression program filenames */
  char compr_switches[10][20];   /* Compression program switches */
  char decompr_prog[10][13];     /* Decompression program filenames */
  char decompr_switches[10][20]; /* Decompression program switches */
  char oldgroups[26][21];   /* Descriptions of area groups */
  byte lockmode;            /* See --- Locking method */
  char secure_path[53];     /* From which secure PKTs are tossed */
  char rcvdmailpath[53];    /* Not used */
  char sentmailpath[53];    /* Not used */
  char semaphorepath[53];   /* Where FD rescan files are stored */
  byte version_major;       /* Major GEcho version */
  byte version_minor;       /* Minor GEcho version */
  byte semaphore_mode;      /* See --- Semaphore modes */
  char badecho_path[53];    /* Where sec. violating and unknown mail is stored */
  byte mailer_type;         /* See --- Mailer type */
  word loglevel;            /* See --- Log level */
  AKAMATCH akamatch[20];    /* AKA matching table */
  char mbulogfile[65];      /* MBUTIL log file */
  word maxqqqs;             /* Max. number of QQQ info stored in memory */
  byte maxqqqopen;          /* Not used */
  byte maxhandles;          /* Max. number of files used by GEcho */
  word maxarcsize;          /* Max. archive size, 0 = unlimited */
  word delfuture;           /* Days to delete messages in the future, 0 = disable */
  word extraoptions;        /* See --- Extra option bits */
  byte firstboard;          /* Not used */
  word reserved1;           /* Reserved */
  word copy_persmail;       /* Not used */
  byte oldpersmailboard[USERS];  /* Personal mail board (0 = path) */
  dword old_public_groups;  /* Public groups (bits 0-25) */
  word dupentries;          /* Number of duplicate entries in ECHODUPE.GE */
  byte oldrcvdboard;        /* Where Rcvd netmail is moved to (0 = path) */
  byte oldsentboard;        /* Where Sent netmail is moved to (0 = path) */
  byte oldakaboard[OLDAKAS];  /* Netmail boards for AKAs */
  byte olduserboard[USERS];  /* Netmail boards for system users, 255 = use AKA board */
  byte reserved2;           /* Reserved */
  OLDUPLINK uplink[OLDUPLINKS];  /* Not used */
  char persmail_path[53];   /* Not used */
  char outpkts_path[53];    /* Where outbound packets are temp. stored */
  word compr_mem[10];       /* Memory needed for compression programs */
  word decompr_mem[10];     /* Memory needed for decompression programs */
  dword pwdcrc;             /* CRC-32 of access password, -1L = no password */
  word default_maxmsgs;     /* Maximum number of messages       (Purge) */
  word default_maxdays;     /* Maximum age of non-Rcvd messages (Purge) */
  char gus_prog[13];        /* General Unpack Shell program filename */
  char gus_switches[20];    /* GUS switches */
  word gus_mem;             /* Memory needed for GUS */
  word default_maxrcvddays; /* Maximum age of Rcvd messages     (Purge) */
  byte checkname;           /* See --- Check user name */
  byte maxareacachesize;    /* Area cache size, 0 .. 64 KB */
  char inpkts_path[53];     /* Where inbound mail packets should be stored */
  char pkt_prog[13];        /* Called before each tossed mail packet */
  char pkt_switches[20];    /* Command line switches */
  word pkt_mem;             /* Memory needed */
  word maxareas;            /* Maximum number of areas */
  word maxconnections;      /* Maximum number of connections per area */
  word maxnodes;            /* Maximum number of nodes */
  word default_minmsgs;     /* Minimum number of messages       (Purge) */
  byte bbs_type;            /* See --- BBS type */
  byte decompress_ext;      /* 0 = 0-9, 1 = 0-F, 2 = 0-Z */
  byte reserved3;           /* Reserved */
  byte change_tearline;     /* See --- Change tear line */
  word prog_notavail;       /* Bit 0-9, 1 = program not available */
  COLORSET gscolor;         /* GSETUP color set, See COLORSET structure */
  byte reserved4[9];        /* Reserved */

  ftn_addr aka[AKAS];        /* Main address and AKAs */
  word pointnet[AKAS];       /* Pointnets for all addresses */
  word akaarea[AKAS];        /* AKA netmail areas */
  word userarea[USERS];      /* Netmail areas for system users, 0 = don't import, 65535 = use AKA area */
  word persmailarea[USERS];  /* Personal mail area (0 = don't copy) */
  word rcvdarea;             /* Rcvd netmail area (0 = don't move) */
  word sentarea;             /* Sent netmail area (0 = don't move) */
  word badarea;              /* Where bad echomail is stored (0 = path) */
  word reserved5;            /* Not used */
  char jampath[53];          /* JAM message base path */
  char userbase[53];         /* User base path */
  char dos4gw_exe[65];       /* DOS4GW.EXE protected mode run time file */
  GROUPS public_groups;      /* Public groups (bits 0-255) */
  word maxgroupconnections;  /* Maximum number of connections per group */
  word maxmsgsize;           /* Maximum message size (64-1024 kB) */
  word diskspace_threshold;  /* Amount of free disk space that causes packing */
  byte pktsort;              /* 0 = No, 1 = Area, 2 = Area + Date/Time */
  char wildcatpath[53];      /* Wildcat! home path */
  byte wcoptions;            /* Wildcat! options */
  byte pcboptions;           /* PCBoard options */
} SETUP_GE;

#define IMPORTSB  0x0001  /* Import SEEN-BY lines to message base */
#define SECURITY  0x0002  /* Only accept mail from nodes in connections list */
#define PASSTHRU  0x0004  /* Mail is not imported, only forwarded */
#define VISIBLE   0x0008  /* Area is visible for anyone in AreaMgr's %LIST */
#define REMOVED   0x0010  /* Area should be removed by GSETUP Pack */
#define NOUNLINK  0x0020  /* Do not allow users to unlink this area */
#define TINYSB    0x0040  /* Tiny SEEN-BYs with only nodes in connections list */
#define PVT       0x0080  /* Private bits are preserved and are not stripped */
#define CHECKSB   0x0100  /* Use SEEN-BYs for duplicate prevention */
#define NOPAUSE   0x0200  /* Do not allow users to pause this area */
#define SDM       0x0400  /* Area is stored in *.MSG format */
#define HIDESB    0x0800  /* Hide imported SEEN-BY lines */
#define NOIMPORT  0x1000  /* AreaMgr will set new nodes to Export-Only */
#define DELFUTURE 0x2000  /* Del messages dated in the future */
#define NOTIFIED  0x4000  /* Sysop notified that area was disconnected */
#define UPLDISC   0x8000  /* Disconnected from uplink (only for PT areas) */


#define NODUPECHK 0x01  /* Don't do duplicate checking for this area */
#define NOLINKING 0x02  /* Don't do reply chain linking for this area */
#define HIDDEN    0x04  /* Area is hidden for everyone */


#define ECHOMAIL   0
#define NETMAIL    1
#define LOCAL      2
#define BADECHO    3
#define PERSONAL   4
#define NUM_TYPES  5


#define FORMAT_PT      0  /* Passthru */
#define FORMAT_HMB     1  /* Hudson Message Base */
#define FORMAT_SDM     2  /* *.MSG base */
#define FORMAT_JAM     3  /* Joaquim-Andrew-Mats message base proposal */
#define FORMAT_PCB     4  /* PCBoard 15.0 */
#define FORMAT_SQUISH  5  /* Squish 2.0 */
#define FORMAT_WC      6  /* Wildcat! 4.0 */
#define NUM_FORMATS    7


typedef struct {
  word hdrsize;         /* sizeof(AREAFILE_HDR) */
  word recsize;         /* sizeof(AREAFILE_GE) */
  word maxconnections;  /* Maximum number of entries in connections list */
} AREAFILE_HDR;


typedef struct {
  char name[51];       /* Area name must be uppercase, no spaces */
  char comment[61];    /* Description of the topics discussed in area */
  char path[51];       /* Path where the *.MSG files are stored */
  char originline[61]; /* Custom origin line, used if origlinenr = 0 */
  word areanumber;     /* Area number (1-200 = Hudson) */
  byte group;          /* Group (0-255) */
  word options;        /* See --- Area option bits */
  byte originlinenr;   /* Origin line (1-20, 0 = custom) */
  byte pkt_origin;     /* Address for the packet/Origin line (0-31) */
  dword seenbys;       /* Addresses (bits 0-31) to add to the SEEN-BY */
  word maxmsgs;        /* Maximum number of messages       (MBUTIL Purge) */
  word maxdays;        /* Maximum age of non-Rcvd messages (MBUTIL Purge) */
  word maxrcvddays;    /* Maximum age of Rcvd messages     (MBUTIL Purge) */
  byte areatype;       /* See --- Area type */
  byte areaformat;     /* See --- Area format */
  byte extraoptions;   /* See --- Extra area option bits */
} AREAFILE_GE;

/* --- Connection entry status bits */

#define CONN_NOIMPORT  0x01  /* Don't accept mail from this node */
#define CONN_NOEXPORT  0x02  /* Don't forward mail to this node */
#define CONN_PAUSE     0x04  /* Temporary don't send this area to this node */
#define CONN_NOUNLINK  0x08  /* Don't allow this node to disconnect */
#define CONN_ISUPLINK  0x10  /* Node is uplink for this area */

/* --- Connections list entry */

typedef struct {
  ftn_addr address;
  byte     status;
} CONNECTION;


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack()
#endif


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
