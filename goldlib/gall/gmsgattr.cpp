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

#include <gstrall.h>
#include <gmsgattr.h>


//  ------------------------------------------------------------------
//  Add attributes

void ftn_attr::add(const ftn_attr& b) {

  if(b.pvt())  pvt1();
  if(b.cra())  cra1();
  if(b.rcv())  rcv1();
  if(b.snt())  snt1();
  if(b.att())  att1();
  if(b.trs())  trs1();
  if(b.orp())  orp1();
  if(b.k_s())  k_s1();
  if(b.loc())  loc1();
  if(b.hld())  hld1();
  if(b.rsv())  rsv1();
  if(b.frq())  frq1();
  if(b.rrq())  rrq1();
  if(b.rrc())  rrc1();
  if(b.arq())  arq1();
  if(b.urq())  urq1();

  if(b.grp())  grp1_();
  if(b.imm())  imm1();
  if(b.dir())  dir1();
  if(b.tfs())  tfs1();
  if(b.kfs())  kfs1();
  if(b.lok())  lok1();
  if(b.a_s())  a_s1();
  if(b.zon())  zon1();
  if(b.hub())  hub1();
  if(b.xma())  xma1();
  if(b.cfm())  cfm1();
  if(b.hir())  hir1();
  if(b.cov())  cov1();
  if(b.sig())  sig1();
  if(b.let())  let1();
  if(b.uns())  uns1();

  if(b.del())  del1();
  if(b.nwm())  nwm1();
  if(b.rot())  rot1();
  if(b.hex())  hex1();
  if(b.upd())  upd1();
  if(b.tou())  tou1();
  if(b.fmu())  fmu1();
  if(b.scn())  scn1();
  if(b.pos())  pos1();
  if(b.r_o())  r_o1();
  if(b.nok())  nok1();
  if(b.fax())  fax1();
  if(b.prn())  prn1();
}
 

//  ------------------------------------------------------------------
//  Create string representation of an attribute set

std::string& ftn_attr::make_string(std::string& s) const {

  s = "";

  if(del())  s += "DEL ";
  if(rcv())  s += "Rcv ";
  if(snt())  s += "Snt ";
  if(uns())  s += "Uns ";
  if(imm())  s += "Imm ";
  if(dir())  s += "Dir ";
  if(lok())  s += "Lok ";
  if(tfs())  s += "Tfs ";
  if(kfs())  s += "Kfs ";
  if(pvt())  s += "Pvt ";
  if(cra())  s += "Cra ";
  if(loc())  s += "Loc ";
  if(hld())  s += "Hld ";
  if(k_s())  s += "K/s ";
  if(frq())  s += "Frq ";
  if(urq())  s += "URq ";
  if(att())  s += "Att ";
  if(arq())  s += "ARq ";
  if(rrq())  s += "RRq ";
  if(rrc())  s += "RRc ";
  if(trs())  s += "Trs ";
  if(orp())  s += "Orp ";
  if(a_s())  s += "A/s ";
  if(zon())  s += "Zon ";
  if(hub())  s += "Hub ";
  if(xma())  s += "Xma ";
  if(cfm())  s += "Cfm ";
  if(hir())  s += "Hir ";
  if(cov())  s += "Cov ";
  if(sig())  s += "Sig ";
  if(let())  s += "Let ";
  if(fax())  s += "Fax ";
  if(rsv())  s += "Rsv ";
  if(grp())  s += "Grp ";
  if(scn())  s += "Scn ";
  if(prn())  s += "Prn ";

  size_t len = s.length();
  while(len and ('!' > s[--len]))
    s.erase(len, 1);

  return s;
}


//  ------------------------------------------------------------------

void ftn_attr::get(const std::string& __s) {

  const char *s = __s.c_str();
  if(striinc("PVT", s))  pvt1();
  if(striinc("CRA", s))  cra1();
  if(striinc("RCV", s))  rcv1();
  if(striinc("SNT", s))  snt1();
  if(striinc("ATT", s))  att1();
  if(striinc("TRS", s))  trs1();
  if(striinc("ORP", s))  orp1();
  if(striinc("K/S", s))  k_s1();
  if(striinc("LOC", s))  loc1();
  if(striinc("HLD", s))  hld1();
  if(striinc("RSV", s))  rsv1();
  if(striinc("FRQ", s))  frq1();
  if(striinc("RRQ", s))  rrq1();
  if(striinc("RRC", s))  rrc1();
  if(striinc("ARQ", s))  arq1();
  if(striinc("URQ", s))  urq1();

  if(striinc("GRP", s))  grp1_();
  if(striinc("IMM", s))  imm1();
  if(striinc("DIR", s))  dir1();
  if(striinc("TFS", s))  tfs1();
  if(striinc("KFS", s))  kfs1();
  if(striinc("LOK", s))  lok1();
  if(striinc("A/S", s))  a_s1();
  if(striinc("ZON", s))  zon1();
  if(striinc("HUB", s))  hub1();
  if(striinc("XMA", s))  xma1();
  if(striinc("CFM", s))  cfm1();
  if(striinc("HIR", s))  hir1();
  if(striinc("COV", s))  cov1();
  if(striinc("SIG", s))  sig1();
  if(striinc("LET", s))  let1();
  if(striinc("UNS", s))  uns1();

  if(striinc("DEL", s))  del1();
  if(striinc("NWM", s))  nwm1();
  if(striinc("ROT", s))  rot1();
  if(striinc("HEX", s))  hex1();
  if(striinc("UPD", s))  upd1();
  if(striinc("TOU", s))  tou1();
  if(striinc("FMU", s))  fmu1();
  if(striinc("SCN", s))  scn1();
  if(striinc("POS", s))  pos1();
  if(striinc("R/O", s))  r_o1();
  if(striinc("NOK", s))  nok1();
  if(striinc("FAX", s))  fax1();
  if(striinc("PRN", s))  prn1();
}


//  ------------------------------------------------------------------

bool ftn_attr::equals(const ftn_attr& b) const {

  return (attr1 == b.attr1) and (attr2 == b.attr2);
}


//  ------------------------------------------------------------------
