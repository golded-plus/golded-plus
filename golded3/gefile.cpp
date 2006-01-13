
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
//  File handling.
//  ------------------------------------------------------------------

#include <fcntl.h>
#include <golded.h>
#include <gwildmat.h>
#include <gdirposx.h>

#if defined(__USE_ALLOCA__)
#include <malloc.h>
#endif


//  ------------------------------------------------------------------

char CFG__frqinvfilechars[40] = { "\"()+,.\\/:;<=>[]| " };
char CFG__frqskipwordchars[40] = { "0123456789-[" };

inline bool invalidfilechar(char ch) { return not isascii(ch&0xff) or strchr(CFG__frqinvfilechars, ch); }


//  ------------------------------------------------------------------

int fspecs = 0;
int specfiles = 0;
FileSpec* fspec = NULL;

static FFblk* fblk;
static int selfiles;
static int totfiles;
static long selbytes;
static long totbytes;


//  ------------------------------------------------------------------

static int cmp_ffb(const FFblk* a, const FFblk* b) {

  return stricmp(a->name, b->name);
}


//  ------------------------------------------------------------------

static int cmp_ffbs(const FFblk* a, const FFblk* b) {

  int cmp;

  if((cmp = b->selected - a->selected) != 0)  // Selected first
    return cmp;

  return stricmp(a->name, b->name);
}

class gfileselect : public gwinpick {

  gwindow window;
  GMsg*   msg;

  void open();
  void close();
  void do_delayed();
  void print_line(uint idx, uint pos, bool isbar);
  void scroll(int where);
  bool handle_key();
  const char *gensize(uint32_t size);

public:

  void run(GMsg* cmsg);
};


//  ------------------------------------------------------------------

void gfileselect::open() {

  display_page();

}


//  ------------------------------------------------------------------

void gfileselect::close() {

}


//  ------------------------------------------------------------------

void gfileselect::scroll(int where) {

  wscrollbox(0, 0, maximum_position, xlen-1, 1, where);
}


//  ------------------------------------------------------------------

void gfileselect::do_delayed() {

  char buf[200];
  char tmp[80];

  sprintf(buf, "%s %10s", LNG->SelectedFiles, longdotstr(tmp, selfiles));
  wprints(ylen-4, MAXCOL-34, wattr, buf);
  sprintf(buf, "%s %10s", LNG->SelectedBytes, longdotstr(tmp, selbytes));
  wprints(ylen-3, MAXCOL-34, wattr, buf);

  sprintf(buf, "%s %10s", LNG->TotalFiles, longdotstr(tmp, totfiles));
  wprints(ylen-2, MAXCOL-34, wattr, buf);
  sprintf(buf, "%s %10s", LNG->TotalBytes, longdotstr(tmp, totbytes));
  wprints(ylen-1, MAXCOL-34, wattr, buf);

  if(CFG->switches.get(filelistpagebar))
    wscrollbar(W_VERT, maximum_index+1, maximum_index, index);

}


//  ------------------------------------------------------------------

#define KFIX(A) (int) (((A) * 1000.0 / 1024.0) / 10)

const char *gfileselect::gensize(uint32_t size) {

  static char ret[16];

  if(size >= 1048576000) {
    size += 5242880;
    sprintf(ret, "%3d.%02dG", (int) size/1073741824, KFIX((size%1073741824)/1024));
  }
  else if(size >= 1024000) {
    size += 5120;
    sprintf(ret, "%3d.%02dM", (int) size/1048576, KFIX((size%1048576)/1024));
  }
  else if(size >= 1000) {
    size += 5;
    sprintf(ret, "%3d.%02dk", (int) size/1024, KFIX(size%1024));
  }
  else
    sprintf(ret, "%d", (int) size);
  return ret;
}


//  ------------------------------------------------------------------

