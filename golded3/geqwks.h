
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
//  QWK Implementation Structures and Classes.
//  ------------------------------------------------------------------
#ifndef __GEQWKS_H
#define __GEQWKS_H
//  ------------------------------------------------------------------

struct QwkCfg
{
    Echo badmsgs;
    Path exportpath;
    Path importpath;
    Path replylinker;
    Path tosslog;
};


//  ------------------------------------------------------------------

struct QwkMap
{
    char confname[21];
    int  confno;
    Echo echoid;
};


//  ------------------------------------------------------------------

struct QwkBbs
{
    char    bbsid[9];
    int     kludges : 1;  // TRUE if allowed to export kludges
    int     mixcase : 1;  // TRUE if mixed case is allowed
    int     receipt : 1;  // TRUE if return receipts are supported
    int     maxlines;
    QwkMap* map;
    int     maps;
};


//  ------------------------------------------------------------------

class Qwk
{

    QwkCfg  cfg;    // Global QWK configuration
    QwkBbs* bbs;    // Collection of BBS data
    int     bbss;   // Number of BBS'es stored
    QwkBbs* bbsp;   // Pointer to current BBS
    int     bbsn;   // Number of current BBS
    QwkMap* mapp;   // Pointer to current conference map
    int     mapn;   // Number of current conference map

public:

    Qwk();
    ~Qwk();

    void Reset();

    void AddBBS(char* bbsid);
    void AddMap(char* bbsid, char* echoid, char* confname, int confno=-1);

    int FindBBS(char* bbsid);
    void FindAddBBS(char* bbsid);
    int FoundBBS()
    {
        return !!bbsp;
    }
    int FirstBBS();
    int NextBBS();

    int FindConf(char* echoid);
    int FindEcho(char* confname);
    int FindEcho(int confno);
    int FoundConf()
    {
        return !!mapp;
    }
    int FirstConf();
    int NextConf();

    const char* BbsID();
    const char* ConfName();
    int ConfNo(int set=-2);
    const char* EchoID();

    int KludgesAllowed(int set=-1);
    int MixCaseAllowed(int set=-1);
    int ReceiptAllowed(int set=-1);
    int MaxLines(int set=-1);

    void ResetConfNo(int set=-1);

    char* BadMsgs(char* set=NULL);
    char* ExportPath(char* set=NULL);
    char* ImportPath(char* set=NULL);
    char* ReplyLinker(char* set=NULL);
    char* TossLog(char* set=NULL);
};


//  ------------------------------------------------------------------

#endif
