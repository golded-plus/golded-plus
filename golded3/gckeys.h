
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
//  CRC16 value for keyboard definitions.
//  ------------------------------------------------------------------


//  ------------------------------------------------------------------
//  CRC16 value for keyboard definitions

const word CRC_AREAABORT            = 0x1E02;
const word CRC_AREAASKEXIT          = 0xA3D1;
const word CRC_AREABOARDNOS         = 0x96EA;
const word CRC_AREACATCHUP          = 0xDA5C;
const word CRC_AREADOSSHELL         = 0x2DED;
const word CRC_AREADROPMSGMARKS     = 0xB90C;
const word CRC_AREAGOTOFIRST        = 0x41CB;
const word CRC_AREAGOTOLAST         = 0x177A;
const word CRC_AREAGOTONEXT         = 0x3A9C;
const word CRC_AREAGOTOPREV         = 0x6534;
const word CRC_AREAHEAT             = 0x5B15;
const word CRC_AREAINFO             = 0xDE54;
const word CRC_AREAJUMP             = 0x2342;
const word CRC_AREAJUMPNEXTMATCH    = 0xA556;
const word CRC_AREAMACRO            = 0xFFA7;
const word CRC_AREAMENU             = 0xABE1;
const word CRC_AREAQUITNOW          = 0xAAC4;
const word CRC_AREASCAN             = 0xE440;
const word CRC_AREASCANPM           = 0xC4FF;
const word CRC_AREASELECT           = 0xA271;
const word CRC_AREASELECTMARKS      = 0x0AC3;
const word CRC_AREASOUNDKILL        = 0xDFD3;
const word CRC_AREATOGGLE           = 0xC734;
const word CRC_AREATOUCHNETSCAN     = 0x58C1;
const word CRC_AREAUNDEFINE         = 0x835B;
const word CRC_AREAWRITEGOLDLAST    = 0xD353;
const word CRC_AREAZAP              = 0x5125;
const word CRC_AREASHOWDEL          = 0x80C4;

