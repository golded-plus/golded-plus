
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
//  Nodelist lookup and browser.
//  ------------------------------------------------------------------

#include <golded.h>
#include <gftnnlfd.h>
#include <gftnnlfu.h>
#include <gftnnlge.h>
#include <gftnnlv7.h>


//  ------------------------------------------------------------------

typedef char ListStr[160];


//  ------------------------------------------------------------------
//  Static data for the browser and associated functions

static ftn_nodelist_index_base* NLP = NULL;
static ftn_nodelist_entry       entry;


//  ------------------------------------------------------------------

class NodelistBrowser {

public:

  int  ypos;                         // Window Starting Row
  int  xpos;                         // Window Starting Column
  int  ylen;                         // Window Height
  int  xlen;                         // Window Width
  int  btype;                        // Window Border Type
  int  battr;                        // Window Border Color
  int  wattr;                        // Window Color
  int  tattr;                        // Window Title Color
  int  sattr;                        // Window Selection Bar Color
  int  hattr;                        // Window Highlight Color
  int  loattr;                       // Window LoAttr Color
  int  sbattr;                       // Window Scrollbar Color
  char*  title;                        // Window Title
  int  helpcat;                      // Window Help Category

  int pos;                           // Display Pos (1 to maxpos)
  int maxpos;                        // Display Pos

  int  aborted;                      // True if aborted
  int  listwrap;                     // True if wrap-around is supported

  ftn_nodelist_entry*  entries;
  ListStr* liststr;
  gwindow nodewin;
  gwindow listwin;
  char     user_maybe[45];
  char     titlet[80];
  int     user_fuzidx;
  int     newmaybe;
  int     firstkey;

  NodelistBrowser();
  ~NodelistBrowser();

  void Open();
  void Close();

  void BeforeCursor();
  void AfterCursor();

  void InitDisplay();

  void DisplayBar();
  void DisplayLine(int line=-1);
  void DisplayPage();

  void Home();
  void End();

  void Up();
  void Down();

  void PageUp();
  void PageDown();

  void Center();

  int Run();

  void BuildListString(int line);

  void ScrollUp();
  void ScrollDown();

  int DoKey(gkey& keycode);
};


//  ------------------------------------------------------------------

NodelistBrowser::NodelistBrowser() {

  // Not implemented yet
}


//  ------------------------------------------------------------------

NodelistBrowser::~NodelistBrowser() {

  throw_release(liststr);
  throw_release(entries);
}


//  ------------------------------------------------------------------

void NodelistBrowser::Open() {

  maxpos = ylen - 4;
  
  vcurhide();
  nodewin.openxy(ypos, xpos, 2, xlen, 5, battr, wattr, sbattr);
  listwin.openxy(ypos+2, xpos, ylen-2, xlen, btype, battr, wattr, sbattr);

  entries = (ftn_nodelist_entry*)throw_calloc(maxpos, sizeof(ftn_nodelist_entry));
  liststr = (ListStr*)throw_calloc(maxpos, sizeof(ListStr));

  InitDisplay();
}


//  ------------------------------------------------------------------

void NodelistBrowser::Close() {

  listwin.close();
  nodewin.close();
}


//  ------------------------------------------------------------------

void NodelistBrowser::BeforeCursor() {

  // Nothing to do
}


//  ------------------------------------------------------------------

void NodelistBrowser::AfterCursor() {

  char buf[200], line1[200], line2[200];

  ftn_nodelist_entry* entryp = entries + (pos - 1);

  sprintf(line1, " %s%s%s%s%s%s ",
    entryp->name,
    (*entryp->system ? ", " : ""), entryp->system,
    (*entryp->status ? " <" : ""), entryp->status,
    (*entryp->status ? ">" : "")
  );
  sprintf(buf, "%s %s", LNG->Phone, *entryp->phone ? entryp->phone : "-Unpublished-");
  strrjust(strsetsz(buf, MAXCOL-strlen(line1)-1));
  strcat(line1, buf);
  strcat(line1, " ");

  sprintf(line2, " %s%s%s",
    entryp->location,
    (*entryp->location ? ", " : ""), entryp->address
  );
  sprintf(buf, "%s%s%s%s",
    entryp->baud,
    (*entryp->baud ? " Bps" : ""),
    ((*entryp->baud and *entryp->flags) ? ", " : ""), entryp->flags
  );
  strrjust(strsetsz(buf, MAXCOL-strlen(line2)-1));
  strcat(line2, buf);
  strcat(line2, " ");
  nodewin.prints(0,0, wattr, line1);
  nodewin.prints(1,0, wattr, line2);

  Path nlname;
  *nlname = NUL;
  if(NLP->index_name() and NLP->nodelist_name())
    sprintf(nlname, " %s / %s ", NLP->index_name(), CleanFilename(NLP->nodelist_name()));
  else if(NLP->index_name())
    sprintf(nlname, " %s ", NLP->index_name());
  if(*nlname)
    listwin.title(nlname, tattr, TCENTER|TBOTTOM);
}


