
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
//  Area structures and classes.
//  ------------------------------------------------------------------

#ifndef __gmoarea_h
#define __gmoarea_h


//  ------------------------------------------------------------------

#include <vector>
#include <gmoarea.h>


//  ------------------------------------------------------------------
//  Internal GoldED message header

class GMsg : public gmsg {

public:

  IAdr        iorig;            // Internet "From:" header address
  IAdr        idest;            // Internet "To:" header address
  IAdr        ireplyto;         // Internet "Reply-To:" header address
  IAdr        iaddr;            // Internet destination address           (from ^aREPLYADDR)
  IAdr        igate;            // Internet gate FTN-address and gatename (from ^aREPLYTO)
  INam        ifrom;            // Internet "From:" header
  char        ito[512];         // Internet "To:" header
  char        icc[512];         // Internet "Cc:"  header
  char        ibcc[512];        // Internet "Bcc:"  header
  INam        organization;     // Internet "Organization:" header

  INam        realby;           // Real from-name
  INam        realto;           // Real to-name
  INam        pseudoto;         // Pseudo to-name
  INam        pseudofrom;       // Pseudo from-name
  uint        you_and_I;        // Mail was to/from me/you

  INam        fwdfrom;          // FWDFROM kludge
  ftn_addr    fwdorig;          // FWDORIG kludge
  INam        fwdto;            // FWDTO kludge
  ftn_addr    fwddest;          // FWDDEST kludge
  Subj        fwdsubj;          // FWDSUBJ kludge
  Echo        fwdarea;          // FWDAREA kludge
  char        fwdmsgid[201];    // FWDMSGID kludge

  bool        i51;              // true if I51 kludge was found
  char        charset[100];     // Charset type
  int         charsetlevel;     // Charset level
  int         charsetencoding;  // Charset encoding format (GCHENC_*)

  int         tzutc;  		// TZUTC kludge

  char        tagline[80];      // Tagline for msg
  char        tearline[80];     // Tearline for msg
  char        origin[160];      // Origin for msg

  Line*       lin;              // Index of the lines
  Line**      line;             // Index of the viewable lines
  int         lines;            // Total number of lines

  int         quotepct;         // Quote/text percentage

  uint        foundwhere;       // Where searched string was found (GFIND_*)
  int         foundtopline;     // First line where a searched string was found

  uint        orig_timesread;

  char*       messageid;        // Internet Message-ID (allocated)
  char*       inreplyto;        // Internet In-Reply-To (allocated)
  char*       references;       // Internet References (allocated)

  const char* areakludgeid;     // Echoid from AREA: kludge or NULL

  char* By() { return *realby ? realby : by; }
  char* To() { return *realto ? realto : to; }

  bool to_me()  { return (you_and_I & TO_ME); }
  bool to_all() { return (you_and_I & TO_ALL); }
  bool by_me()  { return (you_and_I & BY_ME); }
  bool to_you() { return (you_and_I & TO_YOU); }
  bool by_you() { return (you_and_I & BY_YOU); }

  void TextToLines(int __line_width, bool header_recode = true);
  void LinesToText();
};


//  ------------------------------------------------------------------

class GAreaListScan {

private:

  vector< pair<string, string> > container;
  vector< pair<string, string> >::iterator currposn;

public:

  inline GAreaListScan() { currposn = container.end(); }
  inline ~GAreaListScan() {}

  inline void Add(pair<string, string> p) { container.push_back(p); currposn = container.end(); }

  inline void Reset() { container.clear(); currposn = container.end(); }

  inline bool First() { currposn = container.begin(); return currposn != container.end(); }
  inline bool Next()  { currposn++; return currposn != container.end(); }

  inline void CurrNo(int posn) { First(); while(posn--) if(not Next()) break; }

  inline const char* MenuText() { return currposn != container.end() ? currposn->first.c_str() : ""; }
  inline const char* File()     { return currposn != container.end() ? currposn->second.c_str() : ""; }

  inline const char* MenuText(int i) { return i < container.size() ? container[i].first.c_str() : ""; }
  inline const char* File(int i)     { return i < container.size() ? container[i].second.c_str() : ""; }
};


