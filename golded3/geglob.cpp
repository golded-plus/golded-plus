
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
//  Global data.
//  ------------------------------------------------------------------

#include <golded.h>
#include <gcprot.h>


//  ------------------------------------------------------------------
//  Various global data

glog LOG;

gareafile* AFILE = NULL;
CfgGed* CFG = NULL;
LangGed* LNG = NULL;
NodeIxl* NODE = NULL;
Qwk* QWK = NULL;
GoldedCfgEdit* EDIT = NULL;

uint locpost=0, netpost=0, echopost=0;

Subj information = "";

Area* AA = NULL;

char m_title[80];         // Menu title string
int m_titlepos;           // Menu title position
int m_titleattr;          // Menu title color

char* tconv;

int AreaKeys = 0;
int ReadKeys = 0;
int ListKeys = 0;
int NodeKeys = 0;
int EditKeys = 0;
int FileKeys = 0;
list<CmdKey>::iterator AreaKey, ReadKey, ListKey, NodeKey, EditKey, FileKey;

int inforow = 18;

char goldmark = ' ';

int startecho = -1;
Echo stecho;

int fieldupd = 2;  // State of field mode in winpdef()

int CurrArea = 0;
int OrigArea = 0;

Esc* MNETable = NULL;
Esc* I51Table = NULL;
Esc* CompTable = NULL;
Chs* CharTable = NULL;
EscTab* CompTP = NULL;
EscTab* I51TP = NULL;
EscTab* MNETP = NULL;
ChsTab* ChsTP = NULL;

bool quiet = true;
bool ignore = false;
int cfgerrors = 0;
bool veryverbose = false;
bool shellvid = true;
word* oldscreen = NULL;

bool disablesound = false;

int _in_editor = NO;
int _in_msglist = NO;
int savedirect = NO;

void(*SIGINT_OldHandler)(void) = NULL;

int arealistnumgrps = false;

int startupscan_success = false;


//  ------------------------------------------------------------------
//        0    1    2    3    4    5    6    7    8    9   10   11

Win gold_color1[16] = {
  {5, {   7,   0,   7,   7,   7,   7,   7,   7,   7,   7,   7,   0}, 2},  // Back  0
  {5, {  31,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7}, 1},  // Stat  1
  {0, {  15,  14,  14,   7,  12,   7,   7,   9,   7,   7,   7,  14}, 0},  // Brag  2
  {0, {   7,   9,  14,  31,  15,   7,   7,   7,   7,   7,   7,   9}, 0},  // Area  3
  {0, {   7,   7,  15,  14, 113,   8,  15,  79,  15,  15,   8,   7}, 5},  // Read  4
  {0, {   7,   9,  14,   7,  15,   7,  15, 113,   7,   7,   7,   9}, 0},  // Head  5
  {0, {   7,  12,  14,  31,  15,   8,   7,   7,   7,   7,   7,  12}, 0},  // Ask   6
  {0, {   7,  12,  14,  31,  15,   8,   7,   2,  12,  15,   7,  12}, 0},  // Menu  7
  {0, {   7,  14,   7,  79,  15,   7,   7,   7,   7,   7,   7,  14}, 0},  // Help  8
  {3, {   7,  12,  14,  31,  15,   8,   7,   7,   7,   7,   7,  12}, 0},  // Cfg   9
  {0, {  15,  12,  14,   7,   7,   7,   7,   7,   7,   7,   7,  12}, 0},  // Info  10
  {3, {   7,  12,  14,  31,  15,   8,   7,   7,   7,   7,   7,  12}, 0},  // Cfgb  11
  {0, {  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15}, 0},  // Style 12
  {0, {   7,   7,   7,   7,   7,   7,   7,   8,   7,   7,   7,   7}, 0},  // Shad  13
  {0, {   0,   0,   0,   0,   0,   0,   8,  15,   0,   0,   0,   0}, 0},  // Read2 14
};


//  ------------------------------------------------------------------
//        0    1    2    3    4    5    6    7    8    9

Win gold_color2[16] = {
  {5, { 240,  15,   7,   7,   7,   7,   7,   7,   7,   7,   7,  15}, 2},  // Back  0
  {5, { 143,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7}, 1},  // Stat  1
  {0, {  15,  14,  14,   7,  12,   7,   7,   9,   7,   7,   7,  14}, 0},  // Brag  2
  {0, { 240, 241, 244, 143, 244,   7,   7,   7,   7,   7,   7, 241}, 0},  // Area  3
  {0, { 241, 240, 242, 244,  64, 248, 240,  64, 240, 240, 248, 240}, 5},  // Read  4
  {0, { 135, 142, 142, 135, 143,   7, 143,  64, 135, 135,   7, 142}, 0},  // Head  5
  {0, { 224, 230, 232, 143, 228, 231,   7,   7,   7,   7,   7, 230}, 0},  // Ask   6
  {0, { 112, 126, 116, 143, 126, 120, 117, 113, 125, 121,   7, 126}, 0},  // Menu  7
  {0, { 135, 142,   7, 228, 143,   7,   7,   7,   7,   7,   7, 142}, 0},  // Help  8
  {3, { 224, 230, 232, 143, 228, 231,   7,   7,   7,   7,   7, 230}, 0},  // Cfg   9
  {0, {  15,  12,  14,   7,   7,   7,   7,   7,   7,   7,   7,  12}, 0},  // Info  10
  {3, { 240, 241, 244, 143, 244,   7,   7,   7,   7,   7,   7, 241}, 0},  // Cfgb  11
  {0, { 249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249}, 0},  // Style 12
  {0, {   7,   7,   7,   7,   7,   7,   7,   8,   7,   7,   7,   7}, 0},  // Shad  13
  {0, {   0,   0,   0,   0,   0,   0, 248, 241,   0,   0,   0,   0}, 0},  // Read2 14
};


