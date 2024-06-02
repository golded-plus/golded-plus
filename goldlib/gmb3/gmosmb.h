//  This may look like C code, but it is really -*- C++ -*-
//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 2000 Alexander S. Aganichev
//  ------------------------------------------------------------------
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License as
//  published by the Free Software Foundation; either version 2 of the
//  License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//  ------------------------------------------------------------------
//  $Id$
//  ------------------------------------------------------------------
//  Based on freeware sources from Digital Dynamics
//  ------------------------------------------------------------------
//  Synchronet message base
//  ------------------------------------------------------------------

#ifndef __GMOSMB_H
#define __GMOSMB_H
//  ------------------------------------------------------------------

#include <smblib.h>
#include <gmoarea.h>
//  ------------------------------------------------------------------
class SMBArea : public gmo_area
{
protected: smb_t * data;
    void data_open();
    void data_close();
    void raw_scan(bool keep_index = false, bool scanpm = false);
    int load_hdr(gmsg * __msg, smbmsg_t * msg);

public: SMBArea()
    {
        data = NULL;
    }

    virtual ~SMBArea()
    {}
virtual bool issoftdelete() const
{
    return true;
}

    //  ----------------------------------------------------------------
    //  Messagebase member functions
    void open();
    void close();
    void suspend();
    void resume();
    void lock();
    void unlock();
    void scan();
    void scan_area();
    void scan_area_pm();

int load_hdr(gmsg * msg)
{
    return load_hdr(msg, NULL);
}

    int load_msg(gmsg * msg);
    void save_hdr(int mode, gmsg * msg);
    void save_msg(int mode, gmsg * msg);
    void del_msg(gmsg * msg);
    void new_msgno(gmsg * msg);
    char * user_lookup(char * lookfor);
    int renumber();
    void update_timesread(gmsg * msg);
    Line * make_dump_msg(Line *& lin, gmsg * msg, char * lng_head);
};
//  ------------------------------------------------------------------
extern smb_t * smbdata;
extern int smbdatano;
//  ------------------------------------------------------------------

#endif // ifndef __GMOSMB_H
//  ------------------------------------------------------------------
