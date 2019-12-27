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
//  Read areas from InterMail 2.26+
//  ------------------------------------------------------------------

#include <cstring>
#include <cstdlib>
#include <gmemdbg.h>
#if defined(__GOLD_GUI__)
#include <gvidall.h>
#include <gvidgui.h>
#endif
#undef GCFG_NOINTERMAIL
#include <gedacfg.h>
#include <gs_inter.h>


//  ------------------------------------------------------------------

void gareafile::ReadInterMail(char* tag)
{

    Path _path;
    char options[80];

    *_path = NUL;
    options[79] = 0;
    strncpy(options, tag, 79);

    char* ptr = strtok(tag, " \t");
    while(ptr)
    {
        if(*ptr != '-')
        {
            AddBackslash(strcpy(_path, ptr));
            break;
        }
        ptr = strtok(NULL, " \t");
    }
    if(*_path == NUL)
    {
        ptr = getenv("IM");
        if(ptr)
            PathCopy(_path, ptr);
    }
    if(*_path == NUL)
        strcpy(_path, areapath);

    const char* _file = AddPath(_path, "fd.sys");

    gfile fp(_file, "rb");
    if (fp.isopen())
    {
        _ctl* ctl = (_ctl*)throw_calloc(1, sizeof(_ctl));

        if (not quiet)
            STD_PRINTNL("* Reading " << _file);

        fp.Fread(ctl, sizeof(_ctl));

        if (not memcmp(ctl->fingerprint, "JoHo", 5) and (ctl->sysrev == IM_THISREV))
        {
            fp.Fclose();

            CfgHudsonpath(ctl->e.qbase);

            AreaCfg aa;

            aa.reset();
            aa.aka = CAST(ftn_addr, ctl->t.newaka[0]);
            aa.type = GMB_NET;
            aa.basetype = "OPUS";
            aa.attr.pvt(ctl->e.msgbits & MSGPRIVATE);
            aa.attr.cra(ctl->e.msgbits & MSGCRASH);
            aa.attr.k_s(ctl->e.msgbits & MSGKILL);
            aa.attr.r_o(ctl->e.netfolderflags & EDREADONLY);
            aa.setpath(ctl->s.mailpath);
            aa.setdesc("InterMail Netmail");
            aa.setautoid("NETMAIL");
            AddNewArea(aa);

            aa.reset();
            aa.aka = CAST(ftn_addr, ctl->t.newaka[ctl->e.dupes.useaka]);
            aa.type = GMB_ECHO;
            switch(ctl->e.dupes.ftype)
            {
            case F_MSG:
                aa.basetype = "OPUS";
                break;
            case F_HUDSON:
                aa.basetype = "HUDSON";
                break;
            case F_PCB15:
                aa.basetype = "PCBOARD";
                break;
            case F_JAM:
                aa.basetype = "JAM";
                break;
            }
            aa.attr.r_o(ctl->e.dupes.behave & EDREADONLY);
            aa.board = ctl->e.dupes.board;
            aa.setpath(ctl->e.dupes.path);
            aa.setdesc(ctl->e.dupes.title);
            aa.setechoid(ctl->e.dupes.areatag);
            aa.setorigin(ctl->e.origin[ctl->e.dupes.origin]);
            AddNewArea(aa);

            aa.reset();
            aa.aka = CAST(ftn_addr, ctl->t.newaka[ctl->e.badecho.useaka]);
            aa.type = GMB_ECHO;
            switch(ctl->e.badecho.ftype)
            {
            case F_MSG:
                aa.basetype = "OPUS";
                break;
            case F_HUDSON:
                aa.basetype = "HUDSON";
                break;
            case F_PCB15:
                aa.basetype = "PCBOARD";
                break;
            case F_JAM:
                aa.basetype = "JAM";
                break;
            }
            aa.attr.r_o(ctl->e.badecho.behave & EDREADONLY);
            aa.board = ctl->e.badecho.board;
            aa.setpath(ctl->e.badecho.path);
            aa.setdesc(ctl->e.badecho.title);
            aa.setechoid(ctl->e.badecho.areatag);
            aa.setorigin(ctl->e.origin[ctl->e.badecho.origin]);
            AddNewArea(aa);

            _file = AddPath(ctl->s.systempath, "imfolder.cfg");
            if(fexist(_file))
            {

                _file = AddPath(ctl->s.systempath, "imfolder.cfg");
                fp.Fopen(_file, "rb");
                if (fp.isopen())
                {
                    if (not quiet)
                        STD_PRINTNL("* Reading " << _file);

                    FOLDER* _folder = (FOLDER*)throw_calloc(1, sizeof(FOLDER));

                    while (fp.Fread(_folder, sizeof(FOLDER)) == 1)
                    {
                        aa.reset();
                        switch(_folder->ftype)
                        {
                        case F_MSG:
                            aa.basetype = "OPUS";
                            break;
                        case F_HUDSON:
                            aa.basetype = "HUDSON";
                            break;
                        case F_PCB15:
                            aa.basetype = "PCBOARD";
                            break;
                        case F_JAM:
                            aa.basetype = "JAM";
                            break;
                        default:
                            continue;
                        }
                        long _behave = _folder->behave;
                        if(not (DELETED & _behave))
                        {
                            aa.aka = CAST(ftn_addr, ctl->t.newaka[_folder->useaka]);
                            if(_behave & F_NETMAIL)
                                aa.type = GMB_NET;
                            else if(_behave & LOCAL)
                                aa.type = GMB_LOCAL;
                            else if(_behave & ECHOMAIL)
                                aa.type = GMB_ECHO;
                            aa.board = _folder->board;
                            aa.setpath(_folder->path);
                            aa.attr.pvt(_behave & PRIVATE);
                            aa.attr.r_o(_behave & READONLY);
                            aa.setechoid(_folder->areatag);
                            aa.setdesc(_folder->title);
                            aa.setorigin(ctl->e.origin[_folder->origin]);
                            AddNewArea(aa);
                        }
                    }
                    throw_free(_folder);
                    fp.Fclose();
                }
            }
            else
            {
                _file = AddPath(ctl->s.systempath, "folder.cfg");
                fp.Fopen(_file, "rb");
                if (fp.isopen())
                {
                    if (not quiet)
                        STD_PRINTNL("* Reading " << _file);

                    OLDFOLDER* _folder = (OLDFOLDER*)throw_calloc(1, sizeof(OLDFOLDER));

                    while (fp.Fread(_folder, sizeof(OLDFOLDER)) == 1)
                    {
                        long _behave = _folder->behave;
                        if(not (DELETED & _behave))
                        {

                            aa.reset();
                            aa.aka = CAST(ftn_addr, ctl->t.newaka[_folder->useaka]);
                            if(_behave & F_NETMAIL)
                                aa.type = GMB_NET;
                            else if(_behave & LOCAL)
                                aa.type = GMB_LOCAL;
                            else if(_behave & ECHOMAIL)
                                aa.type = GMB_ECHO;
                            if(_behave & BOARDTYPE)
                            {
                                aa.basetype = "HUDSON";
                                aa.board = _folder->board;
                            }
                            else
                            {
                                aa.basetype = "OPUS";
                                aa.setpath(_folder->path);
                            }
                            aa.attr.pvt(_behave & PRIVATE);
                            aa.attr.r_o(_behave & READONLY);
                            aa.setdesc(_folder->title);
                            aa.setorigin(ctl->e.origin[_folder->origin]);
                            AddNewArea(aa);
                        }
                    }
                    throw_free(_folder);
                    fp.Fclose();
                }
            }
        }
        else
        {
            char buff[62+sizeof(ctl->sysrev)*2];
            sprintf(buff, "* Error: InterMail revision %04Xh is not supported - Skipping." NL, ctl->sysrev);
            STD_PRINT(buff);
        }
        throw_free(ctl);
    }
}


//  ------------------------------------------------------------------
