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
//  File utility functions
//  ------------------------------------------------------------------

#include <gfilutil.h>
#ifndef __HAVE_DRIVES__
#include <pwd.h>
#endif
#ifdef __WIN32__
#include <windows.h>
#endif
#include <stdlib.h>
#include <string.h>
//  ------------------------------------------------------------------
//  Adds the directory-delimiter character into end of string ('\\' in DOS-based, '/' in
// unix-based OS)
// Replace wrong directory-delimiter character with good.
std::string & AddBackslash(std::string & p)
{
    for(size_t posn = 0; (posn = p.find(GOLD_WRONG_SLASH_CHR, posn)) != p.npos; posn++)
    {
        p[posn] = GOLD_SLASH_CHR;
    }

    if(p[p.length() - 1] != GOLD_SLASH_CHR)
    {
        p += GOLD_SLASH_STR;
    }

    return p;
}

//  ------------------------------------------------------------------
//  Remove one trailing directory-delimiter character ('\\' in DOS-based, '/' in unix-based OS)
std::string & StripBackslash(std::string & p)
{
    std::string::iterator pend = p.end();

    if(isslash(*pend))
    {
        p.erase(pend);
    }

    return p;
}

//  ------------------------------------------------------------------
//  Get size of file. Return values:
// positive number or zero - size of file,
// negative number - error (and error code is stored in errno)
long GetFilesize(const std::string & filename)
{
    if(filename.empty())
    {
        errno = EINVAL;
        return -1;
    }

    struct stat info;

    if(stat(filename.c_str(), &info) == 0)
    {
        return info.st_size;
    }

    return -1;
}

//  ------------------------------------------------------------------
//  Add path to filename, if no path is set. Don't chech size of 'path', be careful!
void MakePathname(std::string & pathname,
                  const std::string & path,
                  const std::string & name)
{
    Path pn;

    MakePathname(pn, path.c_str(), name.c_str());
    pathname = pn;
}

//  ------------------------------------------------------------------
//  Copy pathname with enviroment variables substitution and adds directory delimiter char.
//  Copy not more sizeof(Path) characters (__dst should be type "Path" or equvalence, size is
// GMAXPATH)
void PathCopy(std::string & dst, const char * src)
{
    dst = src;
    strschg_environ(dst);
    AddBackslash(dst);
}

void PathCopy(std::string & dst, const std::string & src)
{
    dst = src;
    strschg_environ(dst);
    AddBackslash(dst);
}

//  ------------------------------------------------------------------
//  DOS-style enviroment variables substitution in string.
//  In unix-like enviroment also tilde substitution in string.
int strschg_environ(std::string & s)
{
    std::string fnd;
    int replaced = 0;
    size_t posn, posn1;

    if(s.length() > 1)
    {
        while(((posn = s.find('%')) !=
               s.npos) and ((posn1 = s.find('%', posn + 1)) != s.npos))
        {
            fnd = s.substr(posn + 1, posn1 - 1 - posn);
            const char * rep = getenv(fnd.c_str());
            rep = rep ? rep : "";
            s.replace(posn, posn1 - posn + 1, rep);
            replaced++;
        }
    }

#ifndef __HAVE_DRIVES__

    if(s[0] == '~')
    {
        struct passwd * pe = NULL;
        size_t slash       = 1;

        if((s.length() > 1) and not isslash(s[1]))
        {
            slash = s.find_first_of(GOLD_SLASH_STR GOLD_WRONG_SLASH_STR, 1);
            fnd.assign(s, 1, slash - 1);
            pe = getpwnam(fnd.c_str());
        }
        else
        {
            pe = getpwuid(getuid());
        }

        if(pe != NULL)
        {
            fnd.assign(pe->pw_dir);

            if(slash != std::string::npos)
            {
                fnd += s.substr(slash);
            }

            s = fnd;
            replaced++;
        }
    }

#endif // ifndef __HAVE_DRIVES__
    return replaced;
} // strschg_environ

