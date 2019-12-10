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
//  FIDOUSER.LST Indexing.
//  ------------------------------------------------------------------

#ifndef __GFTNNLFU_H
#define __GFTNNLFU_H
//  ------------------------------------------------------------------

#include <gftnnl.h>
//  ------------------------------------------------------------------
class ftn_fidouser_nodelist_index : public ftn_nodelist_index_base
{
    int fh;
    char nodebuf[63];
    long node;
    long maxnode;
    long statenode;
    char searchname[80];
    bool getnode();
    int namecmp() const;

int addrcmp() const
{
    return -1;
}

void compare()
{
    exactmatch = not namecmp();
}

    bool search();

public: ftn_fidouser_nodelist_index();
    virtual ~ftn_fidouser_nodelist_index();
bool can_browse_name() const
{
    return true;
}

bool can_browse_address() const
{
    return false;
}

    bool open();
    void close();
    bool find(const char * name);

bool find(const ftn_addr &)
{
    return false;
}

    bool previous();
    bool next();
    void first();
    void last();
    void push_state();
    void pop_state();
    const char * index_name() const;
    const char * nodelist_name() const;
};

//  ------------------------------------------------------------------

#endif // ifndef __GFTNNLFU_H
//  ------------------------------------------------------------------
