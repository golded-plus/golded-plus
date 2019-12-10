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
//  Read areas from WaterGate 0.93 (and newer)
//  ------------------------------------------------------------------

#include <cstdlib>
#include <gstrall.h>
#include <gmemdbg.h>
#if defined (__GOLD_GUI__)
#include <gvidall.h>
#include <gvidgui.h>
#endif
#undef GCFG_NOWATERGATE
#include <gedacfg.h>
#include <gs_wtr.h>
//  ------------------------------------------------------------------
const char * gareafile::gettype(const char * msgtype, const byte wtrtype)
{
    switch(wtrtype)
    {
        case 1:
            return msgtype;

        case 2:
            return "SQUISH";

        case 3:
            return "JAM";

        case 4:
            return "WILDCAT";
    }
    return "";
}

//  ------------------------------------------------------------------
//  Read WaterGate v0.93
void gareafile::ReadWtrGteFile(char * options, gfile & fp)
{
    AreaCfg aa;
    ConfigRecord * _tmp = new ConfigRecord;

    throw_new(_tmp);
    ConfigRecord & c = *_tmp;
    fp.Fread(&c, sizeof(ConfigRecord));
    strp2c(c.systemdir);
    CfgJampath(c.systemdir);
    MapPath(c.systemdir);
    strp2c(c.origins[0]);
    strp2c(c.origins[1]);

    if(c.fidonetmailtype)
    {
        aa.reset();
        aa.basetype = gettype(c.opusdateformat ? "OPUS" : "FTS1", c.fidonetmailtype);
        aa.type     = GMB_NET;
        aa.attr     = attribsnet;
        aa.aka      = CAST(ftn_addr, c.nodenrs[0]);
        aa.setpath(strp2c(c.fidonetmailpath));
        aa.setdesc("WaterGate Netmail");
        aa.setautoid("NETMAIL");
        AddNewArea(aa);
    }

    if(c.fidobadareatype)
    {
        aa.reset();
        aa.basetype = gettype(c.opusdateformat ? "OPUS" : "FTS1", c.fidobadareatype);
        aa.type     = GMB_ECHO;
        aa.attr     = attribsecho;
        aa.aka      = CAST(ftn_addr, c.nodenrs[0]);
        aa.setpath(strp2c(c.fidobadpath));
        aa.setdesc("WaterGate Badmail");
        aa.setautoid("BADMAIL");
        AddNewArea(aa);
    }

    if(c.fidodupeareatype)
    {
        aa.reset();
        aa.basetype = gettype(c.opusdateformat ? "OPUS" : "FTS1", c.fidodupeareatype);
        aa.type     = GMB_NET;
        aa.attr     = attribsnet;
        aa.aka      = CAST(ftn_addr, c.nodenrs[0]);
        aa.setpath(strp2c(c.fidodupepath));
        aa.setdesc("WaterGate Dupemail");
        aa.setautoid("DUPEMAIL");
        AddNewArea(aa);
    }

    FILE * fp2;
    Path file;
    MakePathname(file, c.systemdir, "areabase.tdb");
    fp2 = fsopen(file, "rb", sharemode);

    if(fp2)
    {
        char header[26];

        if(not quiet)
        {
            STD_PRINTNL("* Reading " << file);
        }

        fread(header, 26, 1, fp2);
        strp2c(header);

        if(streql(header, AreaBaseHeader))
        {
            AreaBaseRecord * ar = new AreaBaseRecord;
            throw_new(ar);

            while(fread(ar, sizeof(AreaBaseRecord), 1, fp2) == 1)
            {
                if(not ar->deleted and ar->fidomsgstyle)
                {
                    aa.reset();

                    switch(ar->areatype)
                    {
                        case 0:
                            aa.type = GMB_ECHO;
                            aa.attr = attribsecho;
                            break;

                        case 1:
                            aa.type = GMB_NET;
                            aa.attr = attribsnet;
                            break;

                        case 2:
                            aa.type = GMB_LOCAL;
                            aa.attr = attribslocal;
                            break;

                        case 3:
                            aa.type = GMB_EMAIL;
                            aa.attr = attribsemail;
                            break;

                        default:
                            continue;
                    }
                    aa.basetype = gettype(c.opusdateformat ? "OPUS" : "FTS1",
                                          ar->fidomsgstyle);

                    if(in_range(ar->originaka, (byte)1, (byte)20))
                    {
                        aa.aka = CAST(ftn_addr, c.nodenrs[ar->originaka - 1]);
                    }

                    aa.setpath(strp2c(ar->fidomsgpath));
                    aa.setdesc(strp2c(ar->comment));
                    aa.setautoid(strp2c(ar->areaname_F));

                    switch(ar->originnr)
                    {
                        case 0:
                            aa.setorigin(strp2c(ar->origin));
                            break;

                        case 1:
                            aa.setorigin(c.origins[0]);
                            break;

                        case 2:
                            aa.setorigin(c.origins[1]);
                            break;
                    }
                    AddNewArea(aa);
                }
            }
            throw_delete(ar);
        }
        else
        {
            STD_PRINTNL(
                "* Error: WaterGate Areabase \"" << header <<
                "\" is not supported - Skipping.");
        }

        fclose(fp2);
    }

    throw_delete(_tmp);
} // gareafile::ReadWtrGteFile

//  ------------------------------------------------------------------
void gareafile::ReadWtrGte(char * tag)
{
    char * ptr;
    char options[80];
    Path wtrpath, file;

    *file    = NUL;
    *wtrpath = NUL;
    strcpy(options, tag);
    ptr = strtok(tag, " \t");

    while(ptr)
    {
        if(*ptr != '-')
        {
            AddBackslash(strcpy(wtrpath, ptr));
            break;
        }

        ptr = strtok(NULL, " \t");
    }

    if(*wtrpath == NUL)
    {
        ptr = getenv("WTRGATE");

        if(ptr)
        {
            AddBackslash(strcpy(wtrpath, ptr));
        }
    }

    if(*wtrpath == NUL)
    {
        strcpy(wtrpath, areapath);
    }

    MakePathname(file, wtrpath, "wtrcfg.tdb");
    gfile fp(file, "rb", sharemode);

    if(fp.isopen())
    {
        if(not quiet)
        {
            STD_PRINTNL("* Reading " << file);
        }

        char header[26];
        fp.Fread(header, 26);
        strp2c(header);

        if(streql(header, ConfigHeader))
        {
            ReadWtrGteFile(options, fp);
        }
        else
        {
            STD_PRINTNL(
                "* Error: WaterGate \"" << header << "\" is not supported - Skipping.");
        }
    }
} // gareafile::ReadWtrGte

//  ------------------------------------------------------------------