//  ------------------------------------------------------------------
//  Change dirname to real full pathname
bool maketruepath(std::string & dirname)
{
    bool ok = true;

    std::string ndirname;
    char cwd[GMAXPATH];
    getcwd(cwd, GMAXPATH);

#ifdef __HAVE_DRIVES__
#ifdef __WIN32__
    char expanded[GMAXPATH];
    char * k;

    if(GetFullPathName(dirname.c_str(), GMAXPATH, expanded, &k) == 0)
    {
        dirname = cwd;
        ok      = false;
    }
    else
    {
        if(access(expanded, R_OK) == 0)
        {
            dirname = expanded;
        }
        else
        {
            dirname = cwd;
            ok      = false;
        }
    }

#elif defined __OS2__
    char expanded[GMAXPATH];

    if(_fullpath(expanded, dirname.c_str(), GMAXPATH) == 0)
    {
        if(access(expanded, R_OK) == 0)
        {
            dirname = expanded;
        }
        else
        {
            dirname = cwd;
            ok      = false;
        }
    }
    else
    {
        dirname = cwd;
        ok      = false;
    }

#elif defined __DJGPP__
    char expanded[GMAXPATH];
    _fixpath(dirname.c_str(), expanded);

    if(access(expanded, R_OK) == 0)
    {
        dirname = expanded;
    }
    else
    {
        dirname = cwd;
        ok      = false;
    }

#else // ifdef __WIN32__
    long inspos = -1;

    if((dirname.length() == 2) and (dirname[1] == ':'))
    {
        inspos = 2;
    }
    else if((dirname.length() < 2) or (dirname[1] != ':') or isslash(dirname[0]))
    {
        inspos = 0;
    }
    else if(not isslash(dirname[2]))
    {
        inspos = 2;
    }

    if(inspos != -1)
    {
        char chdrive[] = " :";
        chdrive[0] = (inspos == 2) ? dirname[0] : cwd[0];

        if(not chdir(chdrive))
        {
            char dwd[GMAXPATH];
            getcwd(dwd, GMAXPATH);

            if(isslash(dirname[0]))
            {
                ndirname = chdrive;
            }
            else
            {
                ndirname  = dwd;
                ndirname += GOLD_SLASH_CHR;
            }

            ndirname += dirname.c_str() + inspos;
            dirname   = ndirname;
        }
        else
        {
            dirname = cwd;
            ok      = false;
        }

        chdir(cwd);
    }

#endif // ifdef __WIN32__
#else // ifdef __HAVE_DRIVES__

    if(not dirname.empty() and (dirname[0] == '~'))
    {
        char * lname;
        const char * p = dirname.c_str() + 1;

        if((dirname.length() != 1) and not isslash(*p))
        {
            while(*p and not isslash(*p))
            {
                ndirname += *p++; // get user name
            }
        }
        else
        {
            if((lname = getlogin()) == NULL)
            {
                lname = getenv("LOGNAME");
            }

            ndirname = lname; // get current user name
        }

        struct passwd * pe = getpwnam(ndirname.c_str()); // get home

        if(pe != NULL)
        {
            ndirname  = pe->pw_dir;
            ndirname += GOLD_SLASH_CHR;

            if(isslash(*p))
            {
                ++p;
            }

            ndirname += p;
            dirname   = ndirname;
        }
        else
        {
            dirname = cwd;
            ok      = false;
        }
    }
    else if(not dirname.empty() and not isslash(dirname[0]))
    {
        ndirname  = cwd;
        ndirname += GOLD_SLASH_CHR;
        ndirname += dirname;
        dirname   = ndirname;
    }

#endif // ifdef __HAVE_DRIVES__
    size_t posn;

    for(posn = 0; (posn = dirname.find(GOLD_WRONG_SLASH_CHR, posn)) != dirname.npos;
        posn++)
    {
        dirname[posn] = GOLD_SLASH_CHR;
    }
    size_t skipfrom, skipto;
#ifdef __HAVE_DRIVES__ /* Prevent from destroying UNC names */

    if(dirname.length() > 1)
    {
        while((skipfrom = dirname.find("\\\\", 1)) != dirname.npos)
        {
            dirname.erase(skipfrom, 1);
        }
    }

#endif
    size_t len = dirname.length();
#ifdef __HAVE_DRIVES__

    while((len > 3) and isslash(dirname[--len]))
#else

    while((len > 1) and isslash(dirname[--len]))
#endif
    {
        dirname.erase(len, 1);
    }

    if(access(dirname.c_str(), R_OK))
    {
        dirname = cwd;
        ok      = false;
    }

#ifdef __HAVE_DRIVES__

    if((dirname.length() > 2) and (dirname[0] == '\\') and (dirname[1] == '\\'))
    {
        posn = 2;
    }
    else
#endif
    posn = 0;

    for( ; (posn = dirname.find('\\', posn)) != dirname.npos; posn++)
    {
        dirname[posn] = '/';
    }

    while((skipto = dirname.find("/../")) != dirname.npos)
    {
        skipfrom = (skipto == 0) ? 0 : dirname.rfind('/', skipto - 1);
        skipto  += 2;
        dirname.erase(skipfrom, skipto - skipfrom + 1);
    }

    while((skipfrom = dirname.find("/./")) != dirname.npos)
    {
        dirname.erase(skipfrom, 2);
    }
    len = dirname.length();

    if(len > 2 and not strcmp(&(dirname.c_str()[len - 2]), "/."))
    {
        dirname.erase(len - 2, 2);
    }

    len = dirname.length();
#ifdef __HAVE_DRIVES__

    if((len > 3) and isslash(dirname[--len]))
#else

    if((len > 1) and isslash(dirname[--len]))
#endif
    {
        dirname.erase(len, 1);
    }

    return ok;
} // maketruepath

//  ------------------------------------------------------------------
