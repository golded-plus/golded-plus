
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
//  Misc. msg handling.
//  ------------------------------------------------------------------

#include <cstdarg>
#include <golded.h>
#include <gtimall.h>


//  ------------------------------------------------------------------

static bool tokenxchg(char*& dst, char* tok, const char* src, int len = 0, int cnt = 0, ...) {

  uint toklen = strlen(tok);
  char buf[100];
  bool use;

  if(*dst != *tok)
    return false;
  if(not strnieql(dst, tok, toklen)) {
    if(len and *dst and (*(dst+1) == '_') and *(dst+2)) {
      if(not strnieql(dst+2, tok+1, toklen-1))
        return false;
      else
        toklen++;
    }
    else
      return false;
  }
  else
    len = 0;

  va_list a;
  va_start(a, cnt);
  for(int i = 0; i < cnt; i ++) {
    use = va_arg(a, int);
    if(dst[toklen] == '{') {
      char *p = strchr(dst+toklen, '}');
      if(p) {
        uint dstlen = p-dst-toklen-1;
        if(use && dstlen) {
          strxcpy (buf, dst+toklen+1, dstlen+1);
          src = buf;
        }
        toklen = p-dst+1;
      }
    }
  }
  va_end(a);

  uint sl = strlen(src);
  uint srclen = (len == 0) ? sl : len;
  memmove(dst+srclen, dst+toklen, strlen(dst+toklen)+1);
  memset(dst, ' ', srclen);
  memcpy(dst, src, ((len != 0) and (len < sl)) ? len : sl);
  dst += srclen;
  return true;
}


//  ------------------------------------------------------------------

inline bool domain_requested(const char *str, size_t pos) {

  if(str[1] == '_') pos++;
  return strnieql(str+pos, "{domain}", 8);
}

//  ------------------------------------------------------------------

