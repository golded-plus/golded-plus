//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999-2000 Alexander S. Aganichev
//  Copyright (C) 2000 Jacobo Tarrio
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
//  GCUI: Golded+ Character-oriented User Interface.
//  Windowing kernel.
//  Based on CXL by Mike Smedley.
//  ------------------------------------------------------------------

#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <gmemdbg.h>
#include <gutlmisc.h>
#include <gwinall.h>
#include <gkbdcode.h>


//  ------------------------------------------------------------------
//  Local optimizations

#define GOLD_INLINE inline
#define GOLD_WCHK


//  ------------------------------------------------------------------

static GOLD_INLINE int _wchkrow(int wrow) {

  return ((wrow<0) or (wrow>((gwin.active->erow-gwin.active->border)-(gwin.active->srow+gwin.active->border))));
}

static GOLD_INLINE int _wchkcol(int wcol) {

  return ((wcol<0) or (wcol>((gwin.active->ecol-gwin.active->border)-(gwin.active->scol+gwin.active->border))));
}

static GOLD_INLINE int _wchkcoord(int wrow, int wcol) {

  return (_wchkrow(wrow) or _wchkcol(wcol));
}


//  ------------------------------------------------------------------
//  Checks validity of given window row coordinate

int wchkrow(int wrow) {

  return _wchkrow(wrow);
}


//  ------------------------------------------------------------------
//  Checks validity of given window column coordinate

int wchkcol(int wcol) {

  return _wchkcol(wcol);
}


//  ------------------------------------------------------------------
//  Checks validity of given window coordinates

int wchkcoord(int wrow, int wcol) {

  return _wchkcoord(wrow, wcol);
}


//  ------------------------------------------------------------------
//  Checks for valid window box coordinates

int wchkbox(int wsrow, int wscol, int werow, int wecol) {

  return (_wchkcoord(wsrow,wscol) or _wchkcoord(werow,wecol) or (wsrow>werow) or (wscol>wecol));
}


//  ------------------------------------------------------------------
//  Sets window cursor coordinates

int wgotoxy(int wrow, int wcol) {

  // check for valid cursor coordinates
  #ifdef GOLD_WCHK
  if(wchkcoord(wrow,wcol))
    return gwin.werrno=W_INVCOORD;
  #endif

  // calculate effective cursor coordinates and update window record
  const int &row = gwin.active->row    = gwin.active->srow + wrow + gwin.active->border;
  const int &col = gwin.active->column = gwin.active->scol + wcol + gwin.active->border;

  // set cursor location
  vposset(row,col);

  // return with no error
  return gwin.werrno=W_NOERROR;
}


//  ------------------------------------------------------------------
//  Opens a window and makes it active

int wopen(int srow, int scol, int erow, int ecol, int btype, vattr battr, vattr wattr, vattr sbattr, vattr loattr) {

  // check for valid box type
  if(btype<0 or btype>7) {
    gwin.werrno=W_INVBTYPE;
    return 0;
  }

  if (ecol >= gvid->numcols)
    ecol = gvid->numcols - 1;

  // see if window is to have a border
  int border = (btype==5) ? NO : YES;

  // check for valid coordinates
  if(srow>(erow-border) or scol>(ecol-border)) {
    gwin.werrno=W_INVCOORD;
    return 0;
  }

  // allocate memory for new record
  _wrec_t* wrec = (_wrec_t*)throw_xmalloc(sizeof(_wrec_t));
  if(wrec==NULL) {
    gwin.werrno=W_ALLOCERR;
    return 0;
  }

  // save affected area of screen
  vsavebuf* wbuf = vsave(srow,scol,erow,ecol);
  if(wbuf==NULL) {
    throw_xrelease(wrec);
    gwin.werrno=W_ALLOCERR;
    return 0;
  }

  // add new record to linked list
  if(gwin.active!=NULL)
    gwin.active->next=wrec;
  wrec->prev=gwin.active;
  wrec->next=NULL;
  gwin.active=wrec;

  // draw and fill text box on screen
  switch(gwin.style) {
    case STYLE_NORMAL:
      if(border)
        vbox(srow,scol,erow,ecol,btype,battr,loattr);
      vfill(srow+border,scol+border,erow-border,ecol-border,gwin.fillch,wattr);
      break;
  }

  // increment window handle counter
  gwin.handle++;

  // save window info in window record
  gwin.active->wbuf     = wbuf;
  gwin.active->whandle  = gwin.handle;
  gwin.active->srow     = srow;
  gwin.active->scol     = scol;
  gwin.active->erow     = erow;
  gwin.active->ecol     = ecol;
  gwin.active->btype    = btype;
  gwin.active->wattr    = wattr;
  gwin.active->battr    = battr;
  gwin.active->loattr   = loattr;
  gwin.active->sbattr   = sbattr;
  gwin.active->border   = border;
  gwin.active->row      = srow+border;
  gwin.active->column   = scol+border;
  gwin.active->attr     = wattr;
  gwin.active->title    = NULL;
  gwin.active->tpos     = 0;
  gwin.active->help     = 0;
  gwin.active->form     = NULL;
  gwin.active->wsbuf    = NULL;

  // increment total number of open windows
  gwin.total++;

  // initialize cursor location to window row 0 column 0
  wgotoxy(0,0);

  // return normally
  gwin.werrno=W_NOERROR;
  return gwin.handle;
}


//  ------------------------------------------------------------------
//  Closes a window

int wclose() {

  // check for active window
  if(!gwin.total or !gwin.active)
    return(gwin.werrno=W_NOACTIVE);

  // if window has a shadow, close shadow first
  if(gwin.active->wsbuf!=NULL)
    wshadoff();

  // restore contents of and free memory held by window
  vrestore(gwin.active->wbuf);
  throw_xrelease(gwin.active->wbuf);

  // decrement total number of open windows
  gwin.total--;

  // free memory held by window's record and update linked list
  _wrec_t *wrec = gwin.active->prev;
  throw_xrelease(gwin.active);
  gwin.active=wrec;
  if(gwin.active!=NULL)
    gwin.active->next=NULL;

  // update cursor location and help category
  if(gwin.active!=NULL) {
    vposset(gwin.active->row,gwin.active->column);
    if(gwin.active->help)
      gwin.help=gwin.active->help;
  }

  // return normally
  return gwin.werrno=W_NOERROR;
}


//  ------------------------------------------------------------------
//  Closes all open windows

int wcloseall() {

  if(!gwin.total)
    return gwin.werrno=W_NOACTIVE;

  while(gwin.total)
    if(wclose())
      return gwin.werrno;

  // close hidden windows too
  _wrec_t* prev;
  while(gwin.hidden!=NULL) {
    prev = gwin.hidden->prev;
    throw_xfree(gwin.hidden->wbuf);
    throw_xfree(gwin.hidden);
    gwin.hidden = prev;
  }

  return gwin.werrno=W_NOERROR;
}


