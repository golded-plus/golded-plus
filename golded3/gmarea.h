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

#ifndef __GMAREA_H
#define __GMAREA_H
//  ------------------------------------------------------------------

#include <vector>
#include <gecfgg.h>
#include <gmoarea.h>
//  ------------------------------------------------------------------
//  Internal GoldED message header
class GMsg : public gmsg
{
public: IAdr iorig;               // Internet "From:" header address
    IAdr idest;                   // Internet "To:" header address
    IAdr ireplyto;                // Internet "Reply-To:" header address
    IAdr iaddr;                   // Internet destination address           (from ^aREPLYADDR)
    IAdr igate;                   // Internet gate FTN-address and gatename (from ^aREPLYTO)
    INam ifrom;                   // Internet "From:" header
    char ito[512];                // Internet "To:" header
    char icc[512];                // Internet "Cc:"  header
    char ibcc[512];               // Internet "Bcc:"  header
    INam organization;            // Internet "Organization:" header
    INam realby;                  // Real from-name
    INam realto;                  // Real to-name
    INam pseudoto;                // Pseudo to-name
    INam pseudofrom;              // Pseudo from-name
    uint you_and_I;               // Mail was to/from me/you
    INam fwdfrom;                 // FWDFROM kludge
    ftn_addr fwdorig;             // FWDORIG kludge
    INam fwdto;                   // FWDTO kludge
    ftn_addr fwddest;             // FWDDEST kludge
    Subj fwdsubj;                 // FWDSUBJ kludge
    Echo fwdarea;                 // FWDAREA kludge
    char fwdmsgid[201];           // FWDMSGID kludge
    bool i51;                     // true if I51 kludge was found
    char charset[100];            // Charset type
    int charsetlevel;             // Charset level
    int charsetencoding;          // Charset encoding format (GCHENC_*)
    int tzutc;                    // TZUTC kludge
    char tagline[80];             // Tagline for msg
    char tearline[80];            // Tearline for msg
    char origin[160];             // Origin for msg
    Line * lin;                   // Index of the lines
    Line ** line;                 // Index of the viewable lines
    int lines;                    // Total number of lines
    int quotepct;                 // Quote/text percentage
    uint foundwhere;              // Where searched string was found (GFIND_*)
    int foundtopline;             // First line where a searched string was found
    uint orig_timesread;
    char * messageid;             // Internet Message-ID (allocated)
    char * inreplyto;             // Internet In-Reply-To (allocated)
    char * references;            // Internet References (allocated)
    const char * areakludgeid;    // Echoid from AREA: kludge or NULL
    GMsg();
    char * By();
    char * To();
    bool to_me();
    bool to_all();
    bool by_me();
    bool to_you();
    bool by_you();
    void TextToLines(int __line_width, bool getvalue = true, bool header_recode = true);
    void LinesToText();
    void Reset();
};
//  ------------------------------------------------------------------
class GAreaListScan
{
private: std::vector<std::pair<std::string, std::string> > container;
    std::vector<std::pair<std::string, std::string> >::iterator currposn;
public: inline GAreaListScan()
    {
        currposn = container.end();
    }

    inline ~GAreaListScan()
    {}
inline void Add(std::pair<std::string, std::string> p)
{
    container.push_back(p);
    currposn = container.end();
}

inline void Reset()
{
    container.clear();
    currposn = container.end();
}

inline bool First()
{
    currposn = container.begin();
    return currposn != container.end();
}

inline bool Next()
{
    currposn++;
    return currposn != container.end();
}

inline void CurrNo(int posn)
{
    First();

    while(posn--)
    {
        if(not Next())
        {
            break;
        }
    }
}

inline const char * MenuText()
{
    return currposn != container.end() ? currposn->first.c_str() : "";
}

inline const char * File()
{
    return currposn != container.end() ? currposn->second.c_str() : "";
}

inline const char * MenuText(unsigned i)
{
    return i < container.size() ? container[i].first.c_str() : "";
}

inline const char * File(unsigned i)
{
    return i < container.size() ? container[i].second.c_str() : "";
}
};

