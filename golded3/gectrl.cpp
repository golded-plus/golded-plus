
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
//  Random system and control info handling.
//  ------------------------------------------------------------------

#include <golded.h>
#include <gutlcode.h>
#include <gutlmisc.h>
#include <gtimall.h>


//  ------------------------------------------------------------------

int _use_fwd = true;


//  ------------------------------------------------------------------

bool isuucp(const char *name) {

  return strieql("UUCP", name) or (*AA->Internetgate().name and strieql(AA->Internetgate().name, name));
}


//  ------------------------------------------------------------------

char* MakeOrigin(GMsg* msg, const char* orig) {

  char buf[256];
  char origin[100];

  strxcpy(origin, orig, sizeof(origin));

  if(msg->orig.net)
    msg->orig.make_string(buf, *AA->Netname() ? (*msg->odom ? msg->odom : AA->Netname()) : NULL);
  else
    AA->Aka().addr.make_string(buf, *AA->Netname() ? (*AA->Aka().domain ? AA->Aka().domain : AA->Netname()) : NULL);

  if(*origin == '@')
    GetRandomLine(origin, sizeof(origin), origin+1);

  sprintf(msg->origin, "%.*s (%s)", (int)(79 - 11 - 2 - strlen(buf) - 1), origin, buf);
  return msg->origin;
}


//  ------------------------------------------------------------------

char* MakeTearline(GMsg* msg, char* buf) {

  if(*msg->tearline == '@')
    GetRandomLine(msg->tearline, sizeof(msg->tearline), msg->tearline+1);

  strcpy(stpcpy(buf, "--- "), strbtrim(msg->tearline));

  return strtrim(buf);
}


//  ------------------------------------------------------------------

void MakeFlags(GMsg* msg, Line** line, char* buf) {

  // The FrontDoor FLAGS netmail kludge
  if(CFG->switches.get(useflags)) {
    sprintf(buf, "\001FLAGS ");
    if(AA->ishudson()) {
      if(msg->attr.hld())
        strcat(buf, "HLD ");
      if(msg->attr.frq())
        strcat(buf, "FRQ ");
    }
    if(msg->attr.imm())
      strcat(buf, "IMM ");
    if(msg->attr.dir())
      strcat(buf, "DIR ");
    if(msg->attr.tfs())
      strcat(buf, "TFS ");
    if(msg->attr.kfs())
      strcat(buf, "KFS ");
    if(msg->attr.lok())
      strcat(buf, "LOK ");
    if(msg->attr.a_s())
      strcat(buf, "A/S ");
    if(msg->attr.zon())
      strcat(buf, "ZON ");
    if(msg->attr.hub())
      strcat(buf, "HUB ");
    if(msg->attr.xma())
      strcat(buf, "XMA ");
    if(msg->attr.cfm())
      strcat(buf, "CFM ");
    if(msg->attr.hir())
      strcat(buf, "HIR ");
    if(msg->attr.cov())
      strcat(buf, "COV ");
    if(msg->attr.sig())
      strcat(buf, "SIG ");
    if(msg->attr.let())
      strcat(buf, "LET ");
    if(msg->attr.fax())
      strcat(buf, "FAX ");
    if(strlen(buf) > 7) {
      buf[strlen(buf)-1] = 0;
      *line = AddKludge(*line, buf);
      (*line)->kludge = GKLUD_FLAGS;
    }
  }
}


//  ------------------------------------------------------------------