//  ------------------------------------------------------------------
//  Gives active window a shadow

int wshadow(vattr attr) {

  // check for active window
  if(!gwin.total)
    return gwin.werrno=W_NOACTIVE;

  // see if window already has a shadow
  if(gwin.active->wsbuf!=NULL)
    return gwin.werrno=W_NOERROR;

  // get window coordinates from the window's record
  const int &srow = gwin.active->srow;
  const int &scol = gwin.active->scol;
  const int &erow = gwin.active->erow;
  const int &ecol = gwin.active->ecol;

  // allocate buffer to hold shadow's contents
  vatch* wsbuf = (vatch*)throw_xmalloc((((erow-srow)*sizeof(vatch))+ecol-scol+1)*sizeof(vatch));
  
  if(wsbuf == NULL)
    return gwin.werrno=W_ALLOCERR;

  // start at upper right corner of shadow and work down
  int crow = srow+1;
  int ccol = ecol+1;
  vatch* q = wsbuf;

  // draw shadow to right of window
  while(crow<=erow) {

    // read current screen characters/attributes and save in shadow's buffer
    vatch tmp[2];
    *q = vgetw(crow, ccol);
#if defined(__USE_NCURSES__) || !defined(__UNIX__)
    tmp[0] = vsattr(*q, attr);
#else
    tmp[0] = vsattr(' ', attr);
#endif
    q++;
    *q = vgetw(crow, ccol + 1);
#if defined(__USE_NCURSES__) || !defined(__UNIX__)
    tmp[1] = vsattr(*q, attr);
#else
    tmp[1] = vsattr(' ', attr);
#endif
    q++;

    // write characters back to screen using shadow's attribute
    vputws(crow++, ccol, tmp, 2);
  }

  // start at lower left corner of shadow and work right
  crow = erow+1;
  ccol = scol+2;
  int stop = ecol+2;
  int len = stop - ccol + 1;
  vatch* wptr = (vatch*)gvid->bufwrd;

  // draw bottom shadow
  while(ccol<=stop) {

    // read attribs/chars and store in buffers
    *q = vgetw(crow, ccol++);
#if defined(__USE_NCURSES__) || !defined(__UNIX__)
    *wptr++ = vsattr(*q, attr);
#else
    *wptr++ = vsattr(' ', attr);
#endif
    q++;
  }

  // display complete buffer
  vputws(crow, scol+2, gvid->bufwrd, len);

  // save info in window's record
  gwin.active->wsbuf  = wsbuf;
  gwin.active->wsattr = attr;

  // reset cursor
  vposset(gwin.active->row,gwin.active->column);

  // return with no error
  return gwin.werrno=W_NOERROR;
}


//  ------------------------------------------------------------------
//  Removes shadow from active window

int wshadoff() {

  // check for active window
  if(!gwin.total)
    return gwin.werrno=W_NOACTIVE;

  // if window doesn't have a shadow, ignore request
  if(gwin.active->wsbuf==NULL)
    return gwin.werrno=W_NOERROR;

  // get window coordinates from the window's record
  const int &srow = gwin.active->srow;
  const int &scol = gwin.active->scol;
  const int &erow = gwin.active->erow;
  const int &ecol = gwin.active->ecol;

  // start at upper right corner of shadow and work down
  int crow = srow+1;
  int ccol = ecol+1;
  vatch* q = gwin.active->wsbuf;

  // delete shadow to right of window
  while(crow<=erow) {
    vputw(crow,   ccol,   *q++);
    vputw(crow++, ccol+1, *q++);
  }

  // start at lower left corner of shadow and work right
  crow = erow+1;
  ccol = scol+2;
  int stop = ecol+2;

  // delete bottom shadow
  while(ccol<=stop)
    vputw(crow,ccol++,*q++);

  // free memory held by shadow
  throw_xrelease(gwin.active->wsbuf);

  // update window's record
  gwin.active->wsattr = WHITE_|_WHITE;

  // return with no error
  return gwin.werrno=W_NOERROR;
}


//  ------------------------------------------------------------------
//  Scrolls the active window up or down

int wscroll(int count, int direction) {

  // check for window border
  const int &border = gwin.active->border;

  vscroll(
    gwin.active->srow + border,
    gwin.active->scol + border,
    gwin.active->erow - border,
    gwin.active->ecol - ((border or (gwin.active->sbattr != DEFATTR)) ? 1 : 0),
    gwin.active->wattr,
    direction == SUP ? count : -count
  );

  // return with no error
  return gwin.werrno = W_NOERROR;
}


//  ------------------------------------------------------------------
//  Scrolls a region of the active window up or down

int wscrollbox(int wsrow, int wscol, int werow, int wecol, int count, int direction) {

  // check for window border
  const int &border = gwin.active->border;

  vscroll(
    gwin.active->srow+wsrow+border,
    gwin.active->scol+wscol+border,
    gwin.active->srow+werow+border,
    gwin.active->scol+wecol+border,
    gwin.active->wattr,
    direction == SUP ? count : -count
  );

  // return with no error
  return gwin.werrno=W_NOERROR;
}


//  ------------------------------------------------------------------
//  Displays a character inside active window

int wputc(char ch) {

  int cwcol;

  // check for active window
  if(!gwin.total)
    return gwin.werrno=W_NOACTIVE;

  // get coordinates from window's record
  int crow = gwin.active->row;
  int ccol = gwin.active->column;
  const int &scol = gwin.active->scol;
  const int &border = gwin.active->border;

  // test the input character for control characters
  switch(ch) {
    case '\n':
      crow++;
    case '\r':
      ccol=scol+border;
      break;
    case '\b':
      if(ccol==(scol+border)) {
        ccol=gwin.active->ecol-border;
        crow--;
        if(crow<(gwin.active->srow+border))
          crow++;
      }
      else {
        ccol--;
      }
      break;
    case '\t':
      cwcol=ccol-border-scol;
      ccol+=(tabstop(cwcol,gwin.tabwidth)-cwcol);
      break;
    default:
      vputc(crow, ccol++, gwin.active->attr, ch);
  }

  // see if wrap-around is needed
  if(ccol > (gwin.active->ecol-border)) {
    ccol = scol+border;
    crow++;
  }

  // see if scroll is needed
  if(crow > (gwin.active->erow-border)) {
    wscroll(1,SUP);
    crow--;
  }

  // update window's record
  gwin.active->row=crow;
  gwin.active->column=ccol;

  // reset cursor position
  vposset(crow,ccol);

  // return normally
  return gwin.werrno=W_NOERROR;
}


//  ------------------------------------------------------------------
//  Reads current cursor location inside window

