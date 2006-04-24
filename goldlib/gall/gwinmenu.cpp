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
//  wmenubeg()  starts a menu definition
//  wmenuitem() defines a menu item
//  wmenuend()  ends a menu definition
//  wmenuget()  processes the defined menu
//  ------------------------------------------------------------------

#include <gctype.h>
#include <cstdio>
#include <cstdlib>
#include <gmemdbg.h>
#include <gkbdcode.h>
#include <gkbdbase.h>
#include <gmoubase.h>
#include <gvidall.h>
#include <gwinall.h>
#include <gwinhelp.h>

#if defined(__USE_ALLOCA__)
  #include <malloc.h>
#endif


//  ------------------------------------------------------------------

int _overtagid;
int _finaltagid;

int gmnudropthrough = NO;


//  ------------------------------------------------------------------
//  prototypes for local functions

static _item_t* down_item(_item_t *curr);
static _item_t* left_item(_item_t *curr);
static _item_t* right_item(_item_t *curr);
static _item_t* up_item(_item_t *curr);


//  ------------------------------------------------------------------
//  menu item movement definitions

#define ITM_LT  0
#define ITM_RT  1
#define ITM_UP  2
#define ITM_DN  3
#define ITM_FR  4
#define ITM_LS  5


//  ------------------------------------------------------------------
//  array of function pointers to some of the item movement functions

static _item_t *(*mnu_funcs[4])(_item_t *) = {
  left_item,
  right_item,
  up_item,
  down_item
};

//  ------------------------------------------------------------------
//  flag used for initial display of menu selections

static int dispdesc=YES;


//  ------------------------------------------------------------------
// this function will calculate the width of the selection bar

static int calc_bar_width(_menu_t *wmenu,_item_t *witem)
{
    register int width;

    width=strlen(witem->str);
    if(wmenu->barwidth) width=wmenu->barwidth;
    return(width);
}


//  ------------------------------------------------------------------
// this function will calculate the window column
// at the center of the given menu item

static int calc_center_item(_item_t *item)
{
    return( ((int)item->wcol) + (strlen(item->str)/2) );
}


//  ------------------------------------------------------------------
//  this function will hide the mouse cursor
//  if mouse cursor mode is on

static void hide_mouse_cursor_mnu(void) {

  #ifdef GOLD_MOUSE
  if(gmou.FreeCursor())
    gmou.HideCursor();
  #endif
}


//  ------------------------------------------------------------------
//  this function will display the mouse
//  cursor if mouse cursor mode is on

static void show_mouse_cursor_mnu(void) {

  #ifdef GOLD_MOUSE
  if(gmou.FreeCursor()) {
    gmou.ShowCursor();
    gmou.SetCursor(0,0xFFFF,((LGREY_|_LGREY)<<8));
  }
  #endif
}


//  ------------------------------------------------------------------
//  this function calls the given function

static void mnu_call_func(VfvCP func) {

    _menu_t *menu;
    int w;
    int err;

    hide_mouse_cursor_mnu();
    menu=gwin.cmenu;
    w=whandle();
    err=whelpush();
    (*func)();
    wactiv(w);
    if(!err) whelpop();
    gwin.cmenu=menu;
    show_mouse_cursor_mnu();
}


//  ------------------------------------------------------------------
// this function will call a menu item's "after" function if it exists

static void call_after(_item_t *citem)
{
    if(citem->after!=NULL)
      mnu_call_func(citem->after);
}


//  ------------------------------------------------------------------
// this function will call a menu item's "before" function if it exists

static void call_before(_item_t *citem)
{
    if(citem->before!=NULL)
      mnu_call_func(citem->before);
}


//  ------------------------------------------------------------------
// this function closes the current menu's window and reactivates
// the menu window open prior to opening the current menu window

static void close_window(int w)
{
    if(!gwin.cmenu->usecurr) {
        hide_mouse_cursor_mnu();
        wclose();
        wactiv(w);
        show_mouse_cursor_mnu();
    }
}


//  ------------------------------------------------------------------
// this function displays a menu selection, using selection bar if specified

