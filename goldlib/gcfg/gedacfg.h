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
//  AREAFILE processor.
//  ------------------------------------------------------------------

#ifndef __gedacfg_h
#define __gedacfg_h


//  ------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <string>
#include <gftnall.h>
#include <gfile.h>
#include <gmsgattr.h>
#include <geall.h>


//  ------------------------------------------------------------------
//  Area types

const uint GMB_NET       = 0x0001;
const uint GMB_EMAIL     = 0x0002;
const uint GMB_ECHO      = 0x0004;
const uint GMB_NEWSGROUP = 0x0008;
const uint GMB_LOCAL     = 0x0010;
const uint GMB_QWK       = 0x1000;
const uint GMB_SOUP      = 0x2000;
const uint GMB_NONE      = 0x00ff;
const uint GMB_DEFAULT   = 0xf00f;


//  ------------------------------------------------------------------

class AreaCfgBase
{

public:

    int      areaid;          // Unique internal area number
    int      groupid;         // Group id (A-Z)
    int      originno;        // Origin number
    uint     board;           // Board number (Hudson/Goldbase/Ezycom/PCBoard fmts)
    uint     type;            // Type of msgarea (GMB_xxx)
    std::string basetype;     // Type of msgbase
    ftn_addr aka;             // The AKA to use in the area
    ftn_attr attr;            // Default attributes
    byte     scan       : 1;  // TRUE if listed with AREASCAN
    byte     scanexcl   : 1;  // TRUE if listed with AREASCANEXCL
    byte     scanincl   : 1;  // TRUE if listed with AREASCANINCL
    byte     pmscan     : 1;  // TRUE if listed with AREAPMSCAN
    byte     pmscanexcl : 1;  // TRUE if listed with AREAPMSCANEXCL
    byte     pmscanincl : 1;  // TRUE if listed with AREAPMSCANINCL

    int setorigin(std::string& origin);

    bool isseparator() const
    {
        return (basetype == "SEPARATOR");
    }

    bool isnet() const
    {
        return !!(type & GMB_NET);
    }
    bool isecho() const
    {
        return !!(type & GMB_ECHO);
    }
    bool islocal() const
    {
        return !!(type & GMB_LOCAL);
    }
    bool isemail() const
    {
        return !!(type & GMB_EMAIL);
    }
    bool isnewsgroup() const
    {
        return !!(type & GMB_NEWSGROUP);
    }
    bool isinternet() const
    {
        return !!(type & (GMB_EMAIL|GMB_NEWSGROUP));
    }
    bool isqwk() const
    {
        return !!(type & GMB_QWK);
    }
    bool issoup() const
    {
        return !!(type & GMB_SOUP);
    }

};


//  ------------------------------------------------------------------

class AreaCfg : public AreaCfgBase
{

public:

    static int autoid;

    Echo          echoid;    // Echo tag
    Desc          desc;      // Area description
    Path          path;      // Path to message area
    std::string   origin;    // Origin

    AreaCfg()
    {
        reset();
    }
    ~AreaCfg()  { }

    AreaCfg& operator=(const AreaCfg& x);

    void reset();

    const char* setdesc(const char* s);
    const char* setautoid(const char* s);
    const char* setechoid(const char* s);
    const char* setorigin(const char* s);
    const char* setpath(const char* s);
};


//  ------------------------------------------------------------------

struct EchoList
{

    Echo echoid;
    Path path;
    Desc desc;
};


//  ------------------------------------------------------------------

struct DescList
{

    Echo echoid;
    Desc desc;
};


//  ------------------------------------------------------------------

class EchoListClass
{

private:

    EchoList** echolist;
    DescList** desclist;
    int echos;
    int descs;

public:

    EchoListClass();
    ~EchoListClass();

    void FreeAll();
    int  Echos()
    {
        return(echos);
    }
    void SortEchos();
    void AddDesc(char* echoid, char* desc);
    int  FindDesc(char* echoid, char** desc);
    void AddEcho(char* echoid, char* path, char* desc);
    int  FindEcho(char* echoid, char* path, char* desc);
    int  GetEcho(int n, char** echoid, char** path, char** desc);
};


//  ------------------------------------------------------------------

class gareafile
{

protected:

