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

#ifndef __gs_max3_h
#define __gs_max3_h


//  ------------------------------------------------------------------

#include <gtimall.h>


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif


//  ------------------------------------------------------------------

typedef FTime SCOMBO;         // DOS-style bitmapped datestamp

typedef struct _netaddr {
  word zone;
  word net;
  word node;
  word point;
} NETADDR;

typedef word zstr;            // Offset of string within area heap


//  ------------------------------------------------------------------

#define MSGTYPE_SDM     0x01
#define MSGTYPE_SQUISH  0x02

#ifdef PATHLEN
#undef PATHLEN
#endif

#define PATHLEN           120 // Max. length of a path


//  ------------------------------------------------------------------
//  Menu server enumerations and structures

//  Enumeration `option' -- All possible values for menu.option[x].type

typedef enum {
  nothing,

  MISC_BLOCK=100, display_menu, display_file, message, file, other,
                  o_press_enter, key_poke, clear_stacked, o_if,
                  o_menupath, o_cls, mex, link_menu, o_return,


  XTERN_BLOCK=200, xtern_erlvl, xtern_dos, xtern_run, xtern_chain,
                   xtern_concur,

  MAIN_BLOCK=300, goodbye, statistics, o_yell, userlist, o_version,
                  user_editor, leave_comment, climax,

  MSG_BLOCK=400, same_direction, read_next, read_previous,
                 enter_message, msg_reply, read_nonstop,
                 read_original, read_reply, msg_list, msg_scan,
                 msg_inquir, msg_kill, msg_hurl, forward, msg_upload,
                 xport, read_individual, msg_checkmail, msg_change,
                 msg_tag, msg_browse, msg_current, msg_edit_user,
                 msg_upload_qwk, msg_toggle_kludges, msg_unreceive,
                 msg_restrict, msg_area, msg_track, msg_dload_attach,
                 msg_reply_area,

  FILE_BLOCK=500, locate, file_titles, file_type, upload, download, raw,
                  file_kill, contents, file_hurl, override_path,
                  newfiles, file_tag, file_area,

  // Options generally found on the Change Setup menu

  CHANGE_BLOCK=600, chg_city, chg_password, chg_help, chg_nulls,
                    chg_width, chg_length, chg_tabs, chg_more,
                    chg_video, chg_editor, chg_clear, chg_ibm,
                    chg_phone, chg_realname, chg_hotkeys,
                    chg_language, chg_userlist, chg_protocol,
                    chg_fsr, chg_archiver, chg_rip,

  EDIT_BLOCK=700, edit_save, edit_abort, edit_list, edit_edit,
                  edit_insert, edit_delete, edit_continue, edit_to,
                  edit_from, edit_subj, edit_handling, read_diskfile,
                  edit_quote,

  // Stuff that was hacked on after the original implementation

  CHAT_BLOCK=800, who_is_on, o_page, o_chat_cb, chat_toggle, o_chat_pvt,

  END_BLOCK,


  // Everything below here is RESERVED by Maximus for future uses!
  // Also, everything ABOVE is fairly stable.  If changes have to be made,
  // the old options above will not be re-used.  For example, if the
  // `edit_insert' command should become obsoleted for some reason, that
  // slot would either get retired and do nothing, or perform the NEW
  // edit_insert function.

  rsvd=32766  // This was stuck in to make sure that the `option'
              // enumeration uses a word, instead of a byte, in case we
              // really expand this structure sometime soon.

} option __attribute__((packed));


//  ------------------------------------------------------------------
//  .prm file structure and definitions

typedef word OFS;

#ifndef MAX_DRIVES
#define MAX_DRIVES           26   // Maximum number of drives on system
#endif

#ifndef CHAR_BITS
#define CHAR_BITS             8   // Number of bits in a 'char'
#endif

#define CTL_VER               9   // Version number of bbs.prm

