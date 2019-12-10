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
//  Read areas from QuickBBS (old and new)
//  ------------------------------------------------------------------

#include <cstdlib>
#include <gstrall.h>
#include <gmemdbg.h>
#if defined (__GOLD_GUI__)
#include <gvidall.h>
#include <gvidgui.h>
#endif
#undef GCFG_NOQBBS
#include <gedacfg.h>
#include <gs_qbbs.h>
//  ------------------------------------------------------------------
//  Read QuickBBS 2.60 (old) area config
void gareafile::ReadQ260(char * qbpath, char * origin, char * options)
{
    AreaCfg aa;
    FILE * fp;
    Path file;
    Q260BrdRecP brd;
    Q260CfgRecP cfg;

    cfg = new Q260CfgRecT;
    throw_new(cfg);
    brd = cfg->Boards;
    MakePathname(file, qbpath, "config.bbs");
    fp = fsopen(file, "rb", sharemode);

    if(fp)
    {
        if(not quiet)
        {
            STD_PRINTNL("* Reading " << file);
        }

        fread(cfg, sizeof(Q260CfgRecT), 1, fp);
        STRNP2C(cfg->OriginLine);

        for(int n = 0; n < 200; n++)
        {
            if(*brd[n].Name)
            {
                aa.reset();
                aa.board    = n + 1;
                aa.basetype = "HUDSON";
                aa.setdesc(STRNP2C(brd[n].Name));
                aa.setorigin(*cfg->OriginLine ? cfg->OriginLine : origin);

                if(cfg->UseAka[n] == 0)
                {
                    aa.aka.zone  = cfg->MatrixZone;
                    aa.aka.net   = cfg->MatrixNet;
                    aa.aka.node  = cfg->MatrixNode;
                    aa.aka.point = cfg->MatrixPoint;
                }
                else
                {
                    aa.aka.zone  = cfg->AkaZone[cfg->UseAka[n] - 1];
                    aa.aka.net   = cfg->AkaNet[cfg->UseAka[n] - 1];
                    aa.aka.node  = cfg->AkaNode[cfg->UseAka[n] - 1];
                    aa.aka.point = cfg->AkaPoint[cfg->UseAka[n] - 1];
                }

                switch(brd[n].Typ)
                {
                    case 0:
                        aa.type = GMB_LOCAL;
                        aa.attr = attribslocal;
                        break;

                    case 1:
                        aa.type = GMB_NET;
                        aa.attr = attribsnet;
                        break;

                    default:
                        aa.type = GMB_ECHO;
                        aa.attr = attribsecho;
                }

                switch(brd[n].Kinds)
                {
                    case 1:
                        aa.attr.pvt1();
                        break;

                    case 0:
                    case 2:
                        aa.attr.pvt0();
                        break;

                    case 3:
                        aa.attr.r_o1();
                        break;
                }
                AddNewArea(aa);
            }
        }
        fclose(fp);
    }

    throw_delete(cfg);
} // gareafile::ReadQ260

//  ------------------------------------------------------------------
//  Read QuickBBS 2.76.G2 (new) area config
void gareafile::ReadQ276(char * qbpath, char * origin, char * options)
{
    AreaCfg aa;
    FILE * fp;
    Path file;
    Q276BrdRecP brd;
    Q276CfgRecP cfg;

    brd = new Q276BrdRecT;
    throw_new(brd);
    cfg = new Q276CfgRecT;
    throw_new(cfg);
    MakePathname(file, qbpath, "quickcfg.dat");
    fp = fsopen(file, "rb", sharemode);

    if(fp)
    {
        if(not quiet)
        {
            STD_PRINTNL("* Reading " << file);
        }

        fread(cfg, sizeof(Q276CfgRecT), 1, fp);
        STRNP2C(cfg->MsgPath);
        CfgHudsonpath(cfg->MsgPath);
        fclose(fp);
    }

    MakePathname(file, qbpath, "MSGCFG.DAT");
    fp = fsopen(file, "rb", sharemode);

    if(fp)
    {
        if(not quiet)
        {
            STD_PRINTNL("* Reading " << file);
        }

        int _recs = (int)(filelength(fileno(fp)) / sizeof(Q276BrdRecT));
//    int _fmt = (_recs > 200) ? GMB_GOLDBASE : GMB_HUDSON;
        const char * _fmt = (_recs > 200) ? "GOLDBASE" : "HUDSON";

        for(int n = 0; n < _recs; n++)
        {
            fread(brd, sizeof(Q276BrdRecT), 1, fp);

            if(*brd->Name)
            {
                aa.reset();
                aa.board    = n + 1;
                aa.basetype = _fmt;
                //aa.groupid = brd->Group;
                aa.setorigin(*brd->OriginLine ? STRNP2C(brd->OriginLine) : origin);
                aa.aka.zone  = cfg->MatrixZone[brd->Aka];
                aa.aka.net   = cfg->MatrixNet[brd->Aka];
                aa.aka.node  = cfg->MatrixNode[brd->Aka];
                aa.aka.point = cfg->MatrixPoint[brd->Aka];
                aa.setdesc(STRNP2C(brd->Name));

                switch(brd->Typ)
                {
                    case 0:
                        aa.type = GMB_LOCAL;
                        aa.attr = attribslocal;
                        break;

                    case 1:
                        aa.type = GMB_NET;
                        aa.attr = attribsnet;
                        break;

                    default:
                        aa.type = GMB_ECHO;
                        aa.attr = attribsecho;
                }

                switch(brd->Kinds)
                {
                    case 1:
                        aa.attr.pvt1();
                        break;

                    case 0:
                    case 2:
                        aa.attr.pvt0();
                        break;

                    case 3:
                        aa.attr.r_o1();
                        break;
                }
                AddNewArea(aa);
            }
        }
        fclose(fp);
    }

    throw_delete(cfg);
    throw_delete(brd);
} // gareafile::ReadQ276

//  ------------------------------------------------------------------
//  Read QuickBBS (old or new) configuration
void gareafile::ReadQuickBBS(char * tag)
{
    char * ptr;
    Path qbpath;
    char origin[80], options[80];

    ptr = getenv("QUICKBBS");

    if(ptr == NULL)
    {
        ptr = getenv("QBBS");
    }

    if(ptr)
    {
        AddBackslash(strcpy(qbpath, ptr));
    }
    else
    {
        strcpy(qbpath, areapath);
    }

    // Read AREAS.BBS
    *qbpath = NUL;
    *origin = NUL;
    strcpy(options, tag);
    ptr = strtok(tag, " \t");

    while(ptr)
    {
        if(*ptr != '-')
        {
            if(is_dir(ptr) and (*qbpath == NUL))
            {
                AddBackslash(strcpy(qbpath, ptr));
            }
            else
            {
                GetAreasBBS(ptr, origin, options);
            }
        }

        ptr = strtok(NULL, " \t");
    }

    // Detect general version range of QuickBBS
    if(fexist(AddPath(qbpath, "quickcfg.dat")))
    {
        ReadQ276(qbpath, origin, options);
    }
    else
    {
        ReadQ260(qbpath, origin, options);
    }
} // gareafile::ReadQuickBBS

//  ------------------------------------------------------------------
