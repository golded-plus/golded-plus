
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
//  Common code for the area compile.
//  ------------------------------------------------------------------

#include <golded.h>
#include <gmoprot.h>


//  ------------------------------------------------------------------
//  Fix up the echoid and make default if none specified

void FixEchoid(char* echoid, int type) {

  static uint netnum = 1;
  static uint echonum = 1;
  static uint localnum = 1;

  if(*echoid == NUL) {
    const char* t = CFG->areaautoid == AUTOID_LONG ? "MAIL" : "";
    if(type & GMB_NET)
      sprintf(echoid, "NET%s%03u", t, netnum++);
    else if(type & GMB_ECHO)
      sprintf(echoid, "ECHO%s%03u", t, echonum++);
    else if(type & GMB_LOCAL)
      sprintf(echoid, "LOCAL%03u", localnum++);
  }
}


//  ------------------------------------------------------------------
//  Sort a group of areas

void AreaList::SortAreaGroup(const char* options, int beginarea, int endarea) {

  if(beginarea != endarea) {
    const char* ptr = striinc("-S", options);
    if(ptr) {
      ptr += 2;
      if(*ptr == '=')
        ptr++;
      Sort(ptr, beginarea, endarea);
    }
  }
}


//  ------------------------------------------------------------------
//  Rename an echoid

void RenameArea(char* echoid)
{
  std::vector<EchoRen>::iterator n = CFG->arearename.begin();
  std::vector<EchoRen>::iterator end = CFG->arearename.end();

  for (; n != end; n++)
  {
    if (strieql(echoid, n->from.c_str()))
    {
      strxcpy(echoid, n->to.c_str(), sizeof(Echo));
      break;
    }
  }
}


//  ------------------------------------------------------------------
//  Check if the echoid is on the exclusion list

bool CheckExcl(char* echoid)
{
  gstrarray::iterator n = CFG->areaexcl.begin();
  gstrarray::iterator end1 = CFG->areaexcl.end();

  for (; n != end1; n++)
  {
    if (strwild(echoid, n->c_str()))
    {
      // Found excl, now check for incl
      gstrarray::iterator x = CFG->areaincl.begin();
      gstrarray::iterator end2 = CFG->areaincl.end();

      for (; x != end2; x++)
      {
        if (strwild(echoid, x->c_str()))
          return false;
      }

      return true;   // Echoid is excluded
    }
  }

  return false;
}


//  ------------------------------------------------------------------
//  Check if the echo is email or news

void CheckEMailOrNews(char* echoid, uint& type)
{
  gstrarray::iterator i = CFG->areaisemail.begin();
  gstrarray::iterator end = CFG->areaisemail.end();

  for (; i != end; i++)
  {
    if (strwild(echoid, i->c_str()))
    {
      type = GMB_EMAIL | GMB_NET;
      break;
    }
  }

  i = CFG->areaisnews.begin();
  end = CFG->areaisnews.end();

  for(; i != end; i++)
  {
    if (strwild(echoid, i->c_str()))
    {
      type = GMB_NEWSGROUP | GMB_ECHO;
      break;
    }
  }
}


//  ------------------------------------------------------------------
//  Add or update area data

void AddNewArea(AreaCfg* aa) {

  AL.AddNewArea(aa);
}


//  ------------------------------------------------------------------
//  Add or update area data

void AddNewArea(AreaCfg& aa) {

  AL.AddNewArea(&aa);
}


//  ------------------------------------------------------------------
//  Add or update area data

