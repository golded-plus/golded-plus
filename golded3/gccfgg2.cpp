
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
//  Configuration functions, part 2.
//  ------------------------------------------------------------------

#include <golded.h>
#include <gcprot.h>


//  ------------------------------------------------------------------

extern char* val;


//  ------------------------------------------------------------------

void CfgAreareplydirect() {

  bool flag = make_bool(GetYesno(val));
  if(cfgingroup)
    CFG->grp.AddItm(GRP_AREAREPLYDIRECT, flag);
  else
    CFG->areareplydirect = flag;
}

//  ------------------------------------------------------------------

void CfgAreareplyto() {

  Echo buf;
  strxcpy(buf, val, sizeof(buf));
  if(cfgingroup)
    CFG->grp.AddItm(GRP_AREAREPLYTO, buf, strlen(buf)+1);
  else
    strcpy(CFG->areareplyto, buf);
}

//  ------------------------------------------------------------------

void CfgAreascan() {

  tokenize(CFG->areascan, val);
}

//  ------------------------------------------------------------------

void CfgAreascanexcl() {

  tokenize(CFG->areascanexcl, val);
}

//  ------------------------------------------------------------------

void CfgAreascanincl() {

  tokenize(CFG->areascanincl, val);
}

//  ------------------------------------------------------------------

void CfgAreascansort() {

  strupr(strxcpy(CFG->areascansort, val, sizeof(CFG->areascansort)));
}

//  ------------------------------------------------------------------

void CfgAreasep() {

  AL.GetAreaSep(val);
}

//  ------------------------------------------------------------------

void CfgAreastart() {

  strupr(strxcpy(CFG->areastart, val, sizeof(Echo)));
}

//  ------------------------------------------------------------------

void CfgAreatypeorder() {
  
  const word CRC_NET   = 0xEC5E;
  const word CRC_ECHO  = 0xC2D1;
  const word CRC_LOCAL = 0x4CD5;
  const word CRC_EMAIL = 0x9C64;
  const word CRC_NEWS  = 0x61F1;

  GTok t;
  int order = 1;
  if(t.First(val)) {
    do {
      switch(strCrc16(t.Token())) {
        case CRC_NET:    CFG->areatypeorder[GMB_NET] = order;  break;
        case CRC_EMAIL:  CFG->areatypeorder[GMB_NET|GMB_EMAIL] = order;  break;
        case CRC_ECHO:   CFG->areatypeorder[GMB_ECHO] = order;  break;
        case CRC_NEWS:   CFG->areatypeorder[GMB_ECHO|GMB_NEWSGROUP] = order;  break;
        case CRC_LOCAL:  CFG->areatypeorder[GMB_LOCAL] = order; break;
      }
      order++;
    } while(t.Next());
  }
}

//  ------------------------------------------------------------------

void CfgAreayouwroteto() {

  Echo buf;
  strxcpy(buf, val, sizeof(buf));
  if(cfgingroup)
    CFG->grp.AddItm(GRP_AREAYOUWROTETO, buf, strlen(buf)+1);
  else
    strcpy(CFG->areayouwroteto, buf);
}

//  ------------------------------------------------------------------

void CfgAttachpath() {

  PathCopy(CFG->attachpath, val);
}

//  ------------------------------------------------------------------

void CfgAttribsattach() {

  GetAttribstr(&CFG->attribsattach, val);
}

//  ------------------------------------------------------------------

void CfgAttribscc() {

  GetAttribstr(&CFG->attribscc, val);
}

//  ------------------------------------------------------------------

void CfgAttribscfm() {

  GetAttribstr(&CFG->attribscfm, val);
}

//  ------------------------------------------------------------------

void CfgAttribsecho() {

  GetAttribstr(&CFG->attribsecho, val);
}

//  ------------------------------------------------------------------

void CfgAttribsemail() {

  GetAttribstr(&CFG->attribsemail, val);
}

//  ------------------------------------------------------------------

void CfgAttribsfrq() {

  GetAttribstr(&CFG->attribsfrq, val);
}

//  ------------------------------------------------------------------

void CfgAttribslocal() {

  GetAttribstr(&CFG->attribslocal, val);
}

//  ------------------------------------------------------------------

void CfgAttribsnet() {

  GetAttribstr(&CFG->attribsnet, val);
}

//  ------------------------------------------------------------------

void CfgAttribsnews() {

  GetAttribstr(&CFG->attribsnews, val);
}

//  ------------------------------------------------------------------

void CfgAttributes() {

  char buf[256];
  strxcpy(buf, val, 256);
  if(cfgingroup)
    CFG->grp.AddItm(GRP_ATTRIBUTES, buf, strlen(buf)+1);
}

