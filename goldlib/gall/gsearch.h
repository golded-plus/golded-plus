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
//  Search.
//  ------------------------------------------------------------------

#ifndef __gsearch_h
#define __gsearch_h


//  ------------------------------------------------------------------

#include <string>


//  ------------------------------------------------------------------

class gregex;
class gfuzzy;
class gbmh;


//  ------------------------------------------------------------------

class gsearch {

public:

  enum patterntype {
    regex,                // Extended regular expressions
    wildcard,             // Shell-style with wildcards
    fuzzy,                // Fuzzy matching
    plain                 // Plain BMH match
  };

protected:

  class gregex* regexp;
  class gfuzzy* fuzzyp;
  class gbmh*   plainp;

public:

  // Configuration
  string         id;
  string         pattern;
  patterntype    type;
  bool           case_sensitive;
  bool           reverse;
  int            fuzzydegree;
  int            score_success;
  int            score_failure;

  gsearch();
  virtual ~gsearch();

  void set_id(const char* a)          { id = a; }
  void set_type(patterntype a)        { type = a; }
  void set_pattern(const char* a);
  void set_case_sensitive(bool a)     { case_sensitive = a; }
  void set_reverse(bool a)            { reverse = a; }
  void set_fuzzydegree(int a)         { fuzzydegree = a; }
  void set_score_success(int a)       { score_success = a; }
  void set_score_failure(int a)       { score_failure = a; }

  // Result of last search
  bool found;
  int score;

  // Search a string for the pattern.
  // Return true for success, false for failure.
  bool search(const char* string);
  bool search(const string& str);
  bool search(const char* str, int& result);
  bool search(const string& str, int& result);

  gsearch& operator=(const gsearch& a);

};


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