    Path pathprefix;

#ifndef GCFG_NOFIDOCONF
    // Fidoconfig parser functions
    void gettok(char** key, char** val);
#endif
#if !defined(GCFG_NOCMAIL) || !defined(GCFG_NOCECHO)
    // Crashmail II parser function
    bool jbstrcpy(char *dest, char *src, size_t maxlen, size_t *jbc);
    void ReadCrashmailCfg(const char* file);
#endif
#ifndef GCFG_NOTIMED
    // Timed parser function
    void nullastbackslash(char* val);
#endif
#ifndef GCFG_NOWATERGATE
    // Watergate parser function
    const char *gettype(const char *msgtype, const byte wtrtype);
#endif
#ifndef GCFG_NOXMAIL
    // XMail parser function
    char* ClipDosFilename(char* __file);
#endif

    void adjustpath(char* path);

#ifndef GCFG_NODB
    void ReadDB130(char* tag, char* dbpath);
    void ReadDB1046(char* file, char* tag);
    void ReadDB1047A22(char* file, int reclen, char* tag);
    void ReadDB2011(char* file, int reclen, char* tag);
#endif
#ifndef GCFG_NOEZY
    void ReadEzycom102(gfile &fp, char* path, char* file, char* options);
    void ReadEzycom110(gfile &fp, char* path, char* file, char* options);
#endif
#ifndef GCFG_NOFE
    void ReadFastecho11x(int fh);
    void ReadFastecho141(int fh);
    void ReadFastecho142(int fh);
#endif
#ifndef GCFG_NOFMAIL
    void ReadFMail092(gfile &fp, char* path, char* file, char* options);
    void ReadFMail098(gfile &fp, char* path, char* file, char* options);
    void ReadFMail116(gfile &fp, char* path, char* file, char* options);
#endif
#ifndef GCFG_NOFIDOCONF
    AreaCfg echoareadefaults;
    bool ReadHPTLine(gfile &f, std::string& str);
    void ReadHPTFile(char* path, char* file, char* origin, int group);
#endif
#ifndef GCFG_NOIMAIL
    void ReadIMail160(char* options, char* file, char* impath);
    void ReadIMail170(char* options, char* file, char* impath);
    void ReadIMail185(char* options, char* file, char* impath);
#endif
#ifndef GCFG_NOMAXIMUS
    void ReadMaximus3(char* mxpath, char* areafile, char* options);
#endif
#ifndef GCFG_NOQBBS
    void ReadQ260(char* qbpath, char* origin, char* options);
    void ReadQ276(char* qbpath, char* origin, char* options);
#endif
#ifndef GCFG_NOQECHO
    void ReadQEchoFile(char* file, char* options, char* origin);
#endif
#ifndef GCFG_NOSPCT
    void ParseSpaceArea(const char *type_path, AreaCfg &aa);
    void ReadSpaceCtl(const char *path);
    void ReadSpaceNtm(const char *path);
    void ReadSpaceAr(const char *path);
#endif
#if !defined(GCFG_NOSQSH) || !defined(GCFG_NOPARTOSS)
    void ReadSquishFile(char* path, char* file, char* options, char* origin, int group);
#endif
#ifndef GCFG_NOTIMED
    void ReadTimedFile(char* path, char* file, char* options, char* origin);
#endif
#ifndef GCFG_NOTERMAIL
    void ReadTmailFile(char* file, char* options, char* origin);
#endif
#ifndef GCFG_NOWATERGATE
    void ReadWtrGteFile(char* options, gfile &fp);
#endif
#ifndef GCFG_NOXBBS
    void ReadAdeptXbbsFile(char* path, char* file, char* options);
#endif
#ifndef GCFG_NOXMAIL
    void ReadxMailFile(char* file, char* options);
#endif

public:

    gareafile();

    EchoListClass echolist;

    int quiet;

    int sharemode;
    const char *fidomsgtype;
    int ra2usersbbs;
    int squishuserno;

    const char *areapath;
    const char *pcboardpath;

    ftn_addr primary_aka;

    ftn_attr attribsnet;
    ftn_attr attribsecho;
    ftn_attr attribsnews;
    ftn_attr attribsemail;
    ftn_attr attribslocal;

    void GetAreasBBS(char* name, char* origin, char* options);

