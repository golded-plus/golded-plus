
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
//  Internet SOUP packet handling.
//  ------------------------------------------------------------------

#include <fcntl.h>
#include <golded.h>
#include <gesrch.h>


//  ------------------------------------------------------------------

void KludgeDATE(GMsg* msg, const char* ptr);

//  ------------------------------------------------------------------
                      
char* CvtMessageIDtoMSGID(const char* mptr, char* msgidbuf, const char* echoid, char* kludge) {

  sprintf(msgidbuf, "\x1""%s: ", kludge);
  char* bptr = msgidbuf + strlen(msgidbuf);

  if(strnieql(mptr, "<MSGID_", 7)) {
    mptr += 7;
    while((*mptr != '@') and *mptr) {
      if(*mptr == '=') {
        *bptr++ = (char)((xtoi(mptr[1]) << 4) | xtoi(mptr[2]));
        mptr += 3;
      }
      else if(*mptr == '_') {
        *bptr++ = ' ';
        mptr++;
      }
      else {
        *bptr++ = *mptr++;
      }
      if(bptr-msgidbuf > 200) // Check for overrun
        break;
    }
    *bptr = NUL;
  }
  else {

    bool spaces = strchr(mptr, ' ') ? true : false;

    dword crc32 = CRC32_MASK_CCITT;
    crc32 = strCrc32(mptr, NO, crc32);
    crc32 = strCrc32(echoid, YES, crc32);
    crc32 ^= CRC32_MASK_CCITT;

    if(spaces)
      *bptr++ = '\"';
    while(*mptr) {
      if(spaces and (*mptr == '\"'))
        *bptr++ = '\"';
      *bptr++ = *mptr++;
      if(bptr-msgidbuf > (200-9-(spaces?2:0))) // Check for overrun
        break;
    }
    if(spaces)
      *bptr++ = '\"';
    sprintf(bptr, " %08lx", crc32);
  }

  return msgidbuf;
}


//  ------------------------------------------------------------------

char* UnfoldLine(char* mptr) {

  char* eptr = mptr;
  do {
    eptr = strskip_to(eptr, '\n');
    if(*eptr) {
      char lwsp = eptr[1];
      if((lwsp == ' ') or (lwsp == '\t'))
        *eptr = lwsp;
      else {
        *eptr++ = NUL;
        break;
      }
    }
  } while(*eptr);

  return eptr;
}

//  ------------------------------------------------------------------

int CheckMailinglists(GMsg* msg, int current) {

  if(AA->isemail()) {
    std::vector<MailList>::iterator z;
    for(z = CFG->mailinglist.begin(); z != CFG->mailinglist.end(); z++)
      if(z->sender_is_pattern) {
        golded_search_manager srchmgr;
        srchmgr.prepare_from_string(z->sender, GFIND_HDRTXT);
        bool success = srchmgr.search(msg, true, true);
        if(srchmgr.reverse ? not success : success) {
          int areano = AL.AreaEchoToNo(z->echoid);
          if(areano != -1)
            return AL.AreaNoToId(areano);
        }
      }
  }
  return current;
}


//  ------------------------------------------------------------------

int CheckMailinglists(const char* what, int current) {

  if(AA->isemail()) {
    std::vector<MailList>::iterator z;
    for(z = CFG->mailinglist.begin(); z != CFG->mailinglist.end(); z++)
      if(not z->sender_is_pattern and strieql(what, z->sender)) {
        int areano = AL.AreaEchoToNo(z->echoid);
        if(areano != -1)
          return AL.AreaNoToId(areano);
      }
  }
  return current;
}


//  ------------------------------------------------------------------

bool MatchRFC(char*& p, const char* what) {

  bool match = strnieql(p, what, strlen(what));

  if(match)
    p = strskip_wht(p + strlen(what) - 1);

  return match;
}


//  ------------------------------------------------------------------