//  ------------------------------------------------------------------
const word CUR_GOLDLAST_VER = 0x1A02;

#if defined (GOLD_CANPACK)
    #pragma pack(1)
#endif

struct ggoldlast
{
    dword crcechoid;
    dword lastread;
    dword msgncount;
    dword unread;
    word  marks;
    byte  flags;
};

#if defined (GOLD_CANPACK)
    #pragma pack()
#endif
//  ------------------------------------------------------------------
//  Arealist class
class Area;
typedef std::vector<Area *>::iterator area_iterator;
class AreaList
{
private: Desc alistselections[16];
    byte mask;
    friend class Area;
    friend class SelMaskPick;
public:
    // Index of areas in the list
    std::vector<Area *> idx;
    // Area pointer
    area_iterator item;
    // Sort specs (for external compare func)
    char sortspec[AREALISTSORTSIZE];
    // Active msgbases (bitmap of MT_* contants)
    std::vector<std::string> basetypes;
    // Additional items to the area scan menu
    GAreaListScan ListScan;
//  void Set_Mask(word value = 1) { mask = value; }
    void Select_Mask(void);

    // Con/destructor
    AreaList();
    ~AreaList();
    // Subscript operator, returns pointer to area
inline Area * operator [](size_t n)
{
    return (n < idx.size()) ? idx[n] : (Area *)NULL;
}

inline size_t size()
{
    return idx.size();
}

    // Allocate and initialize data
    void Init();

    // Deallocate and reset data
    void Reset();

    // Return pointer to a new'd area of the specified format
    Area * NewArea(const char * basetype);
    Area * NewArea(const std::string & basetype);

    // Sort areas
    void Sort(const char * specs = NULL, int first = 0, int last = -1);

    // Default marks names
    void SetDefaultMarks();

    // Read/write GOLDLAST.LST
    void ReadGoldLast();
    void WriteGoldLast();

    // Miscellaneous
    void AddNewArea(AreaCfg * aa);
    int AreaIdToNo(int __areaid);
    Area * AreaIdToPtr(int __areaid);
    int AreaNoToId(int __areano);
    Area * AreaNoToPtr(int __areano);
    int AreaScan(int mode,
                 uint currno,
                 int pmscan,
                 int & pmails,
                 int & pmareas,
                 const char * file = NULL);
    int AreaEchoToNo(const char * echoid);
    Area * AreaEchoToPtr(const char * echoid);
    void GetAreafile(char * val);
    void GetArea(char * val);
    bool GetAreaFirstPart(AreaCfg & aa, char *& key, char *& val);
    void GetAreaDef(char * val);
    void GetAreaSep(char * val);
    void GetAreaDesc(char * val);
    void ReadEcholist(char * val);
    int SetActiveAreaId(int __areaid);
    int SetActiveAreaNo(int __areano);
    void SetAreaDesc(char * echoid, char * desc);
    void SortAreaGroup(const char * options, int beginarea, int endarea);
    void WriteAreaDef(const char * file);
};

//  ------------------------------------------------------------------
//  Msgbase function prototypes
int AreaCmp(const Area ** __a, const Area ** __b);