int wreadcur(int* wrow, int* wcol) {

  // check for active window
  if(!gwin.total)
    return gwin.werrno=W_NOACTIVE;

  // read effective cursor coordinates
  int row,col;
  vposget(&row,&col);

  // calculate window cursor coordinates
  const int &border = gwin.active->border;
  *wrow = row - gwin.active->srow - border;
  *wcol = col - gwin.active->scol - border;

  // return normally
  return gwin.werrno=W_NOERROR;
}


//  ------------------------------------------------------------------
//  Display a character specified number of times

int wdupc(char ch, int count) {

  // check for active window
  if(!gwin.total)
    return(gwin.werrno=W_NOACTIVE);

  // display ch for count times
  while(count--)
    wputc(ch);

  // return with gwin.werrno set by wputc()
  return(gwin.werrno);
}


//  ------------------------------------------------------------------
//  Clears the active window in specified attribute

int wcclear(vattr attr) {

  // check for active window

  if(!gwin.total)
    return gwin.werrno=W_NOACTIVE;

  // check for window border

  const int &border = gwin.active->border;

  vfill(
    gwin.active->srow+border,
    gwin.active->scol+border,
    gwin.active->erow-border,
    gwin.active->ecol-border,
    gwin.fillch,
    attr
  );

  // home the cursor

  wgotoxy(0,0);

  return gwin.werrno=W_NOERROR;
}


//  ------------------------------------------------------------------
//  Clears from cursor postion to end of window's line

int wclreol() {

  // check for active window
  if(!gwin.total)
    return gwin.werrno=W_NOACTIVE;

  // clear to end of window's line
  const int &column = gwin.active->column;
  vputx(
    gwin.active->row,
    column,
    gwin.active->attr,
    gwin.fillch,
    gwin.active->ecol - gwin.active->border - column + 1
  );

  // return normally
  return gwin.werrno=W_NOERROR;
}


//  ------------------------------------------------------------------
//  Clears from cursor postion to end of window

int wclreos() {

  int wrow, werow, wr, wc;

  // check for active window
  if(!gwin.total)
    return gwin.werrno=W_NOACTIVE;

  // save current window row and column
  wreadcur(&wr, &wc);

  wrow = wr;
  werow = gwin.active->erow - gwin.active->srow - gwin.active->border;
  wclreol();
  wrow++;

  while(wrow <= werow) {
    wgotoxy(wrow,0);
    wclreol();
    wrow++;
  }

  // restore window row and column
  wgotoxy(wr,wc);

  // return normally
  return gwin.werrno=W_NOERROR;
}


//  ------------------------------------------------------------------
//  This function will process an Escape sequence when encountered

static const char* process_esc(const char* str)
{
  int wrow,wcol;

  const char *p = str;
  for(; *p==ESC; p++) {

    vattr attr = gwin.active->attr;

    switch(*(++p)) {

      case '+':   // increase text attribute
        wtextattr(++attr);
        break;

      case '-':   // decrease text attribute
        wtextattr(--attr);
        break;

      case 'A':   // change attribute
        wtextattr(*++p);
        break;

      case 'F':   // change foreground attribute
        wtextattr((int)((*++p & 0x07) | (attr & ~0x07)));
        break;

      case 'B':   // change background attribute
        wtextattr((int)((*++p & 0x70) | (attr & ~0x70)));
        break;

      case 'I':   // toggle intensity bit
        wtextattr((int)(attr ^ INTENSE));
        break;

      case 'L':   // toggle blinking bit
        wtextattr((int)(attr ^ BLINK));
        break;

      case 'X':   // reverse attribute
        wtextattr(revsattr(attr));
        break;

      case 'R':   // set cursor row
        wreadcur(&wrow,&wcol);
        wgotoxy(*++p,wcol);
        break;

      case 'C':   // set cursor column
        wreadcur(&wrow,&wcol);
        wgotoxy(wrow,*++p);
        break;

      case 'E':   // erase
        switch(*++p) {
          case 'W':   // erase window
            wclear();
            break;
          case 'S':   // erase to end of window
            wclreos();
            break;
          case 'L':   // erase to end of window's line
            wclreol();
            break;
        }
        break;

      case 'D':   // duplicate character
        {
          char ch = *++p;
          wdupc(ch,*++p);
        }
        break;

      default:
        p--;
    }
  }

  return --p;
}


//  ------------------------------------------------------------------
//  Displays a string inside active window

int wputs(const char* str) {

  int cwcol;
  const char* q;

  // get effective coordinates from window's record
  int &crow = gwin.active->row;
  int &ccol = gwin.active->column;
  const int &scol = gwin.active->scol;
  const int &border = gwin.active->border;

  // do while not end of string
  for(q=str; *q; q++) {

    // test the input character for control characters
    switch(*q) {
      case '\n':
        crow++;
      case '\r':
        ccol=scol+border;
        break;
      case '\b':
        if(ccol==(scol+border)) {
          ccol=gwin.active->ecol-border;
          crow--;
          if(crow<(gwin.active->srow+border))
            crow++;
        }
        else {
          ccol--;
        }
        break;
      case '\t':
        cwcol=ccol-border-scol;
        ccol+=(tabstop(cwcol,gwin.tabwidth)-cwcol);
        break;
      case ESC:
        q=process_esc(q);
        break;
      default:
        vputc(crow, ccol++, gwin.active->attr, *q);
    }

    // see if wrap-around is needed
    if(ccol > (gwin.active->ecol-border)) {
      ccol=scol+border;
      crow++;
    }

    // see if scroll is needed
    if(crow > (gwin.active->erow-border)) {
      wscroll(1,SUP);
      crow--;
    }
  }

  // reset cursor position
  vposset(crow,ccol);

  // return normally
  return(gwin.werrno=W_NOERROR);
}


//  ------------------------------------------------------------------
//  Displays a character inside active window

int wprintc(int wrow, int wcol, vattr atr, vchar chr) {

  // check for active window
  if(!gwin.total)
    return gwin.werrno=W_NOACTIVE;

  // check for valid coordinates
  #ifdef GOLD_WCHK
  if(wchkcoord(wrow,wcol))
    return gwin.werrno=W_INVCOORD;
  #endif

  vputc(wrow+gwin.active->srow+gwin.active->border, wcol+gwin.active->scol+gwin.active->border, atr, chr);

  // return normally
  return gwin.werrno=W_NOERROR;
}


//  ------------------------------------------------------------------
//  Outputs a formatted string to active window

int wprintf(const char* format, ...) {

  va_list argptr;
  char buf[255];

  // format string using specified parameters into buffer
  va_start(argptr,format);            // access argument list
  int result = vsprintf(buf,format,argptr);        // create string using argument list
  va_end(argptr);                     // end access of argument list

  // display the created string
  wputs(buf);

  return result;
}


