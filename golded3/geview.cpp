
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
//  Message viewer class implementation.
//  ------------------------------------------------------------------

#include <golded.h>
#include <gutlos.h>


//  ------------------------------------------------------------------

GMsgHeaderView::GMsgHeaderView() {

  alive = false;
  at_row = at_column = width = height = 0;
  border_type = gwindow::bordertype_none;
  window_color = from_color = to_color = subject_color = BLUE|_LGREY;
  title_color = highlight_color = RED|_LGREY;
  border_color = YELLOW|_LGREY;
}


//  ------------------------------------------------------------------

GMsgHeaderView::~GMsgHeaderView() {

  Destroy();
}


//  ------------------------------------------------------------------

void GMsgHeaderView::Create() {

  alive = true;

  window.openxy(
    at_row,
    at_column,
    height,
    width,
    border_type,
    border_color,
    window_color
  );
}


//  ------------------------------------------------------------------

void GMsgHeaderView::Destroy() {

  if(alive)
    window.close();
  else
    alive = false;
}


//  ------------------------------------------------------------------

void GMsgHeaderView::Use(Area *areaptr, GMsg *msgptr) {

  area = areaptr;
  msg = msgptr;
}

 
//  ------------------------------------------------------------------

void GMsgHeaderView::Paint() {

  int namewidth = CFG->disphdrnodeset.pos - CFG->disphdrnameset.pos;
  int nodewidth = CFG->disphdrdateset.pos - CFG->disphdrnodeset.pos;
  int datewidth = width - CFG->disphdrdateset.pos;

  vchar headerline[200];
  for(int c = 0; c < width; c++)
    headerline[c] = _box_table(W_BHEAD, 1);
  headerline[width] = NUL;

  INam whofrom;
  if(not area->isecho() and *msg->ifrom and *msg->realby)
    sprintf(whofrom, "%s <%s>", msg->realby, msg->iorig);
  else if(not area->isecho() and *msg->ifrom and *msg->iorig)
    strcpy(whofrom, msg->iorig);
  else
    strcpy(whofrom, msg->By());
  strsetsz(whofrom, (area->isinternet() or *msg->ifrom) ? (namewidth+nodewidth) : namewidth);

  INam whoto;
  if(not area->isecho() and *msg->ito and *msg->realto)
    sprintf(whoto, "%s <%s>", msg->realto, msg->idest);
  else if(not area->isecho() and *msg->ito and *msg->idest)
    strcpy(whoto, msg->idest);
  else
    strcpy(whoto, msg->To());
  strsetsz(whoto, (area->isinternet() or *msg->ito) ? (namewidth+nodewidth) : namewidth);

  // Generate top line fields
  ISub buf;
  char top1[200];
  strtrim(strcpy(buf, area->desc()));
  if((CFG->dispareano == ALWAYS) or (CFG->dispareano and area->board()))
    sprintf(top1, " [%u] %s ", area->board(), buf);
  else
    sprintf(top1, " %s ", buf);
  strtrim(top1);
  strcat(top1, " (" /*)*/);
  if(area->isinternet())
    strcpy(buf, area->Internetaddress());
  else
    area->Aka().addr.make_string(buf);
  strcat(top1, buf);
  strcat(top1, /*(*/ ") ");

  char top2[200];
  if(msg->areakludgeid)
    sprintf(top2, " %s (%s) ", area->echoid(), msg->areakludgeid);
  else
    sprintf(top2, " %s ", area->echoid());

  #if defined(GUTLOS_FUNCS)
  g_set_ostitle_name(struplow(strtmp(area->echoid())), 0);
  #endif

  // Get marks
  int bookmark = (area->bookmark == msg->msgno);
  int markmark = (area->Mark.Count() ? (area->Mark.Find(msg->msgno) ? 1 : 0) : NO);

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
    ptr += sprintf(ptr, " %-5.5s: #%lu [%u]", LNG->Msg, msg->msgno, area->Msgn.Count()+(msg->attr.nwm() ? 1 : 0));
    replyto = msg->link.to();
    replies[0] = msg->link.first();
    replynext = msg->link.next();
    for(int replyn=1; replyn<list_max+1; replyn++)
      replies[replyn] = msg->link.list(replyn-1);
  }
  else {
    uint active = area->Msgn.Count() + (msg->attr.nwm() ? 1 : 0);
    ptr += sprintf(ptr, " %-5.5s: %u %s %u", LNG->Msg, msg->attr.nwm() ? active : area->Msgn.ToReln(msg->msgno), LNG->of, active);
    replyto = area->Msgn.ToReln(msg->link.to());
    replies[0] = area->Msgn.ToReln(msg->link.first());
    replynext = area->Msgn.ToReln(msg->link.next());
    for(int replyn=1; replyn<list_max+1; replyn++)
      replies[replyn] = area->Msgn.ToReln(msg->link.list(replyn-1));
  }
  if(replyto)
    ptr += sprintf(ptr, " -%lu", replyto);
  for(int replyn=0,plus=0; replyn<list_max+1; replyn++)
    if(replies[replyn])
      ptr += sprintf(ptr, " %s%lu", (plus++?"":"+"), replies[replyn]);
  if(replynext)
    sprintf(ptr, " *%lu", replynext);
  int len1 = strlen(bot1) - 8;
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
  if(msg->dest.net and area->isnet()) {
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

  // Paint the total header in the window
  vchar borderchar[2] = { (vchar)' ', 0 };
  *borderchar = _box_table(W_BHEAD, 1);
  window.printvs(0, 0, border_color|ACSET, borderchar);
  window.prints(0, 1, title_color, top1);
  window.printvs(0, strlen(top1)+1, border_color|ACSET, headerline+strlen(top1)+strlen(top2)-2);
  window.prints(0, width-strlen(top2)-1, title_color, top2);
  window.printvs(0, width-1, border_color|ACSET, borderchar);
  window.printvs(5, 0, border_color|ACSET, headerline);
  window.prints(1, 0, window_color, bot1);
  if(bookmark)
    window.prints(1, 5, highlight_color, "\x11");
  if(markmark)
    window.prints(1, 7, highlight_color, "\x10");
  if(*bot2)
    window.prints(1, CFG->disphdrnodeset.pos, window_color, bot2);
  window.prints(2, 0, window_color, LNG->From);
  window.prints(2, CFG->disphdrnameset.pos, ((msg->foundwhere&GFIND_FROM) or msg->attr.fmu() or (msg->attr.loc() and CFG->switches.get(displocalhigh))) ? highlight_color : from_color, whofrom);
  window.prints(3, 0, window_color, LNG->To);
  window.prints(3, CFG->disphdrnameset.pos, ((msg->foundwhere&GFIND_TO) or msg->attr.tou()) ? highlight_color : to_color, whoto);
  if(not area->isinternet()) {
    if(area->isecho() or not (*msg->ifrom and (*msg->realby or *msg->iorig)))
      window.prints(2, CFG->disphdrnodeset.pos, from_color, node1);
    if(area->isecho() or not (*msg->ito and (*msg->realto or *msg->idest)))
      window.prints(3, CFG->disphdrnodeset.pos, to_color, node2);
  }
  window.prints(2, CFG->disphdrdateset.pos, from_color, date1);
  window.prints(3, CFG->disphdrdateset.pos, to_color, date2);
  window.prints(4, 0, window_color, lngsubj);
  window.prints(4, strlen(lngsubj), (msg->foundwhere&GFIND_SUBJECT) ? highlight_color : subject_color, subj);

  // Calculate attach file sizes
  if(msg->attr.att()) {
    if(CFG->dispattachsize) {
      char buf2[GMAXPATH];
      int begpos = strlen(LNG->File);
      strcpy(buf, msg->re);
      char* ptr = strtok(buf, " ");
      while(ptr) {
        if(*ptr == '^')
          ptr++;
        if((isalpha(*ptr) and (ptr[1] == ':')) or (ptr[0] == '\\') or (ptr[0] == '/'))
          strcpy(buf2, ptr);
        else
          sprintf(buf2, "%s%s", CFG->inboundpath, ptr);
        long sz = GetFilesize(MapPath(buf2));
        if(sz == -1)
          sprintf(subj, " %s ", LNG->n_a);
        else {
          switch(CFG->dispattachsize) {
            case ATTACH_BYTES:
              sprintf(subj, " %li ", sz);
              break;
            case ATTACH_KBYTES:
              sprintf(subj, " %lik ", (sz+512L)/1024L);
              break;
          }
        }
        window.prints(5, begpos+int(ptr-buf)-1, title_color, subj);
        ptr = strtok(NULL, " ");
      }
    }
  }

  if(CFG->dispmsgsize and msg->txt and not _in_editor) {
    uint len = strlen(msg->txt);
    switch(CFG->dispmsgsize) {
      case DISPMSGSIZE_BYTES:
        sprintf(subj, "%u", len);
        break;
      case DISPMSGSIZE_KBYTES:
        sprintf(subj, "%uk", (len+512)/1024);
        break;
      case DISPMSGSIZE_KBYTES+1:
        sprintf(subj, "%u", msg->lines);
        break;
      default:
        *subj = NUL;
    }
    if(*subj)
      window.prints(5, 1, title_color, subj);
  }
}


