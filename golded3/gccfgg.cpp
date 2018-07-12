
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
//  Main GOLDED.CFG compiler.
//  ------------------------------------------------------------------

#include <golded.h>
#include <gmoprot.h>
#include <gcharset.h>


//  ------------------------------------------------------------------

extern char v7nodeflags[16][9];
extern char v7modemtype[8][9];


//  ------------------------------------------------------------------

int getgroup(const char *key)
{
  if(*key == '#')
    return atoi(key+1)+0x8000u;
  else
    return (g_isupper(*key) ? *key : 0);
}


//  ------------------------------------------------------------------
//  Read GOLDED.CFG

bool ReadGoldedCfg(int& force) {

  gfile fp;

  CfgInit2();

  Path golded_cfg;
  strcpy(golded_cfg, AddPath(CFG->goldpath, CFG->goldcfg));
  if(ReadCfg(golded_cfg)) {

    // Unallocate echolist
    AFILE->echolist.FreeAll();

    // Set default paths

    if(*CFG->temppath == NUL)       strcpy(CFG->temppath,      CFG->goldpath);
    if(*CFG->areapath == NUL)       strcpy(CFG->areapath,      CFG->goldpath);
    if(*CFG->inboundpath == NUL)    strcpy(CFG->inboundpath,   CFG->goldpath);
    if(*CFG->soundpath == NUL)      strcpy(CFG->soundpath,     CFG->goldpath);
    if(*CFG->cookiepath == NUL)     strcpy(CFG->cookiepath,    CFG->goldpath);
    if(*CFG->templatepath == NUL)   strcpy(CFG->templatepath,  CFG->goldpath);

    if(*CFG->squishuserpath == NUL) {
      char* ptr = getenv("MAXIMUS");
      if(ptr != NULL) {
        CfgSquishuserpath(ptr);
      }
      else {
        ptr = getenv("SQUISH");
        if(ptr != NULL)
          CfgSquishuserpath(ptr);
        else
          CfgSquishuserpath(CFG->areapath);
      }
    }

    CfgHudsonpath(CFG->goldpath);
    if(*CFG->hudsonsyspath == NUL)
      strcpy(CFG->hudsonsyspath, CFG->hudsonpath);
    CfgGoldbasepath(CFG->hudsonpath);
    if(*CFG->goldbasesyspath == NUL)
      strcpy(CFG->goldbasesyspath, CFG->goldbasepath);
    CfgJampath(CFG->hudsonpath);

    MakePathname(CFG->goldcfg, CFG->goldpath, CFG->goldcfg);
    MakePathname(CFG->helpcfg.fn, CFG->goldpath, CFG->helpcfg.fn);
    MakePathname(CFG->helpged, CFG->goldpath, CFG->helpged);
    MakePathname(CFG->keyscfg, CFG->goldpath, CFG->keyscfg);
    MakePathname(CFG->langcfg, CFG->goldpath, CFG->langcfg);
    MakePathname(CFG->xlatged, CFG->goldpath, CFG->xlatged);

    MakePathname(CFG->confirmfile, CFG->goldpath, CFG->confirmfile);
    MakePathname(CFG->logfile, CFG->goldpath, CFG->logfile);
    MakePathname(CFG->userlistfile, CFG->nodepath, CFG->userlistfile);
    MakePathname(CFG->outputfile, CFG->goldpath, CFG->outputfile);
    MakePathname(CFG->inputfile, CFG->goldpath, CFG->inputfile);

    if(*CFG->seqdir)
      MakePathname(CFG->seqdir, CFG->goldpath, CFG->seqdir);

    if(*CFG->souptosslog)
      MakePathname(CFG->souptosslog, CFG->goldpath, CFG->souptosslog);

    std::vector<Tpl>::iterator t;
    for(t = CFG->tpl.begin(); t != CFG->tpl.end(); t++)
      MakePathname(t->file, CFG->templatepath, t->file);

    MakePathname(CFG->semaphore.importlist, CFG->areapath, CFG->semaphore.importlist);
    MakePathname(CFG->semaphore.exportlist, CFG->areapath, CFG->semaphore.exportlist);
    MakePathname(CFG->semaphore.echoscan,   CFG->areapath, CFG->semaphore.echoscan);
    MakePathname(CFG->semaphore.netscan,    CFG->areapath, CFG->semaphore.netscan);

    MakePathname(CFG->semaphore.scanall,    CFG->goldpath, CFG->semaphore.scanall);
    MakePathname(CFG->semaphore.scanthis,   CFG->goldpath, CFG->semaphore.scanthis);
    MakePathname(CFG->semaphore.scannetmail,CFG->goldpath, CFG->semaphore.scannetmail);
    MakePathname(CFG->semaphore.pmscanall,  CFG->goldpath, CFG->semaphore.pmscanall);
    MakePathname(CFG->semaphore.pmscanthis, CFG->goldpath, CFG->semaphore.pmscanthis);
    MakePathname(CFG->semaphore.pmscannetmail, CFG->goldpath, CFG->semaphore.pmscannetmail);

    MakePathname(CFG->semaphore.qwkimport,  CFG->goldpath, CFG->semaphore.qwkimport);
    MakePathname(CFG->semaphore.qwkexport,  CFG->goldpath, CFG->semaphore.qwkexport);
    MakePathname(CFG->semaphore.soupimport, CFG->goldpath, CFG->semaphore.soupimport);
    MakePathname(CFG->semaphore.soupexport, CFG->goldpath, CFG->semaphore.soupexport);
    MakePathname(CFG->semaphore.exitnow,    CFG->goldpath, CFG->semaphore.exitnow);

    if (strieql(CFG->semaphore.exportlist, AddPath(CFG->jampath, "echomail.jam")))
    {
      STD_PRINTNL("* Warning: SEMAPHORE EXPORTLIST must not be the same as ECHOMAIL.JAM!");
      SayBibi();
      cfgerrors++;
    }
  }

  // Set default xlatimport
  if(*CFG->xlatimport == NUL)
    strcpy(CFG->xlatimport, CFG->xlatlocalset);

  ReadXlatTables();

  // Free all mapfile name allocations
  std::vector<Map>::iterator xlt;
  for(xlt = CFG->xlatescset.begin(); xlt != CFG->xlatescset.end(); xlt++)
    throw_release(xlt->mapfile);
  for(xlt = CFG->xlatcharset.begin(); xlt != CFG->xlatcharset.end(); xlt++)
    throw_release(xlt->mapfile);

  return true;
}


