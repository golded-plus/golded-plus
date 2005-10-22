
//  ------------------------------------------------------------------
//  GoldED+
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999-2000 Alexander S. Aganichev
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
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston,
//  MA 02111-1307 USA
//  ------------------------------------------------------------------
//  $Id$
//  ------------------------------------------------------------------
//  Configuration functions, part 1.
//  ------------------------------------------------------------------

#include <golded.h>
#include <gcprot.h>


//  ------------------------------------------------------------------

extern char* val;


//  ------------------------------------------------------------------

void CfgAddress()  { CfgAddress(val); }
void CfgAddress(char* v) {

  if(not strblank(v)) {

    if(veryverbose)
      std::cout << "  CfgAddress(" << v << ")" << std::endl;

    gaka aka;
    aka.domain[0] = NUL;
    aka.pointnet = 0;

    char* ptr = strchr(v, ',');
    if(ptr) {
      aka.pointnet = atow(ptr+1);
      *ptr = NUL;
    }
    if(not CFG->aka.empty()) {
      aka.addr = CFG->aka[0].addr;
      strcpy(aka.domain, CFG->aka[0].domain);
    }
    aka.addr.set(v, aka.domain);

    if(cfgingroup)
      CFG->grp.AddItm(GRP_AKA, &aka, sizeof(gaka));

    int gotaka = GetAkaNo(aka.addr);
    if(gotaka == -1)
      CFG->aka.push_back(aka);
  }
}

//  ------------------------------------------------------------------

void CfgAddressbookadd() {

  CFG->addressbookadd = GetYesno(val);
}

//  ------------------------------------------------------------------

void CfgAddresslookupfirst(){

  if(strieql(val, "MSGID"))
    CFG->addresslookupfirst = YES;
  else
    CFG->addresslookupfirst = NO;
}

//  ------------------------------------------------------------------

void CfgAddressmacro(char* v)  { val = v; CfgAddressmacro(); }
void CfgAddressmacro() {

  AddrMacro tmp;
  tmp.attr.reset();
  int subjwasquoted = NO;
  char* ptr = val;
  char* ptr2 = strchr(ptr, ',');                                 //  macro^,name,address,subj,attribs
  if(ptr2) {
    // Got macroname
    *ptr2++ = NUL;
    strxcpy(tmp.macro, ptr, 20);
    ptr = strskip_wht(ptr2);
    ptr2 = strchr(ptr, ',');                                    //  macro,name^,address,subj,attribs
    if(ptr2) {
      // Got real name
      *ptr2++ = NUL;
      strxcpy(tmp.name, ptr, sizeof(IAdr));
      ptr = strskip_wht(ptr2);
      ptr2 = strchr(ptr, ',');                                  //  macro,name,address^,subj,attribs
      if(ptr2)
        *ptr2++ = NUL;
      // Got address
      if(not CFG->aka.empty())
        tmp.addr = CFG->aka[0].addr;
      tmp.addr.set(ptr);
      if(ptr2) {
        // Got subject
        ptr = strskip_wht(ptr2);
        if((*ptr == '\"') or (*ptr == '\'')) {
          ptr2 = strchr(ptr+1, *ptr);
          if(ptr2) {
            subjwasquoted = YES;
            *ptr2++ = NUL;
            ptr++;
          }
        }
        strxcpy(tmp.subj, ptr, sizeof(Subj));

        // Got quoted subj, so look for attribs
        if(subjwasquoted) {
          ptr2 = strchr(ptr2, ',');                             //  macro,name,address,subj^,attribs
          if(ptr2)
            GetAttribstr(&tmp.attr, ptr2+1);
        }
      }
      else
        tmp.subj[0] = NUL;
      CFG->addressmacro.push_back(tmp);
    }
  }
}

//  ------------------------------------------------------------------

void CfgAdeptxbbspath(const char *path, bool force) {

  if (force or strblank(CFG->adeptxbbspath))
    MapPath(PathCopy(CFG->adeptxbbspath, path));
}

void CfgAdeptxbbspath() {

  CfgAdeptxbbspath(val, true);
}

//  ------------------------------------------------------------------

void CfgAdeptxbbsuserno() {

  CFG->adeptxbbsuserno = atoi(val);
}

//  ------------------------------------------------------------------

void CfgAkamatch() {

  char* key;
  getkeyval(&key, &val);

  AkaMatchG akamatchg;
  akamatchg.mask.set_all(GFTN_ALL);
  akamatchg.mask.set(key);
  akamatchg.aka.reset(val);
  CFG->akamatch.push_back(akamatchg);
}

//  ------------------------------------------------------------------

void CfgAkamatchfromto() {
  CFG->akamatchfromto = GetYesno(val);
}

//  ------------------------------------------------------------------

void CfgAkamatching() {

  if(cfgingroup) {
    bool flag = make_bool(GetYesno(val));
    CFG->grp.AddItm(GRP_AKAMATCHING, flag);
  }
}

//  ------------------------------------------------------------------

void CfgAkamatchmanually()
{
  CFG->akamatchmanually = make_bool(GetYesno(val));
}

//  ------------------------------------------------------------------

void CfgArea() {

  AL.GetArea(val);
}

//  ------------------------------------------------------------------

