//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 2000 Alexander S. Aganichev
//  ------------------------------------------------------------------
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License as
//  published by the Free Software Foundation; either version 2 of the
//  License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//  ------------------------------------------------------------------
//  $Id$
//  ------------------------------------------------------------------
//  Based on freeware sources from Digital Dynamics
//  ------------------------------------------------------------------
//  Synchronet message base
//  ------------------------------------------------------------------

#ifndef __GMOSMB_H
#define __GMOSMB_H


//  ------------------------------------------------------------------

#include <gmoarea.h>


//  ------------------------------------------------------------------
//  SMB library version used as reference (1.21a)

const int SMB_VERSION = 0x0121; // SMB format version (High byte major, low byte minor)


//  ------------------------------------------------------------------

const int SDT_BLOCK_LEN = 256;  // Size of data blocks
const int SHD_BLOCK_LEN	= 256;  // Size of header blocks

const int SMB_SELFPACK   = 0;   // Self-packing storage allocation
const int SMB_FASTALLOC  = 1;   // Fast allocation
const int SMB_HYPERALLOC = 2;   // No allocation

const int SMB_EMAIL = 1;        // User numbers stored in Indexes

// Time zone macros for when_t.zone
const int DAYLIGHT     = 0x8000; // Daylight savings is active
const int US_ZONE      = 0x4000; // U.S. time zone
const int WESTERN_ZONE = 0x2000; // Non-standard zone west of UT
const int EASTERN_ZONE = 0x1000; // Non-standard zone east of UT

// Valid hfield_t.types
const int SENDER            = 0x00;
const int SENDERAGENT       = 0x01;
const int SENDERNETTYPE     = 0x02;
const int SENDERNETADDR     = 0x03;
const int SENDEREXT         = 0x04;
const int SENDERPOS         = 0x05;
const int SENDERORG         = 0x06;
                          
const int AUTHOR            = 0x10;
const int AUTHORAGENT       = 0x11;
const int AUTHORNETTYPE     = 0x12;
const int AUTHORNETADDR     = 0x13;
const int AUTHOREXT         = 0x14;
const int AUTHORPOS         = 0x15;
const int AUTHORORG         = 0x16;

const int REPLYTO           = 0x20;
const int REPLYTOAGENT      = 0x21;
const int REPLYTONETTYPE    = 0x22;
const int REPLYTONETADDR    = 0x23;
const int REPLYTOEXT        = 0x24;
const int REPLYTOPOS        = 0x25;
const int REPLYTOORG        = 0x26;

const int RECIPIENT         = 0x30;
const int RECIPIENTAGENT    = 0x31;
const int RECIPIENTNETTYPE  = 0x32;
const int RECIPIENTNETADDR  = 0x33;
const int RECIPIENTEXT      = 0x34;
const int RECIPIENTPOS      = 0x35;
const int RECIPIENTORG      = 0x36;

const int FORWARDTO         = 0x40;
const int FORWARDTOAGENT    = 0x41;
const int FORWARDTONETTYPE  = 0x42;
const int FORWARDTONETADDR  = 0x43;
const int FORWARDTOEXT      = 0x44;
const int FORWARDTOPOS      = 0x45;
const int FORWARDTOORG      = 0x46;

const int FORWARDED         = 0x48;

const int RECEIVEDBY        = 0x50;
const int RECEIVEDBYAGENT   = 0x51;
const int RECEIVEDBYNETTYPE = 0x52;
const int RECEIVEDBYNETADDR = 0x53;
const int RECEIVEDBYEXT     = 0x54;
const int RECEIVEDBYPOS     = 0x55;
const int RECEIVEDBYORG     = 0x56;

const int RECEIVED          = 0x58;

const int SUBJECT           = 0x60;
const int SUMMARY           = 0x61;
const int SMBCOMMENT        = 0x62;
const int CARBONCOPY        = 0x63;
const int GROUP             = 0x64;
const int EXPIRATION        = 0x65;
const int PRIORITY          = 0x66;

