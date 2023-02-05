
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
//  Userbase (Addressbook) functions.
//  ------------------------------------------------------------------

#include <cerrno>
#include <fcntl.h>
#include <golded.h>
#include <geusrbse.h>
#include <gftnnl.h>
#include <gstrall.h>


//  ------------------------------------------------------------------

guserbase::guserbase()
{

    long tries = 0;

    strcpy(fname, AddPath(CFG->goldpath, CFG->golduser));

    do
    {
        usrbase.Open(fname, O_RDWR|O_CREAT|O_BINARY, SH_DENYNO, S_STDRW);
        if (!usrbase.isopen())
        {
            if((errno != EACCES) or (not PopupLocked(++tries, false, fname)))
            {
                WideLog->ErrOpen();
                WideLog->printf("! Addressbook cannot be opened.");
                WideLog->printf(": %s", fname);
                WideLog->ErrOSInfo();
                OpenErrorExit();
            }
        }
    }
    while(!usrbase.isopen());

    if(tries)
        PopupLocked(0, 0, NULL);

    if (uint(usrbase.FileLength()) < sizeof(gusrbaseheader) + sizeof(gusrbaseentry))
    {
        header.version = 0;

        strcpy(entry.macro, "_asa_");
        strcpy(entry.name, "Alexander S. Aganichev");
        entry.fidoaddr.reset();
        entry.fidoaddr.zone = 2;
        entry.fidoaddr.net  = 5020;
        entry.fidoaddr.node = 201;
        entry.fidoaddr.point = 58;
        strcpy(entry.iaddr, "aaganichev@yandex.ru");
        entry.prefer_internet = YES;
        entry.is_deleted = NO;
        strcpy(entry.pseudo, "");
        strcpy(entry.organisation, "");
        strcpy(entry.snail1, "Zelenograd");
        strcpy(entry.snail2, "Moscow");
        strcpy(entry.snail3, "Russia");
        entry.dataphone[0] = NUL;
        strcpy(entry.voicephone, "+7-(095)-535-10-42");
        entry.faxphone[0] = NUL;
        entry.firstdate = entry.lastdate = entry.times = 0;
        strcpy(entry.homepage, "http://aaganichev.narod.ru");
        entry.group = 0;
        strcpy(entry.comment1, "GoldED+ Maintainer");
        entry.comment2[0] = NUL;
        entry.comment3[0] = NUL;

        usrbase.LseekSet(0);
        usrbase.Write(&header.version, sizeof(header.version));
        write_entry(0);
    }

    index = 0;
    read_time = 0;  // Indicate that we're should reread timestamp
    refresh_maximum_index();
    need_update = false;
}


//  ------------------------------------------------------------------

guserbase::~guserbase()
{

    usrbase.Close();
}


//  ------------------------------------------------------------------

void guserbase::refresh_maximum_index()
{

    // Are we doing it for the first time?
    if (not read_time)
    {
        usrbase.GetFTime(&read_time);
        need_update = true;
    }
    else
    {
        dword tmp;
        usrbase.GetFTime(&tmp);
        if (read_time != tmp)
        {
            read_time = tmp;
            need_update = true;
        }
    }
    if (need_update)
        maximum_index = (uint(usrbase.FileLength()) - sizeof(gusrbaseheader)) / sizeof(gusrbaseentry) - 1;

    if (index > maximum_index)
        index = maximum_index;
}


//  ------------------------------------------------------------------

void guserbase::lock()
{

    if(WideCanLock)
    {

        long tries = 0;

        do
        {
            usrbase.Lock(0, 1);
            if (!usrbase.okay())
            {
                if(not PopupLocked(++tries, false, fname))
                {
                    WideLog->ErrLock();
                    WideLog->printf("! GoldED's Addressbook could not be locked.");
                    WideLog->printf(": %s", fname);
                    WideLog->ErrOSInfo();
                    LockErrorExit();
                }
            }
        }
        while(!usrbase.okay());

        if(tries)
            PopupLocked(0, 0, NULL);
    }
}


