
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
//  Advanced search functions.
//  ------------------------------------------------------------------

#include <golded.h>
#include <gmnubase.h>
#include <geval.h>
#include <gesrch.h>


//  ------------------------------------------------------------------

golded_search_manager::golded_search_manager()
{

}


//  ------------------------------------------------------------------

golded_search_manager::~golded_search_manager()
{

}


//  ------------------------------------------------------------------

inline void search_item_option(bool& a, const char* s)
{

    if(s[1] == '^')
        a = not a;
    else
        a = s[1] != '\'';
}


//  ------------------------------------------------------------------

const char* search_item_set(search_item& item, const char* s, int what)
{
    bool flag = false;

    while(*s)
    {
        switch(*s)
        {
        case ')':
            search_item_option(item.where.from, s);
            search_item_option(item.where.to, s);
            search_item_option(item.where.subject, s);
            flag = true;
            break;
        case '(':
            search_item_option(item.where.body, s);
            search_item_option(item.where.tagline, s);
            search_item_option(item.where.tearline, s);
            search_item_option(item.where.origin, s);
            search_item_option(item.where.signature, s);
            search_item_option(item.where.kludges, s);
            flag = true;
            break;
        case '!':
            search_item_option(item.reverse, s);
            break;
        case '=':
            search_item_option(item.case_sensitive, s);
            break;
        case '<':
            search_item_option(item.where.from, s);
            flag = true;
            break;
        case '>':
            search_item_option(item.where.to, s);
            flag = true;
            break;
        case ':':
            search_item_option(item.where.subject, s);
            flag = true;
            break;
        case '#':
            search_item_option(item.where.body, s);
            flag = true;
            break;
        case '.':
            search_item_option(item.where.tagline, s);
            flag = true;
            break;
        case '_':
            search_item_option(item.where.tearline, s);
            flag = true;
            break;
        case '*':
            search_item_option(item.where.origin, s);
            flag = true;
            break;
        case '@':
            search_item_option(item.where.signature, s);
            flag = true;
            break;
        case '%':
            search_item_option(item.where.kludges, s);
            flag = true;
            break;
        case '?':
            s++;
            switch(g_tolower(*s))
            {
            case 'r':
                item.type = gsearch::regex;
                break;
            case 'w':
                item.type = gsearch::wildcard;
                break;
            case 'p':
                item.type = gsearch::plain;
                break;
            case 'f':
                item.type = gsearch::fuzzy;
                if(isdigit(s[1]))
                {
                    item.fuzzydegree = atoi(s+1);
                    while(isdigit(s[1]))
                        s++;
                }
                break;
            }
            break;
        case '^':
        case '\'':
            break;
        default:
            if (isspace(*s)) break;
            else
                goto lblLoopExit;
        }
        s++;
    }

lblLoopExit:
    if (what && !flag)
    {
        if (what & GFIND_FROM)      item.where.from      = true;
        if (what & GFIND_TO)        item.where.to        = true;
        if (what & GFIND_SUBJECT)   item.where.subject   = true;
        if (what & GFIND_BODY)      item.where.body      = true;
        if (what & GFIND_TAGLINE)   item.where.tagline   = true;
        if (what & GFIND_TEARLINE)  item.where.tearline  = true;
        if (what & GFIND_ORIGIN)    item.where.origin    = true;
        if (what & GFIND_KLUDGES)   item.where.kludges   = true;
        if (what & GFIND_SIGNATURE) item.where.signature = true;
    }

    return s;
}


//  ------------------------------------------------------------------

