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
//  Area structures and classes.
//  ------------------------------------------------------------------

#include <gmoarea.h>
//  ------------------------------------------------------------------
glog * WideLog             = NULL;
int WideDebug              = false;
int WideCanLock            = true;
const char ** WideUsername = NULL;
int WideUsernames          = 0;
uint WideSharemode         = SH_DENYNO;
bool WideDispsoftcr        = true;
int WidePersonalmail       = 0;
//  ------------------------------------------------------------------
gmo_area::gmo_area() : cfg(), cfg2()
{
    Msgn          = NULL;
    PMrk          = NULL;
    lastread      = 0;
    lastreadentry = 0;
    isopen        = false;
}

//  ------------------------------------------------------------------
gmo_area::~gmo_area(){}
//  ------------------------------------------------------------------