//  ------------------------------------------------------------------

const word CUR_GOLDLAST_VER = 0x1A02;

#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif

struct ggoldlast {
  dword crcechoid;
  dword lastread;
  dword msgncount;
  dword unread;
  word marks;

  byte flags;
};

#if defined(GOLD_CANPACK)
#pragma pack()
#endif


//  ------------------------------------------------------------------
//  Arealist class

class Area;

typedef vector<Area *>::iterator area_iterator;

class AreaList {

private:

  Desc        alistselections[16];
  byte        mask;

  friend class Area;
  friend class SelMaskPick;

public:

  // Index of areas in the list
  vector<Area *> idx;

  // Area pointer
  area_iterator item;

  // Sort specs (for external compare func)
  char sortspec[20];

  // Active msgbases (bitmap of MT_* contants)
  uint msgbases;

  // Additional items to the area scan menu
  GAreaListScan ListScan;

  void Set_Mask(word value = 1) { mask = value; }
  void Select_Mask(void);

  // Con/destructor
  AreaList();
  ~AreaList();

  // Subscript operator, returns pointer to area
  inline Area* operator[](size_t n) { return (n < idx.size()) ? idx[n] : (Area*)NULL; }
  inline size_t size() { return idx.size(); }

  // Allocate and initialize data
  void Init();

  // Deallocate and reset data
  void Reset();

  // Return pointer to a new'd area of the specified format
  Area* NewArea(int msgbase);

  // Sort areas
  void Sort(const char* specs=NULL, int first=0, int last=-1);

  // Default marks names
  void SetDefaultMarks();

  // Read/write GOLDLAST.LST
  void ReadGoldLast();
  void WriteGoldLast();

  // Miscellaneous
  void  AddNewArea(AreaCfg* aa);
  int   AreaIdToNo(int __areaid);
  Area* AreaIdToPtr(int __areaid);
  int   AreaNoToId(int __areano);
  Area* AreaNoToPtr(int __areano);
  int   AreaScan(int mode, uint currno, int pmscan, int& pmails, int& pmareas, const char* file=NULL);
  int   AreaEchoToNo(const char* echoid);
  Area* AreaEchoToPtr(const char* echoid);
  void  GetAreafile(char* val);
  void  GetArea(char* val);
  bool  GetAreaFirstPart(AreaCfg& aa, char*& key, char*& val);
  void  GetAreaDef(char* val);
  void  GetAreaSep(char* val);
  void  GetAreaDesc(char* val);
  void  ReadEcholist(char* val);
  int   SetActiveAreaId(int __areaid);
  int   SetActiveAreaNo(int __areano);
  void  SetAreaDesc(char* echoid, char* desc);
  void  SortAreaGroup(const char* options, int beginarea, int endarea);
  void  WriteAreaDef(const char* file);
};


//  ------------------------------------------------------------------
//  Msgbase function prototypes

int AreaCmp(const Area** __a, const Area** __b);


//  ------------------------------------------------------------------
//  Area data (collected from global/Random System)

struct AreaData {

  // Area
  gaka     aka;
  bool     akamatching;
  Attr     attributes;
  bool     areacopyaddid;
  bool     areacopydirect;
  Echo     areacopyto;
  bool     areaforwarddirect;
  bool     areafreqdirect;
  Echo     areafreqto;
  bool     areareplydirect;
  Echo     areareplyto;
  Echo     areayouwroteto;
  int      ctrlinfo;
  bool     edithardterm;
  bool     editmixcase;
  bool     forcetemplate;
  IAdr     internetaddress;
  Node     internetgate;
  bool     internetmsgid;
  bool     internetrfcbody;
  Path     loadlanguage;
  int      msglistdate;
  bool     msglistfast;
  bool     msglistfirst;
  bool     msglistheader;
  bool     msglistwidesubj;
  Name     nickname;
  Name     netname;
  INam     organization;
  char     origin[160];
  Path     quotebuffile;
  char     quotechars[11];
  int      quotectrl;
  char     quotestring[10];
  char     quotestops[41];
  bool     quotewraphard;
  GPlay    play;
  int      replyre;
  char     tagline[76];
  char     taglinechar;
  bool     taglinesupport;
  Tear     tearline;
  Path     tpl;
  bool     templatematch;
  int      usefwd;
  Node     username;
  bool     usetzutc;
  IAdr     whoto;
  Path     wtpl;
  XlatName xlatexport;
  XlatName xlatimport;