char* TokenXlat(int mode, char* input, GMsg* msg, GMsg* oldmsg, int __origarea) {

  static char revbuf[5] = "";
  if(revbuf[0] == NUL)
    sprintf(revbuf, "%02d%02d", str2mon(__gver_date__), atoi(&__gver_date__[4]));

  char attr[80];
  MakeAttrStr(attr, sizeof(attr), &msg->attr);

  const char *xmailer = get_informative_string();
  const char *osver = ggetosstring();

  time_t t = time(NULL);
  struct tm* written_tm = localtime(&t);
  char cdate[80];
  strftimei(cdate, 80, LNG->DateFmt, written_tm);
  char ctime[80];
  strftimei(ctime, 80, LNG->TimeFmt, written_tm);
  char cdtime[80];
  strftimei(cdtime, 80, LNG->DateTimeFmt, written_tm);
  written_tm = gmtime(&oldmsg->written);
  char odate[80];
  strftimei(odate, 80, LNG->DateFmt, written_tm);
  char otime[80];
  strftimei(otime, 80, LNG->TimeFmt, written_tm);
  char odtime[80];
  strftimei(odtime, 80, LNG->DateTimeFmt, written_tm);

  const char* origareaid = AL.AreaIdToPtr(__origarea)->echoid();
  bool origareaisinet = AL.AreaIdToPtr(__origarea)->isinternet();
  bool currareaisinet = AA->isinternet();
  char* modereptr = oldmsg->re;

  char msgno[16];
  sprintf(msgno, "%u", msg->attr.nwm() ? AA->Msgn.Count() + 1 : AA->Msgn.ToReln(msg->msgno));

  char msgs[16];
  sprintf(msgs, "%u", AA->Msgn.Count() + (msg->attr.nwm() ? 1 : 0));

  if((mode == MODE_QUOTE) or (mode == MODE_REPLYCOMMENT) or (mode == MODE_REPLY)) {
    if(AL.AreaIdToPtr(__origarea)->Areareplydirect() and oldmsg->areakludgeid)
      origareaid = oldmsg->areakludgeid;
  }
  else if(mode != MODE_FORWARD)
    modereptr = msg->re;

  char buf[100];
  char* dst = input;

  while(*dst) {
    while((*dst != '@') and (*dst != LF) and *dst)
      dst++;
    if(*dst == LF)
      *dst++ = CR;
    else if(*dst == '@') {
      if(dst[1] == '@') {
        memmove(dst, dst+1, strlen(dst+1)+1);
        dst++;
      }
      else {
        if(tokenxchg(dst, "@cecho", AA->echoid()))
          continue;
        if(tokenxchg(dst, "@cdesc", AA->desc()))
          continue;
        if(tokenxchg(dst, "@oecho", origareaid))
          continue;
        if(tokenxchg(dst, "@odesc", AL.AreaEchoToPtr(origareaid)->desc()))
          continue;
        if(tokenxchg(dst, "@oname", strbtrim(strtmp(oldmsg->By())), 34, 2,
            (int)msg->by_me(), (int)msg->by_you()))
          continue;
        if(tokenxchg(dst, "@ofname", strlword(oldmsg->By()), 0, 2,
            (int)msg->by_me(), (int)msg->by_you()))
          continue;
        if(tokenxchg(dst, "@olname", strrword(oldmsg->By()), 0, 2,
            (int)msg->by_me(), (int)msg->by_you()))
          continue;
        if(tokenxchg(dst, "@odate", odate))
          continue;
        if(tokenxchg(dst, "@otime", otime))
          continue;
        if(tokenxchg(dst, "@odtime", odtime))
          continue;
        if(tokenxchg(dst, "@otzoffset", (oldmsg->tzutc == -32767) ? "" : (sprintf(buf, " %+05d", oldmsg->tzutc), buf)))
          continue;
        if(tokenxchg(dst, "@ofrom", oldmsg->ifrom))
          continue;
        if(tokenxchg(dst, "@oto", oldmsg->ito))
          continue;
        if(tokenxchg(dst, "@omessageid", oldmsg->messageid ? oldmsg->messageid : ""))
          continue;
	if(tokenxchg(dst, "@omsgid", *msg->replys ? msg->replys : ""))
	  continue;
        if(tokenxchg(dst, "@dname", strbtrim(strtmp(oldmsg->To())), 34, 3,
            (int)msg->to_me(), (int)msg->to_you(), (int)oldmsg->to_all()))
          continue;
        if(tokenxchg(dst, "@dpgp", *msg->iaddr ? msg->iaddr : msg->To()))
          continue;
        if(tokenxchg(dst, "@dfname", strlword(oldmsg->To()), 0, 3,
            (int)msg->to_me(), (int)msg->to_you(), (int)oldmsg->to_all()))
          continue;
        if(tokenxchg(dst, "@dlname", strrword(oldmsg->To()), 0, 3,
            (int)msg->to_me(), (int)msg->to_you(), (int)oldmsg->to_all()))
          continue;
        if(origareaisinet) {
          if(tokenxchg(dst, "@oaddr", oldmsg->iorig, 19, 1, 0))
            continue;
          if(tokenxchg(dst, "@daddr", oldmsg->iaddr, 19, 1, 0))
            continue;
        }
        if(currareaisinet) {
          if(tokenxchg(dst, "@caddr", AA->Internetaddress(), 19, 1, 0))
            continue;
          if(tokenxchg(dst, "@faddr", msg->iorig, 19, 1, 0))
            continue;
          if(tokenxchg(dst, "@taddr", msg->iaddr, 19, 1, 0))
            continue;
        }
        if((not origareaisinet or not currareaisinet) and (strlen(dst) >= 6)) {
          bool dr = domain_requested(dst, 6);
          if(not origareaisinet) {
            if(tokenxchg(dst, "@oaddr", oldmsg->orig.make_string(buf, dr ? oldmsg->odom : NULL), 19, 1, 0))
              continue;
            if(strnieql(dst, "@o3daddr", 8)) {
              ftn_addr boss = oldmsg->orig;
              boss.point = 0;
              tokenxchg(dst, "@o3daddr", boss.make_string(buf, domain_requested(dst, 8) ? oldmsg->odom : NULL), 19, 1, 0);
              continue;
            }
            if(tokenxchg(dst, "@daddr", oldmsg->dest.make_string(buf, dr ? oldmsg->ddom : NULL), 19, 1, 0))
              continue;
            if(strnieql(dst, "@d3daddr", 8)) {
              ftn_addr boss = oldmsg->dest;
              boss.point = 0;
              tokenxchg(dst, "@d3daddr", boss.make_string(buf, domain_requested(dst, 8) ? oldmsg->ddom : NULL), 19, 1, 0);
              continue;
            }
          }
          if(not currareaisinet) {
            const gaka &caka=AA->Aka();
            if(tokenxchg(dst, "@caddr", caka.addr.make_string(buf, dr ? caka.domain : NULL), 19, 1, 0))
              continue;
            if(strnieql(dst, "@c3daddr", 8)) {
              ftn_addr boss = caka.addr;
              boss.point = 0;
              tokenxchg(dst, "@c3daddr", boss.make_string(buf, domain_requested(dst, 8) ? caka.domain : NULL), 19, 1, 0);
              continue;
            }
            if(tokenxchg(dst, "@taddr", msg->dest.make_string(buf, dr ? msg->ddom : NULL), 19, 1, 0))
              continue;
            if(strnieql(dst, "@t3daddr", 8)) {
              ftn_addr boss = msg->dest;
              boss.point = 0;
              tokenxchg(dst, "@t3daddr", boss.make_string(buf, domain_requested(dst, 8) ? msg->ddom : NULL), 19, 1, 0);
              continue;
            }
            if(tokenxchg(dst, "@faddr", msg->orig.make_string(buf, dr ? msg->odom : NULL), 19, 1, 0))
              continue;
            if(strnieql(dst, "@f3daddr", 8)) {
              ftn_addr boss = msg->orig;
              boss.point = 0;
              tokenxchg(dst, "@f3daddr", boss.make_string(buf, domain_requested(dst, 8) ? msg->odom : NULL), 19, 1, 0);
              continue;
            }
          }
        }
        if(tokenxchg(dst, "@tname", strbtrim(strtmp(msg->To())), 34, 3,
            (int)false, (int)false, (int)msg->to_all()))
          continue;
        if(tokenxchg(dst, "@tfname", strlword(msg->To()), 0, 3,
            (int)false, (int)false, (int)msg->to_all()))
          continue;
        if(tokenxchg(dst, "@tlname", strrword(msg->To()), 0, 3,
            (int)false, (int)false, (int)msg->to_all()))
          continue;
        if(tokenxchg(dst, "@cname", AA->Username().name, 34))
          continue;
        if(tokenxchg(dst, "@cfname", strlword(strcpy(buf, AA->Username().name))))
          continue;
        if(tokenxchg(dst, "@clname", strrword(strcpy(buf, AA->Username().name))))
          continue;
        if(tokenxchg(dst, "@cfrom", msg->ifrom))
          continue;
        if(tokenxchg(dst, "@cto", msg->ito))
          continue;
        if(tokenxchg(dst, "@cdate", cdate))
          continue;
        if(tokenxchg(dst, "@ctime", ctime))
          continue;
        if(tokenxchg(dst, "@cdtime", cdtime))
          continue;
        if(tokenxchg(dst, "@ctzoffset", AA->Usetzutc() ? (sprintf(buf, " %+05d", tzoffset()), buf) : ""))
          continue;
        if(tokenxchg(dst, "@fname", strbtrim(strtmp(msg->By())), 34))
          continue;
        if(tokenxchg(dst, "fpgp", *msg->iorig ? msg->iorig : msg->By()))
          continue;
        if(tokenxchg(dst, "@ffname", strlword(msg->By())))
          continue;
        if(tokenxchg(dst, "@flname", strrword(msg->By())))
          continue;
        if(strnieql(dst, "@dpseudo", 8)) {
          if(*(oldmsg->pseudoto) == NUL)
            build_pseudo(oldmsg);
          tokenxchg(dst, "@dpseudo", oldmsg->pseudoto, 0, 3, (int)msg->to_me(), (int)msg->to_you(), (int)oldmsg->to_all());
          continue;
        }
        if(strnieql(dst, "@opseudo", 8)) {
          if(*(oldmsg->pseudofrom) == NUL)
            build_pseudo(oldmsg, false);
          tokenxchg(dst, "@opseudo", oldmsg->pseudofrom, 0, 2, (int)msg->by_me(), (int)msg->by_you());
          continue;
        }
        if(strnieql(dst, "@tpseudo", 8)) {
          if(*(msg->pseudoto) == NUL)
            build_pseudo(msg);
          tokenxchg(dst, "@tpseudo", msg->pseudoto, 0, 3, (int)false, (int)false, (int)msg->to_all());
          continue;
        }
        // Same as above (just for backward compatibility)
        if(strnieql(dst, "@pseudo", 7)) {
          if(*(msg->pseudoto) == NUL)
            build_pseudo(msg);
          tokenxchg(dst, "@pseudo", msg->pseudoto, 0, 3, (int)false, (int)false, (int)msg->to_all());
          continue;
        }
        if(strnieql(dst, "@fpseudo", 8)) {
          if(*(msg->pseudofrom) == NUL)
            build_pseudo(msg, false);
          tokenxchg(dst, "@fpseudo", msg->pseudofrom);
          continue;
        }
        if(tokenxchg(dst, "@msgno", msgno))
          continue;
        if(tokenxchg(dst, "@msgs", msgs))
          continue;
        if(tokenxchg(dst, "@cpseudo", *AA->Nickname() ? AA->Nickname() : strlword(strcpy(buf, AA->Username().name), " @")))
          continue;
        if(tokenxchg(dst, "@version", __gver_ver__))
          continue;
        if(tokenxchg(dst, "@ver", __gver_shortver__))
          continue;
        if(tokenxchg(dst, "@rev", revbuf))
          continue;
        if(strnieql(dst, "@align{", 6)) {
          char *ptr = strchr(dst, '}');
          if(ptr) {
            int size = atoi(dst+7) - (dst-input);
            if(size > 0) {
              char filler = ' ';
              if((ptr[1] == '{') and (ptr[3] == '}')) {
                filler = ptr[2];
                ptr += 3;
              }
              memmove(dst+size, ptr+1, strlen(ptr+1)+1);
              memset(dst, filler, size);
              dst += size;
            }
            else {
              if ((ptr[1] == '{') and (ptr[3] == '}'))
                ptr += 3;
              memmove(dst, ptr+1, strlen(ptr+1)+1);
            }
          }
          else {
            memmove(dst, dst+6, strlen(dst+6)+1);
          }
          continue;
        }
        if(tokenxchg(dst, "@pid", __gver_shortpid__))
          continue;
        if(tokenxchg(dst, "@longpid", __gver_longpid__))
          continue;
        if(tokenxchg(dst, "@widepid", xmailer))
          continue;
        if(tokenxchg(dst, "@osver", osver))
          continue;
        if(tokenxchg(dst, "@osslash", __gver_platform__))
          continue;
        if(tokenxchg(dst, "@subject", modereptr))
          continue;
        if(tokenxchg(dst, "@attr", attr))
          continue;
        if(tokenxchg(dst, "@tagline",
          HandleRandomLine(strxcpy(buf, AA->Tagline(), sizeof(buf)), sizeof(buf))))
          continue;
        if(tokenxchg(dst, "@tearline",
          HandleRandomLine(strxcpy(buf, AA->Tearline(), sizeof(buf)), sizeof(buf))))
          continue;
        if(tokenxchg(dst, "@origin",
          HandleRandomLine(strxcpy(buf, AA->Origin(), sizeof(buf)), sizeof(buf))))
          continue;
        dst++;
      }
    }
  }

  return input;
}