void CfgAreaautoid() {

  if(striinc("VERBOSE", val))
    CFG->areaautoid = AUTOID_VERBOSE;
  else if(striinc("SHORT", val))
    CFG->areaautoid = AUTOID_SHORT;
  else if(striinc("LONG", val))
    CFG->areaautoid = AUTOID_LONG;
}

//  ------------------------------------------------------------------

void CfgAreacfmreplyto() {

  strxcpy(CFG->areacfmreplyto, val, sizeof(Echo));
}

//  ------------------------------------------------------------------

void CfgAreacopydirect() {

  bool flag = make_bool(GetYesno(val));
  if(cfgingroup)
    CFG->grp.AddItm(GRP_AREACOPYDIRECT, flag);
  else
    CFG->areacopydirect = flag;
}

//  ------------------------------------------------------------------

void CfgAreacopyto() {

  Echo buf;
  strxcpy(buf, val, sizeof(buf));
  if(cfgingroup)
    CFG->grp.AddItm(GRP_AREACOPYTO, buf, strlen(buf)+1);
  else
    strcpy(CFG->areacopyto, buf);
}

//  ------------------------------------------------------------------

void CfgAreacopyaddid() {

  bool flag = make_bool(GetYesno(val));
  if(cfgingroup)
    CFG->grp.AddItm(GRP_AREACOPYADDID, flag);
  else
    CFG->areacopyaddid = flag;
}

//  ------------------------------------------------------------------

void CfgAreadef() {

  AL.GetAreaDef(val);
}

//  ------------------------------------------------------------------

void CfgAreadesc() {

  AL.GetAreaDesc(val);
}

//  ------------------------------------------------------------------

void CfgAreaexcl() {

  tokenize(CFG->areaexcl, val);
}


//  ------------------------------------------------------------------

void CfgAreaforwarddirect() {

  bool flag = make_bool(GetYesno(val));
  if(cfgingroup)
    CFG->grp.AddItm(GRP_AREAFORWARDDIRECT, flag);
  else
    CFG->areaforwarddirect = flag;
}

//  ------------------------------------------------------------------

void CfgAreafreqdirect() {

  bool flag = make_bool(GetYesno(val));
  if(cfgingroup)
    CFG->grp.AddItm(GRP_AREAFREQDIRECT, flag);
  else
    CFG->areafreqdirect = flag;
}

//  ------------------------------------------------------------------

void CfgAreafreqto() {

  Echo buf;
  strxcpy(buf, val, sizeof(buf));
  if(cfgingroup)
    CFG->grp.AddItm(GRP_AREAFREQTO, buf, strlen(buf)+1);
  else
    strcpy(CFG->areafreqto, buf);
}

//  ------------------------------------------------------------------

void CfgAreaincl() {

  tokenize(CFG->areaincl, val);
}

//  ------------------------------------------------------------------

void CfgAreaisemail() {

  tokenize(CFG->areaisemail, val);
}

//  ------------------------------------------------------------------

void CfgAreaisnews() {

  tokenize(CFG->areaisnews, val);
}

//  ------------------------------------------------------------------

void CfgArealistechomax() {

  CFG->arealistechomax = atoi(val);
}

//  ------------------------------------------------------------------

void CfgArealistformat() {

  strxcpy(CFG->arealistformat, StripQuotes(val), sizeof(CFG->arealistformat));
}


//  ------------------------------------------------------------------

void CfgArealistgrouporder() {

  strxcpy(CFG->arealistgrouporder, StripQuotes(val), sizeof(CFG->arealistgrouporder));
}


//  ------------------------------------------------------------------

void CfgArealistscan() {

  char* _key;
  getkeyval(&_key, &val);
  AL.ListScan.Add(std::pair<std::string, std::string>(StripQuotes(_key), val));
}

//  ------------------------------------------------------------------

void CfgArealistsort() {

  strxcpy(CFG->arealistsort, val, sizeof(CFG->arealistsort));
}

//  ------------------------------------------------------------------

void CfgArealisttype() {

  if(strieql(val, "New"))
    CFG->arealisttype = AL_TOTNEW;
  else if(strieql(val, "Last"))
    CFG->arealisttype = AL_TOTLST;
}

//  ------------------------------------------------------------------

void CfgAreapath() {

  MapPath(PathCopy(CFG->areapath, val));
}

//  ------------------------------------------------------------------

void CfgAreapmscan() {

  tokenize(CFG->areapmscan, val);
}

//  ------------------------------------------------------------------

void CfgAreapmscanexcl() {

  tokenize(CFG->areapmscanexcl, val);
}

//  ------------------------------------------------------------------

void CfgAreapmscanincl() {

  tokenize(CFG->areapmscanincl, val);
}

//  ------------------------------------------------------------------

void CfgAreareadonly() {

  if(strieql(val, "Soft"))
    CFG->areareadonly = READONLY_SOFT;
  else if(strieql(val, "Hard"))
    CFG->areareadonly = READONLY_HARD;
}

//  ------------------------------------------------------------------

void CfgArearename() {

  char* key;
  getkeyval(&key, &val);
  CFG->arearename.push_back(EchoRen(key, strtrim(val)));
}

//  ------------------------------------------------------------------