const word CRC_EDITABORT            = 0x6468;
const word CRC_EDITANCHOR           = 0xDC07;
const word CRC_EDITASKEXIT          = 0x89AF;
const word CRC_EDITBLOCKDOWN        = 0xD4E4;
const word CRC_EDITBLOCKEND         = 0xF55B;
const word CRC_EDITBLOCKHOME        = 0x8B82;
const word CRC_EDITBLOCKLEFT        = 0xED1D;
const word CRC_EDITBLOCKPGUP        = 0x9845;
const word CRC_EDITBLOCKPGDN        = 0x895B;
const word CRC_EDITBLOCKRIGHT       = 0x20F9;
const word CRC_EDITBLOCKUP          = 0xE004;
const word CRC_EDITCLEARDELETEBUF   = 0xF8DE;
const word CRC_EDITCLEARPASTEBUF    = 0xE8E4;
const word CRC_EDITCOPY             = 0x1F77;
const word CRC_EDITCOPYABOVECHAR    = 0x380C;
const word CRC_EDITCUT              = 0xFAE6;
const word CRC_EDITDELCHAR          = 0xA3C7;
const word CRC_EDITDELETE           = 0x4A37;
const word CRC_EDITDELETEEOL        = 0x1143;
const word CRC_EDITDELLEFT          = 0x6552;
const word CRC_EDITDELLINE          = 0xACCF;
const word CRC_EDITDELLTWORD        = 0x3B67;
const word CRC_EDITDELRTWORD        = 0x829A;
const word CRC_EDITDOSSHELL         = 0xD6C5;
const word CRC_EDITDUPLINE          = 0x816A;
const word CRC_EDITEXITMSG          = 0x1813;
const word CRC_EDITEXPORTTEXT       = 0x5AED;
const word CRC_EDITGOBEGLINE        = 0xE045;
const word CRC_EDITGOBOTLINE        = 0x92DD;
const word CRC_EDITGOBOTMSG         = 0x9B29;
const word CRC_EDITGODOWN           = 0x3DC2;
const word CRC_EDITGOEOL            = 0x1CC6;
const word CRC_EDITGOLEFT           = 0x043B;
const word CRC_EDITGOPGDN           = 0x607D;
const word CRC_EDITGOPGUP           = 0x7163;
const word CRC_EDITGORIGHT          = 0x6AFE;
const word CRC_EDITGOTOPLINE        = 0x87C2;
const word CRC_EDITGOTOPMSG         = 0x6BD3;
const word CRC_EDITGOUP             = 0xD6BA;
const word CRC_EDITGOWORDLEFT       = 0xE48C;
const word CRC_EDITGOWORDRIGHT      = 0x20D0;
const word CRC_EDITHEADER           = 0x726F;
const word CRC_EDITIMPORTQUOTEBUF   = 0xF797;
const word CRC_EDITIMPORTTEXT       = 0x0586;
const word CRC_EDITKILLQUOTES       = 0xD821;
const word CRC_EDITLOADFILE         = 0x0931;
const word CRC_EDITLOOKUPCURSOR     = 0xB7CC;
const word CRC_EDITLOOKUPDEST       = 0xFEC4;
const word CRC_EDITLOOKUPORIG       = 0x5AFB;
const word CRC_EDITMACRO            = 0x85CD;
const word CRC_EDITMAINMENU         = 0xDE5D;
const word CRC_EDITMENU             = 0x833E;
const word CRC_EDITNEWLINE          = 0x59F3;
const word CRC_EDITPASTE            = 0x90C4;
const word CRC_EDITQUITNOW          = 0x80BA;
const word CRC_EDITREFLOW           = 0x13F9;
const word CRC_EDITSAVEFILE         = 0x64A4;
const word CRC_EDITSAVEMSG          = 0x660C;
const word CRC_EDITSOUNDKILL        = 0xA9A7;
const word CRC_EDITSPELLCHECK       = 0xE167;
const word CRC_EDITTAB              = 0x8C26;
const word CRC_EDITTABREVERSE       = 0xF5B6;
const word CRC_EDITTOGGLECASE       = 0xC282;
const word CRC_EDITTOGGLEINSERT     = 0xD732;
const word CRC_EDITTOLOWER          = 0x6D85;
const word CRC_EDITTOUPPER          = 0xD3DD;
const word CRC_EDITUNDEFINE         = 0x7873;
const word CRC_EDITUNDELETE         = 0x4C34;
const word CRC_EDITUNDO             = 0xB295;
const word CRC_EDITZAPQUOTEBELOW    = 0x814F;

const word CRC_FILEABORT            = 0xBCED;
const word CRC_FILEASKEXIT          = 0xD40C;
const word CRC_FILEDOSSHELL         = 0xFE9D;
const word CRC_FILEGOTOFIRST        = 0xCAD5;
const word CRC_FILEGOTOLAST         = 0xC40A;
const word CRC_FILEGOTONEXT         = 0xE9EC;
const word CRC_FILEGOTOPREV         = 0xB644;
const word CRC_FILEMACRO            = 0x5D48;
const word CRC_FILEMARK             = 0xF828;
const word CRC_FILEMARKALL          = 0xD115;
const word CRC_FILEMENU             = 0xA4B2;
const word CRC_FILEQUITNOW          = 0xDD19;
const word CRC_FILESELECT           = 0xD8D9;
const word CRC_FILETOGGLEMARK       = 0xB431;
const word CRC_FILEUNDEFINE         = 0x502B;
const word CRC_FILEUNMARK           = 0x6D1B;
const word CRC_FILEUNMARKALL        = 0x121A;