//  ------------------------------------------------------------------
//        0    1    2    3    4    5    6    7    8    9

Win gold_mono1[16] = {
  {5, {  15,   0,   7,   7,   7,   7,   7,   7,   7,   7,   7,   0}, 2},  // Back  0
  {5, { 112,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7}, 1},  // Stat  1
  {0, {  15,   7,  15,   7,  15,   7,   7,   7,   7,   7,   7,   7}, 0},  // Brag  2
  {0, {   7,   7,  15, 112,  15,   7,   7,   7,   7,   7,   7,   7}, 0},  // Area  3
  {0, {   7,   7,  15,  15, 112,  15,   7,   7,   7,   7,  15,   7}, 5},  // Read  4
  {0, {   7,   7,  15,   7,  15,   7, 112,  15,   7,   7,   7,   7}, 0},  // Head  5
  {0, { 112, 112, 112,  15,  15,   7,   7,   7,   7,   7,   7, 112}, 0},  // Ask   6
  {0, {   7,   7, 112, 112,  15,   7,  15,  15,  15,  15,   7,   7}, 0},  // Menu  7
  {0, { 112, 112,   7,  15, 112,   7,   7,   7,   7,   7,   7, 112}, 0},  // Help  8
  {3, {   7,   7, 112, 112,  15,   7,   7,   7,   7,   7,   7,   7}, 0},  // Cfg   9
  {0, {  15,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7}, 0},  // Info  A
  {3, {   7,   7, 112, 112,  15,   7,   7,   7,   7,   7,   7,   7}, 0},  // Cfgb  B
  {0, {   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7}, 0},  // Style C
  {0, {   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7}, 0},  // Shad  F
  {0, {   0,   0,   0,   0,   0,   0,   7,  15,   0,   0,   0,   0}, 0},  // Read2 14
};


//  ------------------------------------------------------------------
//  Deallocate CFG at exit

static void CfgReset(void) {

  throw_delete(QWK);
  throw_delete(EDIT);
  throw_delete(AFILE);

  if(NODE)
    throw_xrelease(NODE->nodelist);
  throw_xrelease(NODE);
  throw_delete(CFG);
}


//  ------------------------------------------------------------------
//  Assign default CFG values

void CfgInit() {

  atexit(CfgReset);

  CFG = new CfgGed;
  throw_new(CFG);

  NODE = (NodeIxl*)throw_calloc(1, sizeof(NodeIxl));

  AFILE = new gareafile;
  throw_new(AFILE);

  EDIT = new GoldedCfgEdit;
  throw_new(EDIT);

  QWK = new Qwk;
  throw_new(QWK);

  LangInit();
}


//  ------------------------------------------------------------------
//  Assign default CFG values before reading the .CFG's

void CfgInit2() {

  //  ----------------------------------------------------------------
  //  Set some default robotnames

  CfgRobotname("AreaFix, AreaMgr, FileFix, AreaLink, SqaFix, AllFix, SqaFixHost, Raid, GEcho");


  //  ----------------------------------------------------------------
  //  Set some default filerequest extensions

  CfgFrqext(".ARC .ARJ .EXE .LHA .LZH .PAK .RUN .SDA .SDN .ZIP .ZOO .DOC .TXT .GIF .RAR .PNG .JPG .HA");


  //  ----------------------------------------------------------------
  //  Setup the standard beeping effects

  static char Attention[] = "Attention PLAY ToYou";
  CfgEvent(Attention);
  static char EditComment[] = "EditComment PLAY ToYou";
  CfgEvent(EditComment);
  static char EndOfMsgs[] = "EndOfMsgs PLAY TheEnd";
  CfgEvent(EndOfMsgs);
  static char JobDone[] = "JobDone PLAY GotIt";
  CfgEvent(JobDone);
  static char JobFailed[] = "JobFailed PLAY TooBad";
  CfgEvent(JobFailed);
  static char MsgIsLocal[] = "MsgIsLocal PLAY ToYou";
  CfgEvent(MsgIsLocal);
  static char MsgToYou[] = "MsgToYou PLAY ToYou";
  CfgEvent(MsgToYou);
  static char SayBiBi[] = "SayBiBi PLAY SayBiBi";
  CfgEvent(SayBiBi);
  static char SearchFailed[] = "SearchFailed PLAY TooBad";
  CfgEvent(SearchFailed);
  static char SearchSuccess[] = "SearchSuccess PLAY GotIt";
  CfgEvent(SearchSuccess);


  //  ----------------------------------------------------------------
}


//  ------------------------------------------------------------------

int IsMacro(gkey key, int type) {

  vector<Macro>::iterator m = CFG->macro.begin();
  while(m != CFG->macro.end()) {
    if((key == m->key) and (type == m->type))
      return true;
    m++;
  }

  return false;
}


//  ------------------------------------------------------------------

int PlayMacro(gkey key, int type) {

  vector<Macro>::iterator m = CFG->macro.begin();
  while(m != CFG->macro.end()) {
    if((key == m->key) and (type == m->type)) {
      RunMacro(m);
      return true;
    }
    m++;
  }

  return false;
}


//  ------------------------------------------------------------------

