//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999-2000 Alexander S. Aganichev
//  ------------------------------------------------------------------
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Library General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Library General Public License for more details.
//
//  You should have received a copy of the GNU Library General Public
//  License along with this program; if not, write to the Free
//  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
//  MA 02111-1307, USA
//  ------------------------------------------------------------------
//  $Id$
//  ------------------------------------------------------------------
//  Based on CXL by Mike Smedley.
//  ------------------------------------------------------------------
//  whelpdef()  defines the help key, file, and window colors
//  whelpwin()  defines the help window boundaries
//  whelpcat()  sets the help category
//  whelppcat() sets the help category, but pushes current one first
//  whelpopc()  pops the last saved help category into the void
//  whelpclr()  clears the help buffer
//  whelpush()  pushes the current help category onto the stack
//  whelpushc() pushes the given help category onto the stack
//  whelpop()   pops last saved help category, sets current
//  ------------------------------------------------------------------

#include <cstdio>
#include <cstdlib>
#include <gfile.h>
#include <gmemdbg.h>
#include <gkbdcode.h>
#include <gkbdbase.h>
#include <gmoubase.h>
#include <gutlmisc.h>
#include <gstrall.h>
#include <gvidall.h>
#include <gwinall.h>
#include <gwinhelp.h>


//  ------------------------------------------------------------------
//  Constants

#define MAXXREF     50
#define BUFSIZE     80
#define BASETAGID   200


//  ------------------------------------------------------------------
//  Global variables

extern int gmnudropthrough;


//  ------------------------------------------------------------------
//  Local variables

bool whelpclosefile = false;
static char* catarray[MAXXREF];
static int arraycnt = 0;
static char buf[BUFSIZE+1];

_help_t whelp = {
  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
  NULL,-1,0,0,0,0,0,3,8,21,71,0,YES,NULL,NULL,0
};


//  ------------------------------------------------------------------
//  Help index file record definition

static Hlpr recd;


//  ------------------------------------------------------------------

static void esc_esc() {

  setonkey(Key_Esc,NULL,0);
  kbput(Key_Esc);
  kbput(Key_Esc);
}


//  ------------------------------------------------------------------

static void esc_pgdn() {

  setonkey(Key_Esc,NULL,0);
  setonkey(Key_PgDn,NULL,0);
  kbput(Key_Esc);
  kbput(Key_PgDn);
}


//  ------------------------------------------------------------------

static void esc_pgup() {

  setonkey(Key_Esc,NULL,0);
  setonkey(Key_PgUp,NULL,0);
  kbput(Key_Esc);
  kbput(Key_PgUp);
}


//  ------------------------------------------------------------------

static void not_found(const char* cat) {

  wtextattr(whelp.textattr);
  wputs("\nHelp category not found:  ");
  wputs(cat);
  wputs("\nPress a key to continue.");
  waitkey();
}


//  ------------------------------------------------------------------

static int find_cat_name(const char* cat) {

  int found=NO;

  // Reset file pointer.
  whelp.fp->fseekset(whelp.offset);

  // Check for "*I" marker.
  whelp.fp->fgets(buf, BUFSIZE);
  if(strnieql(buf,"*I",2)) {

    // Search index for help category entry.  If found,
    // then advance file pointer to specified position.
    for(;;) {
      whelp.fp->fread(&recd,sizeof(Hlpr));
      if(recd.offset==-1L)
        break;
      if(strieql(recd.category,cat)) {
        whelp.fp->fseekset(whelp.offset + recd.offset);
        found=YES;
        break;
      }
    }
  }

  // If help category was not found, display an error message.
  if(not found)
    not_found(cat);

  return found;
}


//  ------------------------------------------------------------------

static int find_cat_number(int cat) {

  int found=NO;

  // Reset file pointer.
  whelp.fp->fseekset(whelp.offset);

  // Check for "*I" marker.
  whelp.fp->fgets(buf, BUFSIZE);
  if(strnieql(buf,"*I",2)) {

    // Search index for help category entry.  If found,
    // then advance file pointer to specified position.
    for(;;) {
      whelp.fp->fread(&recd,sizeof(Hlpr));
      if(recd.offset==-1L)
        break;
      if(recd.help==cat) {
        whelp.fp->fseekset(whelp.offset + recd.offset);
        found=YES;
        break;
      }
    }
  }

  // If help category was not found, display an error message.
  if(!found) {
    sprintf(buf, "%u", cat);
    not_found(buf);
  }

  return found;
}


//  ------------------------------------------------------------------

