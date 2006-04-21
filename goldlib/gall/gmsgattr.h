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
//  Message attributes.
//  ------------------------------------------------------------------

#ifndef __gmsgattr_h
#define __gmsgattr_h


//  ------------------------------------------------------------------

#include <string>
#include <gdefs.h>
#include <gstrall.h>


//  ------------------------------------------------------------------
//  Internal GoldED message attributes

const uint32_t GATTR_PVT = 0x00000001UL;  // private
const uint32_t GATTR_CRA = 0x00000002UL;  // crash
const uint32_t GATTR_RCV = 0x00000004UL;  // received
const uint32_t GATTR_SNT = 0x00000008UL;  // sent

const uint32_t GATTR_ATT = 0x00000010UL;  // file attach
const uint32_t GATTR_TRS = 0x00000020UL;  // transit
const uint32_t GATTR_ORP = 0x00000040UL;  // orphaned
const uint32_t GATTR_K_S = 0x00000080UL;  // kill msg when sent

const uint32_t GATTR_LOC = 0x00000100UL;  // local
const uint32_t GATTR_HLD = 0x00000200UL;  // hold
const uint32_t GATTR_RSV = 0x00000400UL;  // FTS-1 reserved
const uint32_t GATTR_FRQ = 0x00000800UL;  // file request

const uint32_t GATTR_RRQ = 0x00001000UL;  // return receipt request
const uint32_t GATTR_RRC = 0x00002000UL;  // return receipt
const uint32_t GATTR_ARQ = 0x00004000UL;  // audit request
const uint32_t GATTR_URQ = 0x00008000UL;  // file update request

const uint32_t GATTR_GRP = 0x00010000UL;  // group msg (hudson)
const uint32_t GATTR_IMM = 0x00020000UL;  // immediate
const uint32_t GATTR_DIR = 0x00040000UL;  // direct
const uint32_t GATTR_TFS = 0x00080000UL;  // truncate file when sent

const uint32_t GATTR_KFS = 0x00100000UL;  // kill file when sent
const uint32_t GATTR_LOK = 0x00200000UL;  // locked
const uint32_t GATTR_A_S = 0x00400000UL;  // archive msg when sent
const uint32_t GATTR_ZON = 0x00800000UL;  // send through zone gate

const uint32_t GATTR_HUB = 0x01000000UL;  // host- or hub route
const uint32_t GATTR_XMA = 0x02000000UL;  // xmail: alternate form of compressed mail attached
const uint32_t GATTR_CFM = 0x04000000UL;  // confirmation receipt requested
const uint32_t GATTR_HIR = 0x08000000UL;  // fax: hi-resolution image

const uint32_t GATTR_COV = 0x10000000UL;  // fax: cover sheet
const uint32_t GATTR_SIG = 0x20000000UL;  // fax: signature
const uint32_t GATTR_LET = 0x40000000UL;  // fax: letterhead
const uint32_t GATTR_UNS = 0x80000000UL;  // unsent - not scanned (internal)

const uint32_t GATTR_DEL = 0x00000001UL;  // deleted
const uint32_t GATTR_NWM = 0x00000002UL;  // new message (internal)
const uint32_t GATTR_ROT = 0x00000004UL;  // rot-13 encoded (internal)
const uint32_t GATTR_HEX = 0x00000008UL;  // hexdump (internal)

const uint32_t GATTR_UPD = 0x00000010UL;  // update *.msg time stamp (internal)
const uint32_t GATTR_TOU = 0x00000020UL;  // to-you (internal)
const uint32_t GATTR_FMU = 0x00000040UL;  // from-you (internal)
const uint32_t GATTR_SCN = 0x00000080UL;  // scanned (squish)

const uint32_t GATTR_POS = 0x00000100UL;  // set line position (internal)
const uint32_t GATTR_R_O = 0x00000200UL;  // area read-only
const uint32_t GATTR_NOK = 0x00000400UL;  // (not used)
const uint32_t GATTR_FAX = 0x00000800UL;  // file attached is a fax image