//  ------------------------------------------------------------------
//  Print a formatted string at a specific position and attribute

int wprintfs(int wrow, int wcol, vattr attr, const char* format, ...) {

  va_list argptr;
  char buf[2560];

  *buf = NUL;
  va_start(argptr, format);
  int result = vsprintf(buf, format, argptr);
  va_end(argptr);

  wprints(wrow, wcol, attr, buf);

  return result;
}


//  ------------------------------------------------------------------
//  Displays a string inside active window

int wprints(int wrow, int wcol, vattr attr, const char* str) {

  // check for active window
  if(!gwin.total)
    return gwin.werrno=W_NOACTIVE;

  // check for valid coordinates
  #ifdef GOLD_WCHK
  if(wchkcoord(wrow,wcol))
    return gwin.werrno=W_INVCOORD;
  #endif

  const int &border = gwin.active->border;
  vputs(gwin.active->srow+wrow+border,gwin.active->scol+wcol+border,attr,str);
  return gwin.werrno=W_NOERROR;
}

int wprints_box(int wrow, int wcol, vattr attr, const char* str) {

  // check for active window
  if(!gwin.total)
    return gwin.werrno=W_NOACTIVE;

  // check for valid coordinates
  #ifdef GOLD_WCHK
  if(wchkcoord(wrow,wcol))
    return gwin.werrno=W_INVCOORD;
  #endif

  const int &border = gwin.active->border;
  vputs_box(gwin.active->srow+wrow+border,gwin.active->scol+wcol+border,attr,str);
  return gwin.werrno=W_NOERROR;
}


//  ------------------------------------------------------------------
//  Displays a string inside active window

int wprintvs(int wrow, int wcol, vattr attr, const vchar* str) {

  // check for active window
  if(!gwin.total)
    return gwin.werrno=W_NOACTIVE;

  // check for valid coordinates
  #ifdef GOLD_WCHK
  if(wchkcoord(wrow,wcol))
    return gwin.werrno=W_INVCOORD;
  #endif

  const int &border = gwin.active->border;
  vputvs(gwin.active->srow+wrow+border,gwin.active->scol+wcol+border,attr,str);
  return gwin.werrno=W_NOERROR;
}


//  ------------------------------------------------------------------

int wputx(int wrow, int wcol, vattr attr, vchar chr, uint len) {

  const int &border = gwin.active->border;
  vputx(gwin.active->srow+wrow+border,gwin.active->scol+wcol+border,attr,chr,len);
  return gwin.werrno=W_NOERROR;
}


//  ------------------------------------------------------------------

int wputy(int wrow, int wcol, vattr attr, vchar chr, uint len) {

  const int &border = gwin.active->border;
  vputy(gwin.active->srow+wrow+border,gwin.active->scol+wcol+border,attr,chr,len);
  return gwin.werrno=W_NOERROR;
}


//  ------------------------------------------------------------------
//  Displays a string inside active window

int wprintns(int wrow, int wcol, vattr attr,  const std::string &str, uint len, vchar fill, vattr fill_attr)
{
  char* istr = throw_xstrdup(str.c_str());
  char* ostr = istr;
  char och = *ostr;
  uint olen = strlen(istr);
  if(len < olen) {
    ostr += len;
    och = *ostr;
    *ostr = NUL;
  }
  int retval = wprints(wrow, wcol, attr, istr);
  if(len < olen)
    *ostr = och;
  else if(len > olen)
    retval = wputx(wrow, wcol+olen, (fill_attr != DEFATTR) ? fill_attr : attr, fill, len-olen);
  throw_xfree(istr);
  return retval;
}


//  ------------------------------------------------------------------
//  Displays attrib/char buffer inside active window

int wprintws(int wrow, int wcol, vatch* buf, uint len) {

  // check for active window
  if(!gwin.total)
    return gwin.werrno=W_NOACTIVE;

  // check for valid coordinates
  #ifdef GOLD_WCHK
  if(wchkcoord(wrow,wcol))
    return gwin.werrno=W_INVCOORD;
  #endif

  // see if window has border
  const int &border = gwin.active->border;

  // calculate effective coordinates
  int row = gwin.active->srow+wrow+border;
  int col = gwin.active->scol+wcol+border;

  // display buffer
  vputws(row, col, buf, len);

  return gwin.werrno=W_NOERROR;
}


//  ------------------------------------------------------------------
//  Returns address of record of given window handle

_wrec_t* wfindrec(int whandle) {

  _wrec_t *wrec;

  // scan through linked list for record belonging to requested handle

  wrec = gwin.active;
  while(wrec) {
    if(whandle==wrec->whandle)
      break;
    wrec=wrec->prev;
  }

  if(wrec == NULL) {

    // Search through the hidden windows

    wrec = gwin.hidden;
    while(wrec) {
      if(whandle==wrec->whandle)
        break;
      wrec=wrec->prev;
    }
  }

  // return address of found record
  return wrec;
}


//  ------------------------------------------------------------------
//  Hides active window

int whide() {

  vsavebuf  *p;
  vattr     shattr;
  _wrec_t   *temp;

  // check for active window
  if(!gwin.total)
    return(gwin.werrno=W_NOACTIVE);

  // save active window
  p = vsave(gwin.active->srow,gwin.active->scol,gwin.active->erow,gwin.active->ecol);

  // check for/close window's shadow
  if(gwin.active->wsbuf!=NULL) {
    shattr = gwin.active->wsattr;
    wshadoff();
    gwin.active->wsattr = shattr;
  }
  else {
    gwin.active->wsattr = DEFATTR;
  }

  // restore contents of active window's buffer
  vrestore(gwin.active->wbuf);
  throw_xfree(gwin.active->wbuf);
  gwin.active->wbuf = p;

  // update visible window record linked list
  temp = gwin.active;
  gwin.active = gwin.active->prev;
  if(gwin.active)
    gwin.active->next = NULL;
  gwin.total--;

  // update hidden window record linked list
  if(gwin.hidden)
    gwin.hidden->next = temp;
  temp->prev = gwin.hidden;
  temp->next = NULL;
  gwin.hidden = temp;

  // update cursor location and help category
  if(gwin.active) {
    vposset(gwin.active->row,gwin.active->column);
    if(gwin.active->help)
      gwin.help = gwin.active->help;
  }

  // return normally
  return(gwin.werrno=W_NOERROR);
}


//  ------------------------------------------------------------------
//  Unhides a previously hidden window

