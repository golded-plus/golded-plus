
//  ------------------------------------------------------------------
//  GoldED+
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999-2000 Alexander S. Aganichev
//  ------------------------------------------------------------------
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License as
//  published by the Free Software Foundation; either version 2 of the
//  License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston,
//  MA 02111-1307 USA
//  ------------------------------------------------------------------
//  $Id$
//  ------------------------------------------------------------------
//  Definition of system-wide sizes and structures.
//  ------------------------------------------------------------------


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif


//  ------------------------------------------------------------------
//  General data/list size limits

const uint MAX_COMM =  25;        // Editor comments
const uint MAX_FIND =  10;        // Max number of search strings
const uint MAXQUOTELEN = 40;      // Max length of quote


//  ------------------------------------------------------------------
//  Header attach size types

const int ATTACH_BYTES  = 1;
const int ATTACH_KBYTES = 2;


//  ------------------------------------------------------------------
//  Control info types

const int CI_NONE = 0x0000;
const int CI_TEAR = 0x0001;
const int CI_ORIG = 0x0002;
const int CI_TAGL = 0x0004;


//  ------------------------------------------------------------------
//  Misc mode types (ACK!! these need cleaning up!)

const int MODE_AREACONF      = 100;
const int MODE_CONFIRM       = 101;
const int MODE_COPY          = 102;
const int MODE_FORWARD       = 103;
const int MODE_MOVE          = 108;
const int MODE_QUOTE         = 109;
const int MODE_QUOTEBUF      = 110;
const int MODE_REPLY         = 111;
const int MODE_REPLYCOMMENT  = 112;
const int MODE_SAVE          = 114;
const int MODE_SAVENOCTRL    = 115;
const int MODE_VIEW          = 116;
const int MODE_WRITE         = 117;
const int MODE_PRINT         = 118;
const int MODE_APPEND        = 119;
const int MODE_WRITEHEADER   = 120;
const int MODE_HEADER        = 121;

const int MODE_NEW           = 0x0100;
const int MODE_CHANGE        = 0x0200;
const int MODE_UPDATE        = 0x0400;
const int MODE_QUIT          = 0x0800;

const int MODE_PREVIOUS      = 2;
const int MODE_MARKED        = 1;
const int MODE_CURRENT       = 0;
const int MODE_DONT          = -1;


//  ------------------------------------------------------------------
//  Scanning constants

const int SCAN_STARTUP       =  0;
const int SCAN_ALL           =  1;
const int SCAN_MARKED        =  2;
const int SCAN_CURRENT       =  3;
const int SCAN_MATCHING      =  4;
const int SCAN_UNSCANNED     =  5;
const int SCAN_EXPORTQWK     =  6;
const int SCAN_IMPORTQWK     =  7;
const int SCAN_EXPORTSOUP    =  8;
const int SCAN_IMPORTSOUP    =  9;
const int SCAN_LIST          = 10;
const int SCAN_GROUP         = 11;
const int SCAN_NETMAIL       = 12;
const int SCAN_QUIT          = 99;


//  ------------------------------------------------------------------
//  Exit errorcodes

const int EXIT_OK          = 0x00;
const int EXIT_LOCAL       = 0x01;
const int EXIT_NET         = 0x02;
const int EXIT_ECHO        = 0x04;
const int EXIT_NONAME      = 0x20;
const int EXIT_CFGERR      = 0x21;
const int EXIT_ERRORS      = 0x22;


//  ------------------------------------------------------------------
//  Direction defines

const int DIR_PREV  = -1;
const int DIR_NEXT  =  1;

const int DIR_ABOVE = DIR_PREV;
const int DIR_BELOW = DIR_NEXT;


//  ------------------------------------------------------------------
//  Misc screen limits

#define MINROW        6
#define MAXCOL        gvid->numcols
#define MAXROW        gvid->numrows
#define MAXMSGLINES   (MAXROW-MINROW-1)


//  ------------------------------------------------------------------
//  Message marking

const char MMRK_MARK = '\x10';
const char MMRK_BOOK = '\x11';


//  ------------------------------------------------------------------
//  Twit options

const int NOT_TWIT    = 0;
const int TWIT_SHOW   = 1;
const int TWIT_BLANK  = 2;
const int TWIT_SKIP   = 3;
const int TWIT_IGNORE = 4;
const int TWIT_KILL   = 5;


//  ------------------------------------------------------------------
//  CC list options