//  ------------------------------------------------------------------

void NodelistBrowser::InitDisplay() {

  sprintf(titlet, LNG->Lookup, user_maybe);
  listwin.title(titlet, tattr, TCENTER);

  Center();
}


//  ------------------------------------------------------------------

void NodelistBrowser::DisplayBar() {

  listwin.prints(pos-1, 0, sattr, liststr[pos-1]);
}


//  ------------------------------------------------------------------

void NodelistBrowser::DisplayLine(int line) {

  if(line == -1)
    line = pos;
  listwin.prints(line-1, 0, wattr, liststr[line-1]);
}


//  ------------------------------------------------------------------

void NodelistBrowser::BuildListString(int line) {

  int x1 = (MAXCOL-80)/3;
  int x2 = x1;
  int x3 = (MAXCOL-80) - (x1+x2);

  ftn_nodelist_entry* entryp = entries + (line - 1);
  *entryp = NLP->entry();

  if(NLP->browsing_names())
    sprintf(liststr[line-1], " %-*.*s %-*.*s %-*.*s ", 24+x1, 24+x1, entryp->name, 21+x2, 21+x2, entryp->address, 29+x3, 29+x3, entryp->system);
  else
    sprintf(liststr[line-1], " %-*.*s %-*.*s %-*.*s ", 21+x2, 21+x2, entryp->address, 24+x1, 24+x1, entryp->name, 29+x3, 29+x3, entryp->system);
}


//  ------------------------------------------------------------------

void NodelistBrowser::DisplayPage() {

  w_info(LNG->Wait);

  int line = 0;
  while(line < maxpos)
    entries[line++].addr.net = 0;

  NLP->push_state();

  // Get entries above and including current entry
  line = pos;
  do {
   BuildListString(line--);
  } while(line and NLP->previous());

  NLP->pop_state();
  NLP->push_state();

  // Get entries below current entry
  line = pos + 1;
  while((line <= maxpos) and NLP->next())
    BuildListString(line++);

  NLP->pop_state();

  w_info(NULL);

  line = 1;
  char linebuf[200];
  linebuf[MAXCOL-2] = NUL;

  // Display blank lines if necessary
  memset(linebuf, ' ', MAXCOL-2);
  while(line <= maxpos) {
    if(entries[line-1].addr.net)
      break;
    // Display separator line if necessary
    if(entries[line].addr.net)
      memset(linebuf, _box_table(btype, 1), MAXCOL-2);
    listwin.prints(line-1, 0, wattr|ACSET, linebuf);
    line++;
  }

  // Display entry lines
  while((line <= maxpos) and entries[line-1].addr.net) {
    if(line == pos)
      DisplayBar();
    else
      DisplayLine(line);
    line++;
  }

  // Display separator line if necessary
  memset(linebuf, _box_table(btype, 1), MAXCOL-2);
  if(line <= maxpos) {
    listwin.prints(line-1, 0, wattr|ACSET, linebuf);
    line++;
  }

  // Display blank lines if necessary
  memset(linebuf, ' ', MAXCOL-2);
  while(line <= maxpos) {
    listwin.prints(line-1, 0, wattr, linebuf);
    line++;
  }
}


//  ------------------------------------------------------------------

void NodelistBrowser::Home() {

  pos = 1;
  NLP->first();
  DisplayPage();
}


//  ------------------------------------------------------------------

void NodelistBrowser::End() {

  pos = maxpos;
  NLP->last();
  DisplayPage();
}