//  ------------------------------------------------------------------
//  Write GOLDED.GED

void WriteGoldGed() {

  // Mark areas according to scan/pmscan in/excludes
  for(AL.item = AL.idx.begin(); AL.item != AL.idx.end(); AL.item++) {

    gstrarray::iterator i;

    // Check scan in/excludes
    for(i = CFG->areascan.begin(); i != CFG->areascan.end(); i++) {
      const char *current = i->c_str();
      if(((current[0] == '>') and ((*AL.item)->groupid() == getgroup(current+1))) or strwild((*AL.item)->echoid(), current)) {
        (*AL.item)->set_scan(true);
        break;
      }
    }
    for(i = CFG->areascanexcl.begin(); i != CFG->areascanexcl.end(); i++) {
      const char *current = i->c_str();
      if(((current[0] == '>') and ((*AL.item)->groupid() == getgroup(current+1))) or strwild((*AL.item)->echoid(), current)) {
        (*AL.item)->set_scanexcl(true);
        break;
      }
    }
    for(i = CFG->areascanincl.begin(); i != CFG->areascanincl.end(); i++) {
      const char *current = i->c_str();
      if(((current[0] == '>') and ((*AL.item)->groupid() == getgroup(current+1))) or strwild((*AL.item)->echoid(), current)) {
        (*AL.item)->set_scanincl(true);
        break;
      }
    }

    // Check pmscan in/excludes
    for(i = CFG->areapmscan.begin(); i != CFG->areapmscan.end(); i++) {
      const char *current = i->c_str();
      if(((current[0] == '>') and ((*AL.item)->groupid() == getgroup(current+1))) or strwild((*AL.item)->echoid(), current)) {
        (*AL.item)->set_pmscan(true);
        break;
      }
    }
    for(i = CFG->areapmscanexcl.begin(); i != CFG->areapmscanexcl.end(); i++) {
      const char *current = i->c_str();
      if(((current[0] == '>') and ((*AL.item)->groupid() == getgroup(current+1))) or strwild((*AL.item)->echoid(), current)) {
        (*AL.item)->set_pmscanexcl(true);
        break;
      }
    }
    for(i = CFG->areapmscanincl.begin(); i != CFG->areapmscanincl.end(); i++) {
      const char *current = i->c_str();
      if(((current[0] == '>') and ((*AL.item)->groupid() == getgroup(current+1))) or strwild((*AL.item)->echoid(), current)) {
        (*AL.item)->set_pmscanincl(true);
        break;
      }
    }
  }

  // Release lists memory
  CFG->areaexcl.clear();
  CFG->areaincl.clear();
  CFG->areaisemail.clear();
  CFG->areaisnews.clear();
  CFG->areascan.clear();
  CFG->areascanexcl.clear();
  CFG->areascanincl.clear();
  CFG->arearename.clear();
  CFG->areapmscan.clear();
  CFG->areapmscanexcl.clear();
  CFG->areapmscanincl.clear();
}


