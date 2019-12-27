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
//  Function tracker.
//  ------------------------------------------------------------------

#include <gtimall.h>
#include <gdbgtrk.h>
#include <gmemdbg.h>
#include <glog.h>
#include <stdlib.h>

//  ------------------------------------------------------------------

#if defined(GFTRK_ENABLE)


//  ------------------------------------------------------------------
//  Externals

extern glog LOG;


//  ------------------------------------------------------------------
//  Global vars

int __gftrk = false;
int __gftrk_on = true;
int __gftrk_max = 50;
int __gftrk_curr;
int __gftrk_indent;
GFTrk* __gftrk_ptr;
GFTrk* __gftrk_list;

int __gftrk_statusline = false;


//  ------------------------------------------------------------------

static void __gftrk_term(void)
{

    if(__gftrk)
        free(__gftrk_list);
}


//  ------------------------------------------------------------------

void __gftrk_init(int trackmax)
{

    if(trackmax != -1)
        __gftrk_max = trackmax;
    __gftrk_list = (GFTrk*)calloc(__gftrk_max, sizeof(GFTrk));
    if(__gftrk_list)
    {
        __gftrk = true;
        __gftrk_curr = 0;
        __gftrk_indent = 0;
        atexit(__gftrk_term);
    }
}


//  ------------------------------------------------------------------

extern bool cmdlinedebughg;
void update_statusline(const char* info);

void __gftrk_track(const char* text)
{

    if(__gftrk_on)
    {
        if(text)
        {
            __gftrk_ptr = __gftrk_list + __gftrk_curr;
            __gftrk_ptr->tick = clock();
            __gftrk_ptr->text = *text ? text : text+1;
            __gftrk_ptr->indent = __gftrk_indent;
            if(*text)
            {
                __gftrk_indent++;
                if(__gftrk_statusline)
                    update_statusline(text);
                if(cmdlinedebughg)
                    LOG.printf("- %08u  %*s%s", __gftrk_ptr->tick, __gftrk_ptr->indent*2, "", __gftrk_ptr->text);
            }
            __gftrk_curr = (++__gftrk_curr) % __gftrk_max;
        }
        else
        {
            __gftrk_indent--;
        }
        if(__gftrk_statusline)
            THROW_CHECK();
    }
}


//  ------------------------------------------------------------------

void __gftrk_log()
{

    if(__gftrk)
    {
        int _curr = __gftrk_curr - 1;
        if(_curr == -1)
            _curr = __gftrk_max - 1;
        int _count = 0;
        int _first = true;
        while(_count < __gftrk_max)
        {
            GFTrk* _ptr = __gftrk_list + _curr;
            if(_ptr->text)
            {
                if(_first)
                {
                    LOG.printf("! Function track dump follows:");
                    _first = false;
                }
                LOG.printf("- %08u  %*s%s", _ptr->tick, _ptr->indent*2, "", _ptr->text);
            }
            _count++;
            _curr--;
            if(_curr == -1)
                _curr = __gftrk_max - 1;
        }
    }
}


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
