
//  ------------------------------------------------------------------
//  GoldED+
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999-2000 Alexander S. Aganichev
//  ------------------------------------------------------------------
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License as
//  published by the Free Software Foundation; either version 2 of the
//  License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston,
//  MA 02111-1307 USA
//  ------------------------------------------------------------------
//  $Id$
//  ------------------------------------------------------------------
//  Product name and version stuff.
//  ------------------------------------------------------------------

#include <golded.h>


//  ------------------------------------------------------------------

#define __GVER_PID__(A,SIZE) __GVER_PRE##SIZE##NAME__ __GVER_##SIZE##NAME__ __GVER_POST##SIZE##NAME__ __GVER_##SIZE##PLATFORM__
#define __GVER_VER__(A,B,C) __GVER_PREVERSION__ #A "." #B "." #C __GVER_POSTVERSION__
#define __GVER_SVER__(A,B,C) #A "." #B "." #C

#define __GVER__(__GVER_MAJOR__,__GVER_MINOR__,__GVER_RELEASE__) \
int         __gver_major__    = __GVER_MAJOR__; \
int         __gver_minor__    = __GVER_MINOR__; \
int         __gver_release__  = __GVER_RELEASE__; \
const char* __gver_longpid__  = __GVER_PID__(,); \
const char* __gver_shortpid__ = __GVER_PID__(,SHORT); \
const char* __gver_ver__      = __GVER_VER__(__GVER_MAJOR__,__GVER_MINOR__,__GVER_RELEASE__); \
const char* __gver_shortver__ = __GVER_SVER__(__GVER_MAJOR__,__GVER_MINOR__,__GVER_RELEASE__);


//  ------------------------------------------------------------------
//  Product name.

#define __GVER_NAME__           "GoldED"
#define __GVER_SHORTNAME__      "GED"


//  ------------------------------------------------------------------
//  Vendor data displayed on the startup screen.
//  Define in "mygolded.h" if you want.

#define __GVER_VENDOR_NAME__ "Anonymous"
#define __GVER_VENDOR_FIDO__ ""
#define __GVER_VENDOR_EMAIL__ ""
#define __GVER_RELEASENAME__ ""


//  ------------------------------------------------------------------
//  Strings prepended and appended to the product name.
//  Define in "mygolded.h" if you want.

#define __GVER_PRENAME__       ""
#define __GVER_POSTNAME__      ""
#define __GVER_PRESHORTNAME__  ""
#define __GVER_POSTSHORTNAME__ ""


//  ------------------------------------------------------------------
//  Strings prepended and appened to the version numbers.
//  Define in "mygolded.h" if you want.

#define __GVER_PREVERSION__    ""
#define __GVER_POSTVERSION__   ""


//  ------------------------------------------------------------------
//  Platform specific strings.

#if defined(__MSDOS__)
#define __GVER_PLATFORM__      "/386"
#define __GVER_SHORTPLATFORM__ "386"
#define __GVER_SHORTLOGNAME__  "X"
#define __GVER_CFGEXT__        ".gex"
#elif defined(__EMX__)
#define __GVER_PLATFORM__      "/EMX"
#define __GVER_SHORTPLATFORM__ "2"
#define __GVER_SHORTLOGNAME__  "2"
#define __GVER_CFGEXT__        ".geo"
#elif defined(__OS2__)
#define __GVER_PLATFORM__      "/2"
#define __GVER_SHORTPLATFORM__ "2"
#define __GVER_SHORTLOGNAME__  "2"
#define __GVER_CFGEXT__        ".geo"
#elif defined(__WIN32__)
#define __GVER_PLATFORM__      "/W32"
#define __GVER_SHORTPLATFORM__ "W32"
#define __GVER_SHORTLOGNAME__  "W"
#define __GVER_CFGEXT__        ".gew"
#elif defined(__linux__)
#define __GVER_PLATFORM__      "/LNX"
#define __GVER_SHORTPLATFORM__ "LNX"
#define __GVER_SHORTLOGNAME__  "L"
#define __GVER_CFGEXT__        ".gel"
#elif defined(__FreeBSD__) || defined(__OpenBSD__)
#define __GVER_PLATFORM__      "/BSD"
#define __GVER_SHORTPLATFORM__ "BSD"
#define __GVER_SHORTLOGNAME__  "B"
#define __GVER_CFGEXT__        ".geb"
#elif defined(__BEOS__)
#define __GVER_PLATFORM__      "/BeOS"
#define __GVER_SHORTPLATFORM__ "BeOS"
#define __GVER_SHORTLOGNAME__  "B"
#define __GVER_CFGEXT__        ".gbe"
#elif defined(__UNIX__)
#define __GVER_PLATFORM__      "/UNX"
#define __GVER_SHORTPLATFORM__ "UNX"
#define __GVER_SHORTLOGNAME__  "U"
#define __GVER_CFGEXT__        ".geu"
#else
#define __GVER_PLATFORM__      ""
#define __GVER_SHORTPLATFORM__ ""
#define __GVER_SHORTLOGNAME__  "D"
#define __GVER_CFGEXT__        ".ged"
#endif

//  ------------------------------------------------------------------

// Include your personalized strings here:
#include <mygolded.h>


//  ------------------------------------------------------------------
//  Global variables

const char* __gver_vendor_name__   = __GVER_VENDOR_NAME__;
const char* __gver_vendor_fido__   = __GVER_VENDOR_FIDO__;
const char* __gver_vendor_email__  = __GVER_VENDOR_EMAIL__;
const char* __gver_prename__       = __GVER_PRENAME__;
const char* __gver_preshortname__  = __GVER_PRESHORTNAME__;
const char* __gver_name__          = __GVER_NAME__;
const char* __gver_shortname__     = __GVER_SHORTNAME__;
const char* __gver_shortlogname__  = __GVER_SHORTNAME__ __GVER_SHORTLOGNAME__;
const char* __gver_postshortname__ = __GVER_POSTSHORTNAME__;
const char* __gver_postname__      = __GVER_POSTNAME__;
const char* __gver_platform__      = __GVER_PLATFORM__;
const char* __gver_shortplatform__ = __GVER_SHORTPLATFORM__;
const char* __gver_cfgext__        = __GVER_CFGEXT__;
const char* __gver_preversion__    = __GVER_PREVERSION__;
const char* __gver_postversion__   = __GVER_POSTVERSION__;
const char* __gver_releasename__   = __GVER_RELEASENAME__;
const char* __gver_date__          = __DATE__;
const char* __gver_time__          = __TIME__;


//  ------------------------------------------------------------------