//  ------------------------------------------------------------------

void NodelistBrowser::ScrollUp() {

  memmove(entries, entries+1, (maxpos-1)*sizeof(ftn_nodelist_entry));
  memmove(liststr, liststr+1, (maxpos-1)*sizeof(ListStr));
  BuildListString(maxpos);
  listwin.scroll_up();
}


//  ------------------------------------------------------------------

void NodelistBrowser::ScrollDown() {

  memmove(entries+1, entries, (maxpos-1)*sizeof(ftn_nodelist_entry));
  memmove(liststr+1, liststr, (maxpos-1)*sizeof(ListStr));
  BuildListString(1);
  listwin.scroll_down();
}


//  ------------------------------------------------------------------

void NodelistBrowser::Up() {

  if(NLP->previous()) {
    if(pos > 1) {
      DisplayLine();
      pos--;
    }
    else {
      DisplayLine();
      ScrollDown();
    }
  }
  else if(listwrap) {
    End();
  }
}


//  ------------------------------------------------------------------

void NodelistBrowser::Down() {

  if(NLP->next()) {
    if(pos < maxpos) {
      DisplayLine();
      pos++;
    }
    else {
      DisplayLine();
      ScrollUp();
    }
  }
  else if(listwrap) {
    Home();
  }
}


//  ------------------------------------------------------------------

void NodelistBrowser::PageUp() {

  if(pos > 1) {
    DisplayLine();
    do {
      if(not NLP->previous())
        break;
    } while(--pos > 1);
  }
  else {
    int count = 1;
    while(count < maxpos) {
      if(not NLP->previous())
        break;
      count++;
    }
    DisplayPage();
  }
}


//  ------------------------------------------------------------------

void NodelistBrowser::PageDown() {

  if(pos < maxpos) {
    DisplayLine();
    do {
      if(not NLP->next())
        break;
    } while(++pos < maxpos);
  }
  else {
    int count = 1;
    while(count < maxpos) {
      if(not NLP->next())
        break;
      count++;
    }
    DisplayPage();
  }
}


//  ------------------------------------------------------------------

void NodelistBrowser::Center() {

  switch(CFG->displistcursor) {
    case LIST_TOP:
      pos = 1;
      break;
    case LIST_NEARTOP:
      pos = (maxpos / 4) + 1;
      break;
    case LIST_MIDDLE:
      pos = (maxpos / 2) + 1;
      break;
    case LIST_NEARBOTTOM:
      pos = (3 * (maxpos / 4)) + 1;
      break;
    case LIST_BOTTOM:
      pos = maxpos;
      break;
  }
  DisplayPage();
}


//  ------------------------------------------------------------------

int NodelistBrowser::Run() {

  int keyok;
  gkey newkey;
  gkey keycode;

  #ifdef GOLD_MOUSE
  gmou.HideCursor();
  #endif

  Open();
  if(helpcat)
    whelppcat(helpcat);

  do {

    listwin.move_cursor(pos-1, 0);
    DisplayBar();
    AfterCursor();
    #ifdef GOLD_MOUSE
    gmou.ShowCursor();
    #endif
    keyok = YES;

    do {
      newkey = keycode = getxchtick();
      if(newkey == Key_Tick)
        DoKey(keycode);
    } while(newkey == Key_Tick);

    do {
      newkey = 0;
      #ifdef GOLD_MOUSE
      gmou.HideCursor();
      #endif
      switch(keycode) {
        case Key_Up:
          BeforeCursor();
          Up();
          break;
        case Key_Dwn:
          BeforeCursor();
          Down();
          break;
        case Key_PgUp:
          BeforeCursor();
          PageUp();
          break;
        case Key_PgDn:
          BeforeCursor();
          PageDown();
          break;
        case Key_Home:
          BeforeCursor();
          Home();
          break;
        case Key_End:
          BeforeCursor();
          End();
          break;
        default:
          keyok = DoKey(keycode);
          newkey = keycode;
      }
      if(newkey)
        keycode = newkey;
    } while(newkey);
  } while(keyok);

  #ifdef GOLD_MOUSE
  gmou.HideCursor();
  #endif

  if(helpcat)
    whelpop();
  Close();

  #ifdef GOLD_MOUSE
  gmou.ShowCursor();
  #endif

  return aborted;
}


