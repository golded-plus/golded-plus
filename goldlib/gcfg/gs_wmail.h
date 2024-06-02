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
//  WMail 2.2 config structs
//  ------------------------------------------------------------------

#if defined (GOLD_CANPACK)
    #pragma pack(1)
#endif
//  ------------------------------------------------------------------
//  Structure of WMAIL.PRM
typedef struct
{
    word version;
    char sysop[31];
    char key[17];
    struct this_name_for_msvc
    {
        ftn_addr addr;
        char     domain[21];
    } aka[11];
    word fakenet;
    struct
    {
        word net;
        word node;
    } zonegate[10];
    byte active_options;
    byte swap;
    word minspace;
    struct
    {
        char mail[61];
        char files[61];
        char out[61];
        char arcmail[61];
        char msgbase[61];
        char dupes[61];
        char badecho[61];
        char packer[61];
        char stats[61];
        char newareas[61];
        char nodelist[61];
    } dir;
    char origin[10][66];
    char arealistfile[61];
    byte logstyle;
    byte active_options2;
    struct this_name_for_msvc_too
    {
        ftn_addr addr;
        byte     packer;
        char     cmdpw1[21];
        char     cmdpw2[21];
        byte     level;
        short    misc;
        byte     aka;
    } bbsinfo[50];
    char routefile[61];
    struct
    {
        char name[13];
        char parms[21];
    } packer[7];
    struct
    {
        char name[13];
        char parms[21];
    } unpacker[7];
    byte defpacker;
    byte defmisc;
    byte unused[1000];
} TWmailPrm, *PWmailPrm;
//  ------------------------------------------------------------------
//  Structure of AREAS.PRM
typedef struct
{
    char titolo[41];
    char path[81];
    byte tipo;            // Local=1, EchoMail=2, NetMail=3, Passthru=4
    byte wedorigin;
    char tag[41];
    byte def_attr;        //  Bit 0: Pvt, 1: Crash, 4: F/A, 7: Kill/Sent
    char forwardto[3][61];
    byte aka;
    byte misc;
    byte level;
    byte wmorigin;
    char fill[8];
} TAreasPrm, *PAreasPrm;
//  ------------------------------------------------------------------

#if defined (GOLD_CANPACK)
    #pragma pack()
#endif
//  ------------------------------------------------------------------
