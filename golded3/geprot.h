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


#ifndef __GEPROT_H
#define __GEPROT_H

#include "gekeys.h"
//  ------------------------------------------------------------------
//  GCAREA prototypes
void FixEchoid(char * echoid, int type);

//  ------------------------------------------------------------------
//  GCCFGG prototypes
void CfgAddress(char * value);
void CfgAddressmacro(char * value);
void CfgEvent(char * value);
void CfgFrqext(const char * value);
void CfgRobotname(const char * value);
void CfgUsername(char * value);
int SwitchCfgAtoG(word crc, char * val);
int SwitchCfgHtoZ(word crc, char * val);
int ReadGoldCfg(int force);
bool ReadGoldedCfg();
void WriteGoldGed();
void InstallDetect(char * path);
int InstallFinish();
int getgroup(const char * key);

//  ------------------------------------------------------------------
//  GCKEYS prototypes
int ReadKeysCfg();
void KeyCmdAdd(gkey cmd, gkey val, int type);
void RunMacro(Macro * m);

//  ------------------------------------------------------------------
//  GCLANG prototypes
void LangInit();
bool ReadLangCfg();
void LoadLanguage(const char * file);

//  ------------------------------------------------------------------
//  GCMISC prototypes
int ReadHelpCfg(int force);
vattr GetColor(char * ink);
vattr MakeColor(char * colors);
void GetColors(char * value);
void ReadXlatTables();

//  ------------------------------------------------------------------
//  GEAREA prototypes
int AreaPick(char * title, int wpos, int * idx);
void a_closeall();
void GetAreaconf();

//  ------------------------------------------------------------------
//  GECTRL prototypes
char * MakeOrigin(GMsg * msg, const char * _origin);
char * MakeTearline(GMsg * msg, char * buf);
void DoKludges(int mode, GMsg * msg, int kludges = 0);
void DoTearorig(int mode, GMsg * msg);
void GetRandom(int mode, GMsg * msg);
char * GetRandomLine(char * __buf, size_t __bufsize, const char * __file);
char * HandleRandomLine(char * buf, size_t bufsize);
const char * get_informative_string(void);
char * mime_header_encode(char * dest, const char * source, GMsg * msg);

//  ------------------------------------------------------------------
//  GEDOIT prototypes
void CmfMsgs(GMsg * msg, bool torecycle);
void LoadText(GMsg * msg, const char * textfile);
void SaveLines(int mode, const char * savefile, GMsg * msg, int margin,
               bool clip = false);

//  ------------------------------------------------------------------
//  GEDOSS prototypes
void Cleanup(void);
int ShellToDos(const char * command,
               char * message,
               vattr cls,
               int cursor,
               int pause = NO);

//  ------------------------------------------------------------------
//  GEEDIT prototypes
int EditMsg(int mode, uint * position, GMsg * msg);
void FreePastebuf();

//  ------------------------------------------------------------------
//  GEFILE prototypes
void CreateFileAddr(GMsg * msg);
void CreateFileMsgs(int mode, GMsg * msg);
void FileSelect(GMsg * msg, char * title, FileSpec * fspec);

//  ------------------------------------------------------------------
//  GEFIND prototypes
bool FindString(GMsg * msg, const char * prompt, int what);
void FindAll(GMsg * msg, int & topline, int & keyok);
void FindHdr(GMsg * msg, int & topline, int & keyok);
bool SearchHighlight(const Line * line, int row, int width, vattr highlight_color);
void SearchExit();
void AdvancedSearch(GMsg * msg, int & topline, int & keyok);

//  ------------------------------------------------------------------
//  GEGLOB prototypes
void CfgInit();
void CfgInit2();
void CfgReset(void);
int PlayMacro(gkey key, int type);
int IsMacro(gkey key, int type);

//  ------------------------------------------------------------------
//  GEINIT prototypes
void Uninitialize();
void Initialize(int argc, char * argv[]);