int wunhide(int whandle) {

  vsavebuf* p;
  _wrec_t* found;

  // check pointer to hidden window linked list ; must not be NULL
  if(gwin.hidden==NULL)
    return gwin.werrno=W_NOHIDDEN;

  // check to see if input window handle == 0.  if so, then
  // that means to unhide the most recently hidden window.
  if(!whandle)
    whandle=gwin.hidden->whandle;

  // scan through linked list for record belonging to requested handle
  found=gwin.hidden;
  while(found!=NULL) {
    if(whandle==found->whandle)
      break;
    found=found->prev;
  }

  // was handle found in hidden window record linked list?
  if(found==NULL) {

    // see if handle is in visible window record linked list
    if(wfindrec(whandle)==NULL)
      return gwin.werrno=W_NOTFOUND;
    else
      return gwin.werrno=W_NOTHIDD;
  }

  // save area of screen where window is to unhide at
  if((p=vsave(found->srow,found->scol,found->erow,found->ecol))==NULL)
    return gwin.werrno=W_ALLOCERR;

  // restore contents of hidden window back to screen
  vrestore(found->wbuf);
  throw_xfree(found->wbuf);
  found->wbuf=p;

  // update hidden window record linked list
  if(found->prev!=NULL)
    found->prev->next=found->next;
  if(found->next==NULL)
    gwin.hidden=found->prev;
  else
    found->next->prev=found->prev;

  // update visible window record linked list
  if(gwin.active!=NULL)
    gwin.active->next=found;
  found->prev=gwin.active;
  found->next=NULL;
  gwin.active=found;
  gwin.total++;

  // if window had a shadow before hiding, give it one again
  if(gwin.active->wsattr != DEFATTR)
    wshadow(gwin.active->wsattr);

  // update help category
  if(gwin.active->help)
    gwin.help=gwin.active->help;

  // reset cursor
  vposset(gwin.active->row,gwin.active->column);

  // return normally
  return gwin.werrno=W_NOERROR;
}


//  ------------------------------------------------------------------
//  Unlinks and frees a window from memory, but does not manipulate
//  the screen at all

int wunlink(int w) {

  _wrec_t *found, *prev, *next;

  // check to see if input window handle == 0.  if
  // so, then that means to unlink the active window.
  if(!w)
    w=gwin.active->whandle;

  // find address of window record for given window handle
  if((found=wfindrec(w))==NULL)
    return gwin.werrno=W_NOTFOUND;

  // free memory held by shadow's buffer (if shadow exists)
  if(found->wsbuf!=NULL)
    throw_xrelease(found->wsbuf);

  // free memory held by window's buffer
  throw_xrelease(found->wbuf);

  // decrement total number of open windows
  gwin.total--;

  // re-link list pointers around window record to remove
  prev=found->prev;
  next=found->next;
  if(prev!=NULL)
    prev->next=next;
  if(next!=NULL)
    next->prev=prev;

  // free memory held by window record
  throw_xfree(found);

  // see if active window has changed
  if(next==NULL) {
    if(prev!=NULL) {
      gwin.active=prev;
      if(gwin.active->help)
        gwin.help=gwin.active->help;
    }
  }

  // return normally
  return gwin.werrno=W_NOERROR;
}


//  ------------------------------------------------------------------
//  Local variables

static _wrec_t *__curr, *__found;
static int    __crow, __ccol;
static vattr  __gattr;
static const char* __p;


//  ------------------------------------------------------------------
//  This function dectects if given window is blocking the current
//  window or its shadow at specified coordinates

static GOLD_INLINE int window_blocking() {

  return (__crow>=__curr->srow and __crow<=__curr->erow and __ccol>=__curr->scol and __ccol<=__curr->ecol) ? YES : NO;
}


//  ------------------------------------------------------------------
//  This function detects if a given window's bottom shadow is
//  blocking the current window or its shadow at specified coordinates

static GOLD_INLINE int bshadow_blocking() {

  if(__crow==(__curr->erow+1))
    if((__ccol>=(__curr->scol+2)) and (__ccol<=(__curr->ecol+2)))
      return YES;
    else
      return NO;

  return NO;
}


//  ------------------------------------------------------------------
//  This function detects if a given window's right shadow is blocking
//  the current window or its shadow at specified coordinates

static GOLD_INLINE int rshadow_blocking() {

  if(__ccol==(__curr->ecol+1) or __ccol==(__curr->ecol+2))
    if((__crow>=(__curr->srow+1)) and (__crow<=__curr->erow))
      return YES;
    else
      return NO;

  return NO;
}


//  ------------------------------------------------------------------

static GOLD_INLINE vatch* calc_window(_wrec_t *wrec) {

  return wrec->wbuf->data+((__crow-wrec->srow)*(wrec->ecol-wrec->scol+1))+(__ccol-wrec->scol);
}


//  ------------------------------------------------------------------

static GOLD_INLINE vatch* calc_bshadow(_wrec_t *wrec) {

  return wrec->wsbuf+((((__crow-wrec->srow-1)*2)+(__ccol-wrec->scol-2)));
}


//  ------------------------------------------------------------------

static GOLD_INLINE vatch* calc_rshadow(_wrec_t *wrec) {

  return wrec->wsbuf+((((__crow-wrec->srow-1)*2)+(__ccol-wrec->ecol-1)));
}


//  ------------------------------------------------------------------
//  This function will exchange the contents of the applicable buffers

static void swap_contents(vatch* pfound, vatch* pcurr, int shadow) {

  register _wrec_t *wptr;
  register vatch temp, chat;

  // display character from current position in window to
  // activate on the screen.  if character is part of a
  // shadow, reflect the character on the screen.

  temp = vgetw(__crow, __ccol);

  if(shadow&2)
    *pcurr = vschar(*pcurr, vgchar(temp));
  chat = ((vgattr(temp) & BLINK) and shadow) ? vsattr(*pcurr, vgattr(*pcurr) | BLINK) : *pcurr;
  vputw(__crow, __ccol, chat);

  // let window position directly above position
  // to activate have the character that it holds

  *pcurr = *pfound;

  // if current character position to activate will
  // activate over a shadow in another window

  if(shadow&1) {

    // resolve all shadows upwards

    wptr = __curr;
    chat = vsattr(*pfound, __curr->wsattr);

    for(__curr=__curr->next;__curr!=NULL;__curr=__curr->next) {

      if(window_blocking()) {
        *(calc_window(__curr)) = chat;
        chat = temp;
        break;
      }
      else {
        if(bshadow_blocking())
          *(calc_bshadow(__curr)) = chat;
        else {
          if(rshadow_blocking())
            *(calc_rshadow(__curr)) = chat;
        }
      }
    }

    temp = chat;
    __curr = wptr;
  }

  // let character position activated hold character
  // that was on the screen in the same position

  *pfound = temp;
}


//  ------------------------------------------------------------------