const uint32_t GATTR_PRN = 0x00001000UL;  // message is printed (squish)
const uint32_t GATTR_ANO = 0x00002000UL;  // anonymous
const uint32_t GATTR_UME = 0x00004000UL;  // unmoved echo message (adeptxbbs)
const uint32_t GATTR_UMN = 0x00008000UL;  // unmoved net message (adeptxbbs)

const uint32_t GATTR_FSC = 0x00010000UL;  // fidonet scanned (adeptxbbs)
const uint32_t GATTR_GSC = 0x00020000UL;  // groupmail scanned (adeptxbbs)
const uint32_t GATTR_RSC = 0x00040000UL;  // rfc822 scanned (adeptxbbs)
const uint32_t GATTR_TRT = 0x00080000UL;  // treated: soft cr's & lf's removed (adeptxbbs)

const uint32_t GATTR_LZS = 0x00100000UL;  // msg is lzss compressed (adeptxbbs)
const uint32_t GATTR_ARC = 0x00200000UL;  // message is stored (adeptxbbs)
const uint32_t GATTR_TAG = 0x00400000UL;  // tagged: used by offline readers (adeptxbbs & wildcat)
const uint32_t GATTR_RAB = 0x00800000UL;  // receivable (wildcat)

const uint32_t GATTR_CAR = 0x01000000UL;  // carboned (wildcat)
const uint32_t GATTR_FWD = 0x02000000UL;  // forwarded (wildcat)
const uint32_t GATTR_EFL = 0x04000000UL;  // echoflag (wildcat)
const uint32_t GATTR_HRP = 0x08000000UL;  // has replies (wildcat)

const uint32_t GATTR_PKD = 0x10000000UL;  // Archived

class ftn_attr {

protected:

  uint32_t attr1, attr2;

public:

  ftn_attr() { reset(); }
  ftn_attr(const char* s) { get(s); }
  ftn_attr(const std::string& s) { get(s); }
  ftn_attr(const ftn_attr& o) { operator=(o); }

  void reset() { attr1 = attr2 = 0; }

  void add(const ftn_attr& b);
  void get(const std::string& s);
  std::string& make_string(std::string& s) const;

  bool equals(const ftn_attr& b) const;

  ftn_attr& operator=(const char* s)      { get(s); return *this; }
  ftn_attr& operator=(const std::string& s)   { get(s); return *this; }
  ftn_attr& operator=(const ftn_attr& o)  { attr1=o.attr1; attr2=o.attr2; return *this; }

  bool operator==(const ftn_attr& b) const  { return equals(b); }
  bool operator!=(const ftn_attr& b) const  { return !equals(b); }

  //  ----------------------------------------------------------------

  int pvt() const  { return (attr1 & GATTR_PVT) != 0; }
  int cra() const  { return (attr1 & GATTR_CRA) != 0; }
  int rcv() const  { return (attr1 & GATTR_RCV) != 0; }
  int snt() const  { return (attr1 & GATTR_SNT) != 0; }

  int att() const  { return (attr1 & GATTR_ATT) != 0; }
  int trs() const  { return (attr1 & GATTR_TRS) != 0; }
  int orp() const  { return (attr1 & GATTR_ORP) != 0; }
  int k_s() const  { return (attr1 & GATTR_K_S) != 0; }

  int loc() const  { return (attr1 & GATTR_LOC) != 0; }
  int hld() const  { return (attr1 & GATTR_HLD) != 0; }
  int rsv() const  { return (attr1 & GATTR_RSV) != 0; }
  int frq() const  { return (attr1 & GATTR_FRQ) != 0; }

  int rrq() const  { return (attr1 & GATTR_RRQ) != 0; }
  int rrc() const  { return (attr1 & GATTR_RRC) != 0; }
  int arq() const  { return (attr1 & GATTR_ARQ) != 0; }
  int urq() const  { return (attr1 & GATTR_URQ) != 0; }

  int grp() const  { return (attr1 & GATTR_GRP) != 0; }
  int imm() const  { return (attr1 & GATTR_IMM) != 0; }
  int dir() const  { return (attr1 & GATTR_DIR) != 0; }
  int tfs() const  { return (attr1 & GATTR_TFS) != 0; }

