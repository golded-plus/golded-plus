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
//  Input form and field editing.
//  ------------------------------------------------------------------

#ifndef __gwinput_h
#define __gwinput_h


//  ------------------------------------------------------------------

#include <gwindow.h>


//  ------------------------------------------------------------------

class gwinput {

public:

  class field {

  public:

    gwinput* form;

    int pos;
    int max_pos;
    int attr;
    bool fill_acs;
    vchar fill;
    int entry;

    char* buf;
    int buf_left_pos;
    int buf_end_pos;
    int buf_pos;
    int buf_len;

    string& destination;

    int id;
    int row;
    int column;
    int max_column;
    int conversion;
    int entry_mode;

    field* prev;
    field* next;

    field(gwinput* iform, int idnum, int wrow, int wcol, int field_width, string& dest, int dest_size, int cvt, int mode);
    ~field();

    bool visible();

    void move_left();
    void move_right();

    bool left();
    bool right();
    bool left_word();
    bool right_word();
    bool delete_left();
    bool delete_char();
    bool delete_word(bool left);
    bool insert_char(char ch);
    bool overwrite_char(char ch);
    bool home();
    bool end();

    void update();
    void activate();
    void deactivate();

    void restore();
    void commit();

    void convert();
    bool adjust_mode();
    void conditional();

    void move_cursor();
    void draw(int from_pos=0);
  };

  field* first_field;
  field* current;

  enum {
    entry_new,
    entry_update,
    entry_conditional,
    entry_noedit
  };

  enum {
    cvt_none,
    cvt_lowercase,
    cvt_uppercase,
    cvt_mixedcase
  };

  int idle_attr;
  int active_attr;
  int edit_attr;
  bool fill_acs;

  vchar idle_fill;
  vchar active_fill;
  vchar edit_fill;

  int insert_mode;

  int done;
  int dropped;

  int start_id;

  bool cursor_was_hidden;

  gwindow &window;

  gwinput(gwindow &w);
  virtual ~gwinput();

  void setup(int i_attr, int a_attr, int e_attr, vchar fill, bool fill_acs);

  void add_field(int idnum, int wrow, int wcol, int field_width, string& dest, int dest_size, int cvt=gwinput::cvt_none, int mode=gwinput::entry_conditional);

  bool first(int id=0);
  bool next();
  bool previous();
  bool last();

  bool first_visible();
  bool next_visible();
  bool previous_visible();
  bool last_visible();

  bool move_to(int wrow, int wcol);

  field* field_at(int wrow, int wcol);
  field* get_field(int id);

  void draw_all();
  void reload_all();
  void show_cursor();

  void drop_form();
  void form_complete();
  void field_complete();
  void go_next_field();
  void go_previous_field();
  void go_up();
  void go_down();
  void go_left();
  void go_right();
  void delete_left();
  void delete_char();
  void go_field_begin();
  void go_field_end();
  void go_form_begin();
  void go_form_end();
  void toggle_insert();
  void restore_field();
  void delete_left_word();
  void delete_right_word();
  void clear_to_end_field();
  void clear_to_end_form();
  void go_left_word();
  void go_right_word();
  void enter_char(char ch);

  void prepare_form();
  void finish_form();

  bool handle_key(gkey key);

  // These are supposed to be overridden by the inheriting class
  virtual bool handle_other_keys(gkey& key);
  virtual bool validate();
  virtual void before();
  virtual void after();
};


//  ------------------------------------------------------------------

class gwinput2 : public gwinput {

public:

  gwinput2(gwindow &w) : gwinput(w) { }

  bool run(int helpcat);
};


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