  // Reader
  Path   inputfile;
  Path   outputfile;
  INam   searchfor;
  int    twitmode;
  bool   viewhidden;
  bool   viewkludge;
  bool   viewquote;
};

//  ------------------------------------------------------------------

extern AreaList AL;


//  ------------------------------------------------------------------
//  Area information structures

class Area {

private:

  word        marks;            // storing 16 different marks

  friend class AreaList;

public:

  //  ----------------------------------------------------------------
  //  Config data

  gmo_area* area;
  AreaData* adat;


  //  ----------------------------------------------------------------
  //  Constructor and destructor

  Area(gmo_area* a);
  virtual ~Area();


  //  ----------------------------------------------------------------
  //  Data members

  GTag    Msgn;               // Message numbers
  GTag    Mark;               // Message marks
  GTag    PMrk;               // Personal mail marks
  GTag    Expo;               // Messages to be exported

  ulong   bookmark;           // Current bookmark message number

  uint    unread;             // Number of unread messages at last scan

  uint    lastread() const;       // Number of last message read
  ulong   lastreadentry() const;  // Lastread message number at entry to area

  void    set_lastread(uint lr);

  bool    isopen() { return area->isopen; }

  bool    isunreadchg : 1;    // TRUE if unread was changed since last scan
  bool    isreadmark  : 1;    // TRUE if in read marked mode
  bool    isreadpm    : 1;    // TRUE if in read personal mail mode
  bool    isvalidchg  : 1;    // TRUE if isunreadchg is valid
  bool    isscanned   : 1;    // TRUE if scanned
  bool    ispmscanned : 1;    // TRUE if pmscanned
  bool    istossed    : 1;    // TRUE if msgs were tossed to this area
  
  
  //  ----------------------------------------------------------------
  //  Access config data

        int   areaid() const      { return area->areaid(); }
        int   groupid() const     { return area->groupid(); }
        uint  type() const        { return area->type(); }
        uint  msgbase() const     { return area->msgbase(); }
        uint  board() const       { return area->board(); }
  const ftn_addr& aka() const     { return area->aka(); }
        int   originno() const    { return area->originno(); }
        Attr& attr()              { return area->attr(); }

  bool scan()           { return area->ascan(); }
  bool scanexcl()       { return area->ascanexcl(); }
  bool scanincl()       { return area->ascanincl(); }
  bool pmscan()         { return area->pmscan(); }
  bool pmscanexcl()     { return area->pmscanexcl(); }
  bool pmscanincl()     { return area->pmscanincl(); }
  bool ismarked() const { return (marks & (1<<AL.mask)) ? true : false; }

  const char* echoid() const      { return area->echoid(); }
  const char* desc() const        { return area->desc(); }
  const char* path() const        { return area->path(); }

  void set_marked(bool value)   { if(value) marks |= (word)(1<<AL.mask); else marks &= (word)(~(1<<AL.mask)); }

  void set_areaid(int a)        { area->set_areaid(a); }
  void set_groupid(int g)       { area->set_groupid(g); }
  void set_type(uint t)         { area->set_type(t); }
  void set_msgbase(uint m)      { area->set_msgbase(m); }
  void set_board(uint b)        { area->set_board(b); }
  void set_aka(ftn_addr& a)     { area->set_aka(a); }
  void set_originno(int o)      { area->set_originno(o); }
  void set_attr(Attr& a)        { area->set_attr(a); }
  void set_origin(char* o)      { area->set_origin(o); }

