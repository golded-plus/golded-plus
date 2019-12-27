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
//  Read areas from FidoPCB.
//  ------------------------------------------------------------------

#include <cstdlib>
#include <gmemdbg.h>
#include <gcrcall.h>
#include <gstrall.h>
#if defined(__GOLD_GUI__)
#include <gvidall.h>
#include <gvidgui.h>
#endif
#undef GCFG_NOFIDOPCB
#include <gedacfg.h>


//  ------------------------------------------------------------------

struct fparea
{

    Echo echoid;
    Path path;
    int  msgs;
};


//  ------------------------------------------------------------------

const word CRC_AREA      = 0x010B;
const word CRC_AREA_MSGS = 0x90E4;
const word CRC_LOST_MAIL = 0x7C78;
const word CRC_MAIL      = 0x6A39;
const word CRC_NET_MAIL  = 0x7F80;
const word CRC_SYSOP     = 0x967F;
const word CRC_ORIGIN    = 0x4CE5;
const word CRC_PCBOARD   = 0x84EC;
const word CRC_TAG       = 0x5B36;
const word CRC_NODE      = 0xD3AD;


//  ------------------------------------------------------------------

void gareafile::ReadFidoPCB(char* tag)
{

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
        ptr = getenv("FIDOPCB");
        if(ptr)
            AddBackslash(strcpy(path, ptr));
    }
    if(*path == NUL)
        strcpy(path, areapath);

    MakePathname(file, path, "fidopcb.cfg");

    FILE* fp = fsopen(file, "rb", sharemode);
    if (fp)
    {
        setvbuf(fp, NULL, _IOFBF, 8192);

        if (not quiet)
            STD_PRINTNL("* Reading " << file);

        int areas = 0;
        fparea* area = NULL;

        Path mailpath;
        *mailpath = NUL;
        Path lostmailpath;
        *lostmailpath = NUL;
        Path netmailpath;
        *netmailpath = NUL;
        ftn_addr mainaddr;
        char origin[80];
        *origin = NUL;
        int innode = false;

        char buf[512];
        while(fgets(buf, sizeof(buf), fp))
        {

            char* val = strskip_wht(buf);
            if(*val != ';' and *val)
            {

                // Extract keyword and value
                char* key;
                word crc = getkeyvalcrc(&key, &val);

                switch(crc)
                {
                case CRC_MAIL:
                    PathCopy(mailpath, val);
                    break;
                case CRC_PCBOARD:
                    CfgPcboardpath(val);
                    break;
                case CRC_LOST_MAIL:
                    PathCopy(lostmailpath, val);
                    break;
                case CRC_NET_MAIL:
                    PathCopy(netmailpath, val);
                    break;
                case CRC_SYSOP:
                    if(not innode)
                        NW(val);
                    //CfgUsername(val);
                    break;
                case CRC_ORIGIN:
                    mainaddr.set(val);
                    innode = false;
                    break;
                case CRC_TAG:
                    strxcpy(origin, val, sizeof(origin));
                    break;
                case CRC_AREA:
                    area = (fparea*)throw_realloc(area, (areas+1)*sizeof(fparea));
                    area[areas].msgs = false;
                    getkeyval(&key, &val);  // Skip the unique number
                    getkeyval(&key, &val);  // Get echoid
                    strxcpy(area[areas].echoid, key, sizeof(Echo));
                    getkeyval(&key, &val);  // Get path
                    PathCopy(area[areas].path, key);
                    areas++;
                    break;
                case CRC_AREA_MSGS:
                    if(areas)
                    {
                        strcat(area[areas-1].path, val);
                        area[areas-1].msgs = true;
                    }
                    break;
                case CRC_NODE:
                    innode = true;
                    break;
                }

            }
        }

        // *.MSG style netmail
        aa.reset();
        aa.aka = mainaddr;
        aa.basetype = fidomsgtype;
        aa.type = GMB_NET;
        aa.attr = attribsnet;
        aa.setpath(mailpath);
        aa.setdesc("FidoPCB Mailer Netmail");
        aa.setautoid("NETMAIL");
        AddNewArea(aa);

        // Matrix area
        if(*netmailpath)
        {
            aa.reset();
            aa.aka = mainaddr;
            aa.basetype = "PCBOARD";
            aa.type = GMB_NET;
            aa.attr = attribsnet;
            strcat(AddBackslash(netmailpath), "MATRIX");
            aa.setpath(netmailpath);
            aa.setorigin(origin);
            aa.setdesc("FidoPCB Matrix Board");
            aa.setautoid("MATRIX");
            AddNewArea(aa);
        }

        // Lost mail area
        if(*lostmailpath)
        {
            aa.reset();
            aa.aka = mainaddr;
            aa.basetype = "PCBOARD";
            aa.type = GMB_ECHO;
            aa.attr = attribsecho;
            strcat(AddBackslash(lostmailpath), "LOST");
            aa.setpath(lostmailpath);
            aa.setorigin(origin);
            aa.setdesc("FidoPCB Lost Mail Board");
            aa.setautoid("LOST");
            AddNewArea(aa);
        }

        // Process areas
        fparea* ap = area;
        for(int n=0; n<areas; n++,ap++)
        {
            aa.reset();
            aa.aka = mainaddr;
            aa.basetype = "PCBOARD";
            aa.type = GMB_ECHO;
            aa.attr = attribsecho;
            if(not ap->msgs)
                strcat(ap->path, "MSGS");
            aa.setpath(ap->path);
            aa.setechoid(ap->echoid);
            aa.setorigin(origin);
            AddNewArea(aa);
        }

        throw_free(area);

        fclose(fp);
    }
}


//  ------------------------------------------------------------------
