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
#include <gftnall.h>
#include <gfilutil.h>
#include <string>
#include <gmsgattr.h>


//  ------------------------------------------------------------------

const int MAX_DESC =  45;        // Area descriptions
const int MAX_ECHO =  81;        // Echoids


//  ------------------------------------------------------------------
//  Echoid typedefs

typedef char Echo[MAX_ECHO];


//  ------------------------------------------------------------------
//  Description typedefs

typedef char Desc[MAX_DESC];


//  ------------------------------------------------------------------
//  Area types

const uint GMB_NET       = 0x0001;
const uint GMB_EMAIL     = 0x0002;
const uint GMB_ECHO      = 0x0004;
const uint GMB_NEWSGROUP = 0x0008;
const uint GMB_LOCAL     = 0x0010;
const uint GMB_QWK       = 0x1000;
const uint GMB_SOUP      = 0x2000;


//  ------------------------------------------------------------------
//  Msgbase types

const uint GMB_HUDSON    = 0x0001;
const uint GMB_GOLDBASE  = 0x0002;
const uint GMB_JAM       = 0x0004;
const uint GMB_SQUISH    = 0x0008;
const uint GMB_OPUS      = 0x0010;
const uint GMB_FTS1      = 0x0020;
const uint GMB_FIDO      = 0x0030;
const uint GMB_EZYCOM    = 0x0040;
const uint GMB_PCBOARD   = 0x0080;
const uint GMB_WILDCAT   = 0x0100;
const uint GMB_ADEPTXBBS = 0x0200;
const uint GMB_MAILBOX   = 0x1000;
const uint GMB_NEWSSPOOL = 0x2000;
const uint GMB_SEPARATOR = 0x8000;
const uint GMB_SMB       = 0x10000;


//  ------------------------------------------------------------------

class AreaCfgBase {

public:

  int      areaid;          // Unique internal area number
  int      groupid;         // Group id (A-Z)
  int      originno;        // Origin number
  uint     board;           // Board number (Hudson/Goldbase/Ezycom/PCBoard fmts)
  uint     type;            // Type of msgarea (GMB_xxx)
  uint     msgbase;         // Type of msgbase (GMB_xxx)
  ftn_addr aka;             // The AKA to use in the area
  ftn_attr attr;            // Default attributes
  byte     scan       : 1;  // TRUE if listed with AREASCAN
  byte     scanexcl   : 1;  // TRUE if listed with AREASCANEXCL
  byte     scanincl   : 1;  // TRUE if listed with AREASCANINCL
  byte     pmscan     : 1;  // TRUE if listed with AREAPMSCAN
  byte     pmscanexcl : 1;  // TRUE if listed with AREAPMSCANEXCL
  byte     pmscanincl : 1;  // TRUE if listed with AREAPMSCANINCL

  int setorigin(string& origin);

  bool isfts1() const                { return !!(msgbase & GMB_FTS1); }
  bool isopus() const                { return !!(msgbase & GMB_OPUS); }
  bool isezycom() const              { return !!(msgbase & GMB_EZYCOM); }
  bool isfido() const                { return !!(msgbase & (GMB_FTS1|GMB_OPUS)); }
  bool isgoldbase() const            { return !!(msgbase & GMB_GOLDBASE); }
  bool ishudson() const              { return !!(msgbase & GMB_HUDSON); }
  bool isjam() const                 { return !!(msgbase & GMB_JAM); }
  bool ispcboard() const             { return !!(msgbase & GMB_PCBOARD); }
  bool issquish() const              { return !!(msgbase & GMB_SQUISH); }
  bool iswildcat() const             { return !!(msgbase & GMB_WILDCAT); }
  bool isadeptxbbs() const           { return !!(msgbase & GMB_ADEPTXBBS); }
  bool isseparator() const           { return !!(msgbase & GMB_SEPARATOR); }
  bool issmb() const                 { return !!(msgbase & GMB_SMB); }