void AreaList::AddNewArea(AreaCfg* aa)
{
  if (veryverbose)
  {
    std::string temp;
    STD_PRINT("  fmt=" << aa->basetype << ", eid=\"" << aa->echoid);
    STD_PRINT("\", pth=\"" << aa->path << "\", brd=" << aa->board);
    STD_PRINT(", gid=" << aa->groupid << ", aka=" << aa->aka.make_string(temp));
    STD_PRINTNL(" " << aa->attr.make_string(temp));
  }

  Desc desc;
  char buf[256];
  bool newarea = true;
  static int serial = 0;
  static int net1st = false;

  *desc = NUL;

  // Make sure the path field is 100% correct for the msgbase
  if ((aa->basetype == "FTS1") || (aa->basetype == "OPUS"))
  {
    if(*aa->path == NUL)
      return;
    MapPath(aa->path);
    AddBackslash(aa->path);
    strschg_environ(aa->path);
  }
  #ifndef GMB_NOHUDS
  else if (aa->basetype == "HUDSON")
  {
    if ((aa->board < 1) or (aa->board > 200))  // Ignore areas with invalid numbers
      return;
    gsprintf(PRINTF_DECLARE_BUFFER(aa->path), "%u", aa->board);
  }
  #endif
  #ifndef GMB_NOGOLD
  else if (aa->basetype == "GOLDBASE")
  {
    if ((aa->board < 1) or (aa->board > 500))  // Ignore areas with invalid numbers
      return;
    gsprintf(PRINTF_DECLARE_BUFFER(aa->path), "%u", aa->board);
  }
  #endif
  #ifndef GMB_NOEZY
  else if (aa->basetype == "EZYCOM")
  {
    // Ignore areas with invalid numbers
    if ((aa->board < 1) or (aa->board > 1536))
      return;
    gsprintf(PRINTF_DECLARE_BUFFER(aa->path), "%u", aa->board);
  }
  #endif
  #ifndef GMB_NOWCAT
  else if (aa->basetype == "WILDCAT")
  {
    if(*aa->path == NUL)
      return;
    MapPath(aa->path);
    StripBackslash(aa->path);
    strschg_environ(aa->path);
  }
  #endif
  #ifndef GMB_NOXBBS
  else if (aa->basetype == "ADEPTXBBS")
  {
    if(*aa->path == NUL)
      return;
    MapPath(aa->path);
    StripBackslash(aa->path);
    strschg_environ(aa->path);
  }
  #endif
  #ifndef GMB_NOSQSH
  else if (aa->basetype == "SQUISH")
  {
    if(*aa->path == NUL)
      return;
    MapPath(aa->path);
    StripBackslash(aa->path);
    strschg_environ(aa->path);
  }
  #endif
  #ifndef GMB_NOJAM
  else if (aa->basetype == "JAM")
  {
    if(*aa->path == NUL)
      return;
    MapPath(aa->path);
    StripBackslash(aa->path);
    strschg_environ(aa->path);
  }
  #endif
  #ifndef GMB_NOPCB
  else if (aa->basetype == "PCBOARD")
  {
    MapPath(aa->path);
    StripBackslash(aa->path);
    strschg_environ(aa->path);
  }
  #endif
  #ifndef GMB_NOSMB
  else if (aa->basetype == "SMB")
  {
    if(*aa->path == NUL)
      return;
    MapPath(aa->path);
    StripBackslash(aa->path);
    strschg_environ(aa->path);
  }
  #endif
  else if (aa->basetype == "SEPARATOR")
  {
  }
  else {
      return;
  }

  // Note msgbase type
  if(not find(basetypes, aa->basetype))
    basetypes.push_back(aa->basetype);

  // Things to do for real areas only
  if(not aa->isseparator()) {

    // If the echoid is missing, try to find it in the echolist
    if(strblank(aa->echoid))
      AFILE->echolist.FindEcho(aa->echoid, aa->path, aa->desc);

    // Fix up the echoid
    FixEchoid(aa->echoid, aa->type);

    // Rename the echoid if necessary
    RenameArea(aa->echoid);

    // Do not accept excluded areas
    if(CheckExcl(aa->echoid))
      return;

    // Fix description
    if(strblank(aa->desc))
      strxcpy(aa->desc, aa->echoid, sizeof(aa->desc));
  }

  // Check if it's email or news
  CheckEMailOrNews(aa->echoid, aa->type);

  // Give each area a unique serial number
  aa->areaid = serial++;

  // Check if we already have the area (dup echoid or path)
  int _currarea = 0;
  area_iterator ap = idx.begin();
  area_iterator end = idx.end();

  for (; ap != end; ap++)
  {
    ++_currarea;
    int eq_echoid  = strieql(aa->echoid, (*ap)->echoid());
    if (eq_echoid)
    {
      newarea = false;
      if (not (*ap)->isseparator())
      {
        if (strblank((*ap)->desc()))
          strxcpy(desc, aa->desc, sizeof(desc));
      }
      break;
    }
    else if(not (*ap)->isseparator())
    {
      bool eq_path    = strieql(aa->path, (*ap)->path());
      bool eq_board   = (aa->board == (*ap)->board());
      bool eq_msgbase = (aa->basetype == (*ap)->basetype());
      bool eq_isfido  = (((aa->basetype == "OPUS") || (aa->basetype == "FTS1")) &&
                         (((*ap)->basetype() == "OPUS") || ((*ap)->basetype() == "FTS1")));

      if (eq_path and eq_board and (eq_msgbase or eq_isfido))
      {
        // We had it already, so override with the new data
        newarea = false;
        if(strblank((*ap)->desc()))
          strxcpy(desc, aa->desc, sizeof(desc));
        break;
      }
    }
  }

  // If first netmail area, set default for AREAFREQTO and AREAREPLYTO
  if((aa->isnet() and not aa->isseparator()) and not net1st) {
    net1st = true;
    if(not *CFG->areafreqto)
      strcpy(CFG->areafreqto, aa->echoid);
    if(not *CFG->areareplyto)
      strcpy(CFG->areareplyto, aa->echoid);
  }

  // Increase area index size if new area
  if(newarea) {
    idx.push_back(NewArea(aa->basetype));
    throw_new(idx[_currarea]);
    ap = idx.end(); --ap;
  }

  // Add aka if not found
  if (aa->aka.net)
  {
    bool found = false;
    std::vector<gaka>::iterator i = CFG->aka.begin();
    std::vector<gaka>::iterator end = CFG->aka.end();

    for (; i != end; i++)
    {
      if (aa->aka == i->addr)
      {
        found = true;
        break;
      }
    }

    if (not found)     // Then add it
      CfgAddress(aa->aka.make_string(buf));  // Add the aka
  }
  else {
    // If address is missing, use main address
    if(not CFG->aka.empty())
      aa->aka = CFG->aka[0].addr;
  }

  // Copy the data
  if(newarea) {
    (*ap)->set_echoid(aa->echoid);
    (*ap)->set_path(aa->path);
    (*ap)->set_board(aa->board);
    (*ap)->set_basetype(aa->basetype);
  }
  (*ap)->set_desc(newarea or strblank(desc) ? aa->desc : desc);
  (*ap)->set_areaid(aa->areaid);
  (*ap)->set_groupid(aa->groupid);
  (*ap)->set_type(aa->type);
  (*ap)->set_aka(aa->aka);
  (*ap)->set_originno(aa->originno);
  (*ap)->set_attr(aa->attr);
  (*ap)->set_scan(aa->scan);
  (*ap)->set_scanexcl(aa->scanexcl);
  (*ap)->set_scanincl(aa->scanincl);
  (*ap)->set_pmscan(aa->pmscan);
  (*ap)->set_pmscanexcl(aa->pmscanexcl);
  (*ap)->set_pmscanincl(aa->pmscanincl);
}


