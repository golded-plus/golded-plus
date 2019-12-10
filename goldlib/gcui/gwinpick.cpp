//  This may look like C code, but it is really -*- C++ -*-
//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
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
//  GCUI: Golded+ Character-oriented User Interface.
//  Class gwinpick - Pick engine.
//  ------------------------------------------------------------------

#include <gkbdcode.h>
#include <gmoubase.h>
#include <gtimall.h>
#include <gutlmisc.h>
#include <gwinall.h>
#include <gwinhelp.h>
#include <gwinpick.h>
//  ------------------------------------------------------------------
gwinpick::gwinpick()
{
    // memset(this, 0, sizeof(gwinpick));  WHAT!?!?! ARE YOU NUTS?
    key            = 0;
    keyok          = false;
    ypos           = xpos = ylen = xlen = 0;
    btype          = 0;
    battr          = wattr = tattr = sattr = hattr = loattr = sbattr = BLACK_ | _BLACK;
    title          = NULL;
    helpcat        = 0;
    maximum_index  = minimum_index = maximum_position = index = position = 0;
    aborted        = listwrap = false;
    direction      = 0;
    replylinkfloat = 0;
}

//  ------------------------------------------------------------------
void gwinpick::cursor_up()
{
    bool done;

    do
    {
        if(index > minimum_index)
        {
            display_line();

            if(position)
            {
                position--;
            }
            else
            {
                scroll(SDOWN);
            }

            index--;

            if((done = is_selectable(index)) == true)
            {
                update_cursor();
            }
        }
        else
        {
            if(listwrap)
            {
                cursor_last();
            }
            else if(not is_selectable(index))
            {
                cursor_down();
            }

            done = true;
        }
    }
    while(not done);
    direction = -1;
} // gwinpick::cursor_up

//  ------------------------------------------------------------------
void gwinpick::cursor_down()
{
    bool done;

    do
    {
        if(index < maximum_index)
        {
            display_line();
            index++;

            if(position < maximum_position)
            {
                position++;
            }
            else
            {
                scroll(SUP);
            }

            if((done = is_selectable(index)) != false)
            {
                update_cursor();
            }
        }
        else
        {
            if(listwrap)
            {
                cursor_first();
            }
            else if(not is_selectable(index))
            {
                cursor_up();
            }

            done = true;
        }
    }
    while(not done);
    direction = 1;
} // gwinpick::cursor_down

//  ------------------------------------------------------------------
void gwinpick::cursor_pageup()
{
    uint i = index - position;

    while(not is_selectable(i))
    {
        i++;
    }
    uint min = i + position - index;

    if(position > min)
    {
        display_line();
        index    = i;
        position = min;
        update_cursor();
    }
    else if(index > min)
    {
        i        = maximum_position - min + position;
        index   -= (index >= i) ? i : index;
        position = 0;
        display_page();

        if(not is_selectable(index))
        {
            cursor_down();
        }
    }

    direction = -1;
} // gwinpick::cursor_pageup

//  ------------------------------------------------------------------
void gwinpick::cursor_pagedown()
{
    uint max_index = index + maximum_position - position;

    if(max_index > maximum_index)
    {
        max_index = maximum_index;
    }

    while(not is_selectable(max_index))
    {
        --max_index;
    }
    uint max_position = max_index - index + position;

    if(position < max_position)
    {
        if(index == max_index)
        {
            if(maximum_position < maximum_index)
            {
                position = maximum_position;
                index    = maximum_index;
                display_page();
                update_cursor();
            }
        }
        else if(max_index >= max_position)
        {
            display_line();
            position = max_position;
            index    = max_index;
            update_cursor();
        }
        else if(position != maximum_position)
        {
            display_line();
            index   += maximum_position - position;
            position = maximum_position;
            update_cursor();
        }
    }
    else if(index < maximum_index)
    {
        if(index + maximum_position + (maximum_position - max_position) > maximum_index)
        {
            position = maximum_index - index;
            index    = maximum_index;
        }
        else
        {
            index   += maximum_position + (maximum_position - max_position);
            position = maximum_position;
        }

        display_page();
    }

    if(not is_selectable(index))
    {
        cursor_up();
    }

    direction = 1;
} // gwinpick::cursor_pagedown

