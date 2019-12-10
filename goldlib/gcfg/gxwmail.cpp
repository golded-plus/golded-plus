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
//  Read areas from WMail
//  ------------------------------------------------------------------

#include <cstdlib>
#include <gfilutil.h>
#include <gstrall.h>
#include <gmemdbg.h>
#if defined (__GOLD_GUI__)
#include <gvidall.h>
#include <gvidgui.h>
#endif
#undef GCFG_NOWMAIL
#include <gedacfg.h>
#include <gs_wmail.h>
//  ------------------------------------------------------------------
//  Read areas from WMail 2.2
void gareafile::ReadWMail(char * tag)
{
    int fh;
    AreaCfg aa;
    char * ptr;
    PWmailPrm wmprm;
    PAreasPrm arprm;
    char options[80];
    Path file, path;

    *path = NUL;
    wmprm = new TWmailPrm;
    throw_new(wmprm);
    arprm = new TAreasPrm;
    throw_new(arprm);
    strcpy(options, tag);
    ptr = strtok(tag, " \t");

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
        ptr = getenv("WMAIL");

        if(ptr)
        {
            AddBackslash(strcpy(path, ptr));
        }
    }

    if(*path == NUL)
    {
        strcpy(path, areapath);
    }

    sprintf(file, "%swmail.prm", path);
    fh = sopen(file, O_RDONLY | O_BINARY, sharemode, S_STDRD);

    if(fh != -1)
    {
        if(not quiet)
        {
            STD_PRINTNL("* Reading " << file);
        }

        read(fh, wmprm, sizeof(TWmailPrm));
        // The *.MSG netmail area
        aa.reset();
        aa.aka      = wmprm->aka[0].addr;
        aa.type     = GMB_NET;
        aa.attr     = attribsnet;
        aa.basetype = fidomsgtype;
        STRNP2C(wmprm->dir.mail);
        aa.setpath(wmprm->dir.mail);
        aa.setdesc("WMail Netmail");
        aa.setechoid("NETMAIL");
        AddNewArea(aa);

        // The *.MSG badmsg area
        if(*(wmprm->dir.badecho))
        {
            aa.reset();
            aa.aka      = wmprm->aka[0].addr;
            aa.type     = GMB_ECHO;
            aa.attr     = attribsecho;
            aa.basetype = fidomsgtype;
            STRNP2C(wmprm->dir.badecho);
            aa.setpath(wmprm->dir.badecho);
            aa.setdesc("WMail Bad Msgs Area");
            aa.setechoid("ECHO_BAD");
            AddNewArea(aa);
        }

        // The *.MSG dupe area
        if(*(wmprm->dir.dupes))
        {
            aa.reset();
            aa.aka      = wmprm->aka[0].addr;
            aa.type     = GMB_ECHO;
            aa.attr     = attribsecho;
            aa.basetype = fidomsgtype;
            STRNP2C(wmprm->dir.dupes);
            aa.setpath(wmprm->dir.dupes);
            aa.setdesc("WMail Dupes Area");
            aa.setechoid("ECHO_DUPES");
            AddNewArea(aa);
        }

        for(int n = 0; n < 10; n++)
        {
            STRNP2C(wmprm->origin[n]);
        }
        close(fh);
    }

    sprintf(file, "%sareas.prm", path);
    fh = sopen(file, O_RDONLY | O_BINARY, sharemode, S_STDRD);

    if(fh != -1)
    {
        if(not quiet)
        {
            STD_PRINTNL("* Reading " << file);
        }

        // All the echomail areas
        while(read(fh, arprm, sizeof(TAreasPrm)) == sizeof(TAreasPrm))
        {
            if(arprm->tipo < 4)
            {
                aa.reset();

                switch(arprm->tipo)
                {
                    case 1:
                        aa.type = GMB_LOCAL;
                        aa.attr = attribslocal;
                        break;

                    case 2:
                        aa.type = GMB_ECHO;
                        aa.attr = attribsecho;
                        break;

                    case 3:
                        aa.type = GMB_NET;
                        aa.attr = attribsnet;
                        break;
                }
                STRNP2C(arprm->path);
                aa.setpath(arprm->path);
                aa.board = atoi(aa.path);

                if(aa.board and aa.board <= 200)
                {
                    aa.basetype = "HUDSON";
                }
                else
                {
                    aa.basetype = fidomsgtype;
                }

                STRNP2C(arprm->titolo);
                STRNP2C(arprm->tag);
                aa.setdesc(arprm->titolo);
                aa.setechoid(arprm->tag);

                if(arprm->def_attr & 0x01)
                {
                    aa.attr.pvt1();
                }

                if(arprm->def_attr & 0x02)
                {
                    aa.attr.cra1();
                }

                if(arprm->def_attr & 0x08)
                {
                    aa.attr.att1();
                }

                if(arprm->def_attr & 0x80)
                {
                    aa.attr.k_s1();
                }

                aa.setorigin(wmprm->origin[arprm->wmorigin - 1]);
                aa.aka = wmprm->aka[arprm->aka].addr;
                AddNewArea(aa);
            }
        }
        close(fh);
    }

    throw_delete(wmprm);
    throw_delete(arprm);
} // gareafile::ReadWMail

//  ------------------------------------------------------------------