static void disp_item(_item_t *witem,int bar)
{
#if defined(__USE_ALLOCA__)
    char *buf = (char *)alloca(sizeof(vatch)*gvid->numcols);
#else
    __extension__ char buf[sizeof(vatch)*gvid->numcols];
#endif
    char ch;
    vattr chattr;
    _wrec_t* whp;
    register const char* p;
    register vatch* ptr=(vatch*)buf;
    int i, textend,width,wcol,found=NO;

    // if mouse cursor is on, temporarily hide it
    hide_mouse_cursor_mnu();

    // initialize width of output and end of text
    p = witem->str;
    width = calc_bar_width(gwin.cmenu,witem);
    textend = gwin.cmenu->textpos+strlen(p)-1;
    wgotoxy(witem->wrow,wcol=witem->wcol);
    if (width > (gvid->numcols-2)) width = gvid->numcols - 2;

    // display separators
    if (witem->fmask & M_SEPAR)
    {
      const int &border = gwin.active->border;
      const int &btype = gwin.active->btype;
      const vattr &attr = gwin.active->loattr;
      vatch line = vcatch(_box_table(btype, 1), attr);

      if (border) *ptr++ = vcatch(_box_table(btype, 9), attr);
      for (i = 0; i < width; i++) *ptr++ = line;
      if (border) *ptr = vcatch(_box_table(btype, 10), attr);

      int row = gwin.active->srow + witem->wrow + border;
      int col = gwin.active->scol + wcol;
      vputws(row, col, (vatch*)buf, width+2);
    }
    else
    {
      // display menu item including selection bar
      for(i=0; i<width; i++) {

        // see if currently in bar region.  if so, then use
        // a space for the character. otherwise use the
        // character from the current position in the string
        ch = ((i<gwin.cmenu->textpos) or (i>textend)) ? ' ' : (*p++);

        // select attribute of character to be displayed based upon if
        // selection bar was specified, if the menu item is non-selectable,
        // if the character is a tag character, or none of the above.
        if(bar)
          chattr = gwin.cmenu->barattr;
        else if(witem->fmask&M_NOSEL)
          chattr = gwin.cmenu->noselattr;
        else if((ch==witem->schar) and not found) {
          found = YES;
          chattr = gwin.cmenu->scharattr;
        }
        else
          chattr = gwin.cmenu->textattr;

        // display character in selected attribute
        //wprintc(witem->wrow,wcol++,chattr,ch);

        // Build menu line buffer

        *ptr++ = vcatch(ch,chattr);
      }

      // Display complete buffer
      wprintws(witem->wrow, wcol, (vatch*)buf, width);
    }

    // display text description, if one exists
    if((witem->desc!=NULL) and dispdesc) {
      whp = wfindrec(witem->dwhdl);
      i = (1 + whp->ecol - whp->scol) - (whp->border ? 2 : 0);
      sprintf(buf, "%-*.*s", i, i, witem->desc);
      wwprints(witem->dwhdl, witem->dwrow, witem->dwcol, witem->dattr, buf);
    }

    // if mouse cursor is hidden, unhide it
    show_mouse_cursor_mnu();
}


//  ------------------------------------------------------------------
// this function finds the next menu selection down

static _item_t * down_item(_item_t *curr)
{
    _item_t *best,*temp;
    int brow,bcol,tcol,trow,crow,ccol,tdist,bdist;

    // initialize best record to NULL
    best=NULL;
    brow=bcol=32767;

    // calculate window column at center of current item
    crow=(int)curr->wrow;
    ccol=calc_center_item(curr);

    // search backwards through linked list, testing each item
    for(temp=gwin.cmenu->item;temp!=NULL;temp=temp->prev) {

        // calculate window column at center of test item
        trow=(int)temp->wrow;
        tcol=calc_center_item(temp);

        if(trow>crow) {
            tdist=abs((ccol-tcol));
            bdist=abs((ccol-bcol));
            if((trow<brow) or ((trow==brow) and (tdist<bdist))) {
                best=temp;
                brow=trow;
                bcol=tcol;
            }
        }
    }

    // if there wasn't an item downwards, then wrap around
    if(best==NULL) {
        if((temp=(_item_t*)throw_malloc(sizeof(_item_t)))==NULL) {
            best=curr;
        }
        else {
            *temp=*curr;
            temp->wrow=-1;
            best=down_item(temp);
            throw_free(temp);
        }
    }
    else
        // see if menu selection is non-selectable
        if(best->fmask&M_NOSEL)
            best=down_item(best);

    // return best record
    return(best);
}


//  ------------------------------------------------------------------
// this function finds the upper-leftmost menu selection

static _item_t * first_item(void)
{
    _item_t *best,*temp;

    // initialize best record to highest record in linked list
    best=gwin.cmenu->item;

    // search backwards through linked list, testing each item
    for(temp=best->prev;temp!=NULL;temp=temp->prev) {
        if((temp->wrow<best->wrow) or ((temp->wrow==best->wrow) and (temp->wcol<best->wcol)))
            best=temp;
    }

    // see if menu selection is non-selectable
    if(best->fmask&M_NOSEL) best=right_item(best);

    // return best record
    return(best);
}


//  ------------------------------------------------------------------
// this is a recursive function that frees a menu and all of its submenus

static void free_menu(_menu_t *wmenu)
{
    _item_t *witem;

    // free all items in menu, including sub-menus
    while(wmenu->item!=NULL) {
        if(wmenu->item->child!=NULL) free_menu((_menu_t*)wmenu->item->child);
        witem=wmenu->item->prev;
        throw_free(wmenu->item);
        wmenu->item=witem;
        if(wmenu->item!=NULL) wmenu->item->next=NULL;
    }

    // free the menu itself
    throw_free(wmenu);
}


//  ------------------------------------------------------------------
// this function will find the bottom-rightmost menu selection

