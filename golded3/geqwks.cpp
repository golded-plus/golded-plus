
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
//  QWK packet handling.
//  ------------------------------------------------------------------

#include <golded.h>
#include <gs_qwk.h>


//  ------------------------------------------------------------------

static void ReadGldFile() {

  if(QWK->FoundBBS()) {

    gfile fp;
    Path gldfile;

    QWK->ResetConfNo();

    sprintf(gldfile, "%s%s.GLD", CFG->goldpath, QWK->BbsID());
    fp.fopen(gldfile, "rt");
    if(fp.isopen()) {
      char* key;
      char* val;
      char buf[256];
      while(fp.fgets(buf, sizeof(buf))) {
        val = strtrim(buf);
        getkeyval(&key, &val);
        strtrim(StripQuotes(val));
        if(QWK->FindEcho(val))
          QWK->ConfNo(atoi(key));
      }
      fp.fclose();
    }
  }
}


//  ------------------------------------------------------------------

int ImportQWK() {

  if(not *QWK->ImportPath())
    return 0;

  gfile fp;
  gfile fpb;   // For BBSID.GLD
  Path file;
  Path gldfile;
  char bbsid[9] = {""};

  // Parse the control file
  strcpy(file, AddPath(QWK->ImportPath(), "CONTROL.DAT"));
  fp.fopen(file, "rt");
  if(fp.isopen()) {
    char buf[256];
    int line = 0;
    int confno = 0;
    int confcnt = 0;
    int confnos = 0;
    while(fp.fgets(buf, sizeof(buf))) {

      line++;
      strtrim(buf);

      if((line >= 12) and (confcnt < confnos)) {
        if(line % 2) {
          if(fpb.isopen())
            fpb.printf("%u \"%s\"\n", confno, buf);
          confcnt++;
        }
        else
          confno = atoi(buf);
      }
      else if(line == 5) {
        char* ptr = strchr(buf, ',');
        if(ptr) {
          strxcpy(bbsid, strskip_wht(ptr+1), 9);
          sprintf(gldfile, "%s%s.GLD", CFG->goldpath, bbsid);
          fpb.fopen(gldfile, "wt");
        }
      }
      else if(line == 11)
        confnos = atoi(buf) + 1;
    }
    if(fpb.isopen())
      fpb.fclose();
    fp.fclose();
    remove(file);
  }

  if(strblank(bbsid))
    return 0;

  QWK->FindBBS(bbsid);
  ReadGldFile();

  int imported = 0;

  OrigArea = CurrArea = -1;

  strcpy(file, AddPath(QWK->ImportPath(), "MESSAGES.DAT"));
  fp.fopen(file, "rb");
  if(fp.isopen()) {

    // Skip past product info header
    fp.fseekset(sizeof(QWKHdr));

    QWKHdr hdr;

    GMsg* msg = (GMsg*)throw_calloc(1, sizeof(GMsg));

    // Read each message, header first
    int tosstobadmsgs = false;
    int lastconfno = -1;
    int more;
    do {

      ResetMsg(msg);

      memset(&hdr, 0, sizeof(QWKHdr));
      more = 1 == fp.fread(&hdr, sizeof(QWKHdr));
      if(more) {

        char blocks[7];
        strxcpy(blocks, hdr.blocks, 7);
        uint msglen = atoi(blocks);
        if(msglen == 0) {
          break;
        }
        msglen = (msglen-1)*128;

        if(hdr.confno != lastconfno) {
          lastconfno = hdr.confno;
          if(QWK->FindEcho(hdr.confno)) {
            int areano = AL.AreaEchoToNo(QWK->EchoID());
            if(areano != -1) {
              CurrArea = AL.AreaNoToId(areano);
              tosstobadmsgs = false;
            }
          }
          else {
            int areano = AL.AreaEchoToNo(QWK->BadMsgs());
            if(areano != -1) {
              CurrArea = AL.AreaNoToId(areano);
              tosstobadmsgs = true;
            }
            else {
              tosstobadmsgs = -1;
              fp.fseek(msglen, SEEK_CUR);
            }
          }
        }

        if(tosstobadmsgs != -1) {

          if(OrigArea != CurrArea) {
            if(AA->isopen()) {
              AA->Unlock();
              AA->Close();
            }
            AL.SetActiveAreaId(CurrArea);
            OrigArea = CurrArea;
            AA->Open();
            AA->Lock();
            AA->RandomizeData();
          }

          // Convert header
          msg->orig = msg->oorig = AA->aka();
          msg->dest = msg->odest = AA->aka();
          msg->attr.pvt(hdr.status == '*' or hdr.status == '+');
          msg->attr.rcv(hdr.status == '+' or hdr.status == '-' or hdr.status == '`' or hdr.status == '^' or hdr.status == '#');
          msg->attr.del(hdr.activestatus == 226);
          strtrim(strxcpy(msg->by, hdr.from, 26));
          strtrim(strxcpy(msg->to, hdr.to, 26));
          strtrim(strxcpy(msg->re, hdr.subject,  26));
          int _year, _month, _day, _hour, _minute;
          sscanf(hdr.date, "%d%*c%d%*c%2d", &_month, &_day, &_year);
          sscanf(hdr.time, "%d%*c%2d", &_hour, &_minute);
          struct tm _tm;
          _tm.tm_year  = (_year < 80) ? (_year+100) : _year;
          _tm.tm_mon   = _month - 1;
          _tm.tm_mday  = _day;
          _tm.tm_hour  = _hour;
          _tm.tm_min   = _minute;
          _tm.tm_sec   = 0;
          _tm.tm_isdst = -1;
          time32_t a    = gmktime(&_tm);
          struct tm *tp = ggmtime(&a);
          tp->tm_isdst  = -1;
          time32_t b    = gmktime(tp);
          msg->written  = a + a - b;
          a = gtime(NULL);
          tp = ggmtime(&a);
          tp->tm_isdst = -1;
          b = gmktime(tp);
          msg->arrived = a + a - b;

          // Read message text
          char* txtptr = msg->txt = (char*)throw_calloc(1, msglen+128);
          if(tosstobadmsgs) {
            sprintf(msg->txt, "AREA:%s_%u\r", bbsid, hdr.confno);
            txtptr += strlen(msg->txt);
          }
          fp.fread(txtptr, msglen);
          strtrim(txtptr);
          strchg(txtptr, 0xE3, 0x0D);

          imported++;
          AA->istossed = true;
          update_statuslinef("%s: %u", AA->echoid(), imported);

          msg->TextToLines(CFG->dispmargin-1);
          if(msg->messageid or msg->references or msg->inreplyto or *msg->ifrom) {
            char kbuf[256];
            uint txtlen = strlen(msg->txt);
            if(*msg->ifrom) {
              INam _fromname;
              IAdr _fromaddr;
              ParseInternetAddr(msg->ifrom, _fromname, _fromaddr);
              if(AA->Internetgate().addr.valid()) {
                char abuf[40];
                sprintf(kbuf, "\x1""REPLYTO %s %s\r""\x1""REPLYADDR %s\r",
                  AA->Internetgate().addr.make_string(abuf),
                  *AA->Internetgate().name ? AA->Internetgate().name : "UUCP",
                  _fromaddr
                );
                uint addlen = strlen(kbuf);
                msg->txt = (char*)throw_realloc(msg->txt, txtlen+addlen+1);
                memmove(msg->txt+addlen, msg->txt, txtlen+1);
                memcpy(msg->txt, kbuf, addlen);
                txtlen += addlen;
              }
            }
            if(msg->references) {
              char* mptr = msg->references;
              char* sptr = mptr;
              while(*mptr) {
                if(not strnieql(mptr, "<NOMSGID_", 9)) {
                  sptr = strpbrk(mptr, " ,");
                  if(sptr == NULL)
                    sptr = mptr + strlen(mptr);
                  while(((*sptr == ' ') or (*sptr == ',')) and *sptr)
                    *sptr++ = NUL;
                  CvtMessageIDtoMSGID(mptr, kbuf, AA->echoid(), "REPLY");
                  strcat(kbuf, "\r");
                  uint addlen = strlen(kbuf);
                  msg->txt = (char*)throw_realloc(msg->txt, txtlen+addlen+1);
                  memmove(msg->txt+addlen, msg->txt, txtlen+1);
                  memcpy(msg->txt, kbuf, addlen);
                  txtlen += addlen;
                }
                mptr = sptr;
              }
            }
            if(msg->inreplyto) {
              if(not strnieql(msg->inreplyto, "<NOMSGID_", 9)) {
                CvtMessageIDtoMSGID(msg->inreplyto, kbuf, AA->echoid(), "REPLY");
                strcat(kbuf, "\r");
                uint addlen = strlen(kbuf);
                msg->txt = (char*)throw_realloc(msg->txt, txtlen+addlen+1);
                memmove(msg->txt+addlen, msg->txt, txtlen+1);
                memcpy(msg->txt, kbuf, addlen);
                txtlen += addlen;
              }
            }
            if(msg->messageid) {
              if(not strnieql(msg->messageid, "<NOMSGID_", 9)) {
                CvtMessageIDtoMSGID(msg->messageid, kbuf, AA->echoid(), "MSGID");
                strcat(kbuf, "\r");
                uint addlen = strlen(kbuf);
                msg->txt = (char*)throw_realloc(msg->txt, txtlen+addlen+1);
                memmove(msg->txt+addlen, msg->txt, txtlen+1);
                memcpy(msg->txt, kbuf, addlen);
              }
            }
          }

          if(*msg->realto and (strieql(msg->to, AA->Whoto()) or not *msg->to))
            strcpy(msg->to, msg->realto);

          AA->SaveMsg(GMSG_NEW, msg);
        }
      }
    } while(more);

    if(AA->isopen()) {
      AA->Unlock();
      AA->Close();
    }

    ResetMsg(msg);
    throw_free(msg);

    fp.fclose();
    remove(file);

    if(*QWK->TossLog()) {
      fp.fopen(QWK->TossLog(), "at");
      if(fp.isopen()) {
        uint na = 0;
        while(na < AL.size()) {
          if(AL[na]->istossed) {
            AL[na]->istossed = false;
            AL[na]->isunreadchg = true;
            fp.printf("%s\n", AL[na]->echoid());
          }
          na++;
        }
        fp.fclose();
      }
    }

    if(imported and *QWK->ReplyLinker()) {
      char buf[256];
      sprintf(buf, LNG->Replylinker, QWK->ReplyLinker());
      ShellToDos(QWK->ReplyLinker(), buf, LGREY|_BLACK, YES);
    }
  }

  if(imported)
    startupscan_success = true;

  return imported;
}