//  ------------------------------------------------------------------
//  GELINE prototypes
char * Latin2ISO(char * iso_encoding, const char * latin_encoding);
char * ISO2Latin(char * latin_encoding, const char * iso_encoding);
char * MakeQuotedPrintable(const char * encoding);
char * ExtractPlainCharset(const char * encoding);
bool IsQuotedPrintable(const char * encoding);
int GetCurrentTable();

// Loads charset using index returned by GetCurrentTable.
// If index < 0 - reset current charset.
int LoadCharset(int index);
int LoadCharset(const char * imp, const char * exp);
Line * AddKludge(Line * line, const char * buf, int where = DIR_BELOW);
Line * AddLineFast(Line * line, const char * text);
Line * DeleteLine(Line * line);
Line * FirstLine(Line * line);
Line * InsertLine(Line * newline, Line * oldline, int pos);
Line * LastLine(Line * line);
void MakeLineIndex(GMsg * msg, int rmargin, bool getvalue, bool header_recode);
void MsgLineReIndex(GMsg * msg,
                    int viewhidden = -1,
                    int viewkludge = -1,
                    int viewquote  = -1);
void Latin2Local(char * str);
void Latin2Local(std::string & str);
std::string XlatStr(const char * src,
                    int level,
                    Chs * chrtbl,
                    int qpencoded = false,
                    bool i51      = false);
char * mime_header_decode(char * decoded,
                          const char * encoded,
                          char * charset = NULL);
char * strxmimecpy(char * dest,
                   const char * source,
                   int level,
                   int size,
                   bool detect = false);
void InvalidateControlInfo(GMsg * msg);

#ifdef HAS_ICONV
void IconvClear(void);

#endif
//  ------------------------------------------------------------------
//  GEMENU prototypes
void DispHeadAttrs(GMsg * msg);
void AskAttributes(GMsg * msg);
bool ProcessAttrs(gkey & key);
void ChgAttrs(int mode, GMsg * msg);
int ChangeAka();
int ChangeCharsIn();
int ChangeCharsOut();
int ChangeTagline();
int ChangeOrigin();
int ChangeTemplate();
int ChangeUsername();
int ChangeXlatImport();

//  ------------------------------------------------------------------
//  GEMSGS prototypes
void TokenXlat(int mode, std::string & input, GMsg * msg, GMsg * oldmsg, int origarea);
void TokenXlat(int mode,
               char *& input,
               size_t size,
               bool resize,
               GMsg * msg,
               GMsg * oldmsg,
               int origarea);

inline void TokenXlat(int mode,
                      char * input,
                      size_t size,
                      GMsg * msg,
                      GMsg * oldmsg,
                      int origarea)
{
    TokenXlat(mode, input, size, false, msg, oldmsg, origarea);
}

void Rot13(GMsg * msg);
int DoCarboncopy(GMsg & msg, std::vector<GMsg> & carbon);
void DoCrosspost(GMsg * msg, std::vector<int> & postareas);
char * ParseInternetAddr(char * __string,
                         char * __name,
                         char * __addr,
                         bool detect_charset = true);

//  ------------------------------------------------------------------
//  GEMLST prototypes
void MsgThreadlist();

//  ------------------------------------------------------------------
class ThreadEntry
{
public: uint32_t msgno;
    uint32_t replyto;
    uint32_t reply1st;
    uint32_t replynext;
    uint32_t replytoindex;
    uint32_t level;
    std::string entrytext;
};

//  ------------------------------------------------------------------
class GThreadlist : public gwinpick
{
private: gwindow window;
    GMsg msg;
    std::vector<ThreadEntry> treeEntryList;
    dword m_OldMsgno;
    uint m_OldTags;
    std::string m_OldEchoId;
    void BuildThreadIndex(dword msgno);
    void recursive_build(uint32_t msgn, uint32_t rn, uint32_t level, uint32_t index);
    void GenTree(int idx);
    void update_title();
    bool NextThread(bool next);

public: void open();                    // Called after window is opened
    void close();                       // Called after window is closed
    void print_line(uint idx, uint pos, bool isbar);
    void do_delayed();
    bool handle_key();                  // Handles keypress
    void Run();
    bool GoNextUnread(bool reader);

