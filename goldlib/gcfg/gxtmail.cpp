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
//  Read areas from Terminate Mail (TMAIL).
//  ------------------------------------------------------------------

#include <cstdlib>
#include <gcrcall.h>
#include <gstrall.h>
#if defined(__GOLD_GUI__)
#include <gvidall.h>
#include <gvidgui.h>
#endif
#undef GCFG_NOTERMAIL
#include <gedacfg.h>


//  ------------------------------------------------------------------

void gareafile::ReadTmailFile(char* file, char* options, char* origin)
{

    const word CRC_NAME = 0x3B63;
    const word CRC_ADDRESS = 0xFDD6;
    const word CRC_NETMAIL = 0xE42E;
    const word CRC_AREAFILE = 0xB487;
    const word CRC_ORIGIN = 0x4CE5;
    const word CRC_ECHOFILE = 0x3EAA;
    const word CRC_DESC = 0x8A2A;

    AreaCfg aa;
    char buf[512];

    FILE* fp = fsopen(file, "rb", sharemode);
    if (fp)
    {
        setvbuf(fp, NULL, _IOFBF, 8192);

        if (not quiet)
            STD_PRINTNL("* Reading " << file);

        aa.reset();

        while(fgets(buf, sizeof(buf), fp))
        {

            char* ptr = strskip_wht(strtrim(buf));
            if(*ptr != '%' and *ptr)
            {

                aa.type = GMB_NONE;

                char* key;
                char* val = ptr;
                switch(getkeyvalcrc(&key, &val))
                {
                case CRC_NAME:
                    CfgUsername(val);
                    break;
                case CRC_ADDRESS:
                    CfgAddress(val);
                    break;
                case CRC_NETMAIL:
                    aa.reset();
                    aa.type = GMB_NET;
                    aa.attr = attribsnet;
                    aa.basetype = fidomsgtype;
                    aa.setdesc("Terminate Netmail");
                    aa.setautoid("NETMAIL");
                    adjustpath(val);
                    aa.setpath(val);
                    aa.setorigin(origin);
                    AddNewArea(aa);
                    break;
                case CRC_AREAFILE:
                    adjustpath(val);
                    ReadAreasBBS(val);
                    break;
                case CRC_ORIGIN:
                    strcpy(origin, val);
                    break;
                case CRC_ECHOFILE:
                    adjustpath(val);
                    ReadEcholist(val);
                    break;
                case CRC_DESC:
                    getkeyval(&key, &val);
                    ptr = key;
                    getkeyval(&key, &val);
                    strchg(key, '_', ' ');
                    strtrim(key);
                    SetAreaDesc(ptr, key);
                    break;
                }
            }
        }

        fclose(fp);
    }
}


//  ------------------------------------------------------------------
//  Read areas from TMAIL

void gareafile::ReadTmail(char* tag)
{

    char origin[80];
    char options[80];
    Path file, path, tmailcfg;

    *path = NUL;
    *origin = NUL;
    strcpy(tmailcfg, "tmail.cfg");
    strcpy(options, tag);
    char* ptr = strtok(tag, " \t");
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
                if(*(++ptr) == ':')
                    ptr++;
                strcpy(tmailcfg, ptr);
            }
        }
        ptr = strtok(NULL, " \t");
    }
    if(*path == NUL)
    {
        ptr = getenv("TMAIL");
        if(ptr)
            AddBackslash(strcpy(path, ptr));
    }

    MakePathname(file, path, tmailcfg);
    if(not fexist(file))
    {
        strcpy(tmailcfg, "tm.cfg");
        MakePathname(file, path, tmailcfg);
    }

    strcpy(pathprefix, path);
    ReadTmailFile(file, options, origin);
    *pathprefix = NUL;
}


//  ------------------------------------------------------------------