static int find_page(long startpos, int pageofs) {

  long lastpagepos, currpos;
  int currpage = 0;
  int lines = whelp.srow;

  lastpagepos = currpos = startpos;
  whelp.fp->fseekset(startpos);

  while(currpage < pageofs) {
    whelp.fp->fgets(buf, BUFSIZE);
    if(not whelp.fp->okay()) {
      whelp.fp->fseekset(lastpagepos);
      break;
    }
    lines++;
    currpos=whelp.fp->ftell();
    if(strnieql(buf, "*E", 2)) {
      whelp.fp->fseekset(lastpagepos);
      break;
    }
    if(strnieql(buf, "*P", 2)) {
      if(lines != whelp.srow+1) {
        lastpagepos=currpos;
        currpage++;
      }
      lines = whelp.srow;
    }
    else if(lines == whelp.erow-1) {
      lastpagepos=currpos;
      currpage++;
      lines = whelp.srow;
    }
  }

  return currpage;
}


//  ------------------------------------------------------------------

static void disp_cat() {

  int page, wrow, wcol, end, menuopen, itemopen;
  long startpos;
  _menu_t *curr;
  char* p;
  char* q;
  gkey i, kbch;

  // initialize variables
  page = wrow = wcol = end = menuopen = itemopen = 0;

  // save current info
  startpos = whelp.fp->ftell();
  curr     = gwin.cmenu;

  // set text attribute
  wtextattr(whelp.textattr);

  for(;;) {

    // read next line from help file into buffer
    whelp.fp->fgets(buf,BUFSIZE);
    strtrim(buf);

    // if end-of-file or "*E" was found, assume end-of-category
    end = strnieql(buf,"*E",2) ? YES : NO;

    bool pagebreak = false;
    if(strnieql(buf,"*P",2)) {
      if(wrow != whelp.srow)
        pagebreak = true;
      else
        continue;
    }

    // if end-of-category or new-page specified
    if((wrow > whelp.erow-1) or pagebreak or end) {

      loop:               ////////////////////////////////

      const char* _help = " Help ";
      char _title[80];
      sprintf(_title, " %s ", recd.category);
      wtitle(NULL, TTOP, whelp.winattr);
      wtitle(_title, TLEFT, whelp.winattr);
      wtitle(NULL, TCENTER|TBOTTOM, whelp.winattr);
      wmessage(_help, TP_BORD, whelp.ecol-whelp.scol-strlen(_help), whelp.winattr);

      // update bottom border of window with PgUp/PgDn info
      if(page and not end)      p = " PgUp/PgDn ";
      else if(not page and end) p = "";
      else if(page and end)     p = " PgUp ";
      else                      p = " PgDn ";
      wmessage(p,BT_BORD,whelp.ecol-whelp.scol-strlen(p),whelp.winattr);
      wmessage(" ESC ",BT_BORD,1,whelp.winattr);

      // if one or more cross reference menu items were specified
      if (arraycnt)
      {
        // define the PgUp/PgDn/Esc keys
        setonkey(Key_PgUp,esc_pgup,0);
        setonkey(Key_PgDn,esc_pgdn,0);
        setonkey(Key_Esc,esc_esc,0);

        // end the menu and process it
        wmenuend(BASETAGID,M_OMNI|M_NOQS,0,0,whelp.selattr,whelp.selattr,0,whelp.barattr);
        gmnudropthrough = YES;
        kbch = i = (gkey)wmenuget();
        gmnudropthrough = NO;

        // free the keys that were defined
        setonkey(Key_PgUp,NULL,0);
        setonkey(Key_PgDn,NULL,0);
        setonkey(Key_Esc,NULL,0);

        // turn off the menuopen flag and restore menu info
        menuopen=NO;
        gwin.cmenu=curr;

        // if Esc, PgDn, or PgUp was not pressed
        if(i != 0xFFFF) {

          // convert tagid to array subscript
          i -= (gkey)BASETAGID;

          // try to find selected category, if found set
          // file pointer to it, otherwise reset file
          // pointer back to previous help category
          if(find_cat_name(catarray[i]))
            startpos=whelp.fp->ftell();
          else
            whelp.fp->fseekset(startpos);

          // clear help window and set
          // position to upper left corner
          wclear();
          wrow=page=0;

          // free menu item strings
          for(i=0;i<arraycnt;i++)
            throw_release(catarray[i]);
          arraycnt=0;
          continue;
        }

        // free menu item strings
        for(i=0;i<arraycnt;i++)
          throw_release(catarray[i]);
        arraycnt=0;
        if(kbmhit()) {
          gkbd.inmenu = true;
          kbch = getxch();
          gkbd.inmenu = false;
        }
      }
      else {

        gkbd.inmenu = true;
        kbch = getxch();
        gkbd.inmenu = false;
      }

      switch(kbch) {

        case Key_PgUp:

          // find previous page, clear help window,
          // and set position to upper left corner
          page = find_page(startpos, page + (page ? -1 : 0));

          wrow=0;
          wclear();
          kbclear();
          continue;

        case Key_PgDn:

          // find next page, clear help window, and
          // set position to upper left corner
          page = find_page(startpos, page + (end ? 0 : 1));

          wrow=0;
          wclear();
          kbclear();
          continue;

        case -1:        // Aborted links
        case Key_Esc:

          // clear any PgUp or PgDn keystrokes that
          // may be lurking in CXL's keystroke
          // buffer, and return to caller.
          kbclear();
          return;

        default:
          if(kbch) {
            kbclear();
            kbput(kbch);
            return;
          }
      }

      // a goto statement is used here so the continue
      // statement above falls out to the right loop
      goto loop;
    }

    // search for cross reference categories on current line.
    // if any are found, then define them as a menu item
    q = buf;
    do {

      // search for 1st carat. if found, test for double carat.
      // if double carat was found display literal carat. next,
      // search for 2nd carat.  if not found, then display the
      // line as text.  otherwise, if found, define everything
      // in between the 2 carats as a menu item.

      p = strchr(q,'^');
      if(p) {
        if(*(p+1)=='^')  {
          *(p+1) = NUL;
          wprints(wrow, wcol, gwin.active->attr, q);
          wcol += strlen(q);
          q = p + 2;
        }
        else {
          *p = NUL;
          itemopen ^= 1;
          wprints(wrow, wcol, gwin.active->attr, q);
          if((not itemopen) and (arraycnt<MAXXREF)) {
            if((catarray[arraycnt]=(char*)throw_malloc(strlen(q)+1))!=NULL) {
              strcpy(catarray[arraycnt],q);
              if(not menuopen) {
                wmenubegc();
                menuopen=YES;
              }
              wmenuitem(
                wrow,
                wcol,
                catarray[arraycnt],
                *catarray[arraycnt],
                BASETAGID+arraycnt,
                0,
                NULL,
                0,
                0
              );
              arraycnt++;
            }
          }
          wcol += strlen(q);
          q = p + 1;
        }
      }
    } while(p);

    // display text line and increment current window row
    wprints(wrow, wcol, gwin.active->attr, q);
    wrow++;
    wcol=0;
  }
}