//  ------------------------------------------------------------------
//  Area data (collected from global/Random System)
struct AreaData
{
    AreaData(){
        akamatching       = false;
        areacopyaddid     = false;
        areacopydirect    = false;
        areaforwarddirect = false;
        areafreqdirect    = false;
        areareplydirect   = false;
        dispsoftcr        = false;
        edithardterm      = false;
        editmixcase       = false;
        forcetemplate     = false;
        hidestylies       = false;
        usestylies        = false;
        internetmsgid     = false;
        msglistfast       = false;
        msglistfirst      = false;
        msglistheader     = false;
        msglistwidesubj   = false;
        quotewraphard     = false;
        taglinesupport    = false;
        templatematch     = false;
        usearea           = false;
        usesoftcrxlat     = false;
        usetzutc          = false;
        inittwit          = false;
        viewhidden        = false;
        viewkludge        = false;
        viewquote         = false;
        striphtml         = false;
        ctrlinfo          = 0;
        internetrfcbody   = 0;
        msglistdate       = 0;
        quotectrl         = 0;
        replyre           = 0;
        usefwd            = 0;
        twitmode          = 0;
        writeheader       = 0;
        taglinechar       = 0;
        memset(origin, 0, sizeof(origin));
        memset(quotechars, 0, sizeof(quotechars));
        memset(quotestring, 0, sizeof(quotestring));
        memset(quotestops, 0, sizeof(quotestops));
#if defined (GCFG_SPELL_INCLUDED)
        memset(scheckerdeflang, 0, sizeof(scheckerdeflang));
#endif
        memset(tagline, 0, sizeof(tagline));
        memset(areacopyto, 0, sizeof(areacopyto));
        memset(areafreqto, 0, sizeof(areafreqto));
        memset(areareplyto, 0, sizeof(areareplyto));
        memset(areayouwroteto, 0, sizeof(areayouwroteto));
        memset(loadlanguage, 0, sizeof(loadlanguage));
        memset(quotebuffile, 0, sizeof(quotebuffile));
        memset(tpl, 0, sizeof(tpl));
        memset(wtpl, 0, sizeof(wtpl));
        memset(inputfile, 0, sizeof(inputfile));
        memset(outputfile, 0, sizeof(outputfile));
        memset(internetaddress, 0, sizeof(internetaddress));
        memset(whoto, 0, sizeof(whoto));
        memset(nickname, 0, sizeof(nickname));
        memset(netname, 0, sizeof(netname));
        memset(organization, 0, sizeof(organization));
        memset(searchfor, 0, sizeof(searchfor));
        memset(tearline, 0, sizeof(tearline));
        memset(xlatexport, 0, sizeof(xlatexport));
        memset(xlatimport, 0, sizeof(xlatimport));
        //classes
        memset(&aka, 0, sizeof(aka));
        memset(&attributes, 0, sizeof(attributes));
        memset(&internetgate, 0, sizeof(internetgate));
        memset(&play, 0, sizeof(play));
        memset(&username, 0, sizeof(username));
    }
    // Area
    gaka  aka;
    bool  akamatching;
    Attr  attributes;
    bool  areacopyaddid;
    bool  areacopydirect;
    Echo  areacopyto;
    bool  areaforwarddirect;
    bool  areafreqdirect;
    Echo  areafreqto;
    bool  areareplydirect;
    Echo  areareplyto;
    Echo  areayouwroteto;
    int   ctrlinfo;
    bool  dispsoftcr;
    bool  edithardterm;
    bool  editmixcase;
    bool  forcetemplate;
    bool  hidestylies;
    bool  usestylies;
    IAdr  internetaddress;
    Node  internetgate;
    bool  internetmsgid;
    int   internetrfcbody;
    Path  loadlanguage;
    int   msglistdate;
    bool  msglistfast;
    bool  msglistfirst;
    bool  msglistheader;
    bool  msglistwidesubj;
    Name  nickname;
    Name  netname;
    INam  organization;
    char  origin[160];
    Path  quotebuffile;
    char  quotechars[11];
    int   quotectrl;
    char  quotestring[10];
    char  quotestops[41];
    bool  quotewraphard;
    GPlay play;
    int   replyre;
#if defined (GCFG_SPELL_INCLUDED)
    char scheckerdeflang[10240];
#endif
    char     tagline[76];
    char     taglinechar;
    bool     taglinesupport;
    Tear     tearline;
    Path     tpl;
    bool     templatematch;
    bool     usearea;
    int      usefwd;
    Node     username;
    bool     usesoftcrxlat;
    bool     usetzutc;
    IAdr     whoto;
    Path     wtpl;
    XlatName xlatexport;
    XlatName xlatimport;
    // Reader
    Path              inputfile;
    Path              outputfile;
    INam              searchfor;
    int               twitmode;
    bool              inittwit;
    std::vector<Node> twitname;
    gstrarray         twitsubj;
    bool              viewhidden;
    bool              viewkludge;
    bool              viewquote;
    bool              striphtml;
    int               writeheader;
};

