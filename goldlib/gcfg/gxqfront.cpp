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
//  Read areas from QFront.
//  ------------------------------------------------------------------

#include <cstdlib>
#include <gmemdbg.h>
#include <gstrall.h>
#if defined(__GOLD_GUI__)
    #include <gvidall.h>
    #include <gvidgui.h>
#endif
#undef GCFG_NOQFRONT
#include <gedacfg.h>
#include <gs_qfrnt.h>


//  ------------------------------------------------------------------

void gareafile::ReadQFront(char* tag)
{

    FILE* fp;
    AreaCfg aa;
    char options[80];
    Path file, path;

    *path = NUL;
    strcpy(options, tag);
    char* ptr = strtok(tag, " \t");
    while(ptr)
    {
        if(*ptr != '-')
        {
            AddBackslash(strcpy(path, ptr));
            break;
        }
        ptr = strtok(NULL, " \t");
    }
    if(*path == NUL)
    {
        ptr = getenv("QFRONT");
        if(ptr)
            AddBackslash(strcpy(path, ptr));
    }
    if(*path == NUL)
        strcpy(path, areapath);

    OriginLineRecord* origin = (OriginLineRecord*)throw_calloc(1, sizeof(OriginLineRecord));
    MakePathname(file, path, "qorigin.dat");
    fp = fsopen(file, "rb", sharemode);
    if (fp)
    {
        if (not quiet)
            STD_PRINTNL("* Reading " << file);

        fread(origin, sizeof(OriginLineRecord), 1, fp);
        for(int n=0; n<MaxOrigins; n++)
            STRNP2C(origin->OriginLine[n]);
        fclose(fp);
    }

    EchoMailConferenceRecord* area = (EchoMailConferenceRecord*)throw_calloc(1, sizeof(EchoMailConferenceRecord));
    MakePathname(file, path, "qechos.dat");
    fp = fsopen(file, "rb", sharemode);
    if (fp)
    {
        setvbuf(fp, NULL, _IOFBF, 8192);

        if (not quiet)
            STD_PRINTNL("* Reading " << file);

        while(fread(area, sizeof(EchoMailConferenceRecord), 1, fp) == 1)
        {
            if(not area->Deleted)
            {
                aa.reset();
                aa.type = GMB_ECHO;
                aa.attr = attribsecho;
                STRNP2C(area->AreaName);
                aa.board = area->ConfNum;
                aa.basetype = "PCBOARD";
                aa.setechoid(area->AreaName);
                aa.setorigin(origin->OriginLine[area->OriginLine]);
                AddNewArea(aa);
            }
        }

        fclose(fp);
    }

    throw_free(origin);
    throw_free(area);
}


//  ------------------------------------------------------------------
