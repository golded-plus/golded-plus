//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
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
//  Read areas from QEcho (by Eugene Sorochinsky)
//  ------------------------------------------------------------------

#include <cstdlib>
#include <gcrcall.h>
#include <gstrall.h>
#undef GCFG_NOQECHO
#include <gedacfg.h>


//  ------------------------------------------------------------------

void gareafile::ReadQEchoFile(char* file, char* options, char* origin) {

  AreaCfg aa;
  char buf[512];

  FILE* fp = fsopen(file, "rb", sharemode);
  if(fp) {
    setvbuf(fp, NULL, _IOFBF, 8192);

    if(not quiet)
      cout << "* Reading " << file << endl;

    while(fgets(buf, sizeof(buf), fp)) {

      char* ptr = strtok(buf, " \t");
      aa.reset();
      aa.groupid = atoi(ptr);

      if((ptr = strtok(NULL, " \t")) != NULL) {
        if(*ptr == '*') {
          // skip ExpireDays
          if((ptr = strtok(NULL, " \t")) == NULL)
            continue;
          if((ptr = strtok(NULL, " \t")) == NULL)
            continue;
        }
        aa.type = GMB_ECHO;
        aa.setechoid(ptr);
        if((ptr = strtok(NULL, " \t")) != NULL)
          if(not strieql("Passthrough", ptr)) {
            aa.setpath(ptr);
            aa.msgbase = GMB_JAM;
            if((ptr = strtok(NULL, " \t")) != NULL)
              if((*ptr == '*') and ((ptr = strtok(NULL, " \t")) != NULL)) {
                CfgAddress(ptr);
                aa.aka.set(ptr);
                aa.attr = attribsecho;
                aa.setorigin(origin);
                AddNewArea(aa);
              }
          }
      }
    }
    fclose(fp);
  }
}


//  ------------------------------------------------------------------
//  Read areas from QEcho (echomail processor)

void gareafile::ReadQEcho(char* tag) {

  char origin[80];
  char options[80];
  Path file;

  *origin = NUL;
  *file = NUL;
  strcpy(options, tag);
  char* ptr = strtok(tag, " \t");
  while(ptr) {
    if(*ptr != '-')
      strcpy(file, ptr);
    ptr = strtok(NULL, " \t");
  }

  if(not *file)
    strcpy(file, "/etc/qecho/AreaList");

  ReadQEchoFile(file, options, origin);
}


//  ------------------------------------------------------------------