//  ------------------------------------------------------------------
//  ROT13 De/crypting function

void Rot13(GMsg* msg) {

  char c;
  int ptr;
  int n;
  Line* line;
  static char rot13_table[256];
  static char done = NO;
  #define ROT13_DECODE(c)   (((c) & 0x80) ? (c) : rot13_table[c])

  if(not done) {
    for(n=0; n<256; n++) {
      c = (char)n;
      if     (c >= 'a' and c <= 'm') c += '\xD';
      else if(c >= 'n' and c <= 'z') c -= '\xD';
      else if(c >= 'A' and c <= 'M') c += '\xD';
      else if(c >= 'N' and c <= 'Z') c -= '\xD';
      rot13_table[n] = c;
    }
    done = YES;
  }

  line = msg->lin;
  while(line) {
    if(not (line->type & (GLINE_KLUDGE|GLINE_TEAR|GLINE_ORIG))) {
      for(ptr = 0; ptr != line->txt.length(); ptr++) {
        c = line->txt[ptr];
        line->txt[ptr] = ROT13_DECODE(c);
      }
    }
    line = line->next;
  }
}


//  ------------------------------------------------------------------

void LoadText(GMsg* msg, const char* textfile) {

  FILE* fp;
  uint size;
  char* buf;
  char* ptr;
  char* txtptr;
  int hardcr = NO, hardlen;
  char hardline[20];

  fp = fsopen(textfile, "rt", CFG->sharemode);
  if(fp) {

    #define PBUFSIZE 4096   // Allow a 4k long paragraph

    size_t buf_len = PBUFSIZE;
    buf = (char*)throw_malloc(PBUFSIZE);

    int tabsz = CFG->disptabsize ? CFG->disptabsize : 1;
    __extension__ char spaces[tabsz+1];
    memset(spaces, ' ', tabsz);
    spaces[tabsz] = NUL;

    uint tlen = (uint)(fsize(fp)+512);
    msg->txt = txtptr = (char*)throw_realloc(msg->txt, tlen);
    memset(msg->txt, NUL, tlen);

    if(EDIT->HardLines())
      strcpy(hardline, EDIT->HardLine());
    else
      *hardline = NUL;
    strcat(hardline, "\n");
    hardlen = strlen(hardline);
    *txtptr = NUL;

    while(fgets(buf, PBUFSIZE-1, fp)) {

      if(EDIT->HardLines() and strneql(buf, hardline, hardlen)) {
        hardcr = not hardcr;
        if(*txtptr == LF)
          *txtptr = CR;
        if(*txtptr)
          txtptr++;
        continue;
      }
      else {
        size_t read_len = strlen(buf);

        char *ht = buf;
        while((ht = strchr(ht, '\t')) != NULL) {
          int rposn = ht-buf;
          int rstart = rposn%tabsz+1;
          *ht = ' ';
          if(tabsz > rstart) {
            if((read_len + tabsz - rstart) >= (buf_len-1)) {
              buf_len += tabsz;
              buf = (char*)throw_realloc(buf, buf_len);
            }
            strins(spaces+rstart, buf, rposn);
          }
        }

        ptr = buf;
        while(*ptr == ' ')    // Skip leading spaces
          ptr++;
        if(*ptr != LF) {
          strsrep(buf, hardline, "\r");
          read_len = strlen(buf);
          if(hardcr or *buf == CTRL_A) {
            if(not isspace(buf[read_len-1])) read_len += 1;
            buf[read_len-1] = CR;
            buf[read_len] = NUL;
            if(*txtptr == LF)
              *txtptr = CR;
          }
          else if(is_quote(buf)) {
            if(not isspace(buf[read_len-1])) read_len += 1;
            buf[read_len-1] = CR;
            buf[read_len] = NUL;
            if(*txtptr == LF)
              *txtptr = CR;
          }
          else if(not isspace(buf[0]) and (buf[0] == buf[1]) and (buf[0] == buf[2])) {
            if(not isspace(buf[read_len-1])) read_len += 1;
            buf[read_len-1] = CR;
            buf[read_len] = NUL;
            if(*txtptr == LF)
              *txtptr = CR;
          }
          else if(strnieql(buf, " * Origin: ", 11)) {
            if(not isspace(buf[read_len-1])) read_len += 1;
            buf[read_len-1] = CR;
            buf[read_len] = NUL;
            if(*txtptr == LF)
              *txtptr = CR;
          }
          else if(strnieql(ptr, "CC:", 3)) {
            if(not isspace(buf[read_len-1])) read_len += 1;
            buf[read_len-1] = CR;
            buf[read_len] = NUL;
            if(*txtptr == LF)
              *txtptr = CR;
          }
          else if(strnieql(ptr, "XC:", 3)) {
            if(not isspace(buf[read_len-1])) read_len += 1;
            buf[read_len-1] = CR;
            buf[read_len] = NUL;
            if(*txtptr == LF)
              *txtptr = CR;
          }
          else if(strnieql(buf, "SEEN-BY: ", 9)) {
            if(not isspace(buf[read_len-1])) read_len += 1;
            buf[read_len-1] = CR;
            buf[read_len] = NUL;
            if(*txtptr == LF)
              *txtptr = CR;
          }
          if(*txtptr == LF) {
            if((*(txtptr-1) != ' ') and (*ptr != ' '))
              *txtptr++ = ' ';
          }
          else if(*txtptr != 0)
            txtptr++;
        }
        else {
          if(*txtptr == LF)
            *txtptr = CR;
          if(*txtptr)
            txtptr++;
          strcpy(buf, "\r");
        }
      }
      if((*(txtptr-1) == CR) or (*txtptr == NUL)) {
        size = strlen(buf);
        memcpy(txtptr, buf, size);
        txtptr += size-1;
      }
    }
    if(*txtptr != CR)
      *(++txtptr) = CR;
    *(++txtptr) = NUL;
    fclose(fp);
    throw_free(buf);
  }
}


