//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999 Alexander S. Aganichev
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
//  Search manager.
//  ------------------------------------------------------------------

#ifndef __gsrchmgr_h
#define __gsrchmgr_h


//  ------------------------------------------------------------------

#include <gdefs.h>
#include <vector>
#include <gsearch.h>
#include <gwinput.h>


//  ------------------------------------------------------------------

class search_item : public gsearch {

public:

  enum item_logic {
    logic_and,
    logic_or
  };

  struct item_where {
    bool from;
    bool to;
    bool subject;
    bool body;
    bool tagline;
    bool tearline;
    bool origin;
    bool signature;
    bool kludges;
  };

  item_logic logic;
  item_where where;

  search_item();
  virtual ~search_item();

  search_item& operator=(const search_item& a);

};


//  ------------------------------------------------------------------

class search_manager : public gwinput2 {

public:

  enum {
    if_nothing,
    id_pattern1, id_logic1, id_options1,
    id_pattern2, id_logic2, id_options2,
    id_pattern3, id_logic3, id_options3,
    id_pattern4, id_logic4, id_options4,
    id_pattern5, id_logic5, id_options5,
    id_pattern6, id_logic6, id_options6,
    id_pattern7, id_logic7, id_options7,
    id_pattern8, id_logic8, id_options8,
    id_pattern9, id_logic9, id_options9,
    id_direction,
    id_messages,
    id_action,
    id_areas
  };

  enum search_direction {
    direction_backward,
    direction_forward
  };

  enum search_messages {
    messages_new,
    messages_unread,
    messages_all
  };

  enum search_action {
    action_read,
    action_mark,
    action_delete,
    action_write,
    action_copy,
    action_move
  };

  enum search_areas {
    areas_current,
    areas_tagged,
    areas_all
  };

  std::vector<search_item> items;

  search_direction    direction;
  search_messages     messages;
  search_action       action;
  search_areas        areas;

  gwindow window;

  search_manager();
  virtual ~search_manager();

};


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
