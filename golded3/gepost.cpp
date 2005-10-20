
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
//  Mega-functions to handle creation of msgs :-(
//  ------------------------------------------------------------------

#include <golded.h>


//  ------------------------------------------------------------------

static std::vector<int> post_xparea;
bool akamatchreply = false;


//  ------------------------------------------------------------------

std::string &strtrimline(std::string &p) {

  if(not p.empty()) {
    std::string::iterator trail = p.end();
    while(trail != p.begin()) {
      --trail;
      if(not strchr(" \t\r\n", *trail) and not issoftcr(*trail)) {
        ++trail;
        break;
      }
    }
    p.erase(trail, p.end());
  }
  return p;
}


//  ------------------------------------------------------------------

const char* get_subject_re_info(const char* s, ulong& n) {

  if(toupper(*s) == 'R' and tolower(s[1]) == 'e') {

    if(s[2] == ':') {
      n = 1;
      return s + 3;
    }
    else if(s[2] == '^' and isdigit(s[3])) {
      const char* d = s + 4;
      while(isdigit(*d))
        d++;
      if(*d == ':') {
        n = (ulong)atol(s + 3);
        return d + 1;
      }
    }
  }

  n = 0;
  return NULL;
}


//  ------------------------------------------------------------------

void CheckSubject(GMsg* msg, char* subj) {

  for(int n=0; n<fspecs; n++)
    throw_release(fspec[n].fblk);
  fspecs = specfiles = 0;
  fspec = (FileSpec*)throw_realloc(fspec, sizeof(FileSpec));

  if(AA->isnet()) {
    if(EDIT->AutoAttach()) {
      // If the subject was changed
      if(not strieql(subj, msg->re)) {
        // Check if the subj begins with drive spec
        if(not (msg->attr.frq() or msg->attr.att() or msg->attr.urq())) {
          char* ptr = subj;
          if(*ptr == '^')
            ptr++;
          #ifdef __UNIX__
          if((*ptr == '/') and not strchr(ptr, ':')) { /* } */
          #else
          if((in_range((char)tolower(*ptr), (char)'c', (char)'z') and (ptr[1] == ':')) or ((*ptr == '\\') and (ptr[1] == '\\'))) {
          #endif
            // Autoattach
            msg->attr.att1();
            AttrAdd(&msg->attr, &CFG->attribsattach);
            DispHeadAttrs(msg);
          }
        }
      }
    }

    if(msg->attr.frq()) {
      // Handle File Requests
      if(*subj) {
        ISub buf;
        strcpy(buf, subj);
        char* ptr = strtok(buf, " ");
        while(ptr) {
          if(*ptr == '!') {   // Store password
            if(fspecs)
              strcpy(fspec[fspecs-1].password, ptr);
          }
          else {
            fspec = (FileSpec*)throw_realloc(fspec, (fspecs+1)*sizeof(FileSpec));
            memset(fspec+fspecs, 0, sizeof(FileSpec));
            fspec[fspecs].files = 1;
            strxcpy(fspec[fspecs].path, ptr, sizeof(Path));
            specfiles++;
            fspecs++;
          }
          ptr = strtok(NULL, " ");
        }
      }
    }
    else if(msg->attr.att()) {
      // Handle File Attaching
      ISub buf;
      if(strblank(subj)) {
        if(*CFG->attachpath)
          strxcpy(subj, CFG->attachpath, sizeof(ISub));
        else
          strxcpy(subj, CFG->goldpath, sizeof(ISub));
      }
      strcpy(buf, subj);
      char* ptr = strtok(buf, " ");
      while(ptr) {
        fspec = (FileSpec*)throw_realloc(fspec, (fspecs+1)*sizeof(FileSpec));
        memset(fspec+fspecs, 0, sizeof(FileSpec));
        if(*ptr == '^') {
          fspec[fspecs].delsent = YES;
          ptr++;
        }
        if((ptr == CleanFilename(ptr)) and *CFG->attachpath)
          strxmerge(fspec[fspecs].path, sizeof(Path), CFG->attachpath, ptr, NULL);
        else
          strxcpy(fspec[fspecs].path, ptr, sizeof(Path));
        FileSelect(msg, LNG->AttachFiles, &fspec[fspecs]);
        specfiles += fspec[fspecs].files;
        if(fspec[fspecs].files)
          fspecs++;
        ptr = strtok(NULL, " ");
      }
    }
    else if(msg->attr.urq()) {
      // Handle Update Requests
      ISub buf;
      if(not *subj)
        strcpy(subj, ".");
      strcpy(buf, subj);
      char* ptr = strtok(buf, " ");
      while(ptr) {
        if(*ptr == '!') {   // Store password
          if(fspecs)
            strcpy(fspec[fspecs-1].password, ptr);
        }
        else {
          fspec = (FileSpec*)throw_realloc(fspec, (fspecs+1)*sizeof(FileSpec));
          memset(fspec+fspecs, 0, sizeof(FileSpec));
          strxcpy(fspec[fspecs].path, ptr, sizeof(Path));
          FileSelect(msg, LNG->UpdreqFiles, &fspec[fspecs]);
          specfiles += fspec[fspecs].files;
          if(fspec[fspecs].files)
            fspecs++;
        }
        ptr = strtok(NULL, " ");
      }
    }

    // Generate the (first) processed subject line
    if(specfiles) {
      ISub buf;
      *buf = NUL;
      for(int x=0,n=0; x<fspecs; x++) {
        for(int m=0; m<fspec[x].files; m++) {
          ISub subject;
          sprintf(subject, "%s%s%s%s%s ", fspec[x].delsent ? "^" : "", ReMapPath(fspec[x].path), fspec[x].fblk ? (fspec[x].fblk[m].name ? fspec[x].fblk[m].name : "") : "", *fspec[x].password ? " " : "", fspec[x].password);
          if((strlen(buf) + strlen(subject)) > 71) {
            n++;
            break;
          }
          else
            strcat(buf, subject);
        }
        if(n)
          break;
      }
      strtrim(strcpy(subj, buf));
    }
  }
}


//  ------------------------------------------------------------------

static bool have_origin(GMsg *msg) {

  Line* line = msg->lin;

  while(line)
    if(line->type & GLINE_ORIG)
      return true;
    else
      line = line->next;
  return false;
}

//  ------------------------------------------------------------------

static void MakeMsg3(int& mode, GMsg* msg) {

  int n;
  std::vector<gaka>::iterator u;

  msg->charsetlevel = 0;
  if(*AA->Xlatexport()) {
    msg->charsetlevel = LoadCharset(CFG->xlatlocalset, AA->Xlatexport());
    if(msg->charsetlevel)
      sprintf(msg->charset, "%s %d", AA->Xlatexport(), msg->charsetlevel);
    else
      sprintf(msg->charset, "%s 2", CFG->xlatlocalset);
  }

  // Do Timefields
  if(msg->attr.fmu()) {
    time32_t a    = gtime(NULL);
    struct tm *tp = ggmtime(&a);
    tp->tm_isdst  = -1;
    time32_t b    = gmktime(tp);
    a += a - b;
    if(AA->havereceivedstamp())
      msg->received = a;
    if(AA->havearrivedstamp())
      msg->arrived = a;
  }

  GMsg* carbon=NULL;
  int cc = DoCarboncopy(msg, &carbon);

  if(AA->isecho() or have_origin(msg))
    DoTearorig(mode, msg);

  // Do FakeNets
  msg->oorig = msg->orig;
  if(AA->isnet() and msg->orig.point) {
    for(u = CFG->aka.begin(); u != CFG->aka.end(); u++) {
      if(not memcmp(&(*u), &msg->orig, sizeof(Addr))) {
        // Use fakenet to everybody
        if(u->pointnet) {
          msg->oorig.net   = u->pointnet;  // Create fake address
          msg->oorig.node  = msg->orig.point;
          msg->oorig.point = 0;
        }
        break;
      }
    }
  }

  DoKludges(mode, msg);

  if(cc) {
    std::string temp;
    update_statuslinef(LNG->StatusCC, msg->to, msg->dest.make_string(temp).c_str());
  }

  msg->LinesToText();

  if(AA->isnet() and (msg->attr.frq() or msg->attr.att() or msg->attr.urq()) and specfiles >= 1)
    CreateFileMsgs(mode, msg);
  else {
    HeaderView->Use(AA, msg);
    HeaderView->Paint();

    // Do zoneGating
    if(AA->isnet() and CFG->zonegating and (msg->dest.zone != msg->orig.zone)) {
      if(not (msg->attr.dir() or msg->attr.cra() or msg->attr.hld())) {
        GMenuZonegate MenuZonegate;
        if(CFG->zonegating == YES or MenuZonegate.Run())
          ZonegateIt(msg->odest, msg->orig, msg->dest);
      }
    }

    if(msg->odest.net == 0)
      msg->odest = msg->dest;

    HeaderView->Use(AA, msg);
    HeaderView->Paint();
    w_info(LNG->Wait);
    AA->SaveMsg((mode == MODE_CHANGE) ? GMSG_UPDATE : GMSG_NEW, msg);
    if(not (AA->isecho() and mode == MODE_REPLYCOMMENT))
      update_addressbook(msg);
    w_info(NULL);
  }

  if(AL.AreaEchoToNo(AA->Areayouwroteto())>=0) {
    Area* A = AL.AreaEchoToPtr(AA->Areayouwroteto());
    if(AA != A) {
      char* back = NULL;
      uint mlen = 0;

      msg->link.reset();
      char* ptr = msg->txt + (*msg->txt == CTRL_A);
      if(not strnieql(ptr, "AREA:", 5)) {
        uint elen = 6 + strlen(AA->echoid()) + 1;
        mlen = strlen(msg->txt)+1;
        msg->txt = (char*)throw_realloc(msg->txt, elen+mlen);
        back = msg->txt+elen;
        memmove(back, msg->txt, mlen);
        strcpy(msg->txt, "\001AREA:");
        strcpy(msg->txt+6, AA->echoid());
        msg->txt[elen-1] = CR;
      }
      AA->Close();
      A->Open();
      ulong oldmsgno = msg->msgno;
      A->NewMsgno(msg);
      A->SaveMsg(GMSG_NEW|GMSG_NOLSTUPD, msg);
      A->Close();
      AA->Open();
      if(back)
        memmove(msg->txt, back, mlen);
      msg->msgno = oldmsgno;
    }
  }

  if(cc) {

    Area* A = AA;

    if(AA->isecho()) {
      AA = AL.AreaEchoToPtr(AA->Areareplyto());
      if(AA and (AA != A)) {
        AA->Open();
        AA->RandomizeData(mode);
      }
      else
        AA = A;
    }

    for(n=0; n<cc; n++) {
      GMsg* cmsg = &carbon[n];
      cmsg->txt   = msg->txt;
      cmsg->lin   = msg->lin;
      cmsg->line  = msg->line;
      cmsg->lines = msg->lines;
      cmsg->messageid = msg->messageid;
      cmsg->inreplyto = msg->inreplyto;
      cmsg->references = msg->references;
      AA->NewMsgno(cmsg);
      {
        std::string temp;
        update_statuslinef(LNG->StatusCC, cmsg->to, cmsg->dest.make_string(temp).c_str());
      }

      // Do aka matching
      if(AA->Akamatching()) {
        int newaka = AkaMatch(&cmsg->orig, &cmsg->dest);
        cmsg->orig = CFG->aka[newaka].addr;
      }

      // Do FakeNets
      cmsg->oorig = cmsg->orig;
      if(AA->isnet() and cmsg->orig.point) {
        for(u = CFG->aka.begin(); u != CFG->aka.end(); u++) {
          if(not memcmp(&(*u), &cmsg->orig, sizeof(Addr))) {
            // Use fakenet to everybody
            if(u->pointnet) {
              cmsg->oorig.net   = u->pointnet;  // Create fake address
              cmsg->oorig.node  = cmsg->orig.point;
              cmsg->oorig.point = 0;
            }
            break;
          }
        }
      }

      // remove unnessesary To: line from message if present
      for(Line* line = cmsg->lin; line; line = line->next)
        if(strnieql(line->txt.c_str(), "To:", 3))
           line = DeleteLine(line);
        else if(line->type & GLINE_KLUDGE)
          continue;
        else
          break;

      DoKludges(mode, cmsg);

      if(have_origin(cmsg))
        DoTearorig(mode, cmsg);

      cmsg->LinesToText();
      msg->txt   = cmsg->txt;
      msg->lin   = cmsg->lin;
      msg->line  = cmsg->line;
      msg->lines = cmsg->lines;
      msg->messageid  = cmsg->messageid;
      msg->inreplyto  = cmsg->inreplyto;
      msg->references = cmsg->references;
      if(AA->isnet() and (cmsg->attr.frq() or cmsg->attr.att() or cmsg->attr.urq())) {
        CreateFileMsgs(MODE_NEW, cmsg);
        msg->txt   = cmsg->txt;
        msg->lin   = cmsg->lin;
        msg->line  = cmsg->line;
        msg->lines = cmsg->lines;
        msg->messageid  = cmsg->messageid;
        msg->inreplyto  = cmsg->inreplyto;
        msg->references = cmsg->references;
      }
      else {
        HeaderView->Use(AA, cmsg);
        HeaderView->Paint();

        // Do zoneGating
        if(AA->isnet() and CFG->zonegating and (cmsg->dest.zone != cmsg->orig.zone)) {
          if(not cmsg->attr.dir()) {
            GMenuZonegate MenuZonegate;
            if(CFG->zonegating == YES or MenuZonegate.Run())
              ZonegateIt(cmsg->odest, cmsg->orig, cmsg->dest);
          }
        }

        if(cmsg->odest.net == 0)
          cmsg->odest = cmsg->dest;

        HeaderView->Use(AA, cmsg);
        HeaderView->Paint();
        w_info(LNG->Wait);
        AA->SaveMsg(GMSG_NEW, cmsg);
        w_info(NULL);
        update_addressbook(cmsg);
        msg->txt   = cmsg->txt;
        msg->lin   = cmsg->lin;
        msg->line  = cmsg->line;
        msg->lines = cmsg->lines;
        msg->messageid  = cmsg->messageid;
        msg->inreplyto  = cmsg->inreplyto;
        msg->references = cmsg->references;
      }
    }
    throw_release(carbon);

    if(A != AA) {
      AA->Close();
      AA = A;
      AA->RandomizeData();
    }
    AA->UpdateAreadata();
  }
}


//  ------------------------------------------------------------------

static void MakeMsg2(int& mode, int& status, int& forwstat, int& topline, GMsg* msg, GMsg* oldmsg, GMsg* cmpmsg) {

  uint n;
  char buf[256], buf2[256];
  Line* line;
  Line* newline;

  if(status == MODE_CHANGE or (mode == MODE_FORWARD and status == MODE_SAVE)) {
    if(mode == MODE_CHANGE) {
      if(stricmp(msg->By(), AA->Username().name)) {
        std::vector<Node>::iterator x;
        for(n = 0, x = CFG->username.begin(); x != CFG->username.end(); n++, x++) {
          if(strieql(x->name, msg->By())) {
            n = 0;
            break;
          }
        }
        if(n)
          status = MODE_SAVE;
      }
    }
    w_info(LNG->Wait);
    if(mode != MODE_CHANGE) {
      if(not CFG->switches.get(emptytearline))
        strcpy(msg->tearline, AA->Tearline());

      if(AA->Taglinesupport())
        strcpy(msg->tagline, AA->Tagline());

      DoTearorig(mode, msg);

      strcpy(msg->organization, AA->Organization());
    }
    if(mode != MODE_CHANGE or status == MODE_SAVE) {
      if(TemplateToText(mode, msg, oldmsg, AA->Tpl(), OrigArea)) {
        HeaderView->Use(AA, msg);
        HeaderView->Paint();
      }
      msg->attr.pos1();
      msg->TextToLines(CFG->dispmargin-1, false); // Ignore any kludge address found
      msg->attr.pos0();
    }
    uint lineno, position = reader_topline+1;
    for(lineno = 0; lineno < msg->lines; lineno++) {
      if(not (msg->line[lineno]->type & GLINE_QUOT))
        if(msg->line[lineno]->type & GLINE_POSI)
          position = lineno+reader_topline+1;
    }
    if(*EDIT->External() and not EDIT->Internal()) {
      SaveLines(MODE_NEW, AddPath(CFG->goldpath, EDIT->File()), msg, 79);
    }
    int loop = 0;
    w_info(NULL);
    do {
      if(loop) {
        status = MODE_SAVE;
        forwstat = NO;
      }
      if(mode == MODE_FORWARD)
        status = MODE_SAVE;
      if(*EDIT->External() and forwstat == NO and not EDIT->Internal()) {
        strcpy(buf, EDIT->External());
        sprintf(buf2, "%u", position);
        strischg(buf, "@line", buf2);
        strcpy(buf2, AddPath(CFG->goldpath, EDIT->File()));
        strchg(buf2, GOLD_WRONG_SLASH_CHR, GOLD_SLASH_CHR);
        strischg(buf, "@file", buf2);
        long ftbefore = GetFiletime(AddPath(CFG->goldpath, EDIT->File()));
        sprintf(buf2, LNG->EditCmd, buf);
        ShellToDos(buf, buf2, LGREY|_BLACK, YES);
        long ftafter = GetFiletime(AddPath(CFG->goldpath, EDIT->File()));
        if(status != MODE_SAVE) {
          status = MODE_SAVE;
          if(ftbefore == ftafter) {
            if(streql(msg->by, cmpmsg->by))
              if(streql(msg->to, cmpmsg->to))
                if(streql(msg->re, cmpmsg->re))
                  if(not memcmp(&msg->attr, &cmpmsg->attr, sizeof(Attr)))
                    status = MODE_QUIT;
          }
        }
        if(status != MODE_QUIT) {
          LoadCharset("N/A", "N/A");
          strcpy(stpcpy(msg->charset, CFG->xlatlocalset), " 2");
          msg->charsetlevel = 2;
        }
      }
      else if(forwstat == NO and (EDIT->Internal() or not *EDIT->External())) {
        w_info(LNG->Wait);
        status = MODE_SAVE;
        newline = line = msg->lin;
        while(line) {
          newline = line;
          if((line->type & GLINE_KLUD) and not AA->Viewkludge() and not (line->kludge & GKLUD_RFC))
            newline = line = DeleteLine(line);
          else {
            strtrimline(line->txt);
            if(line->type & GLINE_HARD)
              line->txt += "\n";
            else
              line->txt += " ";
            line = line->next;
          }
        }
        msg->lin = FirstLine(newline);

        w_info(NULL);
        if(position)
          position--;
        if(not savedirect)
          status = EditMsg(mode, &position, msg);
        _in_editor = NO;
        w_info(LNG->Wait);

        if(status != MODE_QUIT) {
          line = msg->lin;
          while(line) {
            if(line->txt.find('\n') != line->txt.npos)
              line->type = GLINE_HARD;
            else if(line->next and (line->next->type & GLINE_QUOT))
              line->type = GLINE_HARD;
            else
              line->type = 0;
            strtrimline(line->txt);
            if(AA->isinternet() or *msg->ito or *msg->ifrom) {
              // Check for signature indicator
              if(streql(line->txt.c_str(), "--")) {
                line->txt = "-- ";
                line->type = GLINE_HARD;
              }
            }
            line = line->next;
          }
          LoadCharset("N/A", "N/A");
          strcpy(stpcpy(msg->charset, CFG->xlatlocalset), " 2");
          msg->charsetlevel = 2;
          msg->LinesToText();
        }
        w_info(NULL);
      }
      if(status == MODE_SAVE) {

        if(*EDIT->External() and not EDIT->Internal())
          LoadText(msg, AddPath(CFG->goldpath, EDIT->File()));
        if(mode == MODE_FORWARD)
          msg->attr.pos1();
        int adat_viewhidden = AA->Viewhidden();
        int adat_viewkludge = AA->Viewkludge();
        int adat_viewquote  = AA->Viewquote();
        bool adat_striphtml = AA->StripHTML();
        AA->adat->viewhidden = true;
        AA->adat->viewkludge = true;
        AA->adat->viewquote = true;
        AA->adat->striphtml = false;
        msg->TextToLines(CFG->dispmargin-1, false); // Ignore any kludge address found
        AA->adat->viewhidden = adat_viewhidden;
        AA->adat->viewkludge = adat_viewkludge;
        AA->adat->viewquote = adat_viewquote;
        AA->adat->striphtml = adat_striphtml;
        msg->attr.pos0();

        InvalidateControlInfo(msg);

        if(not savedirect) {
          HeaderView->Use(AA, msg);
          HeaderView->Paint();
          BodyView->Use(AA, msg, topline);
          BodyView->Paint();
          do {
            if(gkbd.quitall)
              status = MODE_SAVE;
            else {
              GMenuEditfile MenuEditfile;
              status = MenuEditfile.Run(msg);
            }
            if(status == MODE_VIEW) {
              BodyView->Use(AA, msg, topline);
              BodyView->Paint();
              ViewMessage();
            }
          } while(status == MODE_VIEW);
        }

        if(status == MODE_SAVE)
          DoCrosspost(msg, post_xparea);
      }
      loop++;
      if(status == MODE_CHANGE) {
        _in_editor = YES;
        HeaderView->Use(AA, msg);
        HeaderView->Paint();
      }
    } while(status == MODE_CHANGE);
  }
}


//  ------------------------------------------------------------------

static void GetLastLink(GMsg* msg, ulong& msgno) {

  GMsg* uplink = (GMsg*)throw_calloc(1, sizeof(GMsg));

  uplink->msgno = msg->msgno;
  uplink->link.first_set(msg->link.first());

  if(uplink->link.first()) {
    while(AA->Msgn.ToReln(uplink->link.first())) {
      if(not AA->LoadHdr(uplink, uplink->link.first(), false))
        uplink->msgno = 0;
    }
  }

  // Return msgno of the last link
  msgno = uplink->msgno;

  ResetMsg(uplink);
  throw_free(uplink);
}


//  ------------------------------------------------------------------

void MakeMsg(int mode, GMsg* omsg, bool ignore_replyto) {

  GFTRK("MakeMsg");

  // Tell GMsgHeaderView not to show msg size
  _in_editor = YES;

  // Allocate some msgs
  GMsg* msg = (GMsg*)throw_calloc(1, sizeof(GMsg));
  GMsg* reply = (GMsg*)throw_calloc(1, sizeof(GMsg));
  GMsg* cmpmsg = (GMsg*)throw_calloc(1, sizeof(GMsg));
  ulong reply_msgno = 0;

  // Keep copy of original aka for later restoration
  Addr origaka = AL.AreaIdToPtr(OrigArea)->aka();

  // Force template choice if necessary
  if(AA->Forcetemplate() and (mode != MODE_CHANGE))
    ChangeTemplate();

  // Random System
  AA->RandomizeData();

  // Prepare confirmation receipts
  int confirm = NO;
  if(mode == MODE_CONFIRM) {
    confirm = YES;
    mode = MODE_REPLY;
  }

  // Prepare crossposting
  int crosspost = NO;
  post_xparea.clear();

  do {
    if(post_xparea.size()) {
      mode = MODE_COPY;
      AL.SetActiveAreaNo(post_xparea.back());
      if(CurrArea != OrigArea) {
        AA->Open();
        AA->RandomizeData(mode);
      }

      msg->TextToLines(CFG->dispmargin-1);
      msg->orig = AA->Aka().addr;
      msg->charsetlevel = LoadCharset(CFG->xlatlocalset, AA->Xlatexport());
      if(msg->charsetlevel)
        sprintf(msg->charset, "%s %d", AA->Xlatexport(), msg->charsetlevel);
      else
        sprintf(msg->charset, "%s 2", CFG->xlatlocalset);
      strcpy(msg->odom, CFG->aka[AkaMatch(&msg->orig, &AA->Aka().addr)].domain);
      if(AA->isecho() or have_origin(msg))
        DoTearorig(mode, msg);
      DoKludges(mode, msg);
      msg->LinesToText();
      post_xparea.pop_back();
    }

    int status = MODE_SAVE;
    int hardlines = EDIT->HardLines();
    int topline = 0;
    fspecs = specfiles = 0;
    fspec = (FileSpec*)throw_realloc(fspec, sizeof(FileSpec));
    memset(fspec, 0, sizeof(FileSpec));

    // Get msg number and reset/copy msg data
    if(mode != MODE_COPY)
      ResetMsg(msg);
    switch(mode) {
      case MODE_FORWARD:
        memcpy(msg, omsg, sizeof(GMsg));
        msg->txt = (char*)throw_strdup(msg->txt);
        msg->lin = NULL;
        msg->line = NULL;
        msg->messageid = NULL;
        msg->inreplyto = NULL;
        msg->references = NULL;
        if(CurrArea != OrigArea)
          AA->SetXlatimport(AL.AreaIdToPtr(OrigArea)->Xlatimport());
        msg->TextToLines(CFG->dispmargin-1, true, false);
        msg->msgid.reset();
        *msg->iorig = NUL;
        *msg->idest = NUL;
        *msg->ireplyto = NUL;
        *msg->iaddr = NUL;
        *msg->igate = NUL;
        *msg->ifrom = NUL;
        *msg->ito = NUL;
        *msg->organization = NUL;
        *msg->realby = NUL;
        *msg->realto = NUL;
        *msg->origin = NUL;
        // Drop through ...
      case MODE_COPY:
        AA->NewMsgno(msg);
        msg->link.reset();
        break;
      case MODE_CHANGE:
        memcpy(msg, omsg, sizeof(GMsg));
        msg->txt = (char*)throw_strdup(msg->txt);
        msg->lin = NULL;
        msg->line = NULL;
        msg->messageid = NULL;
        msg->inreplyto = NULL;
        msg->references = NULL;
        msg->TextToLines(CFG->dispmargin-1, true, false);
        break;
      case MODE_NEW:
        wfill(MINROW, 0, MAXROW-2, MAXCOL-1, ' ', C_READW);
      case MODE_QUOTE:
      case MODE_REPLY:
      case MODE_REPLYCOMMENT:
        AA->NewMsgno(msg);
        msg->timesread = 1;
    }

    msg->oorig = msg->orig;
    msg->board = AA->board();
    msg->odest = msg->dest;

    if(mode != MODE_COPY) {

      // Set msg date and time
      bool dochgdate = true;
      if(mode == MODE_CHANGE) {
        dochgdate = EDIT->ChangeDate() ? true : false;
        if((EDIT->ChangeDate() == YES) and not msg->attr.fmu())
          dochgdate = false;
      }
      if(dochgdate) {
        time32_t a    = gtime(NULL);
        struct tm *tp = ggmtime(&a);
        tp->tm_isdst  = -1;
        time32_t b    = gmktime(tp);
        a += a - b;
        msg->received = msg->arrived = msg->written = a;
      }

      // Prepare msg header

      if(mode != MODE_CHANGE)
        msg->attr = AA->Attributes();   // Set default attributes

      switch(mode) {
        case MODE_QUOTE:
        case MODE_REPLYCOMMENT:
          if(CurrArea != OrigArea)
            AA->SetXlatimport(AL.AreaIdToPtr(OrigArea)->Xlatimport());
          omsg->TextToLines(-CFG->quotemargin, true, false);
          if(ignore_replyto)
            *omsg->ireplyto = NUL;
          if(omsg->attr.rot())
            Rot13(omsg);
          // Drop through
        case MODE_REPLY:
          if(confirm)
            msg->attr = CFG->attribscfm;
          if(omsg->attr.pvt() & !AA->isecho())
            msg->attr.pvt1();    // Set Pvt attr in response to Pvt in orig
          if(omsg->attr.frq() and (mode == MODE_REPLY) and (not confirm)) {
            msg->attr = CFG->attribsfrq;
            msg->attr.frq1();   // Special case for the file request function
          }

          // Change header data if replying to a forwarded message
          if((mode == MODE_REPLYCOMMENT) and omsg->fwdorig.net) {
            strcpy(omsg->by, omsg->fwdfrom);
            strcpy(omsg->realby, omsg->fwdfrom);
            omsg->oorig = omsg->orig = omsg->fwdorig;
            strcpy(omsg->to, omsg->fwdto);
            strcpy(omsg->realto, omsg->fwdto);
            omsg->odest = omsg->dest = omsg->fwddest;
            strcpy(omsg->re, omsg->fwdsubj);
            *omsg->ireplyto = *omsg->iorig = NUL;
          }

          // Do aka matching
          if(AA->Akamatching()) {
            // ... but only if we did NOT change aka manually
            if(AA->Aka().addr.equals(AA->aka())) {

              Addr aka_addr = AA->Aka().addr;

              if (CFG->akamatchfromto)
              {
                bool useto = true;
                std::vector<gaka>::iterator a;

                for (a = CFG->aka.begin(); useto && (a != CFG->aka.end()); a++)
                {
                  if(omsg->orig.equals(a->addr))
                    useto = false;
                }

                if (useto)
                {
                  for (a = CFG->aka.begin(); a != CFG->aka.end(); a++)
                    if(omsg->dest.equals(a->addr))
                    {
                      akamatchreply = true;
                      break;
                    }
                }
              }

              if (CFG->akamatchfromto && akamatchreply)
                aka_addr = omsg->dest;
              else
                aka_addr = AA->Aka().addr;

              AkaMatch(&aka_addr, &omsg->orig);
              AA->SetAka(aka_addr);
            }

            if (CFG->akamatchmanually) ChangeAka();
          }

          if((mode == MODE_REPLYCOMMENT) and not omsg->fwdorig.net) {
            strcpy(msg->to, omsg->to);
            strcpy(msg->realto, omsg->realto);
            msg->dest = omsg->dest;
            strcpy(msg->idest, omsg->idest);
          }
          else {
            strcpy(msg->to, omsg->By());
            strcpy(msg->realto, omsg->realby);
            msg->dest = omsg->orig;
            strcpy(msg->idest, *omsg->ireplyto ? omsg->ireplyto : *omsg->iorig ? omsg->iorig : omsg->iaddr);
          }
          if(not *msg->iaddr)
            strcpy(msg->iaddr, msg->idest);
          strcpy(msg->re, omsg->re);

          if(AA->Replyre() or AA->isinternet()) {

            ulong number;
            const char* r = get_subject_re_info(msg->re, number);
            if(r) {
              if(AA->Replyre() == REPLYRE_NUMERIC and not AA->isinternet()) {
                ISub subj;
                sprintf(subj, "Re^%lu:%s", ((number+1) > number) ? (number+1) : number, r);
                strcpy(msg->re, subj);
              }
            }
            else if(not msg->attr.frq() and not msg->attr.urq()) {
              memmove(msg->re+4, msg->re, sizeof(ISub)-5);
              memmove(msg->re, "Re: ", 4);
            }
          }
          else {
            char* r;
            ulong number;
            char* ptr = msg->re;
            do {
              r = (char*)get_subject_re_info(ptr, number);
              if(r)
                ptr = strskip_wht(r);
            } while(r);
            strcpy(msg->re, ptr);
          }
          strcpy(msg->ddom, omsg->odom);
          if((mode == MODE_REPLYCOMMENT) and omsg->fwdorig.net)
            strcpy(msg->replys, omsg->fwdmsgid);
          else
            strcpy(msg->replys, omsg->msgids);
          if(AA->isnet()) {
            if(omsg->messageid) {
              throw_free(msg->inreplyto);
              msg->inreplyto = throw_strdup(omsg->messageid);
            }
            strcpy(msg->igate, omsg->igate);
            if(*msg->igate) {
              msg->dest.set(msg->igate);
              char* ptr = strchr(msg->igate, ' ');
              if(ptr) {
                if(not isuucp(msg->to))
                  strcpy(msg->realto, msg->to);
                strcpy(msg->to, strskip_wht(ptr));
              }
            }
          }
          else {
            int reflen = omsg->references ? strlen(omsg->references) : 0;
            int midlen = omsg->messageid ? strlen(omsg->messageid) : 0;
            msg->references = (char*)throw_calloc(1, reflen+1+midlen+1);
            if(omsg->references)
              strcpy(msg->references, omsg->references);
            if(omsg->messageid) {
              if(reflen)
                strcat(msg->references, " ");
              strcat(msg->references, omsg->messageid);
            }
            if(*msg->references == NUL)
              throw_release(msg->references);
          }
          if(CurrArea == OrigArea) {
            if((CFG->replylink == REPLYLINK_DIRECT) or streql(AA->basetype(), "JAM"))
              reply_msgno = omsg->msgno;
            else if(CFG->replylink == REPLYLINK_CHAIN)
              GetLastLink(omsg, reply_msgno);
            msg->link.to_set(reply_msgno);
            if(msg->link.to() == omsg->msgno)
              omsg->link.first_set(msg->msgno);
          }

          ///////////
          /////////// Drop through
          ///////////

        case MODE_FORWARD:
          if(mode == MODE_FORWARD) {
            if(msg->fwdorig.net == 0) {
              strcpy(msg->fwdfrom, msg->By());
              msg->fwdorig = msg->orig;
              strcpy(msg->fwdto, msg->To());
              msg->fwddest = msg->dest;
              strxcpy(msg->fwdsubj, msg->re, sizeof(Subj));
              Area* fwdarea = AL.AreaIdToPtr(OrigArea);
              strcpy(msg->fwdarea, fwdarea->isecho() ? fwdarea->echoid() : "");
              strcpy(msg->fwdmsgid, msg->msgids);
            }
          }

          ///////////
          /////////// Drop through
          ///////////

        case MODE_NEW:
          strcpy(msg->by, AA->Username().name);
          msg->attr.nwm1();
          if(not AA->islocal())
            msg->attr.uns1();
          strcpy(msg->odom, CFG->aka[AkaMatch(&msg->orig, &AA->Aka().addr)].domain);
          break;

        case MODE_CHANGE:
          if(not AA->islocal())
            msg->attr.uns1();
          msg->attr.snt0();
          msg->attr.rcv0();
          break;
      }

      // Get random items

      switch(mode) {
        case MODE_NEW:
        case MODE_FORWARD:
          *msg->replys = NUL;
          throw_release(msg->references);
          throw_release(msg->inreplyto);
          if(AA->isnet()) {
            strcpy(msg->to, "");
            msg->dest.reset_fast();
            msg->odest.reset_fast();
          }
          if(not AA->isnet() or (AA->isemail() and strchr(AA->Whoto(), '@')))
            strcpy(msg->to, AA->Whoto());

          ///////
          /////// Drop through
          ///////

        case MODE_REPLYCOMMENT:
        case MODE_QUOTE:
        case MODE_REPLY:
        case MODE_CHANGE:
          AA->RandomizeData(mode);
          if(mode != MODE_CHANGE) {
            msg->orig = AA->Aka().addr;
            if(AA->isinternet())
              strcpy(msg->iorig, AA->Internetaddress());
            if(not strblank(AA->Username().name))
              strcpy(msg->by, AA->Username().name);
            if(CFG->switches.get(emptytearline))
              *msg->tearline = NUL;
          }
          break;
      }

      int forwstat = NO;
      if(msg->odest.net == 0)
        msg->odest = msg->dest;
      status = NO;
      if(not confirm) {
        strcpy(cmpmsg->by, msg->by);
        strcpy(cmpmsg->to, msg->to);
        strcpy(cmpmsg->re, msg->re);
        cmpmsg->attr = msg->attr;
        if(savedirect) {
          CheckSubject(msg, msg->re);
        }
        else {
          GMenuEditHeader MenuEditHeader;
          while((status = MenuEditHeader.Run(mode, msg)) == YES);
        }
      }

      if(status == NO) {
        if(mode == MODE_CHANGE or mode == MODE_FORWARD) {
          status = MODE_SAVE;
          if(mode == MODE_FORWARD)
            forwstat = YES;
        }
        else {
          if(confirm) {
            throw_release(msg->txt);
            FILE* fp = fsopen(AddPath(CFG->goldpath, CFG->confirmfile), "rt", CFG->sharemode);
            if(fp) {
              LoadText(msg, AddPath(CFG->goldpath, CFG->confirmfile));
              fclose(fp);
            }
            if(msg->txt == NULL)
              msg->txt = throw_strdup("\r\rConfirmation Receipt\r\r");
            TokenXlat(mode, msg->txt, msg, omsg, CurrArea);
          }
          else
            CreateFileAddr(msg);
          msg->TextToLines(CFG->dispmargin-1);
          status = MODE_SAVE;
        }
      }
      MakeMsg2(mode, status, forwstat, topline, msg, omsg, cmpmsg);
    }

    if(status != MODE_SAVE) {
      post_xparea.clear();
      crosspost = NO;
    }

    if(status == MODE_SAVE) {

      if(*msg->iorig and (strlen(msg->iorig) < sizeof(Name))) {
        strcpy(msg->realby, msg->by);
        strcpy(msg->by, msg->iorig);
      }

      if(mode == MODE_COPY)
        AA->SaveMsg(GMSG_NEW, msg);
      else
        MakeMsg3(mode, msg);

      // If message is a reply, update the links on the original
      if(CurrArea == OrigArea and (mode == MODE_QUOTE or mode == MODE_REPLYCOMMENT or mode == MODE_REPLY)) {
        if(AA->Msgn.ToReln(reply_msgno)) {
          if(AA->LoadHdr(reply, reply_msgno, false)) {
            ulong replynext;
            bool ok2save = false;
            if(streql(AA->basetype(), "SQUISH")) {
              if(reply->link.first()) {
                for(int r=0; r<reply->link.list_max()-1; r++) {
                  if(reply->link.list(r) == 0) {
                    reply->link.list_set(r, msg->msgno);
                    ok2save = true;
                    break;
                  }
                }
              }
              else {
                reply->link.first_set(msg->msgno);
                ok2save = true;
              }
            }
            else if(streql(AA->basetype(), "JAM")) {
              if(reply->link.first()) {
                replynext = reply->link.first();
                do {
                  reply_msgno = replynext;
                  if(not AA->LoadHdr(reply, reply_msgno, false))
                    break;
                  replynext = reply->link.next();
                } while(reply->link.next());
                if(reply->link.next() == 0) {
                  reply->link.next_set(msg->msgno);
                  ok2save = true;
                }
              }
              else {
                reply->link.first_set(msg->msgno);
                ok2save = true;
              }
            }
            else {
              reply->link.first_set(msg->msgno);
              ok2save = true;
            }
            if(ok2save) {
              reply->pcboard.reply_written = msg->written;
              AA->SaveHdr(GMSG_UPDATE, reply);
            }
          }
        }
      }

      // Ask if the original msg should be deleted while we're at it
      if(CurrArea == OrigArea and CFG->switches.get(askdelorig) and not confirm) {
        switch(mode) {
          case MODE_REPLY:
          case MODE_QUOTE:
          case MODE_REPLYCOMMENT:
            if(not AA->isecho()) {
              topline = 0;
              omsg->TextToLines(CFG->dispmargin-1);
              if(omsg->attr.rot())
                Rot13(omsg);
              HeaderView->Use(AA, omsg);
              HeaderView->Paint();
              BodyView->Use(AA, omsg, topline);
              BodyView->Paint();
              GMenuDelorig MenuDelorig;
              if(MenuDelorig.Run()) {
                AA->Mark.Del(omsg->msgno);
                AA->PMrk.Del(omsg->msgno);
                AA->DeleteMsg(omsg, DIR_PREV);
              }
            }
            break;
        }
      }
    }

    if(CurrArea != OrigArea) {
      AA->UpdateAreadata();
      AA->Close();
      AL.SetActiveAreaId(OrigArea);
      AA->RandomizeData();
    }
    AA->UpdateAreadata();
    EDIT->HardLines(hardlines);
    for(int n=0; n<fspecs; n++)
      throw_release(fspec[n].fblk);
    fspecs = specfiles = 0;
    throw_release(fspec);

    if(crosspost and post_xparea.size())
      update_statuslinef(LNG->Crossposting, AL[post_xparea.back()]->echoid());
  } while(post_xparea.size());

  _in_editor = NO;

  // Restore original aka
  AA->SetAka(origaka);
  akamatchreply = false;

  ResetMsg(omsg);
  ResetMsg(cmpmsg);
  ResetMsg(reply);
  ResetMsg(msg);

  throw_free(cmpmsg);
  throw_free(reply);
  throw_free(msg);

  LoadLanguage(AA->Loadlanguage());

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