//  ------------------------------------------------------------------
//  Get areas from other programs

void AreaList::GetAreafile(char* value) {

  const word CRC_ECHOLIST = 0xCE70;

  word crcval;
  char* keyword;
  char* options;
  int beginarea, endarea;

  crcval = getkeyvalcrc(&keyword, &value);
  options = throw_strdup(value);

  beginarea = idx.size();

  if(crcval == CRC_ECHOLIST)
    ReadEcholist(value);
  else {
    AFILE->quiet = quiet;
    AFILE->sharemode = CFG->sharemode;
    AFILE->fidomsgtype = CFG->fidomsgtype;
    AFILE->ra2usersbbs = CFG->ra2usersbbs;
    AFILE->squishuserno = CFG->squishuserno;
    AFILE->areapath = CFG->areapath;
    AFILE->pcboardpath = CFG->pcboardpath;

    if(not CFG->aka.empty())
      AFILE->primary_aka = CFG->aka[0].addr;
    else
      AFILE->primary_aka = 0;

    AFILE->attribsnet = CFG->attribsnet;
    AFILE->attribsecho = CFG->attribsecho;
    AFILE->attribsnews = CFG->attribsnews;
    AFILE->attribsemail = CFG->attribsemail;
    AFILE->attribslocal = CFG->attribslocal;

    AFILE->ReadAreafile(crcval, value);

    CFG->ra2usersbbs = AFILE->ra2usersbbs;
    CFG->squishuserno = AFILE->squishuserno;
  }

  endarea = idx.size();
  SortAreaGroup(options, beginarea, endarea);

  throw_free(options);
}


