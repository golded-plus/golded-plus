
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
//  The main GoldED configuration structure.
//  ------------------------------------------------------------------

#include <list>


//  ------------------------------------------------------------------

enum en_gswitches {
  internetlookup,
  kludgechrs,
  mouse,
  displocalhigh,
  arealistgroupid,
  askdelorig,
  lookuplocal,
  dispstatusline,
  screenusebios,
  highlightunread,
  akamatchlocal,
  fidonullfix,
  dispautonext,
  lookupuserbase,
  areakeeplast,
  filelistpagebar,
  printformfeed,
  arealistnos,
  disppagebar,
  internetreply,
  arealistpagebar,
  usemsgid,
  lookupnet,
  jamsmapihw,
  squishdirect,
  fidohwmarks,
  msglistpagebar,
  beepnoises,
  screenshadows,
  dispsoftcr,
  emptytearline,
  menudropmsg,
  beeplocalmsg,
  statuslineclock,
  twitto,
  keybdefaults,
  displistwrap,
  quotespacing,
  lookupecho,
  keybext,
  beepcomment,
  rcvdisablescfm,
  quoteblank,
  keybclear,
  dosprompt,
  areacatchupread,
  formfeedseparator,
  useflags,
  jamharddelete,
  disprealmsgno,
  frqwazoo,
  msglistviewsubj,
  akamatchecho,
  areafilegroups,
  areaautonext,
  timeoutsavemsg,
  nodelistwarn,
  akamatchnet,

  gswitches_last
};

class gswitches {

public:

  inline bool get(en_gswitches what) { return cfg[what]; }
  inline void set(en_gswitches what, bool value) { cfg[what] = value; }

  bool handle(word crc, const char* value);

  gswitches() { for(uint i=0; i<gswitches_last; i++) {cfg[i] = false; } };

private:

  bool cfg[gswitches_last];
};


//  ------------------------------------------------------------------

class CfgGed {

public:
  CfgGed();
  ~CfgGed();

  Path        goldcfg;
  Stamp       helpcfg;
  Path        helpged;
  Path        keyscfg;
  Path        langcfg;
  Path        xlatged;
  Path        goldlast;
  Path        golduser;

  Grp         grp;

  int         cfgeditquotemargin;

  std::list<CmdKey> cmdkey;
  std::vector<Macro> macro;