const int CC_REMOVE  = 0;
const int CC_HIDDEN  = 1;
const int CC_VISIBLE = 2;
const int CC_NAMES   = 3;
const int CC_KEEP    = 4;


//  ------------------------------------------------------------------
//  Arealist types

const int AL_TOTLST = 0;
const int AL_TOTNEW = 1;


//  ------------------------------------------------------------------
//  Area R/O modes

const int READONLY_SOFT = 1;
const int READONLY_HARD = 2;


//  ------------------------------------------------------------------
//  Colorset types

const int CS_NORMAL   = 0;
const int CS_INTENSE  = 1;
const int CS_MONO     = 2;


//  ------------------------------------------------------------------
//  Lookup types

const int LOOK_DEST = 0;
const int LOOK_ORIG = 1;
const int LOOK_NAME = 2;
const int LOOK_CITY1 = 3;
const int LOOK_CITY2 = 4;


//  ------------------------------------------------------------------
//  Squish scanning types

//const int SQS_API   = 1;
//const int SQS_QUICK = 2;


//  ------------------------------------------------------------------
//  Quotebuffer modes

const int QBUF_ASK       = 1;
const int QBUF_APPEND    = 2;
const int QBUF_OVERWRITE = 3;


//  ------------------------------------------------------------------
//  Screenblanker types

const int BLANK_ALLBLACK = 0;
const int BLANK_SLIDEWIN = 1;


//  ------------------------------------------------------------------
//  DISPMSGSIZE types

const int DISPMSGSIZE_BYTES  = 1;
const int DISPMSGSIZE_KBYTES = 2;
const int DISPMSGSIZE_LINES = 3;


//  ------------------------------------------------------------------
//  Replylink types

const int REPLYLINK_CHAIN  = 0;
const int REPLYLINK_DIRECT = 1;

const int REPLYLINKLIST_FAST = 0;
const int REPLYLINKLIST_FULL = 1;


//  ------------------------------------------------------------------

const int KEYB_BLOCK = 0;
const int KEYB_POLL  = 1;


//  ------------------------------------------------------------------
//  Registration key valid OS

const int GKEY_VALID_DOS = 0x0001;
const int GKEY_VALID_OS2 = 0x0002;
const int GKEY_VALID_W32 = 0x0004;
const int GKEY_VALID_LNX = 0x0008;
const int GKEY_VALID_WIN = 0x0010;
const int GKEY_VALID_OPM = 0x0020;
const int GKEY_VALID_XWN = 0x0040;


//  ------------------------------------------------------------------
//  File request options

const int FREQ_SORT        = 0x0001;
const int FREQ_FROMTOP     = 0x0002;
const int FREQ_NOFILES     = 0x0004;
const int FREQ_FAST        = 0x0008;
const int FREQ_NOWAZOOMSG  = 0x0010;
const int FREQ_USEFLOWFILE = 0x0020;


//  ------------------------------------------------------------------
//  Peek URLs options

const int PEEK_SORT        = 0x0001;
const int PEEK_FROMTOP     = 0x0002;


//  ------------------------------------------------------------------
//  Automatic echoid names

const int AUTOID_VERBOSE = 0;
const int AUTOID_SHORT   = 1;
const int AUTOID_LONG    = 2;


//  ------------------------------------------------------------------
//  Values for EDITHEADERFIRST

const int EDITHEADERFIRST_NO       = 0x0000;
const int EDITHEADERFIRST_NEW      = 0x0001;
const int EDITHEADERFIRST_CHANGES  = 0x0002;
const int EDITHEADERFIRST_REPLIES  = 0x0004;
const int EDITHEADERFIRST_FORWARDS = 0x0008;
const int EDITHEADERFIRST_YES      = 0x000F;


//  ------------------------------------------------------------------
//  Values for MSGLISTDATE

const int MSGLISTDATE_NONE     = 0;
const int MSGLISTDATE_WRITTEN  = 1;
const int MSGLISTDATE_ARRIVED  = 2;
const int MSGLISTDATE_RECEIVED = 3;


//  ------------------------------------------------------------------
//  Write constants

const int WRITE_CURRENT       = 0;
const int WRITE_MARKED        = 1;
const int WRITE_QUIT          = -1;
const int WRITE_FILE          = 1;
const int WRITE_PRINTER       = 2;
const int WRITE_CLIPBRD       = 4;
const int WRITE_NO_HEADER     = 8;
const int WRITE_ONLY_HEADER   = 16;