//  ------------------------------------------------------------------

int ExportQwkMsg(GMsg* msg, gfile& fp, int confno, int& pktmsgno) {

  // Prepare for Return Receipt Request
  char msg__re[26];
  if(QWK->ReceiptAllowed() and msg->attr.rrq())
    sprintf(msg__re, "RRR%-22.22s", msg->re);
  else
    strxcpy(msg__re, msg->re, 26);

  // Build QWK header
  QWKHdr hdr;
  char buf[26];
  int tolen = strlen(msg->to);  tolen = MinV(25,tolen);
  int bylen = strlen(msg->by);  bylen = MinV(25,bylen);
  int relen = strlen(msg__re);  relen = MinV(25,relen);
  memset(&hdr, ' ', sizeof(QWKHdr));
  hdr.status = msg->attr.pvt() ? '*' : ' ';
  sprintf(buf, "%u", confno);
  memcpy(hdr.msgno, buf, strlen(buf));
  struct tm* _tm = ggmtime(&msg->written);
  int _year = _tm->tm_year % 100;
  sprintf(buf, "%02d-%02d-%02d", _tm->tm_mon+1, _tm->tm_mday, _year);
  memcpy(hdr.date, buf, 8);
  sprintf(buf, "%02d:%02d", _tm->tm_hour, _tm->tm_min);
  memcpy(hdr.time, buf, 5);
  strxcpy(buf, msg->to, tolen+1);
  if(not QWK->MixCaseAllowed())
    strupr(buf);
  memcpy(hdr.to, buf, tolen);
  strxcpy(buf, msg->by, bylen+1);
  if(not QWK->MixCaseAllowed())
    strupr(buf);
  memcpy(hdr.from, buf, bylen);
  memcpy(hdr.subject, msg__re, relen);
  hdr.activestatus = '\xE1';
  hdr.confno = (word)confno;
  hdr.pktmsgno = (word)++pktmsgno;

  // Write preliminary header
  fp.fwrite(&hdr, sizeof(QWKHdr));

  // Write body
  int level = 0;
  if(CharTable)
    level = CharTable->level ? CharTable->level : 2;

  char mbuf[512];
  uint msglen = 0;

  if(msg->charsetencoding & GCHENC_MNE) {
    if(not striinc("MNEMONIC", CharTable->exp))
      LoadCharset(CFG->xlatlocalset, "MNEMONIC");
  }
  // ASA: Do we need it at all?
  else if(IsQuotedPrintable(msg->charset)) {
    LoadCharset(CFG->xlatlocalset, ExtractPlainCharset(msg->charset));
  }
  else {
    LoadCharset(CFG->xlatlocalset, msg->charset);
  }

  char qwkterm = '\xE3';

  // Process kludges and write header lines
  Line* line = msg->lin;
  while(line) {
    if(line->type & GLINE_KLUDGE) {
      if(AA->isinternet()) {
        if((line->kludge == GKLUD_RFC) or (line->kludge == 0)) {
          XlatStr(mbuf, line->txt.c_str(), level, CharTable);
          msglen += fp.printf("%s%c", mbuf, qwkterm);
        }
        else if(line->type & GLINE_WRAP) {
          while(line->next and (line->type & GLINE_WRAP))
            line = line->next;
        }
      }
      else {
        if((line->type & GLINE_KLUDGE) and QWK->KludgesAllowed()) {
          XlatStr(mbuf, line->txt.c_str(), level, CharTable);
          msglen += fp.printf("%s%c", mbuf, qwkterm);
        }
      }
    }
    line = line->next;
  }

  // Write blank line after header lines
  if(AA->Internetrfcbody()) {
    msglen += fp.printf("%c", qwkterm);
  }

  // Write all message lines
  line = msg->lin;
  while(line) {
    if(not (line->type & GLINE_KLUDGE)) {
      XlatStr(mbuf, line->txt.c_str(), level, CharTable);
      msglen += fp.printf("%s%c", mbuf, qwkterm);
    }
    line = line->next;
  }

  // Calculate blocks
  uint endlen = msglen % 128;
  uint blocks = 1+(msglen/128)+(endlen?1:0);
  sprintf(buf, "%u", blocks);
  memcpy(hdr.blocks, buf, strlen(buf));

  // Write padding spaces at the end if necessary
  if(endlen) {
    char padding[128];
    memset(padding, ' ', 128);
    fp.fwrite(padding, 128-endlen);
  }

  // Re-write the header
  fp.fseek(-(blocks*128), SEEK_CUR);
  fp.fwrite(&hdr, sizeof(QWKHdr));
  fp.fseek((blocks-1)*128, SEEK_CUR);

  // Mark msg as sent
  msg->attr.snt1();
  msg->attr.scn1();
  msg->attr.uns0();
  AA->SaveHdr(GMSG_UPDATE, msg);

  return 1;
}