  int         addressbookadd;
  int         addresslookupfirst;
  std::vector<AddrMacro> addressmacro;
  Path        adeptxbbspath;
  int         adeptxbbsuserno;
  std::vector<gaka> aka;
  std::vector<AkaMatchG> akamatch;
  int         areaautoid;
  Echo        areacfmreplyto;
  bool        areacopydirect;
  Echo        areacopyto;
  bool        areacopyaddid;
  gstrarray   areaexcl;
  bool        areaforwarddirect;
  int         areafilegroups;       // areausegroups;
  bool        areafreqdirect;
  Echo        areafreqto;
  gstrarray   areaincl;
  gstrarray   areaisemail;
  gstrarray   areaisnews;
  int         arealistechomax;
  char        arealistformat[80];
  char        arealistgrouporder[256];
  char        arealistsort[20];     // areasort[10];
  int         arealisttype;
  Path        areapath;
  gstrarray   areapmscan;
  gstrarray   areapmscanexcl;
  gstrarray   areapmscanincl;
  int         areareadonly;
  std::vector<EchoRen> arearename;
  bool        areareplydirect;
  Echo        areareplyto;
  gstrarray   areascan;
  gstrarray   areascanexcl;
  gstrarray   areascanincl;
  char        areascansort[20];
  Echo        areastart;            // startecho;
  int         areatypeorder[17];
  Echo        areayouwroteto;
  Path        attachpath;
  Attr        attribsattach;
  Attr        attribscc;            // ccattrib;
  Attr        attribscfm;           // cfmattrib;
  Attr        attribsecho;          // echoattrib;
  Attr        attribsemail;
  Attr        attribsfrq;
  Attr        attribslocal;         // localattrib;
  Attr        attribsnet;           // netattrib;
  Attr        attribsnews;
  int         beepfactor;
  int         beepyourmail;
  int         carboncopylist;       // cclist;
  Win         color[16];
  Path        confirmfile;          // goldedcfm;
  int         confirmresponse;
  Path        cookiepath;
  int         crosspost;
  int         crosspostlist;        // xplist;
  int         ctrlinfoecho;
  int         ctrlinfoemail;
  int         ctrlinfolocal;
  int         ctrlinfonet;
  int         ctrlinfonews;
  int         dispareano;
  int         dispattachsize;
  PosLen      disphdrdateset;
  PosLen      disphdrnameset;
  PosLen      disphdrnodeset;
  int         displistcursor;
  int         dispmargin;           // rightmargin;
  int         dispmsgsize;
  uint        disptabsize;          // tabsize;
  bool        encodeemailheaders;
  std::vector<GEvent> event;
  int         externoptions;
  std::vector<ExtUtil> externutil;
  Ezycom      ezycom;
  int         ezycomuserno;
  Path        fidolastread;         // lastread;
  const char *fidomsgtype;
  Path        fidouserlist;
  int         fidouserno;           // lastreaduser;
  std::vector<FileAlias> filealias;
  bool        forcetemplate;
  gstrarray   frqext;
  std::vector<FrqNodeMap> frqnodemap;
  int         frqoptions;
  bool        gedhandshake;
  Path        goldbasepath;
  Path        goldbasesyspath;
  int         goldbaseuserno;
  Path        goldpath;
  bool        hidestylies;
  bool        highlighturls;
  Path        hudsonpath;
  long        hudsonsizewarn;
  Path        hudsonsyspath;
  int         hudsonuserno;
  bool        ignorecharset;
  char        importbegin[80];
  char        importend[80];
  Path        inboundpath;
  Path        inputfile;
  bool        intensecolors;
  IAdr        internetaddress;
  char        internetdomain[96];
  Node        internetgate;
  int         internetgateexp;
  bool        internetmsgid;
  int         internetrfcbody;
  bool        internetviagate;
  Invalidate  invalidate;
  Path        jampath;
  int         keybmode;
  char        keybstack[80];        // keybuf[80];
  gstrarray   kludge;
  Path        loadlanguage;
  Path        logfile;              // goldedlog;
  int         logformat;
  std::vector<MailList> mailinglist;
  std::vector< std::pair<std::string, std::string> > mappath;
  int         menumarked;
  int         msglistdate;
  bool        msglistfast;
  bool        msglistfirst;
  bool        msglistheader;
  bool        msglistwidesubj;
  Path        namesfile;
  Name        nickname;
  Name        netname;
  Path        nodepath;
  Path        nodepathfd;
  Path        nodepathv7;
  INam        organization;
  gstrarray   origin;
  int         originno;
  Path        outboundpath;
  Path        outputfile;
  Path        pathreportfile;
  Path        pcboardpath;
  int         pcboarduserno;
  int         peekurloptions;
  int         personalmail;
  GPlay       play;
  Path        printdevice;          // p_device;
  char        printinit[80];        // p_init[80];
  int         printlength;          // p_length;
  int         printmargin;          // p_margin;
  char        printreset[80];       // p_reset[80];
  Path        quotebuffile;
  int         quotebufmode;
  char        quotechars[11];
  uint        quotectrl;
  int         quotemargin;
  char        quotestring[10];
  char        quotestops[41];
  bool        quotewraphard;
  int         ra2usersbbs;          // RA2;
  int         replylink;
  int         replylinklist;
  gstrarray   robotname;
  int         screenblanker;        // blanktime;
  int         screenblankertype;
  int         screenmaxcol;         // maxcols;
  int         screenmaxrow;         // maxrows;
  int         screenpalette[17];
  int         screensize;
  INam        searchfor;
  Semaphore   semaphore;
  Path        seqdir;
  int         seqmsgid;
  ulong       seqoutrun;
  int         sharemode;            // share;
  bool        showdeleted;
  Path        soundpath;
  Echo        soupbadmsgs;
  Echo        soupemail;
  int         soupexportmargin;
  Path        soupexportpath;
  Path        soupimportpath;
  Path        soupnewsrcfile;
  Path        soupreplylinker;
  char        soupreplyto[60];
  Path        souptosslog;
  int         squishscan;
  int         squishuserno;
  Path        squishuserpath;
  int         statuslinehelp;
  bool        striphtml;
  char        stylecodepunct[41];
  char        stylecodestops[41];
  gstrarray   tagline;
  int         taglineno;
  char        taglinechar;
  bool        taglinesupport;
  char        tasktitle[60];
  Tear        tearline;
  bool        titlestatus;
  std::vector<Tpl> tpl;
  int         tplno;
  bool        templatematch;
  Path        templatepath;
  Path        temppath;
  int         timeout;
  int         twitmode;             // showtwits;
  std::vector<Node> twitname;
  gstrarray   twitsubj;
  std::vector< std::pair<std::string, std::string> > unpacker;
  ExtUtil     urlhandler;
  bool        usearea;
  bool        usecharset;
  int         usefwd;
  bool        useintl;
  bool        usepid;
  Path        userlistfile;         // goldedlst;
  std::vector<Node> username;
  int         usernameno;
  bool        usestylies;
  bool        usetzutc;
  Path        uudecodepath;
  bool        viewhidden;
  bool        viewkludge;
  bool        viewquote;
  Name        whoto;
  int         wildcatuserno;
  int         writeheader;
  std::string      wtpl;
  std::vector<Map> xlatcharset;
  std::vector<Map> xlatescset;
  char        xlatexport[17];       // exportcharset[17];
  char        xlatimport[17];       // localcharset[17];
  char        xlatlocalset[17];
  Path        xlatpath;
  int         zonegating;