//  ------------------------------------------------------------------

int NodelistBrowser::DoKey(gkey& keycode) {

  gkey key = keycode;
  keycode = 0;
  if(key < KK_Commands) {
    gkey tmpkey = key_tolower(key);
    gkey kk = SearchKey(tmpkey, NodeKey, NodeKeys);
    if(kk)
      key = kk;
  }

  switch(key) {
    case KK_NodeAskExit:
      {
        GMenuQuit MenuQuit;
        if(MenuQuit.Run())
          keycode = KK_NodeQuitNow;
      }
      break;

    case KK_NodeQuitNow:
      gkbd.quitall = YES;
      if(gkbd.kbuf == NULL)
        kbput(Key_Esc);
      // Drop through

    case KK_NodeAbort:
      newmaybe = NO;
      aborted = YES;
      // Drop Through

    case KK_NodeSelect:
      if(newmaybe) {
        newmaybe = NO;
        firstkey = YES;
        Addr matchaddr;
        matchaddr.set_all(0xFFFF);
        matchaddr.set(user_maybe);
        bool gotzone = false;
        bool gotnet = false;
        bool gotnode = false;
        bool gotpoint = false;
        if(matchaddr.zone != 0xFFFF)
          gotzone = true;
        else
          matchaddr.zone = 0;
        if(matchaddr.net != 0xFFFF)
          gotnet = true;
        else
          matchaddr.net = 0;
        if(matchaddr.node != 0xFFFF)
          gotnode = true;
        else
          matchaddr.node = 0;
        if(matchaddr.point != 0xFFFF)
          gotpoint = true;
        else
          matchaddr.point = 0;
        if(gotzone or gotnet or gotnode or gotpoint) {
          if(not gotzone)
            matchaddr.zone = AA->Aka().addr.zone;
          if(not gotnet)
            matchaddr.net = AA->Aka().addr.net;
          if(gotpoint and not gotnode)
            matchaddr.node = AA->Aka().addr.node;
          if(matchaddr.point >= GFTN_FIRST)
            matchaddr.point = 0;
          if(matchaddr.node >= GFTN_FIRST)
            matchaddr.node = 0;
          if(matchaddr.net >= GFTN_FIRST)
            matchaddr.net = 0;
          if(matchaddr.zone >= GFTN_FIRST)
            matchaddr.zone = 0;
        }

        if(matchaddr.net)
          NLP->find(matchaddr);
        else
          NLP->find(user_maybe);
        InitDisplay();
      }
      else {
        BeforeCursor();
        return NO;
      }
      break;

    case Key_Tab:
      if(NLP->browsing_names() and NLP->can_browse_address()) {
        strcpy(user_maybe, NLP->address());
        NLP->find(NLP->addrs());
      }
      else if(NLP->browsing_addresses() and NLP->can_browse_name()) {
        strcpy(user_maybe, NLP->name());
        NLP->find(NLP->name());
      }
      user_fuzidx = strlen(user_maybe);
      InitDisplay();
      break;

    case KK_NodeGotoPrev:
      keycode = Key_Up;
      firstkey = YES;
      break;

    case KK_NodeGotoNext:
      keycode = Key_Dwn;
      firstkey = YES;
      break;

    case KK_NodeGotoFirst:
      keycode = Key_Home;
      firstkey = YES;
      break;

    case KK_NodeGotoLast:
      keycode = Key_End;
      firstkey = YES;
      break;

    case KK_NodeDosShell:
      DosShell();
      break;

    case Key_Tick:
      CheckTick(KK_NodeQuitNow);
      break;

    case KK_NodeUndefine:
      break;

    default:
      if(not PlayMacro(key, KT_N)) {
        int n = key & 0xFF;
        if((user_fuzidx < 41) or (key == Key_BS)) {
          if(firstkey) {
            if((key != Key_BS) and not isspace(n)) {
              *user_maybe = 0;
              user_fuzidx = 0;
            }
          }
          firstkey = NO;
          if((n >= ' ') or (key == Key_BS)) {
            newmaybe = YES;
            if(key != Key_BS)
              user_maybe[user_fuzidx++] = (char)n;
            else if(user_fuzidx)
              user_maybe[--user_fuzidx] = 0;
            user_maybe[user_fuzidx] = 0;
            sprintf(titlet, LNG->Lookup, user_maybe);
            listwin.title(titlet, tattr, TCENTER);
          }
        }
      }
  }

  return YES;
}


