
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
//  Collective header for GE*.CPP source files.
//  ------------------------------------------------------------------


#ifndef __GEHDRS_H
#define __GEHDRS_H


//  ------------------------------------------------------------------
//  GCAREA prototypes

void FixEchoid(char* echoid, int type);


//  ------------------------------------------------------------------
//  GCCFGG prototypes

void CfgAddress(char* value);
void CfgAddressmacro(char* value);
void CfgEvent(char* value);
void CfgFrqext(const char* value);
void CfgRobotname(const char* value);
void CfgUsername(char* value);

int SwitchCfgAtoG(word crc, char* val);
int SwitchCfgHtoZ(word crc, char* val);

int ReadGoldCfg(int force);
bool ReadGoldedCfg(int& force);
void WriteGoldGed();

void InstallDetect(char* path);
int InstallFinish();


//  ------------------------------------------------------------------
//  GCKEYS prototypes

int ReadKeysCfg(int force);
void KeyCmdAdd(gkey cmd, gkey val, int type);
void RunMacro(Macro* m);


//  ------------------------------------------------------------------
//  GCLANG prototypes

void LangInit();
bool ReadLangCfg(int force);
void LoadLanguage(const char* file);


//  ------------------------------------------------------------------
//  GCMISC prototypes

int ReadHelpCfg(int force);
int GetColor(char* ink);
int MakeColor(char* colors);
void GetColors(char* value);
void ReadXlatTables();


//  ------------------------------------------------------------------
//  GEAREA prototypes

int AreaPick(char* title, int wpos, int* idx);
void a_closeall();
void GetAreaconf();


//  ------------------------------------------------------------------
//  GECTRL prototypes

char* MakeOrigin(GMsg* msg, const char* _origin);
char* MakeTearline(GMsg* msg, char* buf);
void DoKludges(int mode, GMsg* msg, bool attronly = false);
void DoTearorig(int mode, GMsg* msg);
void GetRandom(int mode, GMsg* msg);
char* GetRandomLine(char* __buf, size_t __bufsize, const char* __file);
char* HandleRandomLine(char* buf, size_t bufsize);
const char* get_informative_string(void);


//  ------------------------------------------------------------------
//  GEDOIT prototypes

void CmfMsgs(GMsg* msg);
void LoadText(GMsg* msg, const char* textfile);
void SaveLines(int mode, const char* savefile, GMsg* msg, int margin, bool clip=false);


//  ------------------------------------------------------------------
//  GEDOSS prototypes

void Cleanup(void);
int ShellToDos(char* command, char* message, int cls, int cursor, int pause=NO);


//  ------------------------------------------------------------------
//  GEEDIT prototypes

int EditMsg(int mode, uint* position, GMsg* msg);
void FreePastebuf();


//  ------------------------------------------------------------------
//  GEFILE prototypes

void CreateFileAddr(GMsg* msg);
void CreateFileMsgs(int mode, GMsg* msg);
void FileSelect(GMsg* msg, char* title, FileSpec* fspec);


//  ------------------------------------------------------------------
//  GEFIND prototypes

bool FindString(GMsg* msg, const char* prompt, int what);
void FindAll(GMsg* msg, int& topline, int& keyok);
void FindHdr(GMsg* msg, int& topline, int& keyok);
bool SearchHighlight(const Line* line, int row, int width, int highlight_color);
void SearchExit();
void AdvancedSearch(GMsg* msg, int& topline, int& keyok);


//  ------------------------------------------------------------------
//  GEGLOB prototypes

void CfgInit();
void CfgInit2();
void CfgReset(void);
int PlayMacro(gkey key, int type);
int IsMacro(gkey key, int type);


//  ------------------------------------------------------------------
//  GEINIT prototypes

void Initialize(int argc, char* argv[]);


//  ------------------------------------------------------------------
//  GELINE prototypes

int   LoadCharset(const char* imp, const char* exp, int query = 0);
Line* AddKludge(Line* line, char* buf, int where=DIR_BELOW);
Line* AddLineFast(Line* line, char* text);
Line* DeleteLine(Line* line);
Line* FirstLine(Line* line);
Line* InsertLine(Line* newline, Line* oldline, int pos);
Line* LastLine(Line* line);
void  MakeLineIndex(GMsg* msg, int rmargin, bool header_recode = true);
void  MsgLineReIndex(GMsg* msg, int viewhidden=-1, int viewkludge=-1, int viewquote=-1);
char* XlatStr(char* dest, const char* src, int level, Chs* chrtbl, int qpencoded=false, bool i51=false);
char* mime_header_decode(char* decoded, const char* encoded, char* charset = NULL);
char* strxmimecpy(char* dest, const char* source, int level, int size, bool detect = false);
void InvalidateControlInfo(GMsg *msg);
    

