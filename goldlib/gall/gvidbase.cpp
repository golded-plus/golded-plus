//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999-2000 Alexander S. Aganichev
//  Copyright (C) 2000 Jacobo Tarrio
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
//  Device-independent video functions.
//  ------------------------------------------------------------------

#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <gmemall.h>
#include <gmemdbg.h>
#include <gstrall.h>
#include <gvidall.h>

#if defined(__OS2__)
#define INCL_BASE
#include <os2.h>
#ifndef __EMX__
#define PCCH CHAR*
#endif
#endif

#ifdef __WIN32__
#include <windows.h>
#endif

#ifdef __GNUC__
#include <unistd.h>
#endif

#if defined(__DJGPP__)
#include <sys/farptr.h>
#endif


//  ------------------------------------------------------------------
//  Check if Borland C++ for OS/2 1.0 header has been fixed

#if defined(__OS2__) && defined(__BORLANDC__)
  #if __BORLANDC__ <= 0x400
    #ifndef BCOS2_BSESUB_FIXED
    #error There is a bug in the BSESUB.H header. Please fix it.
    //
    // Add/change the following in BSESUB.H:
    //
    // #define BCOS2_BSESUB_FIXED
    // APIRET16  APIENTRY16    VioGetState (PVOID16 pState, HVIO hvio);
    // APIRET16  APIENTRY16    VioSetState (PVOID16 pState, HVIO hvio);
    //
    // Borland forgot this (was only PVOID)      ^^
    //
    #endif
  #endif
#endif


//  ------------------------------------------------------------------

static bool __vcurhidden = false;
#if defined(__UNIX__) || defined(__USE_NCURSES__)
static uint32_t gvid_boxcvtc(char);
#endif

#if !defined(__USE_NCURSES__)

//  ------------------------------------------------------------------

#ifdef __WIN32__
extern HANDLE gvid_hout;
extern OSVERSIONINFO WinVer; // defined in gutlwin.cpp
extern WCHAR oem2unicode[]; // defined in gutlwin.cpp

//  ------------------------------------------------------------------
//  Transform character < 32 into printable Unicode equivalent


inline WCHAR gvid_tcpr(vchar chr) {

  return oem2unicode[chr & 0xff];
}

#endif


//  ------------------------------------------------------------------

#if defined(__MSDOS__) || defined(__UNIX__)

#if defined(__MSDOS__)
extern int __gdvdetected;
#endif

#ifndef __DJGPP__
const uint16_t _dos_ds = 0;

inline uint16_t _my_ds(void) {

  return 0;
}

inline void _farpokew(uint16_t s, gdma ptr, word chat) {

  NW(s);
  *ptr = chat;
}

inline void _farnspokew(gdma ptr, word chat) {

  *ptr = chat;
}

inline word _farpeekw(uint16_t s, gdma ptr) {

  NW(s);
  return *ptr;
}

inline void _farnspokeb(byte *ptr, byte chr) {

  *ptr = chr;
}

inline void _farsetsel(uint16_t s) {

  NW(s);
}
#endif

#ifdef __DJGPP__
const int ATTRSIZE = sizeof(word);
#else
const int ATTRSIZE = 1;
#endif

inline void gdmacpy(uint16_t seg_d, gdma sel_d, uint16_t seg_s, gdma sel_s, int len) {

  #ifdef __DJGPP__
  movedata(seg_s, sel_s, seg_d, sel_d, len);
  #else
  NW(seg_d);
  NW(seg_s);
  memcpy(sel_d, sel_s, len);
  #endif
}

inline gdma gdmaptr(int col, int row) {

  return gvid->dmaptr+ATTRSIZE*((row*gvid->numcols)+col);
}
#endif


//  ------------------------------------------------------------------

#if defined(__UNIX__)


//  ------------------------------------------------------------------

extern int gvid_stdout;
extern const char* gvid_acs_enable;
extern const char* gvid_acs_disable;
int gvid_last_attr = 0;


//  ------------------------------------------------------------------

void gvid_printf(const char* fmt, ...) {

  char buf[1024];
  va_list argptr;
  va_start(argptr, fmt);
  int n = vsprintf(buf, fmt, argptr);
  va_end(argptr);

  write(gvid_stdout, buf, n);
}


//  ------------------------------------------------------------------
//  Control chars      01234567890123456789012345678901

const char* gvid_x0 = "x@xxxxxxxxxxxxxx><xxxxxx^vxxxx^v";


//  ------------------------------------------------------------------

inline void gvid_cvtchr(char& ch) {

  register const char* x0 = gvid_x0;

  if(ch < ' ')
    ch = x0[ch];
}


//  ------------------------------------------------------------------

void gvid_cvtstr(char* s, int len) {

  register char* p = s;

  for(int n=0; n<len; n++,p++)
    gvid_cvtchr(*p);
}


//  ------------------------------------------------------------------

void gvid_cvtstr(word* ws, int len) {

  register word* wp = ws;

  for(int n=0; n<len; n++,wp++)
    gvid_cvtchr(*(char*)wp);
}


//  ------------------------------------------------------------------

static int _atr_to_ans[8] = {
  0,  // BLACK    0
  4,  // BLUE     1
  2,  // GREEN    2
  6,  // CYAN     3
  1,  // RED      4
  5,  // MAGENTA  5
  3,  // BROWN    6
  7   // WHITE    7
};


//  ------------------------------------------------------------------

inline int vatr2ansin(int x) {

  return (x & 8) ? 1 : 0;
}


//  ------------------------------------------------------------------

inline int vatr2ansfg(int x) {

  return _atr_to_ans[x & 7];
}


//  ------------------------------------------------------------------

inline int vatr2ansbg(int x) {

  return _atr_to_ans[(x>>4) & 7];
}


//  ------------------------------------------------------------------

void vputansi(int row, int col, word* buf, int len) {

  char ch;
  int in, fg, bg, acs;
  int atr = gvid_last_attr;
  int in0 = vatr2ansin(atr);
  int fg0 = vatr2ansfg(atr);
  int bg0 = vatr2ansbg(atr);
  int acs0 = atr & ACSET;

  // Get pointer to ANSI line buffer
  char* ptr = gvid->bufansi;

  // Get pointer to video memory image
  byte* p = (byte*)buf;

  for(int n=0; n<len; n++,p+=2) {    // For each screen element

    if(p[1] != atr) {                 // If attribute is different

      atr = p[1];                     // Store new attribute
      gvid_last_attr = atr;

      in = vatr2ansin(atr);           // Get intensity
      fg = vatr2ansfg(atr);           // Get foreground color
      bg = vatr2ansbg(atr);           // Get background color
      acs = atr & ACSET;              // Get Alt Color Set

      if(acs != acs0) {
        ptr = stpcpy(ptr, acs ? gvid_acs_enable : gvid_acs_disable);
        acs0 = acs;
      }

      *ptr++ = 0x1B;                  // Start ANSI color sequence
      *ptr++ = '[';

      if(in != in0) {                 // Set intensity if different
        if(in)
          *ptr++ = '1';               // Intense
        else {
          *ptr++ = '0';               // Reset
          fg0 = bg0 = -1;
        }
        in0 = in;
        if((fg != fg0) or (bg != bg0))
          *ptr++ = ';';
      }

      if(fg != fg0) {                 // Set foreground if different
        *ptr++ = '3';
        *ptr++ = (char)('0' + fg);
        fg0 = fg;
        if(bg != bg0)
          *ptr++ = ';';
      }

      if(bg != bg0) {                 // Set background if different
        *ptr++ = '4';
        *ptr++ = (char)('0' + bg);
        bg0 = bg;
      }

      *ptr++ = 'm';                   // End ANSI color sequence
    }

    ch = p[0];
    gvid_cvtchr(ch);
    *ptr++ = ch;                    // Output the character
  }

  *ptr = NUL;                         // Terminate string

  // Print complete ANSI string at the specified position
  gvid_printf("\x1B[%u;%uH%s", row+1, col+1, gvid->bufansi);
}


