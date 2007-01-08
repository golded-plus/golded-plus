
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
//  Configuration functions, part 4.
//  ------------------------------------------------------------------

#include <golded.h>
#include <gcprot.h>


//  ------------------------------------------------------------------

extern char* val;


//  ------------------------------------------------------------------

void CfgExcludenodes() {

  // Only used by GoldNODE
}

//  ------------------------------------------------------------------

void CfgExternoptions() {

  const word CRC_CLS            = 0x34F4;
  const word CRC_CURSOR         = 0x0D67;
  const word CRC_RELOAD         = 0xDEC0;
  const word CRC_PAUSE          = 0x3926;
  const word CRC_PAUSEONERROR   = 0xD99A;
  const word CRC_WIPE           = 0x1E80;
  const word CRC_KEEPCTRL       = 0x0738;
  const word CRC_NOCLS          = 0x2FAA;
  const word CRC_NOCURSOR       = 0x9345;
  const word CRC_NORELOAD       = 0x40E2;
  const word CRC_NOPAUSE        = 0x5D94;
  const word CRC_NOPAUSEONERROR = 0xDBAA;
  const word CRC_NOWIPE         = 0xE3DA;
  const word CRC_NOKEEPCTRL     = 0x38FC;

  // Handle options
  while(*val) {

    if(*val == '-')
      val++;

    char* _key;
    word _crc = getkeyvalcrc(&_key, &val);

    switch(_crc) {

      case CRC_CLS:
        CFG->externoptions |= EXTUTIL_CLS;
        break;
      case CRC_CURSOR:
        CFG->externoptions |= EXTUTIL_CURSOR;
        break;
      case CRC_RELOAD:
        CFG->externoptions |= EXTUTIL_RELOAD;
        break;
      case CRC_PAUSE:
        CFG->externoptions |= EXTUTIL_PAUSE;
        break;
      case CRC_PAUSEONERROR:
        CFG->externoptions |= EXTUTIL_PAUSEONERROR;
        break;
      case CRC_WIPE:
        CFG->externoptions |= EXTUTIL_WIPE;
        break;
      case CRC_KEEPCTRL:
        CFG->externoptions |= EXTUTIL_KEEPCTRL;
        break;
      case CRC_NOCLS:
        CFG->externoptions &= ~EXTUTIL_CLS;
        break;
      case CRC_NOCURSOR:
        CFG->externoptions &= ~EXTUTIL_CURSOR;
        break;
      case CRC_NORELOAD:
        CFG->externoptions &= ~EXTUTIL_RELOAD;
        break;
      case CRC_NOPAUSE:
        CFG->externoptions &= ~EXTUTIL_PAUSE;
        break;
      case CRC_NOPAUSEONERROR:
        CFG->externoptions &= ~EXTUTIL_PAUSEONERROR;
        break;
      case CRC_NOWIPE:
        CFG->externoptions &= ~EXTUTIL_WIPE;
        break;
      case CRC_NOKEEPCTRL:
        CFG->externoptions &= ~EXTUTIL_KEEPCTRL;
        break;
    }
  }
}


//  ------------------------------------------------------------------

void CfgExternutil()
{
  ExtUtil extutil;

  // Get util number
  char* _key;
  char* _val = val;
  getkeyval(&_key, &_val);
  extutil.utilno = atoi(_key);

  // Get options
  int _optbak = CFG->externoptions;

  while(strchr("-", *_val))
  {
    getkeyval(&_key, &_val);
    val = _key;
    CfgExternoptions();
  }

  extutil.options = CFG->externoptions;
  CFG->externoptions = _optbak;

  extutil.cmdline = _val;  // Get commandline

  // Count it
  CFG->externutil.push_back(extutil);
}

//  ------------------------------------------------------------------

void CfgEzycommsgbase(const char *path, bool force) {

  if(force or strblank(CFG->ezycom.msgbasepath))
    MapPath(PathCopy(CFG->ezycom.msgbasepath, path));
}

void CfgEzycommsgbase() {

  CfgEzycommsgbase(val, true);
}

//  ------------------------------------------------------------------

void CfgEzycomuserbase(const char *path, bool force) {

  if(force or strblank(CFG->ezycom.userbasepath))
    MapPath(PathCopy(CFG->ezycom.userbasepath, path));
}

void CfgEzycomuserbase() {

  CfgEzycomuserbase(val, true);
}

//  ------------------------------------------------------------------

void CfgEzycomuserno() {

  CFG->ezycomuserno = atoi(val);
}