  gswitches   switches;
};



//  ------------------------------------------------------------------

class GSaveUtil : public GStrBag2 {

public:

  void Add(int n, char* s)  { GStrBag2::Add((void*)&n, sizeof(n), s); }

  int Number()       { return *(int*)Current1(); }
  const char* Text()         { return Current2(); }

  int Number(int i)       { return *(int*)Index1(i); }
  const char* Text(int i)         { return Index2(i); }
};


//  ------------------------------------------------------------------

class GEditTrigger : public GStrBag2 {

public:

  const char* Trigger()  { return Current1(); }
  const char* Text()     { return Current2(); }

  const char* Trigger(int i)  { return Index1(i); }
  const char* Text(int i)     { return Index2(i); }
};


//  ------------------------------------------------------------------

class GoldedCfgEdit {

protected:

  // -----------------------------------------------------------------
  // Configuration data

  struct {

    // Boolean values
    uint autoattach  : 1;
    uint crlfterm    : 1;
    uint fieldclear  : 1;
    uint hardlines   : 1;
    uint hardterm    : 1;
    uint headerattrs : 1;
    uint internal    : 1;
    uint menu        : 1;
    uint mixcase     : 1;
    uint savemenu    : 1;

    // Multi values
    uint changedate;
    int  autosave;
    char charpara;
    char charspace;
    int  hdrnamepos;
    int  hdrnamelen;
    int  hdrnodepos;
    int  hdrnodelen;
    int  headerfirst;
    int  quotemargin;
    int  replyre;
    char softcrxlat;
    int  undelete;

    // String handles
    int external;
    int file;
    int hardline;
    int spellchecker;
  } cfg;
  
  // -----------------------------------------------------------------
  // String bag

  gstrarray str;

public:

  // -----------------------------------------------------------------
  // Configuration data