const int FILEATTACH        = 0x70;
const int DESTFILE          = 0x71;
const int FILEATTACHLIST    = 0x72;
const int DESTFILELIST      = 0x73;
const int FILEREQUEST       = 0x74;
const int FILEPASSWORD      = 0x75;
const int FILEREQUESTLIST   = 0x76;
const int FILEPASSWORDLIST  = 0x77;

const int IMAGEATTACH       = 0x80;
const int ANIMATTACH        = 0x81;
const int FONTATTACH        = 0x82;
const int SOUNDATTACH       = 0x83;
const int PRESENTATTACH     = 0x84;
const int VIDEOATTACH       = 0x85;
const int APPDATAATTACH     = 0x86;

const int IMAGETRIGGER      = 0x90;
const int ANIMTRIGGER       = 0x91;
const int FONTTRIGGER       = 0x92;
const int SOUNDTRIGGER      = 0x93;
const int PRESENTTRIGGER    = 0x94;
const int VIDEOTRIGGER      = 0x95;
const int APPDATATRIGGER    = 0x96;

const int FIDOCTRL          = 0xa0;
const int FIDOAREA          = 0xa1;
const int FIDOSEENBY        = 0xa2;
const int FIDOPATH          = 0xa3;
const int FIDOMSGID         = 0xa4;
const int FIDOREPLYID       = 0xa5;
const int FIDOPID           = 0xa6;
const int FIDOFLAGS         = 0xa7;

const int RFC822HEADER      = 0xb0;
const int RFC822MSGID       = 0xb1;
const int RFC822REPLYID     = 0xb2;

const int UNKNOWN           = 0xf1;
const int UNKNOWNASCII      = 0xf2;
const int UNUSED            = 0xff;

// Valid dfield_t.types
const int TEXT_BODY         = 0x00;
const int TEXT_SOUL         = 0x01;
const int TEXT_TAIL         = 0x02;
const int TEXT_WING         = 0x03;
const int IMAGEEMBED        = 0x20;
const int ANIMEMBED         = 0x21;
const int FONTEMBED         = 0x22;
const int SOUNDEMBED        = 0x23;
const int PRESENTEMBED      = 0x24;
const int VIDEOEMBED        = 0x25;
const int APPDATAEMBED      = 0x26;
//const int UNUSED          = 0xff;

// Message attributes
const int MSG_PRIVATE       = (1<<0);
const int MSG_READ          = (1<<1);
const int MSG_PERMANENT     = (1<<2);
const int MSG_LOCKED        = (1<<3);
const int MSG_DELETE        = (1<<4);
const int MSG_ANONYMOUS     = (1<<5);
const int MSG_KILLREAD      = (1<<6);
const int MSG_MODERATED     = (1<<7);
const int MSG_VALIDATED     = (1<<8);

// Auxillary header attributes
const int MSG_FILEREQUEST   = (1<<0);  // File request
const int MSG_FILEATTACH    = (1<<1);  // File(s) attached to Msg
const int MSG_TRUNCFILE     = (1<<2);  // Truncate file(s) when sent
const int MSG_KILLFILE      = (1<<3);  // Delete file(s) when sent
const int MSG_RECEIPTREQ    = (1<<4);  // Return receipt requested
const int MSG_CONFIRMREQ    = (1<<5);  // Confirmation receipt requested
const int MSG_NODISP        = (1<<6);  // Msg may not be displayed to user

// Message network attributes
const int MSG_LOCAL         = (1<<0);  // Msg created locally
const int MSG_INTRANSIT     = (1<<1);  // Msg is in-transit
const int MSG_SENT          = (1<<2);  // Sent to remote
const int MSG_KILLSENT      = (1<<3);  // Kill when sent
const int MSG_ARCHIVESENT   = (1<<4);  // Archive when sent
const int MSG_HOLD          = (1<<5);  // Hold for pick-up
const int MSG_CRASH         = (1<<6);  // Crash
const int MSG_IMMEDIATE     = (1<<7);  // Send Msg now, ignore restrictions
const int MSG_DIRECT        = (1<<8);  // Send directly to destination
const int MSG_GATE          = (1<<9);  // Send via gateway
const int MSG_ORPHAN        = (1<<10); // Unknown destination
const int MSG_FPU           = (1<<11); // Force pickup
const int MSG_TYPELOCAL     = (1<<12); // Msg is for local use only
const int MSG_TYPEECHO      = (1<<13); // Msg is for conference distribution
const int MSG_TYPENET       = (1<<14); // Msg is direct network mail