//  ------------------------------------------------------------------

void guserbase::unlock()
{
    if (WideCanLock)
        usrbase.Unlock(0, 1);
}


//  ------------------------------------------------------------------

void guserbase::open()
{

    window.openxy(ypos, xpos, ylen+2, xlen+2,  btype, battr, LGREY_|_BLACK);
    cwidth = (xlen-28) / 2;

    window.message(LNG->UserHeaderName, TP_BORD, 3, tattr);
    window.message(LNG->UserHeaderOrg,  TP_BORD, 4+cwidth, tattr);
    window.message(LNG->UserHeaderAka,  TP_BORD, 5+(cwidth*2)/3 + cwidth, tattr);

    center(CFG->displistcursor);
}


//  ------------------------------------------------------------------

void guserbase::close()
{

    window.close();
}


//  ------------------------------------------------------------------

void guserbase::do_delayed()
{

    wscrollbar(W_VERT, maximum_index+1, maximum_index, index);
    update_statuslinef(LNG->UserStatusline, "ST_USERSTATUSLINE", index+1, maximum_index+1, maximum_index-index);
}


//  ------------------------------------------------------------------

void guserbase::print_line(uint idx, uint pos, bool isbar)
{

    CREATEBUFFER(char, buf, MAXCOL);
    const size_t buflen = MAXCOL;
    buf[buflen - 1] = '\0';

    read_entry(idx);

    std::string useraddr;
    if(AA->isinternet() or not entry.fidoaddr.valid())
    {
        if(*entry.iaddr)
        {
            ((useraddr = "<") + entry.iaddr) + ">";
        }
    }
    else
    {
        if(entry.fidoaddr.valid())
        {
            entry.fidoaddr.make_string(useraddr);
            useraddr.insert(useraddr.begin(),'(');
            useraddr.append(")");

        }
    }

    gsprintf(PRINTF_DECLARE_BUFFER_AUTO(buf, MAXCOL), "%c %-*.*s %-*.*s %s ",
             entry.is_deleted ? 'D' : ' ',
             cwidth, (int)cwidth, entry.name,
             (cwidth*2)/3, (int)(cwidth*2)/3, entry.organisation,
             useraddr.c_str() );


    std::string line_to_print(buf);
    line_to_print.resize(xlen,' ');
    window.prints(pos, 0, isbar ? sattr : wattr, line_to_print.c_str());
}


//  ------------------------------------------------------------------

addressbook_form::~addressbook_form() { }
addressbook_form::addressbook_form(gwindow& w) : gwinput2(w) { }
void addressbook_form::after()
{
    gwinput2::after();
}
void addressbook_form::before()
{
    gwinput2::before();
}


//  ------------------------------------------------------------------

bool addressbook_form::validate()
{

    return true;
}


//  ------------------------------------------------------------------

void addressbook_form::LoadForm()
{

    gusrbaseentry& entry = g->entry;

    entry.fidoaddr.make_string(fidoaddr);
    name      = entry.name;
    macro     = entry.macro;
    pseudo    = entry.pseudo;
    iaddr     = entry.iaddr;
    organisation = entry.organisation;
    voicephone= entry.voicephone;
    faxphone  = entry.faxphone;
    dataphone = entry.dataphone;
    group     = entry.group;
    snail1    = entry.snail1;
    snail2    = entry.snail2;
    snail3    = entry.snail3;
    comment1  = entry.comment1;
    comment2  = entry.comment2;
    comment3  = entry.comment3;
    homepage  = entry.homepage;
}


//  ------------------------------------------------------------------