void ProcessSoupMsg(char* lbuf, GMsg* msg, int& msgs, char* areaname, int tosstobadmsgs) {

  int entryCurrArea = CurrArea;

  if(msg->txt) {

    msgs++;

    msg->orig = msg->oorig = CFG->internetgate.addr.valid() ? CFG->internetgate.addr : AA->aka();
    msg->dest = msg->odest = AA->aka();
    time_t a = time(NULL);
    struct tm *tp = gmtime(&a);
    tp->tm_isdst = -1;
    time_t b = mktime(tp);
    msg->arrived = a + a - b;

    Line* line = NULL;
    Line* fline = NULL;
    int lineno = 0;
    int inhdr = true;
    char* mptr = msg->txt;
    char smsgid[2010]; *smsgid = NUL;
    char sreply[2010]; *sreply= NUL;

    if(tosstobadmsgs)
      AddLineF(line, "AREA:%s", areaname);

    while(*mptr) {

      char* eptr = mptr;
      do {
        eptr = strskip_to(eptr, '\n');
        if(*eptr) {
          char lwsp = eptr[1];
          if(inhdr and (mptr != eptr) and ((lwsp == ' ') or (lwsp == '\t'))) {
            *eptr = lwsp;
          }
          else {
            *eptr++ = NUL;
            break;
          }
        }
      } while(*eptr);

      strchg(mptr, '\t', ' ');

      if(inhdr) {
        if(*mptr == NUL) {
          inhdr = false;

          CurrArea = CheckMailinglists(msg, CurrArea);                    
          
          const char* echo_id = AL.AreaIdToPtr(CurrArea)->echoid();
          fline = FirstLine(line);
          char buf[2010];

          if(*smsgid) {
            CvtMessageIDtoMSGID(smsgid, buf, echo_id, "MSGID");
            fline = AddKludge(fline, buf);
          }
          if(*sreply) {
            CvtMessageIDtoMSGID(sreply, buf, echo_id, "REPLY");
            fline = AddKludge(fline, buf);
          }
        }
        else {

          int addkludge = true;
          *lbuf = CTRL_A;
          strcpy(lbuf+1, mptr);

          if(MatchRFC(mptr, "From: ")) {
            INam fromname;
            IAdr fromaddr;
            ParseInternetAddr(mptr, fromname, fromaddr, false);
            strxcpy(msg->by, *fromname ? fromname : fromaddr, sizeof(msg->by));
            if(AA->Internetgate().addr.valid()) {
              char abuf[40];
              char kbuf[2048];
              sprintf(kbuf, "\x1""REPLYTO %s %s",
                AA->Internetgate().addr.make_string(abuf),
                *AA->Internetgate().name ? AA->Internetgate().name : "UUCP"
              );
              line = AddKludge(line, kbuf);
              sprintf(kbuf, "\x1""REPLYADDR %s", fromaddr);
              line = AddKludge(line, kbuf);
            }
            CurrArea = CheckMailinglists(fromaddr, CurrArea);
          }
          else if(MatchRFC(mptr, "To: ")) {
            strxcpy(msg->ito, mptr, sizeof(msg->ito));
            INam toname;
            IAdr toaddr;
            ParseInternetAddr(mptr, toname, toaddr, false);
            strxcpy(msg->to, *toname ? toname : toaddr, sizeof(msg->to));
          }
          else if(MatchRFC(mptr, "X-Comment-To: ")) {
            strxcpy(msg->to, mptr, sizeof(msg->to));
          }
          else if(MatchRFC(mptr, "Cc: ")) {
            char* ccbuf = (char*)throw_malloc(strlen(msg->icc) + strlen(mptr) + 3);
            strcpy(stpcpy(stpcpy(ccbuf, msg->icc), *msg->icc ? ", " : ""), mptr);
            strxcpy(msg->icc, ccbuf, sizeof(msg->icc));
            throw_free(ccbuf);
          }
          else if(MatchRFC(mptr, "Bcc: ")) {
            char* bccbuf = (char*)throw_malloc(strlen(msg->ibcc) + strlen(mptr) + 3);
            strcpy(stpcpy(stpcpy(bccbuf, msg->ibcc), *msg->ibcc ? ", " : ""), mptr);
            strxcpy(msg->ibcc, bccbuf, sizeof(msg->ibcc));
            throw_free(bccbuf);
          }
          else if(MatchRFC(mptr, "X-To: ")) {
            INam toname;
            IAdr toaddr;
            ParseInternetAddr(mptr, toname, toaddr, false);
            strxcpy(msg->to, *toname ? toname : toaddr, sizeof(msg->to));
          }
          else if(MatchRFC(mptr, "Subject: ")) {
            strxcpy(msg->re, mptr, sizeof(msg->re));
          }
          else if(MatchRFC(mptr, "Date: ")) {
            KludgeDATE(msg, mptr);
          }
          else if(MatchRFC(mptr, "Organization: ")) {
            // not used
          }
          else if(MatchRFC(mptr, "Message-ID: ")) {
            if(not strnieql(mptr, "<NOMSGID_", 9))
              strxcpy(smsgid, mptr, 201);
          }
          else if(MatchRFC(mptr, "References: ")) {
            while(*mptr) {
              char* sptr = strpbrk(mptr, " ,");
              if(sptr == NULL)
                sptr = mptr + strlen(mptr);
              while(((*sptr == ' ') or (*sptr == ',')) and *sptr)
                *sptr++ = NUL;
              if(not strnieql(mptr, "<NOMSGID_", 9))
                strxcpy(sreply, mptr, 201);
              mptr = sptr;
            }
          }
          else if(MatchRFC(mptr, "In-Reply-To: ")) {
            if(not strnieql(mptr, "<NOMSGID_", 9))
              strxcpy(sreply, mptr, 201);
          }
          else if(MatchRFC(mptr, "Sender: ")) {
            INam sendername;
            IAdr senderaddr;
            ParseInternetAddr(mptr, sendername, senderaddr, false);
            CurrArea = CheckMailinglists(senderaddr, CurrArea);
          }
          else if(MatchRFC(mptr, "X-Mailing-List: ")) {
            INam listname;
            IAdr listaddr;
            ParseInternetAddr(mptr, listname, listaddr, false);
            CurrArea = CheckMailinglists(listaddr, CurrArea);
          }

          if(addkludge)
            line = AddKludge(line, lbuf);
        }
      }
      else {
        line = AddLineFast(line, mptr);
        lineno++;
      }

      mptr = eptr;
    }

    if(not *msg->to)
      strcpy(msg->to, AA->Whoto());

    msg->lin = FirstLine(line);

    if(OrigArea != CurrArea) {
      AA->Unlock();
      AA->Close();
      AL.SetActiveAreaId(CurrArea);
      OrigArea = CurrArea;
      AA->Open();
      AA->Lock();
      AA->RandomizeData();
    }

    if(lineno) {

      AA->istossed = true;
      update_statuslinef("%s: %u", AA->echoid(), msgs);

      msg->LinesToText();
      AA->SaveMsg(GMSG_NEW, msg);
    }

    ResetMsg(msg);

    CurrArea = entryCurrArea;
  }
}