//  ------------------------------------------------------------------

#endif

#endif // !defined(__USE_NCURSES__)


//  ------------------------------------------------------------------
//  Converts an attribute to monochrome equivalent

int mapattr(int attr) {

  switch(attr&112) {      // test for a light background

    case _LGREY:
    case _GREEN:
    case _CYAN:
    case _BROWN:
      attr &= 240;        // foreground = black
      attr |= 112;        // background = light grey
      break;

    default:
      if((attr&15)==8)    // if foreground = dark grey
        attr &= 247;      // clear intensity bit
      attr |= 7;          // foreground = light grey
      attr &= 143;        // background = black
  }

  return attr;              // return converted attribute
}


//  ------------------------------------------------------------------
//  Reverses the attribute given

int revsattr(int attr) {

  return (int)(((attr>>4)&0x07)|((attr<<4)&0x70)|(attr&0x80)|(attr&0x08));
}

#if !defined(__USE_NCURSES__)


//  ------------------------------------------------------------------

#if defined(__UNIX__)
char* gvid_newattr(int& attr) {

  // 12345678901234567890
  // E[1;33;44mE[11m
  static char newattr[20];
  *newattr = NUL;
  if(attr != gvid_last_attr) {
    if((attr & ~ACSET) != (gvid_last_attr & ~ACSET)) {
      sprintf(newattr, "\033[%c;3%u;4%um",
        vatr2ansin(attr) ? '1' : '0',
        vatr2ansfg(attr),
        vatr2ansbg(attr)
      );
    }
    if((attr & ACSET) != (gvid_last_attr & ACSET))
      strcat(newattr, (attr & ACSET) ? gvid_acs_enable : gvid_acs_disable);
    gvid_last_attr = attr;
  }

  return newattr;
}
#endif


//  ------------------------------------------------------------------
//  OS/2 Vio* wrappers for prevent 16-bit segment overrun

#if defined(__OS2__)

#ifndef _THUNK_PTR_SIZE_OK
#define _THUNK_PTR_SIZE_OK(ptr,size) (((ULONG)(ptr) & ~0xffff) == (((ULONG)(ptr) + (size) - 1) & ~0xffff))
#endif

static USHORT VioReadCellStr_(PCH str, PUSHORT pcb, USHORT row, USHORT col, HVIO hvio) {
  USHORT rc, cb = *pcb;

  if(_THUNK_PTR_SIZE_OK(str, cb))
    return VioReadCellStr(str, pcb, row, col, hvio);
  PCH newstr = (PCH)throw_xmalloc(cb * 2);
  if(_THUNK_PTR_SIZE_OK(newstr, cb)) {
    rc = VioReadCellStr(newstr, pcb, row, col, hvio);
    if(rc == 0)
      memcpy(str, newstr, *pcb);
  }
  else {
    rc = VioReadCellStr(newstr + cb, pcb, row, col, hvio);
    if(rc == 0)
      memcpy(str, newstr + cb, *pcb);
  }
  throw_xfree(newstr);
  return rc;
}


static USHORT VioWrtCellStr_(PCCH str, USHORT cb, USHORT row, USHORT col, HVIO hvio) {
  USHORT rc;

  if(_THUNK_PTR_SIZE_OK(str, cb ))
    return VioWrtCellStr(str, cb, row, col, hvio);
  PCH newstr = (PCH)throw_xmalloc(cb * 2);
  if(_THUNK_PTR_SIZE_OK(newstr, cb)) {
    memcpy(newstr, str, cb);
    rc = VioWrtCellStr(newstr, cb, row, col, hvio);
  }
  else {
    memcpy(newstr + cb, str, cb);
    rc = VioWrtCellStr(newstr + cb, cb, row, col, hvio);
  }
  throw_xfree(newstr);
  return rc;
}


static USHORT VioWrtCharStrAtt_(PCCH str, USHORT cb, USHORT row, USHORT col, PBYTE attr, HVIO hvio) {
  USHORT rc;

  if(_THUNK_PTR_SIZE_OK(str, cb))
    return VioWrtCharStrAtt(str, cb, row, col, attr, hvio);
  PCH newstr = (PCH)throw_xmalloc(cb * 2);
  if(_THUNK_PTR_SIZE_OK(newstr, cb)) {
    memcpy(newstr, str, cb);
    rc = VioWrtCharStrAtt(newstr, cb, row, col, attr, hvio);
  }
  else {
    memcpy(newstr + cb, str, cb);
    rc = VioWrtCharStrAtt(newstr + cb, cb, row, col, attr, hvio);
  }
  throw_xfree(newstr);
  return rc;
}

#define VioReadCellStr         VioReadCellStr_
#define VioWrtCellStr          VioWrtCellStr_
#define VioWrtCharStrAtt       VioWrtCharStrAtt_

#endif

//  ------------------------------------------------------------------
//  ncurses support functions

#else // defined(__USE_NCURSES__)

//  ------------------------------------------------------------------
//  Compute our attributes from DOS attributes

int gvid_attrcalc(int dosattr) {

  // DOS attrs: XRGBxrgb
  // color pair definition: 00RGBrgb, with last 3 bits negated
  int attr;
  attr = COLOR_PAIR(((dosattr & 0x70) >> 1) | ((~dosattr) & 0x07));
  if(dosattr & 0x08)
    attr |= A_BOLD;
//  if(dosattr & 0x80)
//    attr |= A_BLINK;
    
  return attr;
}

//  ------------------------------------------------------------------
//  Compute DOS attributes from our attributes

int gvid_dosattrcalc(int ourattr) {

  int attr = 0;
  attr = PAIR_NUMBER(ourattr);
  attr = ((attr & 0x38) << 1) | ((~attr) & 0x07);
  if(ourattr & A_BLINK)
    attr |= 0x80;
  if(ourattr & A_BOLD)
    attr |= 0x08;
  
  return attr;
}

//  ------------------------------------------------------------------
//  Transform character < 32 into printable equivalent


chtype gvid_tcpr(vchar chr) {

  const chtype gvid_cpr[] = {
    (chtype)' ', (chtype)'@', (chtype)'@', (chtype)'x',
    (chtype) ACS_DIAMOND, (chtype)'x', (chtype)'x', ACS_BULLET,
    ACS_BULLET, ACS_BULLET, ACS_BULLET, (chtype)'x',
    (chtype)'x', (chtype)'x', (chtype)'x', ACS_LANTERN,
    (chtype)ACS_RARROW, ACS_LARROW, (chtype)'x', (chtype)'!',
    (chtype)'x', (chtype)'x', ACS_S1, (chtype)'x',
    ACS_UARROW, ACS_DARROW, ACS_LARROW, (chtype)ACS_RARROW,
    (chtype)'x', (chtype)'x', ACS_UARROW, ACS_DARROW
  };

  chtype ch = chr & A_CHARTEXT;
  chtype at = chr & (~A_CHARTEXT);

  if(ch<' ')
    return gvid_cpr[ch] | at;
  else
    return ch | at;
}


//  ------------------------------------------------------------------

#endif // defined(__USE_NCURSES__)


//  ------------------------------------------------------------------
//  Print character and attribute at specfied location

#if (defined(__MSDOS__) || defined(__UNIX__)) && !defined(__USE_NCURSES__)
inline void _vputw(int row, int col, word chat) {

  _farpokew(_dos_ds, gdmaptr(col, row), chat);
}
#endif


