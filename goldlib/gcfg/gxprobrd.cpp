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
//  Read areas from ProBoard.
//  ------------------------------------------------------------------

#include <cstring>
#include <cstdlib>
#include <gmemdbg.h>
#if defined (__GOLD_GUI__)
    #include <gvidall.h>
    #include <gvidgui.h>
#endif
#undef GCFG_NOPROBOARD
#include <gedacfg.h>
#include <gs_pb200.h>
//  ------------------------------------------------------------------
void gareafile::ReadProBoard(char * tag)
{
    FILE * fp;
    AreaCfg aa;
    char options[80];
    Path file, path;
    *path = NUL;
    strcpy(options, tag);
    char * ptr = strtok(tag, " \t");

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
        ptr = getenv("PB");

        if(ptr)
        {
            AddBackslash(strcpy(path, ptr));
        }
    }

    if(*path == NUL)
    {
        strcpy(path, areapath);
    }

    Config * cfg = (Config *)throw_calloc(1, sizeof(Config));
    MakePathname(file, path, "config.pro");
    fp = fsopen(file, "rb", sharemode);

    if(fp)
    {
        if(not quiet)
        {
            STD_PRINTNL("* Reading " << file);
        }

        fread(cfg, sizeof(Config), 1, fp);
        CfgHudsonpath(cfg->msgpath);
        fclose(fp);
    }

    MakePathname(file, path, "aka.pro");
    uint akasz     = (uint)GetFilesize(file);
    ftn_addr * aka = (ftn_addr *)throw_calloc(1, akasz + sizeof(ftn_addr));
    fp = fsopen(file, "rb", sharemode);

    if(fp)
    {
        if(not quiet)
        {
            STD_PRINTNL("* Reading " << file);
        }

        fread(aka, akasz, 1, fp);
        fclose(fp);
    }

    MsgAreas * area = (MsgAreas *)throw_calloc(1, sizeof(MsgAreas));
    MakePathname(file, path, "msgareas.pb");
    fp = fsopen(file, "rb", sharemode);

    if(fp)
    {
        setvbuf(fp, NULL, _IOFBF, BUFSIZ);

        if(not quiet)
        {
            STD_PRINTNL("* Reading " << file);
        }

        while(fread(area, sizeof(MsgAreas), 1, fp) == 1)
        {
            aa.reset();

            switch(area->msgkind)
            {
                case MSGKIND_LOCAL:
                    aa.type = GMB_LOCAL;
                    aa.attr = attribslocal;
                    break;

                case MSGKIND_NET:
                    aa.type = GMB_NET;
                    aa.attr = attribsnet;
                    break;

                case MSGKIND_ECHO:
                    aa.type = GMB_ECHO;
                    aa.attr = attribsecho;
                    break;

                case MSGKIND_PVTECHO:
                    aa.type = GMB_ECHO;
                    aa.attr = attribsecho;
                    aa.attr.pvt1();
                    break;
            }

            if(area->msgtype == MSGTYPE_PVT)
            {
                aa.attr.pvt1();
            }

            switch(area->msgbasetype)
            {
                case MSGBASE_HUDSON:
                    aa.basetype = "HUDSON";
                    aa.board    = area->areanum;
                    break;

                case MSGBASE_SQUISH:
                    aa.basetype = "SQUISH";
                    aa.setpath(area->path);
                    break;

                case MSGBASE_SDM:
                    aa.basetype = "OPUS";
                    aa.setpath(area->path);
                    break;

                case MSGBASE_JAM:
                    aa.basetype = "JAM";
                    aa.setpath(area->path);
                    break;
            }
            aa.aka = aka[area->aka];
            aa.setdesc(area->name);
            aa.setechoid(area->echotag);
            aa.setorigin(area->origin);
            AddNewArea(aa);
        }
        fclose(fp);
    }

    throw_free(area);
    throw_free(aka);
    throw_free(cfg);
} // gareafile::ReadProBoard

//  ------------------------------------------------------------------