  void set_scan(bool s)           { area->set_scan(s); }
  void set_scanexcl(bool s)       { area->set_scanexcl(s); }
  void set_scanincl(bool s)       { area->set_scanincl(s); }
  void set_pmscan(bool s)         { area->set_pmscan(s); }
  void set_pmscanexcl(bool s)     { area->set_pmscanexcl(s); }
  void set_pmscanincl(bool s)     { area->set_pmscanincl(s); }

  void set_echoid(const char* s)   { area->set_echoid(s); }
  void set_desc(const char* s)     { area->set_desc(s); }
  void set_path(const char* s)     { area->set_path(s); }


  //  ----------------------------------------------------------------
  //  Determine msgbase format

  int isfts1() const;
  int isopus() const;
  int isezycom() const;
  int isfido() const;
  int isgoldbase() const;
  int ishudson() const;
  int isjam() const;
  int ispcboard() const;
  int issquish() const;
  int issmb() const;
  int iswildcat() const;
  int isadeptxbbs() const;
  int isseparator() const;


  //  ----------------------------------------------------------------
  //  Determine area type

  int isnet() const;
  int isecho() const;
  int islocal() const;
  int isemail() const;
  int isnewsgroup() const;
  int isinternet() const;
  int isqwk() const;
  int issoup() const;


  //  ----------------------------------------------------------------
  //  Miscellaneous

  void SetBookmark(uint __relno);
  void DeleteMsg(GMsg* msg, int direction);
  void DelMsg();
  void DelMsgs(GMsg* msg);
  void SigExportlistUpdate();
  void UpdateAreadata();

  void SetHighwaterMark();
  void ResetHighwaterMark();


  //  ----------------------------------------------------------------
  //  High-level messagebase member functions

  void Open();
  void Close();

  void Suspend();
  void Resume();

  void Lock();
  void Unlock();

  void Scan();
  void ScanArea();
  void ScanAreaPM();

  int LoadHdr(GMsg* msg, ulong msgno, bool enable_recode = true);
  int LoadMsg(GMsg* msg, ulong msgno, int margin, int mode=0);

  void SaveHdr(int mode, GMsg* msg);
  void SaveMsg(int mode, GMsg* msg);

  void DelMsg(GMsg* msg);

  void NewMsgno(GMsg* msg);
  char* UserLookup(char* lookfor);
  int Renumber();

  Line* MakeDumpMsg(GMsg* msg, char* lng_head);

  void UpdateTimesread(GMsg* msg);


  //  ----------------------------------------------------------------
  //  Random System access functions

  void InitData();
  void RandomizeData(int mode=0);