void addressbook_form::SaveForm()
{

    gusrbaseentry& entry = g->entry;
    entry.fidoaddr.reset();
    entry.fidoaddr.set(fidoaddr);
    strcpy(entry.name, name.c_str());
    strcpy(entry.macro, macro.c_str());
    strcpy(entry.pseudo, pseudo.c_str());
    strcpy(entry.iaddr, iaddr.c_str());
    strcpy(entry.organisation, organisation.c_str());
    strcpy(entry.voicephone, voicephone.c_str());
    strcpy(entry.faxphone, faxphone.c_str());
    strcpy(entry.dataphone, dataphone.c_str());
    entry.group = group.c_str()[0];
    strcpy(entry.snail1, snail1.c_str());
    strcpy(entry.snail2, snail2.c_str());
    strcpy(entry.snail3, snail3.c_str());
    strcpy(entry.comment1, comment1.c_str());
    strcpy(entry.comment2, comment2.c_str());
    strcpy(entry.comment3, comment3.c_str());
    strcpy(entry.homepage, homepage.c_str());
}


//  ------------------------------------------------------------------

bool guserbase::edit_entry(uint idx)
{

    gwindow window;
    char tbuf[50];

    const int width = 75;
    const int height = 16;

    window.openxy((MAXROW-height)/2, (MAXCOL-width)/2, height, width, btype, battr, wattr);
    window.shadow(C_SHADOW);

    sprintf(tbuf, " Record <%d> ", idx+1);
    window.title(tbuf, tattr);

    window.prints( 0, 1, wattr, "Full Name :");
    window.prints( 1, 1, wattr, "Macro Name:");
    window.prints( 1,34, wattr, "Nick Name :");
    window.prints( 2, 1, wattr, "Fidonet   :");
    window.prints( 2,34, wattr, "Internet  :");
    window.prints( 3, 1, wattr, "Organisat.:");
    window.horizontal_line(4, 0, width-2, btype, battr);

    window.prints( 5, 1, wattr, "Voice Num.:");
    window.prints( 6, 1, wattr, "Fax Number:");
    window.prints( 7, 1, wattr, "Data Num. :");
    window.prints( 5,34, wattr, "Address(1):");
    window.prints( 6,34, wattr, "Address(2):");
    window.prints( 7,34, wattr, "Address(3):");
    window.prints( 8, 1, wattr, "Group     :");
    window.prints( 8,34, wattr, "Homepage  :");
    window.prints( 9, 1, wattr, "Comment(1):");
    window.prints(10, 1, wattr, "Comment(2):");
    window.prints(11, 1, wattr, "Comment(3):");
    window.horizontal_line(12, 0, width-2, btype, battr);

    window.prints(13, 1, wattr, "First Used:");
    window.prints(13, 27, wattr, "Last Used:");
    window.prints(13, 53, wattr, "Times Used:");

    char dbuf[16];
    time32_t dt = entry.firstdate;

    if (dt)
    {
        struct tm tm;
        ggmtime(&tm, &dt);
        window.prints(13, 13, wattr, strftimei(dbuf, 16, "%d %b %y", &tm));
    }

    dt = entry.lastdate;
    if (dt)
    {
        struct tm tm;
        ggmtime(&tm, &dt);
        window.prints(13, 38, wattr, strftimei(dbuf, 16, "%d %b %y", &tm));
    }

    sprintf(dbuf, "%8u", entry.times);
    window.prints(13, width-11, wattr, dbuf);

    addressbook_form form(window);
    form.g = this;
    form.setup(C_HEADW, C_HEADW, C_HEADE, _box_table(W_BHEAD, 13), true);

    read_entry(idx);
    form.LoadForm();

    form.add_field(addressbook_form::id_name,         0, 13, 59, form.name, sizeof(entry.name));
    form.add_field(addressbook_form::id_macro,        1, 13, 20, form.macro, sizeof(entry.macro));
    form.add_field(addressbook_form::id_pseudo,       1, 46, 26, form.pseudo, sizeof(entry.pseudo));
    form.add_field(addressbook_form::id_fidoaddr,     2, 13, 20, form.fidoaddr, 24);
    form.add_field(addressbook_form::id_iaddr,        2, 46, 26, form.iaddr, sizeof(entry.iaddr));
    form.add_field(addressbook_form::id_organisation, 3, 13, 59, form.organisation, sizeof(entry.organisation));
    form.add_field(addressbook_form::id_voicephone,   5, 13, 20, form.voicephone, sizeof(entry.voicephone));
    form.add_field(addressbook_form::id_faxphone,     6, 13, 20, form.faxphone, sizeof(entry.faxphone));
    form.add_field(addressbook_form::id_dataphone,    7, 13, 20, form.dataphone, sizeof(entry.dataphone));
    form.add_field(addressbook_form::id_group,        8, 13, 20, form.group, sizeof(entry.group)+1);
    form.add_field(addressbook_form::id_snail1,       5, 46, 26, form.snail1, sizeof(entry.snail1));
    form.add_field(addressbook_form::id_snail2,       6, 46, 26, form.snail2, sizeof(entry.snail2));
    form.add_field(addressbook_form::id_snail3,       7, 46, 26, form.snail3, sizeof(entry.snail3));
    form.add_field(addressbook_form::id_homepage,     8, 46, 26, form.homepage, sizeof(entry.homepage));
    form.add_field(addressbook_form::id_comment1,     9, 13, 59, form.comment1, sizeof(entry.comment1));
    form.add_field(addressbook_form::id_comment2,    10, 13, 59, form.comment2, sizeof(entry.comment2));
    form.add_field(addressbook_form::id_comment3,    11, 13, 59, form.comment3, sizeof(entry.comment3));

    form.run(H_EditAdrEntry);
    window.close();

    if(not form.dropped)
        form.SaveForm();

    return not form.dropped;
}


