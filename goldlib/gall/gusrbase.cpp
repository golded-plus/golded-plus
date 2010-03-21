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
//  Userfile base class implementation.
//  ------------------------------------------------------------------

#include <gfilutil.h>
#include <gstrall.h>
#include <gwildmat.h>
#include <gusrbase.h>


//  ------------------------------------------------------------------

GUser::GUser()
{
  gufh    = -1;
  found   = false;
  index   = 0;
  name    = NULL;
  recno   = 0;
  records = 0;
  recptr  = NULL;
  recsize = 0;
}


//  ------------------------------------------------------------------

GUser::~GUser() {

  // No action
}


//  ------------------------------------------------------------------

void GUser::founduser() {

  index = recno;
  found = true;
}


//  ------------------------------------------------------------------

void GUser::inctimesposted(int) {

  // No action
}


//  ------------------------------------------------------------------

uint32_t GUser::lastread() {

  // No action
  return 0;
}


//  ------------------------------------------------------------------

void GUser::lastread(uint32_t) {

  // No action
}


//  ------------------------------------------------------------------

void GUser::recinit(const char* __name) {
  
  memset(recptr, 0, recsize);
  strcpy(name, __name);
}


//  ------------------------------------------------------------------

int GUser::moveto(uint __rec)
{
  if (gufh != -1)
  {
    if(__rec <= records) {
      lseek(gufh, (long)__rec*(long)recsize, SEEK_SET);
      recno = __rec;
      return true;
    }
  }
  recno = records;
  return false;
}


//  ------------------------------------------------------------------

int GUser::next() {

  return moveto(++recno);
}


//  ------------------------------------------------------------------

int GUser::prev() {
                      
  return moveto(--recno);
}


//  ------------------------------------------------------------------

void GUser::seekread()
{
  if (gufh != -1)
  {
    lseek(gufh, (long)recno*(long)recsize, SEEK_SET);
    ::read(gufh, recptr, recsize);
  }
}

                      
//  ------------------------------------------------------------------

void GUser::seekwrite()
{
  if (gufh != -1)
  {
    lseek(gufh, (long)recno*(long)recsize, SEEK_SET);
    ::write(gufh, recptr, recsize);
  }
}


//  ------------------------------------------------------------------

int GUser::find(const char* __name, char* __result, int __wildcards) {

  // Init variables
  index = 0;
  recno = 0;
  found = false;

  // If userfile is open
  if (gufh != -1)
  {
    // Rewind file to start
    lseek(gufh, 0, SEEK_SET);

    // Get number of records in the userfile
    records = (uint)(filelength(gufh)/recsize);

    // Searching loop
    for(recno=0; recno<records; recno++) {

      // Read a user record
      if(read()) {

        // Compare our name with name in user record
        if(__wildcards) {

          // Compare for wildcard match
          if(strwild(name, __name)) {

            // Copy the name we found
            if(__result)
              strcpy(__result, name);
            founduser();
            break;
          }
        }
        else {

          // Compare for exact, case-insensitive, match
          if(strieql(name, __name)) {
            founduser();
            break;
          }
        }
      }
    }
  }

  // Report the result
  return found;
}


//  ------------------------------------------------------------------

int GUser::findwild(const char* __name, char* __result) {

  return find(__name, __result, true);
}


//  ------------------------------------------------------------------

void GUser::add(const char* __name) {

  recinit(__name);
  recno = records++;
  seekwrite();
  moveto(recno);
  read();
  founduser();
}


//  ------------------------------------------------------------------