int wactiv(int whandle) {

  register int startcol, stopcol;
  _wrec_t *prev, *next;

  // check for active window

  if(!gwin.total)
    return gwin.werrno=W_NOACTIVE;

  // if window is already active, ignore request

  if(whandle==gwin.active->whandle)
    return gwin.werrno=W_NOERROR;

  // find address of window's record

  __found=wfindrec(whandle);
  if(__found==NULL)
    return gwin.werrno=W_NOTFOUND;

  // check every character position in window to activate

  for(__crow=__found->srow;__crow<=__found->erow;__crow++) {
    for(__ccol=__found->scol;__ccol<=__found->ecol;__ccol++) {

      // check all window records "above" window to activate

      for(__curr=__found->next;__curr!=NULL;__curr=__curr->next) {

        // see if current position in window to activate
        // is blocked by same position in test window

        if(window_blocking()) {

          // calculate buffer addresses and swap contents

          swap_contents(calc_window(__found),calc_window(__curr),0);
          break;
        }

        // see if test window has a shadow

        if(__curr->wsbuf!=NULL) {

          // see if shadow to the right of test window is
          // blocking the current position of window to activate

          if(rshadow_blocking()) {
            swap_contents(calc_window(__found),calc_rshadow(__curr),1);
            break;
          }

          // see if shadow to the bottom of test window is
          // blocking the current position of window to activate

          if(bshadow_blocking()) {
            swap_contents(calc_window(__found),calc_bshadow(__curr),1);
            break;
          }
        }
      }
    }
  }

  // if window to activate has a shadow, then check
  // every character position in the shadow to see
  // if it is blocked by another window or window shadow

  if(__found->wsbuf!=NULL) {

    // search the right shadow of window to activiate

    startcol=__found->ecol+1;
    stopcol=startcol+1;
    for(__crow=__found->srow+1;__crow<=__found->erow;__crow++) {
      for(__ccol=startcol;__ccol<=stopcol;__ccol++) {

        // check all window records "above" shadow to activate

        for(__curr=__found->next;__curr!=NULL;__curr=__curr->next) {

          // see if current position in shadow to activate
          // is blocked by same position in current window

          if(window_blocking()) {

            // calculate buffer addresses and swap contents

            swap_contents(calc_rshadow(__found),calc_window(__curr),2);
            break;
          }

          // see if test window has a shadow

          if(__curr->wsbuf!=NULL) {

            // see if current position of window to activate is
            // blocked by the right shadow of the test window

            if(rshadow_blocking()) {
              swap_contents(calc_rshadow(__found),calc_rshadow(__curr),3);
              break;
            }

            // see if current position of window to activate is
            // blocked by the bottom shadow of the test window

            if(bshadow_blocking()) {
              swap_contents(calc_rshadow(__found),calc_bshadow(__curr),3);
              break;
            }
          }
        }
      }
    }

    // search bottom shadow

    startcol=__found->scol+2;
    stopcol=__found->ecol+2;
    __crow=__found->erow+1;
    for(__ccol=startcol;__ccol<=stopcol;__ccol++) {

      // check all window records "above" shadow to activate

      for(__curr=__found->next;__curr!=NULL;__curr=__curr->next) {

        // see if current position in shadow to activate
        // is blocked by same position in test window

        if(window_blocking()) {

          // calculate buffer addresses and swap contents

          swap_contents(calc_bshadow(__found),calc_window(__curr),2);
          break;
        }

        // see if test window has a shadow

        if(__curr->wsbuf!=NULL) {
          if(rshadow_blocking()) {
            swap_contents(calc_bshadow(__found),calc_rshadow(__curr),3);
            break;
          }
          if(bshadow_blocking()) {
            swap_contents(calc_bshadow(__found),calc_bshadow(__curr),3);
            break;
          }
        }
      }
    }
  }

  // re-link pointer to window record to be activated

  prev=__found->prev;
  next=__found->next;
  if(prev!=NULL)
    prev->next=next;
  next->prev=prev;
  gwin.active->next=__found;
  __found->prev=gwin.active;
  __found->next=NULL;
  gwin.active=__found;

  // update help category

  if(gwin.active->help)
    gwin.help=gwin.active->help;

  // reset cursor position

  vposset(gwin.active->row,gwin.active->column);

  // return normally

  return gwin.werrno=W_NOERROR;
}


//  ------------------------------------------------------------------
//  Activates a window without overlap checking

int wactiv_(int whandle) {

  // if window is already active, ignore request
  if(gwin.active and (whandle == gwin.active->whandle))
    return gwin.werrno = W_NOERROR;

  // find address of window's record
  __found = wfindrec(whandle);
  if(__found == NULL)
    return gwin.werrno = W_NOTFOUND;

  // re-link pointer to window record to be activated
  _wrec_t* prev = __found->prev;
  _wrec_t* next = __found->next;
  if(prev)
    prev->next = next;
  next->prev = prev;
  gwin.active->next = __found;
  __found->prev = gwin.active;
  __found->next = NULL;
  gwin.active = __found;

  // update help category
  if(gwin.active->help)
    gwin.help = gwin.active->help;

  // reset cursor position
  vposset(gwin.active->row,gwin.active->column);

  // return normally
  return gwin.werrno = W_NOERROR;
}


//  ------------------------------------------------------------------
//  this function will update buffers above window string will be displayed in

static void update_buffers(vatch* pcurr, int shadow) {

  _wrec_t *tcurr;
  vattr   tgattr;

  // put current string character and attribute into found window's buffer

  *pcurr = vcatch(*__p, __gattr);

  // if window's shadow is what's blocking, check to see
  // if it is the highest shadow.  If it is, display the
  // character in the shadow's attribute, otherwise search
  // for other blocking windows

  if(shadow) {
    if(__curr->next==NULL) {
      vputc(__crow, __ccol, __gattr & BLINK ? (__curr->wsattr | BLINK) : __curr->wsattr, *__p);
    }
    else {
      tcurr = __curr;
      __curr = __curr->next;
      tgattr = __gattr;
      __gattr = __curr->wsattr;
      if(window_blocking())
        update_buffers(calc_window(__curr), 0);
      else {
        if(bshadow_blocking())
          update_buffers(calc_bshadow(__curr), 1);
        else {
          if(rshadow_blocking())
            update_buffers(calc_rshadow(__curr), 1);
        }
      }
      __gattr = tgattr;
      __curr = tcurr;
    }
  }
}

//  ------------------------------------------------------------------

int wwprintc(int whandle, int wrow, int wcol, vattr attr, const vchar chr) {

  // check for existance of active window or hidden windows
  if(!gwin.total and gwin.hidden==NULL)
    return gwin.werrno=W_NOACTIVE;

  // find address of window's record
  _wrec_t* found = wfindrec(whandle);
  if(found==NULL) {
    found = gwin.hidden;
    while(found) {
      if(whandle==found->whandle)
        break;
      found = found->prev;
    }
    if(found==NULL)
      return gwin.werrno=W_NOTFOUND;
  }

  // display character
  vputc(found->srow+wrow+found->border, found->scol+wcol+found->border, attr, chr);

  // return to caller
  return gwin.werrno = W_NOERROR;
}