static _item_t * last_item(void)
{
    _item_t *best,*temp;
    int bcol;

    // initialize best record to highest record in linked list
    best=gwin.cmenu->item;
    bcol=best->wcol;

    // search backwards through linked list, testing each item
    for(temp=best->prev;temp!=NULL;temp=temp->prev) {
        if((temp->wrow>best->wrow) or ((temp->wrow==best->wrow) and (temp->wcol>bcol))) {
            best=temp;
            bcol=best->wcol;
        }
    }

    // see if menu selection is non-selectable
    if(best->fmask&M_NOSEL) best=left_item(best);

    // return best record
    return(best);
}


//  ------------------------------------------------------------------
// this function is called after a menu bar move

static void post_move(_item_t *citem)
{
    gwin.cmenu->citem=citem;
    gwin.help=citem->help;
    disp_item(citem,1);
    call_before(citem);
}


//  ------------------------------------------------------------------
// this function prepares for a menu bar move

static void pre_move(_item_t *citem)
{
    disp_item(citem,0);
    call_after(citem);
}


//  ------------------------------------------------------------------
// this function moves the selection bar to another menu item,
// automatically taking care of necessary pre/post move actions

static _item_t * goto_item(_item_t *citem,int which)
{
    _item_t *item;

    if(which==ITM_FR)       item=first_item();
    else if(which==ITM_LS)  item=last_item();
    else                    item=(*mnu_funcs[which])(citem);
    if(item!=citem) {
        pre_move(citem);
        post_move(citem=item);
    }
    return(citem);
}


//  ------------------------------------------------------------------
// this function will find the menu selection to the left of current

static _item_t * left_item(_item_t *curr)
{
    _item_t *best,*temp;
    int wwidth,bpos,tpos,cpos;

    // calculate window width and current position
    wwidth=gwin.cmenu->ecol - gwin.cmenu->scol + 1;
    cpos=(curr->wrow * wwidth) + curr->wcol;

    // initialize best record to NULL, and best position to -1
    best=NULL;
    bpos=-1;

    // search backwards through linked list, testing each item
    for(temp=gwin.cmenu->item;temp!=NULL;temp=temp->prev) {

        // calculate position of test each item
        tpos=(temp->wrow*wwidth) + temp->wcol;

        // compare position of test item with best item, current item
        if((tpos>bpos) and (tpos<cpos)) {
            best=temp;
            bpos=tpos;
        }
    }

    // if there wasn't a item to the left, then wrap around
    if(best==NULL)
        best=last_item();
    else
        // see if menu selection is non-selectable
        if(best->fmask&M_NOSEL)
            best=left_item(best);

    // return best record
    return(best);
}


//  ------------------------------------------------------------------
// this function determines if the mouse cursor is on a menu item

#ifdef GOLD_MOUSE
static _item_t *mouse_on_item(_menu_t *menu,int mcrow,int mccol)
{
    int srow,scol,border,start,end;
    _item_t *item,*found;

    found  = NULL;
    srow   = menu->srow;
    scol   = menu->scol;
    border = menu->btype==5?0:1;
    for(item=menu->item;item!=NULL;item=item->prev) {
        if(mcrow==(srow+border+item->wrow)) {
            start = scol+border+item->wcol;
            end   = start+calc_bar_width(menu,item)-1;
            if((mccol>=start) and (mccol<=end)) {
                found=item;
                break;
            }
        }
    }
    return(found);
}
#endif


//  ------------------------------------------------------------------

static void pre_exit(int w,int close)
{
    _menu_t *wmenu;

    hide_mouse_cursor_mnu();

    // if not using current window for menu, then close it
    if(close) close_window(w);

    // if at highest menu then free the whole menu structure
    if(gwin.cmenu==gwin.menu) {
        wmenu=gwin.menu->prev;
        if(gwin.cmenu!=NULL)
          free_menu(gwin.cmenu);
        gwin.menu=wmenu;
        if(gwin.menu!=NULL)
          gwin.menu->next=NULL;
        gwin.cmenu=gwin.menu;
    }
}


//  ------------------------------------------------------------------
// this function reads the mouse for input

static gkey read_mouse(_item_t* citem) {

  #ifdef GOLD_MOUSE
  register _item_t *item;

  // if free-floating mouse cursor support is on
  if(gmou.FreeCursor()) {

    // clear mouse button queue
    gmou.ClearEvents();

    // loop until a key is pressed
    while(!kbxhit() and gkbd.kbuf==NULL) {

      // call the keyboard loop function
      //if(gkbd.kbloop!=NULL)
        //(*gkbd.kbloop)();

      // if left button was pressed, and mouse cursor is on
      // a selectable menu item, then move selection bar to
      // that item, and select it.  If mouse cursor is on
      // a main menu item of a pull-down menu system, then
      // stuff that item's selection character into the CXL
      // keyboard buffer and return Esc to close the current
      // pull-down menu.
      gmou.GetLeftRelease();
      if(gmou.Count()) {
        item = mouse_on_item(gwin.cmenu,gmou.Row(),gmou.Column());
        if(item == NULL) {
          if(gwin.cmenu->menutype&M_PD) {
            item = mouse_on_item(gwin.cmenu->parent,gmou.Row(),gmou.Column());
            if(item and (!(item->fmask&M_NOSEL))) {
              kbput(item->schar);
              return Key_Esc;
            }
          }
        }
        else {
          if(!(item->fmask&M_NOSEL)) {
            if(citem != item) {
              pre_move(citem);
              gwin.cmenu->citem = citem = item;
              post_move(item);
            }
            return Key_Ent;
          }
        }
      }

      // if right button was pressed, simulate pressing the Esc key
      gmou.GetRightRelease();
      if(gmou.Count())
        return Key_Esc;
    }
  }
  #endif

  // return zero - it means a key was pressed
  return 0;
}


