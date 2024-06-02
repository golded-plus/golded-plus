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
//  AdeptXBBS userfile class implementation.
//  ------------------------------------------------------------------

#include <gmemdbg.h>
#include <gstrall.h>
#include <gusrxbbs.h>
//  ------------------------------------------------------------------
XbbsUser::XbbsUser()
{
    recsize = sizeof(XbbsUsers);
    record  = new XbbsUsers;
    throw_new(record);
    recptr = (char *)record;
    name   = record->name;
}

//  ------------------------------------------------------------------
XbbsUser::~XbbsUser()
{
    throw_delete(record);
}

//  ------------------------------------------------------------------
int XbbsUser::isvalid()
{
    return not (record->attribs & U_DELETED);
}

//  ------------------------------------------------------------------
int XbbsUser::read()
{
    if(gufh != -1)
    {
        ::read(gufh, record, sizeof(XbbsUsers));
        return isvalid();
    }

    return false;
}

//  ------------------------------------------------------------------
void XbbsUser::recinit(const char * __name)
{
    GUser::recinit(__name);
}

//  ------------------------------------------------------------------
void XbbsUser::add(const char *)
{
}

//  ------------------------------------------------------------------
