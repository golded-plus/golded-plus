
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
//  Standart filenames.
//  ------------------------------------------------------------------


#ifndef __GEFN_H__
#define __GEFN_H__

#ifndef GEDCFG
    #define GEDCFG "golded.cfg"
#endif /* GEDCFG */

#ifndef GEDCFG2

    #if defined(__OS2__)
        #define GEDCFG2 "ged2.cfg"
    #elif defined(__WIN32__)
        #define GEDCFG2 "gedw32.cfg"
    #elif defined(__DOS__) || defined(__MSDOS__)
        #define GEDCFG2 "geddos.cfg"
    #elif defined(__UNIX__)
        #define GEDCFG2 "golded.conf"
    #endif

#endif /* GEDCFG2 */

#ifndef GOLDAREA_INC
    #define GOLDAREA_INC "goldarea.inc"
#endif

#ifndef GOLDED_MSG
    #define GOLDED_MSG "golded.msg"
#endif

#ifndef GOLDHELP_CFG
    #define GOLDHELP_CFG "goldhelp.cfg"
#endif

#ifndef GOLDKEYS_CFG
    #define GOLDKEYS_CFG "goldkeys.cfg"
#endif

#ifndef GOLDLANG_CFG
    #define GOLDLANG_CFG "goldlang.cfg"
#endif

#ifndef GOLDLAST_LST
    #define GOLDLAST_LST "goldlast.lst"
#endif

#ifndef GOLDUSER_LST
    #define GOLDUSER_LST "golduser.lst"
#endif

#ifndef GOLDED_CFM
    #define GOLDED_CFM "golded.cfm"
#endif

#ifndef GOLDXLAT
    #define GOLDXLAT "goldxlat"
#endif

#ifndef GOLDED_LOG
    #define GOLDED_LOG "golded.log"
#endif

#ifndef NAMES_FD
    #define NAMES_FD "names.fd"
#endif

#ifndef GOLDED_LST
    #define GOLDED_LST "golded.lst"
#endif

#ifndef FIDOLASTREAD
    #define FIDOLASTREAD "lastread"
#endif

#ifndef CFGUSERPATH1
    #ifdef __UNIX__
        #define CFGUSERPATH1 "~/fido/etc/" /* Trailing slash: this is directory */
    #endif
#endif

#ifndef CFGUSERPATH2
    #ifdef __UNIX__
        #define CFGUSERPATH2 "~/ftn/etc/" /* Trailing slash: this is directory */
    #endif
#endif

#ifndef CFGPATH
    #ifdef __UNIX__
        #define CFGPATH "~/.golded/" /* Trailing slash: this is directory */
    #endif
#endif

#endif /* __GEFN_H__ */
