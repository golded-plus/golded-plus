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

#include <cstring>
#include <gkbdcode.h>
#include <gmemdbg.h>
#include <gstrall.h>
#include <gwinall.h>
#include <gwinhelp.h>
#include <gwinput.h>


//  ------------------------------------------------------------------

gwinput::gwinput(gwindow &w) : window(w) {

  first_field = current = NULL;
  fill_acs = false;
  idle_attr = active_attr = edit_attr = 7;
  idle_fill = active_fill = edit_fill = ' ';
  insert_mode = true;
  done = dropped = false;
  start_id = 0;
}


//  ------------------------------------------------------------------

gwinput::~gwinput() {

  field* current = first_field;
  if(current) {
    do {
      field* junk = current;
      current = current->next;
      delete junk;
    } while(current);
  }
}


//  ------------------------------------------------------------------

void gwinput::setup(int i_attr, int a_attr, int e_attr, vchar fill, bool f_acs) {

  idle_attr = i_attr;
  active_attr = a_attr;
  edit_attr = e_attr;
  fill_acs = f_acs;
  active_fill = edit_fill = fill;
}


//  ------------------------------------------------------------------

bool gwinput::validate() {

  return true;
}


//  ------------------------------------------------------------------

void gwinput::before() {

  current->activate();
}


//  ------------------------------------------------------------------

void gwinput::after() {

  current->deactivate();
}


//  ------------------------------------------------------------------

void gwinput::add_field(int idnum, int wrow, int wcol, int field_width, string& dest, int dest_size, int cvt, int mode) {

  field* fld = new field(this, idnum, wrow, wcol, field_width, dest, dest_size, cvt, mode);
  throw_new(fld);
  if(current) {
    current->next = fld;
    fld->prev = current;
    current = fld;
  }
  else {
    first_field = current = fld;
  }
}


//  ------------------------------------------------------------------

bool gwinput::move_to(int wrow, int wcol) {

  field* f = field_at(wrow, wcol);
  if(f) {
    after();
    current = f;
    before();
    return true;
  }
  return false;
}


//  ------------------------------------------------------------------

gwinput::field* gwinput::field_at(int wrow, int wcol) {

  field* here = first_field;
  do {
    if(here->row == wrow)
      if(in_range(wcol, here->column, here->max_column))
        return here;
    here = here->next;
  } while(here);
  return NULL;
}


//  ------------------------------------------------------------------

gwinput::field* gwinput::get_field(int id) {

  field* here = first_field;
  do {
    if(here->id == id)
      return here;
    here = here->next;
  } while(here);
  return NULL;
}


//  ------------------------------------------------------------------

void gwinput::draw_all() {

  first();
  do {
    current->draw();
  } while(next());
}


//  ------------------------------------------------------------------

void gwinput::reload_all() {

  first();
  do {
    if(current->entry == gwinput::entry_new)
      *current->buf = NUL;
    else
      strxcpy(current->buf, current->destination.c_str(), current->buf_len+1);
    current->convert();
    current->buf_end_pos = strlen(current->buf);
    current->draw();
  } while(next());
}


//  ------------------------------------------------------------------

bool gwinput::first_visible() {

  if(first()) {
    if(not current->visible())
      if(next_visible())
        return true;
  }
  return false;
}


//  ------------------------------------------------------------------

bool gwinput::next_visible() {

  field* here = current;
  while(here->next) {
    here = here->next;
    if(here->visible()) {
      current = here;
      return true;
    }
  }
  return false;
}


//  ------------------------------------------------------------------

bool gwinput::previous_visible() {

  field* here = current;
  while(here->prev) {
    here = here->prev;
    if(here->visible()) {
      current = here;
      return true;
    }
  }
  return false;
}


//  ------------------------------------------------------------------

bool gwinput::last_visible() {

  if(last()) {
    if(not current->visible())
      if(previous_visible())
        return true;
  }
  return false;
}


//  ------------------------------------------------------------------

bool gwinput::first(int id) {

  if(first_field) {
    current = first_field;
    if(id)
      while(current->id != id)
        next();
    return true;
  }
  return false;
}


//  ------------------------------------------------------------------

bool gwinput::next() {

  if(current->next) {
    current = current->next;
    return true;
  }
  return false;
}


//  ------------------------------------------------------------------

bool gwinput::previous() {

  if(current->prev) {
    current = current->prev;
    return true;
  }
  return false;
}