//  ------------------------------------------------------------------

bool guserbase::find_entry(char* name, bool lookup)
{

    if(not strblank(name))
    {
        std::string tmpaddr;
        gusrbaseentry old_entry = entry;
        uint old_index = index;

        refresh_maximum_index();
        usrbase.LseekSet(sizeof(gusrbaseheader));
        for (index=0; index<=maximum_index; index++)
        {
            read_entry(index, &entry);

            if(strieql(name, entry.name))
                return true;
            else if(lookup)
            {
                if(strieql(name, entry.macro) or streql(name, entry.iaddr))
                    return true;
                else
                {
                    entry.fidoaddr.make_string(tmpaddr);
                    if (streql(name, tmpaddr.c_str()))
                        return true;
                }
            }
        }

        index = old_index;
        entry = old_entry;
    }

    return false;
}


//  ------------------------------------------------------------------

void guserbase::write_entry(uint idx, bool updateit)
{
    if (updateit and not entry.is_deleted)
    {
        time32_t a  = gtime(NULL);
        struct tm tp;
        ggmtime(&tp, &a);
        tp.tm_isdst = -1;
        time32_t b  = gmktime(&tp);
        entry.lastdate = a + a - b;

        if(not entry.firstdate)
            entry.firstdate = entry.lastdate;

        entry.times++;
    }

    usrbase.LseekSet(sizeof(gusrbaseheader) + sizeof(gusrbaseentry)*idx);
    usrbase.Write(&entry, sizeof(gusrbaseentry));
}

//  ------------------------------------------------------------------

void guserbase::clear_entry(gusrbaseentry *ent)
{
    memset(ent, 0, sizeof(gusrbaseentry));
}

//  ------------------------------------------------------------------

bool guserbase::read_entry(uint idx, gusrbaseentry *ent)
{
    if (ent == NULL) ent = &entry;
    refresh_maximum_index();

    if (idx > maximum_index)
        return false;
    else
    {
        usrbase.LseekSet(sizeof(gusrbaseheader) + sizeof(gusrbaseentry)*idx);
        usrbase.Read(ent, sizeof(gusrbaseentry));

        return true;
    }
}


//  ------------------------------------------------------------------

