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
//  Intel 80x86 interrupt portable wrapper class.
//  ------------------------------------------------------------------

#ifndef __gmemi86_h
#define __gmemi86_h


//  ------------------------------------------------------------------

#if defined(__MSDOS__)


//  ------------------------------------------------------------------

#include <dos.h>
#include <gdefs.h>


//  ------------------------------------------------------------------

#if defined(__WATCOMC__)

#define GREGPACK REGPACK

#elif defined(__DJGPP__)

#include <dpmi.h>
#define GREGPACK __dpmi_regs

#elif defined(__BORLANDC__)

#define GREGPACK REGS

#else

#error not supported!

#endif


//  ------------------------------------------------------------------

class i86 {

protected:

  GREGPACK r;
  #if defined(__BORLANDC__)
  SREGS sr;
  #endif

public:

  inline uint al()    { return r.h.al; }
  inline uint ah()    { return r.h.ah; }
  inline uint bl()    { return r.h.bl; }
  inline uint bh()    { return r.h.bh; }
  inline uint cl()    { return r.h.cl; }
  inline uint ch()    { return r.h.ch; }
  inline uint dl()    { return r.h.dl; }
  inline uint dh()    { return r.h.dh; }
  #ifdef __DJGPP__
  inline uint ax()    { return r.x.ax; }
  inline uint bx()    { return r.x.bx; }
  inline uint cx()    { return r.x.cx; }
  inline uint dx()    { return r.x.dx; }
  inline uint bp()    { return r.x.bp; }
  inline uint si()    { return r.x.si; }
  inline uint di()    { return r.x.di; }
  inline uint ds()    { return r.x.ds; }
  inline uint es()    { return r.x.es; }
  inline uint flags() { return r.x.flags; }
  #else
  inline uint ax()    { return r.w.ax; }
  inline uint bx()    { return r.w.bx; }
  inline uint cx()    { return r.w.cx; }
  inline uint dx()    { return r.w.dx; }
  inline uint bp()    { return r.w.bp; }
  inline uint si()    { return r.w.si; }
  inline uint di()    { return r.w.di; }
  #if defined(__BORLANDC__)
  inline uint ds()    { return sr.w.ds; }
  inline uint es()    { return sr.w.es; }
  #else
  inline uint ds()    { return r.w.ds; }
  inline uint es()    { return r.w.es; }
  #endif
  inline uint flags() { return r.w.flags; }
  #endif

  inline void al(uint32_t b)    { r.h.al = (byte)b; }
  inline void ah(uint32_t b)    { r.h.ah = (byte)b; }
  inline void bl(uint32_t b)    { r.h.bl = (byte)b; }
  inline void bh(uint32_t b)    { r.h.bh = (byte)b; }
  inline void cl(uint32_t b)    { r.h.cl = (byte)b; }
  inline void ch(uint32_t b)    { r.h.ch = (byte)b; }
  inline void dl(uint32_t b)    { r.h.dl = (byte)b; }
  inline void dh(uint32_t b)    { r.h.dh = (byte)b; }
  #ifdef __DJGPP__
  inline void ax(uint32_t w)    { r.x.ax = (word)w; }
  inline void bx(uint32_t w)    { r.x.bx = (word)w; }
  inline void cx(uint32_t w)    { r.x.cx = (word)w; }
  inline void dx(uint32_t w)    { r.x.dx = (word)w; }
  inline void bp(uint32_t w)    { r.x.bp = (word)w; }
  inline void si(uint32_t w)    { r.x.si = (word)w; }
  inline void di(uint32_t w)    { r.x.di = (word)w; }
  inline void ds(uint32_t w)    { r.x.ds = (word)w; }
  inline void es(uint32_t w)    { r.x.es = (word)w; }
  inline void flags(uint32_t w) { r.x.flags = (word)w; }
  #else
  inline void ax(uint32_t w)    { r.w.ax = (word)w; }
  inline void bx(uint32_t w)    { r.w.bx = (word)w; }
  inline void cx(uint32_t w)    { r.w.cx = (word)w; }
  inline void dx(uint32_t w)    { r.w.dx = (word)w; }
  inline void bp(uint32_t w)    { r.w.bp = (word)w; }
  inline void si(uint32_t w)    { r.w.si = (word)w; }
  inline void di(uint32_t w)    { r.w.di = (word)w; }
  #if defined(__BORLANDC__)
  inline void ds(uint32_t w)    { sr.w.ds = (word)w; }
  inline void es(uint32_t w)    { sr.w.es = (word)w; }
  #else
  inline void ds(uint32_t w)    { r.w.ds = (word)w; }
  inline void es(uint32_t w)    { r.w.es = (word)w; }
  #endif
  inline void flags(uint32_t w) { r.w.flags = (word)w; }
  #endif

  void genint(int intno);
};


//  ------------------------------------------------------------------
//  Inline implementations
 
inline void i86::genint(int intno) {

  #if defined(__BORLANDC__)
  #if defined(__DPMI32__)
  int386x(intno, &r, &r, &sr);
  #else
  int86x(intno, &r, &r, &sr);
  #endif
  #elif defined(__DJGPP__)
  __dpmi_int(intno, &r);
  #else
  intr(intno, &r);
  #endif
}


#if defined(__WATCOMC__) && defined(__386__)
inline int __dpmi_allocate_dos_memory(long len, int &buf) {

  i86 cpu;
  // Determine how much is available
  cpu.ax(0x100);
  cpu.bx(65535u);
  cpu.genint(0x31);
  if(len > (long)cpu.bx()*16L)
    return -1;
  cpu.ax(0x100);
  cpu.bx(len);
  cpu.genint(0x31);
  *buffer = (int) cpu.dx();
  return (int) cpu.ax();
}


inline void __dpmi_free_dos_memory(int buffer) {

  i86 cpu;
  cpu.ax(0x101);
  cpu.dx((word) buffer);
  cpu.genint(0x31);
}
#endif


//  ------------------------------------------------------------------
//  A more portable version of MK_FP()

#if defined(__WATCOMC__) && defined(__386__)

#define gmkfp(s,o) ((s << 4) + o)

#elif defined(__DJGPP__)

#include <sys/nearptr.h>
#include <crt0.h>

inline void *gmkfp(unsigned short seg, unsigned short ofs) {
  if(not (_crt0_startup_flags & _CRT0_FLAG_NEARPTR))
    if(not __djgpp_nearptr_enable())
      return (void *)0;
  return (void *)(seg*16 + ofs + __djgpp_conventional_base);
}

#else

#define gmkfp(s,o) MK_FP(s,o)

#endif


//  ------------------------------------------------------------------

#endif
#endif

//  ------------------------------------------------------------------
