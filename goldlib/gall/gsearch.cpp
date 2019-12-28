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
//  Search with different methods.
//  ------------------------------------------------------------------

#include <gmemdbg.h>
#include <gsearch.h>
#include <gwildmat.h>
#include <gregex.h>
#include <gfuzzy.h>
#include <gbmh.h>


//  ------------------------------------------------------------------

gsearch::gsearch()
{

    regexp         = NULL;
    fuzzyp         = NULL;
    plainp         = NULL;
    type           = plain;
    case_sensitive = false;
    reverse        = false;
    fuzzydegree    = 1;
    score_success  = 1;
    score_failure  = 0;
    score          = 0;
    found          = false;
}


//  ------------------------------------------------------------------

gsearch::~gsearch()
{

    delete regexp;
    delete fuzzyp;
    delete plainp;
}


//  ------------------------------------------------------------------

void gsearch::set_pattern(const char* a)
{

    delete regexp;
    regexp = NULL;
    delete fuzzyp;
    fuzzyp = NULL;
    delete plainp;
    plainp = NULL;

    pattern = a;
}


//  ------------------------------------------------------------------

bool gsearch::search(const std::string& str)
{

    int discard_result;
    return search(str, discard_result);
}


//  ------------------------------------------------------------------

bool gsearch::search(const std::string& str, int& result)
{

    return search(str.c_str(), result);
}


//  ------------------------------------------------------------------

bool gsearch::search(const char* str)
{

    int discard_result;
    return search(str, discard_result);
}


//  ------------------------------------------------------------------

bool gsearch::search(const char* str, int& result)
{

    found = false;

    switch(type)
    {

    case regex:
        if(regexp == NULL)
        {
            regexp = new gregex;
            throw_new(regexp);
            regexp->compile(pattern.c_str(), gregex::extended | (case_sensitive ? 0 : gregex::icase));
        }
        found = regexp->match(str);
        break;

    case wildcard:
        found = gwildmat(str, pattern.c_str(), not case_sensitive);
        break;

    case fuzzy:
        if(fuzzyp == NULL)
        {
            fuzzyp = new gfuzzy;
            throw_new(fuzzyp);
            fuzzyp->init(pattern.c_str(), fuzzydegree, case_sensitive);
        }
        found = fuzzyp->findfirst(str);
        break;

    case plain:
        if(plainp == NULL)
        {
            plainp = new gbmh;
            throw_new(plainp);
            plainp->init(pattern.c_str(), not case_sensitive);
        }
        found = plainp->find(str);
        break;
    }

    if(reverse)
        score = score_failure;
    else
        score = score_success;
    result = score;
    return found;
}


//  ------------------------------------------------------------------

gsearch& gsearch::operator=(const gsearch& a)
{

    regexp         = NULL;
    fuzzyp         = NULL;
    plainp         = NULL;
    id             = a.id;
    pattern        = a.pattern;
    type           = a.type;
    case_sensitive = a.case_sensitive;
    reverse        = a.reverse;
    score_success  = a.score_success;
    score_failure  = a.score_failure;

    return *this;
}


//  ------------------------------------------------------------------