//  ------------------------------------------------------------------
//  Values for REPLYRE

const int REPLYRE_NO = 0;
const int REPLYRE_YES = 1;
const int REPLYRE_NUMERIC = 2;


//  ------------------------------------------------------------------
//  Name typedefs

typedef char Name[36];


//  ------------------------------------------------------------------
//  Combined name/address structure

struct Node {
  Name name;
  Addr addr;
  Node& operator=(const Node& a) { strcpy(name, a.name); addr=a.addr; return *this; }
};


//  ------------------------------------------------------------------
//  Tearline typedefs

typedef char Tear[77];


//  ------------------------------------------------------------------
//  Subject typedefs

typedef char Subj[128];


//  ------------------------------------------------------------------
//  Internet address typedefs

typedef char IAdr[128];


//  ------------------------------------------------------------------
//  Internet subject typedefs

typedef char XlatName[17];


//  ------------------------------------------------------------------
//  Template typedefs

struct Tpl {
  Path file;
  Desc name;
  Addr match;
};


//  ------------------------------------------------------------------
//  Echo rename struct

class EchoRen {
public:
  std::string from;
  std::string to;
  EchoRen() {}
  EchoRen(const char *_from, const char *_to) { from = _from; to = _to; }
  ~EchoRen() {}
  EchoRen& operator=(const EchoRen& a) { from = a.from; to = a.to; return *this; }
};


//  ------------------------------------------------------------------
//  Filename aliases

struct FileAlias {
  char alias[9];
  Path file;
  FileAlias& operator=(const FileAlias& a) {
    strxcpy(alias, a.alias, sizeof(alias));
    strxcpy(file, a.file, sizeof(Path));
    return *this;
  }
};


//  ------------------------------------------------------------------
//  Address macro record

struct AddrMacro {
  char macro[20];
  IAdr name;
  Addr addr;
  Subj subj;
  Attr attr;
};


//  ------------------------------------------------------------------
//  Sound effect defines

const int SND_THEEND  = 0;
const int SND_GOTIT   = 1;
const int SND_TOOBAD  = 2;
const int SND_TOYOU   = 3;
const int SND_S_O_S   = 4;
const int SND_SAYBIBI = 5;


//  ------------------------------------------------------------------
//  Event command "Play". Types and structure

const byte PLAY_NO  =  0;   // * No sound
const byte PLAY_DOS =  1;   // + DOS beeping and effects
const byte PLAY_VOC =  2;   // * Creative Technology Voice samples
const byte PLAY_CMF =  3;   // + Creative Technology FM songs
const byte PLAY_WAV =  4;   // + MPIDS (Multimedia) WAVE samples
const byte PLAY_SAM =  5;   // + Raw sample without any header
const byte PLAY_MID =  6;   // - MIDI songs
const byte PLAY_ROL =  7;   // - Adlib songs
const byte PLAY_SNG =  8;   // - Sequencer plus Jr. songs
const byte PLAY_MOD =  9;   // - ProTracker MOD songs
const byte PLAY_STM = 10;   // - ScreamTracker STM/STX songs
                            //
                            // * = Supported in the current version
                            // + = Planned for support "soon"
                            // - = Will probably not be supported any time soon

struct GPlay {
  byte  type;
  int   effectno;   // For the DOS sound effects SND_xxx
  Path  file;
  int   Play() const;
};


//  ------------------------------------------------------------------
//  Event types (CRC16 values), commands and structure