//  ------------------------------------------------------------------

dword swapendian(dword n) {

  byte* c = (byte*)&n;
  n = ((c[0]*256 + c[1])*256 + c[2])*256 + c[3];
  return n;
}


//  ------------------------------------------------------------------

int ImportSOUP() {

  int imported = 0;

  if(*CFG->soupimportpath) {

    const int MBUF_SIZE = 65535;
    const int LBUF_SIZE = 65535;

    gfile fpa;   // For AREAS file
    gfile fpm;   // For *.MSG files

    int importedmsgs = 0;

    Path areasfile;
    strcpy(areasfile, AddPath(CFG->soupimportpath, "AREAS"));
    fpa.fopen(areasfile, "rt");
    if(fpa.isopen()) {

      char buf[2048];

      LoadCharset("N/A", "N/A");

      char* mbuf = (char*)throw_malloc(MBUF_SIZE);
      char* lbuf = (char*)throw_malloc(LBUF_SIZE);

      GMsg* msg = (GMsg*)throw_calloc(1, sizeof(GMsg));

      while(fpa.fgets(buf, sizeof(buf))) {

        char* delim = "\t\n";
        char* prefix   = strtok(buf,  delim);
        char* areaname = strtok(NULL, delim);
        char* encoding = strtok(NULL, delim);

        char msgfmt = *encoding++;
        char idxfmt = (char)(*encoding ? *encoding++ : 0);
        char kind   = (char)(*encoding ? *encoding   : 0);

        int isemail = false;
        int isnews = false;
        switch(msgfmt) {
          case 'M':
            // not supported yet
            break;
          case 'm':
          case 'b':
            isemail = true;
            break;
          case 'u':
            if(kind == 'm') {
              isemail = true;
              break;
            }
          case 'B':
            isnews = true;
            break;
        }

        Path msgfile, idxfile;
        strcpy(stpcpy(msgfile, prefix), ".msg");
        strcpy(msgfile, AddPath(CFG->soupimportpath, msgfile));
        if(idxfmt) {
          strcpy(stpcpy(idxfile, prefix), ".idx");
          strcpy(idxfile, AddPath(CFG->soupimportpath, idxfile));
        }

        strupr(areaname);
        int tosstobadmsgs = false;
        if(isemail)
          areaname = CFG->soupemail;
        int areano = AL.AreaEchoToNo(areaname);
        if(areano == -1) {
          areano = AL.AreaEchoToNo(CFG->soupbadmsgs);
          tosstobadmsgs = true;
        }

        if((areano != -1) and (isemail or isnews)) {

          AL.SetActiveAreaNo(areano);
          OrigArea = CurrArea;

          fpm.fopen(msgfile, "rb");
          if(fpm.isopen()) {

            imported++;

            int msgs = 0;

            AA->Open();
            AA->Lock();
            AA->RandomizeData();

            uint txtlen = 0;
            char* txtptr = NULL;
            uint allocated_len = 0;

            if((msgfmt == 'b') or (msgfmt == 'B')) {

              // Get binary formats

              dword msglen = 0;
              while(fpm.fread(&msglen, 4) == 1) {

                msglen = swapendian(msglen);
                uint msglensz = (uint)msglen;
                if(msglen != msglensz)
                  msglensz--;
                msg->txt = (char*)throw_calloc(1, msglensz+1);
                fpm.fread(msg->txt, msglensz);
                if(msglen != msglensz)
                  fpm.fseek(msglen-msglensz, SEEK_CUR);
                ProcessSoupMsg(lbuf, msg, msgs, areaname, tosstobadmsgs);
              }
            }
            else {

              // Get non-binary formats

              while(fpm.fgets(mbuf, MBUF_SIZE)) {

                if(msgfmt == 'u') {
                  if(strneql(mbuf, "#! rnews ", 9)) {
                    dword msglen = atol(mbuf+9);
                    uint msglensz = (uint)msglen;
                    if(msglen != msglensz)
                      msglensz--;
                    msg->txt = (char*)throw_calloc(1, msglensz+1);
                    fpm.fread(msg->txt, msglensz);
                    if(msglen != msglensz)
                      fpm.fseek(msglen-msglensz, SEEK_CUR);
                    ProcessSoupMsg(lbuf, msg, msgs, areaname, tosstobadmsgs);
                  }
                  else {
                    w_infof(LNG->ErrorInSoup, msgfile);
                    waitkeyt(10000);
                    w_info(NULL);
                    break;
                  }
                }
                else if(msgfmt == 'm') {
                  if(strneql(mbuf, "From ", 5)) {
                    msg->txt = txtptr;
                    txtptr = NULL;
                    txtlen = 0;
                    allocated_len = 0;
                    ProcessSoupMsg(lbuf, msg, msgs, areaname, tosstobadmsgs);
                  }
                  uint len = strlen(mbuf);
                  if((txtlen+len+1) > allocated_len) {
                    if(allocated_len)
                      allocated_len *= 2;
                    else
                      allocated_len = (txtlen+len+1) * 100;
                    txtptr = (char*)throw_realloc(txtptr, allocated_len);
                  }
                  strcpy(txtptr+txtlen, mbuf);
                  txtlen += len;
                }
                else if(msgfmt == 'M') {
                  // Not supported yet
                }
              }

              if(msgfmt == 'm') {
                if(txtptr) {
                  msg->txt = txtptr;
                  txtptr = NULL;
                  ProcessSoupMsg(lbuf, msg, msgs, areaname, tosstobadmsgs);
                }
              }
            }

            throw_free(txtptr);

            AA->Unlock();
            AA->Close();

            if(msgs)
              importedmsgs += msgs;

            fpm.fclose();
          }
        }

        remove(msgfile);
        if(idxfmt)
          remove(idxfile);
      }

      ResetMsg(msg);
      throw_free(msg);

      throw_free(lbuf);
      throw_free(mbuf);

      fpa.fclose();
      remove(areasfile);

      if(*CFG->souptosslog)
        fpa.fopen(CFG->souptosslog, "at");

      for(uint na = 0; na < AL.size(); na++) {
        if(AL[na]->istossed) {
          AL[na]->istossed = false;
          AL[na]->isunreadchg = true;
          if(fpa.isopen())
            fpa.printf("%s\n", AL[na]->echoid());
        }
      }

      if(fpa.isopen())
        fpa.fclose();

      if(importedmsgs and *CFG->soupreplylinker) {
        sprintf(buf, LNG->Replylinker, CFG->soupreplylinker);
        ShellToDos(CFG->soupreplylinker, buf, LGREY|_BLACK, YES);
      }
    }
  }

  if(imported)
    startupscan_success = true;

  return imported;
}