enum {
  NET_NONE,
  NET_UNKNOWN,
  NET_FIDO,
  NET_POSTLINK,
  NET_QWK,
  NET_INTERNET,
  NET_WWIV,
  NET_MHS,
  // Add new ones here
  NET_TYPES
};

enum {
  AGENT_PERSON,
  AGENT_PROCESS,
  // Add new ones here
  AGENT_TYPES
};

enum {
  XLAT_NONE,              // No translation/End of translation list
  XLAT_ENCRYPT,           // Encrypted data
  XLAT_ESCAPED,           // 7-bit ASCII escaping for ctrl and 8-bit data
  XLAT_HUFFMAN,           // Static and adaptive Huffman coding compression
  XLAT_LZW,               // Limpel/Ziv/Welch compression
  XLAT_MLZ78,             // Modified LZ78 compression
  XLAT_RLE,               // Run length encoding compression
  XLAT_IMPLODE,           // Implode compression (PkZIP)
  XLAT_SHRINK,            // Shrink compression (PkZIP)
  XLAT_LZH,               // LHarc (LHA) Dynamic Huffman coding
  // Add new ones here
  XLAT_TYPES
};


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif


//  ------------------------------------------------------------------

typedef struct {
  time_t time;  // Local time (unix format)
  sword  zone;  // Time zone
} when_t;

typedef struct {
  word to;      // 16-bit CRC of recipient name (lower case)
  word from;    // 16-bit CRC of sender name (lower case)
  word subj;    // 16-bit CRC of subject (lower case, w/o RE:)
  word attr;    // attributes (read, permanent, etc.)
  dword offset; // offset into header file
  dword number; // number of message (1 based)
  time_t time;  // time/date message was imported/posted
} idxrec_t;

typedef struct {
  uchar id[4];  // text or binary unique hdr ID
  word version; // version number (initially 100h for 1.00)
  word length;  // length including this struct
} smbhdr_t;

typedef struct {
  dword last_msg;       // last message number
  dword total_msgs;     // total messages
  dword header_offset;  // byte offset to first header record
  dword max_crcs;       // Maximum number of CRCs to keep in history
  dword max_msgs;       // Maximum number of message to keep in sub
  word  max_age;        // Maximum age of message to keep in sub (in days)
  word  attr;           // Attributes for this message base (SMB_HYPER, etc)
} smbstatus_t;

typedef struct {
  uchar id[4];          // SHD<^Z>
  word  type;           // Message type (normally 0)
  word  version;        // Version of type (initially 100h for 1.00)
  word  length;         // Total length of fixed record + all fields
  word  attr;           // Attributes (bit field) (duped in SID)
  dword auxattr;        // Auxillary attributes (bit field)
  dword netattr;        // Network attributes
  when_t when_written;  // Time message was written (unix format)
  when_t when_imported; // Time message was imported
  dword number;         // Message number
  dword thread_orig;    // Original message number in thread
  dword thread_next;    // Next message in thread
  dword thread_first;   // First reply to this message
  uchar reserved[16];   // Reserved for future use
  dword offset;         // Offset for buffer into data file (0 or mod 256)
  word  total_dfields;  // Total number of data fields
} msghdr_t;

typedef struct {
  word  type;           // Type of data field
  dword offset;         // Offset into buffer 
  dword length;         // Length of data field
} dfield_t;

typedef struct {
  word type;
  word length;          // Length of buffer
} hfield_t;

typedef struct {
  word zone, net, node, point;
} fidoaddr_t;

typedef struct {
  word type;
  void *addr;
} net_t;

//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack()
#endif


//  ------------------------------------------------------------------