const word CRC_LISTABORT            = 0x4313;
const word CRC_LISTASKEXIT          = 0xD922;
const word CRC_LISTDOSSHELL         = 0x0130;
const word CRC_LISTGOTOBOOKMARK     = 0x0F08;
const word CRC_LISTGOTOFIRST        = 0x7925;
const word CRC_LISTGOTOLAST         = 0x3BA7;
const word CRC_LISTGOTONEXT         = 0x1641;
const word CRC_LISTGOTOPREV         = 0x49E9;
const word CRC_LISTMACRO            = 0xA2B6;
const word CRC_LISTMARKINGOPTIONS   = 0x8138;
const word CRC_LISTMENU             = 0x99AA;
const word CRC_LISTQUITNOW          = 0xD037;
const word CRC_LISTSELECT           = 0x3829;
const word CRC_LISTTOGGLEBOOKMARK   = 0xCFA0;
const word CRC_LISTTOGGLEDATE       = 0x6F1F;
const word CRC_LISTTOGGLEMARK       = 0xD389;
const word CRC_LISTTOGGLEWIDESUBJ   = 0x3C76;
const word CRC_LISTUNDEFINE         = 0xAF86;

const word CRC_EXTERNUTIL01         = 0x95B9;
const word CRC_EXTERNUTIL02         = 0x95BA;
const word CRC_EXTERNUTIL03         = 0x95BB;
const word CRC_EXTERNUTIL04         = 0x95BC;
const word CRC_EXTERNUTIL05         = 0x95BD;
const word CRC_EXTERNUTIL06         = 0x95BE;
const word CRC_EXTERNUTIL07         = 0x95BF;
const word CRC_EXTERNUTIL08         = 0x95B0;
const word CRC_EXTERNUTIL09         = 0x95B1;
const word CRC_EXTERNUTIL10         = 0x94B8;
const word CRC_EXTERNUTIL11         = 0x94B9;
const word CRC_EXTERNUTIL12         = 0x94BA;
const word CRC_EXTERNUTIL13         = 0x94BB;
const word CRC_EXTERNUTIL14         = 0x94BC;
const word CRC_EXTERNUTIL15         = 0x94BD;
const word CRC_EXTERNUTIL16         = 0x94BE;
const word CRC_EXTERNUTIL17         = 0x94BF;
const word CRC_EXTERNUTIL18         = 0x94B0;
const word CRC_EXTERNUTIL19         = 0x94B1;
const word CRC_EXTERNUTIL20         = 0x97B8;
const word CRC_EXTERNUTIL21         = 0x97B9;
const word CRC_EXTERNUTIL22         = 0x97BA;
const word CRC_EXTERNUTIL23         = 0x97BB;
const word CRC_EXTERNUTIL24         = 0x97BC;

const word CRC_NODEABORT            = 0xA08C;
const word CRC_NODEASKEXIT          = 0xEE95;
const word CRC_NODEDOSSHELL         = 0xF084;
const word CRC_NODEGOTOFIRST        = 0x321B;
const word CRC_NODEGOTOLAST         = 0xCA13;
const word CRC_NODEGOTONEXT         = 0xE7F5;
const word CRC_NODEGOTOPREV         = 0xB85D;
const word CRC_NODEMACRO            = 0x4129;
const word CRC_NODEMAINMENU         = 0xF81C;
const word CRC_NODEMENU             = 0xE1B6;
const word CRC_NODEQUITNOW          = 0xE780;
const word CRC_NODESELECT           = 0x6A64;
const word CRC_NODEUNDEFINE         = 0x5E32;