void gfileselect::print_line(uint idx, uint pos, bool isbar) {

  char buf[200];

  FFblk& fb = fblk[idx];

  sprintf(buf, "%c%-*.*s %8s %2d-%02d-%02d %2d:%02d ",
      fb.selected ? MMRK_MARK : ' ',
      MAXCOL-62, (int)MAXCOL-62,
      fb.name,
      gensize(fb.size),
      fb.day,
      fb.month,
      fb.year % 100,
      fb.hour,
      fb.minute
    );

  wprints(pos, 0, isbar ? sattr : wattr, buf);
  wprintc(pos, 0, isbar ? sattr : (*buf == ' ' ? wattr : hattr), *buf);

}


//  ------------------------------------------------------------------

bool gfileselect::handle_key() {

  gkey kk;
  uint n;

  if(key < KK_Commands) {
    key = key_tolower(key);
    kk = SearchKey(key, FileKey, FileKeys);
    if(kk)
      key = kk;
  }

  switch(key) {
    case KK_FileUnMarkAll:
      selfiles = 0;
      selbytes = 0;
      for(n=0; n<=maximum_index; n++)
        fblk[n].selected = false;

      update();

      break;

    case KK_FileMarkAll:

      selfiles = totfiles;
      selbytes = totbytes;

      for(n=0; n<=maximum_index; n++)
        fblk[n].selected = true;

      update();

      break;

    case KK_FileToggleMarkAll:

      selfiles = 0;
      selbytes = 0;

      for(n=0; n<=maximum_index; n++) {
        if(fblk[n].selected) {
          fblk[n].selected = false;
        }
        else {
          fblk[n].selected = true;
          selbytes += fblk[n].size;
          selfiles++;
        }
      }

      update();

      break;

    case KK_FileAskExit:
      {
        GMenuQuit MenuQuit;
        if(not MenuQuit.Run())
          break;
      }
      ////////////// Drop through

    case KK_FileQuitNow:
      gkbd.quitall = true;
      ////////////// Drop through

    case KK_FileAbort:

      for(n=0; n<=maximum_index; n++)
        fblk[n].selected = false;

      selfiles = 0;
      selbytes = 0;

      aborted = true;

      return false;

    case KK_FileSelect:

      if(not selfiles) {  // If no files selected, select cursor file
        fblk[index].selected = true;
        selbytes += fblk[index].size;
        selfiles++;
      }

      return false;

    case KK_FileToggleMark:
      key = fblk[index].selected ? KK_FileUnMark : KK_FileMark;

      /////////// Drop through
    case KK_FileUnMark:
    case KK_FileMark:

      if(key == KK_FileMark) {
        if(not fblk[index].selected) {
          fblk[index].selected = true;
          selbytes += fblk[index].size;
          selfiles++;
        }
      }
      else {
        if(fblk[index].selected) {
          fblk[index].selected = false;
          selbytes -= fblk[index].size;
          selfiles--;
        }
      }
      precursor(); display_bar(); cursor_down();
      break;
    case KK_FileGotoPrev:  precursor(); cursor_up();    break;
    case KK_FileGotoNext:  precursor(); cursor_down();  break;
    case KK_FileGotoFirst: precursor(); cursor_first(); break;
    case KK_FileGotoLast:  precursor(); cursor_last();  break;

    case KK_FileDosShell:
      DosShell();
      break;

    case Key_Tick:
      CheckTick(KK_FileQuitNow);
      break;

    case KK_FileUndefine:
      break;

    default:
      if(not PlayMacro(key, KT_F))
        SayBibi();
  }

  return true;
}


//  ------------------------------------------------------------------

void gfileselect::run(GMsg* cmsg) {

  ypos    = 6;                                // Window Starting Row
  xpos    = 0;                                // Window Starting Column
  ylen    = MAXROW - 9;                       // Window Height
  xlen    = MAXCOL - 36;                      // Window Width
  btype   = W_BMENU;                          // Window Border Type
  battr   = C_MENUB;                          // Window Border Color
  wattr   = C_MENUW;                          // Window Color
  tattr   = C_MENUW;                          // Window Title Color
  sattr   = C_MENUS;                          // Window Selection Bar Color
  hattr   = C_MENUQ;                          // Window Highlight Color
  sbattr  = C_MENUB;                          // Window Scrollbar Color
  title   = " title ";                        // Window Title
  helpcat = H_FileSelect;                     // Window Help Category
  msg     = cmsg;

  maximum_index    = totfiles - 1;            // List Entries - 1
  minimum_index    = 0;                       // Minimum index in the list
  maximum_position = MinV(ylen-1,(uint) totfiles);// Display Pos
  index = 0;                                  // List Index
  position = 0;                               // Display Pos
  aborted = false;                            // True if aborted
  listwrap = CFG->switches.get(displistwrap); // True if wrap-around is supported

  run_picker();

}