//  ------------------------------------------------------------------

void CfgFidolastread(const char *path) {

  MapPath(strxcpy(CFG->fidolastread, val, sizeof(Path)));
}

void CfgFidolastread() {

  CfgFidolastread(val);
}

//  ------------------------------------------------------------------

void CfgFidomsgtype() {

  if(striinc("FTS", val))
    CFG->fidomsgtype = "FTS1";
  else
    CFG->fidomsgtype = "OPUS";
}

//  ------------------------------------------------------------------

void CfgFidouserlist() {

  strcpy(CFG->fidouserlist, val);
}

//  ------------------------------------------------------------------

void CfgFidouserno() {

  CFG->fidouserno = atoi(val);
}

//  ------------------------------------------------------------------

void CfgFilealias() {

  char* ptr = strskip_txt(val);
  if(*ptr) {
    *ptr++ = NUL;
    ptr = strskip_wht(ptr);
    FileAlias tmp;
    strxcpy(tmp.alias, val, sizeof(tmp.alias));
    strxcpy(tmp.file, ptr, sizeof(Path));
    CFG->filealias.push_back(tmp);
  }
}

//  ------------------------------------------------------------------

void CfgForcetemplate() {

  bool flag = make_bool(GetYesno(val));
  if(cfgingroup)
    CFG->grp.AddItm(GRP_FORCETEMPLATE, flag);
  else
    CFG->forcetemplate = flag;
}

//  ------------------------------------------------------------------

void CfgFrqext(const char* v)  {

  char* ptr = val = throw_strdup(v);
  CfgFrqext();
  throw_free(ptr);
}

void CfgFrqext() {

  tokenize(CFG->frqext, val);
}

//  ------------------------------------------------------------------

void CfgFrqnodemap() {

  char* key;
  FrqNodeMap fnm;
  getkeyval(&key, &val);
  fnm.to.reset(key);
  getkeyval(&key, &val);
  fnm.from.reset(key);

  CFG->frqnodemap.push_back(fnm);
}

//  ------------------------------------------------------------------

void CfgFrqoptions() {

  if(striinc("SORT", val))
    CFG->frqoptions |= FREQ_SORT;
  if(striinc("FROMTOP", val))
    CFG->frqoptions |= FREQ_FROMTOP;
  if(striinc("NOFILES", val))
    CFG->frqoptions |= FREQ_NOFILES;
  if(striinc("FAST", val))
    CFG->frqoptions |= FREQ_FAST;
  if(striinc("NOWAZOOMSG", val))
    CFG->frqoptions |= FREQ_NOWAZOOMSG;
  if(striinc("NOTFROMTOP", val))
    CFG->frqoptions &= ~FREQ_FROMTOP;
}

//  ------------------------------------------------------------------

void CfgGedhandshake() {

  CFG->gedhandshake = make_bool(GetYesno(val));
}

//  ------------------------------------------------------------------

void CfgGermankeyboard() {

  right_alt_same_as_left = not GetYesno(val);
}

//  ------------------------------------------------------------------

void CfgGoldbasepath(const char *path, bool force) {

  if(force or strblank(CFG->goldbasepath))
    MapPath(PathCopy(CFG->goldbasepath, path));
}

void CfgGoldbasepath() {

  CfgGoldbasepath(val, true);
}

//  ------------------------------------------------------------------

void CfgGoldbasesyspath() {

  PathCopy(CFG->goldbasesyspath, val);
}

//  ------------------------------------------------------------------

void CfgGoldbaseuserno() {

  CFG->goldbaseuserno = atoi(val);
}

//  ------------------------------------------------------------------

void CfgGoldhelp() {

  MapPath(strxcpy(CFG->helpcfg.fn, val, sizeof(Path)));
  replaceextension(CFG->helpged, CFG->helpcfg.fn, __gver_cfgext__);
}

//  ------------------------------------------------------------------

void CfgGoldlang() {

  MapPath(strxcpy(CFG->langcfg, val, sizeof(Path)));
}

//  ------------------------------------------------------------------

void CfgGoldpath() {

  MapPath(PathCopy(CFG->goldpath, val));
}

//  ------------------------------------------------------------------

void CfgGoldxlat() {

  MapPath(val);
  replaceextension(CFG->xlatged, val, __gver_cfgext__);
}

//  ------------------------------------------------------------------

void CfgGroup() {

  char* _key;
  getkeyval(&_key, &val);
  CFG->grp.AddGrp(_key);
  cfgingroup = true;
}

//  ------------------------------------------------------------------
