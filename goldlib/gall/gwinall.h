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
//  Based on CXL by Mike Smedley.
//  Windowing functions.
//  ------------------------------------------------------------------

#ifndef __gwinall_h
#define __gwinall_h


//  ------------------------------------------------------------------

#include <gdefs.h>
#include <gstrall.h>
#include <gkbdbase.h>
#include <gvidall.h>


//  ------------------------------------------------------------------
//  Error codes returned from windowing functions

#define W_NOERROR   0       // no error
#define W_ESCPRESS  1       // Escape key was pressed
#define W_ALLOCERR  2       // memory allocation error
#define W_NOTFOUND  3       // record not found
#define W_NOACTIVE  4       // no active window
#define W_INVCOORD  5       // invalid coordinates
#define W_INVFORMT  6       // invalid format string
#define W_NOINPDEF  7       // no input fields defined
#define W_STRLONG   8       // string too long for window
#define W_INVBTYPE  9       // invalid box type
#define W_NOBORDER  10      // no window border
#define W_NOHIDDEN  11      // no hidden windows
#define W_NOTHIDD   12      // window is not hidden
#define W_NOSELECT  13      // no selectable menu items
#define W_NOITMDEF  14      // no menu items defined
#define W_NOMNUEND  15      // no end of menu specified
#define W_NOMNUDEF  16      // no menu defined
#define W_NOMNUBEG  17      // no begin of menu specified
#define W_NOFRMDEF  18      // no form defined
#define W_NOFRMBEG  19      // no begin of form specified
#define W_NOHLPDEF  20      // no help record defined
#define W_HLPSTKOV  21      // help stack overflow
#define W_HLPSTKUN  22      // help stack underflow
#define W_DOSERROR  23      // DOS error
#define W_NOMATCH   24      // no files matched input filespec
#define W_INVTAGID  25      // invalid tag identifier

                                           
//  ------------------------------------------------------------------
//  Window border identifiers

#define TP_BORD  0      // top border
#define BT_BORD  1      // bottom border
#define LT_BORD  2      // left border
#define RT_BORD  3      // right border


//  ------------------------------------------------------------------
//  Direction codes

#define D_DOWN    0
#define D_UP      1
#define D_LEFT    2
#define D_RIGHT   3
#define SDOWN     D_DOWN    // scroll down
#define SUP       D_UP      // scroll up


//  ------------------------------------------------------------------
//  Scrollbar orientation

const int W_HORZ = 1;
const int W_VERT = 2;


//  ------------------------------------------------------------------
//  Menu item record definition

struct _item_t {
  _item_t*  prev;       // pointer to previous record
  _item_t*  next;       // pointer to next record
  void*     child;      // pointer to submenu's record
  const char* str;      // address of selection string
  const char* desc;     // text description of menu option
  VfvCP     select;     // address of selection function
  VfvCP     before;     // address of "before" function
  VfvCP     after;      // address of "after"  function
  gkey      hotkey;     // hot key to select function
  int       tagid;      // tag identifier
  int       help;       // help category number
  int       wrow;       // start of text - window row
  int       wcol;       // start of text - window column
  char      schar;      // quick selection character
  int       fmask;      // special feature mask
  int       dwhdl;      // description window handle
  int       dwrow;      // description window row
  int       dwcol;      // description window column
  vattr     dattr;      // description attribute
  int       redisp;     // redisplay flag
};


//  ------------------------------------------------------------------
//  Window menu record definition

struct _menu_t {
  _menu_t*  prev;       // pointer to prev menu structure
  _menu_t*  next;       // pointer to next menu structure
  _menu_t*  parent;     // pointer to parent menu
  _item_t*  item;       // pointer to head menu item
  _item_t*  citem;      // pointer to current menu item
  VfvCP     open;       // address of post-opening function
  int       tagcurr;    // tag ID of item selection bar on
  int       usecurr;    // will menu use current window?
  int       srow;       // starting row of menu window
  int       scol;       // starting column of menu window
  int       erow;       // ending row of menu window
  int       ecol;       // ending column of menu window
  int       btype;      // menu window border type
  vattr     battr;      // menu window "hi" border attribute
  vattr     loattr;     // menu window "lo" border attribute
  vattr     sbattr;     // menu window scrollbar attribute
  vattr     wattr;      // menu window attribute
  int       menutype;   // menu type mask
  int       barwidth;   // width of menu bar or zero
  int       textpos;    // offset of text from start of bar
  vattr     textattr;   // attribute of menu text
  vattr     scharattr;  // attribute of selection character
  vattr     noselattr;  // non-selectable text attribute
  vattr     barattr;    // attribute of selection bar
  const char* title;    // menu title string or NULL if no title
  int       titlepos;   // position of menu title (TLEFT,TCENTER,TRIGHT)
  vattr     titleattr;  // attribute of menu title
  vattr     shadattr;   // shadow attribute or -1 if no shadow
  int       items;      // number of items in menu
  bool      hotkey;
};