//  ------------------------------------------------------------------
void gwinpick::cursor_first()
{
    uint min_select = minimum_index;

    while(not is_selectable(min_select))
    {
        min_select++;
    }

    if(index > min_select)
    {
        if(index <= position)
        {
            display_line();
            index = position = min_select;
            update_cursor();
        }
        else
        {
            index = position = min_select;
            display_page();
        }
    }

    direction = -1;
}

//  ------------------------------------------------------------------
void gwinpick::cursor_last()
{
    uint max_select = maximum_index;

    while(not is_selectable(max_select))
    {
        max_select--;
    }

    if(index < max_select)
    {
        if(index + (maximum_position - position) >= maximum_index)
        {
            display_line();
            position += max_select - index;
            index     = max_select;
            update_cursor();
        }
        else
        {
            position = (maximum_index - index) <
                       maximum_position ? maximum_index - index : maximum_position;
            position -= maximum_index - max_select;
            index     = max_select;
            display_page();
        }
    }
    else if(position < maximum_position)
    {
        if(maximum_position < maximum_index)
        {
            position = maximum_position + max_select - maximum_index;
            index    = max_select;
            display_page();
        }
    }

    direction = 1;
} // gwinpick::cursor_last

//  ------------------------------------------------------------------
void gwinpick::cursor_scroll_up()
{
    uint oldidx = index;

    do
    {
        if(index - position)
        {
            index--;
        }
        else
        {
            while((not is_selectable(index)) and (index < maximum_index))
            {
                index++;
            }
        }
    }
    while(not is_selectable(index));

    if(index != oldidx)
    {
        display_page();
    }

    direction = -1;
} // gwinpick::cursor_scroll_up

//  ------------------------------------------------------------------
void gwinpick::cursor_scroll_down()
{
    uint oldidx = index;

    if(index != maximum_index)
    {
        do
        {
            if(index < maximum_index)
            {
                index++;
            }
            else
            {
                while((not is_selectable(--index)) and (index > minimum_index))
                {}
            }
        }
        while(not is_selectable(index));
    }

    if(index != oldidx)
    {
        display_page();
    }

    direction = 1;
} // gwinpick::cursor_scroll_down

//  ------------------------------------------------------------------
void gwinpick::display_line(bool bar)
{
    print_line(index, position, bar);
}

//  ------------------------------------------------------------------
void gwinpick::display_page()
{
    if(index > position)
    {
        index -= position;
    }
    else
    {
        index = 0;
    }

    register uint m = maximum_index - index;
    uint n;

    if(replylinkfloat)
    {
        if(h_offset == new_hoffset)
        {
            for(n = 0; n <= maximum_position and n <= m; n++)
            {
                if(position == n)
                {
                    print_line(index + n, n, true);
                }
            }
        }

        h_offset = new_hoffset;
    }

    for(n = 0; n <= maximum_position and n <= m; n++)
    {
        print_line(index + n, n, (position == n));
    }

    if(n < ylen)
    {
        wputx(n, 0, battr | ACSET, _box_table(btype, 1), xlen);
    }

    for(++n; n < ylen; n++)
    {
        wputx(n, 0, wattr, (vchar)' ', xlen);
    }
    index += position;
    wgotoxy(position, 0);
} // gwinpick::display_page

//  ------------------------------------------------------------------
void gwinpick::open()
{
    update();
}

void gwinpick::close(){}
void gwinpick::precursor(){}
void gwinpick::do_delayed(){}
void gwinpick::scroll(int direction)
{
    wscroll(1, direction);
}

bool gwinpick::handle_key()
{
    return false;
}

bool gwinpick::is_selectable(uint)
{
    return true;
}