//  ------------------------------------------------------------------

GMsgBodyView::GMsgBodyView() {

  alive = false;
  scrollbar_visible = false;
  at_row = at_column = width = height = visible_width = 0;
  border_type = gwindow::bordertype_none;
  window_color = BLACK|_LGREY;
  highlight_color = WHITE|_RED;
  scrollbar_color = DGREY|_LGREY;
  border_color = YELLOW|_LGREY;
}


//  ------------------------------------------------------------------

GMsgBodyView::~GMsgBodyView() {

  Destroy();
}


//  ------------------------------------------------------------------

void GMsgBodyView::Create() {

  alive = true;

  window.openxy(
    at_row,
    at_column,
    height,
    width,
    border_type,
    border_color,
    window_color,
    scrollbar_color
  );

  dummy_line.txt = "";
  dummy_line.type = 0;
  dummy_line.color = window_color;

  visible_width = width - (CFG->switches.get(disppagebar) ? 1 : 0);
}


//  ------------------------------------------------------------------

void GMsgBodyView::Destroy() {

  if(alive)
    window.close();
  else
    alive = false;
}


//  ------------------------------------------------------------------

void GMsgBodyView::Use(Area *areaptr, GMsg *msgptr, int startline) {

  area = areaptr;
  msg = msgptr;

  // Find the first highlighted line and go to it
  if(msg->foundwhere & GFIND_BODY) {
    for(startline=0; startline<msg->lines; startline++) {
      if(msg->line[startline]->type & GLINE_HIGH)
        break;
    }
    // Adjust position in window
    startline -= height / 3;
    if(startline < 0)
      startline = 0;
    else if((startline + height - 1) >= msg->lines)
      startline = msg->lines - height;
  }

  if(startline >= msg->lines)
    startline = msg->lines - 1;

  if(startline < 0)
    startline = 0;

  upperline = startline;
  lowerline = upperline + height - 1;
  can_pagedown = lowerline < (msg->lines-1);

  scrollbar_visible = CFG->switches.get(disppagebar) ? (msg->lines > height) : false;
  visible_width = scrollbar_visible ? width-1 : width;
  window.set_scrollbar_color(scrollbar_visible ? scrollbar_color : -1);
}

 
//  ------------------------------------------------------------------