void vputw(int row, int col, vatch chat) {

  #if defined(__USE_NCURSES__)

  mvaddch(row, col, chat);
  refresh();

  #elif defined(__MSDOS__)

  if(gvid->isdma()) {
    _vputw(row, col, chat);
  }
  else if(gvid->isbios() or gvid->iscga()) {
    i86 cpu;
    cpu.ah(2);
    cpu.bh(0);
    cpu.dh((byte)row);
    cpu.dl((byte)col);
    cpu.genint(0x10);
    cpu.ah(9);
    cpu.al(vgchar(chat));
    cpu.bh(0);
    cpu.bl(vgattr(chat));
    cpu.cx(1);
    cpu.genint(0x10);
  }

  #elif defined(__OS2__)

  VioWrtNCell((BYTE *)&chat, 1, (USHORT)row, (USHORT)col, 0);

  #elif defined(__WIN32__)

  const COORD coord = {0, 0};
  const COORD size = {1, 1};
  SMALL_RECT rect;

  rect.Top = row;
  rect.Left = col;
  rect.Bottom = row+size.Y-1;
  rect.Right = col+size.X-1;
  if(WinVer.dwPlatformId == VER_PLATFORM_WIN32_NT) {
    chat.Char.UnicodeChar = gvid_tcpr(vgchar(chat));
    WriteConsoleOutputW(gvid_hout, &chat, size, coord, &rect);
  }
  else
    WriteConsoleOutputA(gvid_hout, &chat, size, coord, &rect);

  #elif defined(__UNIX__)

  char chr = vgchar(chat);
  int atr = vgattr(chat);
  char* color = gvid_newattr(atr);

  gvid_cvtstr(&chat, 1);
  _vputw(row, col, chat);

  gvid_printf("\033[%u;%uH%s%c", row+1, col+1, color, chr);

  #endif
}


//  ------------------------------------------------------------------
//  Print attrib/char buffer at specfied location

void vputws(int row, int col, vatch* buf, uint len) {

  #if defined(__USE_NCURSES__)

  move(row, col);
  for(int counter = 0; counter < len; counter++)
    addch(buf[counter]);
  refresh();

  #elif defined(__MSDOS__)

  if(gvid->isdma()) {
    gdmacpy(_dos_ds, (gdma)gdmaptr(col, row), _my_ds(), (gdma)buf, len*sizeof(word));
  }
  else if(gvid->isbios() or gvid->iscga()) {
    i86 cpu;
    byte* p = (byte*)buf;
    for(uint n=0; n<len; n++) {
      cpu.ah(2);
      cpu.bh(0);
      cpu.dh((byte)row);
      cpu.dl((byte)col++);
      cpu.genint(0x10);
      cpu.ah(9);
      cpu.al(*p++);
      cpu.bh(0);
      cpu.bl(*p++);
      cpu.cx(1);
      cpu.genint(0x10);
    }
  }

  #elif defined(__OS2__)

  VioWrtCellStr((PCCH)buf, (USHORT)(len*2), (USHORT)row, (USHORT)col, 0);

  #elif defined(__WIN32__)

  const COORD coord = {0, 0};
  COORD size = {len, 1};
  SMALL_RECT rect;

  rect.Top = row;
  rect.Left = col;
  rect.Bottom = row+size.Y-1;
  rect.Right = col+size.X-1;
  if(WinVer.dwPlatformId == VER_PLATFORM_WIN32_NT) {
    for(int i = 0; i < len; i++) {
      buf[i].Char.UnicodeChar = gvid_tcpr(vgchar(buf[i]));
    }
    WriteConsoleOutputW(gvid_hout, buf, size, coord, &rect);
  }
  else
    WriteConsoleOutputA(gvid_hout, buf, size, coord, &rect);

  #elif defined(__UNIX__)

  gvid_cvtstr(buf, len);
  gdmacpy(_dos_ds, (gdma)gdmaptr(col, row), _my_ds(), (gdma)buf, len*sizeof(word));
  vputansi(row, col, buf, len);

  #endif
}


//  ------------------------------------------------------------------
//  Print character and attribute at specfied location

void vputc(int row, int col, int atr, vchar chr) {

  #if defined(__USE_NCURSES__)

  mvaddch(row, col, vcatch(gvid_tcpr(chr), atr));
  refresh();

  #elif defined(__MSDOS__)

  if(gvid->isdma()) {
    _vputw(row, col, vcatch(chr, atr));
  }
  else if(gvid->isbios() or gvid->iscga()) {
    i86 cpu;
    cpu.ah(2);
    cpu.bh(0);
    cpu.dh((byte)row);
    cpu.dl((byte)col);
    cpu.genint(0x10);
    cpu.ah(9);
    cpu.al(chr);
    cpu.bh(0);
    cpu.bl((byte)atr);
    cpu.cx(1);
    cpu.genint(0x10);
  }

  #elif defined(__OS2__) || defined(__WIN32__)

  vputw(row, col, vcatch(chr, atr));

  #elif defined(__UNIX__)

  char* color = gvid_newattr(atr);
  gvid_cvtstr(&chr, 1);
  _vputw(row, col, vcatch(chr, atr));

  gvid_printf("\033[%u;%uH%s%c", row+1, col+1, color, chr);

  #endif
}


//  ------------------------------------------------------------------
//  Print string with attribute at specfied location

void vputvs(int row, int col, int atr, const vchar* str) {

  #if defined(__USE_NCURSES__)

  uint counter;
  int attr = gvid_attrcalc(atr);
  move(row, col);
  for(counter = 0; str[counter] != 0; counter++)
    addch(gvid_tcpr(str[counter]) | attr);
  refresh();

  #else

  vputs(row, col, atr, str);

  #endif
}


//  ------------------------------------------------------------------
//  Print string with attribute at specfied location

void vputs_box(int row, int col, int atr, const char* str) {
#if defined(__USE_NCURSES__)
  uint counter;
  int len = strlen(str);
  int attr = gvid_attrcalc(atr);
  move(row, col);
  for(counter = 0; counter < len; counter++)
    addch(gvid_tcpr(gvid_boxcvtc(str[counter])) | attr);
  refresh();
#else
  vputs(row, col, atr, str);
#endif
}

void vputs(int row, int col, int atr, const char* str) {

  #if defined(__USE_NCURSES__)

  uint counter;
  int len = strlen(str);
  int attr = gvid_attrcalc(atr);
  move(row, col);
  for(counter = 0; counter < len; counter++)
    addch(gvid_tcpr(str[counter]) | attr);
  refresh();
  
  #elif defined(__MSDOS__)

  if(gvid->isdma()) {
    gdma p = gdmaptr(col, row);
    _farsetsel(_dos_ds);
    while(*str) {
      _farnspokew(p, vcatch(*str++, atr));
      p += ATTRSIZE;
    }
  }
  else if(gvid->isbios() or gvid->iscga()) {
    i86 cpu;
    for(const char* q=str; *q; q++) {
      // Write as fast as possible on XT bios...
      cpu.ah(2);
      cpu.bh(0);
      cpu.dh((byte)row);
      cpu.dl((byte)(col++));
      cpu.genint(0x10);
      cpu.ah(9);
      cpu.al(*q);
      cpu.bh(0);
      cpu.bl((byte)atr);
      cpu.cx(1);
      cpu.genint(0x10);
    }
  }

  #elif defined(__OS2__)

  VioWrtCharStrAtt((PCCH)str, (USHORT)strlen(str), (USHORT)row, (USHORT)col, (PBYTE)&atr, 0);

  #elif defined(__WIN32__)

  int i;

  for(i = 0; *str; i++)
    gvid->bufwrd[i] = vcatch(*str++, atr);
  if(i)
    vputws(row, col, gvid->bufwrd, i);

  #elif defined(__UNIX__)

  char buf[1024];
  strcpy(buf, str);
  char* color = gvid_newattr(atr);
  gvid_cvtstr(buf, strlen(buf));
  gdma p = gdmaptr(col, row);
  _farsetsel(_dos_ds);
  while(*str) {
    _farnspokew(p, vcatch(*str++, atr));
    p += ATTRSIZE;
  }

  gvid_printf("\033[%u;%uH%s%s", row+1, col+1, color, buf);

  #endif
}