  int kfs() const  { return (attr1 & GATTR_KFS) != 0; }
  int lok() const  { return (attr1 & GATTR_LOK) != 0; }
  int a_s() const  { return (attr1 & GATTR_A_S) != 0; }
  int zon() const  { return (attr1 & GATTR_ZON) != 0; }

  int hub() const  { return (attr1 & GATTR_HUB) != 0; }
  int xma() const  { return (attr1 & GATTR_XMA) != 0; }
  int cfm() const  { return (attr1 & GATTR_CFM) != 0; }
  int hir() const  { return (attr1 & GATTR_HIR) != 0; }

  int cov() const  { return (attr1 & GATTR_COV) != 0; }
  int sig() const  { return (attr1 & GATTR_SIG) != 0; }
  int let() const  { return (attr1 & GATTR_LET) != 0; }
  int uns() const  { return (attr1 & GATTR_UNS) != 0; }

  int del() const  { return (attr2 & GATTR_DEL) != 0; }
  int nwm() const  { return (attr2 & GATTR_NWM) != 0; }
  int rot() const  { return (attr2 & GATTR_ROT) != 0; }
  int hex() const  { return (attr2 & GATTR_HEX) != 0; }

  int upd() const  { return (attr2 & GATTR_UPD) != 0; }
  int tou() const  { return (attr2 & GATTR_TOU) != 0; }
  int fmu() const  { return (attr2 & GATTR_FMU) != 0; }
  int scn() const  { return (attr2 & GATTR_SCN) != 0; }

  int pos() const  { return (attr2 & GATTR_POS) != 0; }
  int r_o() const  { return (attr2 & GATTR_R_O) != 0; }
  int nok() const  { return (attr2 & GATTR_NOK) != 0; }
  int fax() const  { return (attr2 & GATTR_FAX) != 0; }

  int prn() const  { return (attr2 & GATTR_PRN) != 0; }
  int ano() const  { return (attr2 & GATTR_ANO) != 0; }
  int ume() const  { return (attr2 & GATTR_UME) != 0; }
  int umn() const  { return (attr2 & GATTR_UMN) != 0; }

  int fsc() const  { return (attr2 & GATTR_FSC) != 0; }
  int gsc() const  { return (attr2 & GATTR_GSC) != 0; }
  int rsc() const  { return (attr2 & GATTR_RSC) != 0; }
  int trt() const  { return (attr2 & GATTR_TRT) != 0; }

  int lzs() const  { return (attr2 & GATTR_LZS) != 0; }
  int arc() const  { return (attr2 & GATTR_ARC) != 0; }
  int tag() const  { return (attr2 & GATTR_TAG) != 0; }
  int rab() const  { return (attr2 & GATTR_RAB) != 0; }

  int car() const  { return (attr2 & GATTR_CAR) != 0; }
  int fwd() const  { return (attr2 & GATTR_FWD) != 0; }
  int efl() const  { return (attr2 & GATTR_EFL) != 0; }
  int hrp() const  { return (attr2 & GATTR_HRP) != 0; }

  int pkd() const  { return (attr2 & GATTR_PKD) != 0; }

  //  ----------------------------------------------------------------

  void pvt(uint32_t x)  { if(x) attr1 |= GATTR_PVT; else attr1 &= ~GATTR_PVT; }
  void cra(uint32_t x)  { if(x) attr1 |= GATTR_CRA; else attr1 &= ~GATTR_CRA; }
  void rcv(uint32_t x)  { if(x) attr1 |= GATTR_RCV; else attr1 &= ~GATTR_RCV; }
  void snt(uint32_t x)  { if(x) attr1 |= GATTR_SNT; else attr1 &= ~GATTR_SNT; }

  void att(uint32_t x)  { if(x) attr1 |= GATTR_ATT; else attr1 &= ~GATTR_ATT; }
  void trs(uint32_t x)  { if(x) attr1 |= GATTR_TRS; else attr1 &= ~GATTR_TRS; }
  void orp(uint32_t x)  { if(x) attr1 |= GATTR_ORP; else attr1 &= ~GATTR_ORP; }
  void k_s(uint32_t x)  { if(x) attr1 |= GATTR_K_S; else attr1 &= ~GATTR_K_S; }

