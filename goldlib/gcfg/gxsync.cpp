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
#include <giniprsr.h>
#include <gstrall.h>
#if defined(__GOLD_GUI__)
    #include <gvidall.h>
    #include <gvidgui.h>
#endif
#undef GCFG_NOSYNCHRONET
#include <gedacfg.h>
#include <gs_sync.h>
#include <map>


//  ------------------------------------------------------------------
//  Synchronet configuration reader

namespace
{

using namespace std;
using namespace ini;

struct GroupInfo
{
    string prefix;
    uint16_t index;
};

typedef map<string, GroupInfo> Groups;

const Variables* findSection(const Sections& sections, const string& sectionName)
{
    Sections::const_iterator it = sections.find(sectionName);
    return it == sections.end() ? NULL : &it->second;
}

string getValue(const Variables& vars, const std::string& name, const string& defaultValue = "")
{
    Variables::const_iterator it = vars.find(name);
    return it == vars.end() ? defaultValue : it->second;
}

string makeAbsolutePath(const char* base, const string& path)
{
    return is_absolute_path(path) ? path : base + path;
}

bool readMainConfig(const std::string& file, Path& ctrlDir, string& defaultSubsDir)
{
    Sections sections;
    if (!ReadIniFile(file.c_str(), sections))
        return false;

    extractdirname(ctrlDir, file.c_str());

    const Variables* dirs = findSection(sections, "dir");
    const char* defRelDataDir = ".." GOLD_SLASH_STR "data" GOLD_SLASH_STR;
    string subsDir = (dirs ? getValue(*dirs, "data", defRelDataDir) : defRelDataDir) + "subs" GOLD_SLASH_STR;

    defaultSubsDir = makeAbsolutePath(ctrlDir, subsDir);

    return true;
}


Groups getGroups(const Sections& sections)
{
    Groups result;
    uint16_t index = 0;

    for (Sections::const_iterator grpIt = sections.begin(); grpIt != sections.end(); ++grpIt)
    {
        const string& section = grpIt->first;
        const Variables& vars = grpIt->second;
        if (section.rfind("grp:", 0) == 0)
        {
            // Ignore duplicates
            string group = section.substr(4);
            if (result.find(group) == result.end())
            {
                GroupInfo& info = result[group];
                info.index = index++;
                info.prefix = getValue(vars, "code_prefix");
            }
        }
    }

    return result;
}

bool readIniFile(string mainFile, const gareafile& areafile)
{
    if (not areafile.quiet)
            STD_PRINTNL("* Reading " << mainFile);

    Path ctrlDir;
    string defaultSubsDir;

    if (!readMainConfig(mainFile, ctrlDir, defaultSubsDir));

    string msgsFile = string(ctrlDir) + "msgs.ini";

    Sections msgsSections;
    if (!ReadIniFile(msgsFile.c_str(), msgsSections))
        return false;

    const Groups groups = getGroups(msgsSections);

    for (Sections::const_iterator secIt = msgsSections.begin(); secIt != msgsSections.end(); ++secIt)
    {
        if (secIt->first.rfind("sub:", 0) != 0)
            continue;

        string section(secIt->first.substr(4));
        size_t pos = section.rfind(':');
        if (pos == string::npos)
            continue;

        const string group = section.substr(0, pos);
        const string suffix = section.substr(pos + 1);

        Groups::const_iterator grpIt = groups.find(group);
        if (grpIt == groups.end())
            continue;

        const GroupInfo& groupInfo = grpIt->second;
        const Variables& sectionInfo = secIt->second;

        /* A sub-board's internal code is the combination of the grp's code_prefix & the sub's code_suffix */
        string subCode = groupInfo.prefix + suffix;
        AreaCfg aa;
        uint32_t settings = strtoul(getValue(sectionInfo, "settings").c_str(), NULL, 0);

        aa.type = (settings & SUB_FIDO) ? GMB_ECHO : GMB_LOCAL;
        aa.attr = areafile.attribsecho;
        aa.basetype = "SMB";
        aa.setechoid(((settings & SUB_FIDO) ? getValue(sectionInfo, "name") : subCode).c_str());
        aa.setdesc(getValue(sectionInfo, "description").c_str());
        aa.groupid = 0x8000 + groupInfo.index;
        string subCodeLwr(subCode);
        strlwr(subCodeLwr);
        string areaPath = getValue(sectionInfo, "data_dir");
        areaPath = areaPath.empty() ? defaultSubsDir : makeAbsolutePath(ctrlDir, areaPath);
        areaPath += subCodeLwr;
        aa.setpath(areaPath.c_str());
        aa.aka = areafile.primary_aka;
        aa.aka.set(getValue(sectionInfo, "fidonet_addr"));
        aa.setorigin(getValue(sectionInfo, "fidonet_origin").c_str());
        AddNewArea(aa);
    }
    return true;
}

void readCnfFile(string file, const string& path, const gareafile& areafile)
{
    FILE* in = fsopen(file, "rb", areafile.sharemode);
    if (in)
    {
        setvbuf(in, NULL, _IOFBF, BUFSIZ);

        if (not areafile.quiet)
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
            for(uint16_t i = 0; i < total_groups; i++)
            {
                if(fread(&grp[i], sizeof(grp_t), 1, in) != 1)
                    break;
            }
        }
        uint16_t shrt;
        if(fread(&shrt, sizeof(uint16_t), 1, in) == 1)
        {
            for(uint16_t i = 0; i < shrt; i++)
            {
                sub_t sub;
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
                aa.attr = areafile.attribsecho;
                aa.basetype = "SMB";
                aa.setechoid((sub.misc & SUB_FIDO) ? sub.sname : sub.code);
                aa.setdesc(sub.lname);
                aa.groupid = 0x8000 + sub.grp;
                if(*sub.data_dir)
                    MakePathname(file, sub.data_dir, strlwr(sub.code));
                else
                    MakePathname(file, path, strlwr(sub.code));
                aa.setpath(file.c_str());
                aa.aka = areafile.primary_aka;
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

bool configExists(Path& file, Path& path)
{
    if(not fexist(file))
    {
        AddBackslash(file);
        strxcat(file, "msgs.cnf", sizeof(path));
    }

    if(not fexist(file))
    {
        extractdirname(path, file);
        AddBackslash(path);
        strxmerge(file, sizeof(file), path, "ctrl" GOLD_SLASH_STR "msgs.cnf", NULL);
    }

    if(fexist(file))
    {
        Path ctrl;
        extractdirname(ctrl, file);
        size_t len = strxlen(ctrl, sizeof(ctrl));
        if((len > 0) and isslash(ctrl[len - 1]))
        {
            ctrl[len - 1] = NUL;
            extractdirname(path, ctrl);
        }
        else
        {
            strxcpy(path, ctrl, sizeof(path));
        }
        AddBackslash(path);
        strxcat(path, "data" GOLD_SLASH_STR "subs" GOLD_SLASH_STR, sizeof(path));
        return true;
    }

    return false;
}

} // namespace

void gareafile::ReadSynchronet(char* tag)
{
    Path cnfFile, path;

    char* ptr = strtok(tag, " \t");
    while(ptr)
    {
        if(*ptr != '-')
        {
            strxcpy(cnfFile, ptr, sizeof(path));
            strschg_environ(cnfFile);
        }
        ptr = strtok(NULL, " \t");
    }

    if (!readIniFile(cnfFile, *this) && configExists(cnfFile, path))
    {
        readCnfFile(cnfFile, path, *this);
    }
}


//  ------------------------------------------------------------------