//  ------------------------------------------------------------------
//  Get area definition

void AreaList::GetArea(char* def) {

  AreaCfg aa;
  char* ptr;
  char* tag;
  char* desc;
  char* base;
  char* loc;
  char* attr;

  if(strnieql(def, "-FILE ", 6))  // Check for alternate formats
    GetAreafile(def+6);
  else {
    tag = def;
    desc = strskip_txt(tag);
    *desc = NUL;

    aa.reset();

    // Split up the string definition
    desc = strskip_wht(desc+1);
    switch(*desc) {
      case '\"':
        base = strskip_to(desc+1, '\"')+1;
        break;
      case '\'':
        base = strskip_to(desc+1, '\'')+1;
        break;
      default:
        base = strskip_txt(desc);
    }
    *base = NUL;
    base = strskip_wht(base+1);
    loc = strskip_txt(base);
    *loc = NUL;
    loc = strskip_wht(loc+1);
    attr = strpbrk(loc, " \t");
    if(attr and *attr) {
      *attr = NUL;
      attr = strskip_wht(attr+1);
    }

    // Parse the individual parts

    aa.setechoid(tag);              // Copy the echotag

    StripQuotes(desc);              // Copy description, stripping quotes ("")
    aa.setdesc(desc);

    switch(g_toupper(*base)) {        // Store area info

      case '-':
        aa.basetype = "SEPARATOR";
        break;

      case 'M':
        aa.basetype = "SQUISH";
        aa.setpath(loc);
        break;

      case 'Y':
        aa.basetype = "SMB";
        aa.setpath(loc);
        break;

      case 'H':
      case 'R':
      case 'Q':
        aa.basetype = "HUDSON";
        aa.board = (uint) atoi(loc);
        break;

      case 'G':
        aa.basetype = "GOLDBASE";
        aa.board = (uint) atoi(loc);
        break;

      case 'E':
        aa.basetype = "EZYCOM";
        aa.board = (uint) atoi(loc);
        break;

      case 'J':
        aa.basetype = "JAM";
        aa.setpath(loc);
        break;

      case 'P':
        aa.basetype = "PCBOARD";
        aa.setpath(loc);
        break;

      case 'W':
        aa.basetype = "WILDCAT";
        aa.setpath(loc);
        break;

      case 'X':
        aa.basetype = "ADEPTXBBS";
        aa.setpath(loc);
        break;

      case 'S':
        aa.basetype = "FTS1";
        aa.setpath(loc);
        break;

      case 'F':
      case 'O':
      default:
        aa.basetype = "OPUS";
        aa.setpath(loc);
    }

    aa.type = GMB_ECHO;
    aa.attr = CFG->attribsecho;
    strupr(base);
    if(strchr(base, 'N')) {
      aa.type = GMB_NET;
      aa.attr = CFG->attribsnet;
    }
    if(strchr(base, 'L')) {
      aa.type = GMB_LOCAL;
      aa.attr = CFG->attribslocal;
    }
    if(strchr(base, 'E')) {
      aa.type = GMB_ECHO;
      aa.attr = CFG->attribsecho;
    }
    if(attr) {                      // Get attributes and AKAs
      GetAttribstr(&aa.attr, attr);
      if((ptr = strpbrk(attr, "0123456789")) != NULL) {
        int n = atoi(ptr);
        if((n < 0) or ((uint) n >= CFG->aka.size()))
          n = 0;
        if(not CFG->aka.empty())
          aa.aka = CFG->aka[n].addr;   // Found AKA number
      }
    }

    if(not aa.aka.net and not CFG->aka.empty())
      aa.aka = CFG->aka[0].addr;

    AddNewArea(&aa);
  }
}


