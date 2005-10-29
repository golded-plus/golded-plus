
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
//  Global utility functions.
//  ------------------------------------------------------------------

#include <golded.h>
#include <gwinput.h>

extern bool akamatchreply;


//  ------------------------------------------------------------------

int edit_string(char* buf, int buf_size, char* title, int helpcat) {

  gwindow window;

  window.openxy(inforow,((MAXCOL-63)/2)-1,3,63,W_BASK,C_ASKB,C_ASKW);
  window.title(title, C_ASKT);
  if(CFG->switches.get(screenshadows))
    window.shadow(C_SHADOW);

  gwinput2 iform(window);

  std::string buf2 = buf;
  iform.setup(C_ASKW, C_ASKW, C_ASKQ, _box_table(W_BASK, 13), true);
  iform.add_field(0, 0, 1, 59, buf2, buf_size, gwinput::cvt_none, fieldupd);
  vcurshow();
  iform.run(helpcat);
  vcurhide();

  window.close();

  if(iform.dropped)
    return false;

  size_t len = buf2.length();
  while(len and ('!' > buf2[--len]))
    buf2.erase(len, 1);
  strcpy(buf, buf2.c_str());
  return make_bool(*buf);
}


//  ------------------------------------------------------------------

bool edit_pathname(char* buf, int buf_size, char* title, int helpcat) {

  if(not edit_string(buf, buf_size, title, helpcat))
    return false;

  std::vector<FileAlias>::iterator z;
  for(z = CFG->filealias.begin(); z != CFG->filealias.end(); z++) {
    if(strieql(buf, z->alias)) {
      strcpy(buf, z->file);
      break;
    }
  }

  strschg_environ(buf);
  return true;
}


//  ------------------------------------------------------------------

bool PopupLocked(long tries, int isopen, const char* file) {

  // Close popup window if requested
  if(tries == 0) {
    w_info(NULL);
    return true;
  }

  // Tell the world
  char buf[200];
  if((uint) strlen(file) > (uint) (MAXCOL-25))
    sprintf(buf, "[..]%*.*s", MAXCOL-30, MAXCOL-30, file);
  else
    strcpy(buf, file);
  w_infof(LNG->WaitLocked, buf);
  update_statuslinef(LNG->RetryOrESC, isopen ? LNG->RetryLock : LNG->RetryOpen , tries);

  // Check for keypress
  if(kbxhit()) {

    // Return fail value if Escape was pressed
    gkey keyval = kbxget();
    if(keyval == Key_Esc) {
      GMenuLockLoop MenuLockLoop;
      if(MenuLockLoop.Run()) {
        w_info(NULL);
        return false;
      }
    }
  }

  // Pause approx 1 second before trying again
#ifndef __BEOS__
  usleep(1000);
#else
  sleep(1);
#endif
  // Try again
  return true;
}


//  ------------------------------------------------------------------
//  Build zonegate address from orig/dest address

void ZonegateIt(ftn_addr& gate, ftn_addr& orig, ftn_addr& dest) {

  gate.zone  = orig.zone;  // Send to the local zoneGate
  gate.net   = orig.zone;
  gate.node  = dest.zone;
  gate.point = 0;
}


//  ------------------------------------------------------------------

int GetAkaNo(const ftn_addr& aka) {

  int akano = 0;
  while(akano < CFG->aka.size()) {
    if(CFG->aka[akano].addr.equals(aka))
      return akano;
    akano++;
  }
  return -1;
}


//  ------------------------------------------------------------------

void doinvalidate(char* text, const char* find, const char* replace, bool is_tearline) {

  int n = strlen(find);
  if(strnieql(text, find, n) and (not is_tearline or (text[n] == NUL) or isspace(text[n]))) {
    char buf[256];

    strcpy(buf, text);
    strcpy(stpcpy(text, replace), &buf[n]);
  }
}


//  ------------------------------------------------------------------

int AkaMatchOne(const ftn_addr* mask, const ftn_addr* addr) {

  int match = 0;

  // Count matching parts
  if((mask->zone == 0) or (addr->zone == 0) or (mask->zone == GFTN_ALL) or (mask->zone == addr->zone)) {
    match++;
    if((mask->net == GFTN_ALL) or (mask->net == addr->net)) {
      match++;
      if((mask->node == GFTN_ALL) or (mask->node == addr->node)) {
        match++;
        if((mask->point == GFTN_ALL) or (mask->point == addr->point)) {
          match++;
        }
      }
    }
  }

  return match;
}


//  ------------------------------------------------------------------

