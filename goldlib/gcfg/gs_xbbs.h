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

#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif

//  ------------------------------------------------------------------

#define SMALLEST_MSG_SIZE_FOR_PACKING   384

#define MAX_MSG_AREAS  10240
#define MAX_FIL_AREAS  10240

/* bbs.attribs */
	#define B_LOGALLHELP      0x00000001
	#define B_PREREGISTERED   0x00000002
	#define B_NOPRETTY        0x00000004
	#define B_INTL            0x00000008
	#define B_NOHAPPY         0x00000010
	#define B_ANSINEWS        0x00000020
	#define B_NODUPENUMS      0x00000040
	#define B_COLOR           0x00000080
	#define B_HIGHASCII       0x00000100
	#define B_PAUSE           0x00000200
	#define B_CLEAR           0x00000400
	#define B_COLD            0x00000800
	#define B_WIDTH           0x00001000
	#define B_LENGTH          0x00002000
	#define B_SHOW            0x00004000
	#define B_PHONE           0x00008000
	#define B_ADDRESS         0x00010000
	#define B_HANDLE          0x00020000
	#define B_PASSWORD        0x00040000
	#define B_INTERESTS       0x00080000
	#define B_COMPUTERS       0x00100000
	#define B_NO_NEW_EDIT     0x00200000
	#define B_LOGONQUOTE      0x00400000
	#define B_COMPRESS        0x00800000
	#define B_LOGOFFQUOTE     0x01000000
	#define B_LOGOFFMSG       0x02000000
	#define B_FIRSTLOGMSG     0x04000000
	#define B_FIRSTLOGCALLBK  0x08000000
	#define B_ALWAYSCALLBACK  0x10000000
	#define B_AUTOPASSWORDS   0x20000000
	#define B_FOLLOWBBS       0x40000000
	#define B_SYSOPIN         0x80000000
	#define BIT_1             0x00000001
	#define BIT_2             0x00000002
	#define BIT_3             0x00000004
	#define BIT_4             0x00000008
	#define BIT_5             0x00000010
	#define BIT_6             0x00000020
	#define BIT_7             0x00000040
	#define BIT_8             0x00000080
	#define BIT_9             0x00000100
	#define BIT_10            0x00000200
	#define BIT_11            0x00000400
	#define BIT_12            0x00000800
	#define BIT_13            0x00001000
	#define BIT_14            0x00002000
	#define BIT_15            0x00004000
	#define BIT_16            0x00008000
	#define BIT_17            0x00010000
	#define BIT_18            0x00020000
	#define BIT_19            0x00040000
	#define BIT_20            0x00080000
	#define BIT_21            0x00100000
	#define BIT_22            0x00200000
	#define BIT_23            0x00400000
	#define BIT_24            0x00800000
	#define BIT_25            0x01000000
	#define BIT_26            0x02000000
	#define BIT_27            0x04000000
	#define BIT_28            0x08000000
	#define BIT_29            0x10000000
	#define BIT_30            0x20000000
	#define BIT_31            0x40000000
	#define BIT_32            0x80000000

/* bbs.attribs2 */
	#define B2_ANSI           0x00000001
	#define B2_USEVER7        0x00000002
	#define B2_AUTOVERIFY     0x00000004
	#define B2_FTNTOALL       0x00000008
	#define B2_MINIMIZEBBS    0x00000010
	#define B2_SMALLFONT      0x00000020
	#define B2_DATAPHONE      0x00000040
	#define B2_FAXPHONE       0x00000080
	#define B2_BUSPHONE       0x00000100
	#define B2_ADDRESS2       0x00000200
	#define B2_ADDRESS3       0x00000400
	#define B2_CITY           0x00000800
	#define B2_STATE          0x00001000
	#define B2_ZIPCODE        0x00002000
	#define B2_USEHANDLE      0x00004000
	#define B2_NOVIDWHENMIN   0x00008000
	#define B2_ABORTREXX      0x00010000
	#define B2_NOBBSSCREEN    0x00020000
	#define B2_CANFASTLOGIN   0x00040000
	#define B2_ADDRESS1REQ    0x00080000
	#define B2_ADDRESS2REQ    0x00100000
	#define B2_ADDRESS3REQ    0x00200000
	#define B2_CITYREQ        0x00400000
	#define B2_STATEREQ       0x00800000
	#define B2_ZIPREQ         0x01000000
	#define B2_HOMEREQ        0x02000000
	#define B2_DATAREQ        0x04000000
	#define B2_BUSINESSREQ    0x08000000
	#define B2_FAXREQ         0x10000000
	#define B2_NOREGTXTINMSG  0x20000000              //JM960109
	#define B2_NUMONLYPHONE   0x40000000              //JM960109
	#define B2_ALWAYSSAVEINI  0x80000000              //JM960109