//  ------------------------------------------------------------------
//  Print string with attribute at specfied location

#if (defined(__MSDOS__) || defined(__UNIX__)) && !defined(__USE_NCURSES__)
static void _vputns(int row, int col, int atr, const char* str, uint width) {

  char fillchar = ' ';

  gdma p = gdmaptr(col, row);
  _farsetsel(_dos_ds);
  while(width--) {
    _farnspokew(p, (atr << 8) | (*str ? *str++ : fillchar));
    p += ATTRSIZE;
  }
}
#endif


//  ------------------------------------------------------------------
//  Print string with attribute at specfied location

void vputns(int row, int col, int atr, const char* str, uint width) {

  char fillchar = ' ';

  #if defined(__USE_NCURSES__)

  uint counter;
  int len = strlen(str);
  int attr = gvid_attrcalc(atr);
  move(row, col);
  for(counter = 0; counter < width; counter++) {
    if(counter<len)
      addch(gvid_tcpr(str[counter]) | attr);
    else
      addch(gvid_tcpr(fillchar) | attr);
  }
  refresh();
  
  #elif defined(__MSDOS__)

  if(gvid->isdma()) {
    _vputns(row, col, atr, str, width);
  }
  else if(gvid->isbios() or gvid->iscga()) {
    i86 cpu;
    while(width--) {
      // Write as fast as possible on XT bios...
      cpu.ah(2);
      cpu.bh(0);
      cpu.dh((byte)row);
      cpu.dl((byte)(col++));
      cpu.genint(0x10);
      cpu.ah(9);
      cpu.al(*str ? *str++ : fillchar);
      cpu.bh(0);
      cpu.bl((byte)atr);
      cpu.cx(1);
      cpu.genint(0x10);
    }
  }

  #elif defined(__OS2__)

  uint len = strlen(str);

  VioWrtCharStrAtt((PCCH)str, (USHORT)minimum_of_two(len,width), (USHORT)row, (USHORT)col, (PBYTE)&atr, 0);

  if(width > len) {
    vatch filler = vcatch(fillchar, atr);
    VioWrtNCell((BYTE *)&filler, (USHORT)(width-len), (USHORT)row, (USHORT)(col+len), 0);
  }

  #elif defined(__WIN32__)

  int i;

  for(i = 0; (i < width) and *str; i++)
    gvid->bufwrd[i] = vcatch(*str++, atr);
  vatch filler = vcatch(fillchar, atr);
  for(; i < width; i++)
    gvid->bufwrd[i] = filler;
  vputws(row, col, gvid->bufwrd, width);

  #elif defined(__UNIX__)

  char* color = gvid_newattr(atr);

  uint len = strlen(str);
  uint min_len = minimum_of_two(len, width);
  char buf[1024];
  strcpy(buf, str);
  gvid_cvtstr(buf, len);

  _vputns(row, col, atr, buf, width);

  char fillbuf[256];
  if(width > len) {
    memset(fillbuf, fillchar, width-len);
    fillbuf[width-len] = NUL;
  }
  else {
    *fillbuf = NUL;
  }

  gvid_printf("\033[%u;%uH%s%*.*s%s", row+1, col+1, color,
    min_len, min_len, buf, fillbuf
  );

  #endif
}


//  ------------------------------------------------------------------
//  Print horizontal line of character and attribute

#if (defined(__MSDOS__) || defined(__UNIX__)) && !defined(__USE_NCURSES__)
void _vputx(int row, int col, int atr, char chr, uint len) {

  gdma p = gdmaptr(col, row);
  word tmp = vcatch(chr, atr);
  _farsetsel(_dos_ds);
  for(uint n=0; n<len; n++) {
    _farnspokew(p, tmp);
    p += ATTRSIZE;
  }
}
#endif


//  ------------------------------------------------------------------
//  Print horizontal line of character and attribute

void vputx(int row, int col, int atr, vchar chr, uint len) {

  #if defined(__USE_NCURSES__)

  mvhline(row, col, vcatch(gvid_tcpr(chr), atr), len);
  refresh();
  
  #elif defined(__MSDOS__)

  if(gvid->isdma()) {
    _vputx(row, col, atr, chr, len);
  }
  else if(gvid->isbios() or gvid->iscga()) {
    i86 cpu;
    cpu.ah(2);
    cpu.bh(0);
    cpu.dh((byte)row);
    cpu.dl((byte)col);
    cpu.genint(0x10);
    cpu.ah(9);
    cpu.al(chr);
    cpu.bh(0);
    cpu.bl((byte)atr);
    cpu.cx((word)len);
    cpu.genint(0x10);
  }

  #elif defined(__OS2__)

  vatch filler = vcatch(chr, atr);
  VioWrtNCell((BYTE *)&filler, (USHORT)len, (USHORT)row, (USHORT)col, 0);

  #elif defined(__WIN32__)

  vatch filler = vcatch(chr, atr);
  for(int i = 0; i < len; i++)
    gvid->bufwrd[i] = filler;
  vputws(row, col, gvid->bufwrd, len);

  #elif defined(__UNIX__)

  char buf[256];
  char* color = gvid_newattr(atr);
  gvid_cvtchr(chr);
  _vputx(row, col, atr, chr, len);
  memset(buf, chr, len);
  buf[len] = NUL;
  gvid_printf("\033[%u;%uH%s%s", row+1, col+1, color, buf);

  #endif
}


//  ------------------------------------------------------------------
//  Print vertical line of character and attribute

#if (defined(__MSDOS__) || defined(__UNIX__)) && !defined(__USE_NCURSES__)
inline void _vputy(int row, int col, int atr, char chr, uint len) {

  gdma p = gdmaptr(col, row);
  word tmp = vcatch(chr, atr);
  _farsetsel(_dos_ds);
  for(uint n=0; n<len; n++) {
    _farnspokew(p, tmp);
    p += ATTRSIZE*gvid->numcols;
  }
}
#endif


//  ------------------------------------------------------------------
//  Print vertical line of character and attribute

void vputy(int row, int col, int atr, vchar chr, uint len) {

  #if defined(__USE_NCURSES__)

  mvvline(row, col, vcatch(gvid_tcpr(chr), atr), len);
  refresh();

  #elif defined(__MSDOS__)

  if(gvid->isdma()) {
    _vputy(row, col, atr, chr, len);
  }
  else if(gvid->isbios() or gvid->iscga()) {
    for(uint n=0; n<len; n++) {
      i86 cpu;
      cpu.ah(2);
      cpu.bh(0);
      cpu.dh((byte)row++);
      cpu.dl((byte)col);
      cpu.genint(0x10);
      cpu.ah(9);
      cpu.al(chr);
      cpu.bh(0);
      cpu.bl((byte)atr);
      cpu.cx(1);
      cpu.genint(0x10);
    }
  }

  #elif defined(__OS2__)

  vatch filler = vcatch(chr, atr);
  for(int n=0; n<len; n++)
    VioWrtNCell((BYTE *)&filler, 1, (USHORT)row++, (USHORT)col, 0);

  #elif defined(__WIN32__)

  vatch filler = vcatch(chr, atr);
  for(int i=0; i < len; i++)
    vputw(row++, col, filler);

  #elif defined(__UNIX__)

  char* color = gvid_newattr(atr);
  gvid_cvtchr(chr);
  _vputy(row, col, atr, chr, len);

  char buf[2048];
  sprintf(buf, "\033[%u;%uH%s", row+1, col+1, color);

  char* p = buf + strlen(buf);
  for(uint n=0; n<(len-1); n++) {
    *p++ = chr;
    if(col == gvid->numcols-1) {
      sprintf(p, "\033[%u;%uH", row+n+2, col+1);
      p += strlen(p);
    }
    else {
      strcpy(p, "\033[D\033[B");
      p += 6;
    }
  }
  *p++ = chr;
  *p = NUL;
  gvid_printf("%s", buf);

  #endif
}