//  ------------------------------------------------------------------

bool gwinput::last() {

  if(first_field) {
    current = first_field;
    while(next())
      ;
    return true;
  }
  return false;
}


//  ------------------------------------------------------------------

void gwinput::show_cursor() {

  if(insert_mode)
    vcursmall();
  else
    vcurlarge();
}


//  ------------------------------------------------------------------

void gwinput::drop_form() {

  after();
  done = true;
  dropped = true;
}


//  ------------------------------------------------------------------

void gwinput::form_complete() {

  after();
  done = true;
}


//  ------------------------------------------------------------------

void gwinput::field_complete() {

  if(validate()) {
    after();
    if(not next_visible()) {
      done = true;
      return;
    }
    before();
  }
}


//  ------------------------------------------------------------------

void gwinput::go_next_field() {

  after();
  if(not next_visible())
    first_visible();
  before();
}


//  ------------------------------------------------------------------

void gwinput::go_previous_field() {

  after();
  if(not previous_visible())
    last_visible();
  before();
}


//  ------------------------------------------------------------------

void gwinput::go_up() {

  int min_column = current->column;
  int max_column = current->max_column;
  int check_row = current->row - 1;
  int check_column = current->column + current->pos;
  for(int r=check_row; r>=0; r--) {
    int c;
    for(c=check_column; c<=max_column; c++) {
      if(move_to(r, c))
        return;
    }
    for(c=check_column-1; c>=min_column; c--) {
      if(move_to(r, c))
        return;
    }
  }
}


//  ------------------------------------------------------------------

void gwinput::go_down() {

  int max_row = window.height() - (window.has_border() ? 2 : 0) - 1;
  int min_column = current->column;
  int max_column = current->max_column;
  int check_row = current->row + 1;
  int check_column = current->column + current->pos;
  for(int r=check_row; r<=max_row; r++) {
    int c;
    for(c=check_column; c<=max_column; c++) {
      if(move_to(r, c))
        return;
    }
    for(c=check_column-1; c>=min_column; c--) {
      if(move_to(r, c))
        return;
    }
  }
}


//  ------------------------------------------------------------------

void gwinput::go_left() {

  if(not current->left())
    go_previous_field();
}


//  ------------------------------------------------------------------

void gwinput::go_right() {

  if(not current->right())
    go_next_field();
}


//  ------------------------------------------------------------------

void gwinput::delete_left() {

  current->delete_left();
}


//  ------------------------------------------------------------------

void gwinput::delete_char() {

  current->delete_char();
}


//  ------------------------------------------------------------------

void gwinput::go_field_begin() {

  current->home();
}


//  ------------------------------------------------------------------

void gwinput::go_field_end() {

  current->end();
}


//  ------------------------------------------------------------------

void gwinput::go_form_begin() {

  after();
  first_visible();
  before();
  current->home();
}


//  ------------------------------------------------------------------

void gwinput::go_form_end() {

  after();
  last_visible();
  before();
  current->end();
}


//  ------------------------------------------------------------------

void gwinput::toggle_insert() {

  insert_mode ^= true;
  show_cursor();
}


//  ------------------------------------------------------------------

void gwinput::restore_field() {

  current->restore();
}


//  ------------------------------------------------------------------

void gwinput::delete_left_word() {

  current->delete_word(true);
}


//  ------------------------------------------------------------------

void gwinput::delete_right_word() {

  current->delete_word(false);
}


//  ------------------------------------------------------------------

void gwinput::clear_to_end_field() {

}


//  ------------------------------------------------------------------

void gwinput::clear_to_end_form() {

}


//  ------------------------------------------------------------------

void gwinput::go_left_word() {

  current->left_word();
}


//  ------------------------------------------------------------------

void gwinput::go_right_word() {

  current->right_word();
}


//  ------------------------------------------------------------------

void gwinput::enter_char(char ch) {

  if(ch) {
    if(insert_mode)
      current->insert_char(ch);
    else
      current->overwrite_char(ch);
  }
}


//  ------------------------------------------------------------------

void gwinput::prepare_form() {

  cursor_was_hidden = vcurhidden();
  draw_all();
  first(start_id);
  before();
  show_cursor();
}


//  ------------------------------------------------------------------

void gwinput::finish_form() {

  if(not dropped) {
    first();
    do {
      current->commit();
    } while(next());
  }

  if(cursor_was_hidden)
    vcurhide();
}