/* bbs.attribs3 */
	#define B3_UNVERIFIED_NEWMENU    	0x00000001
	#define B3_NO_NEWUSERALIAS    		0x00000002
	#define B3_DROP_CARRIER_ON_TIMEUP	0x00000004
	#define B3_BBS_WATCH_THREAD		0x00000008
	#define B3_NO_TELNET_TIMEOUT		0x00000010
	#define B3_ENABLE_ACCOUNTING		0x00000020
	#define B3_ENABLE_EXCEPTIONS		0x00000040
	#define B3_FLUSH_SYSTEM			0x00000080
	#define B3_FLUSH_SYSTEM_LOG		0x00000100
	#define B3_NOSTATUS			0x00000200
	#define B3_STARTFTP			0x00000400
	#define B3_NOCHAT			0x00000800
	#define B3_SYSTEM_INFO			0x00001000
	#define B3_BBS_INFO			0x00002000
	#define B3_SYSOP_MAIL			0x00004000
	#define B3_FULL_LANG_BUF		0x00008000
	#define B3_ALLOW_KILL			0x00010000
	#define B3_POP_KEEP			0x00020000


/* user attribs */
	#define U_ANSI            0x00000001
	#define U_COLOR           0x00000002
	#define U_HIGHASCII       0x00000004
	#define U_NOMORE          0x00000008
	#define U_NOCLEAR         0x00000010
	#define U_COLD            0x00000020
	#define U_NOSHOWADDRESS   0x00000040
	#define U_NOSHOWPHONE     0x00000080
	#define U_NOSHOWREAL      0x00000100
	#define U_NOKILL          0x00000200
	#define U_DELETED         0x00000400
	#define U_EXPERT          0x00000800
	#define U_TWIT            0x00001000
	#define U_NEWFILES        0x00002000
	#define U_NEWMSGS         0x00004000
	#define U_DLNEWMSGS       0x00008000
	#define U_DLNEWFILELIST   0x00010000
	#define U_SEENEWS         0x00020000
	#define U_ASKDL           0x00040000
	#define U_NOVICE          0x00080000
	#define U_NOROLL          0x00100000
	#define U_POINTONLY       0x00200000
	#define U_LOCKEDOUT       0x00400000
	#define U_AVAILABLE       0x00800000
	#define U_LISTPROTECTED   0x01000000
	#define U_MISCNEWSOK      0x02000000
	#define U_LIMITFLIST      0x04000000
	#define U_EBCDIC          0x08000000
	#define U_VERIFIED        0x10000000              // user call back verified, turn this off, they get

// verified again.. (if BBS verifies)
	#define U_ANSITE2         0x20000000
	#define U_RIPSCRIPT       0x40000000
	#define U_AVATAR          0x80000000

/* user attribs2*/
	#define U2_FSE            0x00000001
	#define U2_OFF_AFTER_MAIL 0x00000002
	#define U2_MONITOR_MAIL   0x00000004
	#define U2_INT_NEW_FILES  0x00000008
	#define U2_INC_BULLETS    0x00000010
	#define U2_INC_WELCOME    0x00000020
	#define U2_DEFAULT_QWK    0x00000040
	#define U2_DEFAULT_FIDO   0x00000080
	#define U2_DEFAULT_TEXT   0x00000100
	#define U2_DEFAULT_BLUE   0x00000200
	#define U2_AVTANS         0x00000400
	#define U2_8DOT3          0x00000800              //JM940610
	#define U2_NOSHOWBDAY     0x00001000
	#define U2_ALLOW_NET_ATTACH 0x00002000
	#define U2_ALLOW_LOCAL_ATTACH 0x00004000
	#define U2_NO_ALT_ED      0x00008000
	#define U2_POP_SCAN       0x00010000
	#define U2_KEEP_SEENBY    0x00020000
	#define U2_ALLOW_KILL     0x00040000
	#define U2_POP_KEEP       0x00080000


