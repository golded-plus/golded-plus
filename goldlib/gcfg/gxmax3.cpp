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
//  Read areas from Maximus 3.xx
//  ------------------------------------------------------------------

#include <cstdio>
#include <cstdlib>
#include <gfilutil.h>
#include <gmemdbg.h>
#include <gstrall.h>
#if defined(__GOLD_GUI__)
#include <gvidall.h>
#include <gvidgui.h>
#endif
#undef GCFG_NOMAXIMUS
#include <gedacfg.h>
#include <gs_max3.h>


//  ------------------------------------------------------------------
//  Read areas from Maximus 3.xx

void gareafile::ReadMaximus3(char* mxpath, char* areafile, char* options)
{

    FILE* fp;
    AreaCfg aa;
    Path prmfile;

    MakePathname(prmfile, mxpath, "max.prm");

    fp = fsopen(prmfile, "rb", sharemode);
    if (fp)
    {
        if (not quiet)
            STD_PRINTNL("* Reading " << areafile);

        m_pointers* prmp = (m_pointers*)throw_calloc(1, sizeof(m_pointers));
        m_pointers& prm = *prmp;
        fread(prmp, sizeof(m_pointers), 1, fp);
        long heapsz = fsize(fp) - prm.heap_offset;
        char* offsets = (char*)throw_calloc(1, (uint)heapsz);
        fseek(fp, prm.heap_offset, SEEK_SET);
        fread(offsets, (uint)heapsz, 1, fp);
        fclose(fp);

        Path userfile;
        strxmerge(userfile, sizeof(Path), PRM(user_file), ".bbs", NULL);
        CfgSquishuserpath(userfile);

        strcpy(stpcpy(areafile, PRM(marea_name)), ".dat");
        MapPath(areafile);

        fp = fsopen(areafile, "rb", sharemode);
        if (fp)
        {
            setvbuf(fp, NULL, _IOFBF, 32000);

            if (not quiet)
                STD_PRINTNL("* Reading " << areafile);

            long areasize = fsize(fp)-4;

            dword marea_id;
            fread(&marea_id, sizeof(dword), 1, fp);
            if(marea_id == MAREA_ID)
            {

                int arearecsize = 1024;
                char* arearec = (char*)throw_malloc(arearecsize);
                _msgarea* area = (_msgarea*)arearec;

                while(areasize > 0)
                {

                    // Get the record sizes
                    fread(arearec, 6, 1, fp);

                    // Expand the allocation if necessary
                    int needsize = area->cbArea + (area->num_override*sizeof(_ovride)) + area->cbHeap;
                    if(needsize > arearecsize)
                    {
                        arearecsize = needsize;
                        arearec = (char*)throw_realloc(arearec, arearecsize);
                        area = (_msgarea*)arearec;
                    }

                    // Get the rest of this record
                    fread(arearec+6, needsize-6, 1, fp);
                    areasize -= needsize;

                    // Setup zstr heap pointer
                    char* zstrheap = arearec + (needsize - area->cbHeap);
#define ZSTR(s) (zstrheap+area->s)

                    if(area->attribs & MA_DIVBEGIN)
                        continue;

                    if(strblank(ZSTR(path)))
                        continue;

                    aa.reset();
                    switch(area->type)
                    {
                    case MSGTYPE_SQUISH:
                        aa.basetype = "SQUISH";
                        break;
                    case MSGTYPE_SDM:
                        aa.basetype = fidomsgtype;
                        break;
                    default:
                        continue;
                    }

                    Path apath;
                    MakePathname(apath, mxpath, ZSTR(path));
                    aa.setpath(apath);

                    aa.aka = CAST(ftn_addr, area->primary);
                    aa.setdesc(*ZSTR(descript) ? ZSTR(descript) : ZSTR(name));
                    aa.setechoid(ZSTR(echo_tag));
                    aa.setorigin(ZSTR(origin));
                    if(area->attribs & (MA_ECHO|MA_CONF))
                    {
                        aa.type = GMB_ECHO;
                        aa.attr = attribsecho;
                    }
                    else if(area->attribs & MA_NET)
                    {
                        aa.type = GMB_NET;
                        aa.attr = attribsnet;
                    }
                    else
                    {
                        aa.type = GMB_LOCAL;
                        aa.attr = attribslocal;
                    }

                    if(area->attribs & MA_READONLY)
                        aa.attr.r_o1();

                    AddNewArea(aa);
                }

                throw_free(arearec);

                fclose(fp);
            }
        }

        throw_free(offsets);
        throw_free(prmp);
    }
}


//  ------------------------------------------------------------------
//  Read areas from Maximus BBS

void gareafile::ReadMaximus(char* tag)
{

    char options[80];
    int is_maximus3 = false;
    Path maximuspath, mxpath, areafile, path;
    char* ptr;

    *maximuspath = NUL;
    char* mptr = getenv("MAXIMUS");
    if(mptr)
        extractdirname(maximuspath, mptr);

    *mxpath = NUL;
    ptr = maximuspath;
    if(ptr)
        AddBackslash(strcpy(mxpath, ptr));
    else
        strcpy(mxpath, areapath);

    // Get the path or filename from the parameters
    *path = NUL;
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

    // If no path or file was given, try the env.var
    if(*path == NUL)
    {
        ptr = maximuspath;
        if(ptr)
            AddBackslash(strcpy(path, ptr));
        else
            strcpy(path, areapath);
    }

    // Is it a directory?
    if(is_dir(path))
    {
        // Yes, so we use it
        strcpy(mxpath, path);
        MakePathname(areafile, mxpath, "marea.dat");
        if(fexist(areafile))
            is_maximus3 = true;
        else
            MakePathname(areafile, mxpath, "area.dat");
    } // Is it a file?
    else if(fexist(path))
    {
        // Yes, so we split it up to get the path
        strcpy(areafile, path);
        extractdirname(mxpath, path);
    }
    else
    {
        // Nonsense path or file given, so use defaults. (We ought to warn, but we are lazy..)
        ptr = maximuspath;
        if(ptr)
            AddBackslash(strcpy(mxpath, ptr));
        else
            strcpy(mxpath, areapath);
        MakePathname(areafile, mxpath, "marea.dat");
        if(fexist(areafile))
            is_maximus3 = true;
        else
            MakePathname(areafile, mxpath, "area.dat");
    }

    CfgSquishuserpath(mxpath);

    if(is_maximus3)
        ReadMaximus3(mxpath, areafile, options);
}


//  ------------------------------------------------------------------