//  ------------------------------------------------------------------

void GMsg::LinesToText() {

  // Allow paragraphs up to 64k-1 long in the transfer buffer
  int bufsize = 65535;
  char* _buf = (char*)throw_malloc(bufsize);

  Chs* _xlat_table = CharTable;
  int _xlat_level = _xlat_table ? (_xlat_table->level ? _xlat_table->level : 2) : 0;

  XlatStr(_buf, realby, _xlat_level, _xlat_table);
  strxcpy(realby, _buf, sizeof(INam));
  XlatStr(_buf, realto, _xlat_level, _xlat_table);
  strxcpy(realto, _buf, sizeof(INam));
  XlatStr(_buf, by, _xlat_level, _xlat_table);
  strxcpy(by, _buf, sizeof(INam));
  XlatStr(_buf, to, _xlat_level, _xlat_table);
  strxcpy(to, _buf, sizeof(INam));
  if(not (attr.frq() or attr.att() or attr.urq())) {
    XlatStr(_buf, re, _xlat_level, _xlat_table);
    strxcpy(re, _buf, sizeof(ISub));
  }

  int _lfterm = EDIT->CrLfTerm() and not AA->ispcboard();
  int _hardterm = AA->Edithardterm() or AA->ispcboard() or AA->iswildcat();
  int _isezycom = AA->isezycom();

  uint _alloc_size = 1024;
  Line* _line = lin;
  uint _lines = 0;

  // Approximate the size of the text
  while(_line) {
    _alloc_size += _line->txt.length();
    _line = _line->next;
    _lines++;
  }
  _alloc_size += _lines * (_lfterm ? 2 : 1);

  // Allocate the text
  txt = (char*)throw_realloc(txt, _alloc_size);

  // Setup variables for the loop
  _line = lin;
  uint _size = 0;
  char* _ptr = txt;

  // Build the text from the lines

  while(_line) {
    *_buf = NUL;
    char* _bptr;
    bool _hterm = true;
    if(_line->isheader()) {
      _bptr = stpcpy(_buf, _line->txt.c_str());
      while(_line->next and _line->iswrapped()) {
        _line = _line->next;
        _bptr = stpcpy(_bptr, _line->txt.c_str());
        if((_bptr-_buf) > (bufsize-256)) {
          int bptrlen = (int)(_bptr - _buf);
          bufsize += 4096;
          _buf = (char*)throw_realloc(_buf, bufsize);
          _bptr = _buf + bptrlen;
        }
      }
    }
    else {
      _bptr = XlatStr(_buf, _line->txt.c_str(), _xlat_level, _xlat_table);
      if(*_buf and not _line->ishard()) {
        if(_line->next) {
          if((*(_bptr-1) != ' ') and (*_line->next->txt.c_str() != ' '))
            if(_isezycom or not _hardterm)
              *_bptr++ = ' ';
          if(not _hardterm) {
            if(_isezycom and not CFG->switches.get(dispsoftcr))
              *_bptr++ = SOFTCR;
            _hterm = false;
          }
        }
      }
    }
    if(_hterm) {
      *_bptr++ = CR;
      if(_lfterm)
        *_bptr++ = LF;
    }
    if(*_buf) {
      uint _buflen = (uint)(_bptr-_buf);
      _size += _buflen;
      if(_size >= _alloc_size) {
        _alloc_size += 1024;
        txt = (char*)throw_realloc(txt, _alloc_size);
      }
      _ptr = txt + _size;
      memcpy(_ptr-_buflen, _buf, _buflen);
    }
    _line = _line->next;
  }
  *_ptr = NUL;

  throw_free(_buf);
}


//  ------------------------------------------------------------------
