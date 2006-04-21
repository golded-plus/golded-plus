
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

#if defined(__USE_ALLOCA__)
  #include <malloc.h>
#endif


//  ------------------------------------------------------------------

GMsgHeaderView::GMsgHeaderView() {

  alive = false;
  at_row = at_column = width = height = 0;
  border_type = gwindow::bordertype_none;
  window_color = from_color = to_color = subject_color = BLUE_|_LGREY;
  location_color = BLUE_|_LGREY;
  title_color = highlight_color = RED_|_LGREY;
  border_color = YELLOW_|_LGREY;
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

  ISub buf;
  int namewidth = CFG->disphdrnodeset.pos - CFG->disphdrnameset.pos;
  int nodewidth = CFG->disphdrdateset.pos - CFG->disphdrnodeset.pos;
  int datewidth = MinV(width - CFG->disphdrdateset.pos, CFG->disphdrdateset.len);

  #if defined(GUTLOS_FUNCS)
  g_set_ostitle_name(struplow(strtmp(area->echoid())), 0);
  #endif

  // Generate top line fields
  char buf1[16];
  if((CFG->dispareano == ALWAYS) or (CFG->dispareano and area->board()))
    sprintf(buf1, "[%u] ", area->board());
  else
    *buf1 = NUL;

#if defined(__USE_ALLOCA__)
  char *top = (char*)alloca(width+1);
#else
  __extension__ char top[width+1];
#endif
  strxmerge(top, width+1, " ", buf1, strtrim(strtmp(area->desc())), " (",
            area->isinternet() ? area->Internetaddress() : area->Aka().addr.make_string(buf),
            ") ", NULL);

  int desclen = strlen(top);

  window.printc(0, 0, border_color|ACSET, _box_table(W_BHEAD, 1));
  window.prints(0, 1, title_color, top);

  if(msg->areakludgeid)
    strxmerge(top, width+1, " ", area->echoid(), " (", msg->areakludgeid, ") ", NULL);
  else
    strxmerge(top, width+1, " ", area->echoid(), " ", NULL);

  int taglen = strlen(top);

  if((width - (desclen + taglen + 2)) > 0)
    window.fill(0, desclen+1, 0, width-(taglen+1)-1, _box_table(W_BHEAD, 1), border_color|ACSET);

  window.prints(0, width-taglen-1, title_color, top);
  window.printc(0, width-1, border_color|ACSET, _box_table(W_BHEAD, 1));

  // Generate message attributes string
  bool attrsgenerated = false;
  MakeAttrStr(buf, width-CFG->disphdrnodeset.pos, &msg->attr);
  if(*buf) {
    attrsgenerated = true;
    strsetsz(buf, width-CFG->disphdrnodeset.pos);
    window.prints(1, CFG->disphdrnodeset.pos, window_color, buf);
  }

  // Generate message number and reply links string
  char* ptr = buf;
  int list_max = msg->link.list_max();
  uint32_t* replies = (uint32_t*)throw_calloc(list_max+1, sizeof(uint32_t));
  uint32_t replyto, replynext;
  if(CFG->switches.get(disprealmsgno)) {
    ptr += sprintf(ptr, " %-5.5s: #%u [%u]", LNG->Msg, msg->msgno, area->Msgn.Count()+(msg->attr.nwm() ? 1 : 0));
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
    ptr += sprintf(ptr, " -%u", replyto);
  for(int replyn=0,plus=0; (replyn<(list_max+1)) and (not attrsgenerated or ((ptr-buf)<CFG->disphdrnodeset.pos)); replyn++)
    if(replies[replyn])
      ptr += sprintf(ptr, " %s%u", plus++?"":"+", replies[replyn]);
  if(replynext and (not attrsgenerated or ((ptr-buf)<CFG->disphdrnodeset.pos)))
    sprintf(ptr, " *%u", replynext);
  throw_free(replies);

  strsetsz(buf, attrsgenerated ? CFG->disphdrnodeset.pos : width);
  window.prints(1, 0, window_color, buf);

  // Get marks
  if(area->bookmark == msg->msgno)
    window.prints(1, 5, highlight_color, "\x11");
  if(area->Mark.Count() and area->Mark.Find(msg->msgno))
    window.prints(1, 7, highlight_color, "\x10");

  // Generate from info
  bool nodegenerated = false;
  if(not area->isinternet()) {
    if(area->isecho() or not (*msg->ifrom and (*msg->realby or *msg->iorig)))
    {
      // Generate orig node data
      if(msg->orig.net)
        msg->orig.make_string(buf);
      else
        *buf = NUL;
      nodegenerated = true;
      strsetsz(buf, nodewidth);
      window.prints(2, CFG->disphdrnodeset.pos, from_color, buf);
    }
  }

  if((not area->isecho() or area->isnewsgroup()) and *msg->ifrom and *msg->realby)
    strxmerge(buf, (namewidth+nodewidth), msg->realby, " <", msg->iorig, ">", NULL);
  else if((not area->isecho() or area->isnewsgroup()) and *msg->ifrom and *msg->iorig)
    strxcpy(buf, msg->iorig, (namewidth+nodewidth));
  else
    strxcpy(buf, msg->By(), (namewidth+nodewidth));

  strsetsz(buf, nodegenerated ? namewidth : (namewidth+nodewidth));

  window.prints(2, 0, window_color, LNG->From);
  vattr color = ((msg->foundwhere&GFIND_FROM) or msg->attr.fmu() or (msg->attr.loc() and CFG->switches.get(displocalhigh))) ? highlight_color : from_color;
  color = GetColorName(msg->By(), msg->orig, color);
  window.prints(2, CFG->disphdrnameset.pos, color, buf);

  if (datewidth > 0)
  {
    if (msg->written)
    {
      struct tm tm; ggmtime(&tm, &msg->written);
      strftimei(buf, datewidth+1, LNG->DateTimeFmt, &tm);
    }
    else
      *buf = NUL;

    strsetsz(buf, datewidth);
    window.prints(2, CFG->disphdrdateset.pos, from_color, buf);
  }

  // Generate dest node data
  nodegenerated = false;
  if(not area->isinternet()) {
    if(not (*msg->ito and (*msg->realto or *msg->idest)))
    {
      if(msg->dest.net and area->isnet()) {
        msg->dest.make_string(buf);
        if(msg->odest.net) {
          if((msg->odest.net != msg->dest.net) or (msg->odest.node != msg->dest.node)) {
            sprintf(buf+strlen(buf), " %s %u/%u", LNG->Via, msg->odest.net, msg->odest.node);
          }
        }
        nodegenerated = true;
        strsetsz(buf, nodewidth);
        window.prints(3, CFG->disphdrnodeset.pos, to_color, buf);
      }
    }
  }

  if((not area->isecho() or area->isnewsgroup()) and *msg->ito and *msg->realto)
    strxmerge(buf, (namewidth+nodewidth), msg->realto, " <", msg->idest, ">", NULL);
  else if((not area->isecho() or area->isnewsgroup()) and *msg->ito and *msg->idest)
    strxcpy(buf, msg->idest, (namewidth+nodewidth));
  else
    strxcpy(buf, msg->To(), (namewidth+nodewidth));

  strsetsz(buf, nodegenerated ? namewidth : (namewidth+nodewidth));

  window.prints(3, 0, window_color, LNG->To);
  color = ((msg->foundwhere&GFIND_TO) or msg->attr.tou()) ? highlight_color : to_color;
  { Addr zero;
    color = GetColorName(msg->To(), area->isnet() ? msg->dest : zero, color);
  }
  window.prints(3, CFG->disphdrnameset.pos, color, buf);

  if (datewidth > 0)
  {
    if (msg->arrived)
    {
      struct tm tm; ggmtime(&tm, &msg->arrived);
      strftimei(buf, datewidth+1, LNG->DateTimeFmt, &tm);
    }
    else
      *buf = NUL;

    strsetsz(buf, datewidth);
    window.prints(3, CFG->disphdrdateset.pos, to_color, buf);
  }

  // Generate subjectline
  strxcpy(buf, (msg->attr.att() or msg->attr.frq() or msg->attr.urq()) ? LNG->File : LNG->Subj, 10);
  int lngsubjlen = strlen(buf);
  window.prints(4, 0, window_color, buf);

  strxcpy(buf, msg->re, width-lngsubjlen);
  strsetsz(buf, width-lngsubjlen);
  window.prints(4, lngsubjlen, (msg->foundwhere&GFIND_SUBJECT) ? highlight_color : subject_color, buf);

  // Generate bottom line
  window.fill(5, 0, 5, width-1, _box_table(W_BHEAD, 1), border_color|ACSET);

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
        if((g_isalpha(*ptr) and (ptr[1] == ':')) or (ptr[0] == '\\') or (ptr[0] == '/'))
          strcpy(buf2, ptr);
        else
          sprintf(buf2, "%s%s", CFG->inboundpath, ptr);
        long sz = GetFilesize(MapPath(buf2));
        if(sz == -1)
          sprintf(buf1, " %s ", LNG->n_a);
        else {
          switch(CFG->dispattachsize) {
            case ATTACH_BYTES:
              sprintf(buf1, " %li ", sz);
              break;
            case ATTACH_KBYTES:
              sprintf(buf1, " %lik ", (sz+512L)/1024L);
              break;
          }
        }
        window.prints(5, begpos+int(ptr-buf)-1, title_color, buf1);
        ptr = strtok(NULL, " ");
      }
    }
  }

  if(CFG->dispmsgsize and msg->txt and not _in_editor) {
    uint len = strlen(msg->txt);
    switch(CFG->dispmsgsize) {
      case DISPMSGSIZE_BYTES:
        sprintf(buf1, "%u", len);
        break;
      case DISPMSGSIZE_KBYTES:
        sprintf(buf1, "%uk", (len+512)/1024);
        break;
      case DISPMSGSIZE_LINES:
        sprintf(buf1, "%u", msg->lines);
        break;
      default:
        *buf1 = NUL;
    }
    if(*buf1)
      window.prints(5, 1, title_color, buf1);
  }

  if (CFG->disphdrlocation && !_in_editor)
  {
    std::string loc;
    LookupNodeLocation(msg, loc, LOOK_CITY1);
    if (loc.length()) loc = " " + loc;

    if (AA->isnet())
    {
      std::string loc2;
      LookupNodeLocation(msg, loc2, LOOK_CITY2);
      if (loc.length()) loc += CFG->locationdelimiter + loc2;
    }

    if (loc.length())
    {
      loc += " ";
      int pos = window.width() - loc.length();
      pos = ((CFG->disphdrlocation >> 16) == TCENTER) ? pos/2 : pos-1;
      window.prints(5, pos, location_color, loc.c_str());
    }
  }
}


