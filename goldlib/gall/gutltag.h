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
//  Tag number types and set class.
//  ------------------------------------------------------------------

#ifndef __gutltag_h
#define __gutltag_h


//  ------------------------------------------------------------------

#include <gdefs.h>
#include <gfile.h>


//  ------------------------------------------------------------------
//  Constants

const ulong TAGN_INVALID = 0;
const uint RELN_INVALID = 0;

const ulong TAGN_MAX = ((ulong)-1);
const uint RELN_MAX = ((uint)-1);

const int TAGN_CLOSEST = true;
const int TAGN_EXACT   = false;


//  ------------------------------------------------------------------
//  Tag class

class GTag {

public:

  //  ----------------------------------------------------------------
  //  Internal data

  ulong* tag;               // tag array
  uint  tags;               // tags in array
  uint  count;              // fake tags count
  uint  allocated;          // actual allocated tags
  uint  granularity;        // memory allocation optimization

  //  ----------------------------------------------------------------
  //  Constructor and destructor

  GTag();
  ~GTag();

  //  ----------------------------------------------------------------
  //  User functions

  void  Reset();
  void  ResetAll()  { Reset(); count = 0; }

  ulong* Resize(uint __tags);

  ulong* Append(ulong __tagn);
  ulong* Add(ulong __tagn);
  uint Del(ulong __tagn);
  uint DelReln(uint __reln);
  uint DelResize(ulong __tagn);
  void  Sort();
  void  ElimDups();

  ulong CvtReln(uint __reln);
  uint ToReln(ulong __tagn);
  uint ToReln(ulong __tagn, int __closest);

  uint Find(ulong __tagn)  { return ToReln(__tagn); }

  uint Tags() const { return tags; }
  uint Count() const { return count; }
  uint SetCount(uint n)  { tags = count = n; return count; }

  void  Set(uint n, ulong t)  { tag[n] = t; }
  ulong Get(uint n)  { return (tags and (n<tags)) ? tag[n] : TAGN_INVALID; }

  ulong& operator[](uint n)  { return tag[n]; }
  ulong& at(uint n)  { return tag[n]; }

  void Load(gfile& fp);
  void Save(gfile& fp);

  //  ----------------------------------------------------------------
};


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
