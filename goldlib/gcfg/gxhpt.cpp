//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1999-2002 Alexander S. Aganichev
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
//  Read areas from Fidoconfig
//  ------------------------------------------------------------------

#include <cstdlib>
#include <cerrno>
#include <gcrcall.h>
#include <gstrall.h>
#include <gmemdbg.h>
#if defined(__GOLD_GUI__)
    #include <gvidall.h>
    #include <gvidgui.h>
#endif
#undef GCFG_NOFIDOCONF
#include <gedacfg.h>


//  ------------------------------------------------------------------

static char comment_char = '#';

//  ------------------------------------------------------------------

bool gareafile::ReadHPTLine(gfile &f, std::string& str)
{
    char buf[1024];

    if (!f.Fgets(buf, ARRAYSIZE(buf)))
        return false; // eof

    str = buf;

    if (!str.empty() && (*(str.end() - 1) != '\n'))
    {
        while (f.Fgets(buf, ARRAYSIZE(buf)))
        {
            str += buf;
            if (*(str.end() - 1) == '\n')
                break;
        }
    }

    std::string::iterator ptr = str.begin();
    std::string::iterator end = str.end();
    bool state = false;

    // state 0: normal state
    //       1: between ""
    //       2: end
    //       3: whitespace
    for (; ptr != end; ptr++)
    {
        if (comment_char == *ptr)
        {
            if (!state)
            {
                str.erase(ptr, end);
                break;
            }
        }
        else if ('\"' == *ptr)
        {
            state = !state;
        }
    }

    strltrim(strtrim(str));

    return true;
}


//  ------------------------------------------------------------------

void gareafile::gettok(char** key, char** val)
{

    char* p = strskip_wht(*val);

    // Get the keyword
    switch(*p)
    {

    // Strip "double" quotes
    case '\"':
        *key = ++p;
        do
        {
            p = strskip_to(p, '\"');
            if(not *p or (*(p-1) != '\\'))
                break;
            else
                p++;
        }
        while(*p);
        break;

    // Strip 'single' quotes
    case '\'':
        *key = ++p;
        do
        {
            p = strskip_to(p, '\'');
            if(not *p or (*(p-1) != '\\'))
                break;
            else
                p++;
        }
        while(*p);
        break;

    // Strip round brackets
    case '(': /*)*/
        *key = ++p;
        p = strskip_to(p, /*(*/ ')');
        break;

    // Get straight keyword
    default:
        *key = p;
        while(*p and not isspace(*p))
            p++;
    }

    // Nul-terminate keyword
    if(*p)
        *p++ = NUL;

    // Skip whitespace if any
    p = strskip_wht(p);

    // Get the value
    *val = p;
}


//  ------------------------------------------------------------------

