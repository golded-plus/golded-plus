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
//  Userfile base class.
//  ------------------------------------------------------------------

#ifndef __gusrbase_h
#define __gusrbase_h
//  ------------------------------------------------------------------

#include <gdefs.h>
//  ------------------------------------------------------------------
//  Userfile base class
class GUser
{
public:
    //  ----------------------------------------------------------------
    //  Variables
    int gufh;         // File handle
    int found;        // User found
    uint index;       // User index number
    char * name;      // Pointer to name in user record
    uint recno;       // Current user record number
    uint records;     // Total number of user records
    char * recptr;    // Pointer to user record
    uint recsize;     // Size of user records
    //  ----------------------------------------------------------------
    //  Constructor and destructor
    GUser();
    virtual ~GUser() = 0;
    //  ----------------------------------------------------------------
    //  Pure virtual functions
    virtual int isvalid() = 0;
    virtual int read()    = 0;

    //  ----------------------------------------------------------------
    //  Virtual functions
    virtual void founduser();
    virtual void inctimesposted(int __times);
    virtual uint32_t lastread();
    virtual void lastread(uint32_t __lastread);
    virtual void recinit(const char * __name);

    //  ----------------------------------------------------------------
    //  Normal functions
    int moveto(uint __rec);
    int next();
    int prev();
    void seekread();
    void seekwrite();
    int find(const char * __name, char * __result = NULL, int __wildcards = false);
    int findwild(const char * __name, char * __result = NULL);
    void add(const char * __name);

    //  ----------------------------------------------------------------
};

//  ------------------------------------------------------------------

#endif // ifndef __gusrbase_h
//  ------------------------------------------------------------------