char* mime_header_encode(char* dest, const char* source, GMsg* msg) {

  bool inmime = false;
  const char* s = source;
  char* bp = dest;
  const char* lp;
  char* temp_src = (char*)throw_malloc(4096);

  if(*msg->charset) {
    if((msg->charsetlevel&3) and ChsTP) {
      char chln, *d = temp_src;

      for(uint len = 0; *s; s++) {
        char* tptr = (char*)ChsTP[(byte)*s];
        chln = *tptr++;
        while(chln-- and (len < 4096)) {
          *(d++) = *tptr++;
          ++len;
        }
        if(len == 4096)
          break;
      }
      *d = NUL;
      s = temp_src;
    }
  }

  if(CFG->encodeemailheaders) {

    lp = s + strlen(s);

    while((lp>s) and in_range(lp[-1], ' ', '\x7F'))
      lp--;

    for(const char* ptr = s; *ptr and (ptr<lp); ptr++) {
      if(iscntrl(*ptr) or not isascii(*ptr) or (inmime and strchr(" =?", *ptr))) {
        if(not inmime) {
          if(msg->charset) {
            bp = stpcpy(bp, "=?");
            if(strnieql(msg->charset, "latin", 5)) {
              bp = Latin2ISO(bp, msg->charset);
            }
            else {
              char *pbp = bp;
              bp = stpcpy(bp, IsQuotedPrintable(msg->charset) ? ExtractPlainCharset(msg->charset) : strlword(msg->charset));
              strlwr(pbp);
            }
            bp = stpcpy(bp, "?Q?");
          }
          else {
            strcpy(bp, "=?iso-8859-1?Q?");
            bp += 15;
          }
          inmime = true;
        }
        sprintf(bp, "=%02X", (*ptr)&0xff);
        bp += 3;
      }
      else
        *bp++ = *ptr;
    }

    if(inmime) {
      strcpy(bp, "?=");
      bp += 2;
    }
  }
  else
    lp = s;

  strcpy(bp, lp);
  throw_free(temp_src);

  return bp;
}


//  ------------------------------------------------------------------

const char* get_informative_string(void) {

  static char informative_string[356] = "";

  if(informative_string[0] == NUL)
    sprintf(informative_string, "%s%s%s %s%i.%i.%i%s (%s)",
            __gver_prename__, __gver_name__, __gver_postname__,
            __gver_preversion__, __gver_major__, __gver_minor__,
            __gver_release__, __gver_postversion__, ggetosstring());
  return informative_string;
}


//  ------------------------------------------------------------------