const word CRC_READADDRESSBOOK      = 0xBAC1;
const word CRC_READADDRESSBOOKADD   = 0x00FA;
const word CRC_READASKEXIT          = 0xA500;
const word CRC_READCHANGEAKA        = 0x2015;
const word CRC_READCHANGEATTRS      = 0x47F8;
const word CRC_READCHANGECHARSIN    = 0x0B87;
const word CRC_READCHANGECHARSOUT   = 0x4DF9;
const word CRC_READCHANGEMSG        = 0xF99F;
const word CRC_READCHANGEORIGIN     = 0xFD2C;
const word CRC_READCHANGETAGLINE    = 0xBDD2;
const word CRC_READCHANGETEMPLATE   = 0x6C86;
const word CRC_READCHANGEUSERNAME   = 0x7438;
const word CRC_READCHANGEXLATIMPORT = 0xF169;
const word CRC_READCOMMENTMSG       = 0x5E89;
const word CRC_READCOPYMOVEFORWARD  = 0xBFAF;
const word CRC_READDECREASEMARGIN   = 0xD8EA;
const word CRC_READDELETEMSG        = 0x8BCE;
const word CRC_READDOSSHELL         = 0x9C2B;
const word CRC_READEXTERNUTILMENU   = 0x2000;
const word CRC_READFIDORENUMBER     = 0x1E7C;
const word CRC_READFILEREQUEST      = 0x0A07;
const word CRC_READFINDALL          = 0xEF19;
const word CRC_READFINDHEADER       = 0x6343;
const word CRC_READGOTOBOOKMARK     = 0x4383;
const word CRC_READGOTOFIRSTMSG     = 0x392F;
const word CRC_READGOTOLASTMSG      = 0x9E72;
const word CRC_READGOTOMSGNO        = 0x73F7;
const word CRC_READGOTONEXTAREA     = 0x5B23;
const word CRC_READGOTONEXTMSG      = 0xE0F1;
const word CRC_READGOTONEXTUNREAD   = 0xBA34;
const word CRC_READGOTOPREVAREA     = 0xEECB;
const word CRC_READGOTOPREVMSG      = 0x06D9;
const word CRC_READGOTOPREVUNREAD   = 0x27EA;
const word CRC_READGOTOREPLIES      = 0x1A6B;
const word CRC_READGOTOREPLY1ST     = 0xE54F;
const word CRC_READGOTOREPLYNEXT    = 0x844F;
const word CRC_READGOTOREPLYPREV    = 0xDBE7;
const word CRC_READINCREASEMARGIN   = 0x1797;
const word CRC_READLOOKUPDEST       = 0x0023;
const word CRC_READLOOKUPORIG       = 0xA41C;
const word CRC_READMACRO            = 0xF7D8;
const word CRC_READMAINMENU         = 0x94B3;
const word CRC_READMAKEUSERLIST     = 0xE8C6;
const word CRC_READMAKEPATHREPORT   = 0x4AA6;
const word CRC_READMARKINGOPTIONS   = 0xE97A;
const word CRC_READMENU             = 0x3D0A;
const word CRC_READMESSAGELIST      = 0xD5D8;
const word CRC_READMOVECOMMENTMSG   = 0xE4E7;
const word CRC_READMOVEQUOTEMSG     = 0x1B41;
const word CRC_READMOVEDIRQUOTEMSG  = 0xDBC0;
const word CRC_READMSGCONTINUE      = 0x4511;
const word CRC_READMSGEND           = 0xC5AD;
const word CRC_READMSGHOME          = 0x4BD1;
const word CRC_READMSGLINEDOWN      = 0x5DB2;
const word CRC_READMSGLINEUP        = 0x7238;
const word CRC_READMSGPGDN          = 0x4908;
const word CRC_READMSGPGUP          = 0x5816;
const word CRC_READNEWAREA          = 0xAB5E;
const word CRC_READNEWMSG           = 0x30CA;
const word CRC_READQUITNOW          = 0xAC15;
const word CRC_READQUOTEBUF         = 0x49EE;
const word CRC_READQUOTEMSG         = 0xBE00;
const word CRC_READDIRQUOTEMSG      = 0xB351;
const word CRC_READREPLYMSG         = 0x172F;
const word CRC_READSEARCH           = 0x48EF;
const word CRC_READSOUNDKILL        = 0xAE29;
const word CRC_READSTEALTAGLINE     = 0x7480;
const word CRC_READTHREADTREE       = 0x9827;
const word CRC_READTOGGLEBOOKMARK   = 0xA7E2;
const word CRC_READTOGGLEHEXDUMP    = 0xE6D5;
const word CRC_READTOGGLEHIDDEN     = 0x04CE;
const word CRC_READTOGGLEHIDDKLUD   = 0xA24F;
const word CRC_READTOGGLESTYLES     = 0xF47E;
const word CRC_READTOGGLEKLUDGE     = 0x46AB;
const word CRC_READTOGGLEMARK       = 0x1E64;
const word CRC_READTOGGLEMARKREAD   = 0xCFAE;
const word CRC_READTOGGLEPAGEBAR    = 0x18E4;
const word CRC_READTOGGLEQUOTE      = 0xB293;
const word CRC_READTOGGLEREALMSGNO  = 0x2A3B;
const word CRC_READTOGGLEROT13      = 0xC157;
const word CRC_READTOGGLETWITS      = 0x5FD1;
const word CRC_READTOUCHNETSCAN     = 0x4BF3;
const word CRC_READTOUCHSEMAPHORE   = 0x6B1D;
const word CRC_READUNDEFINE         = 0x329D;
const word CRC_READUUDECODE         = 0x4F0B;
const word CRC_READWRITEMSG         = 0x73AC;