  void loc(uint32_t x)  { if(x) attr1 |= GATTR_LOC; else attr1 &= ~GATTR_LOC; }
  void hld(uint32_t x)  { if(x) attr1 |= GATTR_HLD; else attr1 &= ~GATTR_HLD; }
  void rsv(uint32_t x)  { if(x) attr1 |= GATTR_RSV; else attr1 &= ~GATTR_RSV; }
  void frq(uint32_t x)  { if(x) attr1 |= GATTR_FRQ; else attr1 &= ~GATTR_FRQ; }

  void rrq(uint32_t x)  { if(x) attr1 |= GATTR_RRQ; else attr1 &= ~GATTR_RRQ; }
  void rrc(uint32_t x)  { if(x) attr1 |= GATTR_RRC; else attr1 &= ~GATTR_RRC; }
  void arq(uint32_t x)  { if(x) attr1 |= GATTR_ARQ; else attr1 &= ~GATTR_ARQ; }
  void urq(uint32_t x)  { if(x) attr1 |= GATTR_URQ; else attr1 &= ~GATTR_URQ; }

  void grp(uint32_t x)  { if(x) attr1 |= GATTR_GRP; else attr1 &= ~GATTR_GRP; }
  void imm(uint32_t x)  { if(x) attr1 |= GATTR_IMM; else attr1 &= ~GATTR_IMM; }
  void dir(uint32_t x)  { if(x) attr1 |= GATTR_DIR; else attr1 &= ~GATTR_DIR; }
  void tfs(uint32_t x)  { if(x) attr1 |= GATTR_TFS; else attr1 &= ~GATTR_TFS; }

  void kfs(uint32_t x)  { if(x) attr1 |= GATTR_KFS; else attr1 &= ~GATTR_KFS; }
  void lok(uint32_t x)  { if(x) attr1 |= GATTR_LOK; else attr1 &= ~GATTR_LOK; }
  void a_s(uint32_t x)  { if(x) attr1 |= GATTR_A_S; else attr1 &= ~GATTR_A_S; }
  void zon(uint32_t x)  { if(x) attr1 |= GATTR_ZON; else attr1 &= ~GATTR_ZON; }

  void hub(uint32_t x)  { if(x) attr1 |= GATTR_HUB; else attr1 &= ~GATTR_HUB; }
  void xma(uint32_t x)  { if(x) attr1 |= GATTR_XMA; else attr1 &= ~GATTR_XMA; }
  void cfm(uint32_t x)  { if(x) attr1 |= GATTR_CFM; else attr1 &= ~GATTR_CFM; }
  void hir(uint32_t x)  { if(x) attr1 |= GATTR_HIR; else attr1 &= ~GATTR_HIR; }

  void cov(uint32_t x)  { if(x) attr1 |= GATTR_COV; else attr1 &= ~GATTR_COV; }
  void sig(uint32_t x)  { if(x) attr1 |= GATTR_SIG; else attr1 &= ~GATTR_SIG; }
  void let(uint32_t x)  { if(x) attr1 |= GATTR_LET; else attr1 &= ~GATTR_LET; }
  void uns(uint32_t x)  { if(x) attr1 |= GATTR_UNS; else attr1 &= ~GATTR_UNS; }

  void del(uint32_t x)  { if(x) attr2 |= GATTR_DEL; else attr2 &= ~GATTR_DEL; }
  void nwm(uint32_t x)  { if(x) attr2 |= GATTR_NWM; else attr2 &= ~GATTR_NWM; }
  void rot(uint32_t x)  { if(x) attr2 |= GATTR_ROT; else attr2 &= ~GATTR_ROT; }
  void hex(uint32_t x)  { if(x) attr2 |= GATTR_HEX; else attr2 &= ~GATTR_HEX; }

  void upd(uint32_t x)  { if(x) attr2 |= GATTR_UPD; else attr2 &= ~GATTR_UPD; }
  void tou(uint32_t x)  { if(x) attr2 |= GATTR_TOU; else attr2 &= ~GATTR_TOU; }
  void fmu(uint32_t x)  { if(x) attr2 |= GATTR_FMU; else attr2 &= ~GATTR_FMU; }
  void scn(uint32_t x)  { if(x) attr2 |= GATTR_SCN; else attr2 &= ~GATTR_SCN; }

