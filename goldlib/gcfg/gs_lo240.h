//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (c) 1989-1994 by Marco Maccaferri
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
//  LoraBBS Ver. 2.40 structures
//  ------------------------------------------------------------------

#ifndef __gs_lora240_h
#define __gs_lora240_h


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
    #pragma pack(1)
#endif


//  ------------------------------------------------------------------

#define MAXFLAGS   4

#define SIZEOF_MSGAREA    512
#define SIZEOF_FILEAREA   640

struct _sysmsg
{
    /* Structure for the internal system records.
                            The files SYSMSG.DAT and SYSFILE.DAT
                            are composed by the respective portions of this
                            structure */

    char  msg_name[70];      /* Message area name */
    short msg_num;           /* Message area number */
    char  msg_path[40];      /* Message area path */
    char  origin[56];        /* Echomail origin line */
    bits  echomail  :1;      /* 1=Echomail area */
    bits  netmail   :1;      /* 1=Netmail area */
    bits  dopublic  :1;      /* 1=Public message allowed */
    bits  doprivate :1;      /* 1=Private message allowed */
    bits  anon_ok   :1;      /* 1=Use the user's handle in the from field */
    bits  no_matrix :1;      /* <reserved> (**) */
    bits  squish    :1;      /* Squish<tm> message area */
    bits  kill_unlisted :1;  /* <reserved> (**) */
    word msg_sig;           /* Message area's group */
    char  echotag[32];       /* Echotag name */
    word pip_board;         /* Pip-base board number */
    byte  quick_board;       /* QuickBBS board number */
    byte  msg_priv;          /* Minimum level to access this area */
    long  msg_flags;         /* Flags settings */
    byte  write_priv;        /* Minimum level to write messages */
    long  write_flags;       /* Flags settings */
    byte  use_alias;         /* Use alias # with this area */
    short max_msgs;          /* Maximum number of messages in this area */
    short max_age;           /* Max age of messages (days) */
    short age_rcvd;          /* Max age of received messages (days) */
    char  forward1[80];      /* Forward node list 1/3 */
    char  forward2[80];      /* Forward node list 2/3 */
    char  forward3[80];      /* Forward node list 3/3 */
    bits  msg_restricted :1; /* Access restricted to message area's group */
    bits  passthrough    :1; /* Pass-through area */
    byte  areafix;           /* Areafix access level for that area */
    char  qwk_name[14];      /* Short name used by the QWK mail packer */
    long  afx_flags;         /* Areafix flags */
    word gold_board;        /* Goldbase board number */
    char  filler1[27];
};

/* Privilege levels definitions */

#define TWIT        0x10
#define DISGRACE    0x20
#define LIMITED     0x30
#define NORMALx     0x40
#define WORTHY      0x50
#define PRIVIL      0x60
#define FAVORED     0x70
#define EXTRA       0x80
#define CLERK       0x90
#define ASSTSYSOP   0xA0
#define SYSOP       0xB0
#define HIDDEN      0xC0

#define NOCHANGE        0
#define WFC             1
#define LOGIN           2
#define BROWSING        3
#define UPLDNLD         4
#define READWRITE       5
#define DOOR            6
#define CHATTING        7
#define QUESTIONAIRE    8
#define QWKDOOR         9

struct _useron                 /* Structure for the USERON.BBS file */
{
    char   name[36];             /* User's name */
    short  line;                 /* Task number currently on */
    long   baud;                 /* Baud rate */
    char   city[26];             /* User's city */
    bits   donotdisturb: 1;      /* Do not disturb flag */
    bits   priv_chat   : 1;      /* <reserved> (**) */
    short  line_status;          /* User's status (numeric value) */
    short  cb_channel;           /* CB Chat System channel */
    char   status[32];           /* User's status (string) */
};

struct _lastcall               /* Structure for the LASTCALL.BBS file */
{

    short line;                  /* Task number */
    char  name[36];              /* User's name */
    char  city[26];              /* User's city */
    word  baud;                  /* Baud rate */
    long  times;                 /* Times the user has called the system */
    char  logon[6];              /* Logon time HH:MM */
    char  logoff[6];             /* Logoff time HH:MM */
    long  timestamp;             /* Logout time-stamp (binary format) */
};

#define MAX_ALIAS        20
#define MAX_LANG         20

struct _alias
{
    short zone;              /* Zone number */
    short net;               /* Net number */
    short node;              /* Node number */
    short point;             /* Point number */
    word  fakenet;           /* Fake net number */
    char  *domain;           /* <reserved> (**) */
};

