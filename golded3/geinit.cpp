
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
//  System initializing.
//  ------------------------------------------------------------------

#include <clocale>
#include <golded.h>
#include <gutlos.h>
#include <gmoprot.h>
#include <gdirposx.h>
#ifdef __WIN32__
#include <windows.h>
#endif

//  ------------------------------------------------------------------
//  Handle commandline parameters

#if defined(GFTRK_ENABLE)
static int gftrk_set_max = -1;
extern int __gftrk_statusline;
#endif

#ifdef __OS2__
extern bool gmem_check_overrun;
#endif

extern bool gKeystacking;

std::string keybuf;

static Path cmdlinecfg        = "";
       bool cmdlinedebughg    = false;
static int  cmdlineforce      = 0;
static bool cmdlinehelp       = false;
       bool cmdlinenoscan     = false;
       bool cmdlineexportsoup = false;
       bool cmdlineimportsoup = false;
static bool cmdlineinstall    = false;
static Path cmdlineinstpath   = "";
       bool cmdlineoldkeyw    = true;
       bool cmdlinepriority   = false;
       int  cmdlinesharemode  = SH_DENYNO;
       int  cmdlinetimeout    = -1;
static bool cmdlinewriteareas = false;


//  ------------------------------------------------------------------

static void InitCmdline(char* val) {

  char* key;
  char* trueval;

  if(strchr("-/", *val)) {
    val++;
    getkeyvaleql(&key, &val, true);
    trueval = val;
    if(*val == NUL)
      val = key+1;
    switch(g_toupper(*key)) {
      case 'C':                   // Use another Configfile
        if (*val)
          strcpy(cmdlinecfg, val);
        else
          STD_PRINTNL("Warning: configuration filename missed for -C option, ignored.");
        break;
      case 'D':
        cmdlineoldkeyw = (*val == '-');
        break;
      case 'E':
        if(strieql(key, "EXPORTSOUP"))
          cmdlineexportsoup = true;
        else
          strxcpy(stecho, val, sizeof(stecho));
        break;
      case 'F':
        cmdlineforce = make_bool_not(*val == '-');
        if(g_toupper(key[1]) == 'F')
          cmdlineforce++;
        break;
      case '?':
      case 'H':
        cmdlinehelp = true;
        break;
      case 'I':
        if(strieql(key, "INSTALL")) {
          cmdlineinstall = true;
          cmdlineforce = 1;
          if(*trueval)
            PathCopy(cmdlineinstpath, trueval);
        }
        else if(strieql(key, "IMPORTSOUP"))
          cmdlineimportsoup = true;
        break;
      case 'M':
        disablesound = make_bool_not(*val == '-');
        break;
      case 'N':
        if(strieql(key, "NOSCAN"))
          cmdlinenoscan = true;
        else
          cmdlinesharemode = SH_COMPAT;
        break;
      case 'P':
        cmdlinepriority = make_bool_not(*val == '-');
        break;
      case 'Q':
        quiet = true;
        break;
      case 'S':
        strcpy(AL.sortspec, val);
        break;
      case 'T':
        cmdlinetimeout = atoi(val);
        break;
      case 'V':
        quiet = false;
        veryverbose = (g_toupper(key[1]) == 'V');
        break;
      case 'W':
        cmdlinewriteareas = make_bool_not(*val == '-');
        break;
      case 'Y':
        cmdlinedebughg = make_bool_not(*val == '-');
        break;
      #if defined(GFTRK_ENABLE)
      case 'X':
        __gftrk_statusline = make_bool_not(*val == '-');
        break;
      case 'Z':
        gftrk_set_max = atoi(val);
        if (gftrk_set_max == 0)
        {
          STD_PRINTNL("Warning: Invalid parameter for -Z option, set to 1");
          gftrk_set_max = 1;
        }
        break;
      #endif
    }
  }
  else {
    keybuf += val;
    keybuf += ' ';
  }
}


//  ------------------------------------------------------------------

static void ReadEcholists() {

  Echo* echoin = (Echo*)throw_calloc(1, sizeof(Echo));

  // Read the import taglist
  gfile fp(AddPath(CFG->areapath, CFG->semaphore.importlist), "rt", CFG->sharemode);
  if (fp.isopen())
  {
    char buf[256];
    int echonums = 0;
    update_statusline(LNG->ReadingEcholist);
    while (fp.Fgets(buf, sizeof(buf)))
    {
      if(not strblank(buf)) {
        echonums++;
        echoin = (Echo*)throw_realloc(echoin, (echonums+2)*sizeof(Echo));
        strcpy(echoin[echonums-1], strtrim(strsetsz(buf, sizeof(Echo)-1)));
      }
    }

    *echoin[echonums] = 0;   // Mark end
  }

  // Mark the areas from the import taglist
  for (uint n = 0; n < AL.size(); n++)
  {
    char buf[256];
    strcpy(buf, AL[n]->echoid());
    int x = SearchTaglist(echoin, buf);
    if(*echoin[x])
      AL[n]->set_marked(true);
  }

  throw_free(echoin);
}


