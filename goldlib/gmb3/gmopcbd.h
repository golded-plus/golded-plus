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
//  PCBoard Structures.
//  ------------------------------------------------------------------
//  ------------------------------------------------------------------
//  Only include once!

#ifndef __GMPCBD_H
#define __GMPCBD_H
//  ------------------------------------------------------------------

#include <gmoarea.h>
#include <gs_pcb.h>
#include <gusrpcb.h>
//  ------------------------------------------------------------------

#if defined (GOLD_CANPACK)
    #pragma pack(1)
#endif
//  ------------------------------------------------------------------
typedef char PcbName[25 + 1];
//  ------------------------------------------------------------------
//  PCBoard Message Base Header (<msgbase>. file)
struct PcbBase
{
    int32_t highmsgno;              // High Message Number       (0 to 16,700,000)
    int32_t lowmsgno;               // Low Message Number        (0 to 16,700,000)
    int32_t active;                 // Number of Active Messages (0 to 32,767)
    int32_t callers;                // Number of System Callers (Main Message Base Only)
    char    locked[6];              // The "LOCKED" field for pre-14.2 systems (see note 1)
    byte    reserved[106];          // Reserved for future use
};

//  ------------------------------------------------------------------
//  PCBoard Individual Message Header (in <msgbase>. file)
struct PcbHdr
{
    char    status;                 // Message Status Flag (see note 2)
    int32_t msgno;                  // Message Number   (0 to 16,700,000)
    int32_t refno;                  // Reference Number (0 to 16,700,000)
    byte    blocks;                 // Number of 128 Byte Blocks in Message (see note 3)
    char    date[8];                // Date of Message Entry (in "mm-dd-yy" format)
    char    time[5];                // Time of Message Entry (in "hh:mm" format)
    char    destname[25];           // Name of the User to whom the Message is Addressed
    int32_t replydate;              // Date of the Reply Message (in yymmdd format)
    char    replytime[5];           // Time of the Reply Message (in "hh:mm" format)
    char    hasreply;               // The Letter "R" if the Message has a Reply
    char    origname[25];           // Name of the User who wrote the Message
    char    subject[25];            // Subject of the Message
    char    password[12];           // Password Need to Read the Message (if any)
    byte    activestatus;           // Active Status (225 = active, 226 = inactive)
    char    echoed;                 // The Letter "E" if the Message is to be Echoed
    byte    reserved[4];            // Reserved for future use
    byte    exthdrflags;            // Extended Header Flags (version 15.0)
    byte    reserved2;              // Reserved for future use
};

//  ------------------------------------------------------------------
//  PCBoard Extended Header (version 15.0) (in <msgbase>. file)
struct PcbExtHdr
{
    short id;                       // Extended Header ID = must be equal to 40FFh
    char  function[7];              // Extended Header Function
    char  colon;                    // A colon (:) character
    char  desc[60];                 // Extended Header Description (subj, to, from, etc)
    char  status;                   // Status (N or R)
    char  separator;                // Line Separator (E3h, or 0Dh for foreign systems)
};

//  ------------------------------------------------------------------
//  PCBoard Version 15.0 Style Index (<msgbase>.IDX file)
struct PcbIdx
{
    int32_t offset;                 // Offset (0 if none, >0 if active, <0 if killed)
    int32_t num;                    // Message Number
    char    to[25];                 // TO Name
    char    from[25];               // FROM Name
    char    status;                 // Status Character (from Message Header)
    word    date;                   // Date of Message (in julian date format)
    char    reserved[3];            // Reserved for future use
};

//  ------------------------------------------------------------------

#if defined (GOLD_CANPACK)
    #pragma pack()
#endif
//  ------------------------------------------------------------------
struct PcbData
{
    int     fhmsg;
    int     fhidx;
    int     islocked;
    PcbBase base;
};

//  ------------------------------------------------------------------
struct PcbWide
{
    int            isopen;
    int            fhusr;
    int            fhinf;
    int            userno;
    PcbUser *      user;
    Path           usersidxpath;
    Path           usersinf;
    Path           users;
    int            numareas;
    PcbUsers       usersrec;
    PcbUsersInfHdr usershdr;
    int32_t        usershdrsize;
    int32_t *      lastread;
    int32_t        confbytelen;
    int32_t        extconflen;
    int            foreign;
    const char *   path;
};

//  ------------------------------------------------------------------
class PcbArea : public gmo_area
{
protected: PcbWide * wide;
    PcbData * data;
    int just_scanning;
    void data_open();
    void data_close();
    int test_open(const char * __file);
    void raw_open();
    void raw_close();
    void save_lastread();
    void raw_scan(int __keep_index, int __scanpm = false);
    int load_message(int __mode, gmsg * __msg, PcbHdr & __hdr);
    void save_message(int __mode, gmsg * __msg, PcbHdr & __hdr);

public: PcbArea()
    {
        wide          = NULL;
        data          = NULL;
        just_scanning = false;
    }

    virtual ~PcbArea()
    {}
virtual bool havearrivedstamp() const
{
    return false;
}

virtual bool havereceivedstamp() const
{
    return false;
}

virtual bool requirehardterm() const
{
    return true;
}

    //  ----------------------------------------------------------------
    //  Messagebase member functions
    void open();
    void close();
    void suspend();
    void resume();
    void lock();
    void unlock();
    void scan();
    void scan_area();
    void scan_area_pm();
    int load_hdr(gmsg * msg);
    int load_msg(gmsg * msg);
    void save_hdr(int mode, gmsg * msg);
    void save_msg(int mode, gmsg * msg);
    void del_msg(gmsg * msg);
    void new_msgno(gmsg * msg);
    char * user_lookup(char * lookfor);
    int renumber();
    void update_timesread(gmsg * msg);
    Line * make_dump_msg(Line *& lin, gmsg * msg, char * lng_head);
};
//  ------------------------------------------------------------------
extern PcbData * pcbdata;
extern PcbWide * pcbwide;
extern int pcbdatano;
//  ------------------------------------------------------------------

#endif // ifndef __GMPCBD_H
//  ------------------------------------------------------------------
