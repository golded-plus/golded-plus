
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
//  Various major job handling funcs
//  ------------------------------------------------------------------

#include <golded.h>
#include <gutlclip.h>

//  ------------------------------------------------------------------

static FILE* prnfp;
static int prnheader;


//  ------------------------------------------------------------------

void SaveLines(int mode, const char* savefile, GMsg* msg, int margin, bool clip) {

  int prn=NO;
  char fnam[GMAXPATH];
  char* prnacc;

  if(mode == MODE_APPEND) {
    prnacc = "at";
    mode = MODE_WRITE;
  }
  else
    prnacc = "wt";

  strcpy(fnam, "PRN");
  if(mode == MODE_WRITE and streql(savefile, "\001PRN"))
    prn = YES;
  else {
    strcpy(fnam, savefile);
    strschg_environ(fnam);
    prnfp = fsopen(fnam, prnacc, CFG->sharemode);
  }
  int lines=0;
  if(prnfp) {
#ifdef OLD_STYLE_HEADER
    if(mode == MODE_WRITE) {
      if(prnheader) {
        DispHeader(msg, prn, prnfp, margin);
        if(prn)
          lines = 6;
      }
    }
#else
    TemplateToText(((mode == MODE_WRITE) and prnheader) ? ((prnheader & WRITE_ONLY_HEADER) ? MODE_HEADER : MODE_WRITEHEADER) : MODE_WRITE, msg, msg, AA->Tpl(), CurrArea);
    msg->TextToLines(-margin);
#endif
    int n = 0;
    Line** lin = msg->line;
    if(lin) {
      Line* line = lin[n];
      while(line) {
        uint lineisctrl = line->type & (GLINE_TEAR|GLINE_ORIG|GLINE_KLUDGE);
        if(not ((mode == MODE_SAVENOCTRL) and lineisctrl)) {
          string::iterator p = line->txt.begin();
          while(p != line->txt.end()) {
            if(mode == MODE_WRITE) {
              // Replace control codes, except the ANSI escape code
              if(iscntrl(*p)) {
                // only allow ESC in file write
                if(prn or (*p != '\x1B')) {
                  *p = (*p == CTRL_A) ? '@' : '.';
                }
              }
            }
            p++;
          }
          const char *ptr = line->txt.c_str();
          fwrite(ptr, strlen(ptr), 1, prnfp);
          if(mode == MODE_NEW) {
            if(EDIT->HardLines()) {
              if(line->type & GLINE_HARD) {
                if(not ((line->type & (GLINE_TEAR|GLINE_ORIG|GLINE_KLUDGE|GLINE_QUOT)) or strblank(ptr))) {
                  fwrite(EDIT->HardLine(), strlen(EDIT->HardLine()), 1, prnfp);
                }
              }
            }
          }
          fwrite(prn ? NL : "\n", prn ? sizeof(NL) : 1, 1, prnfp);
          if(prn) {
            lines++;
            if(lines%CFG->printlength == 0 and CFG->switches.get(printformfeed)) {
              fwrite("\f", 1, 1, prnfp);
            }
          }
        }
        line = lin[++n];
      }
    }
    // Add an empty line and formfeed at the bottom
    if(mode == MODE_WRITE) {
      fwrite(prn ? NL : "\n", prn ? sizeof(NL) : 1, 1, prnfp);
    }
    // Add formfeed if requested
    if((prn and CFG->switches.get(printformfeed)) or
       (not prn and not clip and CFG->switches.get(formfeedseparator))) {
      fwrite("\f", 1, 1, prnfp);
    }
    if(not prn) {
      fclose(prnfp);
      prnfp = NULL;
    }
  }
  else {
    char buf[256];
    sprintf(buf, LNG->CouldNotOpen, fnam);
    w_info(buf);
    waitkeyt(10000);
    w_info(NULL);
  }
}


//  ------------------------------------------------------------------

