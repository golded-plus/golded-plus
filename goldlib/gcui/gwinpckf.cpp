//  This may look like C code, but it is really -*- C++ -*-
//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999-2000 Alexander S. Aganichev
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
//  GCUI: Golded+ Character-oriented User Interface.
//  wpickfile() - Allows user to select a file name.
//  ------------------------------------------------------------------

#include <gctype.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <gmemdbg.h>
#include <gfilutil.h>
#include <gwildmat.h>
#include <gwinall.h>
#include <gfilutil.h>
#include <gdirposx.h>
#include <gstrall.h>
//  ------------------------------------------------------------------
static bool path_in_title, case_sensitive;
static IfcpCP open_function = NULL;
static char * cwdp;
static char * tcwdp;
static char * namextp;
//  ------------------------------------------------------------------
//  this function is the compare function for std::sort()
static bool compare(const std::string str1, const std::string str2)
{
    // Sort with directories first
    bool dir1 = !!strchr(str1.c_str(), GOLD_SLASH_CHR);
    bool dir2 = !!strchr(str2.c_str(), GOLD_SLASH_CHR);

    if(dir1 && !dir2)
    {
        return true;
    }

    if(!dir1 && dir2)
    {
        return false;
    }

    std::string s1 = dir1 ? str1.substr(0, str1.length() - 1) : str1;
    std::string s2 = dir2 ? str2.substr(0, str2.length() - 1) : str2;
    int cmp;

    if(case_sensitive)
    {
        cmp = strcmp(s1.c_str(), s2.c_str());
    }
    else
    {
        cmp = stricmp(s1.c_str(), s2.c_str());

        if(cmp == 0)
        {
            cmp = strcmp(s1.c_str(), s2.c_str());
        }
    }

    return cmp < 0;
} // compare

//  ------------------------------------------------------------------
//  this function displays the title on the pick window border
static void disp_title()
{
    char buf[sizeof(Path) + 2];

    if(path_in_title or open_function)
    {
        strcpy(buf, " ");
        PathCopy(buf + 1, cwdp);
        strcat(buf, namextp);
        strcat(buf, " ");
    }

    if(path_in_title)
    {
        wtitle(buf, TCENTER, gwin.active->battr);
    }

    if(open_function)
    {
        (*open_function)(buf);
    }
}

//  ------------------------------------------------------------------
bool wpickfile(int srow,
               int scol,
               int erow,
               int ecol,
               int btype,
               vattr bordattr,
               vattr winattr,
               vattr barattr,
               bool title,
               std::string & filespec,
               IfcpCP open,
               bool casesens)
{
    Path cwd, dir, namext, tcwd, path, spec;

    cwdp    = cwd;
    tcwdp   = tcwd;
    namextp = namext;

    // set static variables
    open_function  = open;
    path_in_title  = title;
    case_sensitive = casesens;
    // save current working directory
    getcwd(tcwd, GMAXPATH);
    // if drive was specified, change to it
    char * q = strxcpy(spec, filespec.c_str(), sizeof(Path));
    // split up input filespec into its current
    // working directory and filename/extension
    char * r = strrchr(q, GOLD_SLASH_CHR);
  #if defined (__HAVE_DRIVES__)

    if(r == NULL)
    {
        if(q[1] == ':')
        {
            r = q + 1;
        }
    }

  #endif

    if(r == NULL)
    {
        *dir = NUL;
        strcpy(namext, q);
    }
    else
    {
        strins(" ", ++r, 0);
        *r++ = NUL;
        strcpy(dir, q);
        strcpy(namext, r);
    }

    bool finished;
    int picked;

    do
    {
        // if directory was specified, change to it
        if(*dir && gchdir(dir))
        {
            gchdir(tcwdp);
            return false;
        }

        // get current working directory
        getcwd(cwd, GMAXPATH);
        strcpy(dir, cwd);
        // find all directories plus all files matching input filespec in
        // current directory, allocating an array element for each
        picked = -1;
        gposixdir d(dir);
        gstrarray strarr;
        const gdirentry * de;

        if(d.ok)
        {
            while((de = d.nextentry()) != NULL)
            {
                const char * name = NULL;

                if(de->is_directory())
                {
                    if(de->name != ".")
                    {
                        strxmerge(path,
                                  sizeof(Path),
                                  de->name.c_str(),
                                  GOLD_SLASH_STR,
                                  NULL);
                        name = path;
                    }
                }
                else if(de->is_file())
                {
                    if(case_sensitive ? gwildmat(de->name.c_str(),
                                                 namext) : gwildmati(de->name.c_str(),
                                                                     namext))
                    {
                        name = de->name.c_str();
                    }
                }

                if(name)
                {
                    strarr.push_back(name);
                }
            }
        }

        // sort files in array by swapping their pointers
        std::sort(strarr.begin(), strarr.end(), compare);

        // let user pick file
        if(strarr.size())
        {
            picked = wpickstr(srow,
                              scol,
                              erow,
                              ecol,
                              btype,
                              bordattr,
                              winattr,
                              barattr,
                              strarr,
                              0,
                              disp_title);
        }

        if(picked == -1 or !strarr.size())
        {
            gchdir(tcwdp);
            return false;
        }

        // see if a directory was selected. if so save
        // directory name, otherwise build whole path name
        const char * slash = strchr(strarr[picked].c_str(), GOLD_SLASH_CHR);

        if(slash)
        {
            finished = false;
            strcpy(dir, strarr[picked].c_str());
            r = strrchr(dir, GOLD_SLASH_CHR);

            if(r)
            {
                *r = NUL;
            }
        }
        else
        {
            finished = true;
            PathCopy(filespec, cwd);
            filespec += strarr[picked].c_str();
        }

        strarr.clear();
    }
    while(not finished); // if a directory was selected, go back and do again
    gchdir(tcwd); // change back to current drive and directory
    return true; // return normally
} // wpickfile

//  ------------------------------------------------------------------