//  ------------------------------------------------------------------

void FileSelect(GMsg* msg, char* title, FileSpec* fspec) {

  gfileselect* p = new gfileselect;
  throw_new(p);

  char buf[256];
  Path fbuf;
  uint MIN_POS=0, MAX_POS=MIN_POS+MAXROW-10;
  bool done, winop = false;

  selfiles = totfiles = 0;
  selbytes = totbytes = 0;

  strcpy(fbuf, fspec->path);
  MapPath(fbuf);

  Path fullpath;
  if(fbuf[0] and is_dir(fbuf))
    AddBackslash(fbuf);
  extractdirname(fullpath, fbuf);
  const char* wildlistname = (isslash(fbuf[strlen(fbuf)-1]) or is_dir(fbuf)) ? "*" : CleanFilename(fbuf);
  gposixdir f(fullpath);
  const gdirentry *de;
  for(done = false; (de = f.nextentry(wildlistname)) != NULL and not done; )
    if(de->is_file())
      done = true;

  if((strpbrk(fbuf, "*?")) or done) {
    winop = true;
    vcurhide();
    wopen_(6,0, MAX_POS+3, MAXCOL, W_BMENU, C_MENUB, C_MENUW, C_MENUPB);

    wvline(0, MAXCOL-36, MAX_POS+1, W_BMENU, C_MENUB);
    whline(MAX_POS-4, MAXCOL-36, 36, W_BMENU, C_MENUB);

    sprintf(buf, " %s ", strtrim(fbuf));
    wmessage(buf, TP_BORD, 1, C_MENUT);
    wmessage(title, TP_BORD, MAXCOL-34, C_MENUT);

    update_statusline(LNG->SelectFiles);

    wprints(MAX_POS/2, 0, C_MENUW, LNG->ScanningDirectory);
  }

  fblk = NULL;

  f.rewind();
  while((de = f.nextentry(wildlistname)) != NULL) {
    if(not de->is_file())
      continue;
    FFblk* fb;
    fblk = (FFblk*)throw_realloc(fblk, (totfiles+1)*sizeof(FFblk));
    fb = fblk + totfiles;
    strcpy(fb->name, de->name.c_str());
    fb->selected = false;
    FFTime ftm;
    dword _ftm = gfixstattime(de->stat_info.st_mtime);
    memcpy(&ftm, &_ftm, sizeof(FFTime));
    fb->year = ftm.ft_year + 1980;
    fb->month = ftm.ft_month;
    fb->day = ftm.ft_day;
    fb->hour = ftm.ft_hour;
    fb->minute = ftm.ft_min;
    fb->second = ftm.ft_tsec * 2;
    fb->size = de->stat_info.st_size;
    totbytes += fb->size;
    totfiles++;
  }

  if(totfiles) {
    if(totfiles > 1) {
      qsort(fblk, totfiles, sizeof(FFblk), (StdCmpCP)cmp_ffb);
      p->run(msg);
    }
    else {
      selfiles = 1;
      fblk[0].selected = YES;
    }

    if(p->aborted or selfiles == 0) {
      fspec->fblk = NULL;
      fspec->files = 0;
      throw_release(fblk);
    }
    else {
      // Realloc to include only the selected files
      qsort(fblk, totfiles, sizeof(FFblk), (StdCmpCP)cmp_ffbs);
      fspec->fblk = fblk = (FFblk*)throw_realloc(fblk, (selfiles+1)*sizeof(FFblk));
      AddBackslash(strcpy(fspec->path, f.fullpath()));
      fspec->files = selfiles;
      fblk = NULL;
    }
  }
  else {
    wprints(MAX_POS/2, 0, C_MENUW, LNG->NoFilesFound);
    update_statusline(LNG->FilesPressKey);
    throw_release(fblk);
    waitkeyt(10000);
  }

  if(winop) {
    wclose();
    vcurshow();
  }

  delete p;
}