//  ------------------------------------------------------------------
//  Get area definition (new)
//
//  Syntax of AREADEF keyword:
//
//    AREADEF <echoid> <"desc"> <group> <type> <msgbase> <path/board> <aka> <(attrs)> ["origin"]
//
//  Examples:
//
//    AREADEF NET.ALL  "Netmail, Line 1" N Net   Opus   R:\NETMAIL\             2:231/77 (PVT K/S)
//    AREADEF TEST     "Testing..."      D Echo  Hudson 67                      .        ()
//    AREADEF 1LOCAL   "Sysop <-> Users" L Local Squish R:\MAX\MSG\LOC\1LOCAL   .        (PVT)     "Your Sysop * The Goldware BBS Line 1"
//    AREADEF DANEINFO "DaneNet Info"    D Echo  Squish R:\MAX\MSG\231\DANEINFO .        (R/O)

bool AreaList::GetAreaFirstPart(AreaCfg& aa, char*& key, char*& val) {

  const word CRC_NET      = 0xEC5E;
  const word CRC_ECHO     = 0xC2D1;
  const word CRC_LOCAL    = 0x4CD5;
  const word CRC_EMAIL    = 0x9C64;
  const word CRC_NEWS     = 0x61F1;

  // Get echoid
  getkeyval(&key, &val);
  aa.setechoid(key);

  // Get description
  getkeyval(&key, &val);
  aa.setdesc(key);

  // Get group (letter)
  getkeyval(&key, &val);
  aa.groupid = getgroup(key);

  // Get type
  word crc = getkeyvalcrc(&key, &val);
  switch(crc) {
    case CRC_NET:
      aa.type = GMB_NET;
      aa.attr = CFG->attribsnet;
      break;
    case CRC_ECHO:
      aa.type = GMB_ECHO;
      aa.attr = CFG->attribsecho;
      break;
    case CRC_LOCAL:
      aa.type = GMB_LOCAL;
      aa.attr = CFG->attribslocal;
      break;
    case CRC_EMAIL:
      aa.type = GMB_EMAIL | GMB_NET;
      aa.attr = CFG->attribsemail;
      break;
    case CRC_NEWS:
      aa.type = GMB_NEWSGROUP | GMB_ECHO;
      aa.attr = CFG->attribsnews;
      break;
    default:
      return false;
  }

  return true;
}


//  ------------------------------------------------------------------

void AreaList::GetAreaSep(char* val) {

  char* key;
  AreaCfg aa;

  aa.reset();
  aa.basetype = "SEPARATOR";

  if(not GetAreaFirstPart(aa, key, val))
    return;

  // Get aka
  getkeyval(&key, &val);
  if(not CFG->aka.empty())               // Seed aka with main address
    aa.aka = CFG->aka[0].addr;

  aa.aka.set(key);

  AddNewArea(&aa);
}


//  ------------------------------------------------------------------

void AreaList::GetAreaDef(char* val) {

  const word CRC_SEP      = 0x2FC6;
  const word CRC_FIDO     = 0x3AC8;
  const word CRC_OPUS     = 0x1254;
  const word CRC_SDMSG    = 0xAEC0;
  const word CRC_FTS1     = 0xEE2A;
  const word CRC_FTSC     = 0xEE58;
  const word CRC_QBBS     = 0x175B;
  const word CRC_HUDSON   = 0xBAB1;
  const word CRC_SQUISH   = 0xFCF6;
  const word CRC_EZYCOM   = 0xC81B;
  const word CRC_JAM      = 0xA8C3;
  const word CRC_GOLD     = 0x6134;
  const word CRC_GOLDBASE = 0x560D;
  const word CRC_PCB      = 0x19B7;
  const word CRC_PCBOARD  = 0x84EC;
  const word CRC_WCAT     = 0xAEDB;
  const word CRC_WILDCAT  = 0x7F3A;
  const word CRC_XBBS     = 0xADC3;
  const word CRC_SMB      = 0x27D4;

  char* key;
  AreaCfg aa;

  aa.reset();

  if(not GetAreaFirstPart(aa, key, val))
    return;

  // Get format
  word crc = getkeyvalcrc(&key, &val);
  getkeyval(&key, &val);        // Get path/board
  switch(crc) {
    case CRC_SEP:
      aa.basetype = "SEPARATOR";
      break;
    case CRC_FIDO:
    case CRC_OPUS:
    case CRC_SDMSG:
      aa.basetype = "OPUS";
      aa.setpath(key);
      break;
    case CRC_FTS1:
    case CRC_FTSC:
      aa.basetype = "FTS1";
      aa.setpath(key);
      break;
    case CRC_QBBS:
    case CRC_HUDSON:
      aa.basetype = "HUDSON";
      aa.board = (uint) atoi(key);
      if((aa.board < 1) or (aa.board > 200)) {
        return;
      }
      break;
    case CRC_GOLD:
    case CRC_GOLDBASE:
      aa.basetype = "GOLDBASE";
      aa.board = atoi(key);
      if((aa.board < 1) or (aa.board > 500)) {
        return;
      }
      break;
    case CRC_SQUISH:
      aa.basetype = "SQUISH";
      aa.setpath(key);
      break;
    case CRC_SMB:
      aa.basetype = "SMB";
      aa.setpath(key);
      break;
    case CRC_EZYCOM:
      aa.basetype = "EZYCOM";
      aa.board = atoi(key);
      break;
    case CRC_JAM:
      aa.basetype = "JAM";
      aa.setpath(key);
      break;
    case CRC_PCB:
    case CRC_PCBOARD:
      aa.basetype = "PCBOARD";
      aa.setpath(key);
      break;
    case CRC_WCAT:
    case CRC_WILDCAT:
      aa.basetype = "WILDCAT";
      aa.setpath(key);
      break;
    case CRC_XBBS:
      aa.basetype = "ADEPTXBBS";
      aa.setpath(key);
      break;
    default:
      return;
  }

  // Get aka
  getkeyval(&key, &val);
  if(not CFG->aka.empty())               // Seed aka with main address
    aa.aka = CFG->aka[0].addr;

  aa.aka.set(key);

  // Get attributes
  getkeyval(&key, &val);
  GetAttribstr(&aa.attr, key);

  // Get origin (optional)
  getkeyval(&key, &val);
  aa.setorigin(key);

  // Add to arealist
  AddNewArea(&aa);
}


