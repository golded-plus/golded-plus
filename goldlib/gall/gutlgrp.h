//  hey, emacs, treat this like -*- C++ -*- file

//  ------------------------------------------------------------------
//  The Goldware Library.
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 2000 Alex. S. Aganichev
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
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//  ------------------------------------------------------------------
//  $Id$
//  ------------------------------------------------------------------
//  Group/Random System.
//  ------------------------------------------------------------------

#ifndef __gutlgrp_h
#define __gutlgrp_h


//  ------------------------------------------------------------------

#include <map>
#include <vector>
#include <string>


//  ------------------------------------------------------------------

#ifdef __GNUG__
#pragma interface "gutlgrp.h"
#endif

//  ------------------------------------------------------------------

enum {
  GRP_AKA,
  GRP_AKAMATCHING,
  GRP_AREACOPYADDID,
  GRP_AREACOPYDIRECT,
  GRP_AREACOPYTO,
  GRP_AREAFORWARDDIRECT,
  GRP_AREAFREQDIRECT,
  GRP_AREAFREQTO,
  GRP_AREAREPLYDIRECT,
  GRP_AREAREPLYTO,
  GRP_AREAYOUWROTETO,
  GRP_ATTRIBUTES,
  GRP_CTRLINFO,
  GRP_EDITHARDTERM,
  GRP_EDITMIXCASE,
  GRP_EDITREPLYRE,
  GRP_FORCETEMPLATE,
  GRP_INPUTFILE,
  GRP_INTERNETADDRESS,
  GRP_INTERNETGATE,
  GRP_INTERNETMSGID,
  GRP_INTERNETRFCBODY,
  GRP_LOADLANGUAGE,
  GRP_MEMBER,
  GRP_MSGLISTDATE,
  GRP_MSGLISTFAST,
  GRP_MSGLISTFIRST,
  GRP_MSGLISTHEADER,
  GRP_MSGLISTWIDESUBJ,
  GRP_NICKNAME,
  GRP_ORGANIZATION,
  GRP_ORIGIN,
  GRP_OUTPUTFILE,
  GRP_PLAY,
  GRP_QUOTEBUFFILE,
  GRP_QUOTECHARS,
  GRP_QUOTECTRL,
  GRP_QUOTESTRING,
  GRP_QUOTEWRAPHARD,
  GRP_SEARCHFOR,
  GRP_TAGLINE,
  GRP_TAGLINECHAR,
  GRP_TAGLINESUPPORT,
  GRP_TEARLINE,
  GRP_TEMPLATE,
  GRP_TEMPLATEMATCH,
  GRP_TWITMODE,
  GRP_USEFWD,
  GRP_USERNAME,
  GRP_USETZUTC,
  GRP_VIEWHIDDEN,
  GRP_VIEWKLUDGE,
  GRP_VIEWQUOTE,
  GRP_WHOTO,
  GRP_XLATEXPORT,
  GRP_XLATIMPORT,

  GRP_MAX
};


//  ------------------------------------------------------------------
//  Group class

class Grp {

private:

  enum {
    TYPE_BOOL,
    TYPE_CHAR,
    TYPE_INT,
    TYPE_STRING,
    TYPE_OBJECT
  };

  class grp_stock { 

  public:

    int type;
    union {
      bool   bool_item;
      char   char_item;
      int    int_item;
      string *string_item;
      void   *object_item;
    } data;

    grp_stock(bool item) { type = TYPE_BOOL; data.bool_item = item; }
    grp_stock(char item) { type = TYPE_CHAR; data.char_item = item; }
    grp_stock(int item) { type = TYPE_INT; data.int_item = item; }
    grp_stock(const string& item) { type = TYPE_STRING; data.string_item = new string(item); throw_new(data.string_item); }
    grp_stock(void *item) { type = TYPE_OBJECT; data.object_item = item; }
  };

  vector< pair<string, multimap<int, grp_stock> > > container;
  vector< pair<string, multimap<int, grp_stock> > >::iterator currgrp;

public:

  int currgrpno;

  Grp();
  ~Grp();

  void AddGrp(const char* id);
  void AddMbr(const char* id);
  const char* SetGrp(const char* id);

  void AddItm(int __type, bool __data);
  void AddItm(int __type, char __data);
  void AddItm(int __type, int __data);
  void AddItm(int __type, const string& __data);
  void AddItm(int __type, void* __data, int __size);

  int GetItm(int __type, bool& __data, int __no=-1);
  int GetItm(int __type, char& __data, int __no=-1);
  int GetItm(int __type, int& __data, int __no=-1);
  int GetItm(int __type, string& __data, int __no=-1);
  int GetItm(int __type, void* __data, int __size, int __no=-1);
};


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