//  ------------------------------------------------------------------

void CreateFileAddr(GMsg* msg) {

  // Create message
  char* _txt;
  if(msg->attr.att())
    _txt = LNG->AutoAttachMsg;
  else if(msg->attr.frq())
    _txt = LNG->AutoRequestMsg;
  else if(msg->attr.urq())
    _txt = LNG->AutoUpdreqMsg;
  else
    _txt = LNG->EmptyMsg;
  msg->txt = (char*)throw_realloc(msg->txt, strlen(_txt)+256);
  strcpy(msg->txt, _txt);
  TokenXlat(MODE_NEW, msg->txt, msg, msg, CurrArea);
}


//  ------------------------------------------------------------------

void WriteFMsgs(int msgsdone, GMsg* msg, Attr o_attr, int modex, int mode, char* buf) {

  if(msgsdone) {
    modex = GMSG_NEW;
    AA->NewMsgno(msg);

    throw_release(msg->txt);
    msg->attr = o_attr;           // copy attributes from original
    CreateFileAddr(msg);
    msg->TextToLines(CFG->dispmargin);
    if(AA->isecho())
      DoTearorig(mode, msg);
  }

  DoKludges(mode, msg);

  strtrim(strcpy(msg->re, buf));
  HeaderView->Use(AA, msg);
  HeaderView->Paint();
  msg->LinesToText();
  AA->SaveMsg(modex, msg);
}


//  ------------------------------------------------------------------

void CreateFileMsgs(int mode, GMsg* msg) {

  char* ptr;
  Attr o_attr;
  int m, n, x;
  int modex, msgsdone=0;
  ISub buf, subj;

  char* LNG_File = NULL;
  char* LNG_Fileing = NULL;
  char* tokenstr = "";

  if(msg->attr.att()) {
    LNG_File    = LNG->File_Attach;
    LNG_Fileing = LNG->FileAttaching;
    tokenstr = "ST_FILEATTACHING";
  }
  else if(msg->attr.frq()) {
    LNG_File    = LNG->File_Request;
    LNG_Fileing = LNG->FileRequesting;
    tokenstr = "ST_FILEREQUESTING";
  }
  else if(msg->attr.urq()) {
    LNG_File    = LNG->File_Updreq;
    LNG_Fileing = LNG->FileUpdreqing;
    tokenstr = "ST_FILEUPDREQING";
  }

  *buf = NUL;
  *subj = NUL;

  o_attr = msg->attr;

  if(specfiles > 1)
    w_progress(MODE_NEW, C_INFOW, 1, specfiles, LNG_File);

  // Do ZoneGating
  if(CFG->zonegating and (msg->dest.zone != msg->orig.zone)) {
    GMenuZonegate MenuZonegate;
    if(CFG->zonegating == YES or MenuZonegate.Run())
      ZonegateIt(msg->odest, msg->orig, msg->dest);
  }

  if(msg->odest.net == 0)
    msg->odest = msg->dest;

  *buf = NUL;
  ptr = throw_strdup(msg->txt);   // Keep a copy of the original
  modex = (mode == MODE_CHANGE) ? GMSG_UPDATE : GMSG_NEW;

  for(n=0,x=0; x<fspecs; x++) {
    for(m=0; m<fspec[x].files; m++,n++) {
      if(specfiles > 1) {
        w_progress(MODE_UPDATE, C_INFOW, n+1, specfiles, LNG_File);
        update_statuslinef(LNG_Fileing, tokenstr, n+1, specfiles,
            msg->dest.zone, msg->dest.net, msg->dest.node, msg->dest.point
        );
      }
      sprintf(subj, "%s%s%s%s%s ", fspec[x].delsent ? "^" : "", fspec[x].path, fspec[x].fblk ? (fspec[x].fblk[m].name ? fspec[x].fblk[m].name : "") : "", *fspec[x].password ? " " : "", fspec[x].password);
#if !defined(__UNIX__) && !defined(__WIN32__)
      strupr(subj);
#endif

      if((strlen(buf) + strlen(subj)) > 71) {

        WriteFMsgs(msgsdone, msg, o_attr, modex, mode, buf);

        msgsdone++;
        strcpy(buf, subj);
      }
      else {
        strcat(buf, subj);
      }
    }
  }

  if(not strblank(buf))
    WriteFMsgs(msgsdone, msg, o_attr, modex, mode, buf);

  throw_release(msg->txt);
  msg->txt = ptr;
  msg->TextToLines(CFG->dispmargin);

  if(specfiles > 1)
    w_progress(MODE_QUIT, BLACK|_BLACK, 0, 0, NULL);
}