//  ------------------------------------------------------------------

void ReadEcholist(char* val) {

  AL.ReadEcholist(val);
}


//  ------------------------------------------------------------------

void getdztoken(char** key, char** val) {

  char* p = *val;

  *key = p;
  p = strskip_to(p, ',');
  if(*p == ',')
    *p++ = NUL;
  *val = p;
}


//  ------------------------------------------------------------------

void AreaList::ReadEcholist(char* val)
{
  char* key;
  Path file;
  char buf[256], options[80];
  bool is_sqafix = false;
  bool is_dz = false;

  // Get filename or options
  *file = NUL;
  *options = NUL;
  while(*val) {
    getkeyval(&key, &val);
    if(*key == '-') {
      // Get option
      if(strieql(key+1, "SQAFIX"))
        is_sqafix = true;
      else if(strieql(key+1, "DZ"))
        is_dz = true;
      else {
        strcat(options, key);
        strcat(options, " ");
      }
      continue;
    }
    strcpy(file, key);
  }

  if (*file)
  {
    gfile fp(file, "rt", CFG->sharemode);
    if (fp.isopen())
    {
      void (*tok)(char**, char**);

      if(is_dz)
        tok = getdztoken;
      else
        tok = getkeyval;

      if (not quiet)
        STD_PRINTNL("* Reading " << file);

      while (fp.Fgets((val=buf), sizeof(buf)))
      {
        // Get echoid
        if(is_sqafix) {
          tok(&key, &val);
          if(not strieql(key, "EchoArea"))
            continue;
        }
        else if(is_dz) {
          if(not g_isalpha(*val) and (*val != ','))
            continue;
          tok(&key, &val);
        }
        tok(&key, &val);
        if(*key and (*key != ';') and (*key != '%')) {
          if(is_sqafix) {
            char *grp, *desc;
            tok(&grp, &val);
            if((desc = strchr(val, '\"')) != NULL) {
              char *p = strchr(++desc, '\"');
              if(p)
                *p = NUL;
              else
                desc = NULL;
            }

            area_iterator ap = idx.begin();
            area_iterator end = idx.end();

            for (; ap != end; ap++)
            {
              if (strieql(key, (*ap)->echoid()))
              {
                (*ap)->set_groupid(g_toupper(*grp));
                if (desc) (*ap)->set_desc(desc);
                break;
              }
            }
          }
          else {
            char *desc;
            if(is_dz)
              tok(&desc, &val);
            else
              desc = val;

            area_iterator ap = idx.begin();
            area_iterator end = idx.end();

            for (; ap != end; ap++)
            {
              if (strieql(key, (*ap)->echoid()))
              {
                (*ap)->set_desc(desc);
                break;
              }
            }
          }
        }
      }
    }
  }
}