void DoKludges(int mode, GMsg* msg, bool attronly) {

  char* buf = (char*)throw_malloc(4096);
  char* buf2 = (char*)throw_malloc(1024);
  Line* line;
  
  // Insert empty line at the top for practical purposes

  line = new Line();
  throw_xnew(line);
  msg->lin = line = InsertLine(line, msg->lin, DIR_PREV);

  // Strip all the kludges we insert ourselves

  while(line) {

    int stripkludges = attronly ? GKLUD_FLAGS : (GKLUD_RFC|GKLUD_FWD|GKLUD_INTL|GKLUD_FMPT|GKLUD_TOPT|GKLUD_FLAGS|GKLUD_AREA|GKLUD_MSGID|GKLUD_REPLY|GKLUD_PID|GKLUD_CHARSET|GKLUD_KNOWN|(mode==MODE_FORWARD?(GKLUD_PATH|GKLUD_SEENBY):0));

    if(line->kludge & stripkludges) {
      bool waswrapped;
      do {
        waswrapped = (line->type & GLINE_WRAP) ? true : false;
        line = DeleteLine(line);
      } while(line and waswrapped);
    }
    else {
      line = line->next;
    }
  }

  line = msg->lin;

  if(attronly) {
    if(AA->isnet())
      MakeFlags(msg, &line, buf);
  }
  else {

    if(not AA->isnet() and msg->areakludgeid) {
      sprintf(buf, "\001AREA:%s", msg->areakludgeid);
      line = AddKludge(line, buf);
      line->kludge = GKLUD_AREA;
    }

    if(AA->isnet()) {

                                                       // 123456789012345678901234567
      if(line->next and strneql(line->next->txt.c_str(), "-----BEGIN PGP ", 15)) {
        line = AddKludge(line, "\001ENC: PGP");
      }

      // The INTL kludge for zone crossing
      if(CFG->useintl and (CFG->useintl == YES or (msg->dest.zone != msg->orig.zone))) {
        sprintf(buf, "\001INTL %u:%u/%u %u:%u/%u",
          msg->dest.zone ? msg->dest.zone : AA->Aka().addr.zone,
          msg->dest.net, msg->dest.node,
          msg->orig.zone ? msg->orig.zone : AA->Aka().addr.zone,
          msg->orig.net, msg->orig.node
        );
        line = AddKludge(line, buf);
        line->kludge = GKLUD_INTL;
      }

      // The FMPT and TOPT kludges for point addressing
      if(msg->dest.point) {
        sprintf(buf, "\001TOPT %u", msg->dest.point);
        line = AddKludge(line, buf);
        line->kludge = GKLUD_TOPT;
      }
      if(msg->orig.point and msg->orig.net == msg->oorig.net and msg->orig.node == msg->oorig.node) {
        sprintf(buf, "\001FMPT %u", msg->orig.point);
        line = AddKludge(line, buf);
        line->kludge = GKLUD_FMPT;
      }

      MakeFlags(msg, &line, buf);
    }

    // The REPLY: kludge for Receiver identification
    if(CFG->switches.get(usemsgid) and *msg->replys) {
      sprintf(buf, "\001REPLY: %s", msg->replys);
      line = AddKludge(line, buf);
      line->kludge = GKLUD_REPLY;
    }

    // The MSGID: kludge for Origination identification
    if(AA->Internetmsgid() and *msg->iorig and AA->isinternet()) {
      // User configured Internetdomain, use it
      if(*CFG->internetdomain) {
        strcpy(buf, CFG->internetdomain);
      }
      else {
        // Fallback method. Yes, its "Crosspoint"-style. Yes, it's not
        // Son-Of-RFC1036-compatible. Yes, I don't care.
        // '%' in domain name activates some braindead spamfilters, so
        // don't use it. I've had to realize that myself (Dirk).
        strcpy(buf, msg->iorig);
        strchg(buf, '@', '.');
      }
      sprintf(buf2, "<GED%08lX@%s>", time(NULL)+(msgcount++), buf);
      throw_release(msg->messageid);
      msg->messageid = throw_strdup(buf2);
      CvtMessageIDtoMSGID(buf2, buf, AA->echoid(), "MSGID");
      strcpy(msg->msgids, buf+8);
    }
    else {
      msg->orig.make_string(buf2, msg->odom);
      sprintf(msg->msgids, "%s %08lx", buf2, time(NULL)+(msgcount++));        
    }
    if(CFG->switches.get(usemsgid) and not AA->ispcboard()) {
      sprintf(buf, "\001MSGID: %s", msg->msgids);
      line = AddKludge(line, buf);
      line->kludge = GKLUD_MSGID;
    }

    // The PID: (Product ID code) kludge
    strxmerge(msg->pid, sizeof(msg->pid), __gver_shortpid__, " ", __gver_ver__, NULL);

    if(CFG->usepid and (CFG->switches.get(emptytearline) or not (striinc(__gver_longpid__, msg->tearline)))) {

      sprintf(buf, "\001PID: %s", msg->pid);
      line = AddKludge(line, strtrim(buf));
      line->kludge = GKLUD_PID;
    }

    // The CHARSET kludge
    if(CFG->usecharset and *msg->charset) {
      *buf = NUL;
      if(strieql(msg->charset, "I51"))
        strcpy(buf, "\001I51");
      else if(not strieql(msg->charset, "COMPOSED"))
        sprintf(buf, "\001%s: %s", CFG->switches.get(kludgechrs) ? "CHRS" : "CHARSET", msg->charset);
      if(*buf) {
        line = AddKludge(line, buf);
        line->kludge = GKLUD_CHARSET;
      }
    }

    // The TZUTC kludge for timezone info
    if(AA->Usetzutc()) {
      int __tzoffset = tzoffset();
      sprintf(buf, "\001TZUTC: %0*d", (__tzoffset < 0) ? 5 : 4, __tzoffset);
      line = AddKludge(line, buf);
      line->kludge = GKLUD_KNOWN;
    }

    // The FWD* kludges
    if(_use_fwd) {
      if(*msg->fwdfrom) {
        sprintf(buf, "\001FWDFROM %s", msg->fwdfrom);
        line = AddKludge(line, buf);
        line->kludge = GKLUD_FWD;
      }
      if(msg->fwdorig.net) {
        sprintf(buf, "\001FWDORIG %s", msg->fwdorig.make_string(buf2));
        line = AddKludge(line, buf);
        line->kludge = GKLUD_FWD;
      }
      if(*msg->fwdto) {
        sprintf(buf, "\001FWDTO %s", msg->fwdto);
        line = AddKludge(line, buf);
        line->kludge = GKLUD_FWD;
      }
      if(msg->fwddest.net) {
        sprintf(buf, "\001FWDDEST %s", msg->fwddest.make_string(buf2));
        line = AddKludge(line, buf);
        line->kludge = GKLUD_FWD;
      }
      if(*msg->fwdsubj) {
        sprintf(buf, "\001FWDSUBJ %s", msg->fwdsubj);
        line = AddKludge(line, buf);
        line->kludge = GKLUD_FWD;
      }
      if(*msg->fwdarea) {
        sprintf(buf, "\001FWDAREA %s", msg->fwdarea);
        line = AddKludge(line, buf);
        line->kludge = GKLUD_FWD;
      }
      if(*msg->fwdmsgid) {
        sprintf(buf, "\001FWDMSGID %s", msg->fwdmsgid);
        line = AddKludge(line, buf);
        line->kludge = GKLUD_FWD;
      }
    }

    if(AA->isinternet()) {

      const char* rfc = AA->Internetrfcbody() ? "" : "\001";

      if(AA->isemail() and (*msg->ito or strchr(msg->to, '@'))) {
        INam _toname;
        IAdr _toaddr;
        char* ptr = *msg->ito ? msg->ito : msg->to;
        strxcpy(buf2, ptr, 1024);
        ParseInternetAddr(buf2, _toname, _toaddr);
        if(_toname[0] != NUL) {
          mime_header_encode(buf2, _toname, msg);
          char quot[2] = "\"";
          if((buf2[0] == '\"') or (strpbrk(buf2, " \t") == NULL))
            quot[0] = NUL;
          sprintf(buf, "%s%sTo: %s%s%s <%s>", rfc, AA->isnewsgroup() ? "X-" : "", quot, buf2, quot, _toaddr);
        }
        else
          sprintf(buf, "%s%sTo: %s", rfc, AA->isnewsgroup() ? "X-" : "", ptr);
        line = AddKludge(line, buf);
        line->kludge = GKLUD_RFC;
      }

      if(CFG->internetviagate) {
        mime_header_encode(buf2, msg->by, msg);
        strxcpy(msg->by, buf2, sizeof(INam));
        mime_header_encode(buf2, msg->to, msg);
        strxcpy(msg->to, buf2, sizeof(INam));
        if(not (msg->attr.frq() or msg->attr.att() or msg->attr.urq())) {
          mime_header_encode(buf2, msg->re, msg);
          strxcpy(msg->re, buf2, sizeof(ISub));
        }
      }
      else {
        if(*msg->ifrom) {
          INam _fromname;
          IAdr _fromaddr;

          strcpy(buf2, msg->ifrom);
          ParseInternetAddr(buf2, _fromname, _fromaddr);
          if(_fromname[0] != NUL) {
            mime_header_encode(buf2, _fromname, msg);
            char quot[2] = "\"";
            if((buf2[0] == '\"') or (strpbrk(buf2, " \t") == NULL))
              quot[0] = NUL;
            sprintf(buf, "%sFrom: %s%s%s <%s>", rfc, quot, buf2, quot, _fromaddr);
          }
          else
            sprintf(buf, "%sFrom: %s", rfc, msg->ifrom);
          line = AddKludge(line, buf);
          line->kludge = GKLUD_RFC;
        }
        else if(*msg->iorig) {
          mime_header_encode(buf2, msg->By(), msg);
          if(*buf2) {
            char quot[2] = "\"";
            if((buf2[0] == '\"') or (strpbrk(buf2, " \t") == NULL))
              quot[0] = NUL;
            sprintf(buf, "%sFrom: %s%s%s <%s>", rfc, quot, buf2, quot, msg->iorig);
          } else
            sprintf(buf, "%sFrom: %s", rfc, msg->iorig);
          line = AddKludge(line, buf);
          line->kludge = GKLUD_RFC;
        }

        if(AA->isnewsgroup()) {
          sprintf(buf, "%sNewsgroups: %s", rfc, strlwr(strcpy(buf2, AA->echoid())));
          line = AddKludge(line, buf);
          line->kludge = GKLUD_RFC;
        }

        if(*msg->re) {
          mime_header_encode(buf2, msg->re, msg);
          sprintf(buf, "%sSubject: %s", rfc, buf2);
          line = AddKludge(line, buf);
          line->kludge = GKLUD_RFC;
        }

        struct tm* tm = gmtime(&msg->written);
        sprintf(buf, "%sDate: %s, %02d %s %04d %02d:%02d:%02d", rfc,
          __gsweekday[tm->tm_wday],
          tm->tm_mday, __gsmonth[tm->tm_mon], 1900+tm->tm_year,
          tm->tm_hour, tm->tm_min, tm->tm_sec
        );
        if(AA->Usetzutc())
          sprintf(buf + strlen(buf), " %+05d", tzoffset());

        line = AddKludge(line, buf);
        line->kludge = GKLUD_RFC;

        if(msg->messageid) {
          sprintf(buf, "%sMessage-ID: %s", rfc, msg->messageid);
          line = AddKludge(line, buf);
          line->kludge = GKLUD_RFC;
        }

        if(msg->references) {
          sprintf(buf, "%sReferences: %s", rfc, msg->references);
          line = AddKludge(line, buf);
          line->kludge = GKLUD_RFC;
        }

        if(msg->inreplyto) {
          sprintf(buf, "%sIn-Reply-To: %s", rfc, msg->inreplyto);
          line = AddKludge(line, buf);
          line->kludge = GKLUD_RFC;
        }

        if(*msg->organization) {
          sprintf(buf, "%sOrganization: %s", rfc, msg->organization);
          line = AddKludge(line, buf);
          line->kludge = GKLUD_RFC;
        }

        sprintf(buf, "%sMIME-Version: 1.0", rfc);
        line = AddKludge(line, buf);
        line->kludge = GKLUD_RFC;
        char encoding[100];
        bool isusascii = striinc("ASCII", msg->charset);
        bool isqp = not isusascii and IsQuotedPrintable(msg->charset);
        if(strnieql(msg->charset, "latin", 5))
          Latin2ISO(encoding, msg->charset);
        else if(isusascii)
          strcpy(encoding, "us-ascii");
        else if(isqp)
          strcpy(encoding, ExtractPlainCharset(msg->charset));
        else
          strcpy(encoding, msg->charset);
        strlwr(encoding);
        sprintf(buf, "%sContent-Type: text/plain; charset=%s", rfc, strlword(encoding));
        line = AddKludge(line, buf);
        line->kludge = GKLUD_RFC;
        sprintf(buf, "%sContent-Transfer-Encoding: %s", rfc, isqp ? "quoted-printable" : isusascii ? "7bit" : "8bit");
        line = AddKludge(line, buf);
        line->kludge = GKLUD_RFC;

        if(*msg->iorig) {
          mime_header_encode(buf2, msg->By(), msg);
          char quot[2] = "\"";
          if((buf2[0] == '\"') or (strpbrk(buf2, " \t") == NULL))
            quot[0] = NUL;
          sprintf(buf, "%sSender: %s%s%s <%s>", rfc, quot, buf2, quot, msg->iorig);
          line = AddKludge(line, buf);
          line->kludge = GKLUD_RFC;
        }

        if((*msg->ireplyto or *CFG->soupreplyto) and (not streql(*msg->ireplyto ? msg->ireplyto : CFG->soupreplyto, msg->iorig))) {
          sprintf(buf, "%sReply-To: %s", rfc, *msg->ireplyto ? msg->ireplyto : CFG->soupreplyto);
          line = AddKludge(line, buf);
          line->kludge = GKLUD_RFC;
        }

        if(striinc("MNEMONIC", msg->charset)) {
          sprintf(buf, "%sX-Charset: ISO_8859-1", rfc);
          line = AddKludge(line, buf);
          line->kludge = GKLUD_RFC;
          sprintf(buf, "%sX-Char-Esc: 29", rfc);
          line = AddKludge(line, buf);
          line->kludge = GKLUD_RFC;
        }

        if(AA->isnewsgroup() or AA->isemail()) {
          sprintf(buf, "%sX-%s: %s", rfc, AA->isnewsgroup() ? "Newsreader" : "Mailreader", get_informative_string());
          line = AddKludge(line, buf);
          line->kludge = GKLUD_RFC;
        }
      }

      if(AA->Internetrfcbody() and line->next and not strblank(line->next->txt.c_str())) {
        line = AddKludge(line, "");
        line->kludge = GKLUD_RFC;
      }
    }
    else if(((mode != MODE_CHANGE) or AA->Internetrfcbody()) and AA->isnet()) {
      if(*msg->ito) {
        sprintf(buf, "To: %s", msg->ito);
        line = AddKludge(line, buf);
        line->kludge = GKLUD_RFC;

        if(AA->Internetrfcbody() and line->next and not strblank(line->next->txt.c_str())) {
          line = AddKludge(line, "");
          line->kludge = GKLUD_RFC;
        }
      }
    }
  }

  // Reset line pointer
  msg->lin = DeleteLine(msg->lin);

  MsgLineReIndex(msg, YES, YES, YES);

  throw_free(buf);
  throw_free(buf2);
}