//  ------------------------------------------------------------------
// this function will find the menu selection to the right of current

static _item_t * right_item(_item_t *curr)
{
    _item_t *best,*temp;
    int wwidth,bpos,tpos,cpos;

    // calculate window width and current position
    wwidth=gwin.cmenu->ecol - gwin.cmenu->scol + 1;
    cpos=(curr->wrow * wwidth) + curr->wcol;

    // initialize best record to NULL, and best position to 32767
    best=NULL;
    bpos=32767;

    // search backwards through linked list, testing items
    for(temp=gwin.cmenu->item;temp!=NULL;temp=temp->prev) {

        // calculate position of test item
        tpos=(temp->wrow*wwidth) + temp->wcol;

        // compare position of test item with best item, current item
        if((tpos<bpos) and (tpos>cpos)) {
            best=temp;
            bpos=tpos;
        }
    }

    // if there wasn't a item to the right, then wrap around
    if(best==NULL)
        best=first_item();
    else
        // see if menu selection is non-selectable
        if(best->fmask&M_NOSEL)
            best=right_item(best);

    // return best record
    return(best);
}


//  ------------------------------------------------------------------
// this function finds the previous menu selection upwards

static _item_t * up_item(_item_t *curr)
{
    _item_t *best,*temp;
    int brow,bcol,tcol,crow,trow,ccol,tdist,bdist;

    // initialize best record to NULL
    best = NULL;
    brow = -1;
    bcol = 32767;

    // calculate window column at center of current item
    crow=(int)curr->wrow;
    ccol=calc_center_item(curr);

    // search backwards through linked list, testing items
    for(temp=gwin.cmenu->item;temp!=NULL;temp=temp->prev) {

        // calculate window column at center of test item
        trow=(int)temp->wrow;
        tcol=calc_center_item(temp);

        if(trow<crow) {
            tdist=abs(ccol-tcol);
            bdist=abs(ccol-bcol);
            if((trow>brow) or ((trow==brow) and (tdist<bdist))) {
                best=temp;
                brow=trow;
                bcol=tcol;
            }
        }
    }

    // if there wasn't a item to the left, then wrap around
    if(best==NULL) {
        if((temp=(_item_t*)throw_malloc(sizeof(_item_t)))==NULL) {
            best=curr;
        }
        else {
            *temp=*curr;
            temp->wrow=255;
            best=up_item(temp);
            throw_free(temp);
        }
    }
    else
        // see if menu selection is non-selectable
        if(best->fmask&M_NOSEL)
            best=up_item(best);

    // return best record
    return(best);
}


//  ------------------------------------------------------------------

int wmenubeg(int srow, int scol, int erow, int ecol, int btype, vattr battr, vattr wattr, VfvCP open, int menutype) {

    _menu_t* wmenu;

    // if this is a submenu, then make sure that it is under a menu item
    if(gwin.mlevel>gwin.ilevel) return(gwin.werrno=W_NOITMDEF);

    // allocate memory for new menu record
    if((wmenu=(_menu_t*)throw_malloc(sizeof(_menu_t)))==NULL)
        return(gwin.werrno=W_ALLOCERR);

    // if menu is not a submenu, make it a new base menu record
    if(!gwin.mlevel) {
        if(gwin.menu!=NULL) gwin.menu->next=wmenu;
        wmenu->prev=gwin.menu;
        wmenu->next=NULL;
        wmenu->parent=NULL;
        gwin.menu=gwin.cmenu=wmenu;
    }
    else {
        if(menutype & M_PD)
          gwin.cmenu->item->fmask |= M_HASPD;   // Has a pull-down menu
        gwin.cmenu->item->fmask &= ~M_CLOSE;    // Don't close parent menu
        wmenu->parent=gwin.cmenu;
        gwin.cmenu = (_menu_t*)(gwin.cmenu->item->child = wmenu);
    }

    // save info in menu record
    wmenu->srow=srow;
    wmenu->scol=scol;
    wmenu->erow=erow;
    wmenu->ecol=ecol;
    wmenu->btype=btype;
    wmenu->battr=battr;
    wmenu->loattr=battr;
    wmenu->sbattr=battr;
    wmenu->wattr=wattr;
    wmenu->menutype=menutype;
    wmenu->open=open;
    wmenu->usecurr=NO;
    wmenu->item=NULL;
    wmenu->title = "";
    wmenu->titlepos = -1;
    wmenu->titleattr = BLACK_|_BLACK;
    wmenu->shadattr = DEFATTR;
    wmenu->items = 0;

    // increment menu level
    gwin.mlevel++;

    // return normally
    return(gwin.werrno=W_NOERROR);
}


