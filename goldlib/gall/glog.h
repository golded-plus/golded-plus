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

#ifndef __glog_h
#define __glog_h


//  ------------------------------------------------------------------

#include <gfile.h>


//  ------------------------------------------------------------------
//  Log types

const int GLOG_NONE = -1;
const int GLOG_FD   = 0;
const int GLOG_MAX  = 1;
const int GLOG_BINK = 2;
const int GLOG_QBBS = 3;
const int GLOG_DB   = 4;


//  ------------------------------------------------------------------

const int GLOG_STORELINES = 10;


//  ------------------------------------------------------------------
//  Logfile base class

class glog {

private:

  gfile fp;

  static int        count;
  static struct tm* time_now;
  static time_t     secs_now;
  static char       timebuf[20];

protected:

  uint  bufsize;
  int   logtype;
  const char* progname;
  const char* shortprogname;
  int   lineswritten;

public:

  int status;

  char storeline[GLOG_STORELINES][79];
  int  storelines;

  glog();
  ~glog();

  int open(const char* filename, const char* name, const char* shortname=NULL, int type=GLOG_NONE, uint bufsz=0, int shflag=SH_DENYWR);
  void close();

  void init(const char* name=NULL, const char* shortname=NULL, int type=GLOG_NONE);
  void printf(const char* format, ...) __attribute__ ((format (printf, 2, 3)));

  void store();
};


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