const word EVTT_AREALIST       = 0x445D;
const word EVTT_ASKYESNO       = 0xB26C;
const word EVTT_ATTENTION      = 0x0F21;
const word EVTT_BLANKEROFF     = 0x16EF;
const word EVTT_BLANKERON      = 0x819F;
const word EVTT_DOSSHELL       = 0x2111;
const word EVTT_EDITCOMMENT    = 0x662C;
const word EVTT_EDITCOMPLETION = 0xDA45;
const word EVTT_ENDOFMSGS      = 0x95BC;
const word EVTT_ERRORFATAL     = 0xE9C1;
const word EVTT_EXIT           = 0x6090;
const word EVTT_JOBDONE        = 0xA81F;
const word EVTT_JOBFAILED      = 0xCC9A;
const word EVTT_MSGCOMMENT     = 0x0761;
const word EVTT_MSGCOPYING     = 0x087B;
const word EVTT_MSGDELETING    = 0x1AFF;
const word EVTT_MSGFROMYOU     = 0xBACB;
const word EVTT_MSGISLOCAL     = 0x2BD6;
const word EVTT_MSGISTWIT      = 0xC466;
const word EVTT_MSGLIST        = 0x84D0;
const word EVTT_MSGMOVING      = 0xAA1B;
const word EVTT_MSGNEW         = 0x2882;
const word EVTT_MSGPRINTING    = 0x29E0;
const word EVTT_MSGREPLY       = 0x3439;
const word EVTT_MSGTOYOU       = 0xFB77;
const word EVTT_MSGWRITING     = 0x0CA8;
const word EVTT_NODEBROWSE     = 0xB6B5;
const word EVTT_SAYBIBI        = 0xD39D;
const word EVTT_SEARCHFAILED   = 0xEB54;
const word EVTT_SEARCHSUCCESS  = 0x2EB2;
const word EVTT_STARTUP        = 0x494B;
const word EVTT_TIMEOUT        = 0x4065;

const word EVTT_BREAKLOOP      = 0xFFFF;
const word EVTT_STOPVOICE      = 0xFFFE;
const word EVTT_REMOVEVOCBUF   = 0xFFFD;
const word EVTT_INITCARD       = 0xFFFC;

const int EVTC_DO_NOTHING    = 0;
const int EVTC_PLAY          = 1;

struct GEvent {
  word type;       // EVTT crc value
  int  command;    // EVTC value
  union {
    GPlay play;
    // Other event command structures can be added here
  };
  GEvent& operator=(const GEvent& a) {
    type = a.type;
    command = a.command;
    switch(command) {
      case EVTC_PLAY:
        memcpy(&play, &a.play, sizeof(play));
        break;
    }
    return *this;
  }
};


//  ------------------------------------------------------------------
//  Sound device structure

const int SNDDEV_NONE  = 0;
const int SNDDEV_DOS   = 1;
const int SNDDEV_CARD  = 2;

struct SoundDos {
  int  type;
  // Doesn't need more data..
};


//  ------------------------------------------------------------------
//  Enums for the marking menu (GEMENU etc)

enum {
  TAG_MARKYOURMAIL=100,
  TAG_MARKHEADER,
  TAG_MARKTXTHDR,
  TAG_MARKTHREAD,
  TAG_MARKASREAD,
  TAG_MARKNEWMSGS,
  TAG_MARKOLDMSGS,
  TAG_MARKALLMSGS,
  TAG_MARKUNMARK,
  TAG_MARKRANGE,
  TAG_MARKTOGGLE,
  TAG_MARKQUIT
};

   
//  ------------------------------------------------------------------
//  The Message Browser

struct MLst {
  uint32_t  msgno;
  byte      high;
  char      num[8];
  char      marks[3];
  char      by[sizeof(Name)+20];
  char      to[sizeof(Name)+20];
  Subj      re;
  time32_t  written;
  time32_t  arrived;
  time32_t  received;
  char      goldmark;
  int       colorby;
  int       colorto;
};


//  ------------------------------------------------------------------
//  Structure for GOLDNODE.IXL

struct NodeIxl {

  // Nodelists
  int    nodelists;
  Stamp* nodelist;

  // Index files
  Path addrindex;
  Path nodeindex;

};


//  ------------------------------------------------------------------
//  GEUSER

struct infoLookup {
  char*  attr;
  char*  node;
  char*  system;
  char*  location;
  char*  name;
  char*  phone;
  char*  baud;
  char*  flags;
};


//  ------------------------------------------------------------------
//  Structures and constants for external utilities

const int EXTUTIL_CLS          = 0x0001;
const int EXTUTIL_CURSOR       = 0x0004;
const int EXTUTIL_RELOAD       = 0x0008;
const int EXTUTIL_PAUSE        = 0x0010;
const int EXTUTIL_PAUSEONERROR = 0x0020;
const int EXTUTIL_WIPE         = 0x0040;
const int EXTUTIL_KEEPCTRL     = 0x0080;

struct ExtUtil {
  int  utilno;
  int  options;
  char cmdline[256];
};

struct SaveUtil {
  int  utilno;
  char menutext[41];
};


//  ------------------------------------------------------------------
//  Window areas