//  ------------------------------------------------------------------

static void help_handler() {

  register int i;
  int help,cat;
  int found;
  KBnd* savekb;

  // save help category
  help=gwin.help;

  // temporarily un-define the onkey list to avoid collision
  savekb=chgonkey(NULL);

  // hide cursor
  int cursorwashidden = vcurhidden();
  vcurhide();

  // hide mouse cursor
  #ifdef GOLD_MOUSE
  gmou.HideCursor();
  #endif

  // open help window
  if(not wopen(whelp.srow, whelp.scol, whelp.erow, whelp.ecol, whelp.btype, whelp.winattr, whelp.winattr))
    return;
    
  // display window title if specified
  if(whelp.title)
    wtitle("[ Help ]",TCENTER,whelp.winattr);

  // see if an open function was given.  If so, then call it
  if(whelp.open)
    (*whelp.open)();

  // determine help category to use.  If the current help category
  // is empty, check the stack for a help category there
  cat=help;
  if(not cat) {
    if(whelp.helpptr>-1) {
      for(i=whelp.helpptr;i>=0;i--) {
        if(whelp.help[i]) {
          cat=whelp.help[i];
          break;
        }
      }
    }
  }

  // check for no defined help category
  if(not cat) {
    wtextattr(whelp.textattr);
    wputs("\nNo help category defined.\nPress a key to continue.");
    waitkey();
  }
  else {

    if(not whelp.fp) {
      whelpclosefile = true;
      whelp.fp = new gfile;  throw_new(whelp.fp);
      whelp.fp->fopen(whelp.file,"rb");
      if(not whelp.fp->isopen()) {
        wtextattr(whelp.textattr);
        wputs("\nHelp file not found:  ");
        wputs(whelp.file);
        wputs("\nPress a key to continue.");
        waitkey();
      }
    }

    if(whelp.fp->isopen()) {

      whelp.fp->fseekset(whelp.offset);

      // find help category in help file
      found=find_cat_number(cat);

      // read and display help category text
      if(found)
        disp_cat();
    }

    if(whelpclosefile) {
      whelp.fp->fclose();
      delete whelp.fp;
      whelp.fp = NULL;
    }
  }

  // close help window
  wclose();

  // reset mouse info
  #ifdef GOLD_MOUSE
  if(gmou.FreeCursor())
    gmou.ShowCursor();
  #endif

  // reset cursor
  if(not cursorwashidden)
    vcurshow();

  // clear any onkeys defined in "open" function and relink the onkey list
  freonkey();
  chgonkey(savekb);

  // restore help category
  gwin.help=help;
}


