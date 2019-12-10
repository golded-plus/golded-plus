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
//  Read areas from timEd.
//  ------------------------------------------------------------------

#include <cstdlib>
#include <gcrcall.h>
#include <gstrall.h>
#if defined (__GOLD_GUI__)
#include <gvidall.h>
#include <gvidgui.h>
#endif
#undef GCFG_NOTIMED
#include <gedacfg.h>
//  ------------------------------------------------------------------
void gareafile::nullastbackslash(char * val)
{
    char * ptr = strrchr(val, '\\');

    if(ptr == NULL)
    {
        ptr = strrchr(val, '/');
    }

    if(ptr)
    {
        *ptr = NUL;
    }
}

//  ------------------------------------------------------------------
void gareafile::ReadTimedFile(char * path, char * file, char * options, char * origin)
{
    const word CRC_ADDRESS      = 0xFDD6;
    const word CRC_NAME         = 0x3B63;
    const word CRC_ORIGIN       = 0x4CE5;
    const word CRC_INCLUDE      = 0x379B;
    const word CRC_HUDSONPATH   = 0x52A7;
    const word CRC_JAMLOG       = 0x9135;
    const word CRC_SQUISHCFG    = 0x11B4;
    const word CRC_FASTECHOCFG  = 0xA013;
    const word CRC_GECHOCFG     = 0xF230;
    const word CRC_IMAILCFG     = 0xFFB5;
    const word CRC_FMAILCFG     = 0x3AB6;
    const word CRC_XMAILCFG     = 0xA091;
    const word CRC_LASTREAD     = 0x87DB;
    const word CRC_SQUISHOFFSET = 0xF55B;
    const word CRC_NETAREA      = 0x8F1C;
    const word CRC_ECHOAREA     = 0x0D63;
    const word CRC_LOCALAREA    = 0xAEC1;
    AreaCfg aa;
    char buf[512];
    char buf2[256];
    FILE * fp = fsopen(file, "rb", sharemode);

    if(fp)
    {
        setvbuf(fp, NULL, _IOFBF, 8192);

        if(not quiet)
        {
            STD_PRINTNL("* Reading " << file);
        }

        aa.reset();

        while(fgets(buf, sizeof(buf), fp))
        {
            char * ptr = strskip_wht(strtrim(buf));

            if(*ptr != ';' and * ptr)
            {
                aa.type = GMB_NONE;
                char * key;
                char * val = ptr;

                switch(getkeyvalcrc(&key, &val))
                {
                    case CRC_ADDRESS:
                        CfgAddress(val);
                        break;

                    case CRC_NAME:
                        CfgUsername(val);
                        break;

                    case CRC_ORIGIN:
                        strcpy(origin, val);
                        break;

                    case CRC_INCLUDE:
                        strcpy(buf2, val);
                        MakePathname(buf2, path, buf2);
                        ReadTimedFile(path, buf2, options, origin);
                        break;

                    case CRC_HUDSONPATH:
                        CfgHudsonpath(val);
                        break;

                    case CRC_JAMLOG:
                        CfgJampath(val);
                        break;

#ifndef GCFG_NOSQSH
                    case CRC_SQUISHCFG:
                        sprintf(buf2, "-c%s", val);
                        ReadSquish(buf2);
                        break;

#endif
#ifndef GCFG_NOFE
                    case CRC_FASTECHOCFG:
                        nullastbackslash(val);
                        ReadFastecho(val);
                        break;

#endif
#ifndef GCFG_NOGECHO
                    case CRC_GECHOCFG:
                        nullastbackslash(val);
                        ReadGEcho(val);
                        break;

#endif
#ifndef GCFG_NOIMAIL
                    case CRC_IMAILCFG:
                        nullastbackslash(val);
                        ReadIMail(val);
                        break;

#endif
#ifndef GCFG_NOXMAIL
                    case CRC_XMAILCFG:
                        nullastbackslash(val);
                        ReadXMail(val);
                        break;

#endif
#ifndef GCFG_NOFMAIL
                    case CRC_FMAILCFG:
                        nullastbackslash(val);
                        ReadFMail(val);
                        break;

#endif
                    case CRC_LASTREAD:
                        CfgFidolastread(val);
                        break;

                    case CRC_SQUISHOFFSET:
                        squishuserno = atoi(val);
                        break;

                    case CRC_NETAREA:
                        aa.type = GMB_NET;
                        break;

                    case CRC_ECHOAREA:
                        aa.type = GMB_ECHO;
                        break;

                    case CRC_LOCALAREA:
                        aa.type = GMB_LOCAL;
                        break;
                } // switch

                if(aa.type != GMB_NONE)
                {
                    // Get description
                    getkeyval(&key, &val);
                    aa.setdesc(key);
                    // Get echoid
                    getkeyval(&key, &val);
                    aa.setechoid(key);
                    // Get path or board number
                    getkeyval(&key, &val);
                    aa.setpath(key);
                    aa.board = atoi(key);

                    if(striinc("-$", val))
                    {
                        aa.basetype = "SQUISH";
                    }
                    else if(striinc("-J", val))
                    {
                        aa.basetype = "JAM";
                    }
                    else if(striinc("-H", val))
                    {
                        aa.basetype = "HUDSON";
                    }
                    else
                    {
                        aa.basetype = fidomsgtype;
                    }

                    const char * ptr2 = striinc("-p", val);

                    if(ptr2)
                    {
                        aa.aka = primary_aka;
                        aa.aka.set(ptr2 + 2);
                    }

                    switch(aa.type)
                    {
                        case GMB_LOCAL:
                            aa.attr = attribslocal;
                            break;

                        case GMB_NET:
                            aa.attr = attribsnet;
                            break;

                        case GMB_ECHO:
                            aa.attr = attribsecho;
                            break;
                    }
                    aa.setorigin(origin);
                    AddNewArea(aa);
                    aa.reset();
                }
            }
        }
        fclose(fp);
    }
} // gareafile::ReadTimedFile

//  ------------------------------------------------------------------
//  Read areas from timEd
void gareafile::ReadTimed(char * tag)
{
    char origin[80];
    char options[80];
    Path file, path, timedcfg;

    *path   = NUL;
    *origin = NUL;
    strcpy(timedcfg, "timed.cfg");
    strcpy(options, tag);
    char * ptr = strtok(tag, " \t");

    while(ptr)
    {
        if(*ptr != '-')
        {
            AddBackslash(strcpy(path, ptr));
        }
        else
        {
            if(g_toupper(*(++ptr)) == 'C')
            {
                if(*(++ptr) == '=')
                {
                    ptr++;
                }

                strcpy(timedcfg, ptr);
            }
        }

        ptr = strtok(NULL, " \t");
    }

    if(*path == NUL)
    {
        ptr = getenv("TIMED");

        if(ptr)
        {
            AddBackslash(strcpy(path, ptr));
        }
    }

    MakePathname(file, path, timedcfg);
    ReadTimedFile(path, file, options, origin);
} // gareafile::ReadTimed

//  ------------------------------------------------------------------