static void WriteMsgs(GMsg* msg) {

  int prnmargin;

  if(AA->Msgn.Tags() == 0)
    return;

  GFTRK("WriteMsgs");

  char buf[256];
  char fname[GMAXPATH], ofname[GMAXPATH];

  int overwrite = NO;

  prnfp = NULL;

  GMenuDomarks MenuDomarks;
  int source = AA->Mark.Count() ? MenuDomarks.Run(LNG->Write) : WRITE_CURRENT;

  if(source != WRITE_QUIT) {

    GMenuWriteMsg MenuWriteMsg;
    int target = MenuWriteMsg.Run();

    if(target != WRITE_QUIT) {

      prnheader = (target & (WRITE_NO_HEADER|WRITE_ONLY_HEADER)) ^ WRITE_NO_HEADER;

      if(target & WRITE_PRINTER)
        prnmargin = CFG->printmargin;
      else
        prnmargin = 79;

      if(source == WRITE_MARKED) {
        if(target & WRITE_FILE) {
          do {
            overwrite = NO;
            strcpy(CFG->outputfile, AA->Outputfile());
            if(not edit_pathname(CFG->outputfile, sizeof(Path), LNG->WriteMsgs, H_WriteMessage))
              goto Finish;
            if(is_dir(CFG->outputfile)) {
              AddBackslash(CFG->outputfile);
              strcat(CFG->outputfile, "golded.txt");
            }
            AA->SetOutputfile(CFG->outputfile);
            w_infof(" %s ", AA->Outputfile());
            if(stricmp(AA->Outputfile(), "PRN") and strnicmp(AA->Outputfile(), "LPT", 3))
              if(fexist(AA->Outputfile())) {
                GMenuOverwrite MenuOverwrite;
                overwrite = MenuOverwrite.Run();
              }
          } while(overwrite == -1);
        }
        else if(target & WRITE_PRINTER) {
          #ifdef __UNIX__
          prnfp = popen(CFG->printdevice, "w");
          #else
          prnfp = fsopen(CFG->printdevice, "wt", CFG->sharemode);
          #endif
          if(prnfp)
            fwrite(CFG->printinit+1, CFG->printinit[0], 1, prnfp);
        }
        else if(target & WRITE_CLIPBRD) {
          overwrite = YES;
          strcpy(ofname, AA->Outputfile());
          mktemp(strcpy(fname, AddPath(CFG->goldpath, "GDXXXXXX")));
          AA->SetOutputfile(fname);
        }
        w_info(NULL);
        w_progress(MODE_NEW, C_INFOW, 0, AA->Mark.Count(), LNG->Writing);
        for(uint n=0; n<AA->Mark.Count(); n++) {
          if(overwrite and n)
            overwrite = NO;  // Overwrite only the first time
          if(kbxhit()) {
            if(kbxget() == Key_Esc) {
              HandleGEvent(EVTT_JOBFAILED);
              break;
            }
          }
          update_statuslinef(LNG->WritingMsg, n+1, AA->Mark.Count());
          w_progress(MODE_UPDATE, C_INFOW, n+1, AA->Mark.Count(), LNG->Writing);
          AA->LoadMsg(msg, AA->Mark[n], prnmargin);
          if(target & WRITE_PRINTER) {
            if(prnfp)
              SaveLines(MODE_WRITE, "\001PRN", msg, prnmargin);
          }
          else {
            SaveLines(overwrite ? MODE_WRITE : MODE_APPEND, AA->Outputfile(), msg, prnmargin, (target & WRITE_CLIPBRD) ? true : false);
          }
        }
        if(prnfp)
          fwrite(CFG->printreset+1, CFG->printreset[0], 1, prnfp);
        if(target & WRITE_CLIPBRD) {
          AA->SetOutputfile(ofname);

          gclipbrd clipbrd;
          gfile fp;

          if(fp.fopen(fname, "rb")) {

            long len = fp.filelength();
            char* buf = (char*) throw_malloc(len+1);
            buf[len] = NUL;
            fp.fread(buf, 1, len);

            clipbrd.writeclipbrd(buf);

            throw_free(buf);

            fp.close();
          }

          remove(fname);
        }
        w_progress(MODE_QUIT, 0, 0, 0, NULL);
      }
      else if(source == WRITE_CURRENT) {
        if(target & WRITE_FILE) {
          do {
            overwrite = NO;
            strcpy(CFG->outputfile, AA->Outputfile());
            if(edit_pathname(CFG->outputfile, sizeof(Path), LNG->WriteMsgs, H_WriteMessage)) {
              if(is_dir(CFG->outputfile)) {
                AddBackslash(CFG->outputfile);
                strcat(CFG->outputfile, "golded.txt");
              }
              AA->SetOutputfile(CFG->outputfile);
              w_infof(" %s ", AA->Outputfile());
              if(stricmp(AA->Outputfile(), "PRN") and strnicmp(AA->Outputfile(), "LPT", 3)) {
                if(fexist(AA->Outputfile())) {
                  GMenuOverwrite MenuOverwrite;
                  overwrite = MenuOverwrite.Run();
                  if(overwrite == -1)   // Escape was hit
                    continue;
                }
              }
              sprintf(buf, LNG->WritingFile, AA->Outputfile());
              w_info(buf);
              AA->LoadMsg(msg, msg->msgno, prnmargin);
              SaveLines(overwrite ? MODE_WRITE : MODE_APPEND, AA->Outputfile(), msg, prnmargin);
              w_info(NULL);
            }
          } while(overwrite == -1);
        }
        else if(target & WRITE_PRINTER) {
          w_info(LNG->WritingPRN);
          AA->LoadMsg(msg, msg->msgno, prnmargin);
          #ifdef __UNIX__
          prnfp = popen(CFG->printdevice, "w");
          #else
          prnfp = fsopen(CFG->printdevice, "wt", CFG->sharemode);
          #endif
          if(prnfp) {
            fwrite(CFG->printinit+1, CFG->printinit[0], 1, prnfp);
            SaveLines(MODE_WRITE, "\001PRN", msg, prnmargin);
            fwrite(CFG->printreset+1, CFG->printreset[0], 1, prnfp);
          }
          w_info(NULL);
        }
        else if(target & WRITE_CLIPBRD) {
          w_info(LNG->Wait);

          mktemp(strcpy(fname, AddPath(CFG->goldpath, "GDXXXXXX")));

          AA->LoadMsg(msg, msg->msgno, prnmargin);
          SaveLines(MODE_WRITE, fname, msg, prnmargin, true);

          gclipbrd clipbrd;
          gfile fp;

          if(fp.fopen(fname, "rb")) {

            long len = fp.filelength();
            char* buf = (char*) throw_malloc(len+1);
            buf[len] = NUL;
            fp.fread(buf, 1, len);

            clipbrd.writeclipbrd(buf);

            throw_free(buf);

            fp.close();
          }

          remove(fname);
          w_info(NULL);
        }
      }
    }
  }

Finish:

  w_info(NULL);

  if(prnfp) {
    #ifdef __UNIX__
    pclose(prnfp);
    #else
    fclose(prnfp);
    #endif
    prnfp = NULL;
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void WriteMsg(GMsg* msg) {

  if(AA->Msgn.Count())
    WriteMsgs(msg);
}


//  ------------------------------------------------------------------
//  Get name of current quotebuffer filename

char* GetCurrQuotebuf(char* quotebuf) {

  if(*AA->Quotebuffile()) {
    strcpy(quotebuf, AA->Quotebuffile());
    MakePathname(quotebuf, CFG->goldpath, quotebuf);
  }
  else {
    if(AA->isezycom())
      sprintf(quotebuf, "%sgld%05u.qbf", CFG->ezycom.msgbasepath, AA->board());
    else if(AA->isfido())
      sprintf(quotebuf, "%s%s", AA->path(), "golded.qbf");
    else if(AA->isgoldbase())
      sprintf(quotebuf, "%sgoldg%03u.qbf", CFG->goldbasepath, AA->board());
    else if(AA->ishudson())
      sprintf(quotebuf, "%sgoldh%03u.qbf", CFG->hudsonpath, AA->board());
    else
      sprintf(quotebuf, "%s%s", AA->path(), ".qbf");
  }

  return quotebuf;
}


//  ------------------------------------------------------------------

void QuoteBuf(GMsg* msg) {

  if(AA->Msgn.Count()) {

    Path quotebuf;
    char openmode[4];

    TemplateToText(MODE_QUOTEBUF, msg, msg, AA->Tpl(), CurrArea);
    msg->attr.tou1();
    msg->TextToLines(-CFG->quotemargin);
    msg->attr.tou0();
    msg->charsetlevel = LoadCharset(CFG->xlatlocalset, CFG->xlatlocalset);
    msg->LinesToText();

    GetCurrQuotebuf(quotebuf);
    w_infof(" %s ", quotebuf);

    switch(CFG->quotebufmode) {

      case QBUF_ASK:
        *openmode = NUL;
        if(fexist(quotebuf)) {
          GMenuOverwrite MenuOverwrite;
          switch(MenuOverwrite.Run()) {
            case true:
              strcpy(openmode, "wt");
              break;
            case false:
              strcpy(openmode, "at");
              break;
            default:
              *openmode = NUL;
          }
        }
        else {
          strcpy(openmode, "wt");
        }
        break;

      case QBUF_APPEND:
        strcpy(openmode, "at");
        break;

      case QBUF_OVERWRITE:
        strcpy(openmode, "wt");
        break;
    }

    if(*openmode) {

      FILE* fp = fsopen(quotebuf, openmode, CFG->sharemode);
      if(fp) {
        strchg(msg->txt, 0x0D, 0x0A);
        fputs(msg->txt, fp);
        fclose(fp);
      }

      HandleGEvent(EVTT_JOBDONE);
      waitkeyt(1000);
    }
    w_info(NULL);
  }
}


//  ------------------------------------------------------------------