//  ------------------------------------------------------------------

int ExportSoupMsg(GMsg* msg, char* msgfile, gfile& fp, int ismail) {

  NW(ismail);

  if(not fp.isopen()) {
    fp.open(AddPath(CFG->soupexportpath, msgfile), O_RDWR|O_CREAT|O_BINARY, "rb+");
    if(fp.isopen())
      fp.fseek(0, SEEK_END);
  }

  if(fp.isopen()) {

    int level = 0;
    if(CharTable)
      level = CharTable->level ? CharTable->level : 2;

    char mbuf[1030];

    // Write placeholder for message length
    dword msglen = 0xFFFFFFFFL;
    fp.fwrite(&msglen, 4);
    msglen = 0;

    bool qp = false;
    if(msg->charsetencoding & GCHENC_MNE) {
      if(not CharTable or not striinc("MNEMONIC", CharTable->exp))
        LoadCharset(CFG->xlatlocalset, "MNEMONIC");
    }
    else if(IsQuotedPrintable(msg->charset)) {
      qp = true;
      LoadCharset(CFG->xlatlocalset, ExtractPlainCharset(msg->charset));
    }
    else {
      LoadCharset(CFG->xlatlocalset, msg->charset);
    }

    // Process kludges and write header lines
    Line* line = msg->lin;
    while(line) {
      if(line->type & GLINE_KLUDGE) {
        if((line->kludge == GKLUD_RFC) or (line->kludge == 0)) {
          const char *ltxt = line->txt.c_str();
          XlatStr(mbuf, (*ltxt == CTRL_A) ? (ltxt + 1) : ltxt, level, CharTable);
          msglen += fp.printf("%s%s", mbuf, (line->type & GLINE_WRAP) ? "" : "\n");
        }
        else if(line->type & GLINE_WRAP) {
          while(line->next and (line->type & GLINE_WRAP))
            line = line->next;
        }
      }
      line = line->next;
    }

    // Write blank line after header lines
    msglen += fp.printf("\n");

    // Write all message lines
    line = msg->lin;
    while(line) {
      if(not (line->type & (GLINE_KLUDGE|GLINE_TEAR|GLINE_ORIG))) {
        XlatStr(mbuf, line->txt.c_str(), level, CharTable);
        char* mptr = mbuf;
        if(qp and strlen(mptr) > 76) {
          // 12345v7890
          //     =FF    - back one
          //    =FF     - back two
          //   =FF      - okay
          do {
            char* mbeg = mptr;
            mptr += 75;
            if(*(mptr-2) == '=')
              mptr -= 2;
            else if(*(mptr-1) == '=')
              mptr--;
            int mlen = (int)(mptr - mbeg);
            msglen += fp.printf("%*.*s=\n", mlen, mlen, mbeg);
          } while(strlen(mptr) > 76);
        }
        msglen += fp.printf("%s\n", mptr);
      }
      line = line->next;
    }

    // Re-write the correct message length
    fp.fseek(-(msglen+4), SEEK_CUR);
    dword be_msglen = swapendian(msglen);
    fp.fwrite(&be_msglen, 4);
    fp.fseek(msglen, SEEK_CUR);

    msg->attr.snt1();
    msg->attr.scn1();
    msg->attr.uns0();
    time_t a = time(NULL);
    struct tm *tp = gmtime(&a);
    tp->tm_isdst = -1;
    time_t b = mktime(tp);
    msg->arrived = a + a - b;
    AA->SaveHdr(GMSG_UPDATE, msg);
    if(msg->attr.k_s())
      AA->DeleteMsg(msg, DIR_NEXT);

    return 1;
  }

  return 0;
}


