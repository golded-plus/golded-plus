
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
//  Advanced search functions.
//  ------------------------------------------------------------------
#ifndef __GESRCH_H
#define __GESRCH_H

#include <gsrchmgr.h>


//  ------------------------------------------------------------------

class golded_search_manager : public search_manager
{

public:

    bool reverse;
    int direction;

    golded_search_manager();
    ~golded_search_manager();

    void prepare_from_string(const char* prompt, int what);

    bool search(GMsg* msg, bool quick, bool shortcircuit);

};


//  ------------------------------------------------------------------

class search_mgr_form : public gwinput2
{

public:

    enum
    {
        id_direction = 100,
        id_messages,
        id_action,
        id_areas
    };

    search_mgr_form(gwindow& w) : gwinput2(w) { }
    virtual ~search_mgr_form() { }

    bool validate();

    void select(gstrarray& menu, const char* title);

    void before();
    void after();
};


//  ------------------------------------------------------------------

#endif