// This macro is ONLY used for accessing *pointers* in the `prm' structure.
// This is required, due to the way Wynn has made OPUS_CTL write the strings
// out (which uses a lot less memory than some other ways).  If you want
// to access an INT, or a non-pointer in the structure, then you can use
// a `prm.var_name'-style reference.

#define PRM(s) (offsets+(prm.s))

#define MAX_LANG             8       // Max. number of possible languages
#define MAX_YELL            10       // Max number of yell slots
#define MAX_EXTERNP         16       // max. number of external programs
#define ALIAS_CNT           15       // number of matrix addresses
#define MAXCLASS            12       // OBSOLETE!  Only used for
                                     // compatibility with Max 2.x.

// OBSOLETE!  Only used for compatibility purposes

struct   cl_rec {
  sword    priv;
  word     max_time;      // max cume time per day
  word     max_call;      // max time for one call
  word     max_dl;        // max dl kbytes per day
  word     ratio;         // ul:dl ratio
  word     min_baud;      // speed needed for logon
  word     min_file_baud; // speed needed for file xfer
};


// Note: To read in the *.PRM structure, first read in the m_pointers
// structure, which is always contained at the beginning of the file.
// Then, seek to the offset prm.heap_offset, and read in everything
// from there to EOF into heap[].  All of the 'OFS' type variables
// are simply offsets into the variable-length heap which started at
// heap_offset. To obtain a string from the .PRM heap, simply
// add the offset in the m_pointers structure to the address of the
// heap[] variable that the heap was read into.  For example, to access
// the string for 'system_name', you'd use '(heap+prm.system_name)'.
// Alternatively, you can declare a macro to do this, such as the
// PRM() macro shown above.  (Maximus itself uses the variable
// 'strings' instead of 'heap' to hold the varible-length strings,
// but the concept is the same.)  When using the PRM() macro to
// access the string for 'system_name', you'd simply write:
// 'PRM(system_name)', which is a lot clearer.  Also, please note that
// NON-OFS variables should be accessed normally!  That means that
// 'task_num', 'auto_kill', can be access with 'prm.task_num',
// 'prm.auto_kill', etc.  The special heap manipulation is only needed
// for strings.

struct m_pointers {

// DATA

  byte  id;             /* Always equal to 'M'               STABLE  */
  byte  version;        /* for safety                        STABLE  */
  word  heap_offset;    /* OFFSET OF BEGINNING OF HEAP!      STABLE  */
  byte  task_num;       /* for multi-tasking systems         STABLE  */
  sword com_port;       /* Com1=0, Com2=1, etc               STABLE  */
  byte  noise_ok;       /* If yell noise is currently on     STABLE  */

  /* Miscellanious system information */

  byte  video;          /* Mode for local video display              */
  byte  log_mode;       /* What style of logging to use              */
  word  max_baud;       /* fastest speed we can use                  */
  sbyte multitasker;    /* flag for DoubleDos (see below)            */
  byte  nlver;          /* Which nodelist version we use (NLVER_XXX) */

  sword min_ulist;      /* OBSOLETE!  Only used for compatibility    */
  sword max_ulist;      /* with Maximus 2.x!                         */

  /* Information about errorlevels */

  byte  exit_val;       /* Erl to use after caller if none of below  */
  byte  edit_exit;      /* erl to use after matrix mail written      */
  byte  echo_exit;      /* ERRORLEVEL for after inbound echomail     */
  byte  local_exit;     /* Errorlevel after entering local msgs      */

  /* Modem information */

  sword carrier_mask;
  sword handshake_mask;

  /* Log-on information */

  word logon_priv;      /* Access level for new users                */
  word  logon_time;     /* time to give for logons                   */
  word  min_baud;       /* minimum baud to get on-line               */
  word  speed_graphics; /* min baud for graphics                     */

  /* Information about message areas */

  byte  auto_kill;      /* RECD PVT msgs. 0=no 1=ask 2=yes           */