//  ------------------------------------------------------------------

GMsgBodyView::GMsgBodyView() {

  alive = false;
  scrollbar_visible = false;
  at_row = at_column = width = height = visible_width = 0;
  border_type = gwindow::bordertype_none;
  window_color = BLACK_|_LGREY;
  highlight_color = WHITE_|_RED;
  scrollbar_color = DGREY_|_LGREY;
  border_color = YELLOW_|_LGREY;
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
  window.set_scrollbar_color(scrollbar_visible ? scrollbar_color : DEFATTR);
}


//  ------------------------------------------------------------------

void GMsgBodyView::PaintLine(int row, Line *line) {

  // Calculate effective coordinates for vputs
  int vrow = gwin.active->srow + row;
  uint llen = line->txt.length();

  vattr color = (line->type & GLINE_HIGH) ? highlight_color : line->color;

  // Trim line if it longer than should be. This actually happens in very rare
  // cases, but always when hex dump displayed.
  if(llen > visible_width) {
    llen = visible_width;
    line->txt.erase(llen);
  }

  // Print it
  if(not SearchHighlight(line, vrow, visible_width, highlight_color)) {
    if(line->type & GLINE_ORIG and strneql(line->txt.c_str(), " * Origin: ", 11)) {
      prints(vrow, 0, color, " * Origin: ");
      StyleCodeHighlight(line->txt.c_str()+11, vrow, 11, not AA->attr().hex() and AA->adat->hidestylies, color);
    }
    else
      StyleCodeHighlight(line->txt.c_str(), vrow, 0, not AA->attr().hex() and AA->adat->hidestylies, color);
    int tlen = strlen(line->txt.c_str());
    printns(vrow, tlen, color, "", visible_width-tlen);
  }
  else
    printns(vrow, 0, color, line->txt.c_str(), visible_width);
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

void GMsgBodyView::prints(int wrow, int wcol, vattr atr, const char* str)
{
  vputs(wrow, wcol, atr, str);
}


//  ------------------------------------------------------------------

void GMsgBodyView::printns(int wrow, int wcol, vattr atr, const char* str, uint len)
{
  vputns(wrow, wcol, atr, str, len);
}


//  ------------------------------------------------------------------
