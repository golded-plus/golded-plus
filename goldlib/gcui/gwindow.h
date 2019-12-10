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
//  Windowing class.
//  ------------------------------------------------------------------

#ifndef __gwindow_h
#define __gwindow_h
//  ------------------------------------------------------------------

#include <string>
#include <gwinall.h>

#undef getc
#undef putc
//  ------------------------------------------------------------------
inline void wgetc(int wrow,
                  int wcol,
                  vattr * atr,
                  vchar * chr)
{
    vgetc(wrow + gwin.active->srow + gwin.active->border,
          wcol + gwin.active->scol + gwin.active->border,
          atr,
          chr);
}

//  ------------------------------------------------------------------
struct gscrollbar_data
{
    long minpos;  // Minimum range position
    long maxpos;  // Maximum range position
    long pos;     // Position of the thumb button
    long page;    // Size of the visible part
};

//  ------------------------------------------------------------------
class gwindow
{
protected: _wrec_t * wrec;
    int owner;
    int window_style;
    vattr window_color;
    vattr border_hi_color;
    vattr border_lo_color;
    vattr scrollbar_color;
    vattr title_color;
    int title_position;
    vattr message_color;
    vattr shadow_color;
public: int start_row;
    int start_column;
    int end_row;
    int end_column;
    enum
    {
        title_top            = TTOP, title_left = TLEFT, title_center = TCENTER,
        title_right          = TRIGHT, title_bottom = TBOTTOM, border_top = TP_BORD,
        border_bottom        = BT_BORD, border_left = LT_BORD, border_right = RT_BORD,
        scrollbar_horizontal = W_HORZ, scrollbar_vertical = W_VERT,
        direction_down       = D_DOWN, direction_up = D_UP, direction_left = D_LEFT,
        direction_right      = D_RIGHT, bordertype_single = BT_SINGLE,
        bordertype_double    = BT_DOUBLE, bordertype_singletop = BT_SINGLETOP,
        bordertype_doubletop = BT_DOUBLETOP, bordertype_blanks = BT_BLANKS,
        bordertype_none      = BT_NONE, bordertype_blocks = BT_BLOCKS,
        bordertype_ascii     = BT_ASCII, bordertypes = 7
    };

    gwindow();
    gwindow(int old_handle);
    virtual ~gwindow();
    void init();

int height()
{
    return 1 + end_row - start_row;
}

int width()
{
    return 1 + end_column - start_column;
}

int border_type()
{
    return window_style & bordertypes;
}

bool has_border()
{
    return border_type() != BT_NONE;
}

vchar boxchar_upper_left_corner()
{
    return _box_table(border_type(), 0);
}

vchar boxchar_upper_horizontal_line()
{
    return _box_table(border_type(), 1);
}

vchar boxchar_upper_right_corner()
{
    return _box_table(border_type(), 2);
}

vchar boxchar_left_vertical_line()
{
    return _box_table(border_type(), 3);
}

vchar boxchar_right_vertical_line()
{
    return _box_table(border_type(), 4);
}

vchar boxchar_lower_left_corner()
{
    return _box_table(border_type(), 5);
}

vchar boxchar_lower_horizontal_line()
{
    return _box_table(border_type(), 6);
}

vchar boxchar_lower_right_corner()
{
    return _box_table(border_type(), 7);
}

vchar boxchar_middle_junction()
{
    return _box_table(border_type(), 8);
}

vchar boxchar_left_vertical_junction()
{
    return _box_table(border_type(), 9);
}

vchar boxchar_right_vertical_junction()
{
    return _box_table(border_type(), 10);
}

vchar boxchar_upper_horizontal_junction()
{
    return _box_table(border_type(), 11);
}

vchar boxchar_lower_horizontal_junction()
{
    return _box_table(border_type(), 12);
}