//  ------------------------------------------------------------------

static void ReadEscsets()
{
  std::vector<Map>::iterator x = CFG->xlatescset.begin();
  for (int n = 0; x != CFG->xlatescset.end(); x++, n++)
  {
    gfile fp(AddPath(CFG->goldpath, CFG->xlatged), "rb", CFG->sharemode);
    if (fp.isopen())
    {
      fp.FseekSet(((long)CFG->xlatcharset.size()*(long)sizeof(Chs)) + ((long)n*(long)sizeof(Esc)));

      if (strieql(x->imp, "Composed"))
      {
        CompTable = (Esc*)throw_realloc(CompTable, sizeof(Esc));
        fp.Fread(CompTable, sizeof(Esc));
        CompTP = CompTable->t;
      }
      else if (strieql(x->imp, "I51"))
      {
        I51Table = (Esc*)throw_realloc(I51Table, sizeof(Esc));
        fp.Fread(I51Table, sizeof(Esc));
        I51TP = I51Table->t;
      }
      else if (strieql(x->imp, "MNEMONIC"))
      {
        MNETable = (Esc*)throw_realloc(MNETable, sizeof(Esc));
        fp.Fread(MNETable, sizeof(Esc));
        MNETP = MNETable->t;
      }
    }
  }
}


//  ------------------------------------------------------------------

static void ReadAddrMacros()
{
  char* ptr;
  char buf[256], path[GMAXPATH];

  ptr = getenv("FD");
  if(ptr)
    AddBackslash(strcpy(path, ptr));
  else
    strcpy(path, CFG->goldpath);

  MakePathname(CFG->namesfile, path, CFG->namesfile);

  gfile fp(CFG->namesfile, "rt", CFG->sharemode);
  if (fp.isopen())
  {
    update_statusline(LNG->ReadingAddrMacros);
    while (fp.Fgets(buf, sizeof(buf)))
    {
      strbtrim(buf);
      if(*buf != ';' and *buf)
        CfgAddressmacro(buf);
    }
  }
}


//  ------------------------------------------------------------------

static void kbputc_(char c) {

  kbput((gkey)((scancode(c) << 8) | c));
}


//  ------------------------------------------------------------------

static void kbputstr(const char* buf) {

  gkey xkey;
  int n, x;

  x = strlen(buf);

  for(n=0; n<x; n++) {
    switch(buf[n]) {
      case '\t':
      case ' ':
        break;
      case '!':
        clearkeys();
        kbclear();
        break;
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        kbput((gkey)atoi(&buf[n]));
        for(; n<x; n++) {
          if(not isdigit(buf[n]))
            break;
        }
        break;
      case '~':
      case '^':
        xkey = (gkey)(g_toupper(buf[++n]) - '@');
        if(xkey)
          kbput(xkey);
        break;
      case '@':
        xkey = (gkey)atoi(&buf[++n]);
        if(xkey == 0)
          xkey = scancode(buf[n]);
        kbput((gkey)((xkey << 8) & 0xFF00));
        for(; n<x; n++) {
          if(buf[n] == ' ')
            break;
        }
        break;
      case '\"':
        for(n+=1; n<x; n++) {
          if(buf[n] == '\"')
            break;
          kbputc_(buf[n]);
        }
        break;
      case '\'':
        for(n+=1; n<x; n++) {
          if(buf[n] == '\'')
            break;
          kbputc_(buf[n]);
        }
        break;
      default:
        kbputc_(buf[n]);
    }
  }
}


//  ------------------------------------------------------------------

static void w_back() {

  vchar fillch = _box_table(W_BBRAG,14);

  W_STAT = wopen(MAXROW-1,0,MAXROW-1,MAXCOL-1,5,C_STATW,C_STATW);
  update_statusline("");
  wfillch(fillch);
  W_BACK = wopen(0,0,MAXROW-2,MAXCOL-1,5,C_BACKB,C_BACKW|ACSET);
  wfillch((vchar)' ');
}


//  ------------------------------------------------------------------