//  ------------------------------------------------------------------
extern AreaList AL;
//  ------------------------------------------------------------------
//  Area information structures
class Area
{
private: bool findfirst;
    uint findtype;  //1 - FindAll, 2 - FindHdr
    word marks;     // storing 16 different marks
    friend class AreaList;
public:
    //  ----------------------------------------------------------------
    //  Config data
    gmo_area * area;
    AreaData * adat;
    //  ----------------------------------------------------------------
    //  Constructor and destructor
    Area(gmo_area * a);
    virtual ~Area();
    //  ----------------------------------------------------------------
    //  Data members
    GTag Msgn;                  // Message numbers
    GTag Mark;                  // Message marks
    GTag PMrk;                  // Personal mail marks
    GTag Expo;                  // Messages to be exported
    uint32_t bookmark;          // Current bookmark message number
    uint unread;                // Number of unread messages at last scan
    uint lastread() const;      // Number of last message read
    uint32_t lastreadentry() const;   // Lastread message number at entry to area
    void set_lastread(uint lr);

void set_findfirst(bool ff)
{
    findfirst = ff;
}

void set_findtype(uint ft)
{
    findtype = ft;
}

bool get_findfirst()
{
    return findfirst;
}

uint get_findtype()
{
    return findtype;
}

bool isopen()
{
    return make_bool(area->isopen);
}

    bool isunreadchg : 1;       // TRUE if unread was changed since last scan
    bool isreadmark  : 1;       // TRUE if in read marked mode
    bool isreadpm    : 1;       // TRUE if in read personal mail mode
    bool isvalidchg  : 1;       // TRUE if isunreadchg is valid
    bool isscanned   : 1;       // TRUE if scanned
    bool ispmscanned : 1;       // TRUE if pmscanned
    bool istossed    : 1;       // TRUE if msgs were tossed to this area
    //  ----------------------------------------------------------------
    //  Access config data
int areaid() const
{
    return area->areaid();
}

int groupid() const
{
    return area->groupid();
}

uint type() const
{
    return area->type();
}

const std::string & basetype() const
{
    return area->basetype();
}

uint board() const
{
    return area->board();
}

const ftn_addr & aka() const
{
    return area->aka();
}

int originno() const
{
    return area->originno();
}

Attr & attr()
{
    return area->attr();
}

bool scan()
{
    return area->ascan();
}

bool scanexcl()
{
    return area->ascanexcl();
}

bool scanincl()
{
    return area->ascanincl();
}

bool pmscan()
{
    return area->pmscan();
}

bool pmscanexcl()
{
    return area->pmscanexcl();
}

bool pmscanincl()
{
    return area->pmscanincl();
}

bool ismarked() const
{
    return make_bool(marks & (1 << AL.mask));
}

bool isnewmail() const
{
    return isvalidchg and isunreadchg;
}

const char * echoid() const
{
    return area->echoid();
}

const char * desc() const
{
    return area->desc();
}

const char * path() const
{
    return area->path();
}

void set_marked(bool value)
{
    if(value)
    {
        marks |= (word)(1 << AL.mask);
    }
    else
    {
        marks &= (word)(~(1 << AL.mask));
    }
}

void set_areaid(int a)
{
    area->set_areaid(a);
}

void set_groupid(int g)
{
    area->set_groupid(g);
}

void set_type(uint t)
{
    area->set_type(t);
}

void set_basetype(const char * m)
{
    area->set_basetype(m);
}

void set_basetype(const std::string & m)
{
    area->set_basetype(m);
}

void set_board(uint b)
{
    area->set_board(b);
}

void set_aka(ftn_addr & a)
{
    area->set_aka(a);
}

void set_originno(int o)
{
    area->set_originno(o);
}

void set_attr(Attr & a)
{
    area->set_attr(a);
}

void set_origin(char * o)
{
    area->set_origin(o);
}

void set_scan(bool s)
{
    area->set_scan(s);
}

void set_scanexcl(bool s)
{
    area->set_scanexcl(s);
}

void set_scanincl(bool s)
{
    area->set_scanincl(s);
}

void set_pmscan(bool s)
{
    area->set_pmscan(s);
}

void set_pmscanexcl(bool s)
{
    area->set_pmscanexcl(s);
}

void set_pmscanincl(bool s)
{
    area->set_pmscanincl(s);
}

void set_echoid(const char * s)
{
    area->set_echoid(s);
}

void set_desc(const char * s)
{
    area->set_desc(s);
}

void set_path(const char * s)
{
    area->set_path(s);
}