    void set_window_at(int srow, int scol);
    void set_window_size(int vlen, int hlen);
    void open(int srow,
              int scol,
              int erow,
              int ecol,
              int style,
              vattr bcolor,
              vattr wcolor,
              vattr sbcolor = DEFATTR,
              vattr locolor = DEFATTR);
    void openxy(int srow,
                int scol,
                int vlen,
                int hlen,
                int style,
                vattr bcolor,
                vattr wcolor,
                vattr sbcolor = DEFATTR,
                vattr locolor = DEFATTR);
    void open();
    void close();
    void unlink();
    void hide();
    void unhide();
    int active();
    void activate();
    void activate_quick();
    int cursor_row();
    int cursor_column();
    void text_color(vattr color);
    void move_cursor(int row, int column);
    void title(const char * title, vattr color = DEFATTR, int position = -1);
    void no_title();
    void message(const char * text, int border, int leftofs, vattr color = DEFATTR);
    void shadow(vattr color = DEFATTR);
    void no_shadow();
    void set_vscrollbar_range(int minpos, int maxpos, int visible, int total,
                              int redraw);
    void set_hscrollbar_range(int minpos, int maxpos, int visible, int total,
                              int redraw);
    void set_vscrollbar_pos(int pos, int redraw);
    void set_hscrollbar_pos(int pos, int redraw);
    void set_scrollbar_color(vattr color);
    void vscrollbar(uint total, uint maxpos, uint pos, int sadd = 0);
    void hscrollbar(uint total, uint maxpos, uint pos, int sadd = 0);
    void scroll_down(int count = 1);
    void scroll_up(int count = 1);
    void scroll_box_down(int scol, int srow, int ecol, int erow, int count = 1);
    void scroll_box_up(int scol, int srow, int ecol, int erow, int count = 1);
    void getc(int row, int col, vattr * atr, vchar * chr);
    void putc(vchar ch);
    void puts(const char * text);
    void printc(int row, int col, vattr color, vchar ch);
    void prints(int row, int col, vattr color, const char * text);
    void printvs(int row, int col, vattr color, const vchar * text);
    void prints(int row, int col, vattr color, const std::string & text);
    void printns(int row,
                 int col,
                 vattr color,
                 const char * text,
                 int len,
                 vchar fill = ' ',
                 vattr fill_color = DEFATTR);
    int printf(const char * format, ...) __attribute__ ((format(printf, 2, 3)));
    int printf(vattr color, const char * format,
               ...) __attribute__ ((format(printf, 3, 4)));
    int printf(int row, int col, const char * format,
               ...) __attribute__ ((format(printf, 4, 5)));
    int printf(int row, int col, vattr color, const char * format,
               ...) __attribute__ ((format(printf, 5, 6)));
    void fill_char(vchar ch);
    void fill(int wsrow, int wscol, int werow, int wecol, vchar ch, vattr color);
    void vertical_line(int wsrow, int wscol, int count, int btype, vattr color);
    void horizontal_line(int wsrow, int wscol, int count, int btype, vattr color);
    void clear(vattr color = DEFATTR);
    void clear_eol();
    void drag(int direction, int howmuch = 1);
    void slide(int row, int col);
    void putx(int wrow, int wcol, vattr color, char chr, uint len);
    void print_center(int row, vattr color, const char * text);
};

//  ------------------------------------------------------------------
inline void gwindow::init()
{
    wrec            = NULL;
    start_row       = start_column = 0;
    end_row         = gvid->curr.screen.rows - 1;
    end_column      = gvid->curr.screen.columns - 1;
    window_style    = 0;
    window_color    = BLACK_ | _LGREY;
    border_hi_color = BLUE_ | _LGREY;
    border_lo_color = DEFATTR;
    scrollbar_color = DEFATTR;
    title_color     = BLUE_ | _LGREY;
    title_position  = title_center;
    message_color   = BLUE_ | _LGREY;
    shadow_color    = DGREY_ | _BLACK;
}

//  ------------------------------------------------------------------
inline void gwindow::set_window_at(int srow, int scol)
{
    start_row    = srow;
    start_column = scol;
}

//  ------------------------------------------------------------------
inline void gwindow::set_window_size(int vlen, int hlen)
{
    end_row    = start_row + vlen - 1;
    end_column = start_column + hlen - 1;
}

//  ------------------------------------------------------------------
inline void gwindow::open(int srow,
                          int scol,
                          int erow,
                          int ecol,
                          int style,
                          vattr bcolor,
                          vattr wcolor,
                          vattr sbcolor,
                          vattr locolor)
{
    start_row       = srow;
    start_column    = scol;
    end_row         = erow;
    end_column      = ecol;
    window_color    = wcolor;
    window_style    = style;
    border_hi_color = bcolor;
    border_lo_color = locolor;
    scrollbar_color = sbcolor;
    wopen(srow, scol, erow, ecol, style, bcolor, wcolor, sbcolor, locolor);
    wrec = gwin.active;
}