//  ------------------------------------------------------------------
//  Get character and attribute at cursor position

#if (defined(__MSDOS__) || defined(__UNIX__)) && !defined(__USE_NCURSES__)
inline word _vgetw(int row, int col) {

  return _farpeekw(_dos_ds, gdmaptr(col, row));
}
#endif


//  ------------------------------------------------------------------
//  Get character and attribute at cursor position

vatch vgetw(int row, int col) {

  #if defined(__USE_NCURSES__)

  return mvinch(row, col);
  
  #elif defined(__MSDOS__)

  if(gvid->isdma()) {
    return _vgetw(row, col);
  }
  else if(gvid->isbios() or gvid->iscga()) {
    i86 cpu;
    cpu.ah(2);
    cpu.bh(0);
    cpu.dh((byte)row);
    cpu.dl((byte)col);
    cpu.genint(0x10);
    cpu.ah(8);
    cpu.bh(0);
    cpu.genint(0x10);
    return cpu.ax();
  }
  return 0;

  #elif defined(__OS2__)

  vatch chat;
  USHORT len=sizeof(chat);

  VioReadCellStr((BYTE *)&chat, &len, (USHORT)row, (USHORT)col, 0);

  return chat;

  #elif defined(__WIN32__)

  vatch chat;
  const COORD coord = {0, 0};
  const COORD size = {1, 1};
  SMALL_RECT rect;

  rect.Top = row;
  rect.Left = col;
  rect.Bottom = row+size.Y-1;
  rect.Right = col+size.X-1;
  ReadConsoleOutput(gvid_hout, &chat, size, coord, &rect);

  return chat;

  #elif defined(__UNIX__)

  return _vgetw(row, col);

  #endif
}


//  ------------------------------------------------------------------
//  Get character and attribute at cursor position

void vgetc(int row, int col, int* atr, vchar* chr) {

  if((row < 0) or (row > gvid->numrows-1) or (col < 0) or (col > gvid->numcols-1)) {
    *chr = ' ';
    *atr = 0;
  }
  else {
    vatch tmp = vgetw(row, col);

    *chr = vgchar(tmp);
    *atr = vgattr(tmp);
  }
}


//  ------------------------------------------------------------------
//  Scroll screen area

#if (defined(__MSDOS__) || defined(__UNIX__)) && !defined(__USE_NCURSES__)
static void _vscroll(int srow, int scol, int erow, int ecol, int atr, int lines) {

  word empty = (atr << 8) | ' ';
  if(lines > 0) {
    while(lines--) {
      int nrow = srow;
      int l = ((ecol - scol) + 1);
      gdma scrptr = gdmaptr(scol, srow);
      while(nrow++ < erow) {
        gdmacpy(_dos_ds, (gdma)scrptr, _dos_ds, (gdma)(scrptr+ATTRSIZE*gvid->numcols), l*sizeof(word));
        scrptr += ATTRSIZE*gvid->numcols;
      }
      _farsetsel(_dos_ds);
      for(l *= ATTRSIZE; l>0;) {
        l -= ATTRSIZE;
        _farnspokew(scrptr+l, empty);
      }
    }
  }
  else {
    while(lines++) {
      int nrow = erow;
      int l = ((ecol - scol) + 1);
      gdma scrptr = gdmaptr(scol, erow);
      while(nrow-- >= (srow + 1)) {
        gdmacpy(_dos_ds, (gdma)scrptr, _dos_ds, (gdma)(scrptr-ATTRSIZE*gvid->numcols), l*sizeof(word));
        scrptr -= ATTRSIZE*gvid->numcols;
      }
      _farsetsel(_dos_ds);
      for(l *= ATTRSIZE; l>0;) {
        l -= ATTRSIZE;
        _farnspokew(scrptr+l, empty);
      }
    }
  }
}
#endif


//  ------------------------------------------------------------------
//  Scroll screen area

void vscroll(int srow, int scol, int erow, int ecol, int atr, int lines) {

  #if defined(__USE_NCURSES__)

  vatch filler = vcatch(' ', atr);

  // Currently implemented with vsave/vrestore
  // Does anyone know a better solution?

  if(lines >= 0) {
    if(lines <= 1 + erow - srow) {
      vsavebuf *buf = vsave(srow + lines, scol, erow, ecol);
      vrestore(buf, srow, scol, erow - lines, ecol);
      throw_xfree(buf);
    }
    else
      lines = 1 + erow - srow;
      
    for(int counter = 0; counter < lines; counter++)
      mvhline(1 + erow + counter - lines, scol, filler, 1 + ecol - scol);
    refresh();
  }
  else {
    lines*=-1;
    if(lines <= 1 + erow - srow) {
      vsavebuf *buf = vsave(srow, scol, erow - lines, ecol);
      vrestore(buf, srow + lines, scol, erow, ecol);
      throw_xfree(buf);
    }
    else
      lines = 1 + erow - srow;
      
    for(int counter = 0; counter < lines; counter++)
      mvhline(srow + counter, scol, filler, 1 + ecol - scol);
    refresh();
  }

  #elif defined(__MSDOS__)

  if(gvid->isdma()) {
    _vscroll(srow, scol, erow, ecol, atr, lines);
  }
  else if(gvid->isbios() or gvid->iscga()) {
    i86 cpu;
    cpu.ah((byte)(lines > 0 ? 6 : 7));
    cpu.al((byte)absolute(lines));
    cpu.bh((byte)atr);
    cpu.ch((byte)srow);
    cpu.cl((byte)scol);
    cpu.dh((byte)erow);
    cpu.dl((byte)ecol);
    cpu.genint(0x10);
  }

  #elif defined(__OS2__)

  vatch filler = vcatch(' ', atr);

  if(lines > 0)
    VioScrollUp((USHORT)srow, (USHORT)scol, (USHORT)erow, (USHORT)ecol, (USHORT)lines, (BYTE *)&filler, 0);
  else
    VioScrollDn((USHORT)srow, (USHORT)scol, (USHORT)erow, (USHORT)ecol, (USHORT)-lines, (BYTE *)&filler, 0);

  #elif defined(__WIN32__)

  SMALL_RECT r;
  COORD c = {scol, srow - lines};
  vatch filler = vcatch(' ', atr);

  r.Left   = (SHORT)scol;
  r.Top    = (SHORT)srow;
  r.Right  = (SHORT)ecol;
  r.Bottom = (SHORT)erow;

  ScrollConsoleScreenBuffer(gvid_hout, &r, &r, c, &filler);

  #elif defined(__UNIX__)

  _vscroll(srow, scol, erow, ecol, atr, lines);

  gdma ptr = gdmaptr(scol, srow);
  int len = ecol-scol+1;
  for(int nrow=srow; nrow<=erow; nrow++) {
    vputansi(nrow, scol, ptr, len);
    ptr += ATTRSIZE*gvid->numcols;
  }

  #endif
}