void gareafile::ReadHPTFile(char* path, char* file, char* origin, int group)
{

    const word CRC_ADDRESS = 0xFDD6;
    const word CRC_INCLUDE = 0x379B;
    const word CRC_NETAREA = 0x8F1C;
    const word CRC_NETMAILAREA = 0xD2D7;
    const word CRC_LOCALAREA = 0xAEC1;
    const word CRC_ECHOAREA = 0x0D63;
    const word CRC_DUPEAREA = 0xD8B9;
    const word CRC_BADAREA = 0x8DA5;
    const word CRC_SYSOP = 0x967F;
    const word CRC_VERSION = 0xF78F;
    const word CRC_COMMENTCHAR = 0xE2CC;
    const word CRC_ECHOAREADEFAULTS = 0x2CBB;
    const word CRC_SET = 0x2FC2;

    AreaCfg aa;
    Path buf2;

    gfile fp(file, "rb", sharemode);
    if (fp.isopen())
    {
        fp.SetvBuf(NULL, _IOFBF, 8192);

        if (not quiet)
            STD_PRINTNL("* Reading " << file);

        aa.reset();
        aa.type = GMB_NONE;
        aa.basetype = fidomsgtype;
        aa.groupid = group;

        std::string s;
        while (ReadHPTLine(fp, s))
        {
            if (not s.empty())
            {
                char *alptr = throw_xstrdup(s.c_str());
                char *ptr = alptr;

                char* key;
                char* val = ptr;
                char* memory = NULL;
                gettok(&key, &val);
                switch (strCrc16(key))
                {
                case CRC_SET:
                    if (strchg(val, '[', '%') != 0)
                        strchg(val, ']', '%');
                    memory = static_cast<char*>(malloc(strlen(val)));
                    strcpy(memory, val);
                    putenv(memory);
                    break;

                case CRC_VERSION:
                {
                    int ver_maj, ver_min;
                    sscanf(val, "%d.%d", &ver_maj, &ver_min);
                    if (((ver_maj << 16) + ver_min) > 0x00010009)
                    {
                        STD_PRINTNL("* Error: Unknown fidoconfig version " << ver_maj << '.' << ver_min << " - Skipping.");
                        throw_xfree(alptr);
                        goto skip_config;
                    }
                }
                break;
                case CRC_ADDRESS:
                    CfgAddress(val);
                    break;
                case CRC_SYSOP:
                    CfgUsername(val);
                    break;
                case CRC_INCLUDE:
                    strxcpy(buf2, val, sizeof(buf2));
                    if(strchg(buf2, '[', '%') != 0)
                        strchg(buf2, ']', '%');
                    MakePathname(buf2, path, buf2);
                    ReadHPTFile(path, buf2, origin, group);
                    break;
                case CRC_COMMENTCHAR:
                    if((strlen(val) == 3) and (val[0] == val[2]) and strpbrk(val, "\'\""))
                        comment_char = val[1];
                    else if(*val)
                        comment_char = val[0];
                    break;
                case CRC_NETAREA:
                case CRC_NETMAILAREA:
                    aa.type = GMB_NET;
                    break;
                case CRC_LOCALAREA:
                case CRC_DUPEAREA:
                case CRC_BADAREA:
                    aa = echoareadefaults;
                    aa.type = GMB_LOCAL;
                    break;
                case CRC_ECHOAREA:
                    aa = echoareadefaults;
                    aa.type = GMB_ECHO;
                    break;
                case CRC_ECHOAREADEFAULTS:
                    echoareadefaults.reset();
                    aa.type = GMB_DEFAULT;
                    aa.basetype = fidomsgtype;
                    aa.groupid = group;
                    break;
                }

                if(aa.type != GMB_NONE)
                {

                    if(aa.type == GMB_DEFAULT)
                    {

                        *key = NUL;
                    }
                    else
                    {

                        // Get echoid
                        gettok(&key, &val);
                        aa.setechoid(key);

                        // Get path
                        gettok(&key, &val);
                    }

                    // If not pass-through
                    if (not strieql("Passthrough", key))
                    {
                        if (strchg(key, '[', '%') != 0)
                            strchg(key, ']', '%');
                        aa.setpath(key);

                        gettok(&key, &val);

                        while (*key)
                        {

                            if (*key == '-')
                            {
                                char *opt = key + 1;

                                if (strieql(opt, "a"))
                                {
                                    gettok(&key, &val);
                                    CfgAddress(key);
                                    aa.aka.set(key);
                                }
                                else if (strieql(opt, "b"))
                                {
                                    gettok(&key, &val);
                                    if (strieql(key, "Squish")) aa.basetype = "SQUISH";
                                    else if (strieql(key, "Jam")) aa.basetype = "JAM";
                                    else if (strieql(key, "MSG")) aa.basetype = fidomsgtype;
                                    else
                                    {
                                        aa.basetype = GMB_NONE;
                                        break;
                                    }
                                }
                                else if (strieql(opt, "g"))
                                {
                                    gettok(&key, &val);

                                    if (isdigit(*key))
                                        aa.groupid = 0x8000 + atoi(key);
                                    else if (g_isalpha(*key))
                                        aa.groupid = g_toupper(*key);
                                }
                                else if (strieql(opt, "d"))
                                {
                                    gettok(&key, &val);
                                    aa.setdesc(key);
                                }
                                else if (strieql(opt, "pass"))
                                {
                                    aa.type = GMB_NONE;
                                    break;
                                }
                            }

                            gettok(&key, &val);
                        }

                        aa.setorigin(origin);

                        switch(aa.type)
                        {
                        case GMB_NET:
                            aa.attr = attribsnet;
                            if (aa.basetype.empty())
                                aa.basetype = fidomsgtype;
                            AddNewArea(aa);
                            break;
                        case GMB_ECHO:
                            aa.attr = attribsecho;
                            if (aa.basetype.empty())
                                aa.basetype = fidomsgtype;
                            AddNewArea(aa);
                            break;
                        case GMB_LOCAL:
                            aa.attr = attribslocal;
                            if (aa.basetype.empty())
                                aa.basetype = fidomsgtype;
                            AddNewArea(aa);
                            break;
                        case GMB_DEFAULT:
                            echoareadefaults = aa;
                            break;
                        default:
                            break;
                        }
                    }

                    aa.reset();
                }
                throw_xfree(alptr);
            }
        }

skip_config:
        ;
    }
    else
    {
        STD_PRINTNL(" * AREAFILE FIDOCONFIG: Can't open file '" << path << GOLD_SLASH_STR << file << ": " << strerror(errno) );
    }
}


//  ------------------------------------------------------------------
//  Read areas from HPT (echomail processor)

void gareafile::ReadHPT(char* tag)
{

    char origin[80];
    char options[80];
    word defaultgroup = 0;
    Path file, path;

    *origin = NUL;
    *file = NUL;
    strcpy(options, tag);
    char* ptr = strtok(tag, " \t");
    while(ptr)
    {
        if(*ptr != '-')
            strcpy(file, ptr);
        else
        {
            if(g_toupper(*(++ptr)) == 'G')
            {
                if(*(++ptr) == '=')
                    ptr++;
                if(*ptr == '#')
                    defaultgroup = (word)(atoi(ptr+1)+0x8000u);
                else
                    defaultgroup = (word)(g_isupper(*ptr) ? *ptr : 0);
            }
        }
        ptr = strtok(NULL, " \t");
    }

    if(not *file and ((ptr = getenv("FIDOCONFIG"))!=NULL))
        strcpy(file, ptr);

    extractdirname(path, file);

    CfgSquishuserpath(path);

    CfgJAMSMAPIHighwater(true);

    echoareadefaults.type = GMB_NONE;
    echoareadefaults.basetype = fidomsgtype;
    echoareadefaults.groupid = defaultgroup;

    ReadHPTFile(path, file, origin, defaultgroup);
}


//  ------------------------------------------------------------------