    GThreadlist() : msg()
    {
        m_OldMsgno     = dword(-1);
        m_OldTags      = uint(-1);
        replylinkfloat = CFG->replylinkfloat;
    }

    ~GThreadlist()
    {
        msg.Reset();
    }
};
//  ------------------------------------------------------------------
//  GEMRKS prototypes
void MarkMsgs(GMsg * msg);
int NextMarkedmsg(int direction, GMsg * msg);

//  ------------------------------------------------------------------
//  GENODE prototypes
void Lookup(GMsg * msg, ftn_addr * addr, char * name, int topline, char * status);
bool Lookup2(Addr & addr, char * name, int topline, const char * status);
const char * lookup_nodelist(ftn_addr * addr);
void LookupNodeLocation(GMsg * msg, std::string & location, int what);
void LookupNode(GMsg * msg, const char * name, int what);
void LookupNodeClear();
void CheckNodelists();

//  ------------------------------------------------------------------
//  GEPLAY prototypes
void InitSound();
void FileToGPlay(GPlay * playbuf, char * filename);
int HandleGEvent(uint event);
int MakeNoise(int type);
void SayBibi();

//  ------------------------------------------------------------------
//  GEHDRE prototypes
typedef struct
{
    char * buf;
    bool   update;
} gsetaddr;
bool set_to_address(GMsg * msg,
                    gsetaddr * toname,
                    gsetaddr * toaddr,
                    gsetaddr * fromaddr,
                    gsetaddr * subj,
                    int pos,
                    char * lng_lookup,
                    bool lookup = true);

//  ------------------------------------------------------------------
//  GEPOST prototypes
void MakeMsg(int mode, GMsg * oldmsg, bool ignore_replyto          = false);
int EditHeaderinfo(int mode, GMsgHeaderView & view, bool doedithdr = true);
void CheckSubject(GMsg * msg, char * subj);

//  ------------------------------------------------------------------
//  GEQWKS prototypes
int ImportQWK();
int ExportQWK();

//  ------------------------------------------------------------------
//  GEREAD prototypes
char * GetCurrQuotebuf(char * quotebuf);
int MsgIsTwit(GMsg * msg, bool & istwitto, bool & istwitsubj);
uint32_t MsgHasReplies(GMsg * msg);
gkey ViewMessage(int istwit = NOT_TWIT);
int LoadMessage(GMsg * msg, int margin);
void LoadRawMsg(GMsg * msg, int margin);
void make_pathreport(char * reportfile);
void NextArea();
void PrevArea();
void Reader();
void UpdateArea(GMsg * msg);
int ExternUtil(GMsg * msg, uint32_t utilno);
void ExternUtilMenu(GMsg * msg);
void ReadPeekURLs(GMsg * msg);
uint next_msg(int direction);
void ChangeAttributes();
void ChangeMsg();
void CommentMsg();
void ConfirmMsg();
void CopyMoveForward(bool torecycle = false);
void DecMargin();
void DosShell();
void ExitAsk();
void FileRequest(GMsg * __msg);
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
void GotoThNextUnread();
void IncMargin();
void MakeUserList();
void MakePathreport();
void MarkingOptions();
void MessageBrowse();
void NewArea(bool jumpnext = false);
void NewMsg();
void OtherAreaQuoteMsg(bool ignore_replyto = false);
void OtherAreaCommentMsg();
void QuitNow();
void QuoteBuf(GMsg * msg);
void QuoteMsg(bool ignore_replyto = false);
void RenumberArea();
void ReplyMsg();
void ToggleBookMark();
void ToggleHidden();
void ToggleHiddKlud();
void ToggleKludge();
void ToggleStripHTML();
void ToggleMark();
void ToggleMarkRead();
void TogglePageBar();
void ToggleQuote();
void ToggleRealMsgno();
void ToggleROT13();
void ToggleTwits();
void ToggleStyles();
void ToggleXlat();
void TouchNetscan(int __popup = true);
void UUDecode(GMsg * msg);
void WriteMsg(GMsg * msg);
void TouchSemaphore();