//  ------------------------------------------------------------------
inline void gwindow::openxy(int srow,
                            int scol,
                            int vlen,
                            int hlen,
                            int style,
                            vattr bcolor,
                            vattr wcolor,
                            vattr sbcolor,
                            vattr locolor)
{
    open(srow,
         scol,
         srow + vlen - 1,
         scol + hlen - 1,
         style,
         bcolor,
         wcolor,
         sbcolor,
         locolor);
}

//  ------------------------------------------------------------------
inline void gwindow::open()
{
    open(start_row,
         start_column,
         end_row,
         end_column,
         window_style,
         border_hi_color,
         window_color,
         scrollbar_color,
         border_lo_color);
}

//  ------------------------------------------------------------------
inline int gwindow::active()
{
    return wrec == gwin.active;
}

//  ------------------------------------------------------------------
inline void gwindow::activate_quick()
{
    if(!active())
    {
        wactiv_(wrec->whandle);
    }
}

//  ------------------------------------------------------------------
inline void gwindow::close()
{
    if(wrec)
    {
        activate_quick();
        wclose();
        wrec = NULL;
    }
}

//  ------------------------------------------------------------------
inline void gwindow::unlink()
{
    wunlink(wrec->whandle);
    wrec = NULL;
}

//  ------------------------------------------------------------------
inline void gwindow::hide()
{
    activate_quick();
    whide();
}

//  ------------------------------------------------------------------
inline void gwindow::unhide()
{
    wunhide(wrec->whandle);
}

//  ------------------------------------------------------------------
inline void gwindow::activate()
{
    if(!active())
    {
        wactiv(wrec->whandle);
    }
}

//  ------------------------------------------------------------------
inline int gwindow::cursor_row()
{
    return wrec->row;
}

//  ------------------------------------------------------------------
inline int gwindow::cursor_column()
{
    return wrec->column;
}

//  ------------------------------------------------------------------
inline void gwindow::text_color(vattr color)
{
    window_color = color;
    activate_quick();
    wtextattr(color);
}

//  ------------------------------------------------------------------
inline void gwindow::set_scrollbar_color(vattr color)
{
    wrec->sbattr    = color;
    scrollbar_color = color;
}

//  ------------------------------------------------------------------
inline void gwindow::move_cursor(int row, int column)
{
    activate_quick();
    wgotoxy(row, column);
}

//  ------------------------------------------------------------------
inline void gwindow::title(const char * title, vattr color, int position)
{
    if(color != DEFATTR)
    {
        title_color = color;
    }

    if(position != -1)
    {
        title_position = position;
    }

    activate_quick();
    wtitle(title, title_position, title_color);
}

//  ------------------------------------------------------------------
inline void gwindow::no_title()
{
    title(NULL);
}

//  ------------------------------------------------------------------
inline void gwindow::message(const char * text, int border, int leftofs, vattr color)
{
    if(color != DEFATTR)
    {
        message_color = color;
    }

    activate_quick();
    wmessage(text, border, leftofs, message_color);
}

//  ------------------------------------------------------------------
inline void gwindow::shadow(vattr color)
{
    if(color != DEFATTR)
    {
        shadow_color = color;
    }

    activate_quick();
    wshadow(shadow_color);
}

//  ------------------------------------------------------------------
inline void gwindow::no_shadow()
{
    activate_quick();
    wshadoff();
}

//  ------------------------------------------------------------------
inline void gwindow::vscrollbar(uint total, uint maxpos, uint pos, int sadd)
{
    activate_quick();
    wscrollbar(scrollbar_vertical, total, maxpos, pos, sadd);
}

//  ------------------------------------------------------------------
inline void gwindow::hscrollbar(uint total, uint maxpos, uint pos, int sadd)
{
    activate_quick();
    wscrollbar(scrollbar_horizontal, total, maxpos, pos, sadd);
}

//  ------------------------------------------------------------------
inline void gwindow::scroll_down(int count)
{
    activate_quick();
    wscroll(count, direction_down);
}

//  ------------------------------------------------------------------
inline void gwindow::scroll_up(int count)
{
    activate_quick();
    wscroll(count, direction_up);
}