//  ------------------------------------------------------------------

int whelpdef(const char* file, gkey key, int winattr, int textattr, int selattr, int barattr, VfvCP open) {

  // is help disengagement requested?  If so, un-define the help key.
  if(file==NULL) {
    if(gwin.helptr==NULL)
      return gwin.werrno=W_NOHLPDEF;
    else {
      gwin.helptr=NULL;
      whelpclr();
      setonkey(whelp.key,NULL,0);
    }
  }
  else {

    // attach help key to help handler
    if(setonkey(key,help_handler,0))
      return gwin.werrno=W_ALLOCERR;

    // point global help info pointer to internal struct
    gwin.helptr=&whelp;
  }

  // add information to window help information record
  whelp.file     = file;
  whelp.key      = key;
  whelp.winattr  = winattr;
  whelp.textattr = textattr;
  whelp.selattr  = selattr;
  whelp.barattr  = barattr;
  whelp.open     = open;

  // return with no error
  return gwin.werrno=W_NOERROR;
}


//  ------------------------------------------------------------------

int whelpwin(int srow, int scol, int erow, int ecol, int btype, int title) {

  // make sure help has been defined
  if(gwin.helptr==NULL)
    return gwin.werrno=W_NOHLPDEF;

  // load given coordinates into local static structure
  whelp.srow =srow;
  whelp.scol =scol;
  whelp.erow =erow;
  whelp.ecol =ecol;
  whelp.btype=btype;
  whelp.title=title;

  // return normally
  return gwin.werrno=W_NOERROR;
}


//  ------------------------------------------------------------------

int whelpcat(int cat) {

  // make sure help record has been defined
  if(gwin.helptr==NULL)
    return gwin.werrno=W_NOHLPDEF;

  // determine if help is to be global or window
  if(gwin.active)
    gwin.active->help=cat;
  gwin.help=cat;

  // return normally
  return gwin.werrno=W_NOERROR;
}


//  ------------------------------------------------------------------

int whelppcat(int cat) {

  // check for existance of help, and push
  // current help category onto help stack
  if(whelpush())
    return gwin.werrno;

  // set help category
  return whelpcat(cat);
}


//  ------------------------------------------------------------------

int whelpopc() {

  // make sure help has been defined
  if(gwin.helptr==NULL)
    return gwin.werrno=W_NOHLPDEF;

  // check for stack underflow
  if(whelp.helpptr==-1)
    return gwin.werrno=W_HLPSTKUN;

  // decrement stack pointer
  whelp.helpptr--;

  // return normally
  return gwin.werrno=W_NOERROR;
}


//  ------------------------------------------------------------------

int whelpclr() {

  // make sure help has been defined
  if(gwin.helptr==NULL)
    return gwin.werrno=W_NOHLPDEF;

  // clear help category and reset help stack pointer
  gwin.help=0;
  whelp.helpptr=-1;

  // return normally
  return(gwin.werrno=W_NOERROR);
}


//  ------------------------------------------------------------------

int whelpush() {

  // make sure help has been defined
  if(gwin.helptr==NULL)
    return(gwin.werrno=W_NOHLPDEF);

  // push the current help category onto the help stack
  return(whelpushc(gwin.help));
}


//  ------------------------------------------------------------------

int whelpushc(int cat) {

  // make sure help has been defined
  if(gwin.helptr==NULL)
    return(gwin.werrno=W_NOHLPDEF);

  // check for stack overflow
  if(gwin.helptr->helpptr==19)
    return(gwin.werrno=W_HLPSTKOV);

  // add help category to stack and increment stack pointer
  gwin.helptr->help[++gwin.helptr->helpptr]=cat;

  // return normally
  return(gwin.werrno=W_NOERROR);
}


//  ------------------------------------------------------------------

int whelpop() {

  // make sure help has been defined
  if(gwin.helptr==NULL)
    return(gwin.werrno=W_NOHLPDEF);

  // check for stack underflow
  if(gwin.helptr->helpptr==-1)
    return(gwin.werrno=W_HLPSTKUN);

  // restore help category and decrement stack pointer
  gwin.help=gwin.helptr->help[gwin.helptr->helpptr--];
  if(gwin.active)
    gwin.active->help=gwin.help;

  // return normally
  return(gwin.werrno=W_NOERROR);
}


//  ------------------------------------------------------------------
