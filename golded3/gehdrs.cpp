
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
//  Header display.
//  ------------------------------------------------------------------

#ifdef OLD_STYLE_HEADER

#include <golded.h>


//  ------------------------------------------------------------------

static char* strconv(char* str) {

  char* s=str;
  char* p = str;

  while(*str) {
    if(iscntrl(*str) and (*str != '\n') and (*str != '\r'))  // Control codes
      *p++ = '.';
    else
      *p++ = *str;
    str++;
  }
  return(s);
}


//  ------------------------------------------------------------------

void DispHeader(GMsg* msg, bool prn, FILE* fp, int width) {

  int namewidth = CFG->disphdrnodeset.pos - CFG->disphdrnameset.pos;
  int nodewidth = CFG->disphdrdateset.pos - CFG->disphdrnodeset.pos;
  int datewidth = width - CFG->disphdrdateset.pos;

  char headerline[200];
  memset(headerline, '=', width);
  headerline[width] = NUL;

  INam whofrom;
  if(*msg->ifrom and *msg->realby)
    sprintf(whofrom, "%s <%s>", msg->realby, msg->iorig);
  else if(*msg->ifrom and *msg->iorig)
    strcpy(whofrom, msg->iorig);
  else
    strcpy(whofrom, msg->By());
  strsetsz(whofrom, (AA->isinternet() or *msg->ifrom) ? (namewidth+nodewidth) : namewidth);

  INam whoto;
  if(*msg->ito and *msg->realto)
    sprintf(whoto, "%s <%s>", msg->realto, msg->idest);
  else if(*msg->ito and *msg->idest)
    strcpy(whoto, msg->idest);
  else
    strcpy(whoto, msg->To());
  strsetsz(whoto, (AA->isinternet() or *msg->ito) ? (namewidth+nodewidth) : namewidth);

  // Generate top line fields
  char buf[256];
  char buf1[200];
  char buf2[200];
  strtrim(strcpy(buf, AA->desc()));
  if((CFG->dispareano == ALWAYS) or (CFG->dispareano and AA->board()))
    sprintf(buf1, " [%u] %s ", AA->board(), buf);
  else
    sprintf(buf1, " %s ", buf);
  strtrim(buf1);
  strcat(buf1, " (" /*)*/);
  if(AA->isinternet())
    strcpy(buf, AA->Internetaddress());
  else
    AA->Aka().addr.make_string(buf);
  strcat(buf1, buf);
  strcat(buf1, /*(*/ ") ");

  if(msg->areakludgeid)
    sprintf(buf2, " %s (%s) ", AA->echoid(), msg->areakludgeid);
  else
    sprintf(buf2, " %s ", AA->echoid());

  // Write the total header to a file
  strcpy(stpcpy(buf, headerline), prn ? NL : "\n");
  strncpy(buf+1, buf1, strlen(buf1));
  strncpy(buf+width-strlen(buf2)-1, buf2, strlen(buf2));
  strconv(buf);
  fwrite(buf, strlen(buf), 1, fp);

  // Generate message attributes string
  MakeAttrStr(buf2, sizeof(buf2), &msg->attr);
  int len2 = strlen(buf2);
  if(len2 > width-CFG->disphdrnodeset.pos) {
    len2 = width-CFG->disphdrnodeset.pos;
    strsetsz(buf2, len2);
  }

  // Generate message number and reply links string
  char* ptr = buf1;
  int list_max = msg->link.list_max();
  ulong* replies = (ulong*)throw_calloc(list_max+1, sizeof(ulong));
  ulong replyto, replynext;
  if(CFG->switches.get(disprealmsgno)) {
    ptr += sprintf(ptr, " %-5.5s: #%lu [%u]", LNG->Msg, msg->msgno, AA->Msgn.Count()+(msg->attr.nwm() ? 1 : 0));
    replyto = msg->link.to();
    replies[0] = msg->link.first();
    replynext = msg->link.next();
    for(int replyn=1; replyn<list_max+1; replyn++)
      replies[replyn] = msg->link.list(replyn-1);
  }
  else {
    uint active = AA->Msgn.Count() + (msg->attr.nwm() ? 1 : 0);
    ptr += sprintf(ptr, " %-5.5s: %u %s %u", LNG->Msg, msg->attr.nwm() ? active : AA->Msgn.ToReln(msg->msgno), LNG->of, active);
    replyto = AA->Msgn.ToReln(msg->link.to());
    replies[0] = AA->Msgn.ToReln(msg->link.first());
    replynext = AA->Msgn.ToReln(msg->link.next());
    for(int replyn=1; replyn<list_max+1; replyn++)
      replies[replyn] = AA->Msgn.ToReln(msg->link.list(replyn-1));
  }
  if(replyto)
    ptr += sprintf(ptr, " -%lu", replyto);
  for(int replyn=0,plus=0; replyn<list_max+1; replyn++)
    if(replies[replyn])
      ptr += sprintf(ptr, " %s%lu", (plus++?"":"+"), replies[replyn]);
  if(replynext)
    sprintf(ptr, " *%lu", replynext);
  int len1 = strlen(buf1)-8;
  if((CFG->disphdrnameset.pos + len1) > CFG->disphdrnodeset.pos) {
    if(8 + len1 + len2 > width) {
      strsetsz(buf1, width-len2-1);
      strtrim(buf1);
    }
    strcat(buf1, " ");
    strcat(buf1, buf2);
    *buf2 = NUL;
  }
  else {
    strsetsz(buf1, namewidth+8);
    strcat(buf1, buf2);
    *buf2 = NUL;
  }
  strsetsz(buf1, width);
  throw_free(replies);

  // Write message info
  strcpy(stpcpy(buf, buf1), buf2);
  strtrim(buf);
  strcat(buf, prn ? NL : "\n");
  strconv(buf);
  fwrite(buf, strlen(buf), 1, fp);

  // Generate orig node data
  if(msg->orig.net)
    msg->orig.make_string(buf1);
  else
    *buf1 = NUL;
  strsetsz(buf1, nodewidth);

  if(msg->written)
    strftimei(buf2, CFG->disphdrdateset.len, LNG->DateTimeFmt, gmtime(&msg->written));
  strsetsz(buf2, datewidth);

  // write from line
  sprintf(buf, "%s%s%s%s",
    LNG->From, whofrom,
    ((not (*msg->ifrom and (*msg->realby or *msg->iorig))) and not AA->isinternet()) ? buf1 : "",
    buf2
  );
  strtrim(buf);
  strcat(buf, prn ? NL : "\n");
  strconv(buf);
  fwrite(buf, strlen(buf), 1, fp);

  // Generate dest node data
  if(msg->dest.net and AA->isnet()) {
    msg->dest.make_string(buf2);
    if(msg->odest.net) {
      if(msg->odest.net != msg->dest.net or msg->odest.node != msg->dest.node) {
        sprintf(buf, " %s %u/%u", LNG->Via, msg->odest.net, msg->odest.node);
        strcat(buf2, buf);
      }
    }
  }
  else
    *buf2 = NUL;
  strsetsz(buf2, nodewidth);

  if(msg->arrived)
    strftimei(buf2, CFG->disphdrdateset.len, LNG->DateTimeFmt, gmtime(&msg->arrived));
  strsetsz(buf2, datewidth);

  // write to line
  sprintf(buf, "%s%s%s%s", LNG->To, whoto,
    ((not (*msg->ito and (*msg->realto or *msg->idest))) and not AA->isinternet()) ? buf2 : "",
    buf2
  );
  strtrim(buf);
  strcat(buf, prn ? NL : "\n");
  strconv(buf);
  fwrite(buf, strlen(buf), 1, fp);

  // Generate subjectline
  strcpy(buf2, (msg->attr.att() or msg->attr.frq() or msg->attr.urq()) ? LNG->File : LNG->Subj);
  strxcpy(buf1, msg->re, sizeof(buf1));
  strsetsz(buf1, width-strlen(buf2));

  // write subject line
  strcpy(stpcpy(buf, buf2), buf1);
  strtrim(buf);
  strcat(buf, prn ? NL : "\n");
  strconv(buf);
  fwrite(buf, strlen(buf), 1, fp);

  // write bottom line
  strcpy(stpcpy(buf, headerline), prn ? NL : "\n");
  strconv(buf);
  fwrite(buf, strlen(buf), 1, fp);    
}


//  ------------------------------------------------------------------

#endif