/* Used by string_input */
/* flags */
	#define STRF_UCASE        0x00000001
	#define STRF_LCASE        0x00000002
	#define STRF_REQUIRED     0x00000004
	#define STRF_NOECHO       0x00000008
	#define STRF_ENCRYPT      0x00000010
	#define STRF_HOT          0x00000020
	#define STRF_EDIT         0x00000040
	#define STRF_PRETTY       0x00000080
	#define STRF_MUSTHOT      0x00000100
	#define STRF_NEAT         0x00000200
	#define STRF_NOHELP       0x00000400
	#define STRF_NOQUE        0x00000800
	#define STRF_NOQUEONE     0x00001000
	#define STRF_CHATCLNT     0x00002000
	#define STRF_NOTIMECHECK  0x00004000
	#define STRF_NOANSIINCHECK 0x00008000
	#define STRF_ENTEROK      0x00010000
	#define STRF_ALWAYSCHAT   0x00020000              //JM960202
	#define STRF_NOFIRSTPROMPT 0x00040000              //JM960202

/* types */
	#define STRT_ALPHA        1
	#define STRT_NUMERIC      2
	#define STRT_ALPHANUMERIC 3
	#define STRT_HEX          4
	#define STRT_FILENAME     5
	#define STRT_FILESPEC     6
	#define STRT_ALL          7
	#define STRT_ANY          8
	#define STRT_YN           9
	#define STRT_MULTFILES    10
	#define STRT_MULTSPECS    11
	#define STRT_YNQ          12
	#define STRT_YNQS         13
	#define STRT_JUSTFILE     14
	#define STRT_MULTJUST     15
	#define STRT_TELEPHONE    16
	#define STRT_YNNUM        17
	#define STRT_TELEPHONENUM 18                      //JM960109

// Input flags (input, as in do_more, wait_inkey, etc)
	#define IF_RETURNONERROR  0x00000001              // return lost carrier, or timeout
	#define IF_NOQUEUE        0x00000002
	#define IF_NOTIMECHECK    0x00000004
	#define SINGLE_INPUT      0x00000008
	#define PASSWORD_PROMPT   0x00000010

// Input return codes (along with TIMEOUT and LOSTCARRIER)
	#define IRC_NOTIMELEFT    -3
	#define IRC_LOCKEDOUT     -4
	#define IRC_MORON         -5
	#define IRC_REPROMPT      -6
	#define IRC_ENDSYSOPCHAT  -7
	#define IRC_CHATWAITING   -8
	#define IRC_CHATDOWN      -9
	#define IRC_IDLETIMEOUT   -10

#if 0
struct _msgarea
	{
	char            *name;
	char            *descr;
	char            *forceto;
	char            *origin;
	char            *path;
	char            *usenettag;
	int             minage;
	int             maxage;
	unsigned long   r_security1;
	unsigned long   r_security2;
	unsigned long   r_flags1;
	unsigned long   r_flags2;
	unsigned long   w_security1;
	unsigned long   w_security2;
	unsigned long   w_flags1;
	unsigned long   w_flags2;
	unsigned long   s_security1;
	unsigned long   s_security2;
	unsigned long   s_flags1;
	unsigned long   s_flags2;
	unsigned long   areaflags;
	unsigned long   *groupnum;
        PADDR           addr;
	PADDR           feeds;
	unsigned int    number;
	LONG            TimeFactor;
	char            *password;
	struct _msgarea *next;
	struct _msgarea *prior;
	};

typedef struct _msgarea MSGAREA, *PMSGAREA;		  //JM931019
#endif

/*----------------------------------------------------------*/
/* Message area flags (areaflags)                           */
/*----------------------------------------------------------*/
	#define M_NOORIG     0x00000001                   /* No Origin or tear       */
	#define M_ANSI       0x00000002                   /* Allow Ansi Messages     */
	#define M_PRIVATE    0x00000004                   /* Force Msgs Public       */
	#define M_PUBLIC     0x00000008                   /* Force Msgs Private      */
	#define M_ECHO       0x00000010                   /* Is a Echomail           */
	#define M_NET        0x00000020                   /* It a Netmail area       */
	#define M_MCI        0x00000040                   /* Allows MCI Commands     */
	#define M_ANON       0x00000080                   /* All Msgs Annon.         */
	#define M_REAL       0x00000100                   /* Requires Real Name      */
	#define M_ASSOC      0x00000200                   /* Msg area allows assoc   */
	#define M_FORCE      0x00000400                   /* Always force            */
	#define M_COMPRESS   0x00000800                   /* Allow LZSS comp here    */
	#define M_GROUP      0x00001000                   /* groupmail               */
	#define M_USENET     0x00002000                   /* usenet conference       */
	#define M_QWK        0x00004000                   /* qwk conference          */
	#define M_SQUISH     0x00008000                   /* Squish MSG Base         */
	#define M_EMAIL      0x00010000                   /* Internet E-Mail         */
	#define M_FIDO       0x00100000                   /* Star . msg Format       */
	#define M_JAM        0x00200000                   /* JAM msg Format          */


