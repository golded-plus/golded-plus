//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 2000 Alexander S. Aganichev
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
//  Log file class.
//  ------------------------------------------------------------------

#include <cstdarg>
#include <gtimall.h>
#include <gstrall.h>
#include <gdbgerr.h>
#include <glog.h>


//  ------------------------------------------------------------------

struct tm* glog::time_now;
int        glog::count = 0;
time_t     glog::secs_now;
char       glog::timebuf[20];


//  ------------------------------------------------------------------

glog::glog() {

  bufsize = 0;
  logtype = GLOG_FD;
  progname = NULL;
  status = GOLD_NO_ERROR;
  storelines = -1;
}


//  ------------------------------------------------------------------

glog::~glog() {

  close();
}


//  ------------------------------------------------------------------

int glog::open(const char* filename, const char* name, const char* shortname, int type, uint bufsz, int shflag) {

  fp.fopen(filename, "at", shflag);
  if(fp.status) {
    status = fp.status;
    return status;
  }

  count++;
  bufsize = bufsz;
  fp.setvbuf(NULL, bufsize ? _IOFBF : _IONBF, bufsize);

  init(name, shortname, type);

  return status;
}


//  ------------------------------------------------------------------

void glog::close() {

  fp.fclose();
  count--;
}


//  ------------------------------------------------------------------

void glog::init(const char* name, const char* shortname, int type) {

  lineswritten = 0;
  logtype = (type!=GLOG_NONE) ? type : logtype;
  progname = name ? name : progname;
  shortprogname = shortname ? shortname : (shortprogname ? shortprogname : progname);
}


//  ------------------------------------------------------------------

void glog::printf(const char* format, ...) {

  va_list argptr;
  char buf[256];
  char logbuf[256];

  secs_now = time(NULL);
  time_now = glocaltime(&secs_now);

  lineswritten++;

  if(lineswritten == 1) {

    switch(logtype) {

      case GLOG_FD:
        sprintf(logbuf, "\n----------  %s, %s\n", strftimei(timebuf, 20, "%a %d %b %y", time_now), progname);
        break;

      case GLOG_MAX:
        sprintf(logbuf, "\n+ %s %4.4s Begin, %s\n", strftimei(timebuf, 20, "%d %b %H:%M:%S", time_now), shortprogname, progname);
        break;

      case GLOG_BINK:
        sprintf(logbuf, "\n> %s %4.4s %s\n", strftimei(timebuf, 20, "%d-%b %H:%M:%S", time_now), shortprogname, progname);
        break;

      case GLOG_QBBS:
        sprintf(logbuf, "\n%s  **************************************************\n%s  %s\n", strftimei(timebuf, 20, "%d-%b-%y %H:%M", time_now), timebuf, progname);
        break;

      case GLOG_DB:
        sprintf(logbuf, "\n%s  %s\n", strftimei(timebuf, 20, "%m/%d/%y %H:%M", time_now), progname);
        break;
    }

    if(fp.isopen())
      fp.printf("%s", logbuf);
  }

  *buf = NUL;
  va_start(argptr, format);
  vsprintf(buf, format, argptr);
  va_end(argptr);

  if(*buf == '!')
    store();

  switch(logtype) {

    case GLOG_FD:
      sprintf(logbuf, "%c %s  %s", *buf, strftimei(timebuf, 10, "%H:%M:%S", time_now), buf+2);
      break;

    case GLOG_MAX:
      sprintf(logbuf, "%c %s %4.4s %s", *buf, strftimei(timebuf, 20, "%d %b %H:%M:%S", time_now), shortprogname, buf+2);
      break;

    case GLOG_BINK:
      sprintf(logbuf, "%c %s %4.4s %s", *buf, strftimei(timebuf, 20, "%d-%b %H:%M:%S", time_now), shortprogname, buf+2);
      break;

    case GLOG_QBBS:
      sprintf(logbuf, "%s  %s", strftimei(timebuf, 20, "%d-%b-%y %H:%M", time_now), buf+2);
      break;

    case GLOG_DB:
      sprintf(logbuf, "%s  %s", strftimei(timebuf, 20, "%m/%d/%y %H:%M", time_now), buf+2);
      break;
  }
  if(fp.isopen()) {
    fp.printf("%s\n", logbuf);
    fp.fflush();
  }
  if(storelines != -1) {
    if(storelines < GLOG_STORELINES)
      strxcpy(storeline[storelines], logbuf, 79);
    storelines++;
  }
}


//  ------------------------------------------------------------------

void glog::store() {

  if(storelines == -1)
    storelines = 0;
}


//  ------------------------------------------------------------------