const word CRC_ADDRESSBOOKQUIT      = 0x47C7;
const word CRC_ADDRESSBOOKADD       = 0xFF55;
const word CRC_ADDRESSBOOKSELECT    = 0x2D4A;
const word CRC_ADDRESSBOOKDELETE    = 0x70D1;
const word CRC_ADDRESSBOOKPACK      = 0x2C5C;

const word CRC_HEADERADDRESSBOOK    = 0x76FC;
const word CRC_HEADERLOOKUP         = 0x64F2;
const word CRC_HEADERTOGGLEATTRWIN  = 0x6F95;
const word CRC_HEADERTOGGLERESERVED = 0xF297;
const word CRC_HEADERTOGGLEGROUPMSG = 0x0C51;
const word CRC_HEADERTOGGLESCANNED  = 0x0459;
const word CRC_HEADERTOGGLEFILE     = 0xC997;
const word CRC_HEADERTOGGLEARCSENT  = 0x88A7;
const word CRC_HEADERTOGGLECRASH    = 0x2CF9;
const word CRC_HEADERTOGGLEDIRECT   = 0xD1D2;
const word CRC_HEADERTOGGLEDELSENT  = 0xCD4B;
const word CRC_HEADERTOGGLEFREQ     = 0x63D9;
const word CRC_HEADERTOGGLEZONEGATE = 0x140B;
const word CRC_HEADERTOGGLEHOLD     = 0x8A5F;
const word CRC_HEADERTOGGLEIMM      = 0x6E15;
const word CRC_HEADERTOGGLETRANSIT  = 0xC112;
const word CRC_HEADERTOGGLEKILL     = 0xBFC2;
const word CRC_HEADERTOGGLELOCKED   = 0x312D;
const word CRC_HEADERTOGGLERETRECREQ= 0x2CF1;
const word CRC_HEADERTOGGLERETREC   = 0x56EA;
const word CRC_HEADERTOGGLEORPHAN   = 0xC98A;
const word CRC_HEADERTOGGLEPVT      = 0xF614;
const word CRC_HEADERTOGGLEAUDIT    = 0x91B7;
const word CRC_HEADERTOGGLERECEIVED = 0x36B9;
const word CRC_HEADERTOGGLESENT     = 0xF68C;
const word CRC_HEADERTOGGLETRUNC    = 0xA5B4;
const word CRC_HEADERTOGGLEUPDREQ   = 0xAC35;
const word CRC_HEADERTOGGLEHUBHOST  = 0x2505;
const word CRC_HEADERTOGGLELOCAL    = 0x47A1;
const word CRC_HEADERTOGGLEXMAIL    = 0x9622;
const word CRC_HEADERTOGGLECFMRECREQ= 0xC12D;
const word CRC_HEADERCLEARATTRIB    = 0xBED5;

const word CRC_K_F1                 = 0x4631;
const word CRC_K_F2                 = 0x4632;
const word CRC_K_F3                 = 0x4633;
const word CRC_K_F4                 = 0x4634;
const word CRC_K_F5                 = 0x4635;
const word CRC_K_F6                 = 0x4636;
const word CRC_K_F7                 = 0x4637;
const word CRC_K_F8                 = 0x4638;
const word CRC_K_F9                 = 0x4639;
const word CRC_K_F10                = 0x1932;
const word CRC_K_F11                = 0x1933;
const word CRC_K_F12                = 0x1930;