struct class_rec
{
    short priv;              /* Access level */
    short max_time;          /* Dayly time limit */
    short max_call;          /* Per call time limit */
    short max_dl;            /* Default download limit */
    short dl_300;            /* Speed specific download limit */
    short dl_1200;           /* Speed specific download limit */
    short dl_2400;           /* Speed specific download limit */
    short dl_9600;           /* Speed specific download limit */
    word  ratio;             /* Download/Upload ratio */
    word  min_baud;          /* Minimum login speed */
    word  min_file_baud;     /* Minimum file transfer speed */
    word  start_ratio;       /* Start ratio after some downloads */
};

struct _configuration               /* Structure for the CONFIG.DAT file */
{

    short version;                    /* Configuration file version */

    char  sys_path[40];               /* System directory */

    char  log_name[40];               /* Log file name */
    char  log_style;                  /* Type of log: 0=Verbose, 1=Short */

    char  sched_name[40];             /* Scheduler file name */
    char  user_file[40];              /* User's file base name (w/o extension) */

    char  norm_filepath[40];          /* Unknow nodes inbound */
    char  know_filepath[40];          /* Know nodes inbound */
    char  prot_filepath[40];          /* Protected nodes inbound */
    char  outbound[40];               /* Outbound base directory */
    char  netmail_dir[40];            /* Netmail directory */
    char  bad_msgs[40];               /* Bad messages directory */
    char  dupes[40];                  /* Dupe messages directory */
    char  quick_msgpath[40];          /* QuickBBS (Hudson) message base path */
    char  pip_msgpath[40];            /* Pip-base path */
    char  ipc_path[40];               /* Temporary files storage */
    char  net_info[40];               /* Nodelist directory */
    char  glob_text_path[40];         /* Text files path */
    char  menu_path[40];              /* Menu/Languages path */
    char  flag_dir[40];               /* Activity flags directory */

    long  keycode;                    /* Registration key code (<2.33) */

    char  about[40];                  /* 'ABOUT' file to send */
    char  files[40];                  /* 'FILES' file to send */
    short norm_max_kbytes;            /* Unknow nodes f/req. kbytes limit */
    short prot_max_kbytes;            /* Protected nodes f/req. kbytes limit */
    short know_max_kbytes;            /* Know nodes f/req. kbytes limit */
    short norm_max_requests;          /* Unknow nodes f/req. files limit */
    short prot_max_requests;          /* Protected nodes f/req. files limit */
    short know_max_requests;          /* Know nodes f/req. files limit */
    char  def_pack;                   /* Default packer */

    char  enterbbs[70];               /* 'Press ESC to enter the BBS' */
    char  banner[70];                 /* Initial mailer banner */
    char  mail_only[70];              /* Mail only banner */

    short com_port;                   /* Serial port (0-based) */
    short old_speed;                  /* Obsolete */
    char  modem_busy[20];             /* Modem busy command */
    char  dial[20];                   /* Modem default dial command */
    char  init[40];                   /* Modem initialization command */
    char  term_init[40];              /* Terminal mode initialization command */

    byte mustbezero;                  /* Must be zero */

    byte echomail_exit;               /* <reserved> (**) */
    byte netmail_exit;                /* <reserved> (**) */
    byte both_exit;                   /* <reserved> (**) */

    long speed;                       /* Modem initial speed */

    byte filler[12];                  /* <reserved> (**) */

    struct _altern_dial               /* Alternative dialing commands */
    {
        char prefix[20];                /* Prefix command */
        char suffix[20];                /* Suffix command */
    } altdial[10];

    bits  lock_baud   :1;             /* 1=Lock serial port to speed baud */
    bits  ansilogon   :2;             /* 0=No ansi at logon, 1=Detect, 2=Ask, 3=Yes */
    bits  birthdate   :1;             /* 1=Ask for birthdate */
    bits  voicephone  :1;             /* 1=Ask for voice phone */
    bits  dataphone   :1;             /* 1=Ask for data phone */
    bits  emsi        :1;             /* 1=EMSI sessions enabled */
    bits  ibmset      :1;             /* 1=Ask for IBM set display */

