//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
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
//  Read areas from an AREAS.BBS file (any type!)
//  ------------------------------------------------------------------

#include <cstdlib>
#include <cstring>
#include <gedacfg.h>


//  ------------------------------------------------------------------
//  Read one or more AREAS.BBS files

void gareafile::ReadAreasBBS(char* tag)
{

    AreaCfg aa;
    int echos;
    char origin[80], options[80];
    char* ptr;
    char* echoid=NULL;
    char* path=NULL;
    char* desc=NULL;

    *origin = NUL;
    strcpy(options, tag);
    ptr = strtok(tag, " \t");

    // Read each AREAS.BBS
    while(ptr)
    {
        if(*ptr != '-')
        {
            int echos_before = echolist.Echos();
            GetAreasBBS(ptr, origin, options);
            echos = echolist.Echos();
            for(int n=echos_before; n<echos; n++)
            {
                echolist.GetEcho(n, &echoid, &path, &desc);
                aa.reset();
                aa.type = GMB_ECHO;
                aa.attr = attribsecho;
                aa.board = atoi(path);
                if(aa.board and (aa.board < 201))
                    aa.basetype = "HUDSON";
                else if((aa.board > 200) and (aa.board < 501))
                    aa.basetype = "GOLDBASE";
                else if(*path == '$')
                {
                    aa.basetype = "SQUISH";
                    adjustpath(path+1);
                    aa.setpath(path+1);
                }
                else if(*path == '!')
                {
                    aa.basetype = "JAM";
                    adjustpath(path+1);
                    aa.setpath(path+1);
                }
                else if((path[0] == 'P') and (path[1] == ' '))
                {
                    aa.basetype = "WILDCAT";
                    adjustpath(path+2);
                    aa.setpath(path+2);
                }
                else if(strnieql(path, "BBS ", 4))
                {
                    aa.basetype = "WILDCAT";
                    adjustpath(path+4);
                    aa.setpath(path+4);
                }
                else if(*path == '!')
                {
                    aa.basetype = "JAM";
                    adjustpath(path+1);
                    aa.setpath(path+1);
                }
                else
                {
                    aa.basetype = fidomsgtype;
                    adjustpath(path);
                    aa.setpath(path);
                }
                aa.setdesc(desc);
                aa.setechoid(echoid);
                aa.setorigin(origin);
                AddNewArea(aa);
            }
        }
        ptr = strtok(NULL, " \t");
    }
}


//  ------------------------------------------------------------------