//  ------------------------------------------------------------------

void DoTearorig(int mode, GMsg* msg) {

  uint  ctrlinfo;
  char  buf[256];
  char* ptr;
  Line* line = msg->lin;
  Line* newline;
  std::string origin;

  origin = AA->Origin();

  if(AA->Taglinesupport()) {
    if(*msg->tagline == '@')
      GetRandomLine(msg->tagline, sizeof(msg->tagline), msg->tagline+1);
    TokenXlat(mode, msg->tagline, msg, msg, CurrArea);
    strbtrim(msg->tagline);
  }

  TokenXlat(mode, msg->tearline, msg, msg, CurrArea);

  ctrlinfo = AA->Ctrlinfo();

  if(not *msg->origin)
    MakeOrigin(msg, origin.c_str());

  // Delete current tag-, tear- and originlines
  int deltypes = GLINE_TEAR|GLINE_ORIG;
  if(AA->Taglinesupport())
    deltypes |= GLINE_TAGL;
  while(line) {
    if(line->type & deltypes) {
      if(line == msg->lin)
        msg->lin = NULL;
      newline = line;
      Line* nextline = newline->next;
      line = DeleteLine(line);
      if(nextline == NULL)
        line = NULL;
    }
    else
      line = line->next;
  }
  
  // Make sure there is a blank line at the bottom, just before the tearline
  // Unless the line is a tagline (..., ___ etc).
  line = LastLine(msg->lin);
  if(line == NULL)
    msg->lin = line = AddLine(NULL, "");

  // Check and fix originline
  if(*msg->origin) {
    ptr = strrchr(msg->origin, '(' /*)*/ );
    if(ptr) {

      if(ptr != msg->origin)
        *((ptr++)-1) = NUL;
      else
        *ptr++ = NUL;

      origin = msg->origin;
    }
    else
      origin = msg->origin;
  }
  MakeOrigin(msg, origin.c_str());

  TokenXlat(mode, msg->tagline, msg, msg, CurrArea);
  TokenXlat(mode, msg->tearline, msg, msg, CurrArea);
  TokenXlat(mode, msg->origin, msg, msg, CurrArea);

  // Add the tagline, tearline and origin as defined
  if(AA->Taglinesupport() and *msg->tagline) {
    sprintf(buf, "%c%c%c %s", AA->Taglinechar(), AA->Taglinechar(), AA->Taglinechar(), msg->tagline);
    strtrim(buf);
    line = AddLine(line, buf);
    line->type |= GLINE_TAGL;
  }
  if(ctrlinfo & CI_TEAR) {
    line = AddLine(line, MakeTearline(msg, buf));
    line->type |= GLINE_TEAR;
  }
  if(ctrlinfo & CI_ORIG) {
    sprintf(buf, " * Origin: %s", msg->origin);
    line = AddLine(line, buf);
    line->type |= GLINE_ORIG;
  }
}


//  ------------------------------------------------------------------
