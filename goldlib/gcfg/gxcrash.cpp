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
//  Read areas from Crashmail II/CrashEcho
//  ------------------------------------------------------------------

#include <cstdlib>
#include <gcrcall.h>
#include <gstrall.h>
#if defined(__GOLD_GUI__)
    #include <gvidall.h>
    #include <gvidgui.h>
#endif
#undef GCFG_NOCMAIL
#include <gedacfg.h>


//  ------------------------------------------------------------------

bool gareafile::jbstrcpy(char *dest, char *src, size_t maxlen, size_t *jbc)
{
    size_t d=0;
    size_t stopchar1,stopchar2;
    size_t jbcpos;

    jbcpos= *jbc;

    while(isspace(src[jbcpos])) jbcpos++;

    if(src[jbcpos]=='\"')
    {
        jbcpos++;
        stopchar1='\"';
        stopchar2=0;
    }
    else
    {
        stopchar1=' ';
        stopchar2=9;
    }

    while(src[jbcpos]!=stopchar1 and src[jbcpos]!=stopchar2 and src[jbcpos]!='\n' and src[jbcpos]!=NUL and d<maxlen-1)
    {
        if(src[jbcpos]=='\\' and src[jbcpos+1]!=NUL and src[jbcpos+1]!='\n')
        {
            jbcpos++;
            dest[d++]=src[jbcpos++];
        }
        else
            dest[d++]=src[jbcpos++];
    }
    dest[d]=0;
    if(isspace(src[jbcpos]) or src[jbcpos]=='\"') jbcpos++;

    *jbc=jbcpos;

    if(d!=0 or stopchar1=='\"') return true;
    return false;
}


//  ------------------------------------------------------------------
//  Read areas from Crashmail II/CrashEcho (echomail processor)

void gareafile::ReadCrashmailCfg(const char* file)
{
    gfile fp(file, "rt", sharemode);
    if (fp.isopen())
    {
        fp.SetvBuf(NULL, _IOFBF, 8192);

        if (not quiet)
            STD_PRINTNL("* Reading " << file);

        char buf[4000];
        char key[30];
        char tmp[100], address[50], domain[50];
        Path path;
        bool unconfirmed = true;
        size_t jbcpos;
        AreaCfg aa;
        aa.reset();
        aa.type = GMB_NONE;
        address[0] = NUL;
        domain[0] = NUL;

        const word CRC_AKA = 0x13A4;
        const word CRC_AREA = 0x010B;
#ifndef GCFG_NOCECHO
        const word CRC_AREAFILE = 0xB487;
#endif
        const word CRC_LOCALAREA = 0xAEC1;
        const word CRC_DESCRIPTION = 0x2DF1;
        const word CRC_DOMAIN = 0xFFCA;
        const word CRC_GROUP = 0x1C9B;
        const word CRC_NETMAIL = 0xE42E;
#ifndef GCFG_NOCECHO
        const word CRC_NETMAILDIR = 0x180A;
#endif
        const word CRC_SYSOP = 0x967F;
        const word CRC_UNCONFIRMED = 0x195E;

        const word CRC_MSG = 0xCA2E;
        const word CRC_JAM = 0xA8C3;
        const word CRC_SQUISH = 0xFCF6;

        word crc16;

        while (fp.Fgets(buf, 4000))
        {
            jbcpos=0;
            jbstrcpy(key, buf, 30, &jbcpos);
            switch(crc16 = strCrc16(key))
            {
            case CRC_SYSOP:
                jbstrcpy(tmp, buf, 100, &jbcpos);
                CfgUsername(tmp);
                break;
            case CRC_AKA:
                if(address[0])
                {
                    strxmerge(tmp, 100, address, "@", domain, NULL);
                    CfgAddress(tmp);
                }
                jbstrcpy(address, buf, 50, &jbcpos);
                break;
            case CRC_DOMAIN:
                jbstrcpy(domain, buf, 50, &jbcpos);
                break;
#ifndef GCFG_NOCECHO
            case CRC_AREAFILE:
                jbstrcpy(path, buf, sizeof(Path), &jbcpos);
                ReadCrashmail(path);
                break;
            case CRC_NETMAILDIR:
                if(aa.type != GMB_NONE)
                {
                    if(not unconfirmed)
                        AddNewArea(aa);
                    aa.reset();
                }
                aa.aka = primary_aka;
                aa.type = GMB_NET;
                aa.attr = attribsnet;
                aa.basetype = fidomsgtype;
                jbstrcpy(path, buf, sizeof(Path), &jbcpos);
                aa.setpath(path);
                aa.setdesc("CrashEcho Netmail");
                aa.setautoid("NETMAIL");
                unconfirmed = false;
                break;
#endif
            case CRC_AREA:
            case CRC_NETMAIL:
            case CRC_LOCALAREA:
                if(aa.type != GMB_NONE)
                {
                    if(not unconfirmed)
                        AddNewArea(aa);
                }
                aa.reset();
                aa.type = GMB_NONE;
                unconfirmed = false;
                jbstrcpy(tmp, buf, 100, &jbcpos);
                if(strieql(tmp, "DEFAULT") or strnieql(tmp, "DEFAULT_", 8))
                {
                    break;
                }
                switch(crc16)
                {
                case CRC_NETMAIL:
                    aa.type = GMB_NET;
                    aa.attr = attribsnet;
                    break;
                case CRC_AREA:
                    aa.type = GMB_ECHO;
                    aa.attr = attribsecho;
                    break;
                case CRC_LOCALAREA:
                    aa.type = GMB_LOCAL;
                    aa.attr = attribslocal;
                    break;
                }
                aa.setechoid(tmp);
                jbstrcpy(tmp, buf, 50, &jbcpos);
                aa.aka = primary_aka;
                aa.aka.set(tmp);
                jbstrcpy(tmp, buf, 10, &jbcpos);
                switch(strCrc16(tmp))
                {
                case CRC_MSG:
                    aa.basetype = fidomsgtype;
                    break;
                case CRC_JAM:
                    aa.basetype = "JAM";
                    break;
                case CRC_SQUISH:
                    aa.basetype = "SQUISH";
                    break;
                default:
                    aa.reset();
                    aa.type = GMB_NONE;
                    break;
                }
                if(aa.type == GMB_NONE)
                    break;
                jbstrcpy(path, buf, sizeof(Path), &jbcpos);
                aa.setpath(path);
                break;
            case CRC_DESCRIPTION:
                jbstrcpy(tmp, buf, 100, &jbcpos);
                aa.setdesc(tmp);
                break;
            case CRC_UNCONFIRMED:
                unconfirmed = true;
                break;
            case CRC_GROUP:
                if(jbstrcpy(tmp, buf, 100, &jbcpos))
                {
                    if(isdigit(tmp[0]))
                        aa.groupid = 0x8000+atoi(tmp);
                    else if(g_isalpha(tmp[0]))
                        aa.groupid = g_toupper(tmp[0]);
                }
                break;
            }
        }
        if(aa.type != GMB_NONE)
        {
            if(not unconfirmed)
            {
                AddNewArea(aa);
            }
        }
        if(address[0])
        {
            strxmerge(tmp, 100, address, "@", domain, NULL);
            CfgAddress(tmp);
        }
    }
}


//  ------------------------------------------------------------------
//  Initialize parser

void gareafile::ReadCrashmail(char* tag)
{

    Path file, path;
    char options[80];

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

    extractdirname(path, file);

    CfgSquishuserpath(path);

    ReadCrashmailCfg(file);
}


//  ------------------------------------------------------------------
