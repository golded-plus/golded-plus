//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999-2000 Alex. S. Aganichev
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
//  Hudson/GoldBase messagebase engine.
//  ------------------------------------------------------------------

#include <cerrno>
#include <gmoprot.h>
#include <gmohuds.h>


//  ------------------------------------------------------------------

HudsWide* hudsonwide = NULL;
GoldWide* goldbasewide = NULL;


//  ------------------------------------------------------------------

void HudsWideOpen() {

  hudsonwide->open();
}


//  ------------------------------------------------------------------

void GoldWideOpen() {

  goldbasewide->open();
}


//  ------------------------------------------------------------------

void HudsWideClose() {

  hudsonwide->close();
}


//  ------------------------------------------------------------------

void GoldWideClose() {

  goldbasewide->close();
}


//  ------------------------------------------------------------------

void HudsExit() {

  if(hudsonwide)
    hudsonwide->exit();
  throw_xrelease(hudsonwide);
}


//  ------------------------------------------------------------------

void GoldExit() {

  if(goldbasewide)
    goldbasewide->exit();
  throw_xrelease(goldbasewide);
}


//  ------------------------------------------------------------------

void HudsInit(const char* path, const char* syspath, int userno, int32_t sizewarn, int ra2usersbbs) {

  // Initialize msgbase-wide data
  hudsonwide = (HudsWide*)throw_calloc(1, sizeof(HudsWide));
  hudsonwide->path = path;
  hudsonwide->syspath = syspath;
  hudsonwide->userno = userno;
  hudsonwide->sizewarn = sizewarn;
  hudsonwide->ra2usersbbs = ra2usersbbs;
  hudsonwide->init();
}


//  ------------------------------------------------------------------

void GoldInit(const char* path, const char* syspath, int userno, int32_t sizewarn, int ra2usersbbs) {

  // Initialize msgbase-wide data
  goldbasewide = (GoldWide*)throw_calloc(1, sizeof(GoldWide));
  goldbasewide->path = path;
  goldbasewide->syspath = syspath;
  goldbasewide->userno = userno;
  NW(sizewarn);
  NW(ra2usersbbs);
  goldbasewide->init();
}


//  ------------------------------------------------------------------