const word CRC_K_SF1                = 0x5230;
const word CRC_K_SF2                = 0x5233;
const word CRC_K_SF3                = 0x5232;
const word CRC_K_SF4                = 0x5235;
const word CRC_K_SF5                = 0x5234;
const word CRC_K_SF6                = 0x5237;
const word CRC_K_SF7                = 0x5236;
const word CRC_K_SF8                = 0x5239;
const word CRC_K_SF9                = 0x5238;
const word CRC_K_SF10               = 0x4A87;
const word CRC_K_SF11               = 0x4A86;
const word CRC_K_SF12               = 0x4A85;

const word CRC_K_AF1                = 0x0EF5;
const word CRC_K_AF2                = 0x0EF6;
const word CRC_K_AF3                = 0x0EF7;
const word CRC_K_AF4                = 0x0EF0;
const word CRC_K_AF5                = 0x0EF1;
const word CRC_K_AF6                = 0x0EF2;
const word CRC_K_AF7                = 0x0EF3;
const word CRC_K_AF8                = 0x0EFC;
const word CRC_K_AF9                = 0x0EFD;
const word CRC_K_AF10               = 0x14FE;
const word CRC_K_AF11               = 0x14FF;
const word CRC_K_AF12               = 0x14FC;

const word CRC_K_CF1                = 0xFD0A;
const word CRC_K_CF2                = 0xFD09;
const word CRC_K_CF3                = 0xFD08;
const word CRC_K_CF4                = 0xFD0F;
const word CRC_K_CF5                = 0xFD0E;
const word CRC_K_CF6                = 0xFD0D;
const word CRC_K_CF7                = 0xFD0C;
const word CRC_K_CF8                = 0xFD03;
const word CRC_K_CF9                = 0xFD02;
const word CRC_K_CF10               = 0x3482;
const word CRC_K_CF11               = 0x3483;
const word CRC_K_CF12               = 0x3480;

const word CRC_K_A0                 = 0x4030;
const word CRC_K_A1                 = 0x4031;
const word CRC_K_A2                 = 0x4032;
const word CRC_K_A3                 = 0x4033;
const word CRC_K_A4                 = 0x4034;
const word CRC_K_A5                 = 0x4035;
const word CRC_K_A6                 = 0x4036;
const word CRC_K_A7                 = 0x4037;
const word CRC_K_A8                 = 0x4038;
const word CRC_K_A9                 = 0x4039;

const word CRC_K_AA                 = 0x4041;
const word CRC_K_AB                 = 0x4042;
const word CRC_K_AC                 = 0x4043;
const word CRC_K_AD                 = 0x4044;
const word CRC_K_AE                 = 0x4045;
const word CRC_K_AF                 = 0x4046;
const word CRC_K_AG                 = 0x4047;
const word CRC_K_AH                 = 0x4048;
const word CRC_K_AI                 = 0x4049;
const word CRC_K_AJ                 = 0x404A;
const word CRC_K_AK                 = 0x404B;
const word CRC_K_AL                 = 0x404C;
const word CRC_K_AM                 = 0x404D;
const word CRC_K_AN                 = 0x404E;
const word CRC_K_AO                 = 0x404F;
const word CRC_K_AP                 = 0x4050;
const word CRC_K_AQ                 = 0x4051;
const word CRC_K_AR                 = 0x4052;
const word CRC_K_AS                 = 0x4053;
const word CRC_K_AT                 = 0x4054;
const word CRC_K_AU                 = 0x4055;
const word CRC_K_AV                 = 0x4056;
const word CRC_K_AW                 = 0x4057;
const word CRC_K_AX                 = 0x4058;
const word CRC_K_AY                 = 0x4059;
const word CRC_K_AZ                 = 0x405A;

