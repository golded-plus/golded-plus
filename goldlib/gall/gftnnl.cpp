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
//  Nodelist index.
//  ------------------------------------------------------------------

#include <cstdio>
#include <gstrall.h>
#include <gftnnl.h>
#include <stdlib.h>

//  ------------------------------------------------------------------

void ftn_nodelist_entry::unpack(char* line) {

  *status = NUL;
  *system = NUL;
  *location = NUL;
  *name = NUL;
  *phone = NUL;
  *baud = NUL;
  *flags = NUL;

  strchg(strtrim(line), '_', ' ');

  char* q = line;
  char* p = strchr(line, ',');
  if(p) {
    *p++ = NUL;
    strxcpy(status, q, sizeof(status));
    p = strchr((q=p), ',');
    if(p) {
      *p++ = NUL;
      // Skip over number
      p = strchr((q=p), ',');
      if(p) {
        *p++ = NUL;
        strxcpy(system, q, sizeof(system));
        p = strchr((q=p), ',');
        if(p) {
          *p++ = NUL;
          strxcpy(location, q, sizeof(location));
          p = strchr((q=p), ',');
          if(p) {
            *p++ = NUL;
            strxcpy(name, q, sizeof(name));
            p = strchr((q=p), ',');
            if(p) {
              *p++ = NUL;
              strxcpy(phone, q, sizeof(phone));
              p = strchr((q=p), ',');
              sprintf(baud, "%lu", atol(q));
              if(p)
                strxcpy(flags, p+1, sizeof(flags));
            }
          }
        }
      }
    }
  }
}


//  ------------------------------------------------------------------

ftn_nodelist_entry& ftn_nodelist_entry::operator=(const ftn_nodelist_entry& e) {

  addr = e.addr;
  strcpy(address,  e.address);
  strcpy(name,     e.name);
  strcpy(status,   e.status);
  strcpy(system,   e.system);
  strcpy(location, e.location);
  strcpy(phone,    e.phone);
  strcpy(baud,     e.baud);
  strcpy(flags,    e.flags);

  return *this;
}


//  ------------------------------------------------------------------