    //  ----------------------------------------------------------------
    //  Determine msgbase format
    bool isseparator() const;

    //  ----------------------------------------------------------------
    //  Determine area features
    bool issoftdelete() const;
    bool havearrivedstamp() const;
    bool havereceivedstamp() const;
    bool requirehardterm() const;
    bool requiresoftterm() const;

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
    void DeleteMsg(GMsg * msg, int direction);
    void DelMsg(bool force = false);
    void DelMsgs(GMsg * msg, bool force = false);
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
    int LoadHdr(GMsg * msg, uint32_t msgno, bool enable_recode   = true);
    int LoadMsg(GMsg * msg, uint32_t msgno, int margin, int mode = 0);
    void SaveHdr(int mode, GMsg * msg);
    void SaveMsg(int mode, GMsg * msg);
    void DelMsg(GMsg * msg);
    void NewMsgno(GMsg * msg);
    char * UserLookup(char * lookfor);
    int Renumber();
    Line * MakeDumpMsg(GMsg * msg, char * lng_head);
    void UpdateTimesread(GMsg * msg);

    //  ----------------------------------------------------------------
    //  Random System access functions
    void InitData();
    void RandomizeData(int mode = 0);

const ftn_aka & Aka() const
{
    return adat->aka;
}

bool Akamatching() const
{
    return adat->akamatching;
}

const Attr & Attributes() const
{
    return adat->attributes;
}

bool Areacopyaddid() const
{
    return adat->areacopyaddid;
}

bool Areacopydirect() const
{
    return adat->areacopydirect;
}

const char * Areacopyto() const
{
    return adat->areacopyto;
}

bool Areaforwarddirect() const
{
    return adat->areaforwarddirect;
}

bool Areafreqdirect() const
{
    return adat->areafreqdirect;
}

const char * Areafreqto() const
{
    return adat->areafreqto;
}

bool Areareplydirect() const
{
    return adat->areareplydirect;
}

const char * Areareplyto() const
{
    return adat->areareplyto;
}

const char * Areayouwroteto() const
{
    return adat->areayouwroteto;
}

int Ctrlinfo() const
{
    return adat->ctrlinfo;
}

bool Edithardterm() const
{
    return adat->edithardterm;
}

bool Editmixcase() const
{
    return adat->editmixcase;
}

bool Forcetemplate() const
{
    return adat->forcetemplate;
}

const char * Inputfile() const
{
    return adat->inputfile;
}

const char * Internetaddress() const
{
    return adat->internetaddress;
}

const Node & Internetgate() const
{
    return adat->internetgate;
}

bool Internetmsgid() const
{
    return adat->internetmsgid;
}

int Internetrfcbody() const
{
    return adat->internetrfcbody;
}

const char * Loadlanguage() const
{
    return adat->loadlanguage;
}

int Msglistdate() const
{
    return adat->msglistdate;
}

bool Msglistfast() const
{
    return adat->msglistfast;
}

bool Msglistfirst() const
{
    return adat->msglistfirst;
}

bool Msglistheader() const
{
    return adat->msglistheader;
}

bool Msglistwidesubj() const
{
    return adat->msglistwidesubj;
}

const char * Nickname() const
{
    return adat->nickname;
}

const char * Netname() const
{
    return adat->netname;
}

const char * Organization() const
{
    return adat->organization;
}

const char * Origin() const
{
    return adat->origin;
}

const char * Outputfile() const
{
    return adat->outputfile;
}

const char * Quotebuffile() const
{
    return adat->quotebuffile;
}

const char * Quotechars() const
{
    return adat->quotechars;
}

int Quotectrl() const
{
    return adat->quotectrl;
}

const char * Quotestring() const
{
    return adat->quotestring;
}

const char * Quotestops() const
{
    return adat->quotestops;
}

bool Quotewraphard() const
{
    return adat->quotewraphard;
}

const GPlay & Play() const
{
    return adat->play;
}

int Replyre() const
{
    return adat->replyre;
}

bool StripHTML() const
{
    return adat->striphtml;
}

const char * Searchfor() const
{
    return adat->searchfor;
}

const char * Tagline() const
{
    return adat->tagline;
}

char Taglinechar() const
{
    return adat->taglinechar;
}

bool Taglinesupport() const
{
    return adat->taglinesupport;
}

const char * Tearline() const
{
    return adat->tearline;
}

const char * Tpl() const
{
    return adat->tpl;
}

bool Templatematch() const
{
    return adat->templatematch;
}

int Twitmode() const
{
    return adat->twitmode;
}

bool Usearea() const
{
    return adat->usearea;
}

int Usefwd() const
{
    return adat->usefwd;
}

const Node & Username() const
{
    return adat->username;
}

bool Usetzutc() const
{
    return adat->usetzutc;
}

bool Viewhidden() const
{
    return adat->viewhidden;
}

bool Viewkludge() const
{
    return adat->viewkludge;
}

bool Viewquote() const
{
    return adat->viewquote;
}

const char * Whoto() const
{
    return adat->whoto;
}

int Writeheader() const
{
    return adat->writeheader;
}

const char * WTpl() const
{
    return adat->wtpl;
}

const char * Xlatexport() const
{
    return adat->xlatexport;
}

const char * Xlatimport() const
{
    return adat->xlatimport;
}

int NextMsglistdate()
{
    adat->msglistdate++;

    if(adat->msglistdate > MSGLISTDATE_RECEIVED)
    {
        adat->msglistdate = MSGLISTDATE_NONE;
    }

    return adat->msglistdate;
}

const ftn_aka & SetAka(const ftn_addr & a)
{
    adat->aka.addr = a;
    return adat->aka;
}

const TCHAR * SetInputfile(const TCHAR * i)
{
    return strxcpy(adat->inputfile, i, ARRAYSIZE(adat->inputfile));
}

int SetMsglistdate(int m)
{
    adat->msglistdate = m;
    return adat->msglistdate;
}

const char * SetOrigin(const char * o)
{
    return strxcpy(adat->origin, o, sizeof(adat->origin));
}

const char * SetOutputfile(const char * o)
{
    return strxcpy(adat->outputfile, o, sizeof(adat->outputfile));
}

const char * SetSearchfor(const char * s)
{
    return strxcpy(adat->searchfor, s, sizeof(adat->searchfor));
}

const char * SetTagline(const char * t)
{
    return strxcpy(adat->tagline, t, sizeof(adat->tagline));
}

const char * SetTpl(const char * t)
{
    return strxcpy(adat->tpl, t, sizeof(adat->tpl));
}

int SetTwitmode(int m)
{
    adat->twitmode = m;
    return adat->twitmode;
}

const Node & SetUsername(const Node & n)
{
    adat->username = n;
    return adat->username;
}

const TCHAR * SetXlatexport(const TCHAR * x)
{
    return strxcpy(adat->xlatexport, x, ARRAYSIZE(adat->xlatexport));
}

const TCHAR * SetXlatimport(const TCHAR * x)
{
    return strxcpy(adat->xlatimport, x, ARRAYSIZE(adat->xlatimport));
}

int ToggleMsglistwidesubj()
{
    adat->msglistwidesubj = not adat->msglistwidesubj;
    return adat->msglistwidesubj;
}

int ToggleViewhidden()
{
    adat->viewhidden = not adat->viewhidden;
    return adat->viewhidden;
}

int ToggleViewkludge()
{
    adat->viewkludge = not adat->viewkludge;
    return adat->viewkludge;
}

int ToggleViewquote()
{
    adat->viewquote = not adat->viewquote;
    return adat->viewquote;
}

bool ToggleStripHTML()
{
    adat->striphtml = not adat->striphtml;
    return adat->striphtml;
}
};