//  ------------------------------------------------------------------

bool gwinput::handle_other_keys(gkey&) {

  return false;
}


//  ------------------------------------------------------------------

bool gwinput::handle_key(gkey key) {

  switch(key) {
    case Key_Esc:           drop_form();                 break;
    case Key_C_Ent:         form_complete();             break;
    case Key_Ent:           field_complete();            break;
    case Key_Tab:           go_next_field();             break;
    case Key_S_Tab:         go_previous_field();         break;
    case Key_Up:            go_up();                     break;
    case Key_Dwn:           go_down();                   break;
    case Key_Lft:           go_left();                   break;
    case Key_Rgt:           go_right();                  break;
    case Key_BS:            delete_left();               break;
    case Key_Del:           delete_char();               break;
    case Key_Home:          go_field_begin();            break;
    case Key_End:           go_field_end();              break;
    case Key_C_Home:        go_form_begin();             break;
    case Key_C_End:         go_form_end();               break;
    case Key_Ins:           toggle_insert();             break;
    case Key_C_R:           restore_field();             break;
    case Key_C_BS:          delete_left_word();          break;
    case Key_C_T:           delete_right_word();         break;
    case Key_C_U:           clear_to_end_field();        break;
    case Key_C_Y:           clear_to_end_form();         break;
    case Key_C_Lft:         go_left_word();              break;
    case Key_C_Rgt:         go_right_word();             break;
    default:
      if(not handle_other_keys(key))
        enter_char(KCodAsc(key));
  }

  return not done;
}


//  ------------------------------------------------------------------

gwinput::field::field(gwinput* iform, int idnum, int wrow, int wcol, int field_width, string& dest, int dest_size, int cvt, int mode)

  : destination(dest)

{
  prev = next = NULL;
  pos = buf_pos = buf_left_pos = 0;

  form = iform;
  id = idnum;
  row = wrow;
  column = wcol;
  max_pos = field_width - 1;
  max_column = wcol + max_pos;
  buf_len = dest_size - 1;
  buf = new char[dest_size];  throw_new(buf);
  conversion = cvt;
  entry = entry_mode = mode;
  attr = form->idle_attr;
  fill = form->idle_fill;
  fill_acs = form->fill_acs;

  if(entry == gwinput::entry_new)
    *buf = NUL;
  else
    strxcpy(buf, dest.c_str(), dest_size);
  convert();
  buf_end_pos = strlen(buf);
}


//  ------------------------------------------------------------------

gwinput::field::~field() {

  delete[] buf;
}


//  ------------------------------------------------------------------

bool gwinput::field::visible() {

  return max_pos != -1;
}


//  ------------------------------------------------------------------

void gwinput::field::convert() {

  switch(conversion) {
    case gwinput::cvt_lowercase:
      strlwr(buf);
      break;
    case gwinput::cvt_uppercase:
      strupr(buf);
      break;
    case gwinput::cvt_mixedcase:
      struplow(buf);
      break;
  }
}


//  ------------------------------------------------------------------

void gwinput::field::update() {

  buf_end_pos = strlen(buf);
  end();
}


//  ------------------------------------------------------------------

void gwinput::field::activate() {

  buf_end_pos = strlen(buf);
  entry = entry_mode;
  if(entry == gwinput::entry_conditional or entry == gwinput::entry_noedit) {
    attr = form->active_attr;
    fill = form->active_fill;
    int entry_bak = entry;
    entry = gwinput::entry_update;   // cheat adjust_mode() in end()
    end();
    entry = entry_bak;
  }
  else {
    // 0 == entry_new, 1 == entry_update
    entry ? end() : home();
  }
}


//  ------------------------------------------------------------------

void gwinput::field::deactivate() {

  fill = form->idle_fill;
  attr = form->idle_attr;
  draw();
}


//  ------------------------------------------------------------------

void gwinput::field::restore() {

  strxcpy(buf, destination.c_str(), buf_len+1);
  convert();
  buf_end_pos = strlen(buf);
  activate();
}


//  ------------------------------------------------------------------

void gwinput::field::commit() {

  destination = buf;
}


//  ------------------------------------------------------------------

void gwinput::field::move_cursor() {

  form->window.move_cursor(row, column+pos);
}


//  ------------------------------------------------------------------