struct _filearea
	{
	char             *name;
	char             *descr;
	int             minage;
	int             maxage;
	unsigned long    r_security1;
	unsigned long    r_security2;
	unsigned long    r_flags1;
	unsigned long    r_flags2;
	unsigned long    w_security1;
	unsigned long    w_security2;
	unsigned long    w_flags1;
	unsigned long    w_flags2;
	unsigned long    s_security1;
	unsigned long    s_security2;
	unsigned long    s_flags1;
	unsigned long    s_flags2;
	unsigned long    areaflags;
	unsigned long    groupnum[256];
	int              cost_per_k;
	unsigned int     number;
	char             *upath;
	char             *dpath;
	char             *password;
	char             *ftpname;
	LONG             TimeFactor;
	struct _filearea *next;
	struct _filearea *prior;
	};


typedef struct _filearea FILEAREA, *PFILEAREA;	  //JM931019

/*-----------------------------------------------------------*/
/* File area flags (areaflags)                               */
/*-----------------------------------------------------------*/
	#define F_FREE       0x00000001                   /*                           */
	#define F_NOUP       0x00000002                   /*                           */
	#define F_NODOWN     0x00000004                   /*                           */
	#define F_LEECH      0x00000008                   /*                           */
	#define F_DOWNBACK   0x00000010                   /*                           */
	#define F_UPBACK     0x00000020                   /*                           */
	#define F_CHARGE     0x00000040                   /*                           */
	#define F_CANTLIST   0x00000080                   // Uploads not listable
	#define F_CANTDL     0x00000100                   // Uploads not downloadable
	#define F_NOFBBS     0x00000200                   /* no FILES.BBS at all       */
	#define F_COMMONONLY 0x00000400                   /* use only common FILES.BBS */
	#define F_UPBLIND    0x00000800                   // UL get desc. afterwards
	#define F_COPYFILE   0x00001000                   // copy download from CD to HD
	#define F_NODIR      0x00002000                   // copy download from CD to HD
	#define F_NOCOMMON   0x00004000                   // copy download from CD to HD
	#define F_NOSEARCH   0x00008000                   // copy download from CD to HD


typedef struct _libarea
	{
	char            *name;
	char            *descr;
	int             minage;
	int             maxage;
	unsigned long   r_security1;
	unsigned long   r_security2;
	unsigned long   r_flags1;
	unsigned long   r_flags2;
	unsigned long   w_security1;
	unsigned long   w_security2;
	unsigned long   w_flags1;
	unsigned long   w_flags2;
	unsigned long   s_security1;
	unsigned long   s_security2;
	unsigned long   s_flags1;
	unsigned long   s_flags2;
	unsigned long   areaflags;
	unsigned int    number;
	char            *path;
	char            *password;
	struct _libarea *next;
	struct _libarea *prior;
	} LIBAREA, *PLIBAREA;


/*----------------------------------------------------------*/
/* Library area flags (areaflags)                           */
/*----------------------------------------------------------*/
	#define L_ANSIONLY   0x00000001                   /*                         */
// Group structure