//  ------------------------------------------------------------------

#if !defined(__GOLD_GUI__)
static int EnterString(char* prompt, char* string, uint length)
{
  STD_PRINT("\r" << prompt << NL << "> " << std::flush);

  *string = NUL;
  char* ptr = string;
  uint pos = 0;
  gkey k;
  while(1) {
    k = kbxget();
    if(k == Key_BS) {
      if(pos) {
        STD_PRINT("\b \b" << std::flush);
        pos--;
        *(--ptr) = NUL;
      }
    }
    else if(k == Key_Esc) {
      STD_PRINT(NL);
      *string = NUL;
      return -1;
    }
    else if(k == Key_Ent) {
      STD_PRINT(NL);
      *ptr = NUL;
      break;
    }
    else {
      if(pos < length) {
        char c = (char)k;
        if(c) {
          STD_PRINT(c << std::flush);
          *ptr++ = c;
          pos++;
        }
      }
    }
  }
  if(*string == NUL)
    return 1;
  return 0;
}


//  ------------------------------------------------------------------

struct AF_entry {
  char* env;
  char* configname;
  char* name;
};

struct AF_entry idetect[] = {
  {    (char *)"DUTCHIE",  (char *)"dutchie.are", (char *)"Dutchie"      },
  {        (char *)"EZY", (char *)"constant.ezy", (char *)"Ezycom"       },
  {   (char *)"FASTECHO", (char *)"fastecho.cfg", (char *)"Fastecho"     },
  {    (char *)"FIDOPCB",  (char *)"fidopcb.cfg", (char *)"FidoPCB"      },
  {      (char *)"FMAIL",    (char *)"fmail.cfg", (char *)"FMail"        },
  {      (char *)"IMAIL",     (char *)"imail.cf", (char *)"IMAIL"        },
  {    (char *)"PCBOARD",  (char *)"pcboard.dat", (char *)"PCBoard"      },
  { (char *)"POPCMDLINE",   (char *)"portal.are", (char *)"Portal"       },
  {         (char *)"RA",   (char *)"config.pro", (char *)"ProBoard"     },
  {     (char *)"QFRONT",   (char *)"qechos.dat", (char *)"QFront"       },
  {     (char *)"RAECHO",    (char *)"areas.rae", (char *)"RA-ECHO"      },
  {         (char *)"RA",    (char *)"config.ra", (char *)"RemoteAccess" },
  {      (char *)"TIMED",    (char *)"timed.cfg", (char *)"timEd"        },
  {         (char *)"TM",       (char *)"tm.cfg", (char *)"Termail"      },
  {      (char *)"WMAIL",    (char *)"wmail.prm", (char *)"WMail"        },
  {         (char *)"XM",    (char *)"xmail.cfg", (char *)"XMail"        },
  {         (char *)"FD",  (char *)"areafile.fd", (char *)"TosScan"      },
  {         (char *)"GE",     (char *)"setup.ge", (char *)"GEcho"        },
  {         (char *)"FD",     (char *)"setup.fd", (char *)"FrontDoor"    },
  {         (char *)"FD",       (char *)"fd.sys", (char *)"FrontDoor"    },
  {       (char *)"OPUS",   (char *)"sysmsg.dat", (char *)"Opus"         },
  {       (char *)"LORA",   (char *)"sysmsg.dat", (char *)"LoraBBS"      },
  {    (char *)"LORABBS",   (char *)"sysmsg.dat", (char *)"LoraBBS"      },
  { (char *)"FIDOCONFIG",             (char *)"", (char *)"Fidoconfig"   },
  {         (char *)"DB",  (char *)"dbridge.prm", (char *)"D\'Bridge"    },
  {    (char *)"DBRIDGE",  (char *)"dbridge.prm", (char *)"D\'Bridge"    },
  {   (char *)"SUPERBBS",  (char *)"sconfig.bbs", (char *)"SuperBBS"     },
  {       (char *)"SBBS",  (char *)"sconfig.bbs", (char *)"SuperBBS"     },
  {   (char *)"QUICKBBS", (char *)"quickcfg.dat", (char *)"QuickBBS"     },
  {       (char *)"QBBS", (char *)"quickcfg.dat", (char *)"QuickBBS"     },
  {   (char *)"QUICKBBS",   (char *)"config.bbs", (char *)"QuickBBS"     },
  {       (char *)"QBBS",   (char *)"config.bbs", (char *)"QuickBBS"     },
  {     (char *)"SQUISH",   (char *)"squish.cfg", (char *)"Squish"       }
};


