
//  ------------------------------------------------------------------
//  GoldED+
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999-2004 Alexander S. Aganichev
//  Copyright (C) 2005 Stas Degteff
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
//
//  See also http://www.gnu.org
//  ------------------------------------------------------------------
//  $Id$
//  ------------------------------------------------------------------
//  Global data.
//  ------------------------------------------------------------------

#ifndef __GEGLOB_H
#define __GEGLOB_H

#ifdef HAS_ICONV
    #include <iconv.h>
#endif

#include "gelang.h"

//  Golded+ version information --------------------------------------
#include "golded3.h"

//  ------------------------------------------------------------------

extern glog LOG;
#define LOG_NULL_POINTER \
  LOG.printf("! Parameter is null pointer. This is internal error in program code! Please make report for developers.")
extern gareafile* AFILE;
extern CfgGed* CFG;
extern LangGed* LNG;
extern NodeIxl* NODE;
extern Qwk* QWK;
extern GoldedCfgEdit* EDIT;

// GCCFGG.CPP
extern bool cfgingroup;

// GCKEYS.CPP
extern CmdKey DefaultKeyset[];

// GEAREA.CPP
extern int  area_pos;
extern int  area_width;
extern int  marked_pos;
extern int  marked_width;
extern const char marked_char;
extern int  desc_pos;
extern int  desc_width;
extern int  count_pos;
extern int  count_width;
extern int  pmark_pos;
extern int  pmark_width;
extern const char pmark_char;
extern int  unread_pos;
extern int  unread_width;
extern int  changed_pos;
extern int  changed_width;
extern const char changed_char;
extern int  echoid_pos;
extern int  echoid_width;
extern int  groupid_pos;
extern int  groupid_width;

// GEFILE.CPP
extern int fspecs;
extern int specfiles;
extern FileSpec* fspec;

// GEMENU.CPP
extern GMsg* MenuMsgPtr;

// GEREAD.CPP
extern int reader_finished;
extern int reader_done;
extern int reader_topline;
extern int reader_keyok;
extern int reader_direction;
extern int reader_rcv_noise;
extern gkey reader_keycode;
extern GMsgHeaderView *HeaderView;
extern GMsgBodyView *BodyView;

// Other

extern uint locpost, netpost, echopost;

extern Subj information;

extern Area* AA;

extern char  m_title[];
extern int   m_titlepos;
extern vattr m_titleattr;

extern int AreaKeys, ReadKeys, ListKeys, NodeKeys, EditKeys, FileKeys, AddressbookKeys, HeaderKeys;
extern std::list<CmdKey>::iterator AreaKey, ReadKey, ListKey, NodeKey, EditKey, FileKey, AddressbookKey, HeaderKey;

extern int inforow;

extern char goldmark;

extern int startecho;
extern Echo stecho;

extern int fieldupd;

extern int CurrArea;
extern int OrigArea;

extern Esc* MNETable;
extern Esc* I51Table;
extern Esc* CompTable;
extern Chs* CharTable;
extern EscTab* CompTP;
extern EscTab* I51TP;
extern EscTab* MNETP;
extern ChsTab* ChsTP;

#ifdef HAS_ICONV
    extern iconv_t iconv_cd;
#endif

//extern char gversion[];

extern bool quiet;
extern bool ignore;
extern int cfgerrors;
extern bool veryverbose;
extern bool shellvid;
extern vsavebuf* oldscreen;

extern int keysread;

extern bool disablesound;

extern int _in_editor;
extern int _in_msglist;
extern int savedirect;

extern void(*SIGINT_OldHandler)(void);

extern bool cmdlinedebughg;
extern bool cmdlineimportsoup;
extern bool cmdlineoldkeyw;
extern int cmdlinesharemode;

extern Win gold_color1[];
extern Win gold_color2[];
extern Win gold_mono1[];

extern Echo area_maybe;

extern int blanked;

extern int startupscan_success;

//  ------------------------------------------------------------------
#endif