//  ------------------------------------------------------------------
//  Returns true if cursor invisible

bool vcurhidden() {

  return __vcurhidden;
}

//  ------------------------------------------------------------------
//  Get cursor position

void vposget(int* row, int* col) {

  #if defined(__USE_NCURSES__)

  getyx(stdscr, gvid->currow, gvid->curcol);
  
  #elif defined(__MSDOS__)

  i86 cpu;
  cpu.ah(3);
  cpu.bh(0);
  cpu.genint(0x10);
  gvid->currow = cpu.dh();
  gvid->curcol = cpu.dl();

  #elif defined(__OS2__)

  USHORT _getrow, _getcol;
  VioGetCurPos(&_getrow, &_getcol, 0);
  gvid->currow = _getrow;
  gvid->curcol = _getcol;

  #elif defined(__WIN32__)

  CONSOLE_SCREEN_BUFFER_INFO i;
  GetConsoleScreenBufferInfo(gvid_hout, &i);
  gvid->currow = i.dwCursorPosition.Y;
  gvid->curcol = i.dwCursorPosition.X;

  #elif defined(__UNIX__)

  // Not available

  #endif

  *row = gvid->currow;
  *col = gvid->curcol;
}


//  ------------------------------------------------------------------
//  Set cursor position

void vposset(int row, int col) {

  gvid->currow = row;
  gvid->curcol = col;

  #if defined(__USE_NCURSES__)

  move(row, col);
  refresh();
  
  #elif defined(__MSDOS__)

  i86 cpu;
  cpu.ah(2);
  cpu.bh(0);
  cpu.dh((byte)row);
  cpu.dl((byte)col);
  cpu.genint(0x10);

  #elif defined(__OS2__)

  VioSetCurPos((USHORT)row, (USHORT)col, 0);

  #elif defined(__WIN32__)

  // No need to set the cursor position if its not visible
  // Strangely, this is a major speedup to screen-output

  if(__vcurhidden)
    return;

  COORD c = {col, row};
  SetConsoleCursorPosition(gvid_hout, c);

  #elif defined(__UNIX__)

  gvid_printf("\x1B[%u;%uH", row+1, col+1);

  #endif
}


//  ------------------------------------------------------------------
//  Clears the screen and homes the cursor

void vclrscr() {

  vclrscr(vgattr(vgetw(gvid->currow, gvid->curcol)));
}


//  ------------------------------------------------------------------
//  Clears the screen using given attribute and homes the cursor

#if (defined(__MSDOS__) || defined(__UNIX__)) && !defined(__USE_NCURSES__)
static void _vclrscr(int atr) {

  int len = gvid->numrows * gvid->numcols;

  _vputx(0, 0, atr, ' ', len);
}
#endif



//  ------------------------------------------------------------------
//  Clears the screen using given attribute and homes the cursor

void vclrscr(int atr) {

  #if defined(__USE_NCURSES__)

  clearok(stdscr, TRUE);
  vatch filler = vcatch(' ', atr);
  for(int row = 0; row < LINES; row++)
    mvhline(row, 0, filler, COLS);
  move(0, 0);
  refresh();
  
  #elif defined(__MSDOS__)

  if(gvid->isdma()) {
    _vclrscr(atr);
  }
  else if(gvid->isbios() or gvid->iscga()) {
    i86 cpu;
    cpu.ax(0x0600);           // clear screen by scrolling it
    cpu.bh((byte)atr);
    cpu.cx(0);
    cpu.dh((byte)(gvid->numrows - 1));
    cpu.dl((byte)(gvid->numcols - 1));
    cpu.genint(0x10);
  }

  #elif defined(__OS2__)

  vatch filler = vcatch(' ', atr);
  VioScrollUp(0, 0, 0xFFFF, 0xFFFF, 0xFFFF, (BYTE *)&filler, 0);

  #elif defined(__WIN32__)

  COORD c = {0, 0};
  DWORD wr, len = gvid->numrows * gvid->numcols;
  // Filling with space seems to work for both Unicode and regular functions
  FillConsoleOutputCharacter(gvid_hout,       ' ', len, c, &wr);
  FillConsoleOutputAttribute(gvid_hout, (WORD)atr, len, c, &wr);

  #elif defined(__UNIX__)

  _vclrscr(atr);

  gvid_printf("%s\x1B[2J", gvid_newattr(atr));

  #endif

  vposset(0,0);
}


//  ------------------------------------------------------------------
//  Saves the current screen and returns pointer to buffer

#if (defined(__MSDOS__) || defined(__UNIX__)) && !defined(__USE_NCURSES__)
static void _vsave(word* buf, int len1, int srow, int scol, int erow) {

  const int len2 = len1*sizeof(word);
  gdma p = gdmaptr(scol, srow);
  for(int nrow=srow; nrow<=erow; nrow++) {
    gdmacpy(_my_ds(), (gdma)buf, _dos_ds, (gdma)p, len2);
    p += ATTRSIZE*gvid->numcols;
    buf += len1;
  }
}
#endif


//  ------------------------------------------------------------------
//  Saves the current screen and returns pointer to buffer

vsavebuf* vsave(int srow, int scol, int erow, int ecol) {

  if(srow == -1)  srow = 0;
  if(scol == -1)  scol = 0;
  if(erow == -1)  erow = gvid->numrows-1;
  if(ecol == -1)  ecol = gvid->numcols-1;

  vsavebuf* sbuf = (vsavebuf*)throw_xmalloc(sizeof(vsavebuf) + (erow - srow + 1) * (ecol - scol + 1) * sizeof(vatch));

  if(sbuf) {

    vatch* buf = sbuf->data;

    sbuf->top = srow;
    sbuf->left = scol;
    sbuf->bottom = erow;
    sbuf->right = ecol;

    #if defined(__USE_NCURSES__)
    
    for(int row=srow; row<=erow; row++)
      for(int col=scol; col<=ecol; col++)
        *buf++ = mvinch(row, col);

    #elif defined(__MSDOS__)

    int len1 = ecol-scol+1;

    if(gvid->isdma()) {
      _vsave(buf, len1, srow, scol, erow);
    }
    else if(gvid->isbios() or gvid->iscga()) {
      i86 cpu;
      byte* p = (byte*)buf;
      for(byte row=(byte)srow; row<=erow; row++) {
        for(byte col=(byte)scol; col<=ecol; col++) {
          cpu.ah(2);
          cpu.bh(0);
          cpu.dh(row);
          cpu.dl(col);
          cpu.genint(0x10);
          cpu.ah(8);
          cpu.bh(0);
          cpu.genint(0x10);
          *p++ = cpu.al();
          *p++ = cpu.ah();
        }
      }
    }

    #elif defined(__OS2__)

    int len1 = (int)(ecol-scol+1);

    #if defined(__BORLANDC__)
    PCHAR16 ptr = (PCHAR16)buf;
    #else
    PCH ptr = (PCH)buf;
    #endif

    USHORT len2 = (USHORT)(len1*sizeof(word));
    for(int nrow=srow; nrow<=erow; nrow++) {
      VioReadCellStr(ptr, &len2, nrow, scol, 0);
      ptr += len2;
    }

    #elif defined(__WIN32__)

    const COORD coord = {0, 0};
    COORD size = {ecol-scol+1, erow-srow+1};
    SMALL_RECT r;

    // Set the source rectangle.
    r.Top = srow;
    r.Left = scol;
    r.Bottom = erow;
    r.Right = ecol;

    if(WinVer.dwPlatformId == VER_PLATFORM_WIN32_NT) 
      ReadConsoleOutputW(gvid_hout, buf, size, coord, &r);
    else
      ReadConsoleOutputA(gvid_hout, buf, size, coord, &r);

    #elif defined(__UNIX__)

    int len1 = ecol-scol+1;

    _vsave(buf, len1, srow, scol, erow);

    #endif
  }

  return sbuf;
}