typedef struct _GROUP
	{
	PSZ     Name;
	PSZ     Password;
	LONG    Number;
	ULONG   Flags;
	ULONG   MinutesPerDay;
	ULONG   MinutesPerCall;
	ULONG   DaysBeforeExpiration;
	LONG    ExpireToGroup;
	ULONG   MaxBankTime;
	ULONG   MaxUKPerDay;
	ULONG   MaxDKPerDay;
	ULONG   ULDLRatio;
	ULONG   RatioCredit;
	ULONG   Security1;
	ULONG   Security2;
	ULONG   Flags1;
	ULONG   Flags2;
	int     CreditsPerUnit;
	int     CreditsPerUnitPeek;
	int     FreeCreditsDay;
	int     FreeCreditsWeek;
	int     FreeCreditsMonth;
	int     FreeCreditsPercentage;
	int     MaxDebt;
	LONG    GroupIfBroke;
	USHORT  DaysBetweenPWChange;
	PSZ     MainMenu;
	PSZ	MainMenuUnVerified;
	struct _GROUP *Next;
	struct _GROUP *Prev;
	} GROUP, *PGROUP;


// Group Flags
	#define G_FLAGSOVERWRITE   0x00000001             // if not overwrite, then accumulate
	#define G_SECOVERWRITE     0x00000002             // if not overwrite, then ignore
	#define G_RATIOONBYTES     0x00000004             // if not bytes, then files
	#define G_TIMEOVERWRITE    0x00000008             // if not overwrite, then ignore
///// New File Search Info


typedef struct _user_info
	{
	CHAR        name[36];
	CHAR        handle[36];
	CHAR        address[60];
	CHAR        address1[60];
	CHAR        address2[60];
	CHAR        city[20];
	CHAR        state[5];
	CHAR        zipcode[10];
	CHAR        home_phone[21];
	CHAR        data_phone[21];
	CHAR        fax_phone[21];
	CHAR        bus_phone[21];
	CHAR        interests[78];
	CHAR        SysOpComment[78];
	USHORT      BankTime;
	USHORT      computer_type;
	ULONG       userid;
	USHORT      pointid;
	time_t      initial_logon;
	time_t      expires;
	time_t      last_logon;
	time_t      logon_today;
	USHORT      birthyear;
	UCHAR       birthmonth;
	UCHAR       birthday;
	UCHAR       Gender;
	LONG        credit;
	LONG        minlimit;								 // what exactly is this?
	LONG        Group;				 // must match Group->Number
	ULONG       CurrLastRead;							 // last Read for current msg area
	ULONG       time_today;								 // seconds used today
	ULONG       time_per_day;							 // minutes allowed per day
	ULONG       time_per_call;							 // minutes per call  0=just per day
	ULONG       numcalls;
	ULONG       ulnum;									 // files uploaded (ever)
	ULONG       dlnum;									 // files downloaded (ever)
	ULONG       ulk;										 // UL in K (ever)
	ULONG       dlk;										 // DL in K (ever)
	ULONG       uktoday;									 // UL in K Today
	ULONG       dktoday;									 // DL in K Today
	ULONG       ukperday;								 // Max UL K Per day
	ULONG       dkperday;								 // Max DL K Per day
	ULONG       numposts;
	ULONG       security1;
	ULONG       security2;
	ULONG       flags1;
	ULONG       flags2;
	ULONG       attribs;
	ULONG       attribs2;
	USHORT      length;
	USHORT      width;
	USHORT      lastmsgarea;
	USHORT      lastfilearea;
	UCHAR       lastprotocol;
	UCHAR       lastarchiver;
	LONG        forsysop;
	UCHAR       interface;
	UCHAR       exp;
	USHORT      leech_percent;
	USHORT      MaxBankTime;
	SHORT       TimeCredit;								 // can be negative or positive
	SHORT       CallsToday;
	LONG        TotalMinsEver;
	LONG        NetmailDebits;
	time_t      LastPWChange;
	time_t      LastListedNewFiles;					 //JM940822
	ULONG       FileKCredit;
	CHAR        UnixId[8];
	CHAR        Country[24];
	USHORT      weight;
	CHAR        height[5];
	CHAR        Pref;
	CHAR        LangIdent[4];							 // permanent language identifier
	USHORT      MailDLsToday;							 //JM960118
	USHORT      UsesOLMail;								 //JM960119
	CHAR       POPPassword[16];
	CHAR       POPLogin[16];
	CHAR       POPServer[128];
	CHAR        NoLongerUsed[22];
	UCHAR       _UnUsedOldTags[256];
	ULONG       PasswordCRC;
	ULONG       PasswordCRC2;
	USHORT      Max_Mail_Pkt;
	CHAR        ReAsked;									 // TRUE if reasked question were answered
	USHORT      timeout;
	USHORT      CurrMsgGroup;
	USHORT      CurrFileGroup;
	CHAR        EMail[37];
	CHAR        FidoMail[32];
	} USER, *PUSER;