//  ------------------------------------------------------------------

static int browse_nodelist(GMsg* msg, char* title, int topline) {

  w_info(NULL);

  NodelistBrowser* browser = new NodelistBrowser;
  throw_new(browser);

  browser->btype = W_BMENU;
  browser->battr = C_MENUB;
  browser->wattr = C_MENUW;
  browser->sattr = C_MENUS;
  browser->tattr = C_MENUT;
  browser->hattr = C_MENUQ;
  browser->sbattr = C_MENUPB;
  browser->helpcat = H_NodelistBrowser;
  browser->ypos = topline;
  browser->xpos = 0;
  browser->ylen = MAXROW-browser->ypos-1;
  browser->xlen = MAXCOL;
  browser->listwrap = CFG->switches.get(displistwrap);
  browser->firstkey = YES;
  browser->newmaybe = NO;
  browser->aborted = NO;
  strxcpy(browser->user_maybe, title, sizeof(browser->user_maybe));
  browser->user_fuzidx = strlen(browser->user_maybe);

  int aborted = browser->Run();
  if(not aborted)
    entry = browser->entries[browser->pos-1];

  delete browser;

  return (not aborted);
}


//  ------------------------------------------------------------------

static bool NLP_open() {

  if(*CFG->nodepathv7) {
    NLP = new ftn_version7_nodelist_index;
    throw_new(NLP);
    NLP->set_path(CFG->nodepathv7);
  }
  else if(*CFG->nodepathfd) {
    NLP = new ftn_frontdoor_nodelist_index;
    throw_new(NLP);
    NLP->set_path(CFG->nodepathfd);
  }
  else if(*CFG->fidouserlist) {
    NLP = new ftn_fidouser_nodelist_index;
    throw_new(NLP);
    NLP->set_path(CFG->fidouserlist);
  }
  else {
    CheckNodelists();
    NLP = new ftn_golded_nodelist_index;
    throw_new(NLP);
    NLP->set_path(CFG->nodepath);
  }

  return NLP->open();
}


//  ------------------------------------------------------------------

static void NLP_close() {

  NLP->close();
  throw_delete(NLP);
}


//  ------------------------------------------------------------------

static Name nlname;

const char *lookup_nodelist(ftn_addr* addr) {

  const char *r = NULL;

  if(NLP_open()) {

    NLP->find(*addr);
    if(NLP->found()) {
      strcpy(nlname, NLP->entry().name);
      r = nlname;
    }
  }
  NLP_close();
  return r;
}


//  ------------------------------------------------------------------