typedef struct {
  idxrec_t idx;           // Index
  msghdr_t hdr;           // Header record (fixed portion)
  uchar    *to,           // To name
           *to_ext,       // To extension
           *from,         // From name
           *from_ext,     // From extension
           *replyto,      // Reply-to name
           *replyto_ext,  // Reply-to extension */
           *subj;         // Subject
  word     to_agent,      // Type of agent message is to
           from_agent,    // Type of agent message is from
           replyto_agent; // Type of agent replies should be sent to
  net_t    to_net,        // Destination network type and address
           from_net,      // Origin network address
           replyto_net;   // Network type and address for replies
  word     total_hfields; // Total number of header fields
  hfield_t *hfield;       // Header fields (fixed length portion)
  void     **hfield_dat;  // Header fields (variable length portion)
  dfield_t *dfield;       // Data fields (fixed length portion)
  dword    offset;        // Offset (number of records) into index
  uchar    forwarded;     // Forwarded from agent to another
} smbmsg_t;


//  ------------------------------------------------------------------

struct SMBData {
  char  shd_buf[SHD_BLOCK_LEN];
  FILE  *sdt_fp, *shd_fp, *sid_fp, *sda_fp, *sha_fp;
};


//  ------------------------------------------------------------------

class SMBArea : public gmo_area {

protected:

  SMBData* data;

  void  data_open();
  void  data_close();

  void  raw_scan(bool keep_index=false, bool scanpm=false);

  FILE* smb_openexlusively(const char *file, int retry_time);
  int   smb_openexlusively2(const char *file, int retry_time);

  int   smb_open(int retry_time);
  void  smb_close(void);
  int   smb_open_da(int retry_time);
  int   smb_open_ha(int retry_time);
  int   smb_create(dword max_crcs, dword max_msgs, word max_age, word attr, int retry_time);
  int   smb_trunchdr(int retry_time);
  int   smb_locksmbhdr(int retry_time);
  int   smb_getstatus(smbstatus_t *status);
  int   smb_putstatus(smbstatus_t status);
  int   smb_unlocksmbhdr(void);
  int   smb_getmsgidx(smbmsg_t *msg);
  int   smb_getlastidx(idxrec_t *idx);
  uint  smb_getmsghdrlen(smbmsg_t msg);
  dword smb_getmsgdatlen(smbmsg_t msg);
  int   smb_lockmsghdr(smbmsg_t msg, int retry_time);
  int   smb_getmsghdr(smbmsg_t *msg);
  int   smb_unlockmsghdr(smbmsg_t msg);
  int   smb_addcrc(dword max_crcs, dword crc, int retry_time);
  int   smb_hfield(smbmsg_t *msg, word type, word length, void *data);
  int   smb_dfield(smbmsg_t *msg, word type, dword length);
  int   smb_addmsghdr(smbmsg_t *msg, smbstatus_t *status, int storage, int retry_time);
  int   smb_putmsg(smbmsg_t msg);
  int   smb_putmsgidx(smbmsg_t msg);
  int   smb_putmsghdr(smbmsg_t msg);
  void  smb_freemsgmem(smbmsg_t msg);
  dword smb_hdrblocks(dword length);
  dword smb_datblocks(dword length);
  long  smb_allochdr(dword length);
  long  smb_fallochdr(dword length);
  long  smb_hallochdr(dword header_offset);
  long  smb_allocdat(dword length, word headers);
  long  smb_fallocdat(dword length, word headers);
  long  smb_hallocdat(void);
  int   smb_incdat(dword offset, dword length, word headers);
  int   smb_freemsg(smbmsg_t msg, smbstatus_t status);
  int   smb_freemsgdat(dword offset, dword length, word headers);
  int   smb_freemsghdr(dword offset, dword length);
  int   load_hdr(gmsg* __msg, smbmsg_t *msg);

public:

  SMBArea() { data = NULL; }
  virtual ~SMBArea() {}

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

  int load_hdr(gmsg* msg) { return load_hdr(msg, NULL); }
  int load_msg(gmsg* msg);

  void save_hdr(int mode, gmsg* msg);
  void save_msg(int mode, gmsg* msg);

  void del_msg(gmsg* msg);

  void new_msgno(gmsg* msg);
  char* user_lookup(char* lookfor);
  int renumber();

  void update_timesread(gmsg* msg);

  Line* make_dump_msg(Line*& lin, gmsg* msg, char* lng_head);
};


//  ------------------------------------------------------------------

extern SMBData* smbdata;
extern int smbdatano;


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
