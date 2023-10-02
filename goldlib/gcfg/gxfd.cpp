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
//  Read areas from FrontDoor 1.99c - 2.0x
//  ------------------------------------------------------------------

#include <cstdlib>
#include <gstrall.h>
#include <gmemdbg.h>
#if defined(__GOLD_GUI__)
    #include <gvidall.h>
    #include <gvidgui.h>
#endif
#undef GCFG_NOFD
#include <gedacfg.h>
#include <gs_fd.h>


//  ------------------------------------------------------------------
//  Read FrontDoor FD.SYS/SETUP.FD and FOLDER.SYS/FOLDER.FD

void gareafile::ReadFrontDoor(char* tag)
{

    AreaCfg aa;
    FILE* fp;
    char* ptr;
    word sysrev;
    long behave;
    FD_Folder* folder;
    FD_Editor* editor;
    FD_Shared* shared;
    Path fdpath, file;
    char buf[256], origin[80];

    *fdpath = NUL;
    *origin = NUL;
    folder = new FD_Folder;
    throw_new(folder);
    editor = new FD_Editor;
    throw_new(editor);
    shared = new FD_Shared;
    throw_new(shared);

    ptr = getenv("FD");
    if(ptr)
        AddBackslash(strcpy(fdpath, ptr));
    else
        strcpy(fdpath, areapath);

    // Read AREAS.BBS
    ptr = strtok(tag, " \t");
    while(ptr)
    {
        if(*ptr != '-')
        {
            if(is_dir(ptr) and (*fdpath == NUL))
                AddBackslash(strcpy(fdpath, ptr));
            else
                GetAreasBBS(ptr, origin);
        }
        ptr = strtok(NULL, " \t");
    }

    if(not fexist(AddPath(fdpath, "setup.fd")))
        MakePathname(file, fdpath, "fd.sys");
    else
        MakePathname(file, fdpath, "setup.fd");

    fp = fsopen(file, "rb", sharemode);
    if (fp)
    {
        if (not quiet)
            STD_PRINTNL("* Reading " << file);

        fread(buf, 5, 1, fp);
        if(streql(buf, "JoHo"))      // Check to see that it is v1.99b or higher
        {
            fread(&sysrev, sizeof(word), 1, fp);
            // This probably ought to be if(sysrev == FD_THISREV)..
            fseek(fp, 4, SEEK_CUR);                   // Seek past CRC32
            fseek(fp, sizeof(FD_Mailer), SEEK_CUR);   // Seek past some data
            fread(editor, sizeof(FD_Editor), 1, fp);
            fread(shared, sizeof(FD_Shared), 1, fp);
            //CfgUsername(shared->user[0].name);
            CfgHudsonpath(editor->qbase);
            aa.reset();
            aa.aka = shared->aka[0];
            aa.type = GMB_NET;
            aa.attr = attribsnet;
            aa.basetype = fidomsgtype;
            aa.attr.r_o(editor->netfolderflags & EDREADONLY);
            aa.attr.pvt(editor->msgbits & MSGPRIVATE);
            aa.attr.cra(editor->msgbits & MSGCRASH);
            aa.attr.k_s(editor->msgbits & MSGKILL);
            aa.setpath(shared->mailpath);
            aa.setdesc("FrontDoor Netmail");
            aa.setautoid("NETMAIL");
            AddNewArea(aa);
        }
        fclose(fp);
    }

    if(fexist(AddPath(fdpath, "folder.fd")))
        MakePathname(file, fdpath, "folder.fd");
    else
        MakePathname(file, fdpath, "folder.sys");

    fp = fsopen(file, "rb", sharemode);
    if (fp)
    {
        setvbuf(fp, NULL, _IOFBF, 8192);

        if (not quiet)
            STD_PRINTNL("* Reading " << file);

        while(fread(folder, sizeof(FD_Folder), 1, fp) == 1)
        {
            behave = folder->behave;
            if(not strblank(folder->title) and not (FOLDER_DELETED & behave))
            {
                aa.reset();
                aa.aka = shared->aka[folder->useaka];
                aa.type = (behave & FOLDER_ECHOMAIL) ? GMB_ECHO : GMB_LOCAL;
                aa.attr = (behave & FOLDER_ECHOMAIL) ? attribsecho : attribslocal;
                aa.attr.r_o(behave & FOLDER_READONLY);
                if(behave & FOLDER_HMB)
                {
                    aa.basetype = "HUDSON";
                    aa.board = folder->board;
                }
                else if(behave & FOLDER_JAM)
                {
                    aa.basetype = "JAM";
                    aa.setpath(folder->path);
                }
                else
                {
                    aa.basetype = fidomsgtype;
                    aa.setpath(folder->path);
                }
                if(behave & FOLDER_PRIVATE)
                    aa.attr.pvt1();
                else
                    aa.attr.pvt0();
                aa.setdesc(folder->title);
                aa.setorigin(editor->origin[folder->origin]);
                AddNewArea(aa);
            }
        }
        fclose(fp);
    }

    throw_delete(editor);
    throw_delete(shared);
    throw_delete(folder);
}


//  ------------------------------------------------------------------