  void pos(uint32_t x)  { if(x) attr2 |= GATTR_POS; else attr2 &= ~GATTR_POS; }
  void r_o(uint32_t x)  { if(x) attr2 |= GATTR_R_O; else attr2 &= ~GATTR_R_O; }
  void nok(uint32_t x)  { if(x) attr2 |= GATTR_NOK; else attr2 &= ~GATTR_NOK; }
  void fax(uint32_t x)  { if(x) attr2 |= GATTR_FAX; else attr2 &= ~GATTR_FAX; }

  void prn(uint32_t x)  { if(x) attr2 |= GATTR_PRN; else attr2 &= ~GATTR_PRN; }
  void ano(uint32_t x)  { if(x) attr2 |= GATTR_ANO; else attr2 &= ~GATTR_ANO; }
  void ume(uint32_t x)  { if(x) attr2 |= GATTR_UME; else attr2 &= ~GATTR_UME; }
  void umn(uint32_t x)  { if(x) attr2 |= GATTR_UMN; else attr2 &= ~GATTR_UMN; }

  void fsc(uint32_t x)  { if(x) attr2 |= GATTR_FSC; else attr2 &= ~GATTR_FSC; }
  void gsc(uint32_t x)  { if(x) attr2 |= GATTR_GSC; else attr2 &= ~GATTR_GSC; }
  void rsc(uint32_t x)  { if(x) attr2 |= GATTR_RSC; else attr2 &= ~GATTR_RSC; }
  void trt(uint32_t x)  { if(x) attr2 |= GATTR_TRT; else attr2 &= ~GATTR_TRT; }

  void lzs(uint32_t x)  { if(x) attr2 |= GATTR_LZS; else attr2 &= ~GATTR_LZS; }
  void arc(uint32_t x)  { if(x) attr2 |= GATTR_ARC; else attr2 &= ~GATTR_ARC; }
  void tag(uint32_t x)  { if(x) attr2 |= GATTR_TAG; else attr2 &= ~GATTR_TAG; }
  void rab(uint32_t x)  { if(x) attr2 |= GATTR_RAB; else attr2 &= ~GATTR_RAB; }

  void car(uint32_t x)  { if(x) attr2 |= GATTR_CAR; else attr2 &= ~GATTR_CAR; }
  void fwd(uint32_t x)  { if(x) attr2 |= GATTR_FWD; else attr2 &= ~GATTR_FWD; }
  void efl(uint32_t x)  { if(x) attr2 |= GATTR_EFL; else attr2 &= ~GATTR_EFL; }
  void hrp(uint32_t x)  { if(x) attr2 |= GATTR_HRP; else attr2 &= ~GATTR_HRP; }

  void pkd(uint32_t x)  { if(x) attr2 |= GATTR_PKD; else attr2 &= ~GATTR_PKD; }

  //  -------------------------------------------------------------

  void pvt0()  { attr1 &= ~GATTR_PVT; }
  void cra0()  { attr1 &= ~GATTR_CRA; }
  void rcv0()  { attr1 &= ~GATTR_RCV; }
  void snt0()  { attr1 &= ~GATTR_SNT; }

  void att0()  { attr1 &= ~GATTR_ATT; }
  void trs0()  { attr1 &= ~GATTR_TRS; }
  void orp0()  { attr1 &= ~GATTR_ORP; }
  void k_s0()  { attr1 &= ~GATTR_K_S; }

  void loc0()  { attr1 &= ~GATTR_LOC; }
  void hld0()  { attr1 &= ~GATTR_HLD; }
  void rsv0()  { attr1 &= ~GATTR_RSV; }
  void frq0()  { attr1 &= ~GATTR_FRQ; }

  void rrq0()  { attr1 &= ~GATTR_RRQ; }
  void rrc0()  { attr1 &= ~GATTR_RRC; }
  void arq0()  { attr1 &= ~GATTR_ARQ; }
  void urq0()  { attr1 &= ~GATTR_URQ; }