//  ------------------------------------------------------------------

int wmenuitem(int wrow, int wcol, const char* str, char schar, int tagid, int fmask, VfvCP select, gkey hotkey, int help) {

    _item_t* witem;

    // make sure that wmenubeg() or wmenubegc() has been called
    if(!gwin.mlevel)
      return (gwin.werrno=W_NOMNUBEG);

    // allocate memory for new item record
    witem = (_item_t*)throw_malloc(sizeof(_item_t));
    if(witem==NULL)
      return (gwin.werrno=W_ALLOCERR);

    // add new menu item record to linked list
    if(gwin.cmenu->item)
      gwin.cmenu->item->next = witem;
    witem->prev = gwin.cmenu->item;
    witem->next = NULL;
    gwin.cmenu->item = witem;

    // save info in item record
    witem->wrow   = wrow;
    witem->wcol   = wcol;
    witem->str    = str;
    witem->schar  = schar;
    witem->tagid  = tagid;
    witem->fmask  = fmask;
    witem->select = select;
    witem->hotkey = hotkey;
    witem->desc   = NULL;
    witem->dwhdl  = -1;
    witem->dwrow  = 0;
    witem->dwcol  = 0;
    witem->dattr  = BLACK_|_BLACK;
    witem->redisp = NO;
    witem->help   = help;
    witem->child  = NULL;
    witem->before = NULL;
    witem->after  = NULL;

    gwin.cmenu->items++;

    int border = (gwin.cmenu->btype == 5) ? 0 : 1;

    int width = 1 + (gwin.cmenu->ecol-border) - (gwin.cmenu->scol+border);
    size_t _titlen = gwin.cmenu->title ? strlen(gwin.cmenu->title) : 0;
    size_t _strlen = strlen(str);
    size_t length = maximum_of_two(_strlen, _titlen);
    if((int)length > width)
      gwin.cmenu->ecol += length - width;

    if(gwin.cmenu->menutype & M_VERT) {
      int height = 1 + (gwin.cmenu->erow-border) - (gwin.cmenu->srow+border);
      if(gwin.cmenu->items > height)
        gwin.cmenu->erow += gwin.cmenu->items - height;
    }

    // set item level == menu level
    gwin.ilevel = gwin.mlevel;

    // return normally
    return (gwin.werrno=W_NOERROR);
}


//  ------------------------------------------------------------------

int wmenuend(int taginit, int menutype, int barwidth, int textpos, vattr textattr, vattr scharattr, vattr noselattr, vattr barattr) {

    _item_t* item;
    int w_width, border, found;

    // make sure at least 1 menu item has been defined
    if(not gwin.mlevel or (gwin.mlevel>gwin.ilevel))
        return(gwin.werrno=W_NOITMDEF);

    // make sure that the specified initial tagid matches the
    // tagid of one of the defined menu items in this menu
    for(found=NO,item=gwin.cmenu->item;item!=NULL;item=item->prev) {
        if(item->tagid==taginit) {
            found=YES;
            break;
        }
    }
    if(!found) return(gwin.werrno=W_INVTAGID);

    // if bar width > window width, then bar width = window width
    border=(gwin.cmenu->btype==5)?0:1;
    w_width=(gwin.cmenu->ecol-border)-(gwin.cmenu->scol+border)+1;
    if(barwidth>w_width) barwidth=w_width;

    // add rest of menu information to menu record
    gwin.cmenu->citem     = NULL;
    gwin.cmenu->tagcurr   = taginit;
    gwin.cmenu->menutype |= menutype;
    gwin.cmenu->barwidth  = barwidth;
    gwin.cmenu->textpos   = barwidth?textpos:0;
    gwin.cmenu->textattr  = textattr;
    gwin.cmenu->scharattr = scharattr;
    gwin.cmenu->noselattr = noselattr;
    gwin.cmenu->barattr   = barattr;

    // set current menu pointer to parent menu
    if((gwin.cmenu=gwin.cmenu->parent)==NULL) gwin.cmenu=gwin.menu;

    // decrement menu and item levels
    gwin.mlevel--;
    gwin.ilevel--;

    // return with no error
    return(gwin.werrno=W_NOERROR);
}


//  ------------------------------------------------------------------