#define GC_BACK_    CFG->color[0]
#define GC_STAT_    CFG->color[1]
#define GC_BRAG_    CFG->color[2]
#define GC_AREA_    CFG->color[3]
#define GC_READ_    CFG->color[4]
#define GC_HEAD_    CFG->color[5]
#define GC_ASK__    CFG->color[6]
#define GC_MENU_    CFG->color[7]
#define GC_HELP_    CFG->color[8]
#define GC_CFG_     CFG->color[9]
#define GC_INFO_    CFG->color[10]
#define GC_CFGB_    CFG->color[11]
#define GC_STYLE_   CFG->color[12]
#define GC_SHAD_    CFG->color[13]


//  ------------------------------------------------------------------
//  Window parts

#define _Window    c[0]
#define _Border    c[1]
#define _Title     c[2]
#define _Cursor    c[2]
#define _Selector  c[3]
#define _Quote     c[3]
#define _Subject   c[3]
#define _Highlight c[4]
#define _Noselect  c[5]
#define _Kludge    c[5]
#define _Location  c[5]
#define _Tearline  c[6]
#define _Edit      c[6]
#define _Block     c[7]
#define _Input     c[7]
#define _Shadow    c[7]
#define _Origin    c[8]
#define _To        c[8]
#define _From      c[9]
#define _Tagline   c[9]
#define _Hidden    c[10]
#define _Pagebar   c[11]
#define _Signature c[12]
#define _Quote2    c[13]
#define _URL       c[14]

#define _WindowUnsent     c[6]
#define _WindowUnread     c[7]
#define _HighlightUnsent  c[8]
#define _HighlightUnread  c[9]

#define _Bold                         c[1]
#define _Italic                       c[2]
#define _BoldItalic                   c[3]
#define _Underscore                   c[4]
#define _BoldUnderscore               c[5]
#define _ItalicUnderscore             c[6]
#define _BoldItalicUnderscore         c[7]
#define _Reverse                      c[8]
#define _ReverseBold                  c[9]
#define _ReverseItalic                c[10]
#define _ReverseBoldItalic            c[11]
#define _ReverseUnderscore            c[12]
#define _ReverseBoldUnderscore        c[13]
#define _ReverseItalicUnderscore      c[14]
#define _ReverseBoldItalicUnderscore  c[15]


//  ------------------------------------------------------------------
//  Window part colors

#define C_BACKW   GC_BACK_._Window
#define C_BACKB   GC_BACK_._Border

#define C_STATW   GC_STAT_._Window
#define C_STATB   GC_STAT_._Border

#define C_BRAGW   GC_BRAG_._Window
#define C_BRAGB   GC_BRAG_._Border
#define C_BRAGT   GC_BRAG_._Title
#define C_BRAGH   GC_BRAG_._Highlight
#define C_BRAGA   GC_BRAG_._Block

#define C_AREAW   GC_AREA_._Window
#define C_AREAB   GC_AREA_._Border
#define C_AREAT   GC_AREA_._Title
#define C_AREAS   GC_AREA_._Selector
#define C_AREAQ   GC_AREA_._Highlight
#define C_AREAPB  GC_AREA_._Pagebar

#define C_READW   GC_READ_._Window
#define C_READB   GC_READ_._Border
#define C_READQ   GC_READ_._Quote
#define C_READK   GC_READ_._Kludge
#define C_READKH  GC_READ_._Hidden
#define C_READC   GC_READ_._Cursor
#define C_READA   GC_READ_._Block
#define C_READH   GC_READ_._Highlight
#define C_READG   GC_READ_._Tagline
#define C_READT   GC_READ_._Tearline
#define C_READO   GC_READ_._Origin
#define C_READPB  GC_READ_._Pagebar
#define C_READS   GC_READ_._Signature
#define C_READQ2  GC_READ_._Quote2
#define C_READU   GC_READ_._URL
#define C_HEADW   GC_HEAD_._Window
#define C_HEADB   GC_HEAD_._Border
#define C_HEADT   GC_HEAD_._Title
#define C_HEADE   GC_HEAD_._Edit
#define C_HEADI   GC_HEAD_._Input
#define C_HEADQ   GC_HEAD_._Highlight
#define C_HEADBY  GC_HEAD_._From
#define C_HEADTO  GC_HEAD_._To
#define C_HEADRE  GC_HEAD_._Subject
#define C_HEADLOC GC_HEAD_._Location

#define C_ASKW    GC_ASK__._Window
#define C_ASKB    GC_ASK__._Border
#define C_ASKT    GC_ASK__._Title
#define C_ASKS    GC_ASK__._Selector
#define C_ASKQ    GC_ASK__._Highlight
#define C_ASKN    GC_ASK__._Noselect
#define C_ASKPB   GC_ASK_._Pagebar