//  ------------------------------------------------------------------
inline void gwindow::scroll_box_down(int scol, int srow, int ecol, int erow, int count)
{
    activate_quick();
    wscrollbox(scol, srow, ecol, erow, count, direction_down);
}

//  ------------------------------------------------------------------
inline void gwindow::scroll_box_up(int scol, int srow, int ecol, int erow, int count)
{
    activate_quick();
    wscrollbox(scol, srow, ecol, erow, count, direction_up);
}

//  ------------------------------------------------------------------
inline void gwindow::getc(int row, int col, vattr * atr, vchar * chr)
{
    activate_quick();
    wgetc(row, col, atr, chr);
}

//  ------------------------------------------------------------------
inline void gwindow::putc(vchar ch)
{
    activate_quick();
    wputc(ch);
}

//  ------------------------------------------------------------------
inline void gwindow::puts(const char * text)
{
    activate_quick();
    wputs(text);
}

//  ------------------------------------------------------------------
inline void gwindow::printc(int row, int col, vattr color, vchar ch)
{
    activate_quick();
    wprintc(row, col, color, ch);
}

//  ------------------------------------------------------------------
inline void gwindow::prints(int row, int col, vattr color, const char * text)
{
    activate_quick();
    wprints(row, col, color == DEFATTR ? window_color : color, text);
}

//  ------------------------------------------------------------------
inline void gwindow::printvs(int row, int col, vattr color, const vchar * text)
{
    activate_quick();
    wprintvs(row, col, color == DEFATTR ? window_color : color, text);
}

//  ------------------------------------------------------------------
inline void gwindow::prints(int row, int col, vattr color, const std::string & text)
{
    prints(row, col, color, text.c_str());
}

//  ------------------------------------------------------------------
inline void gwindow::printns(int row,
                             int col,
                             vattr color,
                             const char * text,
                             int len,
                             vchar fill,
                             vattr fill_color)
{
    activate_quick();
    wprintns(row, col, color, text, len, fill, fill_color);
}

//  ------------------------------------------------------------------
inline void gwindow::fill_char(vchar ch)
{
    activate_quick();
    wfillch(ch);
}

//  ------------------------------------------------------------------
inline void gwindow::fill(int wsrow,
                          int wscol,
                          int werow,
                          int wecol,
                          vchar ch,
                          vattr color)
{
    activate_quick();
    wfill(wsrow, wscol, werow, wecol, ch, color);
}

//  ------------------------------------------------------------------
inline void gwindow::vertical_line(int wsrow,
                                   int wscol,
                                   int count,
                                   int btype,
                                   vattr color)
{
    activate_quick();
    wvline(wsrow, wscol, count, btype, color);
}

//  ------------------------------------------------------------------
inline void gwindow::horizontal_line(int wsrow,
                                     int wscol,
                                     int count,
                                     int btype,
                                     vattr color)
{
    activate_quick();
    whline(wsrow, wscol, count, btype, color);
}

//  ------------------------------------------------------------------
inline void gwindow::clear(vattr color)
{
    activate_quick();
    wcclear(color == DEFATTR ? window_color : color);
}

//  ------------------------------------------------------------------
inline gwindow::gwindow()
{
    init();
    owner = true;
}

//  ------------------------------------------------------------------
inline gwindow::gwindow(int old_handle)
{
    init();
    owner           = false;
    wrec            = wfindrec(old_handle);
    start_row       = wrec->srow;
    start_column    = wrec->scol;
    end_row         = wrec->erow;
    end_column      = wrec->ecol;
    window_color    = wrec->wattr;
    window_style    = wrec->btype;
    border_hi_color = wrec->battr;
    border_lo_color = wrec->loattr;
    scrollbar_color = wrec->sbattr;
}

//  ------------------------------------------------------------------
inline gwindow::~gwindow()
{
    if(owner)
    {
        close();
    }
}

//  ------------------------------------------------------------------
inline void gwindow::slide(int row, int col)
{
    activate_quick();
    wslide(row, col);
}

//  ------------------------------------------------------------------
inline void gwindow::print_center(int row, vattr color, const char * text)
{
    activate_quick();
    wcenters(row, color, text);
}

//  ------------------------------------------------------------------

#endif // ifndef __gwindow_h
//  ------------------------------------------------------------------