//  ------------------------------------------------------------------

void AreaList::GetAreaDesc(char* val) {

  char* key;
  Area* aa = NULL;

  // Get echoid and find area
  getkeyval(&key, &val);

  area_iterator ap = idx.begin();
  area_iterator end = idx.end();

  for (; ap != end; ap++)
  {
    if (strieql(key, (*ap)->echoid()))
    {
      aa = *ap;
      break;
    }
  }

  if (aa == NULL)   // Not found, ignore
    return;

  // Get description
  getkeyval(&key, &val);
  aa->set_desc(key);

  // Get group letter (optional)
  getkeyval(&key, &val);
  if(*key) {
    if(*key != '-') {
      aa->set_groupid(getgroup(key));
    }

    // Get aka (optional)
    getkeyval(&key, &val);
    if(*key) {

      if(*key != '-') {
        ftn_addr aka;
        if(not CFG->aka.empty())         // Seed aka with main address
          aka = CFG->aka[0].addr;

        aka.set(key);
        aa->set_aka(aka);
      }

      // Get attributes (optional)
      getkeyval(&key, &val);
      if(*key) {

        if(*key != '-') {
          Attr attr;
          GetAttribstr(&attr, key);
          aa->set_attr(attr);
        }

        // Get origin (optional)
        getkeyval(&key, &val);
        if(*key)
          aa->set_origin(key);
      }
    }
  }
}


//  ------------------------------------------------------------------

Area::Area(gmo_area* a) {

  throw_new(a);

  area = a;
  adat = NULL;
  bookmark = 0;
  marks = 0;
  unread = 0;
  isvalidchg = false;
  isunreadchg = false;
  isreadmark = false;
  isreadpm = false;
  isscanned = false;
  ispmscanned = false;
  istossed = false;
  findfirst = true;
}


//  ------------------------------------------------------------------

Area::~Area() {

  if(isopen())
    Close();

  throw_delete(area);
}


//  ------------------------------------------------------------------

char* MapPath(char* map, bool reverse)
{
  Path buf, cmap;

  strxcpy(cmap, map, sizeof(Path));
  if(reverse)
    strchg(cmap, GOLD_WRONG_SLASH_CHR, GOLD_SLASH_CHR);

  std::vector< std::pair<std::string, std::string> >::iterator i = CFG->mappath.begin();
  std::vector< std::pair<std::string, std::string> >::iterator end = CFG->mappath.end();

  for (; i != end; i++)
  {
    const char* p = reverse ? i->second.c_str() : i->first.c_str();
    const char* q = reverse ? i->first.c_str() : i->second.c_str();
    if(strnieql(cmap, p, strlen(p))) {
      strxcpy(buf, map, sizeof(Path));
      strxmerge(map, sizeof(Path), q, buf+strlen(p), NULL);
      char sl1, sl2;
      char* ptr;

      ptr = strpbrk((char *)p, "/\\");
      sl1 = ptr ? *ptr : NUL;
      ptr = strpbrk((char *)q, "/\\");
      sl2 = ptr ? *ptr : NUL;

      if(sl1 and sl2 and (sl1 != sl2))
        strchg(map, sl1, sl2);

      break;
    }
  }
  return map;
}


//  ------------------------------------------------------------------
//  Set area origin

int AreaCfgBase::setorigin(std::string& _origin)
{
  if (not strblank(_origin.c_str()))
  {
    // Check if it already exists
    gstrarray::iterator n = CFG->origin.begin();
    gstrarray::iterator end = CFG->origin.end();

    for (originno = 0; n != end; originno++, n++)
    {
      if (*n == _origin)
        return originno;
    }

    // Not found, so add it
    CfgOrigin(_origin.c_str());
    // originno = CFG->origin.size()-1;
  }

  return originno;
}


//  ------------------------------------------------------------------
//  Set area echoid

const char* AreaCfg::setautoid(const char* _echoid) {

  return strxcpy(echoid, CFG->areaautoid ? "" : _echoid, sizeof(Echo));
}


//  ------------------------------------------------------------------