#define C_MENUW   GC_MENU_._Window
#define C_MENUB   GC_MENU_._Border
#define C_MENUT   GC_MENU_._Title
#define C_MENUS   GC_MENU_._Selector
#define C_MENUQ   GC_MENU_._Highlight
#define C_MENUN   GC_MENU_._Noselect
#define C_MENUPB  GC_MENU_._Pagebar
#define C_MENUW_UNREAD      GC_MENU_._WindowUnread
#define C_MENUQ_UNREADHIGH  GC_MENU_._HighlightUnread
#define C_MENUW_UNSENT      GC_MENU_._WindowUnsent
#define C_MENUQ_UNSENTHIGH  GC_MENU_._HighlightUnsent

#define C_HELPW   GC_HELP_._Window
#define C_HELPB   GC_HELP_._Border
#define C_HELPT   GC_HELP_._Title
#define C_HELPS   GC_HELP_._Selector
#define C_HELPQ   GC_HELP_._Highlight
#define C_HELPPB  GC_HELP_._Pagebar

#define C_INFOW   GC_INFO_._Window
#define C_INFOB   GC_INFO_._Border
#define C_INFOT   GC_INFO_._Title
#define C_INFOPB  GC_INFO_._Pagebar

#define C_CFGW    GC_CFG_._Window
#define C_CFGB    GC_CFG_._Border
#define C_CFGT    GC_CFG_._Title
#define C_CFGS    GC_CFG_._Selector
#define C_CFGQ    GC_CFG_._Highlight
#define C_CFGN    GC_CFG_._Noselect
#define C_CFGPB   GC_CFG_._Pagebar

#define C_CFGBW   GC_CFGB_._Window
#define C_CFGBB   GC_CFGB_._Border
#define C_CFGBT   GC_CFGB_._Title
#define C_CFGBS   GC_CFGB_._Selector
#define C_CFGBQ   GC_CFGB_._Highlight
#define C_CFGBN   GC_CFGB_._Noselect
#define C_CFGBPB  GC_CFGB_._Pagebar

#define C_SHADOW  GC_SHAD_._Shadow

#define C_STYLE     GC_STYLE_.c
#define C_STYLE___B GC_STYLE_._Bold
#define C_STYLE__I_ GC_STYLE_._Italic
#define C_STYLE__IB GC_STYLE_._BoldItalic
#define C_STYLE_U__ GC_STYLE_._Underscore
#define C_STYLE_U_B GC_STYLE_._BoldUnderscore
#define C_STYLE_UI_ GC_STYLE_._ItalicUnderscore
#define C_STYLE_UIB GC_STYLE_._BoldItalicUnderscore
#define C_STYLER___ GC_STYLE_._Reverse
#define C_STYLER__B GC_STYLE_._ReverseBold
#define C_STYLER_I_ GC_STYLE_._ReverseItalic
#define C_STYLER_IB GC_STYLE_._ReverseBoldItalic
#define C_STYLERU__ GC_STYLE_._ReverseUnderscore
#define C_STYLERU_B GC_STYLE_._ReverseBoldUnderscore
#define C_STYLERUI_ GC_STYLE_._ReverseItalicUnderscore
#define C_STYLERUIB GC_STYLE_._ReverseBoldItalicUnderscore


//  ------------------------------------------------------------------
//  Window handles

#define W_BACK    GC_BACK_.w
#define W_STAT    GC_STAT_.w
#define W_BRAG    GC_BRAG_.w
#define W_AREA    GC_AREA_.w
#define W_READ    GC_READ_.w
#define W_HEAD    GC_READ_.w
#define W_ASK     GC_ASK__.w
#define W_MENU    GC_MENU_.w
#define W_INFO    GC_INFO_.w


//  ------------------------------------------------------------------
//  Window offsets

#define W_XBACK   GC_BACK_.x
#define W_XSTAT   GC_STAT_.x
#define W_XBRAG   GC_BRAG_.x
#define W_XAREA   GC_AREA_.x
#define W_XREAD   GC_READ_.x
#define W_XHEAD   GC_READ_.x
#define W_XASK    GC_ASK__.x
#define W_XMENU   GC_MENU_.x
#define W_XINFO   GC_INFO_.x