//  ------------------------------------------------------------------
//  Window input field definition

struct _field_t {
  _field_t*  prev;       // pointer to previous field
  _field_t*  next;       // pointer to next field
  char*      str;        // address of receiving string
  char*      buf;        // address of temp receive string
  const char* format;    // input field format string
  IfcpCP     validate;   // address of validation function
  VfvCP      before;     // address of "before" function
  VfvCP      after;      // address of "after"  function
  int        help;       // help category number
  int        lenbuf;     // length of buffer
  int        lenfld;     // length of screen input field
  int        lenfmt;     // length of format string
  int        wrow;       // start of input - window row
  int        wcol;       // start of input - window column
  int        mode;       // 0=init, 1=update, 2=cond update
  int        decpos;     // decimal position (numeric)
  int        redisp;     // redisplay flag
  char       fconv;      // field conversion character
};

                                 
//  ------------------------------------------------------------------
//  Window data entry form definition

typedef gkey (*KfipCP)(int*);

struct _form_t {
  KfipCP    getkey;     // pointer to alternate get func
  _form_t*  prev;       // pointer to previous form record
  _form_t*  next;       // pointer to next form record
  _field_t* field;      // pointer to head field record
  _field_t* cfield;     // pointer to current field record
  gkey*     termkey;    // addr of int for terminating key
  const char* pformat;  // format string pointer
  char*     pbuf;       // buffer string pointer
  int       cwrow;      // current window row
  int       cwcol;      // current window column
  int       decimal;    // decimal field flag
  int       insert;     // insert mode flag
  int       fieldattr;  // field attribute
  int       textattr;   // text attribute
};


//  ------------------------------------------------------------------
//  Structure of window records

struct _wrec_t {
  _wrec_t* prev;       // pointer to previous window record
  _wrec_t* next;       // pointer to next window record
  _form_t* form;       // pointer to head form record
  vsavebuf* wbuf;      // address of window's buffer
  vatch*   wsbuf;      // address of window shadow's buffer
  const char* title;   // address of window's title string
  int      whandle;    // window's handle
  int      help;       // help category number
  int      srow;       // start row of window
  int      scol;       // start column of window
  int      erow;       // end row of window
  int      ecol;       // end column of window
  int      btype;      // window's box type
  vattr    wattr;      // window's initial text attribute
  vattr    battr;      // attribute of window's border
  vattr    loattr;     // attribute of window's border
  vattr    sbattr;     // attribute of window's scrollbar
  int      border;     // has border?  0 = no, 1 = yes
  int      row;        // window's current cursor row
  int      column;     // window's current cursor column
  vattr    attr;       // window's current text attribute
  int      tpos;       // position of window's title
  vattr    tattr;      // attribute of window's title
  vattr    wsattr;     // attribute of window's shadow
};                                 


//  ------------------------------------------------------------------

class _help_t;


//  ------------------------------------------------------------------
//  Window information record

class GWin {

public:

  _wrec_t* active;     // pointer to active window
  _wrec_t* hidden;     // pointer to head hidden window
  _menu_t* menu;       // pointer to head menu record
  _menu_t* cmenu;      // pointer to current menu record
  _help_t* helptr;     // pointer to help info record
  int      handle;     // last handle given to a window
  int      help;       // current help category
  int      werrno;     // error num from last window func
  int      total;      // total number of open windows
  int      mlevel;     // system variable used in menus
  int      ilevel;     // system variable used in menus
  int      esc;        // check for Esc in input funcions?
  int      tabwidth;   // window TTY output tab width
  vchar    fillch;     // character to fill windows with
  int      style;      // how to open the windows

public:

  GWin();
  ~GWin();

};

extern GWin gwin;


//  ------------------------------------------------------------------
//  Window open styles