//  ------------------------------------------------------------------
//  GEMENU prototypes

void DispHeadAttrs(GMsg* msg);
void AskAttributes(GMsg* msg);
void ChgAttrs(int mode, GMsg* msg);
int ChangeAka();
int ChangeCharsIn();
int ChangeCharsOut();
int ChangeTagline();
int ChangeOrigin();
int ChangeTemplate();
int ChangeUsername();
int ChangeXlatImport();


//  ------------------------------------------------------------------
//  GEMLST prototypes

uint MsgBrowser(GMsg* msg);
void MsgThreadlist();


//  ------------------------------------------------------------------
//  GEMRKS prototypes

void MarkMsgs(GMsg* msg);
int NextMarkedmsg(int direction, GMsg* msg);


//  ------------------------------------------------------------------
//  GEMSGS prototypes

char* TokenXlat(int mode, char* input, GMsg* msg, GMsg* oldmsg, int origarea);
void Rot13(GMsg* msg);
void ResetMsg(GMsg* msg);
int DoCarboncopy(GMsg* msg, GMsg** carbon);
void DoCrosspost(GMsg* msg, vector<int> &postareas);
char* ParseInternetAddr(char* __string, char* __name, char* __addr);


//  ------------------------------------------------------------------
//  GENODE prototypes

void Lookup(GMsg* msg, ftn_addr* addr, char* name, int topline, char* status);
bool Lookup2(Addr& addr, char* name, int topline, const char* status);

const char *lookup_nodelist(ftn_addr* addr);

void LookupNode(GMsg* msg, const char* name, int what);
void CheckNodelists();


//  ------------------------------------------------------------------
//  GEPLAY prototypes

void InitSound();
void FileToGPlay(GPlay* playbuf, char* filename);
int HandleGEvent(uint event);
int MakeNoise(int type);
void SayBibi();


//  ------------------------------------------------------------------
//  GEHDRE prototypes

typedef struct {
  char *buf;
  bool update;
} gsetaddr;

bool set_to_address(GMsg* msg, gsetaddr* toname, gsetaddr* toaddr, gsetaddr* fromaddr, gsetaddr* subj, int pos, char* lng_lookup, bool lookup=true);


//  ------------------------------------------------------------------
//  GEPOST prototypes

void MakeMsg(int mode, GMsg* oldmsg, bool ignore_replyto=false);
int EditHeaderinfo(int mode, GMsgHeaderView &view);
void CheckSubject(GMsg* msg, char* subj);


//  ------------------------------------------------------------------
//  GEQWKS prototypes

int ImportQWK();
int ExportQWK();


//  ------------------------------------------------------------------
//  GEREAD prototypes

char* GetCurrQuotebuf(char* quotebuf);
int MsgIsTwit(GMsg* msg, bool& istwitto, bool& istwitsubj);
ulong MsgHasReplies(GMsg* msg);
gkey ViewMessage(int istwit=NOT_TWIT);
int LoadMessage(GMsg* msg, int margin);
#ifdef OLD_STYLE_HEADER
void DispHeader(GMsg* msg, bool prn, FILE* fp=NULL, int width=-1);
#endif
void LoadRawMsg(GMsg* msg, int margin);

void make_pathreport(char* reportfile);
void NextArea();
void PrevArea();
void Reader();
void UpdateArea(GMsg* msg);
int ExternUtil(GMsg* __msg, int __utilno);
void ExternUtilMenu();
uint next_msg(int direction);