  void grp0()  { attr1 &= ~GATTR_GRP; }
  void imm0()  { attr1 &= ~GATTR_IMM; }
  void dir0()  { attr1 &= ~GATTR_DIR; }
  void tfs0()  { attr1 &= ~GATTR_TFS; }

  void kfs0()  { attr1 &= ~GATTR_KFS; }
  void lok0()  { attr1 &= ~GATTR_LOK; }
  void a_s0()  { attr1 &= ~GATTR_A_S; }
  void zon0()  { attr1 &= ~GATTR_ZON; }

  void hub0()  { attr1 &= ~GATTR_HUB; }
  void xma0()  { attr1 &= ~GATTR_XMA; }
  void cfm0()  { attr1 &= ~GATTR_CFM; }
  void hir0()  { attr1 &= ~GATTR_HIR; }

  void cov0()  { attr1 &= ~GATTR_COV; }
  void sig0()  { attr1 &= ~GATTR_SIG; }
  void let0()  { attr1 &= ~GATTR_LET; }
  void uns0()  { attr1 &= ~GATTR_UNS; }

  void del0()  { attr2 &= ~GATTR_DEL; }
  void nwm0()  { attr2 &= ~GATTR_NWM; }
  void rot0()  { attr2 &= ~GATTR_ROT; }
  void hex0()  { attr2 &= ~GATTR_HEX; }

  void upd0()  { attr2 &= ~GATTR_UPD; }
  void tou0()  { attr2 &= ~GATTR_TOU; }
  void fmu0()  { attr2 &= ~GATTR_FMU; }
  void scn0()  { attr2 &= ~GATTR_SCN; }

  void pos0()  { attr2 &= ~GATTR_POS; }
  void r_o0()  { attr2 &= ~GATTR_R_O; }
  void nok0()  { attr2 &= ~GATTR_NOK; }
  void fax0()  { attr2 &= ~GATTR_FAX; }

  void prn0()  { attr2 &= ~GATTR_PRN; }

  //  ----------------------------------------------------------------

  void pvt1()  { attr1 |= GATTR_PVT; }
  void cra1()  { attr1 |= GATTR_CRA; }
  void rcv1()  { attr1 |= GATTR_RCV; }
  void snt1()  { attr1 |= GATTR_SNT; }

  void att1()  { attr1 |= GATTR_ATT; }
  void trs1()  { attr1 |= GATTR_TRS; }
  void orp1()  { attr1 |= GATTR_ORP; }
  void k_s1()  { attr1 |= GATTR_K_S; }

  void loc1()  { attr1 |= GATTR_LOC; }
  void hld1()  { attr1 |= GATTR_HLD; }
  void rsv1()  { attr1 |= GATTR_RSV; }
  void frq1()  { attr1 |= GATTR_FRQ; }

  void rrq1()  { attr1 |= GATTR_RRQ; }
  void rrc1()  { attr1 |= GATTR_RRC; }
  void arq1()  { attr1 |= GATTR_ARQ; }
  void urq1()  { attr1 |= GATTR_URQ; }

  void grp1_()  { attr1 |= GATTR_GRP; }
  void imm1()  { attr1 |= GATTR_IMM; }
  void dir1()  { attr1 |= GATTR_DIR; }
  void tfs1()  { attr1 |= GATTR_TFS; }

  void kfs1()  { attr1 |= GATTR_KFS; }
  void lok1()  { attr1 |= GATTR_LOK; }
  void a_s1()  { attr1 |= GATTR_A_S; }
  void zon1()  { attr1 |= GATTR_ZON; }

  void hub1()  { attr1 |= GATTR_HUB; }
  void xma1()  { attr1 |= GATTR_XMA; }
  void cfm1()  { attr1 |= GATTR_CFM; }
  void hir1()  { attr1 |= GATTR_HIR; }

  void cov1()  { attr1 |= GATTR_COV; }
  void sig1()  { attr1 |= GATTR_SIG; }
  void let1()  { attr1 |= GATTR_LET; }
  void uns1()  { attr1 |= GATTR_UNS; }

  void del1()  { attr2 |= GATTR_DEL; }
  void nwm1()  { attr2 |= GATTR_NWM; }
  void rot1()  { attr2 |= GATTR_ROT; }
  void hex1()  { attr2 |= GATTR_HEX; }