void golded_search_manager::prepare_from_string(const char* prompt, int what)
{
    // Get defaults
    reverse = false;
    direction = DIR_NEXT;

    const char* p = prompt;

    if (*p == '-' or *p == '+')
    {
        direction = (*p == '-') ? DIR_PREV : DIR_NEXT;
        p++;
    }

    search_item default_item;
    p = search_item_set(default_item, p, what);

    search_item item = default_item;

    default_item.reverse         = false;
    default_item.where.from      = false;
    default_item.where.to        = false;
    default_item.where.subject   = false;
    default_item.where.body      = false;
    default_item.where.tagline   = false;
    default_item.where.tearline  = false;
    default_item.where.origin    = false;
    default_item.where.kludges   = false;
    default_item.where.signature = false;

    char buf[256];
    char* b = buf;
    bool item_complete = false;

    do
    {

        switch(*p)
        {

        // Logic AND
        case '&':
            item.logic = search_item::logic_and;
            item_complete = true;
            p++;
            break;

        // Logic OR
        case '|':
            item.logic = search_item::logic_or;
            item_complete = true;
            p++;
            break;

        // Get quoted string
        case '\"':
        case '\'':
        {
            char q = *p++;
            while(*p)
            {
                if(*p == q)
                {
                    p++;
                    break;
                }
                switch(*p)
                {
                case '\\':
                    if(*(++p))
                        *b++ = *p++;
                    break;
                default:
                    *b++ = *p++;
                }
            }
        }
        break;

        // Get literal escaped character
        case '\\':
            if(*(++p))
                *b++ = *p++;
            break;

        // Skip whitespace
        case ' ':
            p++;

        case NUL:
            break;

        default:
            *b++ = *p++;
        }

        if(item_complete or (*p == NUL))
        {
            item_complete = false;
            *b = NUL;
            if(*buf != NUL)
            {
                item.pattern = buf;
                items.push_back(item);
            }
            if(*p == NUL)
                break;

            item = default_item;
            p = search_item_set(item, strskip_wht(p), what);
            b = buf;
        }

    }
    while(*p);
}


//  ------------------------------------------------------------------

bool golded_search_manager::search(GMsg* msg, bool quick, bool shortcircuit)
{

    std::vector<search_item>::iterator item = items.begin();
    bool exit = false;
    bool and_cycle = false;
    bool or_cycle = false;

    for(int i=0; i<items.size(); i++, item++)
    {

        if(shortcircuit)
        {
            if(item->logic == search_item::logic_and)
            {
                if(not and_cycle)
                {
                    if(exit)
                        return true;
                }
                else if(not exit)
                    continue;
                and_cycle = true;
            }
            else
            {
                if(and_cycle)
                {
                    and_cycle = (item->logic == search_item::logic_and);
                    if(not exit)
                        continue;
                }
                else
                {
                    and_cycle = (item->logic == search_item::logic_and);
                    if(exit)
                        return true;
                }
                or_cycle = (item->logic == search_item::logic_or);
            }
        }

        int found = 0;
        if(item->where.from)
        {
            if(item->search(*msg->ifrom ? msg->ifrom : msg->By()))
            {
                msg->foundwhere |= GFIND_FROM;
                found++;
                if(quick)
                    goto quick_found;
            }
        }
        if(item->where.to)
        {
            if(item->search(*msg->ito ? msg->ito : msg->to))
            {
                msg->foundwhere |= GFIND_TO;
                found++;
                if(quick)
                    goto quick_found;
            }
            else if(*msg->icc and item->search(msg->icc))
            {
                msg->foundwhere |= GFIND_TO;
                found++;
                if(quick)
                    goto quick_found;
            }
            else if(*msg->ibcc and item->search(msg->ibcc))
            {
                msg->foundwhere |= GFIND_TO;
                found++;
                if(quick)
                    goto quick_found;
            }
        }
        if(item->where.subject)
        {
            if(item->search(msg->re))
            {
                msg->foundwhere |= GFIND_SUBJECT;
                found++;
                if(quick)
                    goto quick_found;
            }
        }
        if(item->where.body or item->where.tagline or item->where.tearline or item->where.origin or item->where.signature or item->where.kludges)
        {
            Line* line = msg->lin;
            while(line)
            {
                uint type = line->type;
                bool search_this_line = false;
                if(item->where.body and not (type & (GLINE_TAGL|GLINE_TEAR|GLINE_ORIG|GLINE_SIGN|GLINE_KLUDGE)))
                    search_this_line = true;
                if(item->where.tagline and (type & GLINE_TAGL))
                    search_this_line = true;
                if(item->where.tearline and (type & GLINE_TEAR))
                    search_this_line = true;
                if(item->where.origin and (type & GLINE_ORIG))
                    search_this_line = true;
                if(item->where.signature and (type & GLINE_SIGN))
                    search_this_line = true;
                if(item->where.kludges and (type & GLINE_KLUDGE))
                    search_this_line = true;
                if(search_this_line)
                {
                    if(item->search(line->txt.c_str()))
                    {
                        line->type |= GLINE_HIGH;
                        if(type & (GLINE_TAGL|GLINE_TEAR|GLINE_ORIG|GLINE_SIGN|GLINE_KLUDGE))
                        {
                            if(type & GLINE_TAGL)
                                msg->foundwhere |= GFIND_TAGLINE;
                            else if(type & GLINE_TEAR)
                                msg->foundwhere |= GFIND_TEARLINE;
                            else if(type & GLINE_ORIG)
                                msg->foundwhere |= GFIND_ORIGIN;
                            else if(type & GLINE_SIGN)
                                msg->foundwhere |= GFIND_SIGNATURE;
                            else if(type & GLINE_KLUDGE)
                                msg->foundwhere |= GFIND_KLUDGES;
                        }
                        else
                        {
                            msg->foundwhere |= GFIND_BODY;
                        }
                        found++;
                        if(quick)
                            goto quick_found;
                    }
                }
                line = line->next;
            }
        }

quick_found:

        if(item->reverse)
            found = not found;

        // Perform short-circuit logic analysis
        if(shortcircuit)
        {
            exit = make_bool(found);
            if(item == items.end()-1)
                return exit;
        }
        // else save success/failure as score
        else
        {
            item->score = found ? 1 : 0;
        }
    }

    if(not shortcircuit)
    {
        gevalhum logic;
        if(items.size())
        {
            for(int i=0; i<items.size(); i++)
            {
                logic.push_value(items[i].score);
                if((i+1) < items.size())
                {
                    if(items[i].logic == search_item::logic_and)
                        logic.push_operator(geval::logic_and);
                    else
                        logic.push_operator(geval::logic_or);
                }
            }
            return make_bool(logic.evaluate());
        }
    }

    // It should never end up here unless there are no items
    return false;
}