//  ------------------------------------------------------------------
//  Inline implementations
inline bool Area::isseparator() const
{
    return area->isseparator();
}

inline bool Area::issoftdelete() const
{
    return area->issoftdelete();
}

inline bool Area::havearrivedstamp() const
{
    return area->havearrivedstamp();
}

inline bool Area::havereceivedstamp() const
{
    return area->havereceivedstamp();
}

inline bool Area::requirehardterm() const
{
    return area->requirehardterm();
}

inline bool Area::requiresoftterm() const
{
    return area->requiresoftterm();
}

inline int Area::isnet() const
{
    return area->isnet();
}

inline int Area::isecho() const
{
    return area->isecho();
}

inline int Area::islocal() const
{
    return area->islocal();
}

inline int Area::isemail() const
{
    return area->isemail();
}

inline int Area::isnewsgroup() const
{
    return area->isnewsgroup();
}

inline int Area::isinternet() const
{
    return area->isinternet();
}

inline int Area::isqwk() const
{
    return area->isqwk();
}

inline int Area::issoup() const
{
    return area->issoup();
}

inline uint Area::lastread() const
{
    return area->lastread;
}

inline uint32_t Area::lastreadentry() const
{
    return area->lastreadentry;
}

inline void Area::set_lastread(uint lr)
{
    area->lastread = lr;
}