  bool isnet() const                 { return !!(type & GMB_NET); }
  bool isecho() const                { return !!(type & GMB_ECHO); }
  bool islocal() const               { return !!(type & GMB_LOCAL); }
  bool isemail() const               { return !!(type & GMB_EMAIL); }
  bool isnewsgroup() const           { return !!(type & GMB_NEWSGROUP); }
  bool isinternet() const            { return !!(type & (GMB_EMAIL|GMB_NEWSGROUP)); }
  bool isqwk() const                 { return !!(type & GMB_QWK); }
  bool issoup() const                { return !!(type & GMB_SOUP); }

};


//  ------------------------------------------------------------------

class AreaCfg : public AreaCfgBase {

public:

  static int autoid;

  Echo     echoid;    // Echo tag
  Desc     desc;      // Area description
  Path     path;      // Path to message area
  string   origin;    // Origin

  AreaCfg()   { reset(); }
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

struct EchoList {

  Echo echoid;
  Path path;
  Desc desc;
};


//  ------------------------------------------------------------------

struct DescList {

  Echo echoid;
  Desc desc;
};


//  ------------------------------------------------------------------

class EchoListClass {

private:

  EchoList** echolist;
  DescList** desclist;
  int echos;
  int descs;

public:

  EchoListClass();
  ~EchoListClass();

  void FreeAll();
  int  Echos() { return(echos); }
  void SortEchos();
  void AddDesc(char* echoid, char* desc);
  int  FindDesc(char* echoid, char** desc);
  void AddEcho(char* echoid, char* path, char* desc);
  int  FindEcho(char* echoid, char* path, char* desc);
  int  GetEcho(int n, char** echoid, char** path, char** desc);
};


//  ------------------------------------------------------------------

class gareafile {

protected:

  Path pathprefix;

  // Fidoconfig parser functions
  void replace_slashes(char **key);
  void gettok(char** key, char** val);
  // Crashmail II parser function
  bool jbstrcpy(char *dest, char *src, size_t maxlen, size_t *jbc);
  // Timed parser function
  void nullastbackslash(char* val);
  // Watergate parser function
  uint gettype(uint msgtype, const byte wtrtype);
  // XMail parser function
  char* ClipDosFilename(char* __file);

  void adjustpath(char* path);

  void ReadDB130(char* tag, char* dbpath);
  void ReadDB1046(char* file, char* tag);
  void ReadDB1047A22(char* file, int reclen, char* tag);
  void ReadDB2011(char* file, int reclen, char* tag);
  void ReadEzycom102(FILE* fp, char* path, char* file, char* options);
  void ReadEzycom110(FILE* fp, char* path, char* file, char* options);
  void ReadFastecho142(int fh);
  void ReadFMail092(FILE* fp, char* path, char* file, char* options);
  void ReadFMail098(FILE* fp, char* path, char* file, char* options);
  void ReadFMail116(FILE* fp, char* path, char* file, char* options);
  bool ReadHPTLine(FILE* f, string* s, bool add=false, int state=0);
  void ReadHPTFile(char* path, char* file, char* options, char* origin, int group);
  void ReadIMail160(char* options, char* file, char* impath);
  void ReadIMail170(char* options, char* file, char* impath);
  void ReadIMail185(char* options, char* file, char* impath);
  void ReadMaximus3(char* mxpath, char* areafile, char* options);
  void ReadQ260(char* qbpath, char* origin, char* options);
  void ReadQ276(char* qbpath, char* origin, char* options);
  void ReadQEchoFile(char* file, char* options, char* origin);
  void ReadSquishFile(char* path, char* file, char* options, char* origin, int group);
  void ReadTimedFile(char* path, char* file, char* options, char* origin);
  void ReadTmailFile(char* file, char* options, char* origin);
  void ReadWtrGteFile(char* options, FILE* fp);
  void ReadAdeptXbbsFile(char* path, char* file, char* options);
  void ReadxMailFile(char* file, char* options);

public:

