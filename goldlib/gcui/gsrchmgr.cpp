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
//  GCUI: Golded+ Character-oriented User Interface.
//  Search manager.
//  ------------------------------------------------------------------

#include <gsrchmgr.h>


//  ------------------------------------------------------------------

search_item::search_item() {

  logic = logic_or;

  where.from      = false;
  where.to        = false;
  where.subject   = false;
  where.body      = false;
  where.tagline   = false;
  where.tearline  = false;
  where.origin    = false;
  where.signature = false;
  where.kludges   = false;
}


//  ------------------------------------------------------------------

search_item::~search_item() {

}


//  ------------------------------------------------------------------

search_item& search_item::operator=(const search_item& a) {

  gsearch::operator=(a);

  logic           = a.logic;
  where.from      = a.where.from;
  where.to        = a.where.to;
  where.subject   = a.where.subject;
  where.body      = a.where.body;
  where.tagline   = a.where.tagline;
  where.tearline  = a.where.tearline;
  where.origin    = a.where.origin;
  where.signature = a.where.signature;
  where.kludges   = a.where.kludges;

  return *this;
}


//  ------------------------------------------------------------------

search_manager::search_manager() : gwinput2(window) {

  direction = direction_forward;
  messages = messages_new;
  action = action_read;
  areas = areas_current;
}


//  ------------------------------------------------------------------

search_manager::~search_manager() {

}


//  ------------------------------------------------------------------
