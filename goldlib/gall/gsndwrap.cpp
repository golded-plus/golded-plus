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
//  Sound interface class.
//  ------------------------------------------------------------------

#include <cstdlib>
#include <gsndall.h>


//  ------------------------------------------------------------------

static gsnd* snd = NULL;


//  ------------------------------------------------------------------

gsound::gsound() {

  snd = new gsnd;
  installed = (snd->open_api() != -1);
}


//  ------------------------------------------------------------------

gsound::~gsound() {

  snd->close_api();
  delete snd;
}


//  ------------------------------------------------------------------

int gsound::load(const char* file) {

  return snd->open(file);
}


//  ------------------------------------------------------------------

int gsound::unload() {

  return snd->close();
}


//  ------------------------------------------------------------------

int gsound::play() {

  return snd->play();
}


//  ------------------------------------------------------------------

int gsound::stop() {

  return snd->stop();
}


//  ------------------------------------------------------------------

int gsound::is_playing() {

  return snd->is_playing();
}


//  ------------------------------------------------------------------