#define STYLE_NORMAL        0
#define STYLE_EXPLODE       1
#define STYLE_EXPLODENOISY  2


//  ------------------------------------------------------------------
//  Fmask definitions for wmenuitem()

#define M_HASPD     1   // has pull-down menu attached
#define M_NOSEL     2   // is not selectable
#define M_CLOSE     4   // close menu after select func
#define M_CLALL     8   // close all menus when selected
#define M_CLOSB    16   // close menu before select func
#define M_SEPAR    (32 + M_NOSEL) // item is separator

//  ------------------------------------------------------------------
//  Menutype definitions for wmenuend()

#define M_HORZ      1   // horizontal menu
#define M_VERT      2   // vertical menu
#define M_OMNI      7   // omnidirectional menu
#define M_PD        8   // pull-down menu
#define M_NOQS      16  // disable quick selection
#define M_SAVE      32  // save last bar position


//  ------------------------------------------------------------------
//  Special return codes from wmenuget()

#define M_EXIT      32764   // exit menu
#define M_EXITALL   32765   // exit all menus
#define M_PREVPD    32766   // previous pull-down menu
#define M_NEXTPD    32767   // next pull-down menu


//  ------------------------------------------------------------------
//  Window title position definitions for wtitle()

#define TTOP        0   // Top border
#define TLEFT       1   // Left justified
#define TCENTER     2   // Centered
#define TRIGHT      3   // Right justified
#define TBOTTOM     4   // Bottom border


//  ------------------------------------------------------------------
//  Values for the Proportional Bar

#define PROP_PAGE      1
#define PROP_BARGRAPH  2


//  ------------------------------------------------------------------

extern int wpickstr_tag;


//  ------------------------------------------------------------------
//  Function prototypes

int       wactiv      (int whandle);
int       wactiv_     (int whandle);
int       wborder     (int btype);
int       wbox        (int wsrow, int wscol, int werow, int wecol, int btype, int attr);
int       wcclear     (vattr attr);
int       wcenters    (int wrow, vattr attr, const char* str);
int       wchkbox     (int wsrow, int wscol, int werow, int wecol);
int       wchkcol     (int wcol);
int       wchkcoord   (int wrow, int wcol);
int       wchkrow     (int wrow);
int       wclose      ();
int       wcloseall   ();
int       wclreol     ();
int       wclreos     ();
int       wcopy       (int nsrow, int nscol);
int       wdelline    (int wrow, int direc);
int       wdrag       (int direction);
int       wdupc       (char ch, int count);
int       wfill       (int wsrow, int wscol, int werow, int wecol, vchar ch, vattr attr);
_wrec_t*  wfindrec    (int whandle);
int       wgotoxy     (int wrow, int wcol);
int       whandle     ();
int       whide       ();
int       whline      (int wsrow, int wscol, int count, int btype, vattr attr);
int       wmessage    (const char* str, int border, int leftofs, vattr attr);
int       wmove       (int nsrow, int nscol);
int       wopen       (int srow, int scol, int erow, int ecol, int btype, vattr battr, vattr wattr, vattr sbattr = DEFATTR, vattr loattr = DEFATTR);
inline int wopen_     (int srow, int scol, int vlen, int hlen, int btype, vattr battr, vattr wattr, vattr sbattr = DEFATTR, vattr loattr = DEFATTR) { return wopen(srow, scol, srow+vlen-1, scol+hlen-1, btype, battr, wattr, sbattr, loattr); }
int       wperror     (const char* message);
bool      wpickfile   (int srow, int scol, int erow, int ecol, int btype, vattr bordattr, vattr winattr, vattr barattr, bool title, std::string &filespec, IfcpCP open, bool casesens=false);
int       wpickstr    (int srow, int scol, int erow, int ecol, int btype, vattr bordattr, vattr winattr, vattr barattr, gstrarray &strarr, int initelem, VfvCP open);
int       wprintc     (int wrow, int wcol, vattr attr, vchar ch);
int       wprintf     (const char* format, ...) __attribute__ ((format (printf, 1, 2)));
int       wprintaf    (int attr, const char* format, ...) __attribute__ ((format (printf, 2, 3)));
int       wprintfs    (int wrow, int wcol, vattr attr, const char* format, ...) __attribute__ ((format (printf, 4, 5)));
int       wprints     (int wrow, int wcol, vattr attr, const char* str);
int       wprints_box (int wrow, int wcol, vattr attr, const char* str);
int       wprintvs    (int wrow, int wcol, vattr attr, const vchar* str);
int       wprintns    (int wrow, int wcol, vattr attr, const std::string &str, uint len, vchar fill=' ', vattr fill_attr = DEFATTR);
int       wprintsf    (int wrow, int wcol, vattr attr, const char* format, const char* str);
int       wprintws    (int wrow, int wcol, vatch* buf, uint len);
void      wpropbar    (int xx, int yy, long len, vattr attr, long pos, long size);
int       wputc       (vchar ch);
int       wputs       (const char* str);
int       wputx       (int wrow, int wcol, vattr attr, vchar chr, uint len);
int       wputy       (int wrow, int wcol, vattr attr, vchar chr, uint len);
int       wreadcur    (int* wrow, int* wcol);
int       wscroll     (int count, int direc);
void      wscrollbar  (int orientation, uint total, uint maxpos, uint pos, int sadd=0);
int       wscrollbox  (int wsrow, int wscol, int werow, int wecol, int count, int direction);
int       wshadoff    ();
int       wshadow     (vattr attr);
int       wsize       (int nerow, int necol);
int       wslide      (int nsrow, int nscol);
void      wtextattr   (vattr attr);
int       wtitle      (const char* str, int tpos, vattr tattr);
int       wunhide     (int whandle);
int       wunlink     (int w);
int       wvline      (int wsrow, int wscol, int count, int btype, vattr attr);
int       wwprintc    (int whandle, int wrow, int wcol, vattr attr, const vchar chr);
int       wwprints    (int whandle, int wrow, int wcol, vattr attr, const char* str);
int       wwprintstr  (int whandle, int wrow, int wcol, vattr attr, const char* str);