//  ------------------------------------------------------------------

void CfgBeepfactor() {

  CFG->beepfactor = atoi(val);
}

//  ------------------------------------------------------------------

void CfgBeepyourmail() {

  CFG->beepyourmail = GetYesno(val);
}

//  ------------------------------------------------------------------

#define CRC_REMOVE   0xFAF0
#define CRC_HIDDEN   0xE465
#define CRC_VISIBLE  0xD155
#define CRC_NAMES    0xE46B
#define CRC_KEEP     0x8C07

void CfgCarboncopylist() {

  switch(strCrc16(val)) {
    case CRC_REMOVE:   CFG->carboncopylist = CC_REMOVE;   break;
    case CRC_HIDDEN:   CFG->carboncopylist = CC_HIDDEN;   break;
    case CRC_VISIBLE:  CFG->carboncopylist = CC_VISIBLE;  break;
    case CRC_NAMES:    CFG->carboncopylist = CC_NAMES;    break;
    case CRC_KEEP:     CFG->carboncopylist = CC_KEEP;     break;
    default:
      CFG->carboncopylist = GetYesno(val) ? CC_KEEP : CC_REMOVE;
  }
}

//  ------------------------------------------------------------------

void CfgColor() {

  GetColors(val);
}

//  ------------------------------------------------------------------

void CfgColorname()
{
  char* ptr = strrchr(val, ' ');
  if (!ptr) return;
  ptr = strskip_wht(ptr);

  Node cn;
  int color = atoi(ptr);

  // color was given
  *ptr = NUL;
  strbtrim(val);

  ptr = strrchr(val, ' ');
  if (!ptr) ptr = val;
  ptr = strskip_wht(ptr);

  if (isdigit(*ptr))
  {
    cn.addr.set(ptr);

    if (cn.addr.net)
    {
      // Address was given
      *ptr = NUL;
      strbtrim(val);
    }
  }

  strxcpy(cn.name, val, sizeof(cn.name));

  CFG->colorname.push_back(std::pair<Node, int>(cn, color));
}

//  ------------------------------------------------------------------

void CfgColorset() {

  if(strieql(val, "Normal")) {
    memcpy(CFG->color, gold_color1, sizeof(Win)*16);
  }
  else if(strieql(val, "Intense")) {
    CFG->intensecolors = true;
    memcpy(CFG->color, gold_color2, sizeof(Win)*16);
  }
  else if(strieql(val, "Mono")) {
    memcpy(CFG->color, gold_mono1, sizeof(Win)*16);
  }
}

//  ------------------------------------------------------------------

void CfgConfirmresponse() {

  CFG->confirmresponse = GetYesno(val);
}

//  ------------------------------------------------------------------

void CfgConfirmfile() {

  strcpy(CFG->confirmfile, val);
}

//  ------------------------------------------------------------------

void CfgCookiepath() {

  PathCopy(CFG->cookiepath, val);
}

//  ------------------------------------------------------------------

void CfgCrosspost() {

  CFG->crosspost = GetYesno(val);
}

//  ------------------------------------------------------------------

#define CRC_NONE    0xD9AD
#define CRC_RAW     0x3BE0
#define CRC_VERBOSE 0xBB00

void CfgCrosspostlist() {

  switch(strCrc16(val)) {
    case CRC_NONE:    CFG->crosspostlist = CC_REMOVE;  break;
    case CRC_VERBOSE: CFG->crosspostlist = CC_NAMES;   break;
    case CRC_RAW:     CFG->crosspostlist = CC_KEEP;    break;
    default:
      CFG->crosspostlist = GetYesno(val) ? CC_VISIBLE : CC_REMOVE;
  }
}

//  ------------------------------------------------------------------

void CfgCtrlinfo() {

  if(cfgingroup) {
    int ctrlinfo = CI_NONE;
    if(striinc("NO", val))
      ctrlinfo = CI_NONE;
    else if(striinc("YES", val))
      ctrlinfo = CI_TEAR|CI_ORIG;
    else {
      if(striinc("TEARLINE", val))
        ctrlinfo |= CI_TEAR;
      if(striinc("ORIGIN", val))
        ctrlinfo |= CI_ORIG;
    }
    CFG->grp.AddItm(GRP_CTRLINFO, ctrlinfo);
  }
}

//  ------------------------------------------------------------------

void CfgCtrlinfoecho() {

  CFG->ctrlinfoecho = CI_NONE;
  if(striinc("NO", val))
    CFG->ctrlinfoecho = CI_NONE;
  else if(striinc("YES", val))
    CFG->ctrlinfoecho = CI_TEAR|CI_ORIG;
  else {
    if(striinc("TEARLINE", val))
      CFG->ctrlinfoecho |= CI_TEAR;
    if(striinc("ORIGIN", val))
      CFG->ctrlinfoecho |= CI_ORIG;
  }
}

