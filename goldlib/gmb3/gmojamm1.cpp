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
//  JAM msgbase implementation, initialization.
//  ------------------------------------------------------------------


//  ------------------------------------------------------------------

#include <gdbgerr.h>
#include <gmemdbg.h>
#include <gdbgtrk.h>
#include <gstrall.h>
#include <gcrcall.h>

#include <gmojamm.h>


//  ------------------------------------------------------------------

JamWide* jamwide = NULL;
JamData* jamdata = NULL;
int      jamdatano = 0;


//  ------------------------------------------------------------------

void JamExit()
{

    throw_xrelease(jamwide);
    throw_xrelease(jamdata);
}


//  ------------------------------------------------------------------

void JamInit(const char* jampath, int harddelete, int jamsmapihw)
{

    GFTRK("JamInit");

    jamdata = (JamData*)throw_calloc(3, sizeof(JamData));
    jamwide = (JamWide*)throw_calloc(1, sizeof(JamWide));

    jamwide->jampath = jampath;
    jamwide->harddelete = harddelete;
    jamwide->smapihw = jamsmapihw;

    // Calculate CRC32 of our username for the lastreads
    INam _name;
    jamstrlwr(strcpy(_name, WideUsername[0]));
    jamwide->userid = jamwide->usercrc = strCrc32(_name, NO, CRC32_MASK_CCITT);

    // Enable replies lookahead feature
    jamwide->lookreplies = true;

    GFTRK(0);
}


//  ------------------------------------------------------------------

void JamArea::open()
{

    GFTRK("JamArea::open");

    isopen++;
    if(isopen > 2)
    {
        WideLog->ErrTest();
        WideLog->printf("! Trying to open a JAM msgbase more than twice.");
        WideLog->printf(": %s, %s.", echoid(), path());
        WideLog->printf("+ Info: This indicates a serious bug.");
        WideLog->printf("+ Advice: Report to the Author immediately.");
        TestErrorExit();
    }
    if(isopen == 1)
    {
        if(ispacked())
        {
            isopen--;
            const char* newpath = Unpack(path());
            if(newpath == NULL)
                packed(false);
            set_real_path(newpath ? newpath : path());
            isopen++;
        }
        data_open();
        open_area();
        scan();
    }

    GFTRK(0);
}


//  ------------------------------------------------------------------

void JamArea::save_lastread()
{

    GFTRK("JamArea::save_lastread");

    // Lock area while we are updating
    lock();

    // Update .JLR record
    data->lastrec.lastread = Msgn->CvtReln(lastread);
    if(data->lastrec.lastread > data->lastrec.highread)
        data->lastrec.highread = data->lastrec.lastread;

    // Seek to users lastread position
    lseekset(data->fhjlr, data->lastpos, sizeof(JamLast));

    // Write the .JLR record
    write(data->fhjlr, &data->lastrec, sizeof(JamLast));

    // Update header info record
    data->hdrinfo.modcounter++;
    if(data->hdrinfo.basemsgnum == 0)
        data->hdrinfo.basemsgnum = 1;

    // Seek to beginning of the .JHR
    lseekset(data->fhjhr, 0);

    // Write header info record
    write(data->fhjhr, &data->hdrinfo, sizeof(JamHdrInfo));

    // Update userfile
    if(data->timesposted)
    {

        // Currently not supported ...
        data->timesposted = 0;
    }

    // Unlock area
    unlock();

    GFTRK(0);
}


//  ------------------------------------------------------------------

void JamArea::close()
{

    GFTRK("JamArea::close");

    if(isopen)
    {
        if(isopen == 1)
        {
            if(data->fhjhr != -1)
                save_lastread();
            if(data->islocked)
                unlock();
            raw_close();
            Msgn->Reset();
            data_close();
            if(ispacked())
            {
                CleanUnpacked(real_path());
            }
        }
        isopen--;
    }
    else
    {
        WideLog->ErrTest();
        WideLog->printf("! Trying to close an already closed JAM msgbase.");
        WideLog->printf(": %s, %s.", echoid(), path());
        WideLog->printf("+ Info: This indicates a potentially serious bug.");
        WideLog->printf("+ Advice: Report to the Author immediately.");
        TestErrorExit();
    }

    GFTRK(0);
}


//  ------------------------------------------------------------------

void JamArea::suspend()
{

    GFTRK("JamArea::suspend");

    save_lastread();
    raw_close();

    GFTRK(0);
}


//  ------------------------------------------------------------------

void JamArea::resume()
{

    GFTRK("JamSuspendOff");

    raw_open();

    GFTRK(0);
}


//  ------------------------------------------------------------------

char *jamstrlwr(char *str)
{

    char *p = str;
    while(*p)
    {
        if((*p >= 'A') && (*p <= 'Z'))
            *p = *p - 'A' + 'a';
        ++p;
    }
    return str;
}

//  ------------------------------------------------------------------
