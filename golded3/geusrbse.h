
//  ------------------------------------------------------------------
//  GoldED+
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999-2000 Alexander S. Aganichev
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
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston,
//  MA 02111-1307 USA
//  ------------------------------------------------------------------
//  $Id$
//  ------------------------------------------------------------------
//  GEUSRBSE header file.
//  ------------------------------------------------------------------
#ifndef __GEUSRBSE_H
#define __GEUSRBSE_H

#include <gmnubase.h>
#include <gwinput.h>


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
    #pragma pack(1)
#endif

struct gusrbaseheader
{
    word version;
    byte reserved[4094];
};


struct gusrbaseentry
{

    char     macro[20];
    IAdr     name;
    ftn_addr fidoaddr;
    IAdr     iaddr;
    byte     prefer_internet;
    byte     is_deleted;

    char   pseudo[40];
    char   organisation[78];
    char   snail1[78];
    char   snail2[78];
    char   snail3[78];

    char   dataphone[40];
    char   voicephone[40];
    char   faxphone[40];

    uint32_t firstdate;
    uint32_t lastdate;
    uint32_t times;

    char   homepage[128];
    byte   group;

    char   comment1[78];
    char   comment2[78];
    char   comment3[78];

    byte   reserved[915];
};


#if defined(GOLD_CANPACK)
    #pragma pack()
#endif


//  ------------------------------------------------------------------

class guserbase;

class addressbook_form : public gwinput2
{

    std::string macro;
    std::string name;
    std::string fidoaddr;
    std::string iaddr;
    std::string pseudo;
    std::string organisation;
    std::string snail1;
    std::string snail2;
    std::string snail3;
    std::string dataphone;
    std::string voicephone;
    std::string faxphone;
    std::string homepage;
    std::string group;
    std::string comment1;
    std::string comment2;
    std::string comment3;

    bool validate();
    void before();
    void after();

    void LoadForm();
    void SaveForm();

    enum
    {
        id_macro,
        id_name,
        id_fidoaddr,
        id_iaddr,
        id_prefer_internet,
        id_pseudo,
        id_organisation,
        id_snail1,
        id_snail2,
        id_snail3,
        id_dataphone,
        id_voicephone,
        id_faxphone,
        id_firstdate,
        id_lastdate,
        id_times,
        id_homepage,
        id_group,
        id_comment1,
        id_comment2,
        id_comment3
    };

    friend class guserbase;

public:
    guserbase* g;

    addressbook_form(gwindow& w);
    virtual ~addressbook_form();

    void select();
};


//  ------------------------------------------------------------------

class guserbase : public gwinpick
{

private:

    gwindow               window;
    gfile                 usrbase;
    gusrbaseheader        header;
    gusrbaseentry         entry;
    uint                  cwidth;
    bool                  select_list;
    bool                  need_update;
    dword                 read_time;
    Path                  fname;

    void open();
    void close();
    void lock();
    void unlock();
    void refresh_maximum_index();
    void print_line(uint idx, uint pos, bool isbar);
    void do_delayed();
    bool handle_key();

    bool edit_entry(uint idx);
    void clear_entry(gusrbaseentry *ent);
    bool read_entry(uint idx, gusrbaseentry *ent=NULL);
    void write_entry(uint idx, bool updateit = false);
    bool find_entry(char* name, bool lookup = false);
    void pack_addressbook();
    void update_screen(bool force = true);

    friend class addressbook_form;

public:

    bool lookup_addressbook(GMsg* msg, char* name, char* aka, bool browser);
    void build_pseudo(GMsg* msg, char* name, char* aka, bool direction = true);
    void update_addressbook(GMsg* msg, bool reverse = false, bool force = false);
    bool run(GMsg* msg, bool selectonly);


    guserbase();
    ~guserbase();
};


//  ------------------------------------------------------------------
#endif