    bits  wazoo       :1;             /* 1=WAZOO sessions enabled */
    bits  msgtrack    :1;             /* <reserved> (**) */
    bits  keeptransit :1;             /* 1=Keep in-transit netmail */
    bits  hslink      :1;             /* 1=HS-Link enabled */
    bits  puma        :1;             /* 1=Puma enabled */
    bits  secure      :1;             /* 1=Check import messages */
    bits  janus       :1;             /* 1=Janus protocol enabled */
    bits  terminal    :1;             /* 1=Null modem connections */

    bits  fillerbug   :1;             /* <reserved> (**) */
    bits  no_direct   :1;             /* 1=Use BIOS screen writes */
    bits  snooping    :1;             /* 1=Snoop user on-line */
    bits  snow_check  :1;             /* 1=CGA snow check */
    bits  unpack_norm :1;             /* <reserved> (**) */
    bits  unpack_know :1;             /* <reserved> (**) */
    bits  unpack_prot :1;             /* <reserved> (**) */

    short blank_timer;                /* Screen blanker timeout in minutes */

    struct _language                  /* Languages available for the users */
    {
        char txt_path[30];              /* Text files directory */
        char descr[24];                 /* Description */
        char basename[10];              /* Base file name */
    } language[MAX_LANG];

    char  sysop[36];                  /* Sysop name */
    char  system_name[50];            /* System name */
    char  location[50];               /* System's location */
    char  phone[32];                  /* System's phone number */
    char  flags[50];                  /* System's flags */

    struct _alias alias[MAX_ALIAS];   /* Network addresses */

    char  newareas_create[50];        /* Nodes that can create new areas */
    char  newareas_link[50];          /* Nodes that are linked to new areas */

    short line_offset;                /* Task number */
    short min_calls;                  /* <reserved> (**) */
    short vote_limit;                 /* <reserved> (**) */
    short target_up;                  /* <reserved> (**) */
    short target_down;                /* <reserved> (**) */
    byte  vote_priv;                  /* <reserved> (**) */
    byte  max_readpriv;               /* <reserved> (**) */

    word speed_graphics;             /* Speed for ANSI/Avatar graphics */

    byte  aftercaller_exit;           /* After caller exit errorlevel */
    byte  aftermail_exit;             /* After mail exit errorlevel */
    short max_connects;               /* <reserved> (**) */
    short max_noconnects;             /* <reserved> (**) */

    byte  logon_level;                /* New users level */
    long  logon_flags;                /* New users flags */

    char  areachange_key[4];          /* Hotkeys for the change area command */
    char  dateformat[20];             /* Format of the date fields */
    char  timeformat[20];             /* Format fo the time fields */
    char  external_editor[50];        /* External BBS message editor */

    struct class_rec uclass[12];      /* Access level definitions */

    char  local_editor[50];           /* Local message editor */

    char  QWKDir[40];                 /* QWK mail packer work directory */
    char  BBSid[10];                  /* QWK board name */
    word  qwk_maxmsgs;                /* Maximum number of messages for QWK packets */

    char  galileo[30];                /* Clocksynch phone number */

    char  norm_okfile[40];            /* Unknow request list */
    char  know_okfile[40];            /* Know request list */
    char  prot_okfile[40];            /* Protected request list */

    char  reg_name[36];               /* <reserved> (**) */
    long  betakey;                    /* <unused> */

    struct _packers                   /* Mail packers */
    {
        char id[10];                    /* Short name */
        char packcmd[30];               /* Pack commands */
        char unpackcmd[30];             /* unpack commands */
    } packers[10];

    struct _nl                        /* Nodelist database */
    {
        char list_name[14];             /* Nodelist base name */
        char diff_name[14];             /* Nodediff base name */
        char arc_name[14];              /* <reserved> (**) */
    } nl[10];

    bits  ansigraphics   :2;          /* 0=No Ansi, 1=Yes, 2=Ask */
    bits  avatargraphics :2;          /* 0=No Avatar, 1=Yes, 2=Ask */
    bits  hotkeys        :2;          /* 0=No hotkeyed menu, 1=Yes, 2=Ask */
    bits  screenclears   :2;          /* 0=No screen clears, 1=Yes, 2=Ask */

    bits  autozmodem     :1;          /* Terminal mode auto zmodem download */
    bits  avatar         :1;          /* Terminal mode Avatar emulation */
    bits  moreprompt     :2;          /* 0=No more prompt, 1=Yes, 2=Ask */
    bits  mailcheck      :2;          /* 0=No mail check, 1=Yes, 2=Ask */
    bits  fullscrnedit   :2;          /* 0=No full screen editor, 1=Yes, 2=Ask */

