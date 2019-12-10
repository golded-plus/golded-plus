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
//  JAM msgbase definitions and structures.
//  ------------------------------------------------------------------
//  This implementation is based on information in the document
//  JAM-001, revision 001, dated 93-07-01 and the associated example
//  "JAM API". Here is the JAM copyright notice:
//
//  "JAM(mbp) - Copyright 1993 Joaquim Homrighausen, Andrew Milner,
//                             Mats Birch, Mats Wallin.
//                             ALL RIGHTS RESERVED."
//  ------------------------------------------------------------------
//  ------------------------------------------------------------------
//  Only include once!

#ifndef __GM_JAMM_H
#define __GM_JAMM_H
//  ------------------------------------------------------------------

#include <gmoarea.h>
//  ------------------------------------------------------------------

#if defined (GOLD_CANPACK)
#pragma pack(1)
#endif
//  ------------------------------------------------------------------
//  JAM constants

#define JAM_REVISION 1
#define JAM_SIGNATURE "JAM\0"
#define JAM_MAXDATLEN 100
//  ------------------------------------------------------------------
//  Message status bits

#define JAMATTR_LOCAL 0x00000001L          // Msg created locally
#define JAMATTR_INTRANSIT 0x00000002L      // Msg is in-transit
#define JAMATTR_PRIVATE 0x00000004L        // Private
#define JAMATTR_READ 0x00000008L           // Read by addressee
#define JAMATTR_SENT 0x00000010L           // Sent to remote
#define JAMATTR_KILLSENT 0x00000020L       // Kill when sent
#define JAMATTR_ARCHIVESENT 0x00000040L    // Archive when sent
#define JAMATTR_HOLD 0x00000080L           // Hold for pick-up
#define JAMATTR_CRASH 0x00000100L          // Crash
#define JAMATTR_IMMEDIATE 0x00000200L      // Send Msg now, ignore restrictions
#define JAMATTR_DIRECT 0x00000400L         // Send directly to destination
#define JAMATTR_GATE 0x00000800L           // Send via gateway
#define JAMATTR_FILEREQUEST 0x00001000L    // File request
#define JAMATTR_FILEATTACH 0x00002000L     // File(s) attached to Msg
#define JAMATTR_TRUNCFILE 0x00004000L      // Truncate file(s) when sent
#define JAMATTR_KILLFILE 0x00008000L       // Delete file(s) when sent
#define JAMATTR_RECEIPTREQ 0x00010000L     // Return receipt requested
#define JAMATTR_CONFIRMREQ 0x00020000L     // Confirmation receipt requested
#define JAMATTR_ORPHAN 0x00040000L         // Unknown destination
#define JAMATTR_ENCRYPT 0x00080000L        // Msg text is encrypted
#define JAMATTR_COMPRESS 0x00100000L       // Msg text is compressed
#define JAMATTR_ESCAPED 0x00200000L        // Msg text is seven bit ASCII
#define JAMATTR_FPU 0x00400000L            // Force pickup
#define JAMATTR_TYPELOCAL 0x00800000L      // Msg is for local use only (not for export)
#define JAMATTR_TYPEECHO 0x01000000L       // Msg is for conference distribution
#define JAMATTR_TYPENET 0x02000000L        // Msg is direct network mail
#define JAMATTR_NODISP 0x20000000L         // Msg may not be displayed to user
#define JAMATTR_LOCKED 0x40000000L         // Msg is locked, no editing possible
#define JAMATTR_DELETED 0x80000000L        // Msg is deleted
//  ------------------------------------------------------------------
//  Message header subfield types

#define JAMSUB_OADDRESS 0
#define JAMSUB_DADDRESS 1
#define JAMSUB_SENDERNAME 2
#define JAMSUB_RECEIVERNAME 3
#define JAMSUB_MSGID 4
#define JAMSUB_REPLYID 5
#define JAMSUB_SUBJECT 6
#define JAMSUB_PID 7
#define JAMSUB_TRACE 8
#define JAMSUB_ENCLOSEDFILE 9
#define JAMSUB_ENCLOSEDFILEWALIAS 10
#define JAMSUB_ENCLOSEDFREQ 11
#define JAMSUB_ENCLOSEDFILEWCARD 12
#define JAMSUB_ENCLOSEDINDIRECFILE 13
#define JAMSUB_EMBINDAT 1000
#define JAMSUB_FTSKLUDGE 2000
#define JAMSUB_SEENBY2D 2001
#define JAMSUB_PATH2D 2002
#define JAMSUB_FLAGS 2003
#define JAMSUB_TZUTCINFO 2004