const word CRC_K_CA                 = 0x5E41;
const word CRC_K_CB                 = 0x5E42;
const word CRC_K_CC                 = 0x5E43;
const word CRC_K_CD                 = 0x5E44;
const word CRC_K_CE                 = 0x5E45;
const word CRC_K_CF                 = 0x5E46;
const word CRC_K_CG                 = 0x5E47;
const word CRC_K_CH                 = 0x5E48;
const word CRC_K_CI                 = 0x5E49;
const word CRC_K_CJ                 = 0x5E4A;
const word CRC_K_CK                 = 0x5E4B;
const word CRC_K_CL                 = 0x5E4C;
const word CRC_K_CM                 = 0x5E4D;
const word CRC_K_CN                 = 0x5E4E;
const word CRC_K_CO                 = 0x5E4F;
const word CRC_K_CP                 = 0x5E50;
const word CRC_K_CQ                 = 0x5E51;
const word CRC_K_CR                 = 0x5E52;
const word CRC_K_CS                 = 0x5E53;
const word CRC_K_CT                 = 0x5E54;
const word CRC_K_CU                 = 0x5E55;
const word CRC_K_CV                 = 0x5E56;
const word CRC_K_CW                 = 0x5E57;
const word CRC_K_CX                 = 0x5E58;
const word CRC_K_CY                 = 0x5E59;
const word CRC_K_CZ                 = 0x5E5A;
const word CRC_K_CMINUS             = 0x5E2D;

const word CRC_K_ESC                = 0x4B22;

const word CRC_K_TAB                = 0x5B33;
const word CRC_K_STAB               = 0x0886;
const word CRC_K_ATAB               = 0x56FF;

const word CRC_K_BSPACE             = 0x0F07;
const word CRC_K_ABSPACE            = 0xF9D7;
const word CRC_K_CBSPACE            = 0x5A72;

const word CRC_K_ENTER              = 0x87BD;
const word CRC_K_AENTER             = 0xE921;
const word CRC_K_CENTER             = 0x50DC;

const word CRC_K_INS                = 0x97BE;
const word CRC_K_CINS               = 0xBA0E;
const word CRC_K_AINS               = 0x9A72;

const word CRC_K_DEL                = 0x4D0C;
const word CRC_K_CDEL               = 0x60BC;
const word CRC_K_ADEL               = 0x40C0;

const word CRC_K_HOME               = 0x700B;
const word CRC_K_CHOME              = 0x35C4;
const word CRC_K_AHOME              = 0x6DA6;

const word CRC_K_END                = 0x5625;
const word CRC_K_CEND               = 0x7B95;
const word CRC_K_AEND               = 0x5BE9;

const word CRC_K_PGUP               = 0x63CC;
const word CRC_K_CPGUP              = 0x2603;
const word CRC_K_APGUP              = 0x7E61;

const word CRC_K_PGDN               = 0x72D2;
const word CRC_K_CPGDN              = 0x371D;
const word CRC_K_APGDN              = 0x6F7F;

const word CRC_K_LEFT               = 0x1694;
const word CRC_K_CLEFT              = 0x535B;
const word CRC_K_ALEFT              = 0x0B39;

const word CRC_K_RIGHT              = 0xF78D;
const word CRC_K_CRIGHT             = 0x20EC;
const word CRC_K_ARIGHT             = 0x9911;

const word CRC_K_UP                 = 0x5550;
const word CRC_K_CUP                = 0xEE6B;
const word CRC_K_AUP                = 0x1D94;

const word CRC_K_DOWN               = 0x2F6D;
const word CRC_K_CDOWN              = 0x6AA2;
const word CRC_K_ADOWN              = 0x32C0;

const word CRC_K_CGMUL              = 0x66CA;

const word CRC_K_KEY5               = 0x9062;
const word CRC_K_SPACE              = 0x1281;

const word CRC_KK_MACRO             = 0x4BE9;
const word CRC_KK_AUTO              = 0x60E2;

#if not defined(__UNIX__) or defined(__USE_NCURSES__)
const word CRC_K_SINS               = 0xC40B;
const word CRC_K_SDEL               = 0x1EB9;
const word CRC_K_SHOME              = 0xAF9D;
const word CRC_K_SEND               = 0x0590;
const word CRC_K_SPGUP              = 0xBC5A;
const word CRC_K_SPGDN              = 0xAD44;
const word CRC_K_SLEFT              = 0xC902;
const word CRC_K_SRIGHT             = 0x5B1F;
const word CRC_K_SUP                = 0x4151;
const word CRC_K_SDOWN              = 0xF0FB;
#endif

//  ------------------------------------------------------------------