void guserbase::pack_addressbook()
{

    long tries = 0;

    lock();
    refresh_maximum_index();
    uint nidx = 0;
    uint nindex = index;
    for(uint idx = 0; idx <= maximum_index; idx++)
    {
        read_entry(idx);
        if(not entry.is_deleted)
        {
            if(nidx != idx)
                write_entry(nidx);
            ++nidx;
        }
        else if(idx < index)
            --nindex;
    }
    index = nindex;
    // zap
    maximum_index = nidx;
    // At least one record should present
    if (maximum_index) --maximum_index;
    usrbase.ChSize((maximum_index + 1) * sizeof(gusrbaseentry) + sizeof(gusrbaseheader));
    usrbase.Close();

    do
    {
        usrbase.Open(fname, O_RDWR|O_CREAT|O_BINARY, SH_DENYNO, S_STDRW);
        if (!usrbase.isopen())
        {
            if((errno != EACCES) or (not PopupLocked(++tries, false, fname)))
            {
                WideLog->ErrOpen();
                WideLog->printf("! GoldED's Addressbook cannot be opened.");
                WideLog->printf(": %s", fname);
                WideLog->ErrOSInfo();
                OpenErrorExit();
            }
        }
    }
    while(!usrbase.isopen());

    if(tries)
        PopupLocked(0, 0, NULL);

    unlock();
}


//  ------------------------------------------------------------------

void guserbase::update_screen(bool force)
{

    refresh_maximum_index();
    if(force or need_update)
    {
        if(position > index)
            center(CFG->displistcursor);
        update();
        do_delayed();
        need_update = false;
    }
}


//  ------------------------------------------------------------------

bool guserbase::handle_key()
{

    gkey kk;

    if(key < KK_Commands)
    {
        key = key_tolower(key);
        kk = SearchKey(key, AddressbookKey, AddressbookKeys);
        if(kk)
            key = kk;
    }

    switch(key)
    {
    case KK_AddressbookQuit:
        aborted = true;
        return false;
    case KK_AddressbookAdd:
    {
        clear_entry(&entry);
        lock();
        refresh_maximum_index();
        uint nidx = maximum_index + 1;
        if(edit_entry(nidx))
        {
            if(nidx == maximum_index + 1)
            {
                maximum_index++;
                if(maximum_position < ylen-1)
                    maximum_position = maximum_index;
            }
            write_entry(nidx);
            index = nidx;
        }
        unlock();
        center(CFG->displistcursor);
    }
    break;
    case KK_AddressbookSelect:
        if(not select_list)
        {
            if(not entry.is_deleted)
            {
                lock();
                if(edit_entry(index))
                {
                    write_entry(index);
                    display_bar();
                }
                unlock();
            }
            break;
        }
        else
        {
            aborted = false;
            return false;
        }
    case KK_AddressbookDelete:
        lock();
        refresh_maximum_index();
        read_entry(index);
        entry.is_deleted = not entry.is_deleted;
        write_entry(index);
        unlock();
        update_screen();
        break;
    case KK_AddressbookPack:
        pack_addressbook();
        index = position = 0;
        update_screen();
        break;

    case Key_Tick:
        CheckTick(0);    // KK_UserQuitNow ???
        break;
    default:
        SayBibi();
    }
    return true;
}


//  ------------------------------------------------------------------

bool guserbase::run(GMsg* msg, bool selectonly)
{

    select_list = selectonly;

    ypos    = selectonly ? 6 : 1;
    xpos    = 0;
    ylen    = MAXROW-3-ypos;
    xlen    = MAXCOL-2;
    btype   = W_BMENU;
    battr   = C_MENUB;
    wattr   = C_MENUW;
    tattr   = C_MENUT;
    sattr   = C_MENUS;
    hattr   = C_MENUQ;
    sbattr  = C_MENUPB;
    helpcat = H_Userbase;
    listwrap  = CFG->switches.get(displistwrap);
    maximum_position = MinV(maximum_index, ylen - 1);

    if(not select_list)
        find_entry(msg->By(), false);

    run_picker();

    if(not aborted)
    {
        read_entry(index);

        // other stuff not yet implemented
    }

    return not aborted;
}