  GEditTrigger Comment;
  GEditTrigger Completion;
  GSaveUtil    SaveUtil;


  // -----------------------------------------------------------------
  // Constructor/destructor

  GoldedCfgEdit();

  // -----------------------------------------------------------------
  // Return values

  bool AutoAttach()              { return cfg.autoattach; }
  int AutoSave()                 { return cfg.autosave; }
  int ChangeDate()               { return cfg.changedate; }
  char CharPara()                { return cfg.charpara; }
  char CharSpace()               { return cfg.charspace; }
  bool CrLfTerm()                { return cfg.crlfterm; }
  const char* External()         { return str[cfg.external].c_str(); }
  bool FieldClear()              { return cfg.fieldclear; }
  const char* File()             { return str[cfg.file].c_str(); }
  const char* HardLine()         { return str[cfg.hardline].c_str(); }
  bool HardLines()               { return cfg.hardlines; }
  bool HardTerm()                { return cfg.hardterm; }
  int HdrNamePos()               { return cfg.hdrnamepos; }
  int HdrNameLen()               { return cfg.hdrnamelen; }
  int HdrNodePos()               { return cfg.hdrnodepos; }
  int HdrNodeLen()               { return cfg.hdrnodelen; }
  bool HeaderAttrs()             { return cfg.headerattrs; }
  int HeaderFirst()              { return cfg.headerfirst; }
  bool Internal()                { return cfg.internal; }
  bool Menu()                    { return cfg.menu; }
  bool MixCase()                 { return cfg.mixcase; }
  int QuoteMargin()              { return cfg.quotemargin; }
  int ReplyRe()                  { return cfg.replyre; }
  bool SaveMenu()                { return cfg.savemenu; }
  char SoftCrXlat()              { return cfg.softcrxlat; }
  const char* SpellChecker()     { return str[cfg.spellchecker].c_str(); }
  int UnDelete()                 { return cfg.undelete; }

  // -----------------------------------------------------------------
  // Set values

  void AutoAttach(bool s)        { cfg.autoattach = s; }
  void AutoSave(int s)           { cfg.autosave = s; }
  void ChangeDate(int s)         { cfg.changedate = s; }
  void CharPara(char s)          { cfg.charpara = s; }
  void CharSpace(char s)         { cfg.charspace = s; }
  void CrLfTerm(bool s)          { cfg.crlfterm = s; }
  void External(char* s)         { str[cfg.external] = s; }
  void FieldClear(bool s)        { cfg.fieldclear = s; }
  void File(char* s)             { str[cfg.file] = s; }
  void HardLine(char* s)         { str[cfg.hardline] = s; }
  void HardLines(bool s)         { cfg.hardlines = s; }
  void HardTerm(bool s)          { cfg.hardterm = s; }
  void HdrNamePos(int p)         { cfg.hdrnamepos = p; }
  void HdrNameLen(int l)         { cfg.hdrnamelen = l; }
  void HdrNodePos(int p)         { cfg.hdrnodepos = p; }
  void HdrNodeLen(int l)         { cfg.hdrnodelen = l; }
  void HeaderAttrs(bool s)       { cfg.headerattrs = s; }
  void HeaderFirst(int s)        { cfg.headerfirst = s; }
  void Internal(bool s)          { cfg.internal = s; }
  void Menu(bool s)              { cfg.menu = s; }
  void MixCase(bool s)           { cfg.mixcase = s; }
  void QuoteMargin(int s)        { cfg.quotemargin = s; }
  void ReplyRe(int s)            { cfg.replyre = s; }
  void SaveMenu(bool s)          { cfg.savemenu = s; }
  void SoftCrXlat(char s)        { cfg.softcrxlat = s; }
  void SpellChecker(char* s)     { str[cfg.spellchecker] = s; }
  void UnDelete(int s)           { cfg.undelete = s; }
};


//  ------------------------------------------------------------------