//  ------------------------------------------------------------------
//  GESCAN prototypes
void CheckSemaphores();

//  ------------------------------------------------------------------
//  GESOUP prototypes
char * CvtMSGIDtoMessageID(char * midbuf,
                           char * msgid,
                           ftn_addr * addr,
                           char * domain,
                           char * from,
                           char * to,
                           char * subject,
                           struct tm * datetime);
char * CvtMessageIDtoMSGID(const char * mptr,
                           char * msgidbuf,
                           const char * echoid,
                           char * kludge);
int ImportSOUP();
int ExportSOUP();

//  ------------------------------------------------------------------
//  GETPLS prototypes
void CookieIndex(char * textfile, char * indexfile);
int TemplateToText(int mode, GMsg * msg, GMsg * oldmsg, const char * tpl, int origarea);
bool is_user(const char * name);

//  ------------------------------------------------------------------
//  GEUTIL prototypes
void ScreenBlankIdle();
void IdleCheckSemaphores();
int AkaMatch(ftn_addr * __match, const ftn_addr * __addr);
void call_help();
void CheckTick(gkey quitkey);
char * CvtName(char * inp);
int edit_string(char * buf, int buf_size, char * title, int helpcat);
bool edit_pathname(char * buf, int buf_size, char * title, int helpcat);
int GetAkaNo(const ftn_addr & __aka);
int GetQuotestr(const char * ptr, char * qbuf, uint * qlen);
int cmp_quotes(char * q1, char * q2);
int is_quote(const char * ptr);
bool is_quote2(Line * line, const char * ptr);
int IsQuoteChar(const char * s);
void maketitle();
int maketitle_and_status(char *);
void ProgMeter(int mode,
               int xx,
               int yy,
               long len,
               long barlen,
               int attr,
               long pos,
               long size);
int ReadCfg(const char * cfg, int ignoreunknown = false);
void ScanMsgTxtForAddr(GMsg * msg);
gkey SearchKey(gkey key, std::list<CmdKey>::iterator keys, int totkeys);
int SearchTaglist(Echo * taglist, char * tag);
void set_title(const char * t, int p, vattr a);
void title_shadow();
void update_statusline(const char * info);
void update_statuslinef(const char * format, const char * token,
                        ...) __attribute__ ((format(printf, 1, 3)));
void update_statuslines();
void w_info(const char * info = NULL);
void w_infof(const char * format, ...) __attribute__ ((format(printf, 1, 2)));
void w_progress(int mode, vattr attr, long pos, long size, const char * title);
void w_shadow();
void ZonegateIt(ftn_addr & gate, ftn_addr & orig, ftn_addr & dest);
char * strtmp(const char * str);
vattr quotecolor(const char * line);
bool doinvalidate(std::string & text,
                  const std::string & find,
                  const std::string & replace,
                  bool is_tearline = false);
bool find(const std::vector<const char *> & vec, const char * str);
bool find(const std::vector<std::string> & vec, const std::string & str);
vattr GetColorName(const char * name, Addr & addr, vattr color);

//  ------------------------------------------------------------------
//  GEUSRBSE prototypes
bool lookup_addressbook(GMsg * msg, char * name, char * aka, bool browser = false);
void build_pseudo(GMsg * msg, bool direction     = true);
void update_addressbook(GMsg * msg, bool reverse = false, bool force = false);
void edit_addressbook(GMsg * msg);

//  ------------------------------------------------------------------
bool isuucp(const char * name);

//  ------------------------------------------------------------------
const char * url_begin(const char * ptr);

//  ------------------------------------------------------------------
void RemoveHTML(char *& txt);

//  ------------------------------------------------------------------

#endif // ifndef __GEPROT_H
//  ------------------------------------------------------------------