void gwinput::field::draw(int from_pos) {

  if(visible())
    form->window.printns(row, column+from_pos, attr, buf+buf_left_pos+from_pos, 1+max_pos-from_pos, fill, attr | (fill_acs ? ACSET : 0));
}


//  ------------------------------------------------------------------

bool gwinput::field::adjust_mode() {

  if(entry != gwinput::entry_update and entry != gwinput::entry_noedit) {
    entry = gwinput::entry_update;
    attr = form->edit_attr;
    fill = form->edit_fill;
    return true;
  }
  return false;
}


//  ------------------------------------------------------------------

void gwinput::field::conditional() {

  if(entry == gwinput::entry_conditional) {
    pos = buf_pos = buf_left_pos = buf_end_pos = 0;
    *buf = NUL;
    adjust_mode();
    draw();
  }
}


//  ------------------------------------------------------------------

void gwinput::field::move_left() {

  buf_pos--;
  if(pos > 0) {
    pos--;
    move_cursor();
  }
  else {
    buf_left_pos--;
    draw();
  }
}


//  ------------------------------------------------------------------

void gwinput::field::move_right() {

  buf_pos++;
  if(pos < max_pos) {
    pos++;
    move_cursor();
  }
  else {
    buf_left_pos++;
    draw();
  }
}


//  ------------------------------------------------------------------

bool gwinput::field::left() {

  if(adjust_mode())
    draw();

  if(entry != gwinput::entry_noedit) {
    if(buf_pos > 0) {
      move_left();
      return true;
    }
  }
  return false;
}


//  ------------------------------------------------------------------

bool gwinput::field::right() {

  if(adjust_mode()) {
    draw();
    return true;
  }

  if(entry != gwinput::entry_noedit) {
    if(buf_pos < buf_end_pos) {
      move_right();
      return true;
    }
  }
  return false;
}


//  ------------------------------------------------------------------

bool gwinput::field::left_word() {

  if(adjust_mode())
    draw();

  if(entry != gwinput::entry_noedit) {
    if(buf_pos > 0) {
      move_left();
      if(not isxalnum(buf[buf_pos])) {
        while(not isxalnum(buf[buf_pos]) and (buf_pos > 0))
          move_left();
        while(isxalnum(buf[buf_pos]) and (buf_pos > 0))
          move_left();
      }
      else {
        while(isxalnum(buf[buf_pos]) and (buf_pos > 0))
          move_left();
      }

      if(buf_pos != 0)
        move_right();
    }
  }
  return false;
}


//  ------------------------------------------------------------------

bool gwinput::field::right_word() {

  if(adjust_mode())
    draw();

  if(entry != gwinput::entry_noedit) {
    if(buf_pos < buf_end_pos) {
      move_right();
      if(not isxalnum(buf[buf_pos])) {
        while(not isxalnum(buf[buf_pos]) and ((buf_pos+1) <= buf_end_pos))
          move_right();
      }
      else {
        while(isxalnum(buf[buf_pos]) and ((buf_pos+1) <= buf_end_pos))
          move_right();
        while(not isxalnum(buf[buf_pos]) and ((buf_pos+1) <= buf_end_pos))
          move_right();
      }
      return true;
    }
  }
  return false;
}


//  ------------------------------------------------------------------

bool gwinput::field::delete_left() {

  if(adjust_mode())
    draw();

  if(entry != gwinput::entry_noedit) {
    if(buf_pos > 0) {
      left();
      return delete_char();
    }
  }
  return false;
}


//  ------------------------------------------------------------------

bool gwinput::field::delete_char() {

  if(adjust_mode())
    draw();

  if(entry != gwinput::entry_noedit) {
    if(buf_pos < buf_end_pos) {
      buf_end_pos--;
      memmove(buf+buf_pos, buf+buf_pos+1, buf_len-buf_pos);
      draw(pos);
      move_cursor();
      return true;
    }
  }
  return false;
}


//  ------------------------------------------------------------------

bool gwinput::field::delete_word(bool left) {

  if(adjust_mode())
    draw();

  if(entry != gwinput::entry_noedit) {

    bool state = isspace(buf[buf_pos-((int) left)]);

    while(left ? buf_pos > 0 : buf_pos < buf_end_pos) {
      left ? delete_left() : delete_char();

      if(isspace(buf[buf_pos-((int) left)]) != state)
        break;
    }
    return true;

  }
  return false;
}


