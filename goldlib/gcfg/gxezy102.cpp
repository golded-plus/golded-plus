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
//  Read areas from Ezycom
//  ------------------------------------------------------------------

#include <gstrall.h>
#include <gmemdbg.h>
#if defined(__GOLD_GUI__)
#include <gvidall.h>
#include <gvidgui.h>
#endif
#undef GCFG_NOEZY
#include <gedacfg.h>
#include <gs_ez102.h>


//  ------------------------------------------------------------------

void gareafile::ReadEzycom102(gfile &fp, char* path, char* file, char* options)
{
    int n;
    AreaCfg aa;
    char abuf[40];

    CONFIGRECORD* config = new CONFIGRECORD;
    throw_new(config);
    CONSTANTRECORD* constant = new CONSTANTRECORD;
    throw_new(constant);
    MESSAGERECORD* messages = new MESSAGERECORD;
    throw_new(messages);

    fp.Fread(config, sizeof(CONFIGRECORD));
    fp.Fclose();

    MakePathname(file, path, "constant.ezy");
    fp.Fopen(file, "rb", sharemode);
    if (fp.isopen())
    {
        if (not quiet)
            STD_PRINTNL("* Reading " << file);

        fp.Fread(constant, sizeof(CONSTANTRECORD));
        fp.Fclose();

        STRNP2C(config->defaultorigin);
        STRNP2C(config->userbasepath);
        STRNP2C(config->msgpath);
        STRNP2C(config->netmailpath);
        CfgOrigin(config->defaultorigin);

        STRNP2C(constant->sysopname);
        STRNP2C(constant->sysopalias);

        CfgEzycommsgbase(config->msgpath);
        CfgEzycomuserbase(config->userbasepath);

        // Fido netmail directory
        if(not strblank(config->netmailpath))
        {
            aa.reset();
            aa.basetype = fidomsgtype;
            aa.type = GMB_NET;
            aa.attr = attribsnet;
            aa.aka.zone  = constant->netaddress[0].zone;
            aa.aka.net   = constant->netaddress[0].net;
            aa.aka.node  = constant->netaddress[0].node;
            aa.aka.point = constant->netaddress[0].point;
            aa.setpath(config->netmailpath);
            aa.setdesc("Ezycom Netmail");
            aa.setautoid("NETMAIL");
            AddNewArea(aa);
        }

        // Ezycom aka netmail boards
        for(n=0; n<MAXAKA; n++)
        {
            if(constant->netaddress[n].net)
            {
                if(constant->netmailboard[n])
                {
                    aa.reset();
                    aa.basetype = "EZYCOM";
                    aa.type = GMB_NET;
                    aa.attr = attribsnet;
                    aa.aka.zone  = constant->netaddress[n].zone;
                    aa.aka.net   = constant->netaddress[n].net;
                    aa.aka.node  = constant->netaddress[n].node;
                    aa.aka.point = constant->netaddress[n].point;
                    aa.board = constant->netmailboard[n];
                    Desc desc;
                    sprintf(desc, "Ezycom Netmail Board, %s", constant->netaddress[n].make_string(abuf));
                    aa.setdesc(desc);
                    sprintf(desc, "NET_AKA%u", n);
                    aa.setautoid(desc);
                    AddNewArea(aa);
                }
            }
        }

        // Ezycom watchdog board
        if(constant->watchmess)
        {
            aa.reset();
            aa.basetype = "EZYCOM";
            aa.type = GMB_LOCAL;
            aa.attr = attribslocal;
            aa.aka.zone  = constant->netaddress[0].zone;
            aa.aka.net   = constant->netaddress[0].net;
            aa.aka.node  = constant->netaddress[0].node;
            aa.aka.point = constant->netaddress[0].point;
            aa.board = constant->watchmess;
            aa.setdesc("Ezycom Watchdog Board");
            aa.setautoid("LOCAL_WATCHDOG");
            AddNewArea(aa);
        }

        // Ezycom paging board
        if(constant->pagemessboard)
        {
            aa.reset();
            aa.basetype = "EZYCOM";
            aa.type = GMB_LOCAL;
            aa.attr = attribslocal;
            aa.aka.zone  = constant->netaddress[0].zone;
            aa.aka.net   = constant->netaddress[0].net;
            aa.aka.node  = constant->netaddress[0].node;
            aa.aka.point = constant->netaddress[0].point;
            aa.board = constant->pagemessboard;
            aa.setdesc("Ezycom Paging Board");
            aa.setautoid("LOCAL_PAGING");
            AddNewArea(aa);
        }

        // Ezycom bad logon board
        if(constant->badpwdmsgboard)
        {
            aa.reset();
            aa.basetype = "EZYCOM";
            aa.type = GMB_LOCAL;
            aa.attr = attribslocal;
            aa.aka.zone  = constant->netaddress[0].zone;
            aa.aka.net   = constant->netaddress[0].net;
            aa.aka.node  = constant->netaddress[0].node;
            aa.aka.point = constant->netaddress[0].point;
            aa.board = constant->badpwdmsgboard;
            aa.setdesc("Ezycom Bad Logon Board");
            aa.setautoid("LOCAL_BADLOGON");
            AddNewArea(aa);
        }

        // Ezycom bad qwk board
        if(constant->qwkmsgboard)
        {
            aa.reset();
            aa.basetype = "EZYCOM";
            aa.type = GMB_ECHO;
            aa.attr = attribsecho;
            aa.aka.zone  = constant->netaddress[0].zone;
            aa.aka.net   = constant->netaddress[0].net;
            aa.aka.node  = constant->netaddress[0].node;
            aa.aka.point = constant->netaddress[0].point;
            aa.board = constant->qwkmsgboard;
            aa.setdesc("Ezycom Bad QWK Board");
            aa.setautoid("ECHO_BADQWK");
            AddNewArea(aa);
        }

        // Ezycom bad echomail board
        if(constant->badmsgboard)
        {
            aa.reset();
            aa.basetype = "EZYCOM";
            aa.type = GMB_ECHO;
            aa.attr = attribsecho;
            aa.aka.zone  = constant->netaddress[0].zone;
            aa.aka.net   = constant->netaddress[0].net;
            aa.aka.node  = constant->netaddress[0].node;
            aa.aka.point = constant->netaddress[0].point;
            aa.board = constant->badmsgboard;
            aa.setdesc("Ezycom Bad Echomail Board");
            aa.setautoid("ECHO_BAD");
            AddNewArea(aa);
        }

        MakePathname(file, path, "messages.ezy");
        fp.Fopen(file, "rb", sharemode);
        if (fp.isopen())
        {
            if (not quiet)
                STD_PRINTNL("* Reading " << file);

            int record = 1;

            while (fp.Fread(messages, sizeof(MESSAGERECORD)))
            {
                if(record <= constant->maxmess)
                {

                    if(*messages->name)
                    {

                        switch(messages->typ)
                        {

                        case 0:   // localmail
                        case 1:   // netmail
                        case 2:   // echomail
                        case 5:   // allmail

                            aa.reset();

                            STRNP2C(messages->name);
                            STRNP2C(messages->areatag);
                            STRNP2C(messages->originline);

                            aa.board = record;
                            aa.basetype = "EZYCOM";
                            aa.groupid = messages->areagroup;
                            aa.setorigin(*messages->originline ? messages->originline : config->defaultorigin);

                            aa.setdesc(messages->name);
                            aa.setechoid(messages->areatag);

                            switch(messages->typ)
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
                                break;
                            }

                            switch(messages->msgkinds)
                            {
                            case 1:
                                aa.attr.pvt1();
                                break;
                            case 0:
                            case 2:
                                aa.attr.pvt0();
                                break;
                            }

                            aa.aka.zone  = constant->netaddress[messages->originaddress-1].zone;
                            aa.aka.net   = constant->netaddress[messages->originaddress-1].net;
                            aa.aka.node  = constant->netaddress[messages->originaddress-1].node;
                            aa.aka.point = constant->netaddress[messages->originaddress-1].point;

                            AddNewArea(aa);

                            break;
                        }
                    }
                }

                record++;
            }

            fp.Fclose();
        }
    }

    throw_delete(messages);
    throw_delete(constant);
    throw_delete(config);
}


//  ------------------------------------------------------------------