int wmenuget() {

    int valid, pulldown=NO, menuerr, winerr, err;
    _item_t* citem;
    _item_t* item;
    int w = -1;
    gkey xch;
    char ch;
    static int _depth = 0;

    if (_depth == 0)
    {
      _overtagid = -1;
      _finaltagid = -1;
    }

    // make sure we have a defined menu
    if(gwin.cmenu==NULL) {
        gwin.werrno=W_NOMNUDEF;
        return(-1);
    }

    // make sure that wmenuend() was called
    if(gwin.mlevel or gwin.ilevel) {
        gwin.werrno=W_NOMNUEND;
        return(-1);
    }

    // open menu's window (unless menu is to use current window)
    if(!gwin.cmenu->usecurr) {
        w=whandle();
        hide_mouse_cursor_mnu();
        if(!wopen(gwin.cmenu->srow,gwin.cmenu->scol,gwin.cmenu->erow,gwin.cmenu->ecol,gwin.cmenu->btype,gwin.cmenu->battr,gwin.cmenu->wattr,gwin.cmenu->sbattr,gwin.cmenu->loattr))
          return -1;
        if(gwin.cmenu->shadattr != DEFATTR)
          wshadow(gwin.cmenu->shadattr);
        if(gwin.cmenu->title and *gwin.cmenu->title)
          wtitle(gwin.cmenu->title, gwin.cmenu->titlepos, gwin.cmenu->titleattr);
        show_mouse_cursor_mnu();

        // save environment info, call menu open
        // function, then restore environment info
        if(gwin.cmenu->open!=NULL)
          mnu_call_func(gwin.cmenu->open);
    }

    // if mouse cursor mode is on, show cursor
    show_mouse_cursor_mnu();

    // find first item in linked list
    if((item=gwin.cmenu->item)!=NULL) for(;item->prev!=NULL;item=item->prev);

    // display all menu items
    dispdesc=NO;
    valid=NO;
    while(item!=NULL) {
        disp_item(item,0);
        if(!(item->fmask&M_NOSEL)) valid=YES;
        item=item->next;
    }
    dispdesc=YES;

    // make sure there is at least 1 selectable menu item
    if(!valid) {
        pre_exit(w,YES);
        gwin.werrno=W_NOSELECT;
        return(-1);
    }

    // search linked list of item records for the item matching
    // the last item.  If there was no last item, search for
    // the initial tag ID position.  If no initial tag ID position
    // was specified, then find the upper-leftmost menu item.
    citem=NULL;
    if(gwin.cmenu->menutype&M_SAVE) {
        for(citem=gwin.cmenu->item;citem!=NULL;citem=citem->prev) {
            if((gwin.cmenu->citem==citem) and not (citem->fmask&M_NOSEL))
                break;
        }
    }
    if(citem==NULL) {
        citem=wmenuifind(gwin.cmenu->tagcurr);
        if((citem==NULL) or (citem->fmask&M_NOSEL)) citem=first_item();
    }

    // call the current menu item's 'before'
    // function and display current menu item
    post_move(citem);

    // main process of function

    for(;;) {

        // update current menu item and help category
        gwin.cmenu->citem=citem;
        gwin.help=citem->help;

        // read mouse/keyboard for keypress, then test the keypress
        gwin.menu->hotkey = false;
        gkbd.inmenu=true;
        xch = read_mouse(citem);
        citem=gwin.cmenu->citem;
        if(!xch) {
          xch = getxch();
        }
        gkbd.inmenu=false;

        _overtagid = citem->tagid;

        switch(xch) {

            case Key_Esc:

                // if Esc checking is on, erase selection bar,
                // free menu records and return to caller
                if(gwin.esc or (gwin.cmenu!=gwin.menu)) {
                    ESCAPE_KEY:
                    pre_move(citem);
                    pre_exit(w,YES);
                    gwin.werrno=W_ESCPRESS;
                    return(-1);
                }
                break;

            case Key_Home:

                // hide selection bar and find upper-leftmost menu item
                citem=goto_item(citem,ITM_FR);
                break;

            case Key_Lft:

                // if current menu is a pull-down menu,
                // then erase selection bar, free menu records,
                // and return special code to caller
                if(gwin.cmenu->menutype&M_PD) {
                    pre_move(citem);
                    pre_exit(w,YES);
                    gwin.werrno=W_NOERROR;
                    return(M_PREVPD);
                }

                // if current menu is a horizontal menu, then hide
                // selection bar and find menu item to the left
                if(gwin.cmenu->menutype&M_HORZ)
                    citem=goto_item(citem,ITM_LT);

                // if pull-down menu flag is set, select this item
                if(pulldown and (citem->child!=NULL))
                    goto ENTER;
                break;

            case Key_Up:

                // if current menu is a vertical menu, then hide
                // selection bar and find menu item upwards
                if(gwin.cmenu->menutype&(M_VERT|M_PD))
                    citem=goto_item(citem,ITM_UP);
                break;

            case Key_Rgt:

                // if current menu is a pull-down menu,
                // then erase selection bar, free menu records,
                // and return special code to caller
                if(gwin.cmenu->menutype&M_PD) {
                    pre_move(citem);
                    pre_exit(w,YES);
                    gwin.werrno=W_NOERROR;
                    return(M_NEXTPD);
                }

                // if current menu is a horizontal menu, then hide
                // selection bar and find menu item to the right
                if(gwin.cmenu->menutype&M_HORZ)
                    citem=goto_item(citem,ITM_RT);

                // if pulldown flag is set, then select this item
                if(pulldown and (citem->child!=NULL))
                    goto ENTER;
                break;

            case Key_Dwn:

                // if current item has a pull-down menu, select it
                if(citem->fmask&M_HASPD)
                    goto ENTER;

                // if current menu is a vertical menu, then hide
                // selection bar and find menu item downwards
                if(gwin.cmenu->menutype&(M_VERT|M_PD))
                    citem=goto_item(citem,ITM_DN);
                break;

            case Key_End:

                // hide selection bar and find
                // lower-rightmost menu item
                citem=goto_item(citem,ITM_LS);
                break;

            case Key_Ent:

                ENTER:

                // if current menu item has a pull-down menu
                // attached, then set the pulldown flag
                if(citem->fmask&M_HASPD)
                    pulldown=YES;

                // display menu item with selection bar
                disp_item(citem,1);

                menuerr=0;

                // if current menu item has a child menu
                if(citem->child!=NULL) {

                    // save environment info, process child
                    // menu, then restore environment info
                    gwin.cmenu=(_menu_t*)citem->child;
                    hide_mouse_cursor_mnu();
                    err=whelpush();
                    _depth++;
                    menuerr=wmenuget();
                    _depth--;
                    winerr=gwin.werrno;
                    if(!err) whelpop();
                    show_mouse_cursor_mnu();
                    gwin.cmenu=gwin.cmenu->parent;

                    // if an error was returned by
                    // child menu, free menu records
                    // and pass error code to caller
                    if((menuerr==-1) and (winerr!=W_ESCPRESS)) {
                        call_after(citem);
                        menuerr=winerr;
                        pre_exit(w,YES);
                        gwin.werrno=menuerr;
                        return(-1);
                    }
                }

                // if the M_CLOSB feature is on, then close the menu's
                // window before the selection function is called.
                if(citem->fmask&M_CLOSB) close_window(w);

                // this is used as a flag to see if the selection bar's
                // position has been changed by the select function.
                gwin.cmenu->tagcurr=-1;

                // if current menu item has a select function, call it
                if(citem->select!=NULL)
                  mnu_call_func(citem->select);

                // if the M_CLOSB feature is on, then free the current
                // menu a before the selection function is called.
                if(citem->fmask&M_CLOSB) {
                    call_after(citem);
                    _finaltagid = citem->tagid;
                    pre_exit(w,NO);
                    gwin.werrno=W_NOERROR;
                    return _finaltagid;
                }

                // check all menu items in current menu to
                // see if their redisplay flag has been set
                for(item=gwin.cmenu->item;item!=NULL;item=item->prev) {
                    if(item->redisp) {
                        disp_item(item,(item==citem));
                        item->redisp=NO;
                    }
                }

                // see if selection bar position was changed by select
                // function.  if so, then move selection bar to new item
                if(gwin.cmenu->tagcurr!=-1) {
                    item=wmenuifind(gwin.cmenu->tagcurr);
                    if((item!=NULL) and not (item->fmask&M_NOSEL)) {
                        pre_move(citem);
                        post_move(gwin.cmenu->citem=citem=item);
                        break;
                    }
                }

                // if current menu item has a pull-down attached
                if(citem->fmask&M_HASPD) {

                    // if child menu returned previous pull-down menu
                    // flag, find menu item to the left and select it
                    if(menuerr==M_PREVPD) {
                        citem=goto_item(citem,ITM_LT);
                        if(citem->fmask&M_HASPD) goto ENTER;
                        break;
                    }

                    // if child menu returned next pull-down menu
                    // flag, find menu item to the right and select it
                   if(menuerr==M_NEXTPD) {
                        citem=goto_item(citem,ITM_RT);
                        if(citem->fmask&M_HASPD) goto ENTER;
                        break;
                    }
                }

                // turn off pulldown flag
                pulldown=NO;

                // if child menu returned an exit-all-menus flag,
                // then free menu records and pass exit-all-menus
                // flag onto caller
                if((menuerr==M_EXITALL) or (citem->fmask&M_CLALL)) {
                    disp_item(citem,1);
                    call_after(citem);
                    if(citem->fmask&M_CLALL)
                        if(_finaltagid == -1)
                            _finaltagid = citem->tagid;
                    pre_exit(w,YES);
                    gwin.werrno=W_NOERROR;
                    return M_EXITALL;
                }

                // unless an exit-this-menu flag was returned by the
                // child menu, or current item has close-menu
                // specified, free menu records, and return tag
                // identifier of current menu item
                if((citem->child!=NULL) or (citem->select!=NULL))
                    if((menuerr!=M_EXIT) and (not (citem->fmask&M_CLOSE)))
                        break;
                call_after(citem);
                _finaltagid = citem->tagid;
                pre_exit(w,YES);
                gwin.werrno=W_NOERROR;
                return _finaltagid;

            default:

                // separate ASCII code from keypress code, if ASCII
                // code is zero, then it must be an extended key
                ch = char(xch & 0xFF);
                if (!ch and gmnudropthrough)
                {
                    if((xch != Key_PgDn) and (xch != Key_PgUp))
                        kbput(xch);
                    goto ESCAPE_KEY;
                }

                // scan through list of items for one that
                // has a tag identifier matching keypress
                valid=YES;
                item=citem->next;
                for(;;) {
                    while(item!=NULL) {
                        if ((g_toupper(ch)==g_toupper(item->schar)) && !(item->fmask & M_NOSEL))
                        {
                          if (!gwin.menu->hotkey) _overtagid = item->tagid;
                          goto FARBREAK;
                        }
                        if(citem==item) {
                            valid=NO;
                            goto FARBREAK;
                        }
                        item=item->next;
                    }
                    for(item=gwin.cmenu->item; item->prev!=NULL; item=item->prev) {}
                }

                FARBREAK:

                // if a matching tag identifier was found,
                // then hide selection bar, and if quick-key
                // selection is allowed, select the found menu item
                if(valid) {
                    if(item!=citem) {
                        pre_move(citem);
                        post_move(gwin.cmenu->citem=citem=item);
                    }
                    if(!(gwin.cmenu->menutype&M_NOQS)) goto ENTER;
                }
        }
    }
}