//  ------------------------------------------------------------------

int wwprints(int whandle, int wrow, int wcol, vattr attr, const char* str) {

  // check for existance of active window or hidden windows
  if(!gwin.total and gwin.hidden==NULL)
    return gwin.werrno=W_NOACTIVE;

  // find address of window's record
  int hidden = NO;
  _wrec_t* found = wfindrec(whandle);
  if(found==NULL) {
    found = gwin.hidden;
    while(found) {
      if(whandle==found->whandle)
        break;
      found = found->prev;
    }
    if(found==NULL)
      return gwin.werrno=W_NOTFOUND;
    hidden = YES;
  }

  // see if window has a border
  int border = found->border;

  // calculate effective coordinates
  int ecol = found->ecol-border;
  __crow  = found->srow+wrow+border;
  __ccol  = found->scol+wcol+border;
  __gattr = attr;
  __p     = str;

  // check for valid coordinates
  if((__crow > (found->erow-border)) or (__ccol > ecol))
    return gwin.werrno=W_INVCOORD;

  // save current cursor position
  int oldrow, oldcol;
  if(gvid->isbios())
    vposget(&oldrow,&oldcol);

  // do while not end-of-string and not end-of-window
  while(__ccol <= ecol and *__p) {

    // see if output window is hidden.  if so, then there
    // is no need to check for blocking windows/shadows
    if(hidden)
      *(calc_window(found)) = vcatch(*__p, attr);
    else {

      // check all window records "above" window to activate
      for(__curr=found->next; __curr!=NULL; __curr=__curr->next) {

        // see if current position in window to activate
        // is blocked by same position in test window
        if(window_blocking()) {

          // calculate buffer addresses and swap contents
          update_buffers(calc_window(__curr), 0);
          break;
        }

        // see if test window has a shadow
        if(__curr->wsbuf!=NULL) {

          // see if shadow to the right of test window is
          // blocking the current position of window to activate
          if(rshadow_blocking()) {
            update_buffers(calc_rshadow(__curr), 1);
            break;
          }

          // see if shadow to the bottom of test window is
          // blocking the current position of window to activate
          if(bshadow_blocking()) {
            update_buffers(calc_bshadow(__curr), 1);
            break;
          }
        }
      }

      // if current position is not blocked,
      // then display char to screen
      if(__curr==NULL)
        vputc(__crow, __ccol, attr, *__p);
    }

    // update pointer into string and current column
    __ccol++;
    __p++;
  }

  // restore old cursor position
  if(gvid->isbios())
    vposset(oldrow,oldcol);

  // return to caller
  return gwin.werrno = *__p ? W_STRLONG : W_NOERROR;
}


//  ------------------------------------------------------------------

int wwprintstr(int whandle, int wrow, int wcol, vattr attr, const char* str) {

  // check for existance of active window or hidden windows
  if(!gwin.total and gwin.hidden==NULL)
    return gwin.werrno=W_NOACTIVE;

  // find address of window's record
  _wrec_t* found = wfindrec(whandle);
  if(found==NULL) {
    found = gwin.hidden;
    while(found) {
      if(whandle==found->whandle)
        break;
      found = found->prev;
    }
    if(found==NULL)
      return gwin.werrno=W_NOTFOUND;
  }

  // display string
  vputs(found->srow+wrow+found->border, found->scol+wcol+found->border, attr, str);

  // return to caller
  return gwin.werrno = W_NOERROR;
}


//  ------------------------------------------------------------------
//  Changes the active window's border box type

int wborder(int btype) {

  register int border;

  // check for active window
  if(!gwin.total)
    return gwin.werrno=W_NOACTIVE;

  // check for valid box type
  if((btype<0) or (btype>7))
    return gwin.werrno=W_INVBTYPE;

  // see if window is to have a border
  border = (btype==5) ? NO : YES;

  // redraw window's border
  vbox(
    gwin.active->srow,
    gwin.active->scol,
    gwin.active->erow,
    gwin.active->ecol,
    btype,
    border ? gwin.active->battr : gwin.active->wattr,
    border ? gwin.active->loattr : gwin.active->wattr
  );

  // update window's record
  gwin.active->btype=btype;
  gwin.active->border=border;

  // see if cursor position needs to be updated
  if((gwin.active->row==gwin.active->srow)   or
    (gwin.active->row==gwin.active->erow)    or
    (gwin.active->column==gwin.active->scol) or
    (gwin.active->column==gwin.active->ecol)) {
    wgotoxy(0,0);
  }

  // re-display title if one exists
  if(gwin.active->title!=NULL)
    wtitle(gwin.active->title,gwin.active->tpos,gwin.active->tattr);

  // return normally
  return gwin.werrno=W_NOERROR;
}


//  ------------------------------------------------------------------
//  Fills a region of active window w/specified char/attribute

int wfill(int wsrow, int wscol, int werow, int wecol, vchar chr, vattr atr) {

  // check for active window
  if(!gwin.total)
    return gwin.werrno=W_NOACTIVE;

  // check for valid coordinates
  if(wchkbox(wsrow,wscol,werow,wecol))
    return gwin.werrno=W_INVCOORD;

  // check for window border
  const int &border = gwin.active->border;

  // fill in specified region
  vfill(
    gwin.active->srow+wsrow+border,
    gwin.active->scol+wscol+border,
    gwin.active->srow+werow+border,
    gwin.active->scol+wecol+border,
    chr,
    atr
  );

  // return with no error
  return gwin.werrno=W_NOERROR;
}


//  ------------------------------------------------------------------
//  Returns the handle of the active window

int whandle() {

  // test for active window
  if(!gwin.total) {
    gwin.werrno=W_NOACTIVE;
    return 0;
  }

  // return normally
  gwin.werrno = W_NOERROR;
  return gwin.active->whandle;
}


//  ------------------------------------------------------------------
//  Displays text on window's top or bottom border

int wmessage(const char* str, int border, int leftofs, vattr attr) {

  // check for active window
  if(!gwin.total)
    return gwin.werrno=W_NOACTIVE;

  // make sure window has a border
  if(!gwin.active->border)
    return gwin.werrno=W_NOBORDER;

  int left  = gwin.active->scol+1;
  int right = gwin.active->ecol-1;
  int width = right-left+1;
  int len   = strlen(str);

  // Center string
  if(leftofs < 0)
    leftofs = (len > (width-2)) ? left : (((width/2)+left)-(len/2));

  // make sure string fits in window
  if((gwin.active->scol+leftofs+len-1) > (gwin.active->ecol))
    return gwin.werrno=W_STRLONG;

  // display string
  vputs(border ? gwin.active->erow : gwin.active->srow, gwin.active->scol+leftofs, attr, str);

  // return normally
  return gwin.werrno=W_NOERROR;
}


