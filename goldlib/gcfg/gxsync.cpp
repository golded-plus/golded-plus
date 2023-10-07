//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 2002 Alexander S. Aganichev
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
//  Read areas from Synchronet BBS
//  ------------------------------------------------------------------

#include <cstdlib>
#include <gcrcall.h>
#include <gstrall.h>
#if defined(__GOLD_GUI__)
    #include <gvidall.h>
    #include <gvidgui.h>
#endif
#undef GCFG_NOSYNCHRONET
#include <gedacfg.h>
#include <gs_sync.h>


//  ------------------------------------------------------------------
//  Synchronet configuration reader

void gareafile::ReadSynchronet(char* tag)
{

    Path file, path;
    char options[80];
    uint16_t shrt, i;
    sub_t sub;

    strcpy(options, tag);
    char* ptr = strtok(tag, " \t");
    while(ptr)
    {
        if(*ptr != '-')
        {
            strcpy(file, ptr);
            strschg_environ(file);
        }
        ptr = strtok(NULL, " \t");
    }

    if(not fexist(file))
    {
        AddBackslash(file);
        strxcat(file, "msgs.cnf", sizeof(Path));
    }

    if(not fexist(file))
    {
        extractdirname(path, file);
        AddBackslash(path);
        strxmerge(file, sizeof(Path), path, "ctrl/msgs.cnf", NULL);
    }

    if(fexist(file))
    {
        Path ctrl;
        extractdirname(ctrl, file);
        size_t len = strlen(ctrl);
        if((len > 0) and isslash(ctrl[len - 1]))
        {
            ctrl[len - 1] = NUL;
            extractdirname(path, ctrl);
        }
        else
        {
            strcpy(path, ctrl);
        }
        AddBackslash(path);
        strxcat(path, "data/subs/", sizeof(Path));
    }
    else
    {
        *path = NUL;
    }

    FILE* in = fsopen(file, "rb", sharemode);
    if (in)
    {
        setvbuf(in, NULL, _IOFBF, BUFSIZ);

        if (not quiet)
            STD_PRINTNL("* Reading " << file);

        // Skip header:
        // max_qwkmsgs                  4
        // mail_maxcrcs                 4
        // mail_maxage                  2
        // unused (NULL)                    512
        // unused (0xff)                    512
        fseek(in, 1034, SEEK_CUR);

        uint16_t total_groups = 0;
        grp_t* grp = NULL;

        if(fread(&total_groups, sizeof(total_groups), 1, in) == 1
                && total_groups >= 1
                && (grp = (grp_t*)malloc(total_groups * sizeof(grp_t))) != NULL)
        {
            for(i = 0; i < total_groups; i++)
            {
                if(fread(&grp[i], sizeof(grp_t), 1, in) != 1)
                    break;
            }
        }
        if(fread(&shrt, sizeof(uint16_t), 1, in) == 1)
        {
            for(i = 0; i < shrt; i++)
            {
                if(fread(&sub, sizeof(sub_t), 1, in) != 1)
                    break;
                if(sub.grp >= total_groups)	// Illegal group number
                    continue;
                /* A sub-board's internal code is the combination of the grp's code_prefix & the sub's code_suffix */
                memset(sub.code, 0, sizeof(sub.code));
                snprintf(sub.code, sizeof(sub.code)-1, "%s%s", grp[sub.grp].code_prefix, sub.code_suffix);
                AreaCfg aa;
                aa.reset();
                aa.type = (sub.misc & SUB_FIDO) ? GMB_ECHO : GMB_LOCAL;
                aa.attr = attribsecho;
                aa.basetype = "SMB";
                aa.setechoid((sub.misc & SUB_FIDO) ? sub.sname : sub.code);
                aa.setdesc(sub.lname);
                aa.groupid = 0x8000 + sub.grp;
                if(*sub.data_dir)
                    MakePathname(file, sub.data_dir, strlwr(sub.code));
                else
                    MakePathname(file, path, strlwr(sub.code));
                aa.setpath(file);
                aa.aka = primary_aka;
                aa.aka.set(sub.faddr);
                if(*sub.origline)
                    aa.setorigin(sub.origline);
                AddNewArea(aa);
            }
        }
        if(grp != NULL)
            free(grp);
    }
    fclose(in);
}


//  ------------------------------------------------------------------