  word ctla_priv;       /* Priv to see CONTROL-A lines in messages   */
  word seenby_priv;     /* Min priv to see SEEN-BY line              */
  word pvt_priv;        /* Min priv to read pvt msgs                 */

  word msg_ask[16];     /* Array of privs. for message attr ask's    */
  word msg_assume[16];  /* Array of privs. for message attr assume's */
  word  msg_fromfile;   /* Priv. for doing message from file         */
  word  speed_rip;      /* min baud for rip graphics                 */
  byte  rsvd1[2];       /* used to be high_msgarea, begin_msgarea    */
  word unlisted_priv;   /* Priv needed to send to unlisted node      */
  sword unlisted_cost;  /* Charge to send to unlisted node           */

  word mc_reply_priv;   /* Priv to reply to msg with mailchecker     */
  word mc_kill_priv;    /* Priv to kill msg with mailchecker         */


  /* Information about file areas */

  sword date_style;     /* Used for FILES.BBS display                */
  word rsvd20;          /* Reserved (used to be dlall_priv)          */
  word rsvd21;          /* Reserved (used to be ulbbs_priv)          */
  dword k_free;         /* The number of disk space (in K) which     *
                         * must be available before we let a user    *
                         * upload.                                   */
  word  rsvd7;          /* reserved - used to be upload reward       */
  word  ratio_threshold;/* OBSOLETE!  Only used for Max 2.x compat.  */
  byte  rsvd2[4];       /* used to be high_filearea, begin_filearea  */

  /* Our matrix address(es) */

  NETADDR address[ALIAS_CNT];

  byte rsvd3[60]; /* Reserved by Maximus for future use */

  /* About the users */
  struct cl_rec cls[MAXCLASS];  /* OBSOLETE!  Only used for compatibility */
                                /* with Maximus 2.x.                      */

  /* Flags for external protocols */

  sword protoexit;              /* Errorlevel for protocol exit      */
  char  protoflag[MAX_EXTERNP];

  /* General-purpose bit-flags  (See FLAGx_xxx definitions above.) */

  word  flags;
  word  flags2;
  word  flags3;
  word  flags4;

  /* Bit field containing drive letters to save when going outside */
  char  drives_to_save[(MAX_DRIVES/CHAR_BITS)+1];

  byte  fbbs_margin;      /* Margin to use for wrapping FILES.BBS comments */
  byte  kill_attach;      /* Kill file attaches 0=no 1=ask 2=yes           */

  word  max_ptrs;         /* Maximum size of pointers of ALL *.LTFs */
  word  max_heap;         /* Maximus heap size of all *.LTFs */
  byte  max_lang;         /* Current number of languages */
  byte  rsvd_lang;

  word  max_glh_ptrs;
  word  max_glh_len;

  word  max_syh_ptrs;
  word  max_syh_len;

  byte  input_timeout;   /* # of mins until Max hangs up due to no input    */

  byte  charset;         /* Character set support - see CHARSET_XXXX, above */
  word  max_pack;        /* Maximum # of msgs to pack into a .QWK packet    */
  word  msg_localattach; /* Priv. for doing local file attaches             */
  word  kill_attach_priv;/* Priv. for killing local file attaches           */
  word  mcp_sessions;    /* Max number of MCP sessions                      */
  dword max_msgsize;     /* Truncate uploaded messages to this size         */

  byte  rsvd65[2];       /* Reserved by Maximus for future use              */

// OFFSETS

  /* About your system */

#define PRM_HEAP_START sysop

  OFS   sysop;          /* sysop's name. MUST be first offset in prm file */
  OFS   system_name;    /* board's name                              */

  /* Modem commands */

  OFS   m_busy;         /* mdm cmd to take modem off hook            */

  /* Paths to various places */

