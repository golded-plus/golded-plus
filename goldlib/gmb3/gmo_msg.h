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
//  Message structures and classes.
//  ------------------------------------------------------------------


//  ------------------------------------------------------------------

#ifndef __GMB_MSG_H
#define __GMB_MSG_H


//  ------------------------------------------------------------------

#include <gftnall.h>
#include <gtimall.h>
#include <gvidall.h>
#include <gmsgattr.h>


//  ------------------------------------------------------------------
//  Internet name typedefs

typedef char INam[128];


//  ------------------------------------------------------------------
//  Internet subject typedefs

typedef char ISub[1301];


//  ------------------------------------------------------------------
//  Message modes

const uint GMSG_HDR         = 0x0001;
const uint GMSG_TXT         = 0x0002;
const uint GMSG_HDRTXT      = 0x0003;  // GMSG_HDR|GMSG_TXT
const uint GMSG_NEW         = 0x0010;
const uint GMSG_UPDATE      = 0x0020;
const uint GMSG_DELETE      = 0x0040;
const uint GMSG_COPY        = 0x0080;
const uint GMSG_MOVE        = 0x0100;
const uint GMSG_UNS_NOT_RCV = 0x0200;
const uint GMSG_NOLSTUPD    = 0x8000;


//  ------------------------------------------------------------------
//  Line attributes

const uint GLINE_TEAR = 0x0001;
const uint GLINE_ORIG = 0x0002;
const uint GLINE_HIDD = 0x0004;
const uint GLINE_KLUD = 0x0008;
const uint GLINE_QUOT = 0x0010;
const uint GLINE_HARD = 0x0020;
const uint GLINE_WRAP = 0x0040;
const uint GLINE_BLOK = 0x0080;
const uint GLINE_POSI = 0x0100;
const uint GLINE_HIGH = 0x0200;
const uint GLINE_TAGL = 0x0400;
const uint GLINE_SIGN = 0x0800;
const uint GLINE_TXTH = 0x1000;  // text is hiden
const uint GLINE_NOAL = 0x2000;  // text is not allocated

const uint GLINE_KLUDGE = GLINE_HIDD | GLINE_KLUD;
const uint GLINE_ALL    = GLINE_HIDD | GLINE_KLUD | GLINE_QUOT;


//  ------------------------------------------------------------------
//  Kludge types

const uint GKLUD_INTL      = 0x0001;
const uint GKLUD_FMPT      = 0x0002;
const uint GKLUD_TOPT      = 0x0004;
const uint GKLUD_FLAGS     = 0x0008;
const uint GKLUD_AREA      = 0x0010;
const uint GKLUD_MSGID     = 0x0020;
const uint GKLUD_REPLY     = 0x0040;
const uint GKLUD_PID       = 0x0080;
const uint GKLUD_FWD       = 0x0100;
const uint GKLUD_PATH      = 0x0200;
const uint GKLUD_SEENBY    = 0x0400;
const uint GKLUD_CHARSET   = 0x0800;
const uint GKLUD_REPLYTO   = 0x1000;
const uint GKLUD_REPLYADDR = 0x2000;
const uint GKLUD_KNOWN     = 0x4000;
const uint GKLUD_RFC       = 0x8000;


//  ------------------------------------------------------------------
//  Search locations

const uint GFIND_FROM      = 0x0001;
const uint GFIND_TO        = 0x0002;
const uint GFIND_SUBJECT   = 0x0004;
const uint GFIND_BODY      = 0x0008;
const uint GFIND_TAGLINE   = 0x0010;
const uint GFIND_TEARLINE  = 0x0020;
const uint GFIND_ORIGIN    = 0x0040;
const uint GFIND_KLUDGES   = 0x0080;
const uint GFIND_SIGNATURE = 0x0100;
const uint GFIND_HDR       = GFIND_FROM | GFIND_TO | GFIND_SUBJECT;
const uint GFIND_HDRTXT    = GFIND_HDR | GFIND_BODY;


//  ------------------------------------------------------------------
//  Charset encoding formats

const int GCHENC_CMP = 0x0001;    // Composed characters
const int GCHENC_I51 = 0x0002;    // FSC-51
const int GCHENC_MNE = 0x0004;    // K.Simonsen Mnemonic
const int GCHENC_QP  = 0x0008;    // Quoted-Printable


//  ------------------------------------------------------------------
//  Line record

class Line {

public:

  vattr color;            // Line color
  uint  type;             // GLINE_*
  uint  kludge;           // GKLUD_*
  std::string txt;             // The line text
  Line* prev;             // Pointer to previous line
  Line* next;             // Pointer to next line

  Line() : txt ("")     { color = BLACK|_BLACK; type = kludge = 0; prev = next = NULL; }
  Line(const char *str) : txt (str) { color = BLACK|_BLACK; type = kludge = 0; prev = next = NULL; }
  ~Line()              {}