  gareafile();

  EchoListClass echolist;

  int quiet;

  int sharemode;
  int fidomsgtype;
  int ra2usersbbs;
  int squishuserno;

  ftn_addr primary_aka;

  ftn_attr attribsnet;
  ftn_attr attribsecho;
  ftn_attr attribsnews;
  ftn_attr attribsemail;
  ftn_attr attribslocal;

  char* areapath;
  char* adeptxbbspath;
  char* jampath;
  char* squishuserpath;
  char* hudsonpath;
  char* goldbasepath;
  char* pcboardpath;
  char* ezycom_msgbasepath;
  char* ezycom_userbasepath;
  char* fidolastread;

  void GetAreasBBS(char* name, char* origin, char* options);

  void ReadAreasBBS(char* tag);
  void ReadAdeptXBBS(char* tag);
  void ReadCrashmail(char* tag);
  void ReadDBridge(char* tag);
  void ReadDutchie(char* tag);
  void ReadEzycom(char* tag);
  void ReadFastecho(char* tag);
  void ReadFidoPCB(char* tag);
  void ReadFMail(char* tag);
  void ReadFrontDoor(char* tag);
  void ReadGEcho(char* tag);
  void ReadHPT(char* tag);
  void ReadIMail(char* tag);
  void ReadInterMail(char* tag);
  void ReadLoraBBS(char* tag);
  void ReadMaximus(char* tag);
  void ReadME2(char* tag);
  void ReadOpus(char* tag);
  void ReadPCBoard(char* tag);
  void ReadPortal(char* tag);
  void ReadProBoard(char* tag);
  void ReadQEcho(char* tag);
  void ReadQFront(char* tag);
  void ReadQuickBBS(char* tag);
  void ReadRaEcho(char* tag);
  void ReadRemoteAccess(char* tag);
  void ReadSquish(char* tag);
  void ReadSuperBBS(char* tag);
  void ReadTimed(char* tag);
  void ReadTmail(char* tag);
  void ReadTosScan(char* tag);
  void ReadWMail(char* tag);
  void ReadWtrGte(char* tag);
  void ReadXMail(char* tag);

  bool ReadAreafile(word crc, char* parameters);
};


//  ------------------------------------------------------------------

void AddNewArea(AreaCfg& aa);
void CfgAddress(char* value);
void CfgOrigin(const char* value);
void CfgUsername(char* value);
void ReadEcholist(char* value);
void SetAreaDesc(char* echoid, char* desc);


//  ------------------------------------------------------------------
//  Legacy area types

const uint AT_NET       = GMB_NET;
const uint AT_EMAIL     = GMB_EMAIL;
const uint AT_ECHO      = GMB_ECHO;
const uint AT_NEWSGROUP = GMB_NEWSGROUP;
const uint AT_LOCAL     = GMB_LOCAL;
const uint AT_QWK       = GMB_QWK;
const uint AT_SOUP      = GMB_SOUP;


//  ------------------------------------------------------------------
//  Legacy msgbase types

const uint MT_HUDSON    = GMB_HUDSON;
const uint MT_GOLDBASE  = GMB_GOLDBASE;
const uint MT_JAM       = GMB_JAM;
const uint MT_SQUISH    = GMB_SQUISH;
const uint MT_OPUS      = GMB_OPUS;
const uint MT_FTS1      = GMB_FTS1;
const uint MT_FIDO      = GMB_FIDO;
const uint MT_EZYCOM    = GMB_EZYCOM;
const uint MT_PCBOARD   = GMB_PCBOARD;
const uint MT_WILDCAT   = GMB_WILDCAT;
const uint MT_ADEPTXBBS = GMB_ADEPTXBBS;
const uint MT_MAILBOX   = GMB_MAILBOX;
const uint MT_NEWSSPOOL = GMB_NEWSSPOOL;
const uint MT_SEPARATOR = GMB_SEPARATOR;
const uint MT_SMB       = GMB_SMB;


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
