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
//  Squish msgbase.
//  ------------------------------------------------------------------
//  ------------------------------------------------------------------
//  Only include once!

#ifndef __GMSQSH_H
#define __GMSQSH_H
//  ------------------------------------------------------------------

#include <gmoarea.h>
#include <limits.h>
#include <gdefs.h>
#include <gusrmax.h>
//  ------------------------------------------------------------------

#if defined (GOLD_CANPACK)
#pragma pack(1)
#endif
//  ------------------------------------------------------------------

#ifdef GOLD_16BIT
const long MAX_IDXBUFSIZE = 65300L;
#else
const long MAX_IDXBUFSIZE = LONG_MAX;
#endif
//  ------------------------------------------------------------------

#define MSGAREA_NORMAL 0x00
#define MSGAREA_CREATE 0x01
#define MSGAREA_CRIFNEC 0x02
//  ------------------------------------------------------------------

#define MSGTYPE_SDM 0x01
#define MSGTYPE_SQUISH 0x02
#define MSGTYPE_ECHO 0x80
//  ------------------------------------------------------------------

#define MSGNUM_CUR ((dword) - 1L)
#define MSGNUM_PREV ((dword) - 2L)
#define MSGNUM_NEXT ((dword) - 3L)
//  ------------------------------------------------------------------

#define MOPEN_CREATE 0
#define MOPEN_READ 1
#define MOPEN_WRITE 2
#define MOPEN_RW 3
//  ------------------------------------------------------------------

#define SQEXTRA_BUF 16
//  ------------------------------------------------------------------
//  Constants for 'type' argument of SquishUidToMsgn()
const int UID_EXACT = 0;
const int UID_NEXT  = 1;
const int UID_PREV  = 2;
//  ------------------------------------------------------------------
//  Values for msgapierr

#define MERR_NONE 0       // No error
#define MERR_BADH 1       // Invalid handle passed to function
#define MERR_BADF 2       // Invalid or corrupted file
#define MERR_NOMEM 3      // Not enough memory for specified operation
#define MERR_NODS 4       // Maybe not enough disk space for operation
#define MERR_NOENT 5      // File/message does not exist
#define MERR_BADA 6       // Bad argument passed to msgapi function
#define MERR_EOPEN 7      // Couldn't close - messages still open
#define MERR_ACCESS 10    // Access denied - msgbase locked?
//  ------------------------------------------------------------------
//  Bitmasks for 'attr'

#define MSGPRIVATE 0x0001
#define MSGCRASH 0x0002
#define MSGREAD 0x0004
#define MSGSENT 0x0008
#define MSGFILE 0x0010
#define MSGFWD 0x0020
#define MSGORPHAN 0x0040
#define MSGKILL 0x0080
#define MSGLOCAL 0x0100
#define MSGHOLD 0x0200
#define MSGXX2 0x0400
#define MSGFRQ 0x0800
#define MSGRRQ 0x1000
#define MSGCPT 0x2000
#define MSGARQ 0x4000
#define MSGURQ 0x8000
#define MSGSCANNED 0x00010000L
#define MSGUID 0x00020000L
#define MSGPRINTED 0x00040000L
#define MSGSEEN 0x00080000L
#define MSGLOK 0x40000000L
//  ------------------------------------------------------------------
//  Max number of stored replies to one msg

#define MAX_REPLY 9
//  ------------------------------------------------------------------
//  Structure of message headers in .SQD
struct SqshHdr
{
    dword attr;
    char  from[36];
    char  to[36];
    char  subj[72];
    Addr  orig;
    Addr  dest;
    FTime date_written;   // When user wrote the msg (UTC)
    FTime date_arrived;   // When msg arrived on-line (UTC)
    short utc_offset;      // Offset from UTC of message writer, in minutes.
    dword replyto;
    dword replies[MAX_REPLY];
    dword umsgid;
    char  ftsc_date[20];
};

//  ------------------------------------------------------------------
//  Squish frames
const dword SQFRAMEID     = 0xAFAE4453L;
const dword SQFRAME_NULL  = 0;
const word SQFRAME_NORMAL = 0;
const word SQFRAME_FREE   = 1;
//  ------------------------------------------------------------------
//  Structure of message frames in .SQD
struct SqshFrm
{
    dword   id;           // Must always equal SQFRAMEID
    int32_t next;         // Offset of next frame
    int32_t prev;         // Offset of previous frame
    dword   length;       // Length of this frame
    dword   totsize;      // Length of data in frame (hdr+ctl+txt)
    dword   ctlsize;      // Length of control info
    word    type;         // Frm type (normal or free)
    word    reserved;     // Reserved
};