void InstallDetect(char* path)
{
  // Create GOLDED.BAK file if there is an existing GOLDED.CFG
  if(fexist(CFG->goldcfg)) {
    Path cmdlinecfgbak;
    replaceextension(cmdlinecfgbak, CFG->goldcfg, ".bak");
    if(fexist(cmdlinecfgbak))
      remove(cmdlinecfgbak);
    rename(CFG->goldcfg, cmdlinecfgbak);
    STD_PRINTNL("Warning: Existing config backed up to " << cmdlinecfgbak << "!!!");
  }

  STD_PRINTNL("Please wait while GoldED+ is detecting your software.");

  gfile fp(CFG->goldcfg, "wt");
  if (fp.isopen())
  {
    if (*path)
    {
      MapPath(PathCopy(CFG->areapath, path));
      fp.Printf("AREAPATH %s\n", path);
    }

    Path pth;
    char* ptr;
    bool gotareasbbs = false;
    bool gotsquish = false;
    bool detected = false;

    // simple detection
    for(uint i = 0; i < sizeof(idetect)/sizeof(struct AF_entry); i++) {
      strcpy(pth, CFG->areapath);
      ptr = getenv(idetect[i].env);
      if(ptr) {
        PathCopy(pth, ptr);
        // skip additional information
        ptr = strchr(pth, ' ');
        if(ptr)
          *ptr = NUL;
      }
      if (fexist(AddPath(pth, idetect[i].configname)))
      {
        fp.Printf("AREAFILE %s %s\n", idetect[i].name, pth);
        STD_PRINTNL("Found " << idetect[i].name << (ptr ? "." : " (unreliable)."));
        if(streql(idetect[i].name, "Squish"))
          gotsquish = true;
        detected = true;
      }
    }

    // Detect InterMail
    strcpy(pth, CFG->areapath);
    ptr = getenv("IM");
    if(ptr)
      PathCopy(pth, ptr);
    if (fexist(AddPath(pth, "im.exe")) or fexist(AddPath(pth, "intrecho.exe")))
    {
      fp.Printf("AREAFILE InterMail %s\n", pth);
      STD_PRINTNL("Found InterMail and/or InterEcho.");
      detected = true;
    }

    // Detect Maximus
    strcpy(pth, CFG->areapath);
    ptr = getenv("MAXIMUS");
    if(ptr) {
      if(is_dir(ptr))
        PathCopy(pth, ptr);
      else
        extractdirname(pth, pth);
    }
    if (fexist(AddPath(pth, "max.prm")))
    {
      fp.Printf("AREAFILE Maximus %s\n", pth);
      STD_PRINTNL("Found Maximus.");
      detected = true;
    }
    if (not gotsquish and fexist(AddPath(pth, "squish.cfg")))
    {
      fp.Printf("AREAFILE Squish %s\n", pth);
      STD_PRINTNL("Found Squish.");
      detected = true;
    }

    // Detect ME2
    strcpy(pth, CFG->areapath);
    if (fexist(AddPath(pth, "areadesc.me2")))
    {
      fp.Printf("AREAFILE ME2 %sareadesc.me2 %sareas.bbs\n", pth, pth);
      STD_PRINTNL("Found ME2.");
      gotareasbbs = true;
      detected = true;
    }

    // Detect AREAS.BBS
    if(not gotareasbbs) {
      strcpy(pth, CFG->areapath);
      if (fexist(AddPath(pth, "areas.bbs")))
      {
        fp.Printf("AREAFILE AreasBBS %sareas.bbs\n", pth);
        STD_PRINTNL("Found AREAS.BBS.");
        detected = true;
      }
    }

    if(not detected)
      STD_PRINTNL("Sorry, could not find any supported software. Try another path.");
  }
}