//  ------------------------------------------------------------------
//  Proportion bar

void wpropbar(int xx, int yy, long len, vattr attr, long pos, long size) {

  //  xx, yy = start position in window.
  //  len    = length (in chars) of progress field.
  //  attr   = color to use for progress field.
  //  pos    = present position.
  //  size   = total size of field.

  const vchar barchar   = _box_table(gwin.active->btype, 13);
#ifdef __UNIX__ // prefferable under xterm
  const vchar thumbchar = ' ';
  vattr thumbattr       = revsattr(attr);
#else
  const vchar thumbchar = '\xDB';
  vattr thumbattr       = attr;
#endif

  long thumblen = (pos*len)/size;

  int x = xx;
  if(thumblen != 0) {
    wputx(yy, x, thumbattr|ACSET, thumbchar, thumblen);
    x += thumblen;
  }
  if(thumblen != len)
    wputx(yy, x, attr|ACSET, barchar, len-thumblen);
}


//  ------------------------------------------------------------------
//  Gives active window a title

int wtitle(const char* str, int tpos, vattr tattr) {

  // check for active window
  if(!gwin.total)
    return gwin.werrno=W_NOACTIVE;

  // redraw box if deleting or moving title
  if(str==NULL or gwin.active->title!=NULL) {
    if(gwin.active->border) {
      vputx(
        ((tpos&TBOTTOM) ? gwin.active->erow : gwin.active->srow),
        gwin.active->scol+1,
        gwin.active->battr|ACSET,
        _box_table(gwin.active->btype, (tpos&TBOTTOM)?6:1),
        gwin.active->ecol-gwin.active->scol-1
      );
    }
  }

  // if not deleting the title, calculate position and display it
  if(str) {

    int left  = gwin.active->scol+1;
    int right = gwin.active->ecol-1;
    int width = right-left+1;
    int len   = strlen(str);

    // don't display title if window is borderless
    if(gwin.active->border) {

      int start;

      switch(tpos&~TBOTTOM) {
        case TLEFT:
          //start = (len>(width-3)) ? left : (left+1);
          start = left;
          break;
        case TCENTER:
          start = (len>(width-2)) ? left : (((width/2)+left)-(len/2));
          break;
        default:        // default is TRIGHT
          {
            int offs = width-len;
            if(offs>2)
              offs--;
            start = (len>width) ? left : (left+offs+1);
          }
      }

      // allocate space for window title string, and copy it there
      char* p = (char*)throw_xmalloc(((width>len) ? width : len)+1);
      if(p==NULL)
        return gwin.werrno=W_ALLOCERR;
      strcpy(p, str);
      *(p+width) = NUL;

      // display title string
      vputs((tpos&TBOTTOM)?gwin.active->erow:gwin.active->srow, start, tattr, p);

      // free allocated space
      throw_xfree(p);

    }
  }

  // update window's record
  gwin.active->title=str;
  gwin.active->tpos=tpos;
  gwin.active->tattr=tattr;

  // return normally
  return gwin.werrno=W_NOERROR;
}


//  ------------------------------------------------------------------

void wscrollbar(int orientation, uint total, uint maxpos, uint pos, int sadd) {

  vattr attr = (gwin.active->sbattr == DEFATTR) ? gwin.active->battr : gwin.active->sbattr;
  vattr invattr              = revsattr(attr);

  const vchar barchar        = _box_table(gwin.active->btype, 13);
  const vchar arrowupchar    = '\x18';
  const vchar arrowdownchar  = '\x19';
  const vchar arrowleftchar  = '\x1B';
  const vchar arrowrightchar = '\x1A';
#ifdef __UNIX__ // prefferable under xterm
  const vchar thumbchar      = ' ';
  vattr thumbattr            = revsattr(attr);
#else
  const vchar thumbchar      = '\xDB';
  vattr thumbattr            = attr;
#endif

  if(maxpos == 0)
    maxpos = 1;

  uint visiblelen;
  if(orientation == W_VERT)
    visiblelen = (gwin.active->erow - (gwin.active->srow+sadd)) + 1 - (gwin.active->border?2:0);
  else
    visiblelen = (gwin.active->ecol - (gwin.active->scol+sadd)) + 1 - (gwin.active->border?2:0) - 2;
  uint barlen = visiblelen - 2;
  uint thumblen = (visiblelen*barlen) / total;
  if(thumblen == 0)
    thumblen = 1;
  else if(thumblen > barlen)
    thumblen = barlen;
  uint maxthumbpos = barlen - thumblen;
  uint thumbpos = (pos*maxthumbpos) / maxpos;
  uint thumbdiv = (pos*maxthumbpos) % maxpos;
  if((thumbdiv >= (maxpos/2)) and (maxpos > 1))
    thumbpos++;
  if(thumbpos > maxthumbpos)
    thumbpos = maxthumbpos;
  barlen -= thumbpos + thumblen;

  if(orientation == W_VERT) {
    uint scol = gwin.active->ecol - gwin.active->scol - gwin.active->border;
    vputc((sadd++)+gwin.active->srow+gwin.active->border, gwin.active->ecol, invattr|ACSET, arrowupchar);
    if(thumbpos != 0) {
      wputy(sadd, scol, attr|ACSET, barchar, thumbpos);
      sadd += thumbpos;
    }
    if(thumblen != 0) {
      wputy(sadd, scol, thumbattr|ACSET, thumbchar, thumblen);
      sadd += thumblen;
    }
    if(barlen != 0) {
      wputy(sadd, scol, attr|ACSET, barchar, barlen);
      sadd += barlen;
    }
    vputc(sadd+gwin.active->srow+gwin.active->border, gwin.active->ecol, invattr|ACSET, arrowdownchar);
  }
  else {
    uint srow = gwin.active->erow - gwin.active->srow - gwin.active->border;
    vputc(gwin.active->erow, (sadd++)+gwin.active->scol+gwin.active->border, invattr|ACSET, arrowleftchar);
    if(thumbpos != 0) {
      wputx(srow, sadd, attr|ACSET, barchar, thumbpos);
      sadd += thumbpos;
    }
    if(thumblen != 0) {
      wputx(srow, sadd, thumbattr|ACSET, thumbchar, thumblen);
      sadd += thumblen;
    }
    if(barlen != 0) {
      wputx(srow, sadd, attr|ACSET, barchar, barlen);
      sadd += barlen;
    }
    vputc(gwin.active->erow, sadd+gwin.active->scol+gwin.active->border, invattr|ACSET, arrowrightchar);
  }
}


//  ------------------------------------------------------------------