//  ------------------------------------------------------------------

void CfgCtrlinfoemail() {

  CFG->ctrlinfoemail = CI_NONE;
  if(striinc("NO", val))
    CFG->ctrlinfoemail = CI_NONE;
  else if(striinc("YES", val))
    CFG->ctrlinfoemail = CI_TEAR|CI_ORIG;
  else {
    if(striinc("TEARLINE", val))
      CFG->ctrlinfoemail |= CI_TEAR;
    if(striinc("ORIGIN", val))
      CFG->ctrlinfoemail |= CI_ORIG;
  }
}

//  ------------------------------------------------------------------

void CfgCtrlinfolocal() {

  CFG->ctrlinfolocal = CI_NONE;
  if(striinc("NO", val))
    CFG->ctrlinfolocal = CI_NONE;
  else if(striinc("YES", val))
    CFG->ctrlinfolocal = CI_TEAR|CI_ORIG;
  else {
    if(striinc("TEARLINE", val))
      CFG->ctrlinfolocal |= CI_TEAR;
    if(striinc("ORIGIN", val))
      CFG->ctrlinfolocal |= CI_ORIG;
  }
}

//  ------------------------------------------------------------------

void CfgCtrlinfonet() {

  CFG->ctrlinfonet = CI_NONE;
  if(striinc("NO", val))
    CFG->ctrlinfonet &= ~CI_ORIG;
  else if(striinc("YES", val))
    CFG->ctrlinfonet = CI_TEAR|CI_ORIG;
  else {
    if(striinc("TEARLINE", val))
      CFG->ctrlinfonet |= CI_TEAR;
    if(striinc("ORIGIN", val))
      CFG->ctrlinfonet |= CI_ORIG;
  }
}

//  ------------------------------------------------------------------

void CfgCtrlinfonews() {

  CFG->ctrlinfonews = CI_NONE;
  if(striinc("NO", val))
    CFG->ctrlinfonews = CI_NONE;
  else if(striinc("YES", val))
    CFG->ctrlinfonews = CI_TEAR|CI_ORIG;
  else {
    if(striinc("TEARLINE", val))
      CFG->ctrlinfonews |= CI_TEAR;
    if(striinc("ORIGIN", val))
      CFG->ctrlinfonews |= CI_ORIG;
  }
}

//  ------------------------------------------------------------------

void CfgDispattachsize() {

  if(strieql("NO", val))
    CFG->dispattachsize = NO;
  else if(strieql("BYTES", val))
    CFG->dispattachsize = ATTACH_BYTES;
  else if(strieql("KBYTES", val))
    CFG->dispattachsize = ATTACH_KBYTES;
}

//  ------------------------------------------------------------------

void CfgDisphdrdateset() {

  char* key;
  getkeyval(&key, &val);
  CFG->disphdrdateset.pos = atoi(key);
  getkeyval(&key, &val);
  CFG->disphdrdateset.len = atoi(key);
}

//  ------------------------------------------------------------------

void CfgDisphdrlocation()
{
  if (strieql("RIGHT", val))
    CFG->disphdrlocation = TRIGHT;
  else if (strieql("CENTER", val))
    CFG->disphdrlocation = TCENTER;
  else
    CFG->disphdrlocation = GetYesno(val) ? TRIGHT : NO;
}

//  ------------------------------------------------------------------

void CfgDisphdrnameset() {

  char* key;
  getkeyval(&key, &val);
  CFG->disphdrnameset.pos = atoi(key);
  getkeyval(&key, &val);
  CFG->disphdrnameset.len = atoi(key);
}

//  ------------------------------------------------------------------

void CfgDisphdrnodeset() {

  char* key;
  getkeyval(&key, &val);
  CFG->disphdrnodeset.pos = atoi(key);
  getkeyval(&key, &val);
  CFG->disphdrnodeset.len = atoi(key);
}

//  ------------------------------------------------------------------

void CfgDisplistcursor() {

  if(strieql(val, "TOP"))
    CFG->displistcursor = LIST_TOP;
  else if(strieql(val, "NEARTOP"))
    CFG->displistcursor = LIST_NEARTOP;
  else if(strieql(val, "MIDDLE"))
    CFG->displistcursor = LIST_MIDDLE;
  else if(strieql(val, "NEARBOTTOM"))
    CFG->displistcursor = LIST_NEARBOTTOM;
  else if(strieql(val, "BOTTOM"))
    CFG->displistcursor = LIST_BOTTOM;
}

//  ------------------------------------------------------------------

