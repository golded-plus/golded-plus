//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999-2000 Alexander S. Aganichev
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
//  Area structures and classes.
//  ------------------------------------------------------------------


//  ------------------------------------------------------------------

#ifndef __GMBAREA_H
#define __GMBAREA_H


//  ------------------------------------------------------------------

#include <string>
#include <gstrbags.h>
#include <gutltag.h>
#include <glog.h>
#include <gedacfg.h>
#include <gmo_msg.h>


//  ------------------------------------------------------------------
//  Area base class

class gmo_area
{

public:

    //  ----------------------------------------------------------------
    //  Config data

    AreaCfgBase cfg;
    GStrSet3    cfg2;


    //  ----------------------------------------------------------------
    //  Constructor and destructor

    gmo_area();
    virtual ~gmo_area();


    //  ----------------------------------------------------------------
    //  Data members

    GTag*   Msgn;               // Message numbers
    GTag*   PMrk;               // Personal mail marks

    uint    lastread;           // Number of last message read
    uint32_t   lastreadentry;      // Lastread message number at entry to area

    int     isopen;             // NONZERO if open


    //  ----------------------------------------------------------------
    //  Access config data

    int   areaid() const
    {
        return cfg.areaid;
    }
    int   groupid() const
    {
        return cfg.groupid;
    }
    uint  type() const
    {
        return cfg.type;
    }
    const std::string &basetype() const
    {
        return cfg.basetype;
    }
    uint  board() const
    {
        return cfg.board;
    }
    const ftn_addr& aka() const
    {
        return cfg.aka;
    }
    int   originno() const
    {
        return cfg.originno;
    }
    Attr& attr()
    {
        return cfg.attr;
    }
    bool  ispacked() const
    {
        return make_bool(cfg.attr.pkd());
    }
    void  packed(bool a)
    {
        cfg.attr.pkd(a);
    }

    bool ascan()
    {
        return (bool)cfg.scan;
    }
    bool ascanexcl()
    {
        return (bool)cfg.scanexcl;
    }
    bool ascanincl()
    {
        return (bool)cfg.scanincl;
    }
    bool pmscan()
    {
        return (bool)cfg.pmscan;
    }
    bool pmscanexcl()
    {
        return (bool)cfg.pmscanexcl;
    }
    bool pmscanincl()
    {
        return (bool)cfg.pmscanincl;
    }

    const char* echoid() const
    {
        return cfg2.Get1();
    }
    const char* desc() const
    {
        return cfg2.Get2();
    }
    const char* path() const
    {
        return cfg2.Get3();
    }

    void set_areaid(int a)
    {
        cfg.areaid = a;
    }
    void set_groupid(int g)
    {
        cfg.groupid = g;
    }
    void set_type(uint t)
    {
        cfg.type = t;
    }
    void set_basetype(const char *m)
    {
        cfg.basetype = m;
    }
    void set_basetype(const std::string &m)
    {
        cfg.basetype = m;
    }
    void set_board(uint b)
    {
        cfg.board = b;
    }
    void set_aka(ftn_addr& a)
    {
        cfg.aka = a;
    }
    void set_originno(int o)
    {
        cfg.originno = o;
    }
    void set_attr(Attr& a)
    {
        cfg.attr = a;
    }
    void set_origin(char* o)
    {
        std::string tmp = o;
        cfg.setorigin(tmp);
    }

    void set_scan(bool s)
    {
        cfg.scan = (byte)s;
    }
    void set_scanexcl(bool s)
    {
        cfg.scanexcl = (byte)s;
    }
    void set_scanincl(bool s)
    {
        cfg.scanincl = (byte)s;
    }
    void set_pmscan(bool s)
    {
        cfg.pmscan = (byte)s;
    }
    void set_pmscanexcl(bool s)
    {
        cfg.pmscanexcl = (byte)s;
    }
    void set_pmscanincl(bool s)
    {
        cfg.pmscanincl = (byte)s;
    }

    void set_echoid(const char* s)
    {
        cfg2.Change1(s);
    }
    void set_desc(const char* s)
    {
        cfg2.Change2(s);
    }
    void set_path(const char* s)
    {
        cfg2.Change3(s);
    }


    //  ----------------------------------------------------------------
    //  Determine msgbase format

    bool isseparator() const;


    //  ----------------------------------------------------------------
    //  Determine area features

    virtual bool issoftdelete() const
    {
        return false;
    }
    virtual bool havearrivedstamp() const
    {
        return true;
    }
    virtual bool havereceivedstamp() const
    {
        return true;
    }
    virtual bool requirehardterm() const
    {
        return false;
    }
    virtual bool requiresoftterm() const
    {
        return false;
    }


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
    //  Low-level messagebase member functions