static void w_brag() {

  char buf[200];

  W_READ = wopen_(1, 2, MAXROW-4, MAXCOL-5, W_BBRAG, C_BRAGB, C_BRAGW);
  w_shadow();


  if(W_BBRAG == 7) {
    wprints(0, 0, C_BRAGB, "                   88     88            88     ");
    wprints(1, 0, C_BRAGB, "     oooooo oooooo 88 oooo88 oooooo oooo88  o  ");
    wprints(2, 0, C_BRAGB, "     88  88 88  88 88 88  88 88oo88 88  88 o8o ");
    wprints(3, 0, C_BRAGB, "     88oo88 88oo88 88 88oo88 88oooo 88oo88  8  ");
    wprints(4, 0, C_BRAGB, " oo      88                                    ");
    wprints(5, 0, C_BRAGB, " 88oooooo88                                    ");
  }
  else {
    wprints_box(0, 0, C_BRAGT|ACSET, "                   É»     É»            É»     ");
    wprints_box(1, 0, C_BRAGT|ACSET, "     ÉËÍÍË» ÉËÍÍË» ºº ÉËÍÍ¹º ÉËÍÍË» ÉËÍÍ¹º  Ë  ");
    wprints_box(2, 0, C_BRAGT|ACSET, "     ºº  ºº ºº  ºº ºº ºº  ºº ºÌÍÍÊ¼ ºº  ºº ÍÎÍ ");
    wprints_box(3, 0, C_BRAGT|ACSET, "     ÈÊÍÍ¹º ÈÊÍÍÊ¼ È¼ ÈÊÍÍÊ¼ ÈÊÍÍÊ¼ ÈÊÍÍÊ¼  Ê  ");
    wprints_box(4, 0, C_BRAGT|ACSET, " É»      ºº                                    ");
    wprints_box(5, 0, C_BRAGT|ACSET, " ÈÊÍÍÍÍÍÍÊ¼                                    ");
  }

  wprints(4, 46-strlen(__gver_longpid__)-1-strlen(__gver_ver__), C_BRAGW, __gver_longpid__);
  wprints(4, 46-strlen(__gver_ver__), C_BRAGW, __gver_ver__);

  wprints(5, 12, C_BRAGW, "http://golded-plus.sourceforge.net");

  wprints(0, 48, C_BRAGW, " GoldED+  Message Editor ");
  gsprintf(PRINTF_DECLARE_BUFFER(buf), " Copyright (C) 1990-%s ",__gver_date__+7);
  wprints(1, 48, C_BRAGW, buf);
  wprints(2, 48, C_BRAGW, "    by Odinn Sorensen,  ");
  wprints(3, 48, C_BRAGW, "   Alexander Aganichev, ");
  wprints(4, 48, C_BRAGW, "      Jacobo Tarrio     ");
  wprints(5, 48, C_BRAGW, "       and others       ");

  whline(6, 0, MAXCOL-5, W_BBRAG, C_BRAGB);
  wvline(0, 47, 7, W_BBRAG, C_BRAGB);

  gsprintf(PRINTF_DECLARE_BUFFER(buf), "---*-*-*** %s ***-*-*---", __gver_releasename__);
  wcenters(8, C_BRAGW, buf);

  wcenters(10, C_BRAGW, "This program is free software; it is licensed under the");
  wcenters(11, C_BRAGW, "GNU General Public License version 2. You're welcome to");
  wcenters(12, C_BRAGW, "redistribute the program or any parts hereof under cer-");
  wcenters(13, C_BRAGW, "tain conditions. See the LICENSE.TXT for more details.");

  wcenters(MAXROW-10, C_BRAGW, "This executable is distributed by");
  if (*__gver_vendor_fido__)
  {
    gsprintf(PRINTF_DECLARE_BUFFER(buf), "%s (Fido: %s) - <%s>",
      __gver_vendor_name__, __gver_vendor_fido__, __gver_vendor_email__);
  }
  else
  {
    gsprintf(PRINTF_DECLARE_BUFFER(buf), "%s <%s>",
      __gver_vendor_name__, __gver_vendor_email__);
  }

  wcenters(MAXROW-9, C_BRAGW, buf);
  gsprintf(PRINTF_DECLARE_BUFFER(buf), "Compiled on %s %s", __gver_date__, __gver_time__);
  wcenters(MAXROW-8, C_BRAGW, buf);
}


//  ------------------------------------------------------------------

static bool ExistCfg(char* path, char* file) {

  bool found = fexist(AddPath(path, file));
  if(found)
    strcat(path, file);
  return found;
}

//  ------------------------------------------------------------------

static bool FindCfg(char* path) {

  bool found = false;

  if(!is_dir(path)) {
    if(veryverbose) STD_PRINT("Try config file \"" << path << "\"... ");
    if(fexist(path)) {
      if(veryverbose) STD_PRINTNL("File found!");
      return true;
    } else {
      if(veryverbose) STD_PRINTNL("file not found.");
      return false;
    }
  }else if(veryverbose) STD_PRINTNL("Try config dir \"" << path << "\":");
  AddBackslash(path);

  #ifdef GEDCFG2
  found = ExistCfg(path, GEDCFG2);
  if(veryverbose) STD_PRINTNL( "Try config file \"" << path << (GEDCFG2) <<
                             "\"... " << (found?("Found!"):("not found.")) );
  #endif

  if(not found)
    found = ExistCfg(path, GEDCFG);
  if(veryverbose) STD_PRINTNL( "Try config file \"" << path << (GEDCFG) <<
                             "\"... " << (found?("Found!"):("not found.")) );
  return found;
}

//  ------------------------------------------------------------------