//  ------------------------------------------------------------------
//  Redraws a previously saved screen

#if (defined(__MSDOS__) || defined(__UNIX__)) && !defined(__USE_NCURSES__)
static void _vredraw(word* buf, int len1, int srow, int scol, int erow) {

  const int len2 = len1*sizeof(word);
  gdma p = gdmaptr(scol, srow);
  for(int nrow=srow; nrow<=erow; nrow++) {
    gdmacpy(_dos_ds, (gdma)p, _my_ds(), (gdma)buf, len2);
    p += ATTRSIZE*gvid->numcols;
    buf += len1;
  }
}
#endif


//  ------------------------------------------------------------------
//  Redraws a previously saved screen

void vrestore(vsavebuf* sbuf, int srow, int scol, int erow, int ecol) {

  if(srow != -1)  sbuf->top = srow;
  if(scol != -1)  sbuf->left = scol;
  if(erow != -1)  sbuf->bottom = erow;
  if(ecol != -1)  sbuf->right = ecol;

  srow = sbuf->top;
  scol = sbuf->left;
  erow = sbuf->bottom;
  ecol = sbuf->right;

  vatch *buf = sbuf->data;
  
  #if defined(__USE_NCURSES__)
  
  for(int row=srow; row<=erow; row++)
    for(int col=scol; col<=ecol; col++)
      mvaddch(row, col, *buf++);

  refresh();

  #elif defined(__MSDOS__)

  int len1 = ecol-scol+1;

  if(gvid->isdma()) {
    _vredraw(buf, len1, srow, scol, erow);
  }
  else if(gvid->isbios() or gvid->iscga()) {
    i86 cpu;
    byte* p = (byte*)buf;
    for(byte row=(byte)srow; row<=erow; row++) {
      for(byte col=(byte)scol; col<=ecol; col++) {
        cpu.ah(2);
        cpu.bh(0);
        cpu.dh(row);
        cpu.dl(col);
        cpu.genint(0x10);
        cpu.ah(9);
        cpu.al(*p++);
        cpu.bh(0);
        cpu.bl(*p++);
        cpu.cx(1);
        cpu.genint(0x10);
      }
    }
  }

  #elif defined(__OS2__)

  USHORT len1 = (USHORT)(ecol-scol+1);
  USHORT len2 = (USHORT)(len1*sizeof(word));

  #if defined(__BORLANDC__)
  PCHAR16 ptr = (PCHAR16)buf;
  #else
  PCH ptr = (PCH)buf;
  #endif

  for(USHORT nrow=srow; nrow<=erow; nrow++) {
    VioWrtCellStr(ptr, len2, nrow, scol, 0);
    ptr += len2;
  }

  #elif defined(__WIN32__)

  const COORD coord = {0, 0};
  COORD size = {ecol-scol+1, erow-srow+1};
  SMALL_RECT r;

  // Set the source rectangle.
  r.Top = srow;
  r.Left = scol;
  r.Bottom = erow;
  r.Right = ecol;

  if(WinVer.dwPlatformId == VER_PLATFORM_WIN32_NT) 
    WriteConsoleOutputW(gvid_hout, buf, size, coord, &r);
  else
    WriteConsoleOutputA(gvid_hout, buf, size, coord, &r);

  #elif defined(__UNIX__)

  int len1 = ecol-scol+1;

  _vredraw(buf, len1, srow, scol, erow);

  int atr = vgattr(*buf);
  char* color = gvid_newattr(atr);
  gvid_printf("%s", color);

  for(int nrow=srow; nrow<=erow; nrow++) {
    vputansi(nrow, scol, buf, len1);
    buf += len1;
  }

  #endif
}


//  ------------------------------------------------------------------
//  Sets the cursor shape/size

void vcurset(int sline, int eline) {

  if(eline) {
    gvid->curr.cursor.start = sline;
    gvid->curr.cursor.end = eline;
    __vcurhidden = false;
  }

  #if defined(__USE_NCURSES__)

  if((sline == 0) and (eline == 0))
    curs_set(0);
  else if((eline - sline) <= 4)
    curs_set(1);
  else
    curs_set(2);

  #elif defined(__MSDOS__)

  if(eline == 0) {
    int _dvhide = __gdvdetected ? 0x01 : 0x30;
    sline = ((gvid->adapter>=V_HGC) and (gvid->adapter<=V_INCOLOR)) ? 0x3F : _dvhide;
  }

  i86 cpu;
  cpu.ah(1);
  cpu.ch((byte)sline);
  cpu.cl((byte)eline);
  cpu.genint(0x10);

  #elif defined(__OS2__)

  VIOCURSORINFO vioci;
  VioGetCurType(&vioci, 0);
  vioci.yStart = (USHORT)sline;
  vioci.cEnd   = (USHORT)eline;
  vioci.attr   = (USHORT)((eline == 0) ? 0xFFFF : gvid->curr.color.textattr);
  VioSetCurType(&vioci, 0);

  #elif defined(__WIN32__)

  CONSOLE_CURSOR_INFO cci;

  if(eline)
    vposset(gvid->currow, gvid->curcol);
  else  /* Move cursor to bottom right corner (workaround of the win9x console bug) */
    vposset(gvid->numrows-1, gvid->numcols-1);

  cci.dwSize = (eline and sline) ? sline : 100;
  cci.bVisible = eline ? true : false;

  SetConsoleCursorInfo(gvid_hout, &cci);

  #elif defined(__UNIX__)

  gvid_printf("\033[?25%c", eline ? 'h' : 'l');

  #endif
}


//  ------------------------------------------------------------------
//  Hides the cursor

void vcurhide() {

  if(not __vcurhidden) {
    #if defined(__USE_NCURSES__)
    curs_set(0);
    #else
    vcurset(0,0);
    #endif
    __vcurhidden = true;
  }
}


//  ------------------------------------------------------------------
//  Reveals the cursor

void vcurshow() {

  if(__vcurhidden) {
    vcurset(gvid->curr.cursor.start, gvid->curr.cursor.end);
    __vcurhidden = false;
  }
}


//  ------------------------------------------------------------------
//  Sets a large cursor

void vcurlarge() {

  #if defined(__USE_NCURSES__)
  curs_set(2);
  #else
  vcurshow();

  #if defined(__MSDOS__)

  switch(gvid->adapter) {
    case V_CGA:
      vcurset(1,7);
      break;
    case V_EGA:
      if(gvid->numrows == 25) {
        vcurset(1,7);
      }
      else {
        word* p = (word*)0x0463;  // video BIOS data area
        outpw(*p,0x000A);         // update cursor start register
        outpw(*p,0x0A0B);         // update cursor end register
      }
      break;
    case V_VGA:
      vcurset(1,7);
      break;
    default:    // one of the monochrome cards
      vcurset(1,12);
  }

  #elif defined(__OS2__)

  vcurset(1, gvid->curr.screen.cheight-1);

  #elif defined(__WIN32__)

  vcurset(90, true);

  #endif
  #endif
}


//  ------------------------------------------------------------------
//  Sets a small cursor