//  ------------------------------------------------------------------

static int FreqCmp(const char** a, const char** b) {

  return stricmp(*a, *b);
}


//  ------------------------------------------------------------------

static int frqchkdesc(char* desc) {

  char* ptr = desc;

  if(*desc == NUL)
    strcpy(desc, " ");
  else {
    while(*ptr and not isxalnum(*ptr))
      ptr++;
    if(*ptr == NUL) {
      *desc = NUL;
      return true;
    }
  }
  ptr = desc + strlen(desc) - 1;
  while(not isascii(*ptr&0xff) and not isxalnum(*ptr) and (ptr>desc))
    *ptr-- = NUL;

  return false;
}


//  ------------------------------------------------------------------

static int frqgetfile(char* file, char* desc, char* filesize, const char* txt) {

  const char* ptr = txt;

  // Scan filename for first invalid char (.extension)
  while(*ptr and not invalidfilechar(*ptr))
    ptr++;

  // General file extension finder
  if(*ptr++ == '.') {
    if(not invalidfilechar(*ptr) and ((ptr - txt) > 1)) {

      // Find end of extension
      while(*ptr and ((*ptr == '.') or not invalidfilechar(*ptr)))
        ptr++;

      // Strip dots from the end of the extension
      while(*(ptr-1) == '.')
        ptr--;

      // Copy the filename
      strxcpy(file, txt, (uint)(ptr-txt)+1);

      // Get description
      if(desc) {
        if(*ptr) {
          if(not isspace(*ptr))
            ptr = strskip_txt(ptr);
          ptr = strskip_wht(ptr);

          bool skipped = false;
          while(strchr(CFG__frqskipwordchars, *ptr) and *ptr) {
            if(not *filesize) {
              // is a file-size given?
              const char* p  = ptr;
              while(strchr(".,0123456789bBkKmMgG", *p))
                p++;

              // Check for the following cases, as they're not valid
              // 01/01/98 or 01/01/1998 or 1998/01/01
              // 01-01-98 or 01-01-1998 or 1998-01-01
              // 01.01.98 or 01.01.1998 or 1998.01.01
              if(p-ptr>1 and not strchr("-/.", *p)) {
                p = strxcpy(filesize, ptr, MinV((int) (p-ptr+1), 9));
                if(strlen(p)>=6 and p[2] == p[5] and strchr("-/.", p[2]))
                  *filesize = NUL;
              }
            }
            ptr = strskip_wht(strskip_txt(ptr));
            skipped = true;
          }
          if(not *ptr and skipped)
            strcpy(desc, " ");
          else
            strcpy(desc, ptr);
        }

        return frqchkdesc(desc);
      }
    }
  }

  return false;
}


//  ------------------------------------------------------------------