//  ------------------------------------------------------------------
//  Starts a menu definition in active window

int wmenubegc() {

  // call wmenubeg() using current window parameters
  if(wmenubeg(gwin.active->srow,
              gwin.active->scol,
              gwin.active->erow,
              gwin.active->ecol,
              gwin.active->btype,
              gwin.active->battr,
              gwin.active->wattr,
              NULL)) {
    return gwin.werrno;
  }

  // turn on use-current-window flag
  gwin.cmenu->usecurr=YES;

  // return normally
  return gwin.werrno=W_NOERROR;
}


//  ------------------------------------------------------------------

static _item_t* search_menu(_menu_t* menu, int tagid) {

  _item_t *witem, *item;

  // do while more items in this menu
  for(witem=menu->item; witem!=NULL; witem=witem->prev) {

    // if tagid of found item matches item we're
    // searching for, then return that item's address
    if(witem->tagid==tagid)
      return witem;

    // if current item has a child menu, search it
    if(witem->child!=NULL) {
      item = search_menu((_menu_t*)witem->child,tagid);
      if(item!=NULL)
        return item;
    }
  }

  // return address of item found
  return witem;
}


//  ------------------------------------------------------------------
//  Finds item record in a menu structure

_item_t* wmenuifind(int tagid) {

  _item_t* item;

  // check for existance of a menu
  if(gwin.cmenu==NULL) {
    gwin.werrno=W_NOMNUDEF;
    return NULL;
  }

  // start search process at root of menu structure
  item = search_menu(gwin.menu,tagid);

  // return to caller
  gwin.werrno = (item==NULL) ? W_NOTFOUND : W_NOERROR;
  return item;
}


