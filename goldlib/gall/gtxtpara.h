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
//  Text -> Paragraph conversion.
//  ------------------------------------------------------------------

#ifndef __gtxtpara_h
#define __gtxtpara_h


//  ------------------------------------------------------------------

#include <gmemdbg.h>


//  ------------------------------------------------------------------
//  Macro to build a CHR4

#define MK_CHR4(a,b,c,d) ((dword)(a)|((dword)(b)<<8)|((dword)(c)<<16)|((dword)(d)<<24))


//  ------------------------------------------------------------------
//  CHR4 values for standard non-kludge control lines

const dword CHR4_AREA          = MK_CHR4('A','R','E','A');
const dword CHR4_TEARLINESPACE = MK_CHR4('-','-','-',' ');
const dword CHR4_TEARLINENUL   = MK_CHR4('-','-','-','\0');
const dword CHR4_ORIGIN        = MK_CHR4(' ','*',' ','O');
const dword CHR4_SEENBY        = MK_CHR4('S','E','E','N');


//  ------------------------------------------------------------------
//  CHR4 values for standard known kludges

const dword CHR4_INTL    = MK_CHR4('\001','I','N','T');
const dword CHR4_FMPT    = MK_CHR4('\001','F','M','P');
const dword CHR4_TOPT    = MK_CHR4('\001','T','O','P');
const dword CHR4_MSGID   = MK_CHR4('\001','M','S','G');
const dword CHR4_REPLY   = MK_CHR4('\001','R','E','P');
const dword CHR4_PID     = MK_CHR4('\001','P','I','D');
const dword CHR4_SEENBY1 = MK_CHR4('\001','S','E','E');
const dword CHR4_PATH    = MK_CHR4('\001','P','A','T');
const dword CHR4_CHARSET = MK_CHR4('\001','C','H','A');
const dword CHR4_CHRS    = MK_CHR4('\001','C','H','R');
const dword CHR4_TZUTC   = MK_CHR4('\001','T','Z','U');
const dword CHR4_VIA     = MK_CHR4('\001','V','I','A');
const dword CHR4_FLAGS   = MK_CHR4('\001','F','L','A');


//  ------------------------------------------------------------------
//  Control line enumeration

enum {

  // Non-ctrl line
  NOT_CTRL = 0,

  // Standard non-kludge control lines
  CTRL_AREA,
  CTRL_TEARLINE,
  CTRL_ORIGIN,

  // Standard known kludges
  CTRL_KLUDGE,
  CTRL_INTL,
  CTRL_FMPT,
  CTRL_TOPT,
  CTRL_MSGID,
  CTRL_REPLY,
  CTRL_PID,
  CTRL_SEENBY,
  CTRL_SEENBY1,
  CTRL_PATH,
  CTRL_CHARSET,
  CTRL_CHRS,
  CTRL_TZUTC,
  CTRL_VIA,
  CTRL_FLAGS,

  // Unknown kludges
  CTRL_UNKNOWNKLUDGE
};


//  ------------------------------------------------------------------

struct GParaData {

  char* text;
  uint length;
  uint control;
};


//  ------------------------------------------------------------------

class GParagraph {

public:

  int lines;
  char softcr;
  GParaData* paraidx;

  GParagraph();
  ~GParagraph();

  void AddParagraph(char* __text, uint __length);
  int ConvertText(char* __text, uint __length);
  uint CheckCtrlLines();
};


//  ------------------------------------------------------------------

inline GParagraph::GParagraph() {

  lines = 0;
  softcr = LF;
  paraidx = NULL;
}


//  ------------------------------------------------------------------

inline GParagraph::~GParagraph() {

  throw_free(paraidx);
}


//  ------------------------------------------------------------------

inline void GParagraph::AddParagraph(char* __text, uint __length) {

  paraidx = (GParaData*)throw_realloc(paraidx, (lines+1)*sizeof(GParaData));
  GParaData* _para = paraidx + lines++;
  _para->text = __text;
  _para->length = __length;
  _para->control = 0;
}


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