int       wmenubeg    (int srow, int scol, int erow, int ecol, int btype, vattr battr, vattr wattr, VfvCP open, int menutype=M_VERT);
inline int wmenubeg_  (int srow, int scol, int vlen, int hlen, int btype, vattr battr, vattr wattr, VfvCP open, int menutype=M_VERT) { return wmenubeg(srow, scol, srow+vlen-1, scol+hlen-1, btype, battr, wattr, open, menutype); }
int       wmenubegc   ();
int       wmenuend    (int taginit, int menutype, int barwidth, int textpos, vattr textattr, vattr scharattr, vattr noselattr, vattr barattr);
int       wmenuget    ();
int       wmenuiba    (VfvCP before, VfvCP after);
int       wmenuidsab  (int tagid);
int       wmenuienab  (int tagid);
_item_t*  wmenuifind  (int tagid);
int       wmenuinext  (int tagid);
int       wmenuitem   (int wrow, int wcol, const char* str, char schar, int tagid, int fmask, VfvCP select, gkey hotkey, int help);
int       wmenuitxt   (int whdl, int wrow, int wcol, vattr attr, const char* str);


//  ------------------------------------------------------------------
//  Inline functions

inline void wtextattr(vattr attr) { gwin.active->attr = attr; }

inline int       wclear      ()       { return wcclear(gwin.active->wattr); }
inline void      wfillch     (vchar a) { gwin.fillch=a; }
inline vchar     wgetc       (int wrow, int wcol) { return vgetc(gwin.active->srow+wrow+gwin.active->border,gwin.active->scol+wcol+gwin.active->border); }
inline int       wisactiv    (int a)  { return a == gwin.active->whandle; }
inline int       wsetesc     (int a)  { int t=gwin.esc; gwin.esc=a; return t; }
inline void      wsetstyle   (int a)  { gwin.style = a; }
inline int       wstyle      ()       { return gwin.style; }
inline void      wtabwidth   (int a)  { gwin.tabwidth = (a==0) ? 1 : a; }

inline _field_t* winpfcurr   ()       { return gwin.active->form->cfield; }

inline _menu_t*  wmenumcurr  ()       { return gwin.cmenu; }
inline _item_t*  wmenuicurr  ()       { return wmenumcurr()->citem; }

inline int       wmenutitshad(const char* title, int pos, vattr attr, vattr shadattr) { gwin.cmenu->title=title; gwin.cmenu->titlepos=pos; gwin.cmenu->titleattr=attr; gwin.cmenu->shadattr=shadattr; return W_NOERROR; }


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
