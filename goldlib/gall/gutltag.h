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

const uint32_t TAGN_INVALID = 0;
const uint RELN_INVALID = 0;

const uint32_t TAGN_MAX = ((uint32_t)-1);
const uint RELN_MAX = ((uint)-1);

const int TAGN_CLOSEST = true;
const int TAGN_EXACT   = false;


//  ------------------------------------------------------------------
//  Tag class

class GTag {

public:

  //  ----------------------------------------------------------------
  //  Internal data

  uint32_t* tag;               // tag array
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

  uint32_t* Resize(uint __tags);

  uint32_t* Append(uint32_t __tagn);
  uint32_t* Add(uint32_t __tagn);
  uint Del(uint32_t __tagn);
  uint DelReln(uint __reln);
  uint DelResize(uint32_t __tagn);
  void  Sort();
  void  ElimDups();

  uint32_t CvtReln(uint __reln);
  uint ToReln(uint32_t __tagn);
  uint ToReln(uint32_t __tagn, int __closest);

  uint Find(uint32_t __tagn)  { return ToReln(__tagn); }

  uint Tags() const { return tags; }
  uint Count() const { return count; }
  uint SetCount(uint n)  { tags = count = n; return count; }

  void  Set(uint n, uint32_t t)  { tag[n] = t; }
  uint32_t Get(uint n)  { return (tags and (n<tags)) ? tag[n] : TAGN_INVALID; }

  uint32_t& operator[](uint n)  { return tag[n]; }
  uint32_t& at(uint n)  { return tag[n]; }

  void Load(gfile& fp);
  void Save(gfile& fp);

  //  ----------------------------------------------------------------
};


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