  void upd1()  { attr2 |= GATTR_UPD; }
  void tou1()  { attr2 |= GATTR_TOU; }
  void fmu1()  { attr2 |= GATTR_FMU; }
  void scn1()  { attr2 |= GATTR_SCN; }

  void pos1()  { attr2 |= GATTR_POS; }
  void r_o1()  { attr2 |= GATTR_R_O; }
  void nok1()  { attr2 |= GATTR_NOK; }
  void fax1()  { attr2 |= GATTR_FAX; }

  void prn1()  { attr2 |= GATTR_PRN; }

  void pkd1()  { attr2 |= GATTR_PKD; }

  //  ----------------------------------------------------------------

  void pvtX()  { attr1 ^= GATTR_PVT; }
  void craX()  { attr1 ^= GATTR_CRA; }
  void rcvX()  { attr1 ^= GATTR_RCV; }
  void sntX()  { attr1 ^= GATTR_SNT; }

  void attX()  { attr1 ^= GATTR_ATT; }
  void trsX()  { attr1 ^= GATTR_TRS; }
  void orpX()  { attr1 ^= GATTR_ORP; }
  void k_sX()  { attr1 ^= GATTR_K_S; }

  void locX()  { attr1 ^= GATTR_LOC; }
  void hldX()  { attr1 ^= GATTR_HLD; }
  void rsvX()  { attr1 ^= GATTR_RSV; }
  void frqX()  { attr1 ^= GATTR_FRQ; }

  void rrqX()  { attr1 ^= GATTR_RRQ; }
  void rrcX()  { attr1 ^= GATTR_RRC; }
  void arqX()  { attr1 ^= GATTR_ARQ; }
  void urqX()  { attr1 ^= GATTR_URQ; }

  void grpX()  { attr1 ^= GATTR_GRP; }
  void immX()  { attr1 ^= GATTR_IMM; }
  void dirX()  { attr1 ^= GATTR_DIR; }
  void tfsX()  { attr1 ^= GATTR_TFS; }

  void kfsX()  { attr1 ^= GATTR_KFS; }
  void lokX()  { attr1 ^= GATTR_LOK; }
  void a_sX()  { attr1 ^= GATTR_A_S; }
  void zonX()  { attr1 ^= GATTR_ZON; }

  void hubX()  { attr1 ^= GATTR_HUB; }
  void xmaX()  { attr1 ^= GATTR_XMA; }
  void cfmX()  { attr1 ^= GATTR_CFM; }
  void hirX()  { attr1 ^= GATTR_HIR; }

  void covX()  { attr1 ^= GATTR_COV; }
  void sigX()  { attr1 ^= GATTR_SIG; }
  void letX()  { attr1 ^= GATTR_LET; }
  void unsX()  { attr1 ^= GATTR_UNS; }

  void delX()  { attr2 ^= GATTR_DEL; }
  void nwmX()  { attr2 ^= GATTR_NWM; }
  void rotX()  { attr2 ^= GATTR_ROT; }
  void hexX()  { attr2 ^= GATTR_HEX; }

  void updX()  { attr2 ^= GATTR_UPD; }
  void touX()  { attr2 ^= GATTR_TOU; }
  void fmuX()  { attr2 ^= GATTR_FMU; }
  void scnX()  { attr2 ^= GATTR_SCN; }

  void posX()  { attr2 ^= GATTR_POS; }
  void r_oX()  { attr2 ^= GATTR_R_O; }
  void nokX()  { attr2 ^= GATTR_NOK; }
  void faxX()  { attr2 ^= GATTR_FAX; }

  void prnX()  { attr2 ^= GATTR_PRN; }

};


//  ------------------------------------------------------------------

typedef ftn_attr Attr;


//  ------------------------------------------------------------------

inline void AttrAdd(Attr* a, Attr* b) { a->add(*b); }
inline void GetAttribstr(Attr* attr, const TCHAR *attrs) { attr->get(attrs); }
inline TCHAR *MakeAttrStr(TCHAR *str, size_t maxlen, const Attr *attr) { std::string tmp; attr->make_string(tmp); strxcpy(str, tmp.c_str(), maxlen); return str; }


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