//  ------------------------------------------------------------------

void guserbase::update_addressbook(GMsg* msg, bool reverse, bool force)
{

    Addr fidoaddr;
    IAdr iaddr;
    INam name;

    strcpy(name, (reverse ? msg->By() : msg->To()));
    strcpy(iaddr, (reverse ? (*msg->iorig ? msg->iorig : msg->iaddr) : msg->idest));
    fidoaddr = (reverse ? msg->orig : msg->dest);

    if(not strblank(name))
    {
        if(find_entry(name))
        {
            if(not force)
                if((CFG->addressbookadd == NO) or ((CFG->addressbookadd == YES) and not (AA->isnet() or AA->isemail())))
                    return;
        }
        else
        {

            if(not force)
            {

                // Don't automatically add the user if...
                // 1. It's not allowed
                if((CFG->addressbookadd == NO) or ((CFG->addressbookadd == YES) and not (AA->isnet() or AA->isemail())))
                    return;

                // 2. It's a robotname
                for(gstrarray::iterator n = CFG->robotname.begin(); n != CFG->robotname.end(); n++)
                    if(striinc(n->c_str(), name))
                        return;

                // 3. It's a username
                if(is_user(name))
                    return;

                // 4. It's a WHOTO name
                if(strieql(AA->Whoto(), name))
                    return;

                // 5. It's the mailinglist's sender address
                if(AA->isemail())
                {
                    std::vector<MailList>::iterator z;
                    for(z = CFG->mailinglist.begin(); z != CFG->mailinglist.end(); z++)
                        if(strieql(z->sender, name) or strieql(z->sender, iaddr))
                            return;
                }

                // 6. It's a UUCP name
                if(isuucp(name))
                    return;

                // 7. If it is already an email address
                if(AA->isemail() and strchr(name, '@'))
                    return;


                // 8. User listed in nodelist
                const char *nlname = lookup_nodelist(&fidoaddr);
                if(nlname and strieql(nlname, name))
                    return;

                // 9. It is an addressmacro
                if(not CFG->addressmacro.empty())
                {
                    std::vector<AddrMacro>::iterator n;
                    for(n=CFG->addressmacro.begin(); n != CFG->addressmacro.end(); n++)
                    {
                        if(strieql(name, n->name) or strieql(n->name, iaddr))
                            return;
                        if(iaddr[0] != NUL)
                        {
                            INam buf, buf1, buf2;
                            char *ptr;

                            if(n->name[0] == '@')
                            {
                                strcpy(buf, n->name+1);
                                // UUCP/INTERNET addressing
                                ptr = strchr(buf, '/');
                                if(ptr)
                                {
                                    *ptr++ = NUL;
                                    if(strieql(name, buf))
                                        return;
                                }
                                else
                                    ptr = buf;
                            }
                            else
                            {
                                ptr = n->name;
                            }
                            ParseInternetAddr(ptr, buf1, buf2);
                            if(strieql(iaddr, buf2) or strieql(name, buf1))
                                return;
                        }
                    }
                }
            }

            // Ok, let's add it...
            clear_entry(&entry);

            strxcpy(entry.name, name, sizeof(entry.name));

            refresh_maximum_index();
            index = ++maximum_index;
        }

        if((AA->isinternet() and strblank(iaddr)) or (not AA->isinternet() and not fidoaddr.valid()))
            return;

        // Update address
        if(AA->isinternet() or (not *entry.iaddr and *iaddr))
            strxcpy(entry.iaddr, iaddr, sizeof(entry.iaddr));
        // If iaddr is not empty then we adding entry from gated message
        if(not AA->isinternet() and not *iaddr and (not AA->Internetgate().addr.valid() or (AA->Internetgate().addr != fidoaddr)))
            entry.fidoaddr = fidoaddr;

        lock();
        write_entry(index, not force);
        unlock();
    }
}


//  ------------------------------------------------------------------

