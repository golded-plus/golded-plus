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
//  Ezycom msgbase handling
//  ------------------------------------------------------------------

#include <gdbgtrk.h>
#include <gmoezyc.h>


//  ------------------------------------------------------------------

void EzycomArea::raw_scan(int __keep_index)
{

    GFTRK("EzycomRawScan");

    int _wasopen = isopen;
    if(not _wasopen)
    {
        if(ispacked())
        {
            const char* newpath = Unpack(path());
            if(newpath == NULL)
                packed(false);
            set_real_path(newpath ? newpath : path());
        }
        isopen++;
        data_open();
        test_raw_open(__LINE__);
    }

    Msgn->Resize((uint)(filelength(data->fhhdr) / sizeof(EzycHdr)));

    for(uint _count=0; _count<Msgn->Count(); _count++)
        Msgn->at(_count) = _count + 1;

    int _fh = ::sopen(AddPath(wide->userbasepath, "LASTCOMB.BBS"), O_RDONLY|O_BINARY, SH_DENYNO, S_STDRD);
    if(_fh != -1)
    {
        word _lastread;
        lseekset(_fh, wide->userno * (wide->maxmess / 16) * sizeof(EzycLast) +
                 (((board() - 1) / 16) * sizeof(EzycLast) + sizeof(word)) +
                 (board()-1) % 16 * sizeof(word)
                );
        read(_fh, &_lastread, sizeof(word));
        if(_lastread)
            _lastread--;

        uint _active = Msgn->Count();
        uint _count = 1;
        uint32_t* _msgnoptr = Msgn->tag;
        uint _lastread_reln = 0;
        uint _firstmsgno = 0;
        uint _lastmsgno = 0;
        uint _lastreadfound = 0;

        if(_active)
        {

            _firstmsgno = Msgn->at(0);
            _lastmsgno = Msgn->at(_active-1);

            while(1)
            {

                // Set lastread pointer
                if((*_msgnoptr >= _lastread) and (_lastread_reln == 0))
                {
                    _lastreadfound = *_msgnoptr;
                    _lastread_reln = _count - (*_msgnoptr != _lastread ? 1 : 0);
                    break;
                }
                if((++_count) > _active)
                    break;
                _msgnoptr++;
            }
        }

        // If the exact lastread was not found
        if(_active and (_lastreadfound != _lastread))
        {

            // Higher than highest or lower than lowest?
            if(_lastread > _lastmsgno)
                _lastread_reln = _active;
            else if(_lastread < _firstmsgno)
                _lastread_reln = 0;
        }

        // Update area data
        lastread = _lastread_reln;
        lastreadentry = _lastreadfound;

        ::close(_fh);
    }

    if(not __keep_index)
        Msgn->Reset();

    if(not _wasopen)
    {
        raw_close();
        data_close();
        if(ispacked())
        {
            CleanUnpacked(real_path());
        }
        isopen--;
    }

    GFTRK(0);
}


//  ------------------------------------------------------------------

void EzycomArea::scan()
{

    GFTRK("EzycomScan");

    raw_scan(true);

    GFTRK(0);
}


//  ------------------------------------------------------------------

void EzycomArea::scan_area()
{

    GFTRK("EzycomScanArea");

    raw_scan(false);

    GFTRK(0);
}


//  ------------------------------------------------------------------

void EzycomArea::scan_area_pm()
{

    GFTRK("EzycomScanArea*M");

    scan_area();

    GFTRK(0);
}


//  ------------------------------------------------------------------