    void ReadAreasBBS(char* tag);
#ifndef GCFG_NOXBBS
    void ReadAdeptXBBS(char* tag);
#endif
#if !defined(GCFG_NOCMAIL) || !defined(GCFG_NOCECHO)
    void ReadCrashmail(char* tag);
#endif
#ifndef GCFG_NODB
    void ReadDBridge(char* tag);
#endif
#ifndef GCFG_NODUTCHIE
    void ReadDutchie(char* tag);
#endif
#ifndef GCFG_NOEZY
    void ReadEzycom(char* tag);
#endif
#ifndef GCFG_NOFE
    void ReadFastecho(char* tag);
#endif
#ifndef GCFG_NOFIDOPCB
    void ReadFidoPCB(char* tag);
#endif
#ifndef GCFG_NOFMAIL
    void ReadFMail(char* tag);
#endif
#ifndef GCFG_NOFD
    void ReadFrontDoor(char* tag);
#endif
#ifndef GCFG_NOGECHO
    void ReadGEcho(char* tag);
#endif
#ifndef GCFG_NOFIDOCONF
    void ReadHPT(char* tag);
#endif
#ifndef GCFG_NOIMAIL
    void ReadIMail(char* tag);
#endif
#ifndef GCFG_NOINTERMAIL
    void ReadInterMail(char* tag);
#endif
#ifndef GCFG_NOLORA
    void ReadLoraBBS(char* tag);
#endif
#ifndef GCFG_NOMAXIMUS
    void ReadMaximus(char* tag);
#endif
#ifndef GCFG_NOME2
    void ReadME2(char* tag);
#endif
#ifndef GCFG_NOOPUS
    void ReadOpus(char* tag);
#endif
#ifndef GCFG_NOPCB
    void ReadPCBoard(char* tag);
#endif
#ifndef GCFG_NOPORTAL
    void ReadPortal(char* tag);
#endif
#ifndef GCFG_NOPROBOARD
    void ReadProBoard(char* tag);
#endif
#ifndef GCFG_NOQECHO
    void ReadQEcho(char* tag);
#endif
#ifndef GCFG_NOQFRONT
    void ReadQFront(char* tag);
#endif
#ifndef GCFG_NOQBBS
    void ReadQuickBBS(char* tag);
#endif
#ifndef GCFG_NORAECHO
    void ReadRaEcho(char* tag);
#endif
#ifndef GCFG_NORA
    void ReadRemoteAccess(char* tag);
#endif
#if !defined(GCFG_NOSQSH) || !defined(GCFG_NOPARTOSS)
    void ReadSquish(char* tag);
#endif
#ifndef GCFG_NOSPCT
    void ReadSpaceToss(char *tag);
#endif
#ifndef GCFG_NOSBBS
    void ReadSuperBBS(char* tag);
#endif
#ifndef GCFG_NOTIMED
    void ReadTimed(char* tag);
#endif
#ifndef GCFG_NOTERMAIL
    void ReadTmail(char* tag);
#endif
#ifndef GCFG_NOTOSSCAN
    void ReadTosScan(char* tag);
#endif
#ifndef GCFG_NOWMAIL
    void ReadWMail(char* tag);
#endif
#ifndef GCFG_NOWATERGATE
    void ReadWtrGte(char* tag);
#endif
#ifndef GCFG_NOXMAIL
    void ReadXMail(char* tag);
#endif
#ifndef GCFG_NOSYNCHRONET
    void ReadSynchronet(char* tag);
#endif

    bool ReadAreafile(word crc, char* parameters);
};


//  ------------------------------------------------------------------

void AddNewArea(AreaCfg& aa);
void CfgAddress(char* value);
void CfgOrigin(const char* value);
void CfgUsername(char* value);
void CfgJAMSMAPIHighwater(bool value);
void ReadEcholist(char* value);
void SetAreaDesc(char* echoid, char* desc);

void CfgAdeptxbbspath(const char *path, bool force = false);
void CfgEzycommsgbase(const char *path, bool force = false);
void CfgEzycomuserbase(const char *path, bool force = false);
void CfgGoldbasepath(const char *path, bool force = false);
void CfgHudsonpath(const char *path, bool force = false);
void CfgJampath(const char *path, bool force = false);
void CfgPcboardpath(const char *path, bool force = false);
void CfgSquishuserpath(const char *path, bool force = false);
void CfgFidolastread(const char *path);


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