//  ------------------------------------------------------------------

int InstallFinish()
{
  gfile fp(CFG->goldcfg, "at");
  if (fp.isopen())
  {
    char buf[77];

    // Check what we have
    if (CFG->username.empty())
    {
      if (EnterString((char *)"Please enter your name:", buf, sizeof(buf)))
        return -1;
      fp.Printf("USERNAME %s\n", buf);
      CfgUsername(buf);
    }
    if (CFG->aka.empty())
    {
      if (EnterString((char *)"Please enter your FidoNet address:", buf, sizeof(buf)))
        return -1;
      fp.Printf("ADDRESS %s\n", buf);
      CfgAddress(buf);
    }
    if (AL.basetypes.empty())
    {
      if (EnterString((char *)"Please enter the path to your *.msg netmail area:", buf, sizeof(buf)))
        return -1;
      AreaCfg aa;
      aa.reset();
      aa.setdesc("Netmail");
      aa.setechoid("NETMAIL");
      aa.basetype = "OPUS";
      aa.type = GMB_NET;
      aa.setpath(buf);
      aa.attr = CFG->attribsnet;
      AL.AddNewArea(&aa);
      fp.Printf("AREADEF %s \"%s\" 0 Net %s %s\n", aa.desc, aa.echoid, aa.basetype.c_str(), buf);
      if (EnterString((char *)"Please enter the path *and* filename of your areas.bbs file:", buf, sizeof(buf)))
        return -1;
      fp.Printf("AREAFILE AreasBBS %s\n", buf);
      char buf2[200]="AreasBBS ";
      strcat(buf2, buf);
      AL.GetAreafile(buf2);
    }
    #ifndef GMB_NOHUDS
    if (find(AL.basetypes, "HUDSON") and (*CFG->hudsonpath == NUL))
    {
      if (EnterString((char *)"Please enter the path to your Hudson msgbase files:", buf, sizeof(buf)))
        return -1;
      fp.Printf("HUDSONPATH %s\n", buf);
      PathCopy(CFG->hudsonpath, buf);
    }
    #endif
    #ifndef GMB_NOGOLD
    if (find(AL.basetypes, "GOLDBASE") and (*CFG->goldbasepath == NUL))
    {
      if (EnterString((char *)"Please enter the path to your Goldbase msgbase files:", buf, sizeof(buf)))
        return -1;
      fp.Printf("GOLDBASEPATH %s\n", buf);
      PathCopy(CFG->goldbasepath, buf);
    }
    #endif
    #ifndef GMB_NOJAM
    if (find(AL.basetypes, "JAM") and (*CFG->jampath == NUL))
    {
      if (EnterString((char *)"Please enter the path where net/echomail.jam can be placed:", buf, sizeof(buf)))
        return -1;
      fp.Printf("JAMPATH %s\n", buf);
      PathCopy(CFG->jampath, buf);
    }
    #endif
    #ifndef GMB_NOPCB
    if (find(AL.basetypes, "PCBOARD") and (*CFG->pcboardpath == NUL))
    {
      if (EnterString((char *)"Please enter the path to PCBoard:", buf, sizeof(buf)))
        return -1;
      fp.Printf("PCBOARDPATH %s\n", buf);
      PathCopy(CFG->pcboardpath, buf);
    }
    #endif

    return 0;
  }
  return 0;
}
#endif


//  ------------------------------------------------------------------

GoldedCfgEdit::GoldedCfgEdit() {

  memset(&cfg, 0, sizeof(cfg));

  // Set Boolean values
  AutoAttach(true);
  AutoSave(30);
  ChangeDate(true);
  CharPara(' ');
  CharSpace(' ');
  FieldClear(true);
  HardLines(true);
  HdrNamePos(8);
  HdrNameLen(36);
  HdrNodePos(44);
  HdrNodeLen(36);
  HeaderAttrs(true);
  HeaderFirst(EDITHEADERFIRST_YES);
  Internal(true);
  Menu(true);
  QuoteMargin(75);
  SaveMenu(true);
  UnDelete(50);
  SoftCrXlat(0x8d);

  str.push_back(""); cfg.external = str.size()-1;
  str.push_back(GOLDED_MSG); cfg.file = str.size()-1;
  str.push_back("<<"); cfg.hardline = str.size()-1;
  str.push_back(""); cfg.spellchecker = str.size()-1;
}