int AkaMatch(ftn_addr* match, const ftn_addr* addr) {

  int aka;
  std::vector<AkaMatchG>::iterator am;

  for(am = CFG->akamatch.begin(), aka=0; am != CFG->akamatch.end(); am++, aka++) {
    if(addr->match(am->mask)) {
      int akano = GetAkaNo(am->aka);
      if(akano != -1) {
        *match = CFG->aka[akano].addr;
        return akano;
      }
    }
  }

  int bestaka = -1;
  int bestmatch = 0;
  int matchaka = 0;
  std::vector<gaka>::iterator a;

  for(a = CFG->aka.begin(), aka = 0; a != CFG->aka.end(); aka++, a++) {

    if(match->equals(a->addr))
    {
        if (CFG->akamatchfromto && akamatchreply)
            return aka;
        else
            matchaka = aka;
    }

    // Ignore fakenet addresses
    if(a->addr.net != a->pointnet) {

      int match = AkaMatchOne(&a->addr, addr);

      // Note if we found a better match
      if(match > bestmatch) {
        bestmatch = match;
        bestaka = aka;
      }
    }
  }

  // If there was no matching aka, use the aka
  if(bestaka == -1)
    bestaka = matchaka;
  else {
    // If the current address matches at least as good as the best match, don't change it
    if(bestmatch <= AkaMatchOne(match, addr)) {
      int akano = GetAkaNo(*match);
      if(akano != -1)
        return akano;
    }
  }

  *match = CFG->aka[bestaka].addr;
  return bestaka;
}


//  ------------------------------------------------------------------

void ScreenBlankIdle() {

  static int whb = -1;
  static int whh = -1;
  static int wascurhid = false;
  static Clock lastmoved = 0;
  int windowheight = 4;

  IdleCheckSemaphores();

  char blankmsg1[80];
  char blankmsg2[80];
  time32_t t = gtime(NULL);
  sprintf(blankmsg1, " %s %s %s ", __gver_longpid__, __gver_ver__, strftimei(blankmsg2, 40, LNG->StatusLineTimeFmt, glocaltime(&t)));
  sprintf(blankmsg2, " %s ", LNG->BlankMsg);
  if(strblank(blankmsg2)) {
    *blankmsg2 = NUL;
    windowheight--;
  }
  int b1 = strlen(blankmsg1);
  int b2 = strlen(blankmsg2);
  int blankmsglen = MaxV(b1,b2);
  int ry = rand() % (MAXROW-windowheight);
  int rx = rand() % ((MAXCOL-blankmsglen)-2);

  if(blanked and (whb == -1)) {
    wascurhid = vcurhidden();
    vcurhide();
    if(C_BACKB != 0)
      gvid->setoverscan(0);
    whb = wopen(0,0,MAXROW-1,MAXCOL-1, 5, 7, 7);
    if(CFG->screenblankertype == BLANK_SLIDEWIN)
      whh = wopen_(ry, rx, windowheight, blankmsglen+2, W_BINFO, C_INFOB, C_INFOW);
    lastmoved = gkbd.tickvalue;
  }

  if(not blanked and (whb != -1)) {
    if(CFG->screenblankertype == BLANK_SLIDEWIN)  {
      wactiv_(whh);
      wclose();
      whh = -1;
    }
    wactiv_(whb);
    wclose();
    whb = -1;
    if(C_BACKB != 0)
      gvid->setoverscan(C_BACKB);
    if(wascurhid)
      vcurhide();
    else
      vcurshow();
    return;
  }

  if(gkbd.tickvalue > (lastmoved+50L)) {
    lastmoved = gkbd.tickvalue;
    if(CFG->screenblankertype == BLANK_SLIDEWIN)
      wslide(ry, rx);
  }

  if(CFG->screenblankertype == BLANK_SLIDEWIN) {
    wcenters(0, C_INFOW, blankmsg1);
    if(*blankmsg2)
      wcenters(1, C_INFOW, blankmsg2);
  }

  // If timed out, exit screenblanker
  if(CFG->timeout)
    if(gkbd.tickvalue >= (gkbd.tickpress+(CFG->timeout*10L)))
      kbput(Key_Tick);
}


//  ------------------------------------------------------------------

int GetColorName(Addr &addr)
{
  std::vector< std::pair<Node, int> >::iterator it;

  for (it = CFG->colorname.begin(); it != CFG->colorname.end(); it++)
  {
    if (addr.match(it->first.addr))
      return it->second;
  }

  return -1;
}


//  ------------------------------------------------------------------

int GetColorName(char *name)
{
  std::vector< std::pair<Node, int> >::iterator it;

  for (it = CFG->colorname.begin(); it != CFG->colorname.end(); it++)
  {
    if (strieql(it->first.name, name))
      return it->second;
  }

  return -1;
}


//  ------------------------------------------------------------------

