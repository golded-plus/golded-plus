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

#if defined (GOLD_CANPACK)
    #pragma pack(1)
#endif
//  ------------------------------------------------------------------
struct QWKHdr
{
    byte status;
    char msgno[7];
    char date[8];
    char time[5];
    char to[25];
    char from[25];
    char subject[25];
    char password[12];
    char refno[8];
    char blocks[6];
    byte activestatus;
    word confno;
    word pktmsgno;
    char tagline;
};

//  ------------------------------------------------------------------

#if defined (GOLD_CANPACK)
    #pragma pack()
#endif
//  ------------------------------------------------------------------