  OFS   sys_path;       /* path to SYSTEM?.BBS files                 */
  OFS   misc_path;      /* path to `F-key files'                     */
  OFS   net_info;       /* path to NODELIST files                    */
  OFS   temppath;       /* place to put temporary files              */
  OFS   ipc_path;       /* path for inter-process communications     */

  /* General files needed by the system */

  OFS   user_file;      /* path/filename of User.Bbs                 */
  OFS   log_name;       /* name of the log file                      */
  OFS   chat_prog;      /* External chat program, if any             */
  OFS   chat_fbegin;    /* File to display instead of "CHAT: begin"  */
  OFS   chat_fend;      /* File to display instead of "END CHAT"     */
  OFS   local_editor;   /* Command for local editor, if any          */
  OFS   notfound;       /* User name not found in user file          */
  OFS   junk;           /* Don't use this for anything!              */

  /* General *.?BS files needed everywhere */

  OFS   logo;           /* first file shown to a caller              */
  OFS   bad_logon;      /* if user's last logon attempt was bad      */
  OFS   welcome;        /* shown after logon                         */
  OFS   quote;          /* For displaying "random" quotes from       */
  OFS   newuser1;       /* Asks user to type in password             */
  OFS   newuser2;       /* Replaces `welcome' for a new user         */
  OFS   rookie;         /* Replaces `welcome' for rookies            */
  OFS   application;    /* new user questionnaire                    */
  OFS   byebye;         /* file displayed at logoff                  */
  OFS   out_leaving;    /* Bon Voyage                                */
  OFS   out_return;     /* Welcome back from O)utside                */
  OFS   daylimit;       /* Sorry, you've been on too long...         */
  OFS   timewarn;       /* warning about forced hangup               */
  OFS   tooslow;        /* explains minimum logon baud rate          */
  OFS   barricade;      /* Displayed before prompt for access code   */
  OFS   shelltodos;     /* Displayed when Sysop hits Alt-J           */
  OFS   backfromdos;    /* Displayed when Sysop returns from Alt-J   */
  OFS   areanotexist;   /* File to display instead of "That area     *
                         * doesn't exist!"                           */

  /* User priv levels database */

  OFS   access;         /* Database of user privilege levels         */

  /* File-area items */

  OFS   xferbaud;       /* explains minimum file transfer baud rate  */
  OFS   file_area_list; /* dump file... used instead of Dir.Bbs      */
  OFS   no_space;       /* File to display if trying to UL with      *
                         * less than k_free space left on drive.     */
  OFS   fname_format;   /* Essay on MS-DOS filenames for U)ploads    */
  OFS   ul_log;         /* Log file for uploads                      */

  OFS   file_header;    /* Format for file area's A)rea command      */
  OFS   file_format;    /* Format for A)rea command entries          */
  OFS   file_footer;    /* Format for footer for file.area menu      */

  OFS   proto_dump;      /* Dump file for protocol screen            */

  /* Message-area items */

  OFS   msgarea_list;   /* dump file... used instead of Dir.Bbs      */
  OFS   echotoss_name;  /* Name of your echomail tosslog             */
  OFS   nomail;         /* Display by mailchecker if no mail wtng.   */

  OFS   msg_header;     /* Format for msg.area's A)rea command       */
  OFS   msg_format;     /* Format for A)reas command entries         */
  OFS   msg_footer;     /* Format for footer for msg.area menu       */

  /* Help files:  Used to explain various things */

  OFS   hlp_editor;     /* intro to msg editor for novices.          */
  OFS   hlp_replace;    /* Explain the Msg.Editor E)dit command      */
  OFS   msg_inquire;    /* Explain the Msg. I)nquire command         */
  OFS   hlp_locate;     /* Explain the Files L)ocate command         */
  OFS   hlp_contents;   /* Explain the Files C)ontents command       */
  OFS   oped_help;      /* help file for the full-screen editor      */
  OFS   hlp_scan;       /* help file for S)can                       */
  OFS   hlp_list;       /* help file for L)ist                       */