//  ------------------------------------------------------------------

void search_mgr_form::before()
{

    gwinput2::before();
}


//  ------------------------------------------------------------------

void search_mgr_form::after()
{

    gwinput2::after();
}


//  ------------------------------------------------------------------

void search_mgr_form::select(gstrarray& menu, const char* title)
{

    int srow = window.start_row + current->row;
    int scol = window.start_column + current->column;

    if((current->id < id_direction) and ((current->id % 3) == 2))
        scol -= 20;

    GMnu m;
    m.Init();
    m.SetColor(LGREY_|_BLACK, LGREY_|_BLACK, LGREY_|_BLACK, BLACK_|_LGREY, DGREY_|_BLACK);
    m.SetTitle(title, YELLOW_|_BLACK);
    m.SetBorder(0, LGREY_|_BLACK);
    m.SetPos(srow, scol);
    m.SetMask(0);
    m.Begin();
    for(int i=0; i<menu.size(); i++)
        m.Item(i, menu[i].c_str());
    m.End();

    vcurhide();
    m.Start();
    vcurshow();

    if(m.FinalTag() != -1)
    {
        if(not ((current->id < id_direction) and ((current->id % 3) == 2)))
        {
            strbtrim(strcpy(current->buf, menu[m.FinalTag()].c_str()+1));
            current->update();
        }
    }
}


//  ------------------------------------------------------------------

bool search_mgr_form::validate()
{

    gstrarray menu;

    if(current->id >= id_direction)
    {
        switch(current->id)
        {
        case id_direction:
            menu.insert(menu.end(), "F Forward");
            menu.insert(menu.end(), "B Backward ");
            select(menu, "Direction");
            break;
        case id_messages:
            menu.insert(menu.end(), "N New");
            menu.insert(menu.end(), "U Unread ");
            menu.insert(menu.end(), "A All");
            select(menu, "Messages");
            break;
        case id_action:
            menu.insert(menu.end(), "R Read");
            menu.insert(menu.end(), "T Tag");
            menu.insert(menu.end(), "D Delete ");
            menu.insert(menu.end(), "W Write");
            menu.insert(menu.end(), "C Copy");
            menu.insert(menu.end(), "M Move");
            select(menu, "Action");
            break;
        case id_areas:
            menu.insert(menu.end(), "C Current ");
            menu.insert(menu.end(), "A All");
            menu.insert(menu.end(), "T Tagged");
            select(menu, "Areas");
            break;
        }
    }
    else if((current->id % 3) == 1)
    {
        menu.insert(menu.end(), "a and ");
        menu.insert(menu.end(), "o or");
        select(menu, "Logic");
    }
    else if((current->id % 3) == 2)
    {
        menu.insert(menu.end(), " Plain ");
        menu.insert(menu.end(), " Regex ");
        menu.insert(menu.end(), " Wildcard ");
        menu.insert(menu.end(), " Fuzzy ");
        menu.insert(menu.end(), " Success if not found ");
        menu.insert(menu.end(), " Case sensitive ");
        menu.insert(menu.end(), " Search \"from\" ");
        menu.insert(menu.end(), " Search \"to\" ");
        menu.insert(menu.end(), " Search \"subject\" ");
        menu.insert(menu.end(), " Search msg body ");
        menu.insert(menu.end(), " Search tagline ");
        menu.insert(menu.end(), " Search tearline ");
        menu.insert(menu.end(), " Search origin ");
        menu.insert(menu.end(), " Search signature ");
        menu.insert(menu.end(), " Search kludges ");
        select(menu, "Options");
    }

    return true;
}