void FileRequest(GMsg* msg) {

  GFTRK("FileRequest");

  int oldtopline = reader_topline;

  if(AA->Msgn.Count() and msg->line and msg->lines) {

    char buf[256];
    const char* ptr;
    const char* ptr1 = NULL;
    const char* ptr2;
    const char* txtptr;
    char** freqfile = NULL;
    int gotticket = false;
    int getnextdesc = false;
    Line** lin = msg->line;
    char file[GMAXPATH], desc[200], filesize[10];
    int freqfiles = 0;
    bool esc = true;
    int n;
    byte numlines = 0;

    *desc = *file = *filesize = NUL;

    w_info(LNG->Wait);

    // Scan the current msg for file announcements
    int tline = reader_topline;
    if(CFG->frqoptions & FREQ_FROMTOP)
      tline = 0;
    for(n=tline; n<msg->lines; n++) {

      // Don't look in control info for files
      if(lin[n]->type & (GLINE_KLUDGE|GLINE_TEAR|GLINE_ORIG))
        continue;

      ptr = txtptr = lin[n]->txt.c_str();

      // Skip past whitespace and highbit junk
      while(((*ptr < '!') or (*ptr > '\x7F')) and *ptr)
        ptr++;

      // Get description if Ticket was detected in the previous line
      if(gotticket) {
        strxcpy(desc, strskip_wht(strskip_txt(ptr)), sizeof(desc));
        if(*desc == NUL)
          strcpy(desc, " ");
        gotticket = false;
      }

      // Get description from file of previous line
      if(getnextdesc) {
        strxcpy(desc, ptr, sizeof(desc));
        getnextdesc = frqchkdesc(desc);
      }

      // Get filename from typical announcement formats
      if(not *file and (strnieql(ptr, "File", 4) or strnieql(ptr, "Name", 4))) {
        ptr1 = strskip_txt(ptr);
        ptr2 = strskip_wht(ptr1);
        if((*(ptr1-1) == ':') or (*ptr2 == ':')) {
          if(*ptr2 == ':')
            ptr1 = strskip_wht(ptr2+1);
          else
            ptr1 = ptr2;
          ptr2 = strskip_txt(ptr1);
#if defined(__USE_ALLOCA__)
          char *tmpbuf = (char*)alloca(ptr2-ptr1+1);
#else
          __extension__ char tmpbuf[ptr2-ptr1+1];
#endif
          strxcpy(tmpbuf, ptr1, ptr2-ptr1+1);
          frqgetfile(file, desc, filesize, tmpbuf);
          *desc = NUL;  // Description never comes before filename
          continue;
        }
      }

      // Get description from typical announcement formats
      if(not *desc and (strnieql(ptr, "Desc", 4) or strnieql(ptr, "Note", 4))) {
        ptr1 = strskip_txt(ptr);
        ptr2 = strskip_wht(ptr1);
        if((*(ptr1-1) == ':') or (*ptr2 == ':')) {
          if(*ptr2 == ':')
            ptr1 = strskip_wht(ptr2+1);
          else
            ptr1 = ptr2;
          strxcpy(desc, ptr1, sizeof(desc));
          getnextdesc = frqchkdesc(desc);
        }
      }

      // Check for announcement format "Area:" and skip it if found
      if(strnieql(ptr, "Area", 4)) {
        ptr1 = strskip_txt(ptr);
        ptr2 = strskip_wht(ptr1);
        if((*(ptr1-1) == ':') or (*ptr2 == ':')) {
          continue;
        }
      }

      // Get filename from Ticket announcement if detected
      if(not *file and (strlen(txtptr) >= 54)) {
        if(strneql(txtptr+45, " Origin: ", 9)) {
          if(/*(*/ strstr(txtptr+25, " Bytes) ")) {
            if(txtptr[16] == '/' and txtptr[19] == '/' and txtptr[24] == '(' /*)*/) {
              ptr1 = strskip_wht(txtptr);
              ptr2 = strskip_txt(ptr1);
#if defined(__USE_ALLOCA__)
              char *tmpbuf = (char*)alloca(ptr2-ptr1+1);
#else
              __extension__ char tmpbuf[ptr2-ptr1+1];
#endif
              strxcpy(tmpbuf, ptr1, ptr2-ptr1+1);
              frqgetfile(file, desc, filesize, tmpbuf);
              *desc = NUL;  // Description never comes before filename
              gotticket = true;
              continue;
            }
          }
        }
      }

      // Handle blank lines
      if(strblank(ptr)) {
        if(*file)
          strcpy(desc, " ");
        if(*desc and strblank(file))
          *desc = NUL;
      }

      // Get file based on extension (general algorithm)
      if(not *file) {
        if((ptr-txtptr) <= 4) {
          getnextdesc = frqgetfile(file, desc, filesize, ptr);
          if((ptr2 = strrchr(file,'.'))!=NULL and strlen(ptr2)>4) {
            *file = *desc = *filesize = NUL;
            getnextdesc = false;
          }
        }
      }

      // Get file based on FRQEXT list
      if(not *file) {
        int gotone = false;
        ptr2 = strchr(ptr, '.');
        while(ptr2) {
          gstrarray::iterator e;
          for(e = CFG->frqext.begin(); e != CFG->frqext.end(); e++) {
            if(strnicmpw(e->c_str(), ptr2, MinV(e->length(), strlen(ptr2))) == 0) {
              // Find beginning of filename
              const char* ptr3 = ptr2-1;
              while((ptr3 > ptr) and not invalidfilechar(*ptr3))
                ptr3--;
              if(invalidfilechar(*ptr3))
                ptr3++;
              getnextdesc = frqgetfile(file, desc, filesize, ptr3);
              if(*file)
                gotone = true;
              break;
            }
          }
          if(gotone)
            break;
          ptr2 = strchr(ptr2+1, '.');
        }
      }

      // Do we have a complete announcement?
      if(*file and *desc) {

        // Yes, so add it to the list
        freqfile = (char**)throw_realloc(freqfile, (freqfiles+3)*sizeof(char*));
        sprintf(buf, " %-12s %8s %s", file, filesize, desc);
        strsetsz(buf, 76);
        freqfile[freqfiles] = throw_strdup(buf);
        *desc = *file = *filesize = NUL;
        numlines = 0;
        freqfiles++;
      }

      // Maybe there was a false match, so re-initialize if
      // more than 3 lines are checked.
      if(*file and not *desc) {
        if(numlines++ > 2) {
          *desc = *file = *filesize = NUL;
          numlines = 0;
        }
      }
    }

    // Add the filelist
    if(not (CFG->frqoptions & FREQ_NOFILES)) {
      strcpy(file, "FILES");
      sprintf(desc, LNG->FilelistFrom, msg->By());
      freqfile = (char**)throw_realloc(freqfile, (freqfiles+3)*sizeof(char*));
      sprintf(buf, " %-12.12s          %-52.52s ", file, desc);
      freqfile[freqfiles] = throw_strdup(buf);
      *desc = *file = NUL;
      freqfiles++;
    }

    w_info(NULL);

    // Let user select the file(s) to be requested
    if(freqfile) {

      // NULL terminate list
      freqfile[freqfiles] = NULL;

      // Sort list if requested
      if(CFG->frqoptions & FREQ_SORT)
        qsort(freqfile, freqfiles, sizeof(char*), (StdCmpCP)FreqCmp);

      // Run the picker
      int items = MinV(freqfiles, (MAXROW-10));
      set_title(LNG->FreqMenuTitle, TCENTER, C_ASKT);
      update_statusline(LNG->FreqStat);
      whelppcat(H_FileRequest);
      wpickstr_tag = '+';
      int crsr = wpickstr(6, 0, 6+items+1, -1, W_BASK, C_ASKB, C_ASKW, C_ASKS, freqfile, 0, title_shadow);
      wpickstr_tag = false;
      int freqs = 0;
      esc = ((crsr == -1) and (gwin.werrno == W_ESCPRESS));
      whelpop();

      if(not esc) {

        // Open a FILES.BBS in the INBOUNDPATH
        const char* fbfn = AddPath(CFG->inboundpath, "files.bbs");
        int fh = sopen(fbfn, O_RDWR|O_CREAT|O_APPEND|O_TEXT, CFG->sharemode, S_STDRW);
        if(fh == -1) {
          w_infof(LNG->CouldNotOpen, fbfn);
          waitkeyt(10000);
          w_info(NULL);
        }

        // Handle picked files
        msg->re[0] = NUL;
        for(n=0; n<freqfiles; n++) {
          if(*freqfile[n] != ' ') {
            msg->attr.frq1();
            ptr = freqfile[n]+1;      //  01234567890123456
            ptr2 = strskip_txt(ptr);
#if defined(__USE_ALLOCA__)
            char *tmpbuf = (char*)alloca(ptr2-ptr+1);
#else
            __extension__ char tmpbuf[ptr2-ptr+1];
#endif
            strxcpy(tmpbuf, ptr, ptr2-ptr+1);
            ptr2 = strskip_wht(ptr2);
            if((strlen(msg->re) + strlen(tmpbuf)) < sizeof(ISub)) {  // We can only fill one subject line in this version...
              strcat(msg->re, tmpbuf);
              strcat(msg->re, " ");
              if(fh != -1) {
                sprintf(buf, "%-12s %s\n", tmpbuf, ptr2);
                write(fh, buf, strlen(buf));
              }
              freqs++;
            }
          }
        }
        if(freqs == 0) {
          // AARRRGGGHH!!  More bloody duplicate code :-((
          msg->attr.frq1();
          ptr = freqfile[crsr]+1;      //  01234567890123456
          ptr2 = strskip_txt(ptr);
#if defined(__USE_ALLOCA__)
          char *tmpbuf = (char*)alloca(ptr2-ptr+1);
#else
          __extension__ char tmpbuf[ptr2-ptr+1];
#endif
          strxcpy(tmpbuf, ptr, ptr2-ptr+1);
          ptr2 = strskip_wht(ptr2);
          if((strlen(msg->re) + strlen(tmpbuf)) < sizeof(ISub)) {  // We can only fill one subject line in this version...
            strcat(msg->re, tmpbuf);
            strcat(msg->re, " ");
            if(fh != -1) {
              sprintf(buf, "%-12s %s\n", tmpbuf, ptr2);
              write(fh, buf, strlen(buf));
            }
            freqs++;
          }
        }
        strtrim(msg->re);

        // Close the FILES.BBS
        if(fh != -1)
          close(fh);
      }
    }
    else {
      w_info(LNG->FreqInfoNoFiles);
      gkey key = waitkeyt(10000);
      w_info(NULL);
      if(key == Key_Ent) {
        msg->attr.frq1();
        esc = false;
      }
    }

    if(not esc) {

      // Pick area and put the msg in it
      int destarea = CurrArea;
      reader_topline = 0;
      AA->attr().hex0();
      if(*AA->Areafreqto()) {
        for(n=0; (uint) n<AL.size(); n++) {
          if(strieql(AL[n]->echoid(), AA->Areafreqto())) {
            destarea = AL[n]->areaid();
            break;
          }
        }
      }
      if(not AA->Areafreqdirect())
        destarea = AreaPick(LNG->FreqArea, 6, &destarea);
      if(destarea != -1) {
        AL.SetActiveAreaId(destarea);
        if(CurrArea != OrigArea)
          AA->Open();
        if(CFG->frqoptions & FREQ_FAST)
          savedirect = true;
        std::vector<FrqNodeMap>::iterator fnm;
        for(fnm = CFG->frqnodemap.begin(); fnm != CFG->frqnodemap.end(); fnm++) {
          if(fnm->from.equals(msg->orig)) {
            msg->orig = fnm->to;
            break;
          }
        }
        ReplyMsg();
        savedirect = false;
        if(CurrArea != OrigArea) {
          AA->Close();
          AL.SetActiveAreaId(OrigArea);
        }
      }
    }

    if(freqfile) {
      for(n=0; n<freqfiles; n++)
        throw_release(freqfile[n]);
      throw_free(freqfile);
    }

    if(gkbd.quitall)
      QuitNow();
  }

  LoadMessage(msg, CFG->dispmargin-(int)CFG->switches.get(disppagebar));
  reader_topline = oldtopline;
  reader_keyok = YES;

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