void GMsgBodyView::PaintLine(int row, Line *line) {

  // Calculate effective coordinates for vputs
  int vrow = gwin.active->srow + row;
  uint llen = line->txt.length();

  int color = (line->type & GLINE_HIGH) ? highlight_color : line->color;

  // Trim line if it longer than should be. This actually happens in very rare
  // cases, but always when hex dump displayed.
  if(llen > visible_width) {
    llen = visible_width;
    line->txt.erase(llen);
  }

  // Print it
  if(not SearchHighlight(line, vrow, visible_width, highlight_color)) {
    if(line->type & GLINE_ORIG and strneql(line->txt.c_str(), " * Origin: ", 11)) {
      vputs(vrow, 0, color, " * Origin: ");
      StyleCodeHighlight(line->txt.c_str()+11, vrow, 11, not AA->attr().hex() and CFG->hidestylies, color);
    }
    else
      StyleCodeHighlight(line->txt.c_str(), vrow, 0, not AA->attr().hex() and CFG->hidestylies, color);
      int tlen = strlen(line->txt.c_str());
      vputns(vrow, tlen, color, "", visible_width-tlen);
  }
  else
    vputns(vrow, 0, color, line->txt.c_str(), visible_width);
}


//  ------------------------------------------------------------------

void GMsgBodyView::Paint() {

  window.activate_quick();
  
  Line* dummy_index = NULL;
  Line** line_index = msg->line ? (msg->line + upperline) : &dummy_index;

  for(int row=0; row<height; row++)
    PaintLine(row, *line_index ? *line_index++ : &dummy_line);

  UpdateScrollbar();
}


