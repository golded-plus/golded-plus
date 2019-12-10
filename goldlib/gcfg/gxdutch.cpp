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
//  Read areas from Dutchie 2.9x
//  ------------------------------------------------------------------

#include <cstdlib>
#include <gstrall.h>
#if defined (__GOLD_GUI__)
#include <gvidall.h>
#include <gvidgui.h>
#endif
#undef GCFG_NODUTCHIE
#include <gedacfg.h>
//  ------------------------------------------------------------------
//  Read Dutchie DUTCHIE.ARE
void gareafile::ReadDutchie(char * tag)
{
    AreaCfg aa;
    char * ptr;
    char * ptr2;
    Path dupath, file;
    char buf[256], options[80];

    *dupath = NUL;
    strcpy(options, tag);
    ptr = strtok(tag, " \t");

    while(ptr)
    {
        if(*ptr != '-')
        {
            AddBackslash(strcpy(dupath, ptr));
            break;
        }

        ptr = strtok(NULL, " \t");
    }

    if(*dupath == NUL)
    {
        ptr = getenv("DUTCHIE");

        if(ptr)
        {
            AddBackslash(strcpy(dupath, ptr));
        }
    }

    if(*dupath == NUL)
    {
        strcpy(dupath, areapath);
    }

    MakePathname(file, dupath, "dutchie.are");
    gfile fp(file, "rb", sharemode);

    if(fp.isopen())
    {
        fp.SetvBuf(NULL, _IOFBF, 8192);

        if(not quiet)
        {
            STD_PRINTNL("* Reading " << file);
        }

        while(fp.Fgets(buf, 255))
        {
            ptr = strbtrim(buf);

            if(*ptr != ';' and * ptr !=
               '-' and strnicmp(ptr, "PASSTHRU", 8) and * ptr != '\x1A' and * ptr)
            {
                aa.reset();
                aa.basetype = "FTS1";
                ptr         = strtok(ptr, " \t"); // Get path
                aa.setpath(ptr);
                ptr = strtok(NULL, " \t"); // Get description
                aa.setdesc(ptr);
                strchg(aa.desc, '_', ' ');
                aa.type = GMB_ECHO;
                aa.attr = attribsecho;
                ptr2    = strtok(NULL, " \t"); // Skip flags field
                ptr     = strtok(NULL, " \t"); // Get echoid

                if(ptr == NULL)
                {
                    ptr = ptr2; // Echoid field missing - use string in flags field
                }

                aa.setechoid(strbtrim(ptr));
                AddNewArea(aa);
            }
        }
    }
} // gareafile::ReadDutchie

//  ------------------------------------------------------------------