//  ------------------------------------------------------------------

bool gwinput::field::insert_char(char ch) {

  if(entry != gwinput::entry_noedit) {

    conditional();

    if(buf_end_pos < buf_len) {
      int len = buf_end_pos - buf_pos;
      memmove(buf+buf_pos+1, buf+buf_pos, len+1);
      buf_end_pos++;
      return overwrite_char(ch);
    }
  }
  return false;
}


//  ------------------------------------------------------------------

bool gwinput::field::overwrite_char(char ch) {

  if(entry != gwinput::entry_noedit) {

    conditional();

    switch(conversion) {
      case gwinput::cvt_lowercase:
        ch = (char)tolower(ch);
        break;
      case gwinput::cvt_uppercase:
        ch = (char)toupper(ch);
        break;
    }

    buf[buf_pos] = ch;
    if(buf_pos == buf_end_pos) {
      buf_end_pos++;
      buf[buf_end_pos] = NUL;
    }

    if(conversion == gwinput::cvt_mixedcase) {
      struplow(buf);
      draw();
    }
    else {
      draw(pos);
    }

    right();
  }

  return true;
}


//  ------------------------------------------------------------------

bool gwinput::field::home() {

  adjust_mode();

  pos = buf_pos = buf_left_pos = 0;
  draw();
  move_cursor();
  return true;
}


//  ------------------------------------------------------------------

bool gwinput::field::end() {

  adjust_mode();

  buf_pos = buf_end_pos;
  if(buf_pos == buf_len)
    buf_pos--;
  pos = minimum_of_two(max_pos, buf_pos);
  buf_left_pos = buf_pos - pos;
  draw();
  move_cursor();
  return true;
}


//  ------------------------------------------------------------------

bool gwinput2::run(int helpcat) {

  prepare_form();
  whelppcat(helpcat);

  while(handle_key(getxch()));

  whelpop();
  finish_form();

  return not dropped;
}


//  ------------------------------------------------------------------

#if defined(TESTMAIN)

void main() {

  enum {
    FIELD_A,
    FIELD_B,
    FIELD_C,
    FIELD_D,
    FIELD_E
  };

  gwindow w;

  w.open(0, 0, gvid->numrows-1, gvid->numcols-1, 5, YELLOW|_LGREY, BLUE|_LGREY);

  w.horizontal_line(0, 0, w.width(), 0, YELLOW|_LGREY);
  w.horizontal_line(5, 0, w.width(), 0, YELLOW|_LGREY);

  w.prints(1, 0, BLUE|_LGREY, " Msg  : 117 of 123");
  w.prints(2, 0, BLUE|_LGREY, " From : ");
  w.prints(3, 0, BLUE|_LGREY, " To   : ");
  w.prints(4, 0, BLUE|_LGREY, " Subj : ");

  gwinput* i = new gwinput(w);

  char str_a[128] = { "Odinn Sorensen" };
  char str_b[128] = { "2:236/77" };
  char str_c[128] = { "Who Knows" };
  char str_d[128] = { "2:236/77.123" };
  char str_e[128] = { "Testing my new input class" };

  i->idle_attr   =  BLUE|_LGREY;
  i->active_attr = BLACK|_LGREY;
  i->edit_attr   =   RED|_LGREY;

  i->active_fill = '\xB0';
  i->edit_fill   = '\xB0';
  i->fill_acs    = true;

  i->add_field(FIELD_A, 2,  8, 35, str_a, 127);
  i->add_field(FIELD_B, 2, 44, 16, str_b, 127);
  i->add_field(FIELD_C, 3,  8, 35, str_c, 127);
  i->add_field(FIELD_D, 3, 44, 16, str_d, 127);
  i->add_field(FIELD_E, 4,  8, 71, str_e, 127);

  i->start_id = FIELD_E;

  i->prepare_form();

  gkey key = 0;
  do {

    {
      // DEBUG
      char buf[256];
      sprintf(buf, "bp:%i blp:%i", i->current->buf_pos, i->current->buf_left_pos);
      wprintfs(gvid->numrows-1, 0, WHITE|_BLUE, " %-*s ", gvid->numcols-2, buf);
    }

    key = getxch();
  } while(i->handle_key(key));

  i->finish_form();

  delete i;

  window.close();

  gvid->restore_cursor();
}

#endif


//  ------------------------------------------------------------------