    bits  fillerbits     :2;          /* <reserved> (**) */
    bits  ask_protocol   :1;          /* 1=Ask default protocol */
    bits  ask_packer     :1;          /* 1=Ask default packer */
    bits  put_uploader   :1;          /* 1=Put uploader name in files.bbs */
    bits  keep_dl_count  :1;          /* 1=Keep count of download times */
    bits  use_areasbbs   :1;          /* 1=Use the areas.bbs file */
    bits  write_areasbbs :1;          /* 1=Keep the areas.bbs file up to date */

    short rookie_calls;               /* Number of calls for new users to be a rookie */

    char  pre_import[40];             /* Pre import batch */
    char  after_import[40];           /* After import batch */
    char  pre_export[40];             /* Pre export batch */
    char  after_export[40];           /* After export batch */

    byte  emulation;                  /* <reserved> (**) */
    char  dl_path[40];                /* Terminal emulator download path */
    char  ul_path[40];                /* terminal emulator upload path */

    bits  manual_answer  :1;          /* Manually answers phone */
    bits  limited_hours  :1;          /* Answer only at certain hours */
    bits  solar          :1;          /* <reserved> (**) */
    bits  areafix        :1;          /* 1=Use internal areafix */
    bits  doflagfile     :1;          /* 1=Log semaphores creation/deletion */
    bits  multitask      :1;          /* 1=Multitasking environment */
    bits  ask_alias      :1;          /* 1=Ask for an alias name */
    bits  random_birth   :1;          /* 1=Random birthdate check */

    short start_time;                 /* Answer start time in minutes */
    short end_time;                   /* Answer end time in minutes */

    char  boxpath[40];                /* User's filebox base directory */
    char  dial_suffix[20];            /* Dialing suffix command */

    char  galileo_dial[40];           /* Clocksynch dialing command */
    char  galileo_suffix[40];         /* Clocksynch dialing suffix command */
    char  galileo_init[40];           /* Clocksynch initialization command */

    char  areafix_help[40];           /* Areafix's help file name */
    char  alert_nodes[50];            /* Nodes that receives a copy of all areafix response messages */

    char  automaint[40];              /* Batch to execute once a day */

    byte  min_login_level;            /* Minimum login level */
    long  min_login_flags;            /* Minimul login flags */
    byte  min_login_age;              /* Minimul login age */

    char  resync_nodes[50];           /* Nodes to resynch the clock with */
    char  bbs_batch[40];              /* External BBS batch file */
    byte  altx_errorlevel;            /* ALT-X command errorlevel */

    char  fax_response[20];           /* FAX response */
    byte  fax_errorlevel;             /* Fax exit errorlevel */

    char  dl_counter_limits[4];       /* Download counter delimiters */

    char  areas_bbs[40];              /* AREAS.BBS path and file name */
    byte  afx_remote_maint;           /* Areafix level for %FROM command */
    byte  afx_change_tag;             /* Areafix level for #<old>:<new> command */

    bits  allow_rescan    :1;         /* 1=Allow rescan of new areas */
    bits  check_city      :1;         /* 1=Check user's city at logon */
    bits  check_echo_zone :1;         /* 1=Check zones in areafix's requests */
    bits  save_my_mail    :1;         /* 1=Save mail directed to the Sysop */
    bits  mail_method     :2;         /* 0=Separate netmail, 1=Net & echomail */
    /* in the same packet */
    bits  replace_tear    :2;         /* 0=No, 1=Link unlimited, 2=Link */
    /* limited to 35 char, 3=Always */

    char  my_mail[40];                /* Path to Sysop's mail */

    bits  stripdash       :1;         /* 1=Strip dashes from phone numbers */
    bits  use_iemsi       :1;         /* 1=Allows IEMSI logins */
    bits  newmail_flash   :1;         /* 1=Flashing MAIL active */
    bits  mymail_flash    :1;         /* 1=Flashing PERSONAL active */
    bits  keep_empty      :1;         /* 1=Do not import empty netmail */
    bits  show_missing    :1;         /* 1=Show missing files in file listings */
    bits  random_redial   :1;         /* 1=Redial numbers at random time */

    char  override_pwd[20];           /* Mail only override password */
    char  pre_pack[40];               /* Pre-PACK DOS command */
    char  after_pack[40];             /* After-PACK DOS command */