inline void Area::SetBookmark(uint __relno)
{
    bookmark = Msgn.CvtReln(__relno); /*Book.Add(bookmark);*/
}

inline void Area::Suspend()
{
    area->suspend();
}

inline void Area::Resume()
{
    area->resume();
}

inline void Area::Lock()
{
    area->lock();
}

inline void Area::Unlock()
{
    area->unlock();
}

inline void Area::DelMsg(GMsg * msg)
{
    area->del_msg(msg);
}

inline void Area::NewMsgno(GMsg * msg)
{
    area->new_msgno(msg);
}

inline char * Area::UserLookup(char * lookfor)
{
    return area->user_lookup(lookfor);
}

inline int Area::Renumber()
{
    return area->renumber();
}

inline Line * Area::MakeDumpMsg(GMsg * msg, char * lng_head)
{
    return area->make_dump_msg(msg->lin, msg, lng_head);
}

inline void Area::SetHighwaterMark()
{
    area->set_highwater_mark();
}

inline void Area::ResetHighwaterMark()
{
    area->reset_highwater_mark();
}

inline void Area::UpdateTimesread(GMsg * msg)
{
    area->update_timesread(msg);
}

//  ------------------------------------------------------------------
//  Arealist picker class
class GPickArealist : public gwinpick
{
private: int areawin;
    int tempwin;
    int tempwin1;
    int areawin1;
    int areawin2;
    int tempwin2;
    uint area_fuzidx;
    int pmails;
    int pmareas;
    bool pmscan;
    void jump_to();
    void jumpmatch();
    void center();
    void AreasRenumber();
    void AreaCatchUp(uint n);
    void AreaDropMsgMarks(uint n);
    void dispbuf(char * buf, int areano);

protected: public: uint area_maxfuz;
    bool esc_abort;
    void open();                        // Called after window is opened
    void close();                       // Called after window is closed
    void precursor();                   // Called before any cursor movement
    void do_delayed();                  // Called after a delay
    void print_line(uint idx, uint pos, bool isbar);
    bool handle_key();                  // Handles keypress
    bool is_selectable(uint idx);       // is not separator ?
    void close_all();

void update()
{
    display_page();
    display_bar();
}

    int Run(const char * _title, int wpos, int & idx);

    GPickArealist();
};
//  ------------------------------------------------------------------

#endif // ifndef __GMAREA_H
