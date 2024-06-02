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
//  AdeptXBBS messagebase engine.
//  ------------------------------------------------------------------

#include <gdbgerr.h>
#include <gmemdbg.h>
#include <gdbgtrk.h>
#include <gmoxbbs.h>
//  ------------------------------------------------------------------
XbbsWide * xbbswide = NULL;
XbbsData * xbbsdata = NULL;
int xbbsdatano      = 0;
XbbsHdr::XbbsHdr() : majorversion(0), minorversion(0), structlen(0), msgnum(0),
    timesread(0), timerecv(0), length(0), start(0), extra1(0), extra2(0), extra3(0),
    origaddr(), destaddr(), cost(0), fflags(0), xflags(0), iflags(0), oflags(0)
{
    from[0]   = NUL;
    to[0]     = NUL;
    subj[0]   = NUL;
    date[0]   = NUL;
    indate[0] = NUL;
}

//  ------------------------------------------------------------------
void XbbsArea::data_open()
{
    wide = xbbswide;
    data = xbbsdata + (xbbsdatano++);
}

//  ------------------------------------------------------------------
void XbbsArea::data_close()
{
    xbbsdatano--;
}

//  ------------------------------------------------------------------
void XbbsArea::raw_close()
{
    GFTRK("XbbsRawClose");

    if(data->fhdata != -1)
    {
        ::close(data->fhdata);
    }

    data->fhdata = -1;

    if(data->fhindex != -1)
    {
        ::close(data->fhindex);
    }

    data->fhindex = -1;

    if(data->fhtext != -1)
    {
        ::close(data->fhtext);
    }

    data->fhtext = -1;

    if(wide->isopen)
    {
        if(wide->isopen == 1)
        {
            if(wide->user->gufh != -1)
            {
                ::close(wide->user->gufh);
                wide->user->gufh = -1;
            }
        }

        wide->isopen--;
    }

    GFTRK(0);
} // XbbsArea::raw_close

//  ------------------------------------------------------------------
int XbbsArea::test_open(const char * __file, int sharemode)
{
    GFTRK("XbbsTestOpen");
    int _fh;
    long _tries = 0;

    if(sharemode == -1)
    {
        sharemode = WideSharemode;
    }

    do
    {
        _fh = ::sopen(__file, O_RDWR | O_BINARY | O_CREAT, sharemode, S_STDRW);

        if(_fh == -1)
        {
            // Tell the world
            if((errno != EACCES) or (PopupLocked(++_tries, false, __file) == false))
            {
                // User requested to exit
                WideLog->ErrOpen();
                raw_close();
                WideLog->printf("! An AdeptXBBS msgbase file could not be opened.");
                WideLog->printf(": %s.", __file);
                WideLog->ErrOSInfo();
                OpenErrorExit();
            }
        }
    }
    while(_fh == -1);

    // Remove the popup window
    if(_tries)
    {
        PopupLocked(0, 0, NULL);
    }

    GFTRK(0);
    return _fh;
} // XbbsArea::test_open

//  ------------------------------------------------------------------
void XbbsArea::raw_open()
{
    GFTRK("XbbsRawOpen");
    data->fhdata  = test_open(AddPath(real_path(), ".Data"));
    data->fhindex = test_open(AddPath(real_path(), ".Index"));
    data->fhtext  = test_open(AddPath(real_path(), ".Text"));
    wide->isopen++;

    if(wide->isopen == 1)
    {
        wide->user->gufh = ::sopen(AddPath(wide->path, "Users"),
                                   O_RDONLY | O_BINARY,
                                   WideSharemode,
                                   S_STDRW);
    }

    GFTRK(0);
}

//  ------------------------------------------------------------------
void XbbsExit()
{
    if(xbbswide)
    {
        delete xbbswide->user;
    }

    throw_release(xbbswide);
    throw_release(xbbsdata);
}

//  ------------------------------------------------------------------
void XbbsInit(const char * path, int userno)
{
    xbbsdata         = (XbbsData *)throw_calloc(3, sizeof(XbbsData));
    xbbswide         = (XbbsWide *)throw_calloc(1, sizeof(XbbsWide));
    xbbswide->path   = path;
    xbbswide->userno = userno;
    xbbswide->user   = new XbbsUser;
    throw_new(xbbswide->user);
    xbbswide->user->gufh = -1;
    xbbswide->fhpmi      = -1;
    xbbswide->pmi        = NULL;
    xbbswide->isopen     = 0;
    const char * _username = WideUsername[0];

    if(xbbswide->userno == -1)
    {
        xbbswide->user->gufh = ::sopen(AddPath(xbbswide->path, "Users"),
                                       O_RDONLY | O_BINARY,
                                       WideSharemode,
                                       S_STDRD);

        if(xbbswide->user->gufh != -1)
        {
            xbbswide->user->find(_username);

            if(not xbbswide->user->found)
            {
                xbbswide->userno = 0;
                //WideLog->printf("* User \"%s\" not found in %sUsers.", _username,
                // xbbswide->path);
                //xbbswide->user->add(_username);
                //WideLog->printf("* Now added with user number %u.", xbbswide->user->index);
            }

            close(xbbswide->user->gufh);
        }

        xbbswide->userno = xbbswide->user->index;
    }
} // XbbsInit

//  ------------------------------------------------------------------
void XbbsArea::open()
{
    GFTRK("XbbsOpen");
    isopen++;

    if(isopen > 2)
    {
        WideLog->ErrTest();
        WideLog->printf("! Trying to open an AdeptXBBS msgbase more than twice.");
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
            const char * newpath = Unpack(path());

            if(newpath == NULL)
            {
                packed(false);
            }

            set_real_path(newpath ? newpath : path());
            isopen++;
        }

        data_open();
        raw_open();
        refresh();
        scan();
    }

    GFTRK(0);
} // XbbsArea::open

//  ------------------------------------------------------------------
void XbbsArea::save_lastread()
{
    GFTRK("XbbsSaveLastread");
    int _fh = ::sopen(AddPath(real_path(), ".lmr"),
                      O_RDWR | O_CREAT | O_BINARY,
                      WideSharemode,
                      S_STDRW);

    if(_fh != -1)
    {
        uint32_t _lastread = Msgn->CvtReln(lastread);
        lseekset(_fh, wide->userno + 1, sizeof(uint32_t));
        write(_fh, &_lastread, sizeof(uint32_t));
        ::close(_fh);
    }

    GFTRK(0);
}

//  ------------------------------------------------------------------
void XbbsArea::close()
{
    GFTRK("XbbsClose");

    if(isopen)
    {
        if(isopen == 1)
        {
            save_lastread();
            raw_close();
            Msgn->Reset();
            throw_release(data->idx);
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
        WideLog->printf("! Trying to close an already closed AdeptXBBS msgbase.");
        WideLog->printf(": %s, %s.", echoid(), path());
        WideLog->printf("+ Info: This indicates a potentially serious bug.");
        WideLog->printf("+ Advice: Report to the Author immediately.");
        TestErrorExit();
    }

    GFTRK(0);
} // XbbsArea::close

//  ------------------------------------------------------------------
void XbbsArea::suspend()
{
    GFTRK("XbbsSuspend");
    save_lastread();
    GFTRK(0);
}

//  ------------------------------------------------------------------
void XbbsArea::resume()
{
    GFTRK("XbbsResume");
    GFTRK(0);
}

//  ------------------------------------------------------------------
