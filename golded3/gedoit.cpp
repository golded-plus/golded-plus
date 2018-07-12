
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

static gfile prnfp;
static int prnheader;


//  ------------------------------------------------------------------

void SaveLines(int mode, const char* savefile, GMsg* msg, int margin, bool clip) {

  int prn=NO;
  char fnam[GMAXPATH];
  char* prnacc;

  if(mode == MODE_APPEND) {
    prnacc = (char *)"at";
    mode = MODE_WRITE;
  }
  else
    prnacc = (char *)"wt";

  strcpy(fnam, "PRN");
  if(mode == MODE_WRITE and streql(savefile, "\001PRN"))
    prn = YES;
  else
  {
    strcpy(fnam, savefile);
    strschg_environ(fnam, sizeof(fnam));
    prnfp.Fopen(fnam, prnacc, CFG->sharemode);
  }
  int lines=0;
  if (prnfp.isopen())
  {
    char *old_msg_txt = throw_strdup(msg->txt);
#ifdef OLD_STYLE_HEADER
    if(mode == MODE_WRITE) {
      if(prnheader) {
        DispHeader(msg, prn, prnfp, margin);
        if(prn)
          lines = 6;
      }
    }
#else
    if(mode == MODE_WRITE) {
      if(AA->LoadMsg(msg, msg->msgno, margin) == false) {
        msg->txt = throw_strdup("");
        msg->TextToLines(margin);
      }
    }
    else if((mode == MODE_SAVE) or (mode == MODE_SAVENOCTRL)) {
      msg->TextToLines(margin);
    }
    TemplateToText(((mode == MODE_WRITE) and prnheader) ? ((prnheader & WRITE_ONLY_HEADER) ? MODE_HEADER : MODE_WRITEHEADER) : MODE_WRITE, msg, msg, AA->WTpl(), CurrArea);
    msg->TextToLines(margin, false);
#endif
    int n = 0;
    Line** lin = msg->line;
    if(lin) {
      Line* line = lin[n];
      while(line) {
        uint lineisctrl = line->type & (GLINE_TEAR|GLINE_ORIG|GLINE_KLUDGE);
        if(not ((mode == MODE_SAVENOCTRL) and lineisctrl)) {
          if(mode == MODE_WRITE) {
            for(std::string::iterator p = line->txt.begin(); p != line->txt.end(); p++) {
              // Replace control codes, except the ANSI escape code
              if(iscntrl(*p)) {
                // only allow ESC in file write
                if(prn or (*p != '\x1B')) {
                  *p = (*p == CTRL_A) ? '@' : '.';
                }
              }
            }
          }
          const char *ptr = line->txt.c_str();
          prnfp.Fwrite(ptr, strlen(ptr));
          if (mode == MODE_NEW)
          {
            if(EDIT->HardLines()) {
              if(line->type & GLINE_HARD) {
                if (not ((line->type & (GLINE_TEAR|GLINE_ORIG|GLINE_KLUDGE|GLINE_QUOT)) or strblank(ptr)))
                {
                  prnfp.Fwrite(EDIT->HardLine(), strlen(EDIT->HardLine()));
                }
              }
            }
          }
          prnfp.Fwrite(prn ? NL : "\n", prn ? sizeof(NL) : 1);
          if (prn)
          {
            lines++;
            if (lines%CFG->printlength == 0 and CFG->switches.get(printformfeed))
              prnfp.Fwrite("\f", 1);
          }
        }
        line = lin[++n];
      }
    }
    // Add an empty line and formfeed at the bottom
    if (mode == MODE_WRITE)
      prnfp.Fwrite(prn ? NL : "\n", prn ? sizeof(NL) : 1);

    // Add formfeed if requested
    if ((prn and CFG->switches.get(printformfeed)) or
       (not prn and not clip and CFG->switches.get(formfeedseparator)))
    {
      prnfp.Fwrite("\f", 1);
    }

    prnfp.Fclose();

    throw_release(msg->txt);
    msg->txt = old_msg_txt;
  }
  else
  {
    char buf[256];
    gsprintf(PRINTF_DECLARE_BUFFER(buf), LNG->CouldNotOpen, fnam);
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
          prnfp.Popen(CFG->printdevice, "w");
          #else
          prnfp.Fopen(CFG->printdevice, "wt", CFG->sharemode);
          #endif
          if (prnfp.isopen())
            prnfp.Fwrite(CFG->printinit+1, CFG->printinit[0]);
        }
        else if(target & WRITE_CLIPBRD) {
          overwrite = YES;
          strcpy(ofname, AA->Outputfile());
          mktemp(strcpy(fname, AddPath(CFG->temppath, "GDXXXXXX")));
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
          update_statuslinef(LNG->WritingMsg, "ST_WRITINGMSG", n+1, AA->Mark.Count());
          w_progress(MODE_UPDATE, C_INFOW, n+1, AA->Mark.Count(), LNG->Writing);
          AA->LoadMsg(msg, AA->Mark[n], prnmargin);
          if (target & WRITE_PRINTER)
          {
            if (prnfp.isopen())
              SaveLines(MODE_WRITE, "\001PRN", msg, prnmargin);
          }
          else {
            SaveLines(overwrite ? MODE_WRITE : MODE_APPEND, AA->Outputfile(), msg, prnmargin, make_bool(target & WRITE_CLIPBRD));
          }
        }
        if (prnfp.isopen())
          prnfp.Fwrite(CFG->printreset+1, CFG->printreset[0]);
        if (target & WRITE_CLIPBRD)
        {
          AA->SetOutputfile(ofname);

          gclipbrd clipbrd;
          gfile fp(fname, "rb");

          if (fp.isopen())
          {
            long len = fp.FileLength();
            char* buf = (char*) throw_malloc(len+1);
            buf[len] = NUL;
            fp.Fread(buf, len);

            clipbrd.writeclipbrd(buf);

            throw_free(buf);

            fp.Fclose();
          }

          remove(fname);
        }
        w_progress(MODE_QUIT, BLACK_|_BLACK, 0, 0, NULL);
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
              gsprintf(PRINTF_DECLARE_BUFFER(buf), LNG->WritingFile, AA->Outputfile());
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
          prnfp.Popen(CFG->printdevice, "w");
          #else
          prnfp.Fopen(CFG->printdevice, "wt", CFG->sharemode);
          #endif
          if (prnfp.isopen())
          {
            prnfp.Fwrite(CFG->printinit+1, CFG->printinit[0]);
            SaveLines(MODE_WRITE, "\001PRN", msg, prnmargin);
            prnfp.Fwrite(CFG->printreset+1, CFG->printreset[0]);
          }
          w_info(NULL);
        }
        else if(target & WRITE_CLIPBRD) {
          w_info(LNG->Wait);

          mktemp(strcpy(fname, AddPath(CFG->temppath, "GDXXXXXX")));

          AA->LoadMsg(msg, msg->msgno, prnmargin);
          SaveLines(MODE_WRITE, fname, msg, prnmargin, true);

          gclipbrd clipbrd;
          gfile fp(fname, "rb");

          if (fp.isopen())
          {
            long len = fp.FileLength();
            char* buf = (char*) throw_malloc(len+1);
            buf[len] = NUL;
            fp.Fread(buf, len);

            clipbrd.writeclipbrd(buf);

            throw_free(buf);

            fp.Fclose();
          }

          remove(fname);
          w_info(NULL);
        }
      }
    }
  }

Finish:

  w_info(NULL);

  #ifdef __UNIX__
  prnfp.Pclose();
  #else
  prnfp.Fclose();
  #endif

  GFTRK(0);
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
  else
  {
    if (AA->basetype() == "EZYCOM")
      sprintf(quotebuf, "%sgld%05u.qbf", CFG->ezycom.msgbasepath, AA->board());
    else if ((AA->basetype() == "FTS1") || (AA->basetype() == "OPUS"))
      sprintf(quotebuf, "%s%s", AA->path(), "golded.qbf");
    else if (AA->basetype() == "GOLDBASE")
      sprintf(quotebuf, "%sgoldg%03u.qbf", CFG->goldbasepath, AA->board());
    else if (AA->basetype() == "HUDSON")
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
    msg->TextToLines(-CFG->quotemargin, false);
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

    if (*openmode)
    {
      gfile fp(quotebuf, openmode, CFG->sharemode);
      if (fp.isopen())
      {
        strchg(msg->txt, 0x0D, 0x0A);
        fp.Fputs(msg->txt);
      }

      HandleGEvent(EVTT_JOBDONE);
      waitkeyt(1000);
    }
    w_info(NULL);
  }
}


//  ------------------------------------------------------------------