//  ------------------------------------------------------------------

int ExportSoupArea(int areano, char* msgfile, gfile& fp, int ismail) {

  int exported = 0;

  AL.SetActiveAreaNo(areano);

  AA->Open();
  AA->Lock();
  AA->RandomizeData();

  GMsg* msg = (GMsg*)throw_calloc(1, sizeof(GMsg));

  for(uint n=0; n<AA->Expo.Count(); n++) {
    if(AA->LoadMsg(msg, AA->Expo[n], CFG->soupexportmargin)) {
      if(msg->attr.uns() and not (msg->attr.del() or msg->attr.lok())) {
        exported += ExportSoupMsg(msg, msgfile, fp, ismail);
        update_statuslinef("%s: %u", AA->echoid(), exported);
      }
    }
  }

  ResetMsg(msg);
  throw_free(msg);

  AA->Unlock();
  AA->Close();

  AA->Expo.ResetAll();

  return exported;
}


//  ------------------------------------------------------------------

int ExportSOUP() {

  int mailexported = 0;
  int newsexported = 0;

  if(*CFG->soupexportpath) {

    Path scanfile;
    gfile fp, mfp, nfp;

    // Get the scan list
    strcpy(scanfile, AddPath(CFG->goldpath, "goldsoup.lst"));
    fp.fopen(scanfile, "rt");
    if(fp.isopen()) {
      char buf[256];
      while(fp.fgets(buf, sizeof(buf))) {
        char* ptr = strchr(buf, ' ');
        if(ptr) {
          *ptr++ = NUL;
          int a = AL.AreaEchoToNo(buf);
          if(a != -1)
            AL[a]->Expo.Add(atol(ptr));
        }
      }
      fp.fclose();
    }

    // Export from the e-mail and newsgroup areas
    for(uint na = 0; na < AL.size(); na++) {
      Area* ap = AL[na];
      if(ap->isemail() and ap->Expo.Count())
        mailexported += ExportSoupArea(na, "GOLDMAIL.MSG", mfp, true);
      else if(ap->isnewsgroup() and ap->Expo.Count())
        newsexported += ExportSoupArea(na, "GOLDNEWS.MSG", nfp, false);
    }

    // Close any open SOUP files
    if(mfp.isopen())
      mfp.fclose();
    if(nfp.isopen())
      nfp.fclose();

    // Update the REPLIES file
    fp.open(AddPath(CFG->soupexportpath, "REPLIES"), O_RDWR|O_CREAT|O_BINARY, "rb+");
    if(fp.isopen()) {
      char buf[512];
      int hasmail = false;
      int hasnews = false;
      while(fp.fgets(buf, sizeof(buf))) {
        strtok(buf, "\t\n");
        if(strieql(buf, "GOLDMAIL"))
          hasmail = true;
        else if(strieql(buf, "GOLDNEWS"))
          hasnews = true;
      }
      if(mailexported and not hasmail)
        fp.printf("GOLDMAIL\tmail\tbn\n");
      if(newsexported and not hasnews)
        fp.printf("GOLDNEWS\tnews\tBn\n");
      fp.fclose();
    }

    // Delete the scanfile
    remove(scanfile);
  }

  return mailexported + newsexported;
}


//  ------------------------------------------------------------------