  int  istearline()    { return !!(type & GLINE_TEAR); }
  int  isorigin()      { return !!(type & GLINE_ORIG); }
  int  ishidden()      { return !!(type & GLINE_HIDD); }
  int  iskludge()      { return !!(type & GLINE_KLUD); }
  int  isquote()       { return !!(type & GLINE_QUOT); }
  int  ishard()        { return !!(type & GLINE_HARD); }
  int  iswrapped()     { return !!(type & GLINE_WRAP); }
  int  isblock()       { return !!(type & GLINE_BLOK); }
  int  isposition()    { return !!(type & GLINE_POSI); }
  int  ishighlighted() { return !!(type & GLINE_HIGH); }
  int  istagline()     { return !!(type & GLINE_TAGL); }
  int  istxthidden()   { return !!(type & GLINE_TXTH); }
  int  isheader()      { return !!kludge; }

  int  isallocated()   { return !(type & GLINE_NOAL); }
};


//  ------------------------------------------------------------------

Line* AddLineF(Line*& line, const char* format, ...) __attribute__ ((format (printf, 2, 3)));
Line* AddLine(Line* line, const char* buf);
Line* AddHexdump(Line*& line, void* data, size_t datalen);


//  ------------------------------------------------------------------

class gmsg_links {

private:

  enum { list_limit = 29 };

  uint32_t reply_to;
  uint32_t reply_first;
  uint32_t reply_list[list_limit];
  uint32_t reply_next;

public:

  void reset() {
    reply_to = reply_first = reply_next = 0;
    for(int n=0; n<list_max(); n++)
      reply_list[n] = 0;
  }

  int list_max() const          { return list_limit; }
  
  void to_set(uint32_t m)          { reply_to = m; }
  void first_set(uint32_t m)       { reply_first = m; }
  void list_set(int n, uint32_t m) { reply_list[n] = m; }
  void next_set(uint32_t m)        { reply_next = m; }
  
  uint32_t to() const              { return reply_to; }
  uint32_t first() const           { return reply_first; }
  uint32_t list(int n) const       { return reply_list[n]; }
  uint32_t next() const            { return reply_next; }

};


//  ------------------------------------------------------------------

struct gmsg_jam_fields {

  int32_t subfieldlen;   // Size of subfields
};


//  ------------------------------------------------------------------

struct gmsg_pcboard_fields {

  char status;          // Msg header status byte
  byte exthdrflags;     // Msg extended header flags
  char password[13];    // Password needed to read the message
  time32_t reply_written;   // Timestamp of the original
};


//  ------------------------------------------------------------------

struct gmsg_wildcat_fields {

  char  from_title[11];
  int32_t  from_userid;
  char  to_title[11];
  int32_t  to_userid;
  char  network[9];
  char  internal_attach[13];
  char  external_attach[13];
  uint32_t next_unread;
  uint32_t prev_unread;
  char  reserved[20];
};


//  ------------------------------------------------------------------

struct gmsg_adeptxbbs_fields {

  uint32_t iflags;       // Internet related flags
  uint32_t oflags;       // Other network related flags
};


//  ------------------------------------------------------------------

struct gmsg_ezycom_fields {

  byte extattr;
};


//  ------------------------------------------------------------------
//  Base class

class gmsg {

public:

  uint        board;            // Board number (if applicable)

  uint32_t       msgno;            // Message number
  gmsg_links  link;             // Message reply links

  ftn_addr    oorig;            // Original origination address
  ftn_addr    orig;             // Origination address
  ftn_domain  odom;             // Originating domain

  ftn_addr    odest;            // Original destination address
  ftn_addr    dest;             // Destination address
  ftn_domain  ddom;             // Destination domain

  INam        by;               // Who from
  INam        to;               // Who to
  ISub        re;               // Subject

  time32_t      written;          // Timestamp
  time32_t      arrived;          // Timestamp
  time32_t      received;         // Timestamp

  Attr        attr;             // Message attributes
  uint        cost;             // Cost of msg if Netmail
  uint        timesread;        // Number of times read

  ftn_addr    msgid;            // MSGID kludge address
  char        msgids[201];      // MSGID kludge string
  char        replys[201];      // REPLY kludge string

  char        pid[80];          // PID kludge string

  int32_t        txtstart;         // Text starting position or record
  int32_t        txtlength;        // Text length or number of records
  uint        txtblocks;        // Number of msg text blocks

  char*       txt;              // Message text

  gmsg_jam_fields       jam;
  gmsg_pcboard_fields   pcboard;
  gmsg_wildcat_fields   wildcat;
  gmsg_adeptxbbs_fields adeptxbbs;
  gmsg_ezycom_fields    ezycom;

};


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------