void vcursmall() {

  #if defined(__USE_NCURSES__)
  curs_set(1);
  #else
  vcurshow();

  #if defined(__MSDOS__)

  switch(gvid->adapter) {
    case V_CGA:
      vcurset(6,7);
      break;
    case V_EGA:
      if(gvid->numrows == 25) {
        vcurset(6,7);
      }
      else {
        word* p = (word*)0x0463;    // video BIOS data area
        outpw(*p,0x060A);           // update cursor start register
        outpw(*p,0x000B);           // update cursor end register
      }
      break;
    case V_VGA:
      vcurset(6,7);
      break;
    default:    // one of the monochrome cards
      vcurset(11,12);
  }

  #elif defined(__OS2__)

  vcurset(gvid->curr.screen.cheight-2, gvid->curr.screen.cheight-1);

  #elif defined(__WIN32__)

  vcurset(13, true);

  #endif
  #endif
}


//  ------------------------------------------------------------------
//  Table of characters used to display boxes
//
//  Access box table characters via:
//      _box_table(boxtype, x)
//
//  where:
//      boxtype is the number of the box type you want to use (0 - 5)
//
//      x will be one of the following:
//           0 - upper left corner
//           1 - upper horizontal line
//           2 - upper right corner
//           3 - left vertical line
//           4 - right vertical line
//           5 - lower left corner
//           6 - lower horizontal line
//           7 - lower right corner
//           8 - middle junction
//           9 - left vertical junction
//          10 - right vertical junction
//          11 - upper horizontal junction
//          12 - lower horizontal junction
//          13 - checkerboard
//          14 - solid block
//  ------------------------------------------------------------------

#if !defined(__USE_NCURSES__)

char* __box_table[] = {

  #if defined(__UNIX__) // This table will be actually patched at startup...

  ".-.||`-'+||-- #",    // box type 0     Single border
  ".-.||`-'+||-- #",    // box type 1     Double border
  ".-.||`-'+||-- #",    // box type 2     Single top
  ".-.||`-'+||-- #",    // box type 3     Double top
  "              #",    // box type 4     With empty border
  ".-.||`-'+||-- #",    // box type 5     No border at all
  ".-.||`-'+||-- #",    // box type 6     Blocky border
  ".-.||`-'+||-- #",    // box type 7     ASCII border
  "lqkxxmqjntuwvaa",    // box type 8     xterm single border

  #else

  "ÚÄ¿³³ÀÄÙÅÃ´ÂÁ°±",    // box type 0     Single border
  "ÉÍ»ººÈÍ¼ÎÌ¹ËÊ°±",    // box type 1     Double border
  "ÖÄ·ººÓÄ½×Ç¶ÒÐ°±",    // box type 2     Single top
  "ÕÍ¸³³ÔÍ¾ØÆµÑÏ°±",    // box type 3     Double top
  "             °±",    // box type 4     With empty border
  "ÚÄ¿³³ÀÄÙÅÃ´ÂÁ°±",    // box type 5     No border at all
  "ÜÜÜÝÞßßßÝÝÝÝÝ°±",    // box type 6     Blocky border
  ".-.||`-'+||--##",    // box type 7     ASCII border
  "lqkxxmqjntuwvaa",    // box type 8     xterm single border

  #endif
};
#else

// ncurses ACS_nnn characters are usually computed at runtime, so
// we cannot use a static array

chtype _box_table(int type, int c) {

  char asciiborder[] = ".-.||-'+||--##";

  switch(type) {
    case 4:
      switch(c) {
        case 13:
          return ACS_BOARD;
        case 14:
          return ACS_BLOCK;
        default:
          return (chtype) ' ';
      }
    case 6:
      switch(c) {
        case 13:
          return ACS_BOARD;
        default:
          return ACS_BLOCK;
      }
    case 7:
      return (chtype) (asciiborder[c]);
    default:
      switch (c) {
        case 0:
          return ACS_ULCORNER;
        case 1:
        case 6:
          return ACS_HLINE;
        case 2:
          return ACS_URCORNER;
        case 3:
        case 4:
          return ACS_VLINE;
        case 5:
          return ACS_LLCORNER;
        case 7:
          return ACS_LRCORNER;
        case 8:
          return ACS_PLUS;
        case 9:
          return ACS_LTEE;
        case 10:
          return ACS_RTEE;
        case 11:
          return ACS_TTEE;
        case 12:
          return ACS_BTEE;
        case 13:
          return ACS_BOARD;
        default:
          return ACS_BLOCK;
      }
  }
}

#endif


//  ------------------------------------------------------------------

#if defined(__UNIX__)
void gvid_boxcvt(char* s) {
  while(*s)
    *s++ = (char)gvid_boxcvtc(*s);
}

static uint32_t gvid_boxcvtc(char c) {
    switch(c) {
#if 0
      case 'Ú': return _box_table(8, 0);
      case 'Ä': return _box_table(8, 1);
      case '¿': return _box_table(8, 2);
      case '³': return _box_table(8, 4);
      case 'À': return _box_table(8, 5);
      case 'Ù': return _box_table(8, 7);
      case 'Å': return _box_table(8, 8);
      case 'Ã': return _box_table(8, 9);
      case '´': return _box_table(8, 10);
      case 'Â': return _box_table(8, 11);
      case 'Á': return _box_table(8, 12);
#else
      case 'Ú': return _box_table(0, 0);
      case 'Ä': return _box_table(0, 1);
      case '¿': return _box_table(0, 2);
      case '³': return _box_table(0, 4);
      case 'À': return _box_table(0, 5);
      case 'Ù': return _box_table(0, 7);
      case 'Å': return _box_table(0, 8);
      case 'Ã': return _box_table(0, 9);
      case '´': return _box_table(0, 10);
      case 'Â': return _box_table(0, 11);
      case 'Á': return _box_table(0, 12);
      case 'É': return _box_table(1, 0);
      case 'Í': return _box_table(1, 1);
      case '»': return _box_table(1, 2);
      case 'º': return _box_table(1, 4);
      case 'È': return _box_table(1, 5);
      case '¼': return _box_table(1, 7);
      case 'Î': return _box_table(1, 8);
      case 'Ì': return _box_table(1, 9);
      case '¹': return _box_table(1, 10);
      case 'Ë': return _box_table(1, 11);
      case 'Ê': return _box_table(1, 12);
#endif
    }
    return c;
}
#endif


//  ------------------------------------------------------------------
//  Draws a text box on the screen

void vbox(int srow, int scol, int erow, int ecol, int box, int hiattr, int loattr) {

  if(loattr == -1)
    loattr = hiattr;
  else if(loattr == -2)
    loattr = (int)((hiattr & 0x08) ? (hiattr & 0xF7) : (hiattr | 0x08));

  #if defined(__UNIX__)
  hiattr |= ACSET;
  loattr |= ACSET;
  #endif

  vputc(srow,   scol,   hiattr, _box_table(box, 0));               // Top left corner
  vputx(srow,   scol+1, hiattr, _box_table(box, 1), ecol-scol-1);  // Top border
  vputc(srow,   ecol,   loattr, _box_table(box, 2));               // Top right corner
  vputy(srow+1, scol,   hiattr, _box_table(box, 3), erow-srow-1);  // Left border
  vputy(srow+1, ecol,   loattr, _box_table(box, 4), erow-srow-1);  // Right border
  vputc(erow,   scol,   hiattr, _box_table(box, 5));               // Bottom left corner
  vputx(erow,   scol+1, loattr, _box_table(box, 6), ecol-scol-1);  // Bottom border
  vputc(erow,   ecol,   loattr, _box_table(box, 7));               // Bottom right corner
}


//  ------------------------------------------------------------------
//  Fills an area of screen with a character & attribute

void vfill(int srow, int scol, int erow, int ecol, vchar chr, int atr) {

  int width = ecol-scol+1;
  for(int crow=srow; crow<=erow; crow++)
    vputx(crow, scol, atr, chr, width);
}


//  ------------------------------------------------------------------
