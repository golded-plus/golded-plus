//  This may look like C code, but it is really -*- C++ -*-
//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
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
//  WildCat! derived userfile class.
//  ------------------------------------------------------------------

#ifndef __gusrwcat_h
#define __gusrwcat_h
//  ------------------------------------------------------------------

#include <gfile.h>
#include <gusrbase.h>
//  ------------------------------------------------------------------

#if defined (GOLD_CANPACK)
#pragma pack(1)
#endif
//  ------------------------------------------------------------------
struct WCatUserConfFileHdr
{
    word totalconfs;
};

//  ------------------------------------------------------------------
const byte ucrIndex = 0;
const byte ucrData  = 1;
struct WCatUserConfIndex
{
    word reclen;
    byte rectype;
    long offsets[32];
};

struct WCatUserConfData
{
    byte flags;
    word lastread;
    word firstunread;
};

typedef WCatUserConfData WCatUserConfArray[1024];
struct WCatUserConfPageHeader
{
    word  reclen;
    byte  rectype;
    long  userid;
    short page;
    long  thisx;
};

struct WCatUserConfPage
{
    word              reclen;
    byte              rectype;
    long              userid;
    short             page;
    long              thisx;
    WCatUserConfArray userconfdata;
};

#define MaxChunk 1024
#define MaxPages(MaxConfAreas) ((MaxConfAreas + MaxChunk - 1) / MaxChunk)
//  ------------------------------------------------------------------

#if defined (GOLD_CANPACK)
#pragma pack()
#endif
//  ------------------------------------------------------------------
//  WildCat! userfile class
class WCatUser : public GUser
{
public: uint maxindex;
    int firstread;
    WCatUserConfIndex header;
    WCatUser();
    ~WCatUser();
    int isvalid();
    int read();
    void founduser();
    void recinit(const char * __name);
};

//  ------------------------------------------------------------------

#endif // ifndef __gusrwcat_h
//  ------------------------------------------------------------------