typedef struct _xmsg
	{
	CHAR    MajorVersion;
	CHAR    MinorVersion;
	USHORT  StructLen;
	CHAR    from[60];
	CHAR    to[60];
	CHAR    subj[70];
	CHAR    date[35];										 /*   */
	CHAR    indate[4];									 /* import date  (YMD(null))                     */
	ULONG   msgnum;
	ULONG   timesread;
	time_t  timerecv;
	ULONG   length;										 /* Length of message */
	LONG    start;											 /* Starting postition in text file              */
	ULONG   OrigMsg;
	ULONG   miscdatlen;
	ULONG   miscdatstart;
	USHORT  o_zone;
	USHORT  o_net;
	USHORT  o_node;
	USHORT  o_point;
	USHORT  d_zone;
	USHORT  d_net;											 /* Destination network number                   */
	USHORT  d_node;										 /* Destination node                             */
	USHORT  d_point;
	USHORT  cost;											 /* Unit cost charged to send the message        */
	USHORT  fflags;										 /* Attribute (behavior) of the message          */
	USHORT  xflags;										 /* Extra attributes                             */
	ULONG   bflags;
	ULONG   cflags;
	} XMSG, *PXMSG;


/*----------------------------------------------------*/
/* FIDO Message attributes (fflags)                   */
/*----------------------------------------------------*/
	#define MSGPRIVATE 0x0001                         /* private message,        */
	#define MSGCRASH   0x0002                         /* accept for forwarding   */
	#define MSGREAD    0x0004                         /* read by addressee       */
	#define MSGSENT    0x0008                         /* sent OK (remote)        */
	#define MSGFILE    0x0010                         /* file attached to msg    */
	#define MSGFWD     0x0020                         /* being forwarded         */
	#define MSGORPHAN  0x0040                         /* unknown dest node       */
	#define MSGKILL    0x0080                         /* kill after mailing      */
	#define MSGLOCAL   0x0100                         /* FidoNet vs. local       */
	#define MSGXX1     0x0200                         /*                         */
	#define MSGXX2     0x0400                         /* STRIPPED by FidoNet<tm> */
	#define MSGFRQ     0x0800                         /* file request            */
	#define MSGRRQ     0x1000                         /* receipt requested       */
	#define MSGCPT     0x2000                         /* is a return receipt     */
	#define MSGARQ     0x4000                         /* audit trail requested   */
	#define MSGURQ     0x8000                         /* update request          */

/*----------------------------------------------------*/
/*----------------------------------------------------*/
/* Adept Message attributes (xflags)                  */
/*----------------------------------------------------*/
	#define MSGDELETED 0x0001                         /* deleted message,          0000 0000 0000 0001*/
	#define MSGANON    0x0002                         /* anonymous message         0000 0000 0000 0010*/
	#define MSGECHO    0x0004                         /* unmoved echo message      0000 0000 0000 0100*/
	#define MSGNET     0x0008                         /* unmoved net message       0000 0000 0000 1000*/
	#define MSGHOLD    0x0010                         /* file attached to msg      0000 0000 0001 0000*/
	#define MSGHOST    0x0020                         /* being forwarded           0000 0000 0010 0000*/
	#define MSGSCANNED 0x0040                         /* fidonet scanned           0000 0000 0100 0000*/
	#define MSGKEEP    0x0080                         /* don't delete              0000 0000 1000 0000*/
	#define MSGTREATED 0x0100                         /* soft cr's & lf's removed  0000 0001 0000 0000*/
	#define MSGPACKED  0x0200                         /* message LZSS compressed   0000 0010 0000 0000*/
	#define MSGGSCAN   0x0400                         /* groupmail scanned         0000 0100 0000 0000*/
	#define MSGRSCAN   0x0800                         /* rfc0822 scanned           0000 1000 0000 0000*/
/*#define    0x1000      0001 0000 0000 0000*/
/*#define    0x2000      0010 0000 0000 0000*/
	#define MSGARCHIVED 0x4000                        /*                           0100 0000 0000 0000*/
	#define MSGTAGGED  0x8000                         /* used by offline readers   1000 0000 0000 0000*/
/*----------------------------------------------------*/

#if defined(GOLD_CANPACK)
#pragma pack()
#endif

//  ------------------------------------------------------------------