//  ------------------------------------------------------------------
//  Structure of index file .SQI
struct SqshIdx
{
    int32_t offset;        // Offset of frame in .SQD
    dword   msgno;         // Message number
    dword   hash;          // Hash value of TO: name
};

//  ------------------------------------------------------------------
//  Structure of base record in .SQD
struct SqshBase
{
    word  size;           // sizeof(SqshBaseT)
    word  reserved1;      // reserved
    dword totalmsgs;      // Number of messages in area
    dword highestmsg;     // Highest msg in area. Same as totalmsgs
    dword protmsgs;       // Skip killing first x msgs in area
    dword highwatermark;  // Relno (not Tagno) of HWM
    dword nextmsgno;      // Next message number to use
    char  name[80];       // Base name of SquishFile
    dword firstframe;     // Offset of first frame in file
    dword lastframe;      // Offset to last frame in file
    dword firstfreeframe; // Offset of first FREE frame in file
    dword lastfreeframe;  // Offset of last free frame in file
    dword endframe;       // Pointer to end of file
    dword maxmsgs;        // Max # of msgs to keep in area
    word  daystokeep;     // Max age of msgs in area (for packing util)
    word  framesize;      // sizeof(SqshFrmT)
    byte  reserved2[124]; // Reserved by Squish for future use
};

//  ------------------------------------------------------------------

#if defined (GOLD_CANPACK)
#pragma pack()
#endif
//  ------------------------------------------------------------------
struct SqshData
{
    int       fhsqd;
    int       fhsqi;
    SqshBase  base;
    SqshIdx * idx;
    int       softlock;
    int       islocked;
};

//  ------------------------------------------------------------------
struct SqshWide
{
    int           userno;
    MaximusUser * user;
    const char *  userpath;
    int           direct;
    int           recycle;
    int           squishscan;
};

//  ------------------------------------------------------------------
class SquishArea : public gmo_area
{
protected: SqshWide * wide;
    SqshData * data;
    void data_open();
    void data_close();
    int test_open(const char * __file);
    void save_lastread();
    void raw_scan(int __keep_index, int __scanpm = false);
    void raw_open();
    void raw_close();
    void refresh();
    int load_message(int __mode, gmsg * __msg, SqshHdr & __hdr);
    void read_frm(dword __offset, SqshFrm * __frm);
    void write_frm(dword __offset, SqshFrm * __frm);
    void upd_frm_next(dword __offset, SqshFrm * __frm, dword __next);
    void upd_frm_prev(dword __offset, SqshFrm * __frm, dword __prev);
    void add_to_free_chain(dword __delframe, SqshFrm * __delfrm);
    void delete_msg(uint __reln);
    void init_frm(SqshFrm * __frm);
    void excess_frm(dword __lastframe,
                    dword __newframe,
                    SqshFrm * __newfrm,
                    dword __totsize);
    uint find_msgn(uint32_t __tagn);
    void save_message(int __mode, gmsg * __msg);

public: SquishArea()
    {
        wide = NULL;
        data = NULL;
    }

    virtual ~SquishArea(){}
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
    Line * make_dump_msg(Line * & lin, gmsg * msg, char * lng_head);
};

//  ------------------------------------------------------------------
extern SqshData * squishdata;
extern SqshWide * squishwide;
extern int squishdatano;
//  ------------------------------------------------------------------
//  Squish scanning types
const int SQS_API   = 1;
const int SQS_QUICK = 2;
//  ------------------------------------------------------------------
//  Squish recycling types
const int SQUISHRECYCLE_NO      = false;
const int SQUISHRECYCLE_YES     = true;
const int SQUISHRECYCLE_MSGAPI1 = SQUISHRECYCLE_YES + 1;
const int SQUISHRECYCLE_MSGAPI2 = SQUISHRECYCLE_YES + 2;
//  ------------------------------------------------------------------

#endif // ifndef __GMSQSH_H
//  ------------------------------------------------------------------