//  ------------------------------------------------------------------

void AdvancedSearch(GMsg*, int&, int&)
{

    int   patterns = 9;
    int   width = 77;
    int   height = patterns+11;
    int   widths[3] = { 55, 5, 7 };
    int   field_widths[3] = { 100, 5, 7 };
    int   border_type   = BT_SINGLE;
    vattr title_color   = YELLOW_|_BLUE;
    vattr heading_color = YELLOW_|_BLUE;
    vattr window_color  = LGREY_ |_BLUE;
    vattr border_color  = LBLUE_ |_BLUE;
    vattr edit_color    = YELLOW_|_BLUE;
    vattr idle_color    = LGREY_ |_BLUE;
    vattr active_color  = WHITE_ |_BLUE;
    vattr shadow_color  = DGREY_ |_BLACK;

    widths[0] = width - 3*2 - 2 - 5 - 7 - 2;

    gwindow window;

    window.openxy(1, ((MAXCOL-width)/2), height, width,
                  border_type, border_color, window_color);
    window.shadow(shadow_color);

    window.horizontal_line( 1, 0, width-2, border_type, border_color);
    window.horizontal_line(patterns+2, 0, width-2, border_type, border_color);

    window.vertical_line(0, widths[0]+2,           patterns+3, border_type, border_color);
    window.vertical_line(0, widths[0]+widths[1]+5, patterns+3, border_type, border_color);

    window.title(" Advanced Search Manager ", title_color);

    window.prints(0, 1,                     heading_color, "Pattern");
    window.prints(0, widths[0]+4,           heading_color, "Logic");
    window.prints(0, widths[0]+widths[1]+7, heading_color, "Options");

    window.prints(patterns+3, 1, heading_color, "Direction   : ");
    window.prints(patterns+4, 1, heading_color, "Messages    : ");
    window.prints(patterns+5, 1, heading_color, "Action      : ");
    window.prints(patterns+6, 1, heading_color, "Areas       : ");
    window.prints(patterns+7, 1, heading_color, "Destination : ");
    window.prints(patterns+8, 1, heading_color, "Identifier  : ");

    search_mgr_form iform(window);

    iform.setup(idle_color, active_color, edit_color, _box_table(border_type, 13), true);

    std::string buffers[9*3 + 6];

    int i = 0;
    for(int r=0; r<9; r++)
    {
        int cs = 1;
        for(int c=0; c<3; c++,i++)
        {
            char z[80];
            sprintf(z, "%i,%i", r+1, c+1);
            buffers[i] = z;
            iform.add_field(i, r+2, cs, widths[c], buffers[i], field_widths[c]);
            cs += widths[c] + 3;
        }
    }

    buffers[i+0] = "Forward";
    buffers[i+1] = "New";
    buffers[i+2] = "Read";
    buffers[i+3] = "Current";
    buffers[i+4] = "C:\\TEMP\\DUMPFILE.TXT";
    buffers[i+5] = "Testing";

    for(int y=0; y<6; y++,i++)
        iform.add_field(100+y, patterns+3+y, 15, width-15-3, buffers[i], width-15-3, gwinput::cvt_none, y>=4 ? gwinput::entry_conditional : gwinput::entry_noedit);

    iform.run(0);

    window.close();
}


//  ------------------------------------------------------------------

