
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

#include <golded.h>


//  ------------------------------------------------------------------

char* strconv(char* str, char* conv) {

  char* s=str;
  char* p = str;

  NW(conv);   // Dummy

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
  char top1[200];
  strtrim(strcpy(buf, AA->desc()));
  if((CFG->dispareano == ALWAYS) or (CFG->dispareano and AA->board()))
    sprintf(top1, " [%u] %s ", AA->board(), buf);
  else
    sprintf(top1, " %s ", buf);
  strtrim(top1);
  strcat(top1, " (" /*)*/);
  if(AA->isinternet())
    strcpy(buf, AA->Internetaddress());
  else
    AA->Aka().addr.make_string(buf);
  strcat(top1, buf);
  strcat(top1, /*(*/ ") ");

  char top2[200];
  if(msg->areakludgeid)
    sprintf(top2, " %s (%s) ", AA->echoid(), msg->areakludgeid);
  else
    sprintf(top2, " %s ", AA->echoid());

  // Generate message attributes string
  char bot2[200];
  MakeAttrStr(bot2, &msg->attr);
  int len2 = strlen(bot2);
  if(len2 > width-CFG->disphdrnodeset.pos) {
    len2 = width-CFG->disphdrnodeset.pos;
    strsetsz(bot2, len2);
  }

  // Generate message number and reply links string
  char bot1[200];
  char* ptr = bot1;
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
  int len1 = strlen(bot1)-8;
  if((CFG->disphdrnameset.pos + len1) > CFG->disphdrnodeset.pos) {
    if(8 + len1 + len2 > width) {
      strsetsz(bot1, width-len2-1);
      strtrim(bot1);
    }
    strcat(bot1, " ");
    strcat(bot1, bot2);
    *bot2 = NUL;
  }
  else {
    strsetsz(bot1, namewidth+8);
    strcat(bot1, bot2);
    *bot2 = NUL;
  }
  strsetsz(bot1, width);
  throw_free(replies);

  // Generate orig node data
  char node1[200];
  if(msg->orig.net)
    msg->orig.make_string(node1);
  else
    *node1 = NUL;
  strsetsz(node1, nodewidth);

  char date1[25] = "";
  if(msg->written)
    strftimei(date1, CFG->disphdrdateset.len, LNG->DateTimeFmt, gmtime(&msg->written));
  strsetsz(date1, datewidth);

  // Generate dest node data
  char node2[200];
  if(msg->dest.net and AA->isnet()) {
    msg->dest.make_string(node2);
    if(msg->odest.net) {
      if(msg->odest.net != msg->dest.net or msg->odest.node != msg->dest.node) {
        sprintf(buf, " %s %u/%u", LNG->Via, msg->odest.net, msg->odest.node);
        strcat(node2, buf);
      }
    }
  }
  else
    *node2 = NUL;
  strsetsz(node2, nodewidth);

  char date2[25] = "";
  if(msg->arrived)
    strftimei(date2, CFG->disphdrdateset.len, LNG->DateTimeFmt, gmtime(&msg->arrived));
  strsetsz(date2, datewidth);

  // Generate subjectline
  char subj[200], lngsubj[10];
  strcpy(lngsubj, (msg->attr.att() or msg->attr.frq() or msg->attr.urq()) ? LNG->File : LNG->Subj);
  strxcpy(subj, msg->re, sizeof(subj));
  strsetsz(subj, width-strlen(lngsubj));

  // Write the total header to a file
  strcpy(stpcpy(buf, headerline), prn ? NL : "\n");
  strncpy(buf+1, top1, strlen(top1));
  strncpy(buf+width-strlen(top2)-1, top2, strlen(top2));
  strconv(buf, tconv);
  fwrite(buf, strlen(buf), 1, fp);

  strcpy(stpcpy(buf, bot1), bot2);
  strtrim(buf);
  strcat(buf, prn ? NL : "\n");
  strconv(buf, tconv);
  fwrite(buf, strlen(buf), 1, fp);

  sprintf(buf, "%s%s%s%s",
    LNG->From, whofrom,
    ((not (*msg->ifrom and (*msg->realby or *msg->iorig))) and not AA->isinternet()) ? node1 : "",
    date1
  );
  strtrim(buf);
  strcat(buf, prn ? NL : "\n");
  strconv(buf, tconv);
  fwrite(buf, strlen(buf), 1, fp);

  sprintf(buf, "%s%s%s%s", LNG->To, whoto,
    ((not (*msg->ito and (*msg->realto or *msg->idest))) and not AA->isinternet()) ? node2 : "",
    date2
  );
  strtrim(buf);
  strcat(buf, prn ? NL : "\n");
  strconv(buf, tconv);
  fwrite(buf, strlen(buf), 1, fp);

  strcpy(stpcpy(buf, lngsubj), subj);
  strtrim(buf);
  strcat(buf, prn ? NL : "\n");
  strconv(buf, tconv);
  fwrite(buf, strlen(buf), 1, fp);
	    
  strcpy(stpcpy(buf, headerline), prn ? NL : "\n");
  strconv(buf, tconv);
  fwrite(buf, strlen(buf), 1, fp);	    
}


//  ------------------------------------------------------------------