    byte  modem_timeout;              /* Modem answer timeout (seconds) */
    byte  login_timeout;              /* User's login timeout (minutes) */
    byte  inactivity_timeout;         /* User's inactivity timeout (seconds) */

    struct _altern_prefix
    {
        char flag[6];                   /* Nodelist flag (HST,V32B,etc.) */
        char prefix[20];                /* Dialing prefix */
    } prefixdial[10];

    char  iemsi_handle[36];           /* IEMSI: User's handle name */
    char  iemsi_pwd[20];              /* IEMSI: Password */
    short iemsi_infotime;             /* IEMSI: Seconds to display server */
    /*        informations */

    bits  iemsi_on        :1;         /* IEMSI: 1=Active, 0=Ignore */
    bits  iemsi_hotkeys   :1;         /* IEMSI: 1=Use hot-keyed menu */
    bits  iemsi_quiet     :1;         /* IEMSI: 1=Do not disturb */
    bits  iemsi_pausing   :1;         /* IEMSI: 1=Pause at end of screen */
    bits  iemsi_editor    :1;         /* IEMSI: 1=Use full screen editor */
    bits  iemsi_news      :1;         /* IEMSI: 1=Display news bulletins */
    bits  iemsi_newmail   :1;         /* IEMSI: 1=Check for new mail */
    bits  iemsi_newfiles  :1;         /* IEMSI: 1=Check for new files */

    bits  iemsi_screenclr :1;         /* IEMSI: 1=Send clearing screen codes */
    bits  prot_xmodem     :1;         /* 1=Internal XMODEM active */
    bits  prot_1kxmodem   :1;         /* 1=Internal 1K-XMODEM active */
    bits  prot_zmodem     :1;         /* 1=Internal ZMODEM active */
    bits  prot_sealink    :1;         /* 1=Internal SEALINK active */

    char  newkey_code[30];            /* Registration key code (>=2.33) */
    char  tearline[36];               /* Custom tear line */

    char  uucp_gatename[20];          /* UUCP gateway name */
    short uucp_zone;                  /* UUCP gateway zone */
    short uucp_net;                   /* UUCP gateway net */
    short uucp_node;                  /* UUCP gateway node */

    byte  carrier_mask;               /* Carrier detect bit mask */
    byte  dcd_timeout;                /* Timeout after drop DCD before hangup */

    struct
    {
        char display[20];               /* User's display packer name */
        int  offset;                    /* Offset of the signature id */
        char ident[20];                 /* Packer's signature id */
    } packid[10];

    char quote_string[5];             /* String to put in front of quoted lines */
    char quote_header[50];            /* Quote text file header */

    char tic_help[40];                /* Help file name for TIC processor */
    char tic_alert_nodes[50];         /* Nodes that receives a copy of all TIC response messages */
    char tic_newareas_create[50];     /* Nodes that can create new TIC areas */
    char tic_newareas_link[50];       /* Nodes that are linked to new TIC areas */
    byte tic_remote_maint;            /* TIC level for %FROM command */
    byte tic_change_tag;              /* TIC level for #<old>:<new> command */

    int  uucp_point;                  /* UUCP gateway point */

    byte  dial_timeout;               /*  */
    byte  dial_pause;                 /*  */

    bits  newfilescheck   :2;         /*  */
    bits  mono_attr       :1;         /*  */
    bits  force_intl      :1;         /* 1=Force ^aINTL line in matrix */
    bits  inp_dateformat  :2;         /* Date format for input fields */
    bits  single_pass     :1;         /* <reserved> (**) */

    int   ul_free_space;              /* Min. disk space for uploads */
    char  hangup_string[40];          /* Modem hangup command */
    char  init2[40];                  /* 2nd modem init command */
    char  init3[40];                  /* 3th modem init command */

    int   page_start[7];              /* Sysop paging start hours */
    int   page_end[7];                /* Sysop paging end hours */

    char  logbuffer;                  /* Size of log file buffer */

    char  newareas_path[40];          /* Where to create new areas */
    char  newareas_base;              /* Message base type for new areas */

    char  answer[40];                 /* Modem answer command */

    bits  blanker_type   :3;          /* Screen blanker type */
    bits  tcpip          :2;          /* <reserved> (**) */

    char  internet_gate;              /* Type of internet gate */
    /* 0=Uupc */
    /* 1=GIGO */

    char  areafix_watch[50];          /* Alternate Areafix names */
    char  tic_watch[50];              /* Alternate Raid names */
};


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
    #pragma pack()
#endif


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