  const ftn_aka& Aka() const            { return adat->aka; }
        bool   Akamatching() const      { return adat->akamatching; }
  const Attr&  Attributes() const       { return adat->attributes; }
        bool   Areacopyaddid() const    { return adat->areacopyaddid; }
        bool   Areacopydirect() const   { return adat->areacopydirect; }
  const char*  Areacopyto() const       { return adat->areacopyto; }
        bool   Areaforwarddirect() const{ return adat->areaforwarddirect; }
        bool   Areafreqdirect() const   { return adat->areafreqdirect; }
  const char*  Areafreqto() const       { return adat->areafreqto; }
        bool   Areareplydirect() const  { return adat->areareplydirect; }
  const char*  Areareplyto() const      { return adat->areareplyto; }
  const char*  Areayouwroteto() const   { return adat->areayouwroteto; }
        int    Ctrlinfo() const         { return adat->ctrlinfo; }
        bool   Edithardterm() const     { return adat->edithardterm; }
        bool   Editmixcase() const      { return adat->editmixcase; }
        bool   Forcetemplate() const    { return adat->forcetemplate; }
  const char*  Inputfile() const        { return adat->inputfile; }
  const char*  Internetaddress() const  { return adat->internetaddress; }
  const Node&  Internetgate() const     { return adat->internetgate; }
        bool   Internetmsgid() const    { return adat->internetmsgid; }
        bool   Internetrfcbody() const  { return adat->internetrfcbody; }
  const char*  Loadlanguage() const     { return adat->loadlanguage; }
        int    Msglistdate() const      { return adat->msglistdate; }
        bool   Msglistfast() const      { return adat->msglistfast; }
        bool   Msglistfirst() const     { return adat->msglistfirst; }
        bool   Msglistheader() const    { return adat->msglistheader; }
        bool   Msglistwidesubj() const  { return adat->msglistwidesubj; }
  const char*  Nickname() const         { return adat->nickname; }
  const char*  Netname() const          { return adat->netname; }
  const char*  Organization() const     { return adat->organization; }
  const char*  Origin() const           { return adat->origin; }
  const char*  Outputfile() const       { return adat->outputfile; }
  const char*  Quotebuffile() const     { return adat->quotebuffile; }
  const char*  Quotechars() const       { return adat->quotechars; }
        int    Quotectrl() const        { return adat->quotectrl; }
  const char*  Quotestring() const      { return adat->quotestring; }
  const char*  Quotestops() const       { return adat->quotestops; }
        bool   Quotewraphard() const    { return adat->quotewraphard; }
  const GPlay& Play() const             { return adat->play; }
  const int    Replyre() const          { return adat->replyre; }
  const char*  Searchfor() const        { return adat->searchfor; }
  const char*  Tagline() const          { return adat->tagline; }
        char   Taglinechar() const      { return adat->taglinechar; }
        bool   Taglinesupport() const   { return adat->taglinesupport; }
  const char*  Tearline() const         { return adat->tearline; }
  const char*  Tpl() const              { return adat->tpl; }
        bool   Templatematch() const    { return adat->templatematch; }
        int    Twitmode() const         { return adat->twitmode; }
        int    Usefwd() const           { return adat->usefwd; }
  const Node&  Username() const         { return adat->username; }
        bool   Usetzutc() const         { return adat->usetzutc; }
        bool   Viewhidden() const       { return adat->viewhidden; }
        bool   Viewkludge() const       { return adat->viewkludge; }
        bool   Viewquote() const        { return adat->viewquote; }
  const char*  Whoto() const            { return adat->whoto; }
  const char*  WTpl() const             { return adat->wtpl; }
  const char*  Xlatexport() const       { return adat->xlatexport; }
  const char*  Xlatimport() const       { return adat->xlatimport; }

        int    NextMsglistdate()              { adat->msglistdate++; if(adat->msglistdate > MSGLISTDATE_RECEIVED) adat->msglistdate = MSGLISTDATE_NONE; return adat->msglistdate; }
  const ftn_aka& SetAka(const ftn_addr& a)    { adat->aka.addr = a; return adat->aka; }
  const char*  SetInputfile(const char* i)    { return strcpy(adat->inputfile, i); }
        int    SetMsglistdate(int m)          { adat->msglistdate = m; return adat->msglistdate; }
  const char*  SetOrigin(const char* o)       { return strxcpy(adat->origin, o, sizeof(adat->origin)); }
  const char*  SetOutputfile(const char* o)   { return strxcpy(adat->outputfile, o, sizeof(adat->outputfile)); }
  const char*  SetSearchfor(const char* s)    { return strxcpy(adat->searchfor, s, sizeof(adat->searchfor)); }
  const char*  SetTagline(const char* t)      { return strxcpy(adat->tagline, t, sizeof(adat->tagline)); }
  const char*  SetTpl(const char* t)          { return strxcpy(adat->tpl, t, sizeof(adat->tpl)); }
        int    SetTwitmode(int m)             { adat->twitmode = m; return adat->twitmode; }
  const Node&  SetUsername(const Node& n)     { adat->username = n; return adat->username; }
  const char*  SetXlatexport(const char* x)   { return strcpy(adat->xlatexport, x); }
  const char*  SetXlatimport(const char* x)   { return strcpy(adat->xlatimport, x); }
        int    ToggleMsglistwidesubj()        { adat->msglistwidesubj = not adat->msglistwidesubj; return adat->msglistwidesubj; }
        int    ToggleViewhidden()             { adat->viewhidden = not adat->viewhidden; return adat->viewhidden; }
        int    ToggleViewkludge()             { adat->viewkludge = not adat->viewkludge; return adat->viewkludge; }
        int    ToggleViewquote()              { adat->viewquote = not adat->viewquote; return adat->viewquote; }

};