//  ------------------------------------------------------------------
bool gwinpick::default_handle_key()
{
    keyok = true;

    switch(key)
    {
        case Key_Up:
            precursor();
            cursor_up();
            break;

        case Key_Dwn:
            precursor();
            cursor_down();
            break;

        case Key_PgUp:
            precursor();
            cursor_pageup();
            break;

        case Key_PgDn:
            precursor();
            cursor_pagedown();
            break;

        case Key_Home:
            precursor();
            cursor_first();
            break;

        case Key_End:
            precursor();
            cursor_last();
            break;

        case Key_C_Up:
            precursor();
            cursor_scroll_up();
            break;

        case Key_C_Dwn:
            precursor();
            cursor_scroll_down();
            break;

        default:
            keyok = handle_key();
    } // switch
    return keyok;
} // gwinpick::default_handle_key

//  ------------------------------------------------------------------
void gwinpick::update_cursor()
{
    wgotoxy(position, 0);
    display_bar();
//  do_delayed();
}

//  ------------------------------------------------------------------
void gwinpick::center(int listmode)
{
    if(maximum_index <= maximum_position)
    {
        position = index;
        update();
        return;
    }

    uint botroom = maximum_index - index;

    switch(listmode)
    {
        case LIST_TOP:
            position = (botroom > maximum_position) ? 0 : (maximum_position - botroom);
            break;

        case LIST_NEARTOP:
        {
            uint room;
            uint toproom = index;

            if(toproom > (maximum_position / 4))
            {
                if(botroom > (maximum_position / 4))
                {
                    room = maximum_position / 4;
                }
                else if(botroom)
                {
                    room = maximum_position - botroom;
                }
                else
                {
                    room = maximum_position;
                }
            }
            else
            {
                room = toproom;
            }

            position = room;
        }
        break;

        case LIST_MIDDLE:
        {
            uint room;
            uint toproom = index;

            if(toproom > (maximum_position / 2))
            {
                if(botroom > (maximum_position / 2))
                {
                    room = maximum_position / 2;
                }
                else if(botroom)
                {
                    room = maximum_position - botroom;
                }
                else
                {
                    room = maximum_position;
                }
            }
            else
            {
                room = toproom;
            }

            position = room;
        }
        break;

        case LIST_NEARBOTTOM:
        {
            uint room;
            uint toproom = index;

            if(toproom > 3 * (maximum_position / 4))
            {
                if(botroom > 3 * (maximum_position / 4))
                {
                    room = 3 * (maximum_position / 4);
                }
                else if(botroom)
                {
                    room = maximum_position - botroom;
                }
                else
                {
                    room = maximum_position;
                }
            }
            else
            {
                room = toproom;
            }

            position = room;
        }
        break;

        case LIST_BOTTOM:
            position = maximum_position;
            break;
    } // switch
    update();
} // gwinpick::center

//  ------------------------------------------------------------------
int gwinpick::run_picker()
{
  #ifdef GOLD_MOUSE
    gmou.HideCursor();
  #endif
    // Open and initialize
    open();

    if(helpcat)
    {
        whelppcat(helpcat);
    }

    if(not is_selectable(index))
    {
        cursor_down();
    }

    do
    {
        do_delayed();

    #ifdef GOLD_MOUSE
        gmou.ShowCursor();
    #endif

        keyok = true;

        do
        {
            key = getxchtick();

            if(key == Key_Tick)
            {
                handle_key();
            }
        }
        while(key == Key_Tick);

    #ifdef GOLD_MOUSE
        gmou.HideCursor();
    #endif

        keyok = default_handle_key();

        if(replylinkfloat && (h_offset != new_hoffset))
        {
            display_page();
        }
    }
    while(keyok);

  #ifdef GOLD_MOUSE
    gmou.HideCursor();
  #endif

    // Close and clean
    if(helpcat)
    {
        whelpop();
    }

    close();

  #ifdef GOLD_MOUSE
    gmou.ShowCursor();
  #endif

    return 0;
} // gwinpick::run_picker

//  ------------------------------------------------------------------