#define JAMSUB_OADDRESS_LEN 100
#define JAMSUB_DADDRESS_LEN 100
#define JAMSUB_SENDERNAME_LEN 100
#define JAMSUB_RECEIVERNAME_LEN 100
#define JAMSUB_MSGID_LEN 100
#define JAMSUB_REPLYID_LEN 100
#define JAMSUB_SUBJECT_LEN 100
#define JAMSUB_PID_LEN 40
#define JAMSUB_TRACE_LEN ((uint32_t)(-1))
#define JAMSUB_ENCLOSEDFILE_LEN ((uint32_t)(-1))
#define JAMSUB_ENCLOSEDFILEWALIAS_LEN ((uint32_t)(-1))
#define JAMSUB_ENCLOSEDFREQ_LEN ((uint32_t)(-1))
#define JAMSUB_ENCLOSEDFILEWCARD_LEN ((uint32_t)(-1))
#define JAMSUB_ENCLOSEDINDIRECFILE_LEN ((uint32_t)(-1))
#define JAMSUB_EMBINDAT_LEN ((uint32_t)(-1))
#define JAMSUB_FTSKLUDGE_LEN 255
#define JAMSUB_SEENBY2D_LEN ((uint32_t)(-1))
#define JAMSUB_PATH2D_LEN ((uint32_t)(-1))
#define JAMSUB_FLAGS_LEN ((uint32_t)(-1))
#define JAMSUB_TZUTCINFO_LEN ((uint32_t)(-1))
//  ------------------------------------------------------------------
//  .JHR file header
struct JamHdrInfo
{
    char     signature[4];  // <J><A><M> followed by <NUL>
    time32_t datecreated;   // Creation date
    uint32_t modcounter;    // Update counter
    uint32_t activemsgs;    // Number of active (not deleted) msgs
    uint32_t passwordcrc;   // CRC-32 of password to access
    uint32_t basemsgnum;    // Lowest message number in index file
    uint32_t highwatermark;    // Number of the last msg scanned
    byte     reserved[996]; // Reserved space
};

//  ------------------------------------------------------------------
//  .JHR message headers
struct JamHdr
{
    char     signature[4]; // <J><A><M> followed by <NUL>
    word     revision;    // Revision level of header
    word     reservedword; // Reserved for future use
    uint32_t subfieldlen;    // Length of subfields
    uint32_t timesread;   // Number of times message read
    uint32_t msgidcrc;    // CRC-32 of MSGID line
    uint32_t replycrc;    // CRC-32 of REPLY line
    uint32_t replyto;     // This msg is a reply to..
    uint32_t reply1st;    // First reply to this msg
    uint32_t replynext;   // Next msg in reply chain
    time32_t datewritten;   // When msg was written
    time32_t datereceived;  // When msg was read by recipient
    time32_t dateprocessed; // When msg was processed by tosser/scanner
    uint32_t messagenumber;  // Message number (1-based)
    uint32_t attribute;      // Msg attribute, see "Msg Attributes"
    uint32_t attribute2;     // Reserved for future use
    uint32_t offset;         // Offset of text in ????????.JDT file
    uint32_t txtlen;         // Length of message text
    uint32_t passwordcrc;    // CRC-32 of password to access message
    uint32_t cost;           // Cost of message
};

//  ------------------------------------------------------------------
//  .JHR subfield headers
struct JamSubFieldHdr
{
    word     loid;   // Field ID, 0-65535
    word     hiid;   // Reserved for future use
    uint32_t datlen; // Length of buffer that follows
};

//  ------------------------------------------------------------------
//  .JHR subfields
struct JamSubField
{
    word     loid;    // Field ID, 0-65535
    word     hiid;    // Reserved for future use
    uint32_t datlen;  // Length of buffer that follows
    char     buffer[101]; // DATLEN bytes of data
};

//  ------------------------------------------------------------------
//  .JDX message index
struct JamIndex
{
    dword    usercrc; // CRC-32 of destination username
    uint32_t hdroffset; // Offset of header in .JHR file
};

//  ------------------------------------------------------------------
//  .JLR lastread records
struct JamLast
{
    dword usercrc;   // CRC-32 of user name (lowercase)
    dword userid;    // Unique UserID
    dword lastread;  // Last read message number
    dword highread;  // Highest read message number
};

//  ------------------------------------------------------------------

#if defined (GOLD_CANPACK)
#pragma pack()
#endif
//  ------------------------------------------------------------------
struct JamData
{
    int        fhjhr;
    int        fhjdt;
    int        fhjdx;
    int        fhjlr;
    int        fhjhw;    // highwater if available
    int        islocked; // Area is locked?
    int        timesposted;
    int32_t    lastpos;  // Lastread position
    int32_t    highwater;
    JamLast    lastrec;  // .JLR Lastread record
    JamHdrInfo hdrinfo;  // .JHR Header info record
};

//  ------------------------------------------------------------------
struct JamWide
{
    dword        usercrc;
    dword        userid;
    int          lookreplies;
    const char * jampath;
    int          harddelete;
    int          smapihw;
};

//  ------------------------------------------------------------------
class JamArea : public gmo_area
{
protected: JamData * data;
    JamWide * wide;
    int just_scanning;
    void data_open();
    void data_close();
    void open_area();
    void raw_open();
    void raw_close();
    void save_lastread();
    int test_open(const char * file);
    void raw_scan(int __keep_index, int __scanpm = false);
    int load_message(int __mode, gmsg * __msg, JamHdr & __hdr);
    void add_subfield(JamHdr & __hdr,
                      byte * & __subfield,
                      word __loid,
                      word __hiid,
                      char * __data,
                      uint32_t maxlen);
    void save_message(int __mode, gmsg * __msg, JamHdr & __hdr);

public: JamArea()
    {
        wide          = NULL;
        data          = NULL;
        just_scanning = false;
    }

    virtual ~JamArea(){}
virtual bool issoftdelete() const
{
    return (wide != NULL) ? not wide->harddelete : true;
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
    Line * make_dump_msg(Line * & lin, gmsg * msg, char * lng_head);
};

//  ------------------------------------------------------------------
extern JamWide * jamwide;
extern JamData * jamdata;
extern int jamdatano;
//  ------------------------------------------------------------------
char * jamstrlwr(char * str);

//  ------------------------------------------------------------------

#endif // ifndef __GM_JAMM_H
//  ------------------------------------------------------------------