bool guserbase::lookup_addressbook(GMsg* msg, char* name, char* aka, bool browser)
{

    bool found = false;

    if(not strblank(name) or browser)
    {
        w_info(LNG->UserWait);

        if((find_entry(name, true) and not entry.is_deleted) or browser)
        {

            if(browser)
            {
                if(not run(msg, true))
                {
                    w_info(NULL);
                    return false;
                }
            }

            found = true;

            if(not entry.fidoaddr.valid() and not strblank(entry.iaddr) and not AA->isinternet())
            {
                // do UUCP addressing
                strcpy(msg->realto, entry.name);
                if(*AA->Internetgate().name)
                    strcpy(name, AA->Internetgate().name);
                else
                    strcpy(name, entry.name);
            }
            else
            {
                strcpy(name, entry.name);
                if(AA->isinternet())
                    strcpy(msg->realto, entry.name);
                else
                    *msg->realto = NUL;
            }

            if(not strblank(entry.pseudo))
                strcpy(msg->pseudoto, entry.pseudo);

            if(aka)
            {
                *aka = NUL;
                if(AA->isinternet())
                {
                    strcpy(aka, entry.iaddr);
                    strcpy(msg->idest, entry.iaddr);
                    strcpy(msg->iaddr, entry.iaddr);
                }
                else
                {
                    entry.fidoaddr.make_string(aka);
                    if(strblank(aka) and not strblank(entry.iaddr))
                    {
                        strcpy(msg->idest, entry.iaddr);
                        strcpy(msg->iaddr, entry.iaddr);
                        if(AA->Internetgate().addr.valid())
                            AA->Internetgate().addr.make_string(aka);
                    }
                    else
                    {
                        *msg->iaddr = NUL;
                        *msg->idest = NUL;
                    }
                }
            }
        }
        w_info(NULL);
        return found ? (aka ? not strblank(aka) : true) : false;
    }
    return false;
}


//  ------------------------------------------------------------------

void guserbase::build_pseudo(GMsg* msg, char* name, char* aka, bool direction)
{

    if(*msg->iaddr and isuucp(name))
    {
        if(direction)
            strcpy(msg->pseudoto, strlword(msg->iaddr, " @."));
        else
            strcpy(msg->pseudofrom, strlword(msg->iorig, " @."));
    }
    else
        strcpy(direction ? msg->pseudoto : msg->pseudofrom, strlword(name, " @."));

    if(find_entry(name, true) and not entry.is_deleted)
    {

        if(not strblank(entry.pseudo))
        {

            if(AA->isinternet())
            {
                if(strcmp(aka, entry.iaddr))
                    return;
            }
            else
            {
                if(entry.fidoaddr.valid())
                {
                    Addr AKA = aka;
                    if(entry.fidoaddr != AKA)
                        return;
                }
            }

            strcpy(direction ? msg->pseudoto : msg->pseudofrom, entry.pseudo);
        }
    }
}


//  ------------------------------------------------------------------

void update_addressbook(GMsg* msg, bool reverse, bool force)
{

    guserbase p;

    p.update_addressbook(msg, reverse, force);
}


//  ------------------------------------------------------------------

bool lookup_addressbook(GMsg* msg, char* name, char* aka, bool browser)
{

    guserbase p;

    bool result = p.lookup_addressbook(msg, name, aka, browser);

    return result;
}


//  ------------------------------------------------------------------

void build_pseudo(GMsg* msg, bool direction)
{

    guserbase p;
    char buf[128];

    if(direction)
        p.build_pseudo(msg, strbtrim(msg->To()), AA->isinternet() ? msg->iaddr : msg->dest.make_string(buf));
    else
        p.build_pseudo(msg, strbtrim(msg->By()), AA->isinternet() ? msg->iorig : msg->orig.make_string(buf), false);
}


//  ------------------------------------------------------------------

void edit_addressbook(GMsg* msg)
{

    guserbase p;

    p.run(msg, false);

}


//  ------------------------------------------------------------------