//  ------------------------------------------------------------------
//  Inline implementations

inline int Area::isfts1() const      { return area->isfts1(); }
inline int Area::isopus() const      { return area->isopus(); }
inline int Area::isezycom() const    { return area->isezycom(); }
inline int Area::isfido() const      { return area->isfido(); }
inline int Area::isgoldbase() const  { return area->isgoldbase(); }
inline int Area::ishudson() const    { return area->ishudson(); }
inline int Area::isjam() const       { return area->isjam(); }
inline int Area::ispcboard() const   { return area->ispcboard(); }
inline int Area::issquish() const    { return area->issquish(); }
inline int Area::issmb() const       { return area->issmb(); }
inline int Area::iswildcat() const   { return area->iswildcat(); }
inline int Area::isadeptxbbs() const { return area->isadeptxbbs(); }
inline int Area::isseparator() const { return area->isseparator(); }

inline int Area::isnet() const       { return area->isnet(); }
inline int Area::isecho() const      { return area->isecho(); }
inline int Area::islocal() const     { return area->islocal(); }
inline int Area::isemail() const     { return area->isemail(); }
inline int Area::isnewsgroup() const { return area->isnewsgroup(); }
inline int Area::isinternet() const  { return area->isinternet(); }
inline int Area::isqwk() const       { return area->isqwk(); }
inline int Area::issoup() const      { return area->issoup(); }

inline uint  Area::lastread() const { return area->lastread; }
inline ulong Area::lastreadentry() const { return area->lastreadentry; }
inline void  Area::set_lastread(uint lr) { area->lastread = lr; }

inline void Area::SetBookmark(uint __relno)  { bookmark = Msgn.CvtReln(__relno); /*Book.Add(bookmark);*/ }

inline void Area::Suspend() { area->suspend(); }
inline void Area::Resume() { area->resume(); }

inline void Area::Lock() { area->lock(); }
inline void Area::Unlock() { area->unlock(); }

inline void Area::DelMsg(GMsg* msg) { area->del_msg(msg); }

inline void Area::NewMsgno(GMsg* msg) { area->new_msgno(msg); }
inline char* Area::UserLookup(char* lookfor) { return area->user_lookup(lookfor); }
inline int Area::Renumber() { return area->renumber(); }

inline Line* Area::MakeDumpMsg(GMsg* msg, char* lng_head) { return area->make_dump_msg(msg->lin, msg, lng_head); }

inline void Area::SetHighwaterMark() { area->set_highwater_mark(); }
inline void Area::ResetHighwaterMark() { area->reset_highwater_mark(); }

inline void Area::UpdateTimesread(GMsg* msg) { area->update_timesread(msg); }


//  ------------------------------------------------------------------

class MsgbaseManager {

public:

  void Init();
  void Exit();

  void WideScanBegin();
  void WideScanEnd();


};


//  ------------------------------------------------------------------
//  Arealist picker class

class GPickArealist : public gwinpick {

private:

  int  areawin;
  int  tempwin;
  int  tempwin1;
  int  areawin1;
  int  areawin2;
  int  tempwin2;
  uint area_fuzidx;

  int  pmails;
  int  pmareas;
  bool pmscan;

  void jump_to();
  void jumpmatch();
  void center();
  void AreasRenumber();
  void AreaCatchUp(uint n);
  void AreaDropMsgMarks(uint n);
  void dispbuf(char* buf, int areano);

protected:

public:

  uint area_maxfuz;
  bool esc_abort;

  void open();                        // Called after window is opened
  void close();                       // Called after window is closed
  void precursor();                   // Called before any cursor movement
  void do_delayed();                  // Called after a delay
  void print_line(uint idx, uint pos, bool isbar);
  bool handle_key();                  // Handles keypress
  bool is_selectable(uint idx);       // is not separator ?

  void close_all();
  void update() { display_page(); display_bar(); }

  int  Run(const char* _title, int wpos, int& idx);

  GPickArealist();
};


//  ------------------------------------------------------------------

#endif // __gmoarea_h