void Lookup(GMsg* msg, Addr* addr, char* name, int topline, char* status) {

  char* ptr;
  INam buf, buf1, tmpname;
  bool found = false;
  int robotchk;
  bool dolookup = true;
  bool namelookup = true;
  Addr matchaddr;
  bool exactmatch = true;

  strtrim(name);
  if(*name == NUL) {
    matchaddr = AA->Aka().addr;
    namelookup = false;
    exactmatch = false;
  }
  strcpy(tmpname, name);

  if(not CFG->addressmacro.empty()) {
    vector<AddrMacro>::iterator n;
    ptr = name;
    strcpy(buf, ptr);
    for(n=CFG->addressmacro.begin(); n != CFG->addressmacro.end(); n++) {
      if(strieql(ptr, n->macro)) {
        strcpy(buf1, n->name);
        ptr = buf1;
        if(*buf1 == '@') {
          // UUCP/INTERNET addressing
          ptr = strchr(buf1, '/');
          if(ptr) {
            *ptr++ = NUL;
            strcpy(msg->iaddr, ptr);
            strcpy(msg->idest, ptr);
            ptr = buf1+1;
          }
          else {
            strcpy(msg->iaddr, buf1+1);
            strcpy(msg->idest, buf1+1);
            ptr = strcpy(buf1, *AA->Internetgate().name ? AA->Internetgate().name : "UUCP");
          }
        }
        strcpy(name, ptr);
        if(n->addr.net)
          *addr = n->addr;
        else
          *addr = AA->Internetgate().addr;
        strtrim(name);
        if(topline >= 0)
          return;
      }
    }
  }

  // Automatic internet gating
  if(strchr(name, '@') and AA->Internetgate().addr.net) {
    strcpy(msg->idest, name);
    if(*AA->Internetgate().name) {
      strcpy(msg->iaddr, name);
      strcpy(msg->to, AA->Internetgate().name);
      strcpy(name, msg->to);
    }
    *addr = AA->Internetgate().addr;
    namelookup = false;
    if(topline >= 0)
      return;
  }

  // Check if name is really an address
  if(*tmpname) {
    matchaddr.set_all(0xFFFF);
    matchaddr.set(name);
    bool gotzone = false;
    bool gotnet = false;
    bool gotnode = false;
    bool gotpoint = false;
    if(matchaddr.zone != 65535u)
      gotzone = true;
    else
      matchaddr.zone = 0;
    if(matchaddr.net != 65535u)
      gotnet = true;
    else
      matchaddr.net = 0;
    if(matchaddr.node != 65535u)
      gotnode = true;
    else
      matchaddr.node = 0;
    if(matchaddr.point != 65535u)
      gotpoint = true;
    else
      matchaddr.point = 0;
    if(gotzone or gotnet or gotnode or gotpoint) {
      *addr = matchaddr;
      if(not gotzone)
        addr->zone = AA->Aka().addr.zone;
      if(not gotnet)
        addr->net = AA->Aka().addr.net;
      if(gotpoint and not gotnode)
        addr->node = AA->Aka().addr.node;
      matchaddr = *addr;
      namelookup = false;
      if(matchaddr.point >= GFTN_FIRST) {
        matchaddr.point = 0;
        exactmatch = false;
      }
      if(matchaddr.node >= GFTN_FIRST) {
        matchaddr.node = 0;
        exactmatch = false;
      }
      if(matchaddr.net >= GFTN_FIRST) {
        matchaddr.net = 0;
        exactmatch = false;
      }
      if(matchaddr.zone >= GFTN_FIRST) {
        matchaddr.zone = 0;
        exactmatch = false;
      }
    }
  }

  if(namelookup) {
    ptr = strrchr(name, ' ');
    if(ptr != NULL) {
      addr->reset();
      addr->set(ptr+1);
      if(addr->net) {
        // Address was given
        *ptr = NUL;
        strtrim(name);
        if(topline >= 0)
          return;
      }
    }
  }

  robotchk = NO;
  if(striinc("Sysop", name) or strieql(AA->Whoto(), name))
    robotchk = YES;
  else {
    for(gstrarray::iterator n = CFG->robotname.begin(); n != CFG->robotname.end(); n++)
      if(striinc(n->c_str(), name)) {
        robotchk = YES;
        break;
      }
  }

  if(robotchk) {
    addr->reset();
    if(topline >= 0)
      return;
  }

  if(topline >= 0) {
    dolookup = false;
    if(AA->isnet() and CFG->switches.get(lookupnet))
      dolookup = true;
    else if(AA->isecho() and CFG->switches.get(lookupecho))
      dolookup = true;
    else if(AA->islocal() and CFG->switches.get(lookuplocal))
      dolookup = true;
  }

  if(dolookup) {

    if(NLP_open()) {

      if(namelookup)
        NLP->find(name);
      else
        NLP->find(matchaddr);

      found = (exactmatch and NLP->found()) ? true : false;
      if(found) {
        NLP->push_state();
        if(NLP->next())
          if(NLP->found())
            found = false;
        NLP->pop_state();
      }
      entry = NLP->entry();

      if(not found or (topline < 0)) {
        strcpy(buf, information);
        update_statusline(status);
        found = browse_nodelist(msg, tmpname, abs(topline));
        update_statusline(buf);
      }
    }
    else {
      w_info(LNG->NoNodelist);
      waitkeyt(10000);
      w_info(NULL);
    }

    NLP_close();
  }

  if(found) {
    strcpy(name, entry.name);
    *addr = entry.addr;
  }
  else {
    addr->reset();
  }

  if(topline >= 0) {
    if(CFG->switches.get(internetlookup)) {
      if(*entry.system and strchr(entry.system, '@') and AA->Internetgate().addr.net) {
        strcpy(msg->iaddr, entry.system);
        strcpy(msg->idest, entry.system);
        if(*AA->Internetgate().name)
          strcpy(msg->to, AA->Internetgate().name);
        else if(strlen(entry.system) >= sizeof(Name))
          strcpy(msg->to, *AA->Internetgate().name ? AA->Internetgate().name : "UUCP");
        else {
          strcpy(msg->to, entry.system);
          *msg->iaddr = NUL;
          *msg->idest = NUL;
        }
        strcpy(name, msg->to);
        *addr = AA->Internetgate().addr;
      }
    }
  }
}


