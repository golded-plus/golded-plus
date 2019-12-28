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
//  Nodelist Index Base Class.
//  ------------------------------------------------------------------

#ifndef __gftnnl_h
#define __gftnnl_h


//  ------------------------------------------------------------------

#include <gftnall.h>


//  ------------------------------------------------------------------
//  Nodelist entry data structure

class ftn_nodelist_entry
{

public:

    ftn_addr addr;           // FTN 4D address, binary form
    char     address[30];    // FTN address, string form
    char     name[80];       // Name of the node (sysop)
    char     status[20];     // Status of the node
    char     system[80];     // System name
    char     location[80];   // System location
    char     phone[40];      // Phone number
    char     baud[20];       // Modem baud rate
    char     flags[80];      // Flags (everything after the baud string)

    void     unpack(char* line);

    ftn_nodelist_entry& operator=(const ftn_nodelist_entry& e);

};


//  ------------------------------------------------------------------
//  Nodelist index base class

class ftn_nodelist_index_base
{

protected:

    ftn_nodelist_entry  data;             // Nodelist data for current node
    const char*         nlpath;           // Path to the nodelist files
    bool                exactmatch;       // true if an exact match was found
    bool                isopen;           // true if index is open and ready
    bool                namebrowse;       // true if browsing names
    long                indexmax;         // Maximum index value, if possible
    long                indexpos;         // Current index position, if possible

    virtual bool        search() = 0;

public:

    ftn_nodelist_index_base()
    {
        indexmax = indexpos = 0;
    }
    virtual             ~ftn_nodelist_index_base()  { }

    virtual bool        can_browse_name() const = 0;
    virtual bool        can_browse_address() const = 0;

    bool                browsing_names() const
    {
        return namebrowse;
    }
    bool                browsing_addresses() const
    {
        return not namebrowse;
    }

    void                set_path(const char* path)
    {
        nlpath = path;
    }

    virtual bool        open() = 0;
    virtual void        close() = 0;

    long                index_max() const
    {
        return indexmax;
    }
    long                index_pos() const
    {
        return indexpos;
    }

    bool                is_open() const
    {
        return isopen;
    }

    virtual bool        find(const char* name) = 0;
    virtual bool        find(const ftn_addr& addr) = 0;

    bool                find_again()
    {
        return search();
    }

    bool                found()
    {
        return exactmatch;
    }

    virtual bool        previous() = 0;
    virtual bool        next() = 0;

    virtual void        first() = 0;
    virtual void        last() = 0;

    virtual void        push_state() = 0;
    virtual void        pop_state() = 0;

    virtual const char* index_name() const = 0;
    virtual const char* nodelist_name() const = 0;

    const ftn_addr&     addrs() const
    {
        return data.addr;
    }
    const char*         address() const
    {
        return data.address;
    }
    const char*         name() const
    {
        return data.name;
    }
    const char*         status() const
    {
        return data.status;
    }
    const char*         system() const
    {
        return data.system;
    }
    const char*         location() const
    {
        return data.location;
    }
    const char*         phone() const
    {
        return data.phone;
    }
    const char*         baud() const
    {
        return data.baud;
    }
    const char*         flags() const
    {
        return data.flags;
    }

    const ftn_nodelist_entry& entry() const
    {
        return data;
    }

};


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