#define W_YBACK   GC_BACK_.y
#define W_YSTAT   GC_STAT_.y
#define W_YBRAG   GC_BRAG_.y
#define W_YAREA   GC_AREA_.y
#define W_YREAD   GC_READ_.y
#define W_YHEAD   GC_READ_.y
#define W_YASK    GC_ASK__.y
#define W_YMENU   GC_MENU_.y
#define W_YINFO   GC_INFO_.y


//  ------------------------------------------------------------------
//  Window border types

#define W_BBACK   GC_BACK_.btype
#define W_BSTAT   GC_STAT_.btype
#define W_BBRAG   GC_BRAG_.btype
#define W_BAREA   GC_AREA_.btype
#define W_BREAD   GC_READ_.btype
#define W_BHEAD   GC_READ_.btype
#define W_BASK    GC_ASK__.btype
#define W_BMENU   GC_MENU_.btype
#define W_BHELP   GC_HELP_.btype
#define W_BINFO   GC_INFO_.btype
#define W_BCFG    GC_CFG_.btype
#define W_BCFGB   GC_CFGB_.btype


//  ------------------------------------------------------------------
//  Window structure

struct Win {
  int btype;        // Border type
  int c[16];        // Colors
  int w;            // Window handle
};

//  ------------------------------------------------------------------
//  Charset translation table structure

typedef byte ChsTab[4];

struct Chs {
  long   id;
  int    version;
  int    level;
  XlatName imp;   // From Charset
  XlatName exp;   // To Charset
  ChsTab t[256];       // The Translation Table
};


//  ------------------------------------------------------------------
//  Escaped characters table structure

typedef byte EscTab[5];

struct Esc {
  XlatName exp;   // To Charset
  byte   esc;          // Escape character
  int    pos1;         // Relative position of first character
  int    pos2;         // Relative position of last character
  uint   size;         // Number of escaped chars in table
  EscTab t[256];       // The Escaped Characters Table
};


//  ------------------------------------------------------------------
//  Charset mapping files

struct Map {
  XlatName imp;         // From Charset
  XlatName exp;         // To Charset
  char* mapfile;        // Mapping file and timestamp
};


//  ------------------------------------------------------------------

struct PosLen {
  int pos;
  int len;
};


//  ------------------------------------------------------------------

struct Ezycom {
  Path msgbasepath;
  Path userbasepath;
};


//  ------------------------------------------------------------------

struct Invalidate {
  std::pair<std::string, std::string> origin, seenby, tearline, cc, xc, xp;
};


//  ------------------------------------------------------------------

struct Semaphore {
  Path netscan;
  Path echoscan;
  Path exportlist;
  Path importlist;
  Path scanall;
  Path scanthis;
  Path scannetmail;
  Path pmscanall;
  Path pmscanthis;
  Path pmscannetmail;
  Path qwkimport;
  Path qwkexport;
  Path soupimport;
  Path soupexport;
  Path exitnow;

  uint  idletime;
};


//  ------------------------------------------------------------------

struct AkaMatchG {
  Addr mask;
  Addr aka;
};


//  ------------------------------------------------------------------

struct MailList {
  Echo echoid;
  IAdr sender;
  IAdr contribution;
  bool sender_is_pattern;
};


//  ------------------------------------------------------------------

struct FrqNodeMap {
  Addr from;
  Addr to;
};


//  ------------------------------------------------------------------

typedef char BarList[45];

struct FFblk {
  int  year;
  int  month;
  int  day;
  int  hour;
  int  minute;
  int  second;
  long size;
  Path name;
  bool selected;           // Extra - YES/NO
};

struct FileSpec {
  Path   path;             // Path to file(s)
  char   password[20];     // File Password
  int    files;            // Number of files
  bool   delsent;          // Kill files after sent
  FFblk* fblk;             // Filenames
};


//  ------------------------------------------------------------------

const int RFCAddress = 1;
const int RFCName = 2;

//  ------------------------------------------------------------------
//  Message written by/to me/you ?

const uint TO_ME   = 1;
const uint BY_ME   = 2;
const uint TO_YOU  = 4;
const uint BY_YOU  = 8;
const uint TO_ALL  = 16;

//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack()
#endif


//  ------------------------------------------------------------------
//  Include various GoldED structures

#include <gehelp.h>
#include <gekeys.h>
#include <gelang.h>
#include <gmarea.h>
#include <gecfgg.h>


//  ------------------------------------------------------------------