//  ------------------------------------------------------------------
//  Lookup and display info about the node

void LookupNode(GMsg* msg, const char* name, int what) {

  Subj statuslinebak;
  strcpy(statuslinebak, information);

  vcurhide();
  w_info(LNG->Wait);

  Addr addr;
  char buf[256];
  char* ptr = buf;

  switch(what) {
    case LOOK_DEST:
      if(AA->isnet()) {
        addr = msg->dest;
        if(addr.zone == 0)
          addr.zone = AA->Aka().addr.zone;
        addr.make_string(buf);
      }
      else {
        addr.reset();
        strcpy(buf, msg->to);
      }
      break;
    case LOOK_ORIG:
      addr = msg->orig;
      if(addr.zone == 0)
        addr.zone = AA->Aka().addr.zone;
      addr.make_string(buf);
      break;
    case LOOK_NAME:
      addr = msg->orig;
      strcpy(buf, strskip_wht(name));
      strtok(buf, " \t\n\r");   // Only lookup first word
      break;
  }
  Lookup(msg, &addr, ptr, -6, LNG->LookupInfo);

  update_statusline(statuslinebak);

  w_info(NULL);
}


//  ------------------------------------------------------------------

void CheckNodelists() {

  // Copy of previous timestamp
  static long oldft = 0;

  // Get timestamp of the .GXL file
  Path file;
  strcpy(file, AddPath(CFG->nodepath, "goldnode.gxl"));
  long ft = GetFiletime(file);

  // Check nodelists if timestamp changed
  if(ft != oldft) {

    // Keep copy of timestamp for later lookups
    oldft = ft;

    strcpy(NODE->addrindex, AddPath(CFG->nodepath, "goldnode.gxa"));
    strcpy(NODE->nodeindex, AddPath(CFG->nodepath, "goldnode.gxn"));

    Subj statuslinebak;
    strcpy(statuslinebak, information);

    update_statusline(LNG->CheckingNodelists);

    FILE* fp = fsopen(file, "rt", CFG->sharemode);
    if(fp) {

      // Read the list index
      char buf[256];
      NODE->nodelists = 0;
      while(fgets(buf, sizeof(buf), fp)) {
        NODE->nodelist = (Stamp*)throw_realloc(NODE->nodelist, (NODE->nodelists+1)*sizeof(Stamp));
        char* key;
        char* val=buf;
        getkeyval(&key, &val);
        key = strxcpy(NODE->nodelist[NODE->nodelists].fn, strbtrim(key), sizeof(Path));
        MakePathname(key, CFG->nodepath, key);
        NODE->nodelist[NODE->nodelists].ft = atol(val);
        NODE->nodelists++;
      }
      fclose(fp);

      // Check the files
      if(CFG->switches.get(nodelistwarn)) {
        for(int n=0; n<NODE->nodelists; n++) {
          if(not fexist(MapPath(NODE->nodelist[n].fn))) {
            w_infof(" %s %s ", LNG->NodelistMissing, NODE->nodelist[n].fn);
            HandleGEvent(EVTT_ATTENTION);
            waitkeyt(10000);
            w_info(NULL);
          }
          else if(abs(long(GetFiletime(NODE->nodelist[n].fn)-NODE->nodelist[n].ft))>1) {
            w_infof(" %s %s ", LNG->NodelistOutdated, NODE->nodelist[n].fn);
            HandleGEvent(EVTT_ATTENTION);
            waitkeyt(10000);
            w_info(NULL);
          }
        }
      }
    }

    update_statusline(statuslinebak);
  }

  throw_release(NODE->nodelist);
  NODE->nodelists = 0;
}

//  ------------------------------------------------------------------