    virtual void open() = 0;
    virtual void close() = 0;

    virtual void suspend() = 0;
    virtual void resume() = 0;

    virtual void lock() = 0;
    virtual void unlock() = 0;

    virtual void scan() = 0;
    virtual void scan_area() = 0;
    virtual void scan_area_pm() = 0;

    virtual int load_hdr(gmsg* msg) = 0;
    virtual int load_msg(gmsg* msg) = 0;

    virtual void save_hdr(int mode, gmsg* msg) = 0;
    virtual void save_msg(int mode, gmsg* msg) = 0;

    virtual void del_msg(gmsg* msg) = 0;

    virtual void new_msgno(gmsg* msg) = 0;
    virtual char* user_lookup(char* lookfor) = 0;
    virtual int renumber() = 0;

    virtual void update_timesread(gmsg* msg) = 0;

    virtual Line* make_dump_msg(Line*& lin, gmsg* msg, char* lng_head) = 0;

    virtual void set_highwater_mark() { }
    virtual void reset_highwater_mark() { }

protected:
    Path realpath;
    const char* real_path() const
    {
        return ispacked() ? realpath : path();
    }
    void set_real_path(const char* newpath)
    {
        strxcpy(realpath, newpath, sizeof(Path));
    }
};


//  ------------------------------------------------------------------
//  Inline implementations

inline bool gmo_area::isseparator() const
{
    return cfg.isseparator();
}

inline int gmo_area::isnet() const
{
    return cfg.isnet();
}
inline int gmo_area::isecho() const
{
    return cfg.isecho();
}
inline int gmo_area::islocal() const
{
    return cfg.islocal();
}
inline int gmo_area::isemail() const
{
    return cfg.isemail();
}
inline int gmo_area::isnewsgroup() const
{
    return cfg.isnewsgroup();
}
inline int gmo_area::isinternet() const
{
    return cfg.isinternet();
}
inline int gmo_area::isqwk() const
{
    return cfg.isqwk();
}
inline int gmo_area::issoup() const
{
    return cfg.issoup();
}


//  ------------------------------------------------------------------

class SepArea : public gmo_area
{

public:

    SepArea() { }
    virtual ~SepArea() { }

    //  ----------------------------------------------------------------
    //  Messagebase member functions

    void open() { }
    void close() { }

    void suspend() { }
    void resume() { }

    void lock() { }
    void unlock() { }

    void scan() { }
    void scan_area() { }
    void scan_area_pm() { }

    int load_hdr(gmsg*)
    {
        return 0;
    }
    int load_msg(gmsg*)
    {
        return 0;
    }

    void save_hdr(int, gmsg*) { }
    void save_msg(int, gmsg*) { }

    void del_msg(gmsg*) { }

    void new_msgno(gmsg*) { }
    char* user_lookup(char*)
    {
        return NULL;
    }
    int renumber()
    {
        return false;
    }

    void update_timesread(gmsg*) { }

    Line* make_dump_msg(Line*&, gmsg*, char*)
    {
        return NULL;
    }
};


//  ------------------------------------------------------------------
//  Personal mail

const int PM_STARTUP  = 0x0001;
const int PM_ALLNAMES = 0x0002;
const int PM_LISTONLY = 0x0004;


//  ------------------------------------------------------------------

extern glog*        WideLog;
extern int          WideDebug;
extern int          WideCanLock;
extern const char** WideUsername;
extern int          WideUsernames;
extern uint         WideSharemode;
extern bool         WideDispsoftcr;  // DispsoftCR for current area
extern int          WidePersonalmail;

//  ------------------------------------------------------------------

bool PopupLocked(long __tries, int __isopen, const char* __file);
const char* Unpack(const char* archive);
void CleanUnpacked(const char* unpacked);

//  ------------------------------------------------------------------
//  SOFTCR management

// Detects SoftCR only if DispsoftCR is ON
inline bool issoftcr(char c)
{
    return not WideDispsoftcr and (c == SOFTCR);
}

//  Skip spaces and SoftCRs (if DispsoftCR is ON)
inline char *spanspaces(const char *str)
{
    while ( (isspace(*str) and (*str != CR)) or issoftcr(*str) )
        str++;
    return (char *)str;
}

// Skip LineFeeds and SoftCRs (if DispsoftCR is ON)
inline char *spanfeeds(const char *str)
{
    while ( (*str == LF) or issoftcr(*str) )
        str++;
    return (char *)str;
}


#endif

//  ------------------------------------------------------------------