#ifdef __WIN32__
BOOL WINAPI GoldedCtrlHandler(DWORD dwCtrlType) {

  switch(dwCtrlType) {
    case CTRL_BREAK_EVENT:
      return true;
    case CTRL_CLOSE_EVENT:
    case CTRL_LOGOFF_EVENT:
    case CTRL_SHUTDOWN_EVENT:
      break;
    default:
      return false;
  }

  Cleanup();
  ExitProcess(errorlevel);
  return true;
}
#endif


//  ------------------------------------------------------------------
//  Initialize defaults and generally get the system up and running

void Initialize(int argc, char* argv[]) {

  char* ptr;
  char* ptr2;
  bool dbedit = false;
  bool found = false, compiled;
  std::string truepathtmp;

  throw_init();
  #if defined(GTHROW_LOG)
  throw_log = &LOG;
  #endif
  // First inits
  tzset();
  // set locale
  setlocale(LC_CTYPE, "");
  // and get it's name
  const char* lc = setlocale(LC_CTYPE, "");
  if(lc and not (strstr(lc, "German_") or strstr(lc, "Polish_")))
    right_alt_same_as_left = true;
  #if defined(GUTLOS_FUNCS)
  g_init_os(1);
  #endif
  srand(gtime(NULL));

  // Display startup banner
  STD_PRINTNL(__gver_longpid__ << " " << __gver_ver__ );

  // Check environment commandline
  ptr = getenv("GEDCMD");
  if(ptr) {
    ptr = strskip_wht(ptr);
    if(*ptr) {
      char *v = throw_strdup(ptr);
      ptr2 = v;
      for(;;) {
        getkeyval(&ptr, &ptr2);
        if(*ptr)
          InitCmdline(ptr);
        else
          break;
      }
      throw_free(v);
    }
  }

  // Check if we have been renamed to replace the D'Bridge Editor
  if(striinc("DBEDIT.EXE", argv[0])) {
    for(int a=1; a<argc; a++) {
      if(striinc("INTERNAL", argv[a])) {
        dbedit = true;  // Do not accept any commandline arguments
        break;
      }
    }
  }

  // Check the commandline arguments
  if(argc > 1 and not dbedit) {
    for(int i = 1; i < argc; i++) {
      char *v = throw_strdup(argv[i]);
      InitCmdline(v);
      throw_free(v);
    }
  }

  // Print commandline help and exit if requested
  if (cmdlinehelp)
  {
    STD_PRINTNL("Copyright (C) 1990-2005 Odinn Sorensen, Alexander Aganichev, Jacobo Tarrio,");
    STD_PRINTNL("                        Stas Degteff and others");
    STD_PRINTNL("-------------------------------------------------------------------------------");
    STD_PRINTNL("");
    STD_PRINTNL("Invocation: " << argv[0] << " [-options] [keystacking]");
    STD_PRINTNL("");
    STD_PRINTNL("-C<configfile>  Use a different configuration file.");
    STD_PRINTNL("-D              Disable old obsolete configuration keywords.");
    STD_PRINTNL("-E=<echoid>");
    STD_PRINTNL("-E<echoid>      Start directly in the specified mail area.");
    STD_PRINTNL("-EXPORTSOUP     Export SOUP packets during startup.");
    STD_PRINTNL("-F  or  -FF     Force recompile of most (or all with -FF) configuration files.");
    STD_PRINTNL("-H              Command line help (this message).");
    STD_PRINTNL("-INSTALL[=path] Start the quick install procedure. Look in path, if given.");
    STD_PRINTNL("-IMPORTSOUP     Import SOUP packets during startup.");
    STD_PRINTNL("-M              Mute sounds. Disables all noises in GoldED+.");
    STD_PRINTNL("-N              Disable share-compatible file opens during startup.");
    STD_PRINTNL("-NOSCAN         Temporarily disable area scan during startup.");
#if defined(GUTLOS_FUNCS) && !defined(__MSDOS__)
    STD_PRINTNL("-P              Increase/decrease program priority to run faster/slower.");
#endif
    STD_PRINTNL("-Q              Don't print information messages at config load time (Quiet mode).");
    STD_PRINTNL("-S<sortspec>    Sorts all mail areas according to the sort specs.");
    STD_PRINTNL("-T<seconds>     Set a timeout value. GoldED+ will auto-exit after timeout.");
    STD_PRINTNL("-V  or  -VV     Verbose or Very verbose (-VV) config compile. Use -VV to debug.");
    STD_PRINTNL("-W              Write a GOLDAREA.INC file with AREADEF's of all mail areas.");
    STD_PRINTNL("-X              Debug messages in status line.");
    STD_PRINTNL("-Y              Debug log: function calls trace.");
    STD_PRINTNL("-Z<level>       Debug log with level <level>, default is level 1.");
    STD_PRINTNL("");
    STD_PRINTNL("Any non-option parameter is stuffed into the keyboard buffer.");

    exit(0);
  }

  gvid = new GVid;
  throw_new(gvid);
  CfgInit();
  GFTrkInit(gftrk_set_max);

  // Register exit cleanup functions
  atexit(Cleanup);
  signal(SIGINT, SIG_IGN);

  #ifdef __WIN32__
  SetConsoleCtrlHandler(GoldedCtrlHandler, true);
  #endif

  if(*cmdlinecfg)
    found = true;

  if(not found) {

    // Look for configfilename in the environment
    #if defined(__OS2__)
    ptr = getenv("GED2");
    #elif defined(__WIN32__)
    ptr = getenv("GEDW32");
    #else
    ptr = NULL;
    #endif
    if(not(ptr and *ptr))
      ptr = getenv("GOLDED");
    if(not(ptr and *ptr))
      ptr = getenv("GED");
    if(ptr and *ptr) {
      strxcpy(cmdlinecfg, ptr, sizeof(cmdlinecfg));
      found = FindCfg(cmdlinecfg);
    }

    #ifdef CFGUSERPATH1
    // Get it in user directory (step 1)
    if(not found) {
      strxcpy(cmdlinecfg, CFGUSERPATH1, sizeof(cmdlinecfg));
      strschg_environ(cmdlinecfg);
      found = FindCfg(cmdlinecfg);
    }
    #endif
    #ifdef CFGUSERPATH2
    // Get it in user directory (step 2)
    if(not found) {
      strxcpy(cmdlinecfg, CFGUSERPATH2, sizeof(cmdlinecfg));
      strschg_environ(cmdlinecfg);
      found = FindCfg(cmdlinecfg);
    }
    #endif
    #ifdef CFGPATH
    // Get it in system config directory
    if(not found) {
      strxcpy(cmdlinecfg, CFGPATH, sizeof(cmdlinecfg));
      strschg_environ(cmdlinecfg);
      found = FindCfg(cmdlinecfg);
    }
    #endif

    // Get it in current directory
    if(not found) {
      getcwd(cmdlinecfg, sizeof(cmdlinecfg));
      found = FindCfg(cmdlinecfg);
    }

    // Get it where the the binary file is
    if(not found) {
      extractdirname(cmdlinecfg, argv[0]);
      found = FindCfg(cmdlinecfg);

      // If we still could not find config name...
      if(not found)
        strcat(cmdlinecfg, GEDCFG);
    }
  }
  extractdirname(CFG->goldpath, cmdlinecfg);
  truepathtmp = CFG->goldpath;
  maketruepath(truepathtmp);
  strxcpy(CFG->goldpath, truepathtmp.c_str(), sizeof(CFG->goldpath)-1);
  strxmerge(CFG->goldpath, sizeof(Path), truepathtmp.c_str(), GOLD_SLASH_STR, NULL);
  strxmerge(CFG->goldcfg, sizeof(Path), CFG->goldpath, CleanFilename(cmdlinecfg), NULL);

  // Call install detect procedure
  if(cmdlineinstall)
    InstallDetect(cmdlineinstpath);

  if (not fexist(CFG->goldcfg))
  {
    STD_PRINTNL("*** Cannot start: " << CFG->goldcfg << " not found! ***");
    errorlevel = EXIT_NONAME;
    exit(0);
  }

  // Read/compile the config files
  compiled = ReadGoldedCfg(cmdlineforce);

  // Call install finish procedure
  if (cmdlineinstall && InstallFinish())
  {
    STD_PRINTNL("*** INSTALL NOT COMPLETED ***");
    remove(CFG->goldcfg);
    errorlevel = EXIT_NONAME;
    exit(0);
  }

  HeaderView = new GMsgHeaderView;
  throw_new(HeaderView);
  BodyView = new GMsgBodyView;
  throw_new(BodyView);

  if((CFG->screensize == 50) and (gvid->adapter & V_EGA))
    CFG->screensize = 43;

  #if defined(GUTLOS_FUNCS)
  g_init_title(CFG->tasktitle, CFG->titlestatus);
  g_set_ostitle_name(CFG->tasktitle, 1);
  g_set_osicon();
  if(cmdlinepriority)
    g_increase_priority();
  #endif

  // Mouse support
  #ifdef GOLD_MOUSE
  if(CFG->mouse) {
    gmou.Init();
    gmou.SetLevel(GMOU_LEVEL_CURS);
  }
  #endif

  // Start the log
  static char buf[200];
  gsprintf(PRINTF_DECLARE_BUFFER(buf), "%s %s (%s %s)", __gver_longpid__, __gver_ver__, __gver_date__, __gver_time__);
  LOG.open(CFG->logfile, buf, __gver_shortlogname__, CFG->logformat);

  // Read/compile various configs
  compiled |= ReadLangCfg(cmdlineforce);
  compiled |= make_bool(ReadKeysCfg(cmdlineforce));
  compiled |= make_bool(ReadHelpCfg(cmdlineforce));

  // Initialize sound system
  InitSound();

  // Handle extended keyboard
  if(not CFG->switches.get(keybext))
    gkbd.extkbd = CFG->switches.get(keybext);

  // Report detected multitasker
  if (!quiet && gmtsk.detected)
    STD_PRINTNL("* Running under " << gmtsk.name << ".");

  if (cfgerrors)
  {
    STD_PRINTNL("* Total CFG errors found: " << cfgerrors << ". Press almost any key to continue.");
    kbclear();
    waitkey();
  }

  if (CFG->switches.get(keybclear) || !keybuf.empty() || *CFG->keybstack)
  {
    clearkeys();
    kbclear();
  }

  if (not keybuf.empty())       // The commandline keys
  {
    kbputstr(keybuf.c_str());
    keybuf.erase();
    gKeystacking = true;
  }
  else if (*CFG->keybstack)     // The config keys
  {
    kbputstr(CFG->keybstack);
    gKeystacking = true;
  }

  fieldupd = EDIT->FieldClear() ? 2 : 1;

  vposget(&gvid->orig.cursor.row, &gvid->orig.cursor.column);

  #ifndef __UNIX__
  oldscreen = vsave();
  #endif

  if(CFG->screensize > 0xFF) {
    gvid->setmode(CFG->screensize >> 8);    // Set video mode
  }
  else if(CFG->screensize) {
    gvid->setrows(CFG->screensize);         // Just set rows
  }

  gvid->setintensity(CFG->intensecolors);

  if (C_BACKB != (BLACK_|_BLACK))
    gvid->setoverscan(C_BACKB);

  vcurhide();

  if(CFG->screenmaxrow >= 10)
    MAXROW = CFG->screenmaxrow;

  if(CFG->screenmaxcol >= 80)
    MAXCOL = CFG->screenmaxcol;

  if(CFG->dispmargin <= 0)
    CFG->dispmargin += MAXCOL;

  if(CFG->dispmargin > MAXCOL)
    CFG->dispmargin = MAXCOL;

  if(CFG->quotemargin <= 0)
    CFG->quotemargin += MAXCOL;

  if(EDIT->QuoteMargin() <= 0)
    EDIT->QuoteMargin(EDIT->QuoteMargin()+MAXCOL);

  #ifdef __MSDOS__
  if(CFG->switches.get(screenusebios))
    gvid->setdevice(GVID_BIO);
  else
    gvid->setdevice(GVID_DMA);
  #endif

  // Set palette if changes were specified
  if(CFG->screenpalette[16])
    gvid->setpalette(CFG->screenpalette);

  // Set window opening style
  wsetstyle(STYLE_NORMAL);

  // If keyboard polling is enabled
  if(CFG->keybmode == KEYB_POLL) {

    // Switch to keyboard polling instead of blocking
    gkbd.polling = true;
    gkbd.tickinterval = 5;      // Tick twice per second
    kbdsettickfunc(update_statuslines);
  }

  inforow = ((MAXROW-1)/2)+6;

  if(CFG->aka.empty()) {
    LOG.ErrConfig();
    LOG.printf("! There do not seem to be any ADDRESS's defined.");
    LOG.printf("+ Advice: Check your setup of ADDRESS's.");
    ConfigErrorExit();
  }

  bool areasdefined = false;

  // Do checking for an area and unknown aka's in area
  // config at once.
  for(AL.item = AL.idx.begin(); AL.item != AL.idx.end(); AL.item++) {
    if(not (*AL.item)->isseparator())
      areasdefined = true;

    if(not CFG->aka.empty() and (not (*AL.item)->aka().valid()))
      (*AL.item)->set_aka(CFG->aka[0].addr);
  }

  if(not areasdefined) {
    LOG.ErrConfig();
    LOG.printf("! There do not seem to be any mail areas defined.");
    LOG.printf("+ Advice: Check your setup of mail areas.");
    ConfigErrorExit();
  }

  if(CFG->username.empty()) {
    LOG.ErrConfig();
    LOG.printf("! There do not seem to be any USERNAME's defined.");
    LOG.printf("+ Advice: Check your setup of USERNAME's.");
    ConfigErrorExit();
  }

  if(CFG->cmdkey.empty()) {
    LOG.ErrConfig();
    LOG.printf("! There do not seem to be any command keys defined.");
    LOG.printf("+ Advice: Check your key setup in " GOLDKEYS_CFG );
    ConfigErrorExit();
  }

  if(CFG->origin.empty()) {
    static char orig[] = "";
    CfgOrigin(orig);
  }

  w_back();     // Open a nice background window
  w_brag();     // Display Brag Window

  update_statusline(LNG->Initializing);

  HandleGEvent(EVTT_STARTUP);

  // Write the binary config if any of the cfgs were compiled.
  if(compiled)
    WriteGoldGed();

  // Override timeout from commandline
  if(cmdlinetimeout != -1)
    CFG->timeout = cmdlinetimeout;

  // Adjust header item sizes if negative
  if(CFG->disphdrnameset.pos < 0)    CFG->disphdrnameset.pos += MAXCOL;
  if(CFG->disphdrnodeset.pos < 0)    CFG->disphdrnodeset.pos += MAXCOL;
  if(CFG->disphdrdateset.pos < 0)    CFG->disphdrdateset.pos += MAXCOL;
  if(CFG->disphdrnameset.len < 0)    CFG->disphdrnameset.len += MAXCOL;
  if(CFG->disphdrnodeset.len < 0)    CFG->disphdrnodeset.len += MAXCOL;
  if(CFG->disphdrdateset.len < 0)    CFG->disphdrdateset.len += MAXCOL;

  if(EDIT->HdrNamePos() < 0)  EDIT->HdrNamePos(EDIT->HdrNamePos()+MAXCOL);
  if(EDIT->HdrNameLen() < 0)  EDIT->HdrNameLen(EDIT->HdrNameLen()+MAXCOL);
  if(EDIT->HdrNodePos() < 0)  EDIT->HdrNodePos(EDIT->HdrNodePos()+MAXCOL);
  if(EDIT->HdrNodeLen() < 0)  EDIT->HdrNodeLen(EDIT->HdrNodeLen()+MAXCOL);

  // Set default marks names
  AL.SetDefaultMarks();

  // Read the lastreads from the last session
  if(CFG->switches.get(areakeeplast))
    AL.ReadGoldLast();

  // Setup start echo
  if(not *stecho)
    strcpy(stecho, CFG->areastart);

  // Sort the areas
  if(*AL.sortspec)
    strcpy(CFG->arealistsort, AL.sortspec);
  AL.Sort();

  // Write GOLDAREA.INC if told
  if(cmdlinewriteareas)
    AL.WriteAreaDef(AddPath(CFG->goldpath, GOLDAREA_INC));

  // Get the global max length of echoids (for the arealist)
  extern int arealistnumgrps;
  extern int areaswithgroupid;
  for(uint n=0; n<AL.size(); n++) {
    int _tmp = strlen(AL[n]->echoid());
    if(echoid_width < _tmp)
      echoid_width = _tmp;
    if(AL[n]->groupid()) {
      areaswithgroupid++;
      if(AL[n]->groupid() & 0x8000u)
        arealistnumgrps = true;
    }
  }
  if(CFG->arealistechomax) {
    if(CFG->arealistechomax > (int)(sizeof(Echo)-1))
      CFG->arealistechomax = sizeof(Echo)-1;
    if(CFG->arealistechomax < 0)
      echoid_width += CFG->arealistechomax;
    else
      echoid_width = CFG->arealistechomax;
  }
  if(areaswithgroupid and CFG->switches.get(arealistgroupid))
    groupid_width = arealistnumgrps ? 3 : 1;

  int spaces = 0;
  bool area_found = false;
  bool marked_found = false;
  bool desc_found = false;
  bool count_found = false;
  bool pmark_found = false;
  bool unread_found = false;
  bool changed_found = false;
  bool echoid_found = false;
  bool groupid_found = false;
  for(int pass=1; pass<=2; pass++) {
    if(pass == 2) {
      if(not area_found)     area_width = 0;
      if(not marked_found)   marked_width = 0;
      if(not desc_found)     desc_width = 0;
      if(not count_found)    count_width = 0;
      if(not pmark_found)    pmark_width = 0;
      if(not unread_found)   unread_width = 0;
      if(not changed_found)  changed_width = 0;
      if(not echoid_found)   echoid_width = 0;
      if(not groupid_found)  groupid_width = 0;
      if(desc_width == -1) {
        desc_width =
          MAXCOL - 2 - spaces - area_width - marked_width -
          count_width - pmark_width - unread_width - changed_width -
          echoid_width - groupid_width;
      }
    }
    int pos = 0;
    char* p = CFG->arealistformat;
    while(*p) {
      char c = (char)g_toupper(*p);
      char d = *(++p);
      int w = atoi(p);
      while(isdigit(*p))
        p++;
      if(pass == 1) {
        if(g_isalpha(c)) {
          switch(c) {
            case 'A':  area_found = true;     if(isdigit(d)) area_width = w;     break;
            case 'M':  marked_found = true;   if(isdigit(d)) marked_width = w;   break;
            case 'D':  desc_found = true;     if(isdigit(d)) desc_width = w;     break;
            case 'C':  count_found = true;    if(isdigit(d)) count_width = w;    break;
            case 'P':  pmark_found = true;    if(isdigit(d)) pmark_width = w;    break;
            case 'U':  unread_found = true;   if(isdigit(d)) unread_width = w;   break;
            case 'N':  changed_found = true;  if(isdigit(d)) changed_width = w;  break;
            case 'E':  echoid_found = true;   if(isdigit(d)) echoid_width = w;   break;
            case 'G':  groupid_found = true;  if(isdigit(d)) groupid_width = w;  break;
          }
        }
        else {
          spaces++;
        }
      }
      else {
        switch(c) {
          case 'A':  area_pos = pos;     pos += area_width;     break;
          case 'M':  marked_pos = pos;   pos += marked_width;   break;
          case 'D':  desc_pos = pos;     pos += desc_width;     break;
          case 'C':  count_pos = pos;    pos += count_width;    break;
          case 'P':  pmark_pos = pos;    pos += pmark_width;    break;
          case 'U':  unread_pos = pos;   pos += unread_width;   break;
          case 'N':  changed_pos = pos;  pos += changed_width;  break;
          case 'E':  echoid_pos = pos;   pos += echoid_width;   break;
          case 'G':  groupid_pos = pos;  pos += groupid_width;  break;
          default:   pos++;
        }
      }
    }
  }

  update_statuslines();
  ReadEcholists();            // Read the Confmail compatible echotag lists
  update_statuslines();
  ReadAddrMacros();           // Read the address macro file
  update_statuslines();
  ReadEscsets();              // Read the escape tables
  update_statuslines();

  // Initialize the messagebases
  update_statuslinef(LNG->LockShareCap, "ST_LOCKSHARECAP", LNG->Checking);
  WideLog = &LOG;
  WideDebug = cmdlinedebughg;
  WideCanLock = CFG->sharemode ? TestLockPath(CFG->temppath) : false;
  WideSharemode = CFG->sharemode;
  WideUsernames = CFG->username.size();
  WideUsername = new const char*[WideUsernames];
  std::vector<Node>::iterator i;
  int w;
  for(w = 0, i = CFG->username.begin(); w < WideUsernames; w++, i++)
    WideUsername[w] = i->name;
  WidePersonalmail = CFG->personalmail;

  if(CFG->loadlanguage[0])
    LoadLanguage(CFG->loadlanguage);

  if(find(AL.basetypes, "OPUS") or find(AL.basetypes, "FTS1")) {
    update_statuslinef("%s Fido", "", LNG->Checking);
    FidoInit(CFG->fidolastread, CFG->switches.get(fidohwmarks), CFG->switches.get(fidonullfix), CFG->fidouserno, CFG->squishuserpath);
  }
  #ifndef GMB_NOEZY
  if(find(AL.basetypes, "EZYCOM")) {
    update_statuslinef("%s Ezycom", "", LNG->Checking);
    EzycomInit(CFG->ezycom.msgbasepath, CFG->ezycom.userbasepath, CFG->ezycomuserno);
  }
  #endif
  #ifndef GMB_NOGOLD
  if(find(AL.basetypes, "GOLDBASE")) {
    update_statuslinef("%s Goldbase", "", LNG->Checking);
    GoldInit(CFG->goldbasepath, CFG->goldbasesyspath, CFG->goldbaseuserno);
  }
  #endif
  #ifndef GMB_NOHUDS
  if(find(AL.basetypes, "HUDSON")) {
    update_statuslinef("%s Hudson", "", LNG->Checking);
    HudsInit(CFG->hudsonpath, CFG->hudsonsyspath, CFG->hudsonuserno, CFG->hudsonsizewarn, CFG->ra2usersbbs);
  }
  #endif
  #ifndef GMB_NOJAM
  if(find(AL.basetypes, "JAM")) {
    update_statuslinef("%s JAM", "", LNG->Checking);
    JamInit(CFG->jampath, CFG->switches.get(jamharddelete), CFG->switches.get(jamsmapihw));
  }
  #endif
  #ifndef GMB_NOPCB
  if(find(AL.basetypes, "PCBOARD")) {
    update_statuslinef("%s PCBoard", "", LNG->Checking);
    PcbInit(CFG->pcboardpath, CFG->pcboarduserno);
  }
  #endif
  #ifndef GMB_NOSQSH
  if(find(AL.basetypes, "SQUISH")) {
    update_statuslinef("%s Squish", "", LNG->Checking);
    SquishInit(CFG->squishuserpath, CFG->squishuserno, CFG->switches.get(squishdirect), true, CFG->squishscan);
  }
  #endif
  #ifndef GMB_NOWCAT
  if(find(AL.basetypes, "WILDCAT")) {
    update_statuslinef("%s WildCat!", "", LNG->Checking);
    WCatInit(CFG->wildcatuserno);
  }
  #endif
  #ifndef GMB_NOXBBS
  if(find(AL.basetypes, "ADEPTXBBS")) {
    update_statuslinef("%s AdeptXBBS", "", LNG->Checking);
    XbbsInit(CFG->adeptxbbspath, CFG->adeptxbbsuserno);
  }
  #endif
  #ifndef GMB_NOSMB
  if(find(AL.basetypes, "SMB")) {
    update_statuslinef("%s Synchronet", "", LNG->Checking);
    SMBInit();
  }
  #endif
  update_statuslinef("...", "");

  // Delete the D'Bridge "mail waiting" semaphore files
  if(dbedit) {
    remove(AddPath(CFG->areapath, "DBRIDGE.NMW"));
    remove(AddPath(CFG->areapath, "DBRIDGE.EMW"));
  }

  // Unlink windows
  wunlink(W_READ);

  THROW_CHECK();
}


//  ------------------------------------------------------------------