  /* External protocols */

  OFS   protocols[MAX_EXTERNP]; /* external file protocol programs   */
  OFS   protoname[MAX_EXTERNP]; /* name of protocol, on menu         */

  /* Date/Time format strings */

  OFS   timeformat;
  OFS   dateformat;

  /* **OBSOLETE** names of the old Max 2.x area.dat/idx files */

  OFS   adat_name;
  OFS   aidx_name;

  /* Menu paths/names */

  OFS   menupath;        /* The default place to look for the menus */
  OFS   first_menu;      /* The name of the first menu to display */
  OFS   edit_menu;       /* Name of the EDIT menu */

  /* Miscellaneous */

  OFS   achg_keys;       /* Characters used to change area -/+ */
  OFS   tune_file;       /* Path to TUNES.MAX */
  OFS   lang_path;       /* Path to *.LTF files */

  OFS   lang_file[MAX_LANG]; /* Array of all *.LTF names */

  OFS   m_init;          /* Modem initialization string */
  OFS   m_ring;          /* Command modem sends when phone ringing */
  OFS   m_answer;        /* Cmd to send to modem when ring detect */
  OFS   m_connect;       /* Connect string, as returned by modem */

  OFS   high_msgarea;
  OFS   begin_msgarea;  /* Msg area to put new users in              */

  OFS   high_filearea;
  OFS   begin_filearea; /* File area to put new users in             */

  OFS   fidouser;       /* Name of FIDOUSER.LST file to use          */
  OFS   cmtarea;        /* Message area to put comments in           */

  OFS   arc_ctl;        /* Control file for archiving programs       */
  OFS   olr_name;       /* OLR: Filename to use for DL packets       */
  OFS   olr_dir;        /* OLR: Directory for off-line stuff         */
  OFS   phone_num;
  OFS   viruschk;       /* Name of batch file to call for virus check*/
  OFS   protocol_max;   /* Name of compiled protocol data file       */
  OFS   track_privview; /* Priv req'd to see tracking info owned by others */
  OFS   track_privmod;  /* Priv req'd to modify track info owned by others */
  OFS   track_base;     /* Base name of the tracking database files  */
  OFS   track_exclude;  /* File containing list of names to exclude  */
  OFS   stagepath;      /* Path for staged CD-ROM transfers          */
  OFS   attach_base;    /* Base name of the local attach database    */
  OFS   attach_path;    /* Default path for local file attaches      */
  OFS   attach_archiver;/* File attach archiver                      */
  OFS   inbound_files;  /* Inbound FTS-1 attaches for users          */
  OFS   rippath;        /* Default icon & RIP screens path           */

  /* More help files */

  OFS   hlp_hdrentry;   /* Help screen for message header entry      */
  OFS   hlp_msgentry;   /* Help screen for start of message entry    */
  OFS   not_configured; /* Displayed if user is not configured       */

  /* MCP stuff */

  OFS   mcp_pipe;       /* Default MCP pipe                          */

  /* Paths to the new marea/farea message/file data files */

  OFS   marea_name;     /* Name of message area data file            */
  OFS   farea_name;     /* Name of file area data file               */

  /* Caller information log */

  OFS   caller_log;     /* Caller information log                    */

#define PRM_HEAP_END caller_log
};


//  ------------------------------------------------------------------
//  General structure of MAREA.DAT and FAREA.DAT:
//  MAREA_ID or FAREA_ID
//  _msgarea
//     _ovride ...
//     zstr heap
//  _msgarea
//     _ovride ...
//     zstr heap
//  eof


// This key is present at the beginning of an area file.  MAREA_ID is
// used for the message data file and FAREA_ID is used for the
// file data file.

#define MAREA_ID      0x1a49023fL
#define FAREA_ID      0x1a01953aL

struct _ovride {

