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
//  Maximus userfile class implementation.
//  ------------------------------------------------------------------

#include <gstrall.h>
#include <gfilutil.h>
#include <gmemdbg.h>
#include <gusrmax.h>
//  ------------------------------------------------------------------
MaximusUser::MaximusUser()
{
    recsize = sizeof(MaxUsers);
    recptr  = new char[recsize];
    throw_new(recptr);
    record = (MaxUsers *)recptr;
    memset(record, 0, recsize);
    name = record->name;
    record->struct_len = (byte)(recsize / 20);
    firstread          = true;
}

//  ------------------------------------------------------------------
MaximusUser::~MaximusUser()
{
    throw_deletearray(recptr);
}

//  ------------------------------------------------------------------
int MaximusUser::isvalid()
{
    return not (record->delflag & MAXIMUS_USERDELETED);
}

//  ------------------------------------------------------------------
int MaximusUser::read()
{
    if(gufh != -1)
    {
        if(firstread)
        {
            firstread = false;

            if(filelength(gufh) >= 180)
            {
                ::read(gufh, record, recsize);
                lseek(gufh, -(long)recsize, SEEK_CUR);
                uint _tmp = record->struct_len ? record->struct_len * 20 : 180;

                if(_tmp != recsize)
                {
                    recsize = _tmp;
                    throw_deletearray(recptr);
                    recptr = new char[recsize];
                    throw_new(recptr);
                    record = (MaxUsers *)recptr;
                    name   = record->name;
                }
            }
        }

        ::read(gufh, record, recsize);

        if(isvalid())
        {
            index    = record->lastread_ptr;
            maxindex = maximum_of_two(index, maxindex);
            return true;
        }
    }

    return false;
} // MaximusUser::read

//  ------------------------------------------------------------------
void MaximusUser::founduser()
{
    index = record->lastread_ptr;
    found = true;
}

//  ------------------------------------------------------------------
void MaximusUser::recinit(const char * __name)
{
    GUser::recinit(__name);
    record->lastread_ptr = (word)(++maxindex);
    record->struct_len   = (byte)(recsize / 20);
    record->delflag      = MAXIMUS_USERPERMANENT;
    record->priv         = MAXIMUS_PRIV_TWIT;
}

//  ------------------------------------------------------------------