//  ------------------------------------------------------------------

int GMsgBodyView::Top(int redraw) {

  if(redraw or (msg->lines > height)) {
    can_pagedown = true;
    upperline = 0;
    lowerline = height - 1;
    Paint();
    return true;
  }
  
  return false;
}


//  ------------------------------------------------------------------

int GMsgBodyView::Bottom() {
  
  if(msg->lines > height) {
    if(msg->lines <= height) {
      can_pagedown = true;
      upperline = 0;
    }
    else {
      can_pagedown = false;
      upperline = msg->lines - height;
    }
    lowerline = upperline + height - 1;
    Paint();
    return true;
  }
  
  return false;
}


//  ------------------------------------------------------------------

int GMsgBodyView::PageUp() {

  if(msg->lines > height) {
    can_pagedown = true;
    if(upperline) {
      upperline -= height - 1;
      if(upperline < 0)
        upperline = 0;
      lowerline = upperline + height - 1;
      Paint();
      return true;
    }
  }
  
  return false;
}


//  ------------------------------------------------------------------

int GMsgBodyView::PageDown() {

  if(msg->lines > height) {
    if(can_pagedown) {
      if(lowerline < msg->lines-1) {
        lowerline += height - 1;
        if(lowerline >= msg->lines-1)
          can_pagedown = false;
        upperline = lowerline - height + 1;
        Paint();
        return true;
      }
    }
  }
  
  return false;
}


//  ------------------------------------------------------------------

int GMsgBodyView::LineUp() {
  
  if(msg->lines > height) {
    if(upperline) {
      window.scroll_down();
      upperline--;
      lowerline--;
      #ifdef GOLD_MOUSE
      gmou.HideCursor();
      #endif
      PaintLine(0, msg->line[upperline]);
      #ifdef GOLD_MOUSE
      gmou.ShowCursor();
      #endif
      can_pagedown = lowerline < (msg->lines-1);
      UpdateScrollbar();
      return true;
    }
  }
  return false;
}


//  ------------------------------------------------------------------

int GMsgBodyView::LineDown() {

  if(msg->lines > height) {
    if(lowerline < (msg->lines-1)) {
      window.scroll_up();
      upperline++;
      lowerline++;
      #ifdef GOLD_MOUSE
      gmou.HideCursor();
      #endif
      PaintLine(height-1, msg->line[lowerline]);
      #ifdef GOLD_MOUSE
      gmou.ShowCursor();
      #endif
      can_pagedown = lowerline < (msg->lines-1);
      UpdateScrollbar();
      return true;
    }
  }

  return false;
}


//  ------------------------------------------------------------------

int GMsgBodyView::Continue() {
  
  if(can_pagedown)
    return PageDown();
  
  return false;
}


//  ------------------------------------------------------------------

int GMsgBodyView::ThumbTrack(int pos) {

  return ThumbPosition(pos);
}


//  ------------------------------------------------------------------

int GMsgBodyView::ThumbPosition(int pos) {
  
  upperline = pos;
  lowerline = upperline + height - 1;
  can_pagedown = lowerline < (msg->lines-1);
  Paint();
  return true;
}


//  ------------------------------------------------------------------

void GMsgBodyView::UpdateScrollbar() {

  if(CFG->switches.get(disppagebar) and scrollbar_visible)
    window.vscrollbar(msg->lines, msg->lines-height, upperline);
}


//  ------------------------------------------------------------------

void GMsgBodyView::prints(int wrow, int wcol, int atr, const char* str) {

  vputs(wrow, wcol, atr, str);
}


//  ------------------------------------------------------------------