  /* Only one of 'opt' or 'name' should be used.  If opt==0, use name.  If  *
   * name==0, use opt.                                                      */

  option opt;           /* Type of menu option to override     ...OR...     */
  byte   name;          /* First letter of command to override              */
  byte   rsvd1;         /* Reserved for future use                          */
  zstr   acs;           /* New ACS required to access option                */
  zstr   menuname;      /* Use this access level on the given menu only     */
};


//  ------------------------------------------------------------------
//  Bit masks for the ma.attribs field

#define MA_PVT      0x0001  /* Private msgs allowed */
#define MA_PUB      0x0002  /* Public msgs allowed */
#define MA_HIBIT    0x0004  /* High bit msgs allowed */

#define MA_NET      0x0008  /* Netmail area */
#define MA_ECHO     0x0010  /* Echomail area */
#define MA_CONF     0x0020  /* Conference area */

#define MA_ANON     0x0040  /* Anonymous messages are OK */
#define MA_NORNK    0x0080  /* Don't use the REALNAME kludge for this area */
#define MA_REAL     0x0100  /* Force  use of       real name for this area */
#define MA_ALIAS    0x0200  /* Force  use of alias      name for this area */
#define MA_AUDIT    0x0400  /* Use auditing (msg tracking) controls in area*/
#define MA_READONLY 0x0800  /* Area is read-only */
#define MA_HIDDN    0x1000  /* Area does not display on normal area list   */
#define MA_ATTACH   0x2000  /* Area allows local file attaches             */
#define MA_DIVBEGIN 0x4000  /* A message area division, not a real area    */
#define MA_DIVEND   0x8000  /* End of the message area division            */

#define MA2_NOMCHK  0x0001  /* Don't do personal mail check in this area   */

#define MA_SHARED   (MA_ECHO | MA_CONF)


typedef struct _msgarea {

  word cbArea;          /* Length of THIS INDIVIDUAL RECORD                0*/
  word num_override;    /* Number of overrides following this record       2*/
  word cbHeap;          /* Length of the zstr heap following the overrides 4*/
  word division;        /* Reserved for future use                         6*/
  zstr name;            /* String format of area's name.                   8*/
  zstr acs;             /* Access control string for this area            10*/
  zstr path;            /* Path to messages (but for MA_DIVBEGIN only,    12*
                         * used instead as name of custom .bbs file).       */
  zstr echo_tag;        /* The 'tag' of the area, for use in ECHOTOSS.LOG 14*/
  zstr descript;        /* The DIR.BBS-like description for msg section   16*/
  zstr origin;          /* The ORIGIN line for this area                  18*/
  zstr menuname;        /* Custom menu name                               20*/
  zstr menureplace;     /* Replace this menu name with menuname from above22*/

  word attribs;         /* Attributes for this area                       24*/

  NETADDR primary;      /* Use as primary address for this area           26*/
  NETADDR seenby;       /* Use as address in seen-bys                     34*/
  word attribs_2;       /* More attributes                                42*/
  word type;            /* Message base type.  MSGTYPE_SDM = *.MSG.       44*
                         * MSGTYPE_SQUISH = SquishMail.  (Constants are     *
                         * in MSGAPI.H)                                     */
  word killbyage;       /* Make sure msgs are less than X days old        46*/
  word killbynum;       /* Make sure there are less than X msgs           48*/
  word killskip;        /* Exempt the first X msgs from this processing   50*/
  zstr barricade;       /* Barricade file                                 52*/
  zstr barricademenu;   /* Apply barricade priv while using this menu     54*/
  sdword cbPrior;       /* Seek offset from start of this area to get back56*
                         * to prior area.                                   */
  zstr attachpath;      /* Reserved for future use                        58*/
  dword rsvd4;                                                          /*60*/
};                                                                      /*64*/


//  -----------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack()
#endif


//  -----------------------------------------------------------------

#endif // __gs_max3_h

//  -----------------------------------------------------------------