void ChangeAttributes();
void ChangeMsg();
void CommentMsg();
void ConfirmMsg();
void CopyMoveForward();
void DecMargin();
void DosShell();
void ExitAsk();
void FileRequest(GMsg* __msg);
void GotoBookMark();
void GotoFirstMsg();
void GotoLastMsg();
void GotoMsgno();
void GotoNextMsg();
void GotoPrevMsg();
void GotoNextUnread();
void GotoPrevUnread();
void GotoReplies();
void GotoReply1st();
void GotoReplyNext();
void GotoReplyPrev();
void IncMargin();
void MakeUserList();
void MakePathreport();
void MarkingOptions();
void MessageBrowse();
void NewArea(bool jumpnext = false);
void NewMsg();
void OtherAreaQuoteMsg(bool ignore_replyto=false);
void OtherAreaCommentMsg();
void QuitNow();
void QuoteBuf(GMsg* msg);
void QuoteMsg(bool ignore_replyto=false);
void RenumberArea();
void ReplyMsg();
void ToggleBookMark();
void ToggleHidden();
void ToggleHiddKlud();
void ToggleKludge();
void ToggleMark();
void ToggleMarkRead();
void TogglePageBar();
void ToggleQuote();
void ToggleRealMsgno();
void ToggleROT13();
void ToggleTwits();
void ToggleStyles();
void ToggleXlat();
void TouchNetscan(int __popup=true);
void UUDecode(GMsg* msg);
void WriteMsg(GMsg* msg);
void TouchSemaphore();


//  ------------------------------------------------------------------
//  GESCAN prototypes

void CheckSemaphores();


//  ------------------------------------------------------------------
//  GESOUP prototypes

char* CvtMSGIDtoMessageID(char* midbuf, char* msgid, ftn_addr* addr, char* domain, char* from, char* to, char* subject, struct tm* datetime);
char* CvtMessageIDtoMSGID(const char* mptr, char* msgidbuf, const char* echoid, char* kludge);

int ImportSOUP();
int ExportSOUP();


//  ------------------------------------------------------------------
//  GETPLS prototypes

void CookieIndex(char* textfile, char* indexfile);
int TemplateToText(int mode, GMsg* msg, GMsg* oldmsg, const char* tpl, int origarea);
bool is_user(const char* name);


//  ------------------------------------------------------------------
//  GEUTIL prototypes

void ScreenBlankIdle();
void IdleCheckSemaphores();
int   AkaMatch(ftn_addr* __match, const ftn_addr* __addr);
void  call_help();
void  CheckTick(gkey quitkey);
char* CvtName(char* inp);
int   edit_string(char* buf, int buf_size, char* title, int helpcat);
bool  edit_pathname(char* buf, int buf_size, char* title, int helpcat);
int   GetAkaNo(const ftn_addr& __aka);
int   GetQuotestr(const char* ptr, char* qbuf, uint* qlen);
int   is_quote(const char* ptr);
int   IsQuoteChar(const char* s);
void  maketitle();
void  ProgMeter(int mode, int xx, int yy, long len, long barlen, int attr, long pos, long size);
int   ReadCfg(const char* cfg, int ignoreunknown=false);
void  ScanMsgTxtForAddr(GMsg* msg);
gkey  SearchKey(gkey key, list<CmdKey>::iterator keys, int totkeys);
int   SearchTaglist(Echo* taglist, char* tag);
void  set_title(const char* t, int p, int a);
void  title_shadow();
void  update_statusline(const char* info);
void  update_statuslinef(const char* format, ...) __attribute__ ((format (printf, 1, 2)));
void  update_statuslines();
void  w_info(const char* info=NULL);
void  w_infof(const char* format, ...) __attribute__ ((format (printf, 1, 2)));
void  w_progress(int mode, int attr, long pos, long size, const char* title);
void  w_shadow();
void  ZonegateIt(ftn_addr& gate, ftn_addr& orig, ftn_addr& dest);
char* strtmp(const char* str);
int   quotecolor(const char* line);
void  doinvalidate(char* text, const char* find, const char* replace, bool is_tearline = false);


//  ------------------------------------------------------------------
//  GEUSRBSE prototypes

bool lookup_addressbook(GMsg* msg, char* name, char* aka, bool browser = false);
void build_pseudo(GMsg* msg, bool direction = true);
void update_addressbook(GMsg* msg, bool reverse = false, bool force = false);
void edit_addressbook(GMsg* msg);


//  ------------------------------------------------------------------
//  SOFTCR management
 
inline bool issoftcr(char c) {
  return (c == SOFTCR) and not CFG->switches.get(dispsoftcr);
}


inline char *spanspaces(const char *str) {
  if(CFG->switches.get(dispsoftcr))
    while(isspace(*str))
      str++;
  else
    while(isspace(*str) or (*str == SOFTCR))
      str++;
  return (char *)str;
}


inline char *spanfeeds(const char *str) {
  if(CFG->switches.get(dispsoftcr))
    while(*str == LF)
      str++;
  else
    while((*str == LF) or (*str == SOFTCR))
      str++;
  return (char *)str;
}


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------