//  ------------------------------------------------------------------

CfgGed::CfgGed() {

  // pathes
  strcpy(goldcfg, GEDCFG);
  strcpy(helpcfg.fn, GOLDHELP_CFG); helpcfg.ft = 0;
  replaceextension(helpged, helpcfg.fn, __gver_cfgext__);
  strcpy(keyscfg, GOLDKEYS_CFG);
  strcpy(langcfg, GOLDLANG_CFG);
  strcpy(goldlast, GOLDLAST_LST);
  strcpy(golduser, GOLDUSER_LST);
  strcpy(confirmfile, GOLDED_CFM);
  strxmerge(xlatged, sizeof(xlatged), GOLDXLAT, __gver_cfgext__, NULL);
  strcpy(fidolastread, FIDOLASTREAD);
  strcpy(logfile, GOLDED_LOG);
  strcpy(namesfile, NAMES_FD);
  strcpy(userlistfile, GOLDED_LST);
  *adeptxbbspath = 0;
  *areapath = 0;
  *attachpath = 0;
  *cookiepath = 0;
  *ezycom.msgbasepath = 0;
  *ezycom.userbasepath = 0;
  *fidouserlist = 0;
  *goldbasepath = 0;
  *goldbasesyspath = 0;
  *goldpath = 0;
  *hudsonpath = 0;
  *hudsonsyspath = 0;
  *inboundpath = 0;
  *inputfile = 0;
  *jampath = 0;
  *loadlanguage = 0;
  *nodepath = 0;
  *nodepathfd = 0;
  *nodepathv7 = 0;
  *outboundpath = 0;
  *outputfile = 0;
  *pathreportfile = 0;
  *pcboardpath = 0;
  *quotebuffile = 0;
  *seqdir = 0;
  *soundpath = 0;
  *soupexportpath = 0;
  *soupimportpath = 0;
  *soupnewsrcfile = 0;
  *soupreplylinker = 0;
  *souptosslog = 0;
  *squishuserpath = 0;
  *templatepath = 0;
  *temppath = 0;
  *uudecodepath = 0;
  *xlatpath = 0;

  // vectors
  // cmdkey.clear();
  // macro.clear();
  // addressmacro.clear();
  // aka.clear();
  // akamatch.clear();
  // areaexcl.clear();
  // areaincl.clear();
  // areaisemail.clear();
  // areaisnews.clear();
  // areapmscan.clear();
  // areapmscanexcl.clear();
  // areapmscanincl.clear();
  // arearename.clear();
  // areascan.clear();
  // areascanexcl.clear();
  // areascanincl.clear();
  // event.clear();
  // externutil.clear();
  // filealias.clear();
  // frqext.clear();
  // frqnodemap.clear();
  // kludge.clear();
  // mailinglist.clear();
  // mappath.clear();
  // origin.clear();
  // robotname.clear();
  // tagline.clear();
  // tpl.clear();
  // twitname.clear();
  // twitsubj.clear();
  // unpacker.clear();
  // username.clear();
  // wtpl.clear();
  // xlatcharset.clear();
  // xlatescset.clear();

  // echotags
  *areacfmreplyto = 0;
  *areacopyto = 0;
  *areafreqto = 0;
  *arearecyclebin = 0;
  *areareplyto = 0;
  *areastart = 0;
  *areayouwroteto = 0;
  *soupbadmsgs = 0;
  *soupemail = 0;
  *soupreplyto = 0;

  // strings
  strcpy(arealistformat, "AM D CPUN E G ");
  *arealistgrouporder = 0;
  strcpy(arealistsort, "FYTUE");
  strcpy(areascansort, "XZBE");
  strcpy(importbegin, "=== Cut ===");
  *importend = 0;
  *internetaddress = 0;
  *internetdomain = 0;
  *keybstack = 0;
  *nickname = 0;
  *netname = 0;
  *organization = 0;
  #ifdef __UNIX__
  strcpy(printdevice, "/usr/bin/lpr > /dev/null 2>&1");
  #else
  strcpy(printdevice, "PRN");
  #endif
  *printinit = 0;
  *printreset = 0;
  *quotechars = 0;
  strcpy(quotestring, " FL> ");
  strcpy(quotestops, "<\"\'-");
  *searchfor = 0;
  strcpy(stylecodepunct, " !?\"$%&()+,.:;<=>@[\\]^`{|}~\n\r\t");
  *stylecodestops = 0;
  gsprintf(PRINTF_DECLARE_BUFFER(tasktitle), "%s%s%s%s", __gver_prename__,
    __gver_name__, __gver_postname__, __gver_platform__);
  strcpy(tearline, "@longpid @version");
  strcpy(whoto, "All");
  strcpy(xlatlocalset, get_charset());
  strcpy(xlatimport, get_dos_charset(xlatlocalset));
  strcpy(xlatexport, xlatimport);
  // variables & switches
  adeptxbbsuserno = 0;
  addressbookadd = YES;
  addresslookupfirst = false;
  akamatchfromto = NO;
  akamatchmanually = false;
  areaautoid = AUTOID_LONG;
  areacopydirect = false;
  areacopyaddid = false;
  areaforwarddirect = false;
  areafilegroups = YES;
  areafreqdirect = false;
  arealistechomax = 0;
  arealisttype = AL_TOTNEW;
  areareadonly = READONLY_SOFT;
  arearecyclebinask = false;
  areareplydirect = false;
  beepfactor = 1000;
  beepyourmail = true;
  carboncopylist = CC_NAMES;
  cfgeditquotemargin = 75;
  confirmresponse = ASK;
  crosspost = ASK;
  crosspostlist = CC_NAMES;
  ctrlinfoecho = CI_TAGL|CI_TEAR|CI_ORIG;
  ctrlinfoemail = 0;
  ctrlinfolocal = CI_TAGL;
  ctrlinfonet = CI_TAGL|CI_TEAR;
  ctrlinfonews = 0;
  dispareano = true;
  dispattachsize = ATTACH_KBYTES;
  disphdrlocation = NO;
  displistcursor = NO;
  dispmargin = 0;
  dispmsgsize = DISPMSGSIZE_BYTES;
  disppmfirst = true;
  dispsoftcr = true;
  disptabsize = 8;
  encodeemailheaders = true;
  externoptions = EXTUTIL_CLS | EXTUTIL_CURSOR | EXTUTIL_RELOAD | EXTUTIL_PAUSEONERROR | EXTUTIL_KEEPCTRL;
  ezycomuserno = 0;
  fidomsgtype = "OPUS";
  fidouserno = 0;
  forcetemplate = false;
  frqoptions = FREQ_FROMTOP;
  peekurloptions = PEEK_FROMTOP;
  gedhandshake = true;
  goldbaseuserno = 0;
  hidestylies = false;
  highlighturls = true;
  hudsonsizewarn = 16000000L;
  hudsonuserno = 0;
  ignorecharset = false;
  intensecolors = false;
  transparentcolors = false;
  internetmsgid = false;
  internetrfcbody = 0;
  internetviagate = false;
  keybmode = KEYB_POLL;
  latin2local = false;
  logformat = 0;
  locationdelimiter = " \x1A ";
  menumarked = MODE_DONT;
  menunextarea = YES;
  msglistdate = MSGLISTDATE_WRITTEN;
  msglistfast = true;
  msglistfirst = false;
  msglistheader = false;
  msglistwidesubj = false;
  originno = 0;
  pcboarduserno = 0;
  personalmail = 0;
  printlength = 60;
  printmargin = 80;
  quotebufmode = QBUF_ASK;
  quotectrl = CI_TEAR|CI_ORIG;
  quotemargin = 70;
  quotewraphard = false;
  quoteusenewai = true;
  ra2usersbbs = 0;
  replylink = REPLYLINK_DIRECT;
  replylinkfloat = true;
  replylinklist = 0;
  replylinkshowalways = true;
#if defined(GCFG_SPELL_INCLUDED)
  scheckerenabled = NO;
  strcpy(scheckerdeflang, "65535");
  strcpy(scheckeruserdic, "user.dic");
  strcpy(scheckerdicpath, "dicts");
#endif
  screenblanker = 180;
  screenblankertype = BLANK_SLIDEWIN;
  screenmaxcol = 0;
  screenmaxrow = 0;
  screensize = 0;
  seqmsgid = MAYBE;
  seqoutrun = 0;
  sharemode = cmdlinesharemode;
  showdeleted = false;
  soupexportmargin = 76;
  squishscan = 2; // SQS_QUICK
  squishuserno = 0;
  statuslinehelp = 0;
  striphtml = false;
  taglineno = 0;
  taglinechar = '.';
  taglinesupport = true;
  titlestatus = true;
  tplno = 0;
  templatematch = false;
  timeout = 0;
  twitmode = TWIT_BLANK;
  usearea = true;
  usecharset = true;
  usefwd = NO;
  useintl = true;
  usepid = true;
  usernameno = 0;
  usesoftcrxlat = false;
  usestylies = true;
  usetzutc = true;
  viewhidden = false;
  viewkludge = false;
  viewquote = true;
  writeheader = 1;
  wildcatuserno = 0;
  zonegating = ASK;

  // classes
  attribsattach.loc1();
  attribscc.loc1();
  attribscfm.loc1();
  attribsecho.loc1();
  attribsemail.loc1();
  attribsfrq.loc1();
  attribslocal.loc1();
  attribsnet.loc1();
  attribsnews.loc1();
  switches.set(akamatchnet, true);
  switches.set(areaautonext, true);
  switches.set(areacatchupread, true);
  switches.set(areakeeplast, true);
  switches.set(arealistgroupid, true);
  switches.set(arealistpagebar, true);
  switches.set(askdelorig, false);
  switches.set(beepcomment, true);
  switches.set(beepnoises, true);
  switches.set(dispautonext, true);
  switches.set(displocalhigh, true);
  switches.set(disppagebar, true);
  switches.set(dispstatusline, true);
  switches.set(dosprompt, true);
  switches.set(filelistpagebar, true);
  switches.set(internetreply, true);
  switches.set(keybdefaults, true);
  switches.set(keybext, true);
  switches.set(kludgechrs, true);
  switches.set(lookupnet, true);
  switches.set(msglistpagebar, true);
  switches.set(msglistviewsubj, true);
  switches.set(nodelistwarn, true);
  switches.set(printformfeed, true);
  switches.set(screenshadows, true);
  switches.set(statuslineclock, true);
  switches.set(timeoutsavemsg, true);
  switches.set(useflags, true);
  switches.set(usemsgid, true);
  switches.set(rcvdisablescfm, true);

  // screen configuration
  disphdrdateset.len = 19;
  disphdrdateset.pos = -19;
  disphdrnameset.len = 36;
  disphdrnameset.pos = 8;
  disphdrnodeset.len = 16;
  disphdrnodeset.pos = 44;

  // unsorted structures
  areatypeorder[GMB_NET]                = 1;
  areatypeorder[GMB_NET|GMB_EMAIL]      = 2;
  areatypeorder[GMB_ECHO]               = 3;
  areatypeorder[GMB_ECHO|GMB_NEWSGROUP] = 4;
  areatypeorder[GMB_LOCAL]              = 5;
  if(gvid->adapter & V_MONO)
    memcpy(color, gold_mono1, sizeof(color));
  else
    memcpy(color, gold_color1, sizeof(color));
  *internetgate.name = 0;
  internetgateexp = (RFCName << 2) | RFCAddress;
  // internetgate.addr.reset();
  invalidate.origin.first = " * Origin: ";
  invalidate.origin.second = " + Origin: ";
  invalidate.seenby.first = "SEEN-BY";
  invalidate.seenby.second = "SEEN+BY";
  invalidate.tearline.first = "---";
  invalidate.tearline.second = "-+-";
  invalidate.cc.first = "CC:";
  invalidate.cc.second = "!CC:";
  invalidate.xc.first = "XC:";
  invalidate.xc.second = "!XC:";
  invalidate.xp.first = "XP:";
  invalidate.xp.second = "!XP:";
  play.type = 0;
  play.effectno = 0;
  *play.file = 0;
  for(int n=0; n<16; n++)
    screenpalette[n] = -1;
  screenpalette[16] = 0;   /* indicates empty array */
  *semaphore.netscan = 0;
  *semaphore.echoscan = 0;
  *semaphore.exportlist = 0;
  *semaphore.importlist = 0;
  *semaphore.scanall = 0;
  *semaphore.scanthis = 0;
  *semaphore.scannetmail = 0;
  *semaphore.pmscanall = 0;
  *semaphore.pmscanthis = 0;
  *semaphore.pmscannetmail = 0;
  *semaphore.qwkimport = 0;
  *semaphore.qwkexport = 0;
  *semaphore.soupimport = 0;
  *semaphore.soupexport = 0;
  *semaphore.exitnow = 0;
  semaphore.idletime = 0;
}


//  ------------------------------------------------------------------

CfgGed::~CfgGed() {

}


//  ------------------------------------------------------------------
