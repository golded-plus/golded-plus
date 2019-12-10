//  This may look like C code, but it is really -*- C++ -*-
//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1999-2000 Alexander S. Aganichev
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
//  Various Windows functions.
//  ------------------------------------------------------------------
//  Clipboard handling donated by Eugene Roshal
//  ------------------------------------------------------------------

#include <cstdlib>
#include <gutlos.h>
#include <windows.h>
#include <mmsystem.h>
//  ------------------------------------------------------------------
int g_send_mci_string(char * string, char * his_buffer)
{
    char our_buffer[BUFFERSIZE], * return_buffer;

    return_buffer = his_buffer ? his_buffer : our_buffer;
    memset(return_buffer, 0, BUFFERSIZE);
    MCIERROR rc = mciSendString(string, return_buffer, BUFFERSIZE, NULL);

    if(rc == 0)
    {
        return 1;
    }
    else
    {
        mciGetErrorString(rc, return_buffer, BUFFERSIZE);
        return 0;
    }
}

//  ------------------------------------------------------------------