//  ------------------------------------------------------------------

int ExportQwkArea(int areano, gfile& fp, int confno, int& pktmsgno) {

  int exported = 0;

  AL.SetActiveAreaNo(areano);

  AA->Open();
  AA->Lock();
  AA->RandomizeData();

  GMsg* msg = (GMsg*)throw_calloc(1, sizeof(GMsg));

  for(uint n=0; n<AA->Expo.Count(); n++) {
    if(AA->LoadMsg(msg, AA->Expo[n], 80)) {
      if(msg->attr.uns()) {
        exported += ExportQwkMsg(msg, fp, confno, pktmsgno);
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

int ExportQWK() {

  if(not *QWK->ExportPath())
    return 0;

  int exported = 0;

  gfile fp;
  Path scanfile;

  // Get the scan list
  strcpy(scanfile, AddPath(CFG->goldpath, "GOLDQWK.LST"));
  fp.fopen(scanfile, "rt");
  if(fp.isopen()) {
    char buf[256];
    while(fp.fgets(buf, sizeof(buf))) {
      char* ptr = strchr(buf, ' ');
      if(ptr) {
        *ptr++ = NUL;
        int a = AL.AreaEchoToNo(buf);
        if(a != -1 and AL[a]->isqwk())
          AL[a]->Expo.Add(atol(ptr));
      }
    }
    fp.fclose();
  }

  // Export from the QWK areas
  if(QWK->FirstBBS()) {
    do {
      ReadGldFile();
      Path replyfile;
      int pktmsgno = 0;
      if(QWK->FirstConf()) {
        sprintf(replyfile, "%s%s.MSG", QWK->ExportPath(), QWK->BbsID());
        fp.fopen(replyfile, "wb");
        if(fp.isopen()) {
          char firstrec[128];
          memset(firstrec, ' ', 128);
          memcpy(firstrec, QWK->BbsID(), strlen(QWK->BbsID()));
          fp.fwrite(firstrec, 128);
          pktmsgno = 0;
        }
        do {
          int a = AL.AreaEchoToNo(QWK->EchoID());
          if(a != -1 and AL[a]->Expo.Count()) {
            if(QWK->ConfNo() != -1)
              exported += ExportQwkArea(a, fp, QWK->ConfNo(), pktmsgno);
          }
        } while(QWK->NextConf());
      }
      if(fp.isopen()) {
        fp.fclose();
        if(pktmsgno == 0)
          remove(replyfile);
      }
    } while(QWK->NextBBS());
  }

  // Delete the scanfile
  remove(scanfile);

  return exported;
}


//  ------------------------------------------------------------------

Qwk::Qwk() {

  bbs = bbsp = NULL;
  mapp = NULL;
  bbss = 0;
}


//  ------------------------------------------------------------------

Qwk::~Qwk() {

  Reset();
}


//  ------------------------------------------------------------------

void Qwk::Reset() {

  for(int n=0; n<bbss; n++)
    throw_release(bbs[n].map);
  throw_xrelease(bbs);
  bbsp = NULL;
  mapp = NULL;
  bbss = 0;
}


//  ------------------------------------------------------------------

void Qwk::AddBBS(char* bbsid) {

  bbs = (QwkBbs*)throw_realloc(bbs, (bbss+1)*sizeof(QwkBbs));
  bbsp = bbs + bbss++;
  strxcpy(bbsp->bbsid, bbsid, sizeof(bbsp->bbsid));
  bbsp->kludges = false;
  bbsp->mixcase = false;
  bbsp->receipt = false;
  bbsp->maxlines = -1;
  bbsp->map = NULL;
  bbsp->maps = 0;
  mapp = NULL;
}


//  ------------------------------------------------------------------

void Qwk::AddMap(char* bbsid, char* echoid, char* confname, int confno) {

  FindAddBBS(bbsid);
  bbsp->map = (QwkMap*)throw_realloc(bbsp->map, (bbsp->maps+1)*sizeof(QwkMap));
  mapp = bbsp->map + bbsp->maps++;
  strxcpy(mapp->confname, confname, sizeof(mapp->confname));
  strxcpy(mapp->echoid, echoid, sizeof(mapp->echoid));
  mapp->confno = confno;
}


//  ------------------------------------------------------------------

int Qwk::FindBBS(char* bbsid) {

  bbsp = bbs;
  for(int n=0; n<bbss; n++,bbsp++) {
    if(strieql(bbsid, bbsp->bbsid)) {
      mapp = bbsp->map;
      return true;
    }
  }
  mapp = NULL;
  return false;
}


//  ------------------------------------------------------------------

void Qwk::FindAddBBS(char* bbsid) {

  if(not FindBBS(bbsid))
    AddBBS(bbsid);
}


//  ------------------------------------------------------------------

int Qwk::FirstBBS() {

  if(bbs) {
    bbsp = bbs;
    bbsn = 1;
    return true;
  }

  bbsp = NULL;
  bbsn = 0;
  return false;
}


//  ------------------------------------------------------------------

int Qwk::NextBBS() {

  if(bbsp and (bbsn < bbss)) {
    bbsp++;
    bbsn++;
    return true;
  }

  bbsp = NULL;
  bbsn = 0;
  return false;
}


//  ------------------------------------------------------------------

int Qwk::FindConf(char* echoid) {

  if(bbsp) {
    mapp = bbsp->map;
    for(int n=0; n<bbsp->maps; n++,mapp++)
      if(strieql(echoid, mapp->echoid))
        return true;
  }

  mapp = NULL;
  return false;
}


//  ------------------------------------------------------------------

int Qwk::FindEcho(char* confname) {
           
  if(bbsp) {
    mapp = bbsp->map;
    for(int n=0; n<bbsp->maps; n++,mapp++)
      if(strieql(confname, mapp->confname))
        return true;
  }

  mapp = NULL;
  return false;
}


//  ------------------------------------------------------------------

int Qwk::FindEcho(int confno) {

  if(bbsp) {
    mapp = bbsp->map;
    for(int n=0; n<bbsp->maps; n++,mapp++)
      if(confno == mapp->confno)
        return true;
  }

  mapp = NULL;
  return false;
}


//  ------------------------------------------------------------------

int Qwk::FirstConf() {

  if(bbsp and bbsp->map) {
    mapp = bbsp->map;
    mapn = 1;
    return true;
  }

  mapp = NULL;
  mapn = 0;
  return false;
}


//  ------------------------------------------------------------------

int Qwk::NextConf() {

  if(mapp and (mapn < bbsp->maps)) {
    mapp++;
    mapn++;
    return true;
  }

  mapp = NULL;
  mapn = 0;
  return false;
}


//  ------------------------------------------------------------------

const char* Qwk::BbsID() {

  return bbsp ? bbsp->bbsid : "";
}


//  ------------------------------------------------------------------

const char* Qwk::ConfName() {

  return mapp ? mapp->confname : "";
}


//  ------------------------------------------------------------------

int Qwk::ConfNo(int set) {

  if(mapp) {
    if(set != -2)
      mapp->confno = set;
    return mapp->confno;
  }
  return -1;
}


//  ------------------------------------------------------------------

const char* Qwk::EchoID() {

  return mapp ? mapp->echoid : "";
}


//  ------------------------------------------------------------------

int Qwk::KludgesAllowed(int set) {

  if(bbsp) {
    if(set != -1)
      bbsp->kludges = set;
    return bbsp->kludges;
  }

  return false;
}


//  ------------------------------------------------------------------

int Qwk::MixCaseAllowed(int set) {

  if(bbsp) {
    if(set != -1)
      bbsp->mixcase = set;
    return bbsp->mixcase;
  }

  return false;
}


//  ------------------------------------------------------------------

int Qwk::ReceiptAllowed(int set) {

  if(bbsp) {
    if(set != -1)
      bbsp->receipt = set;
    return bbsp->receipt;
  }

  return false;
}


//  ------------------------------------------------------------------

int Qwk::MaxLines(int set) {

  if(bbsp) {
    if(set != -1)
      bbsp->maxlines = set;
    return bbsp->maxlines;
  }

  return 0;
}


//  ------------------------------------------------------------------

void Qwk::ResetConfNo(int set) {

  if(bbsp) {
    mapp = bbsp->map;
    for(int n=0; n<bbsp->maps; n++,mapp++)
      mapp->confno = set;
  }
}


//  ------------------------------------------------------------------

char* Qwk::BadMsgs(char* set) {

  if(set)
    strxcpy(cfg.badmsgs, set, sizeof(Echo));

  return cfg.badmsgs;
}


//  ------------------------------------------------------------------

char* Qwk::ExportPath(char* set) {

  if(set)
    PathCopy(cfg.exportpath, set);

  return cfg.exportpath;
}


//  ------------------------------------------------------------------

char* Qwk::ImportPath(char* set) {

  if(set)
    PathCopy(cfg.importpath, set);

  return cfg.importpath;
}


//  ------------------------------------------------------------------

char* Qwk::ReplyLinker(char* set) {

  if(set)
    strxcpy(cfg.replylinker, set, sizeof(Path));

  return cfg.replylinker;
}


//  ------------------------------------------------------------------

char* Qwk::TossLog(char* set) {

  if(set)
    strxcpy(cfg.tosslog, set, sizeof(Path));

  return cfg.tosslog;
}


//  ------------------------------------------------------------------