//  ------------------------------------------------------------------
//  Adds a text description to menu item

int wmenuitxt(int whdl, int wrow, int wcol, vattr attr, const char* str) {

  // make sure at least 1 menu item has been defined
  if(!gwin.mlevel or gwin.mlevel>gwin.ilevel)
    return gwin.werrno=W_NOITMDEF;

  // add description info to menu record
  _item_t* citem = gwin.cmenu->item;
  citem->dwhdl = whdl;
  citem->dwrow = wrow;
  citem->dwcol = wcol;
  citem->dattr = attr;
  citem->desc  = str;

  // return normally
  return gwin.werrno=W_NOERROR;
}


//  ------------------------------------------------------------------

int wmenuiba(VfvCP before, VfvCP after) {

  // make sure at least 1 menu item has been defined
  if(!gwin.mlevel or gwin.mlevel > gwin.ilevel)
      return(gwin.werrno=W_NOITMDEF);

  // update menu item record
  gwin.cmenu->item->before = before;
  gwin.cmenu->item->after = after;

  // return normally
  return (gwin.werrno=W_NOERROR);
}


//  ------------------------------------------------------------------

int wmenuidsab(int tagid) {

  struct _item_t *item;

  // check for existance of a menu
  if(gwin.cmenu==NULL)
    return (gwin.werrno=W_NOMNUDEF);

  // find address of requested menu item
  if((item=wmenuifind(tagid))==NULL)
    return gwin.werrno;

  // disable menu item by making it nonselectable
  item->fmask |= M_NOSEL;

  // set menu item's redisplay flag
  item->redisp = true;

  // return normally
  return (gwin.werrno=W_NOERROR);
}


//  ------------------------------------------------------------------

int wmenuienab(int tagid) {

  struct _item_t *item;

  // check for existance of a menu
  if(gwin.cmenu==NULL)
    return (gwin.werrno=W_NOMNUDEF);

  // find address of requested menu item
  if((item=wmenuifind(tagid))==NULL)
    return gwin.werrno;

  // enable menu item by making it selectable
  item->fmask &= (~M_NOSEL);

  // set menu item's redisplay flag
  item->redisp = true;

  // return normally
  return (gwin.werrno=W_NOERROR);
}


//  ------------------------------------------------------------------

int wmenuinext(int tagid) {

  //  see if tagid is valid
  if(wmenuifind(tagid)==NULL)
    return gwin.werrno;

  // set current menu's current tagid to input tagid
  wmenumcurr()->tagcurr = tagid;

  // return normally
  return (gwin.werrno=W_NOERROR);
}


//  ------------------------------------------------------------------
