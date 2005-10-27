
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
  #endif

#endif /* GEDCFG2 */

#ifndef GOLDAREA_INC
#define GOLDAREA_INC "goldarea.inc"
#endif

#endif /* __GEFN_H__ */
