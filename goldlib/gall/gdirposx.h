//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
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
//  Directory maintenance class.
//  ------------------------------------------------------------------

#ifndef __gdirposx
#define __gdirposx


//  ------------------------------------------------------------------

#include <gfilutil.h>
#include <gstrarr.h>


//  ------------------------------------------------------------------

class gdirentry {

public:
	string name;
	struct stat stat_info;
	const char *dirname;
	gdirentry();
	~gdirentry();
	bool is_directory() const { return S_ISDIR(stat_info.st_mode); }
	bool is_file() const { return S_ISREG(stat_info.st_mode); }
};


//  ------------------------------------------------------------------

class gposixdir {

private:
	string dirname;
	gdirentry ret;
	gstrarray entries;
	unsigned long last_entry;

public:
	bool ok;
	gposixdir();
	gposixdir(const char *name);
	~gposixdir();
	void cd(const char *name, bool relative=false);
	inline void rewind() { last_entry = 0; }
	const gdirentry *nextentry(const char *mask=NULL, bool nameonly=false);
	inline const char *fullpath() { return dirname.c_str(); }
};


//  ------------------------------------------------------------------

#endif // __gdirposx

//  ------------------------------------------------------------------
