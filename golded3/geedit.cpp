
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
//  The Internal Editor (IE), part 1.
//  ------------------------------------------------------------------

#ifdef __GNUG__
#pragma implementation "geedit.h"
#endif

//  ------------------------------------------------------------------

#include <golded.h>
#include <geedit.h>


//  ------------------------------------------------------------------
//  Globals

int CFG__editquotewrap = YES;

Line* Edit__killbuf  = NULL;
Line* Edit__pastebuf = NULL;

Path Edit__exportfilename = {""};

UndoItem** UndoItem::last_item;


//  ------------------------------------------------------------------

#ifndef NDEBUG

void IEclass::debugtest(char* __test, int __a, int __b, char* __file, int __line, int __values) {

  #if defined(GFTRK_ENABLE)
  int _tmp = __gftrk_on;
  __gftrk_on = false;
  #endif

  savefile(MODE_UPDATE);

  #if defined(GFTRK_ENABLE)
  __gftrk_on = _tmp;
  #endif

  if(__values)
    w_infof(" Range check: (%s) <%i,%i> [%s,%u] ", __test, __a, __b, __file, __line);
  else
    w_infof(" Range check: (%s) [%s,%u] ", __test, __file, __line);
  update_statusline(LNG->EscOrContinue);
  SayBibi();
  while(kbxhit())
    kbxget();
  gkey _key = kbxget();
  w_info(NULL);
  if(_key == Key_Esc) {
    LOG.errtest(__file, __line);
    LOG.printf("! An internal editor range check failed.");
    if(__values)
      LOG.printf(": Details: (%s) <%i,%i>.", __test, __a, __b);
    else
      LOG.printf(": Details: (%s).", __test);
      LOG.printf(": Details: r%u,c%u,mr%u,mc%u,i%u,dm%u,qm%u,eqm%u.",
      row, col, maxrow, maxcol, insert,
      CFG->dispmargin, CFG->quotemargin, EDIT->QuoteMargin()
    );
    LOG.printf("+ Advice: Report to the Author.");
    TestErrorExit();
  }
}

#endif


//  ------------------------------------------------------------------
//  Make sure line type is correct

void IEclass::setlinetype(Line* __line) {

  _test_halt(__line == NULL);

  __line->type &= ~GLINE_ALL;
  __line->type |= is_quote(__line->txt.c_str()) ? GLINE_QUOT : (__line->txt[0] == CTRL_A) ? GLINE_HIDD : 0;
}


//  ------------------------------------------------------------------
//  Insert character in string at position

//  ------------------------------------------------------------------
//  Zero-based

int IEclass::dispchar(vchar __ch, int attr) {

  if(__ch != '\n') {
    if(__ch == ' ')
      __ch = EDIT->CharSpace();
  }
  else {
    __ch = EDIT->CharPara();
  }

  int atr;
  vchar chr;
  editwin.getc(crow, ccol, &atr, &chr);
  editwin.printc(crow, ccol, attr == -1 ? atr : attr, __ch);
  return atr;
}


//  ------------------------------------------------------------------

void IEclass::cursoroff() { 

  vcurhide();
}


//  ------------------------------------------------------------------

void IEclass::cursoron() { 

  vcurshow();
}


//  ------------------------------------------------------------------
//  Zero-based

void IEclass::scrollup(int __scol, int __srow, int __ecol, int __erow, int __lines) {

  editwin.scroll_box_up(__srow, __scol, __erow, __ecol, __lines);
}


//  ------------------------------------------------------------------
//  Zero-based

void IEclass::scrolldown(int __scol, int __srow, int __ecol, int __erow, int __lines) {

  editwin.scroll_box_down(__srow, __scol, __erow, __ecol, __lines);
}


//  ------------------------------------------------------------------
//  Zero-based

void IEclass::prints(int wrow, int wcol, int atr, char* str) {

  editwin.prints(wrow, wcol, atr, str);
}


//  ------------------------------------------------------------------

Line* IEclass::findfirstline() {

  GFTRK("Editfindfirstline");

  if(not currline)
    return NULL;

  // Rewind to the first line
  Line* _firstline = currline;
  while(_firstline->prev)
    _firstline = _firstline->prev;

  GFTRK(NULL);

  return _firstline;
}


//  ------------------------------------------------------------------
//  Find out what number the current line is and put it in "thisrow"

void IEclass::getthisrow(Line* __currline) {

  GFTRK("Editgetthisrow");

  Line* _templine = findfirstline();

  thisrow = 0;
  while((_templine != __currline) and _templine->next) {
    _templine = _templine->next;
    thisrow++;
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------
//  Zero-based

void IEclass::gotorowcol(uint __col, uint __row) {

  GFTRK("Editgotorowcol");

  _test_haltab(__col > maxcol, __col, maxcol);
  _test_haltab(__row > maxrow, __row, maxrow);

  editwin.move_cursor(__row, __col);

  ccol = __col;
  crow = __row;

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::dispstring(const char* __string, uint __row, int attr, Line* line) {

  GFTRK("Editdispstring");

  _test_halt(__string == NULL);
  _test_haltab(__row > maxrow, __row, maxrow);

  // Get string length
  uint _length = strlen(__string);

  // String longer than window width?
  _test_haltab(_length > (maxcol+1), _length, (maxcol+1));
  _length = MinV(_length, (maxcol+1));

  // Buffer for translation to visual representation
  char _buf[EDIT_BUFLEN];

  // Space-pad and nul-terminate the buffer
  memset(_buf, ' ', maxcol+1);
  _buf[maxcol+1] = NUL;

  // Copy/translate string into buffer
  if(attr == -1) {
    char* _bufptr = _buf;
    uint _position = 0;
    const char* __str = __string;
    while(_position < _length) {
      switch(*__str) {
        case ' ':   *_bufptr = EDIT->CharSpace();  break;
        case '\n':  *_bufptr = EDIT->CharPara();   break;
        default:    *_bufptr = *__str;
      }
      _position++;
      _bufptr++;
      __str++;
    }
  }
  else {
    memcpy(_buf, __string, _length);
  }

  // mark selected block
  if(line and (blockcol != -1)) {
    int selected = 0;
    for(Line *ln = findfirstline(); ln and ln != line; ln = ln->next) {
      if(ln == currline)
        selected ^= 1;
      if(ln->type & GLINE_BLOK)
        selected ^= 1;
    }
    if((line->type & GLINE_BLOK) and (line == currline)) {
      int begblock = ((col < blockcol) ? col : blockcol) - mincol;
      int endblock = ((col > blockcol) ? col : blockcol) - mincol;

      char savechar = _buf[begblock];
      _buf[begblock] = NUL;
      StyleCodeHighlight(_buf, __row, mincol, false, attr);
      _buf[begblock] = savechar;

      savechar = _buf[endblock];
      _buf[endblock] = NUL;
      StyleCodeHighlight(_buf+begblock, __row, mincol+begblock, false, C_READA);
      _buf[endblock] = savechar;
      StyleCodeHighlight(_buf+endblock, __row, mincol+endblock, false, attr);
    }
    else if((line->type & GLINE_BLOK) or (line == currline)) {
      int blockmark = ((line->type & GLINE_BLOK) ? blockcol : col) - mincol;

      char savechar = _buf[blockmark];
      _buf[blockmark] = NUL;
      StyleCodeHighlight(_buf, __row, mincol, false, selected ? C_READA : attr);
      _buf[blockmark] = savechar;
      StyleCodeHighlight(_buf+blockmark, __row, mincol+blockmark, false, selected ? attr : C_READA);
    }
    else
      StyleCodeHighlight(_buf, __row, mincol, false, selected ? C_READA : attr);
  }
  else
    StyleCodeHighlight(_buf, __row, mincol, false, attr);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::setcolor(Line* __line) {

  // Set window attribute
  if(__line->type & GLINE_HIDD)
    editwin.text_color(C_READKH);
  else if(__line->type & GLINE_KLUD)
    editwin.text_color(C_READK);
  else if(__line->type & GLINE_TAGL)
    editwin.text_color(C_READG);
  else if(__line->type & GLINE_TEAR)
    editwin.text_color(C_READT);
  else if(__line->type & GLINE_ORIG)
    editwin.text_color(C_READO);
  else if(__line->type & GLINE_QUOT)
    editwin.text_color(quotecolor(__line->txt.c_str()));
  else if(__line->type & GLINE_SIGN)
    editwin.text_color(C_READS);
  else
    editwin.text_color(C_READW);

}


//  ------------------------------------------------------------------
//  Zero-based

void IEclass::displine(Line* __line, uint __row) {

  GFTRK("Editdispline");

  _test_halt(__line == NULL);
  _test_haltab(__row > maxrow, __row, maxrow);

  // Display line
  setcolor(__line);
  dispstring(__line->txt.c_str(), __row, -1, __line);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------
//  Zero-based

void IEclass::clreol(int __col, int __row) {

  GFTRK("Editclreol");

  if(__col == -1)
    __col = ccol;

  if(__row == -1)
    __row = crow;

  if((uint)__col <= maxcol)
    editwin.fill(__row, __col, __row, maxcol, ' ', C_READW);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------
//  Zero-based

void IEclass::refresh(Line* __currline, uint __row) {

  GFTRK("Editrefresh");

  _test_halt(__currline == NULL);

  cursoroff();

  // Display as many lines as we can
  while(__currline and (__row <= maxrow)) {
    displine(__currline, __row++);
    __currline = __currline->next;
  }

  // If we ran out of lines, blank the rest
  if(__row <= maxrow) {
    vchar vbuf[256];
    for(int c = 0; c < maxcol+1; c++)
      vbuf[c] = _box_table(W_BREAD, 1);
    vbuf[maxcol+1] = NUL;
    wprintvs(__row++, mincol, C_READB|ACSET, vbuf);
    while(__row <= maxrow)
      dispstring("", __row++);
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

Line* IEclass::insertlinebelow(Line* __currline, const char* __text, long __batch_mode) {

  GFTRK("Editinsertlinebelow");

  Line* _nextline = new Line(__text ? __text : "");
  throw_xnew(_nextline);

  if(__currline) {

    _nextline->prev = __currline;
    _nextline->next = __currline->next;
    if(_nextline->next)
      _nextline->next->prev = _nextline;
    __currline->next = _nextline;
  }
  
  Undo->PushItem(EDIT_UNDO_NEW_LINE|batch_mode|__batch_mode, _nextline);

  GFTRK(NULL);

  return _nextline;
}


//  ------------------------------------------------------------------
//  Zero-based

int IEclass::downoneline(uint __row) {

  GFTRK("Editdownoneline");

  _test_haltab(__row > maxrow, __row, maxrow);

  thisrow++;

  if(__row == maxrow)
    scrollup(mincol, minrow, maxcol, maxrow);
  else
    __row++;

  gotorowcol(mincol, __row);

  GFTRK(NULL);

  return __row;
}


//  ------------------------------------------------------------------

void IEclass::GoEOL() {

  GFTRK("EditGoEOL");

  // Move cursor to the last char on the line
  col = currline->txt.length();
  if(col)
    col--;

  // String must not be longer than the window width
  _test_haltab(col > maxcol, col, maxcol);

  if(blockcol != -1)
    displine(currline, row);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::GoUp() {

  GFTRK("EditGoUp");

  _test_haltab(row < minrow, row, minrow);

  if(currline->prev) {

    currline = currline->prev;
    thisrow--;

    if(row == minrow) {
      scrolldown(mincol, row, maxcol, maxrow);
      if(blockcol != -1)
        displine(currline->next, row+1);
      displine(currline, row);
    }
    else {
      row--;
      if(blockcol != -1) {
        displine(currline->next, row+1);
        displine(currline, row);
      }
    }

    if((col+1) > currline->txt.length())
      GoEOL();
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::GoDown() {

  GFTRK("EditGoDown");

  _test_haltab(row > maxrow, row, maxrow);

  if(currline->next) {

    currline = currline->next;
    thisrow++;

    if(row == maxrow) {
      scrollup(mincol, minrow, maxcol, maxrow);
      if(blockcol != -1)
        displine(currline->prev, row-1);
      displine(currline,row);
    }
    else {
      row++;
      if(blockcol != -1) {
        displine(currline->prev, row-1);
        displine(currline, row);
      }
    }

    if((col+1) > currline->txt.length())
      GoEOL();
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::GoLeft() {

  GFTRK("EditGoLeft");

  _test_haltab(col < mincol, col, mincol);

  if(col == mincol) {
    if(currline->prev) {
      GoUp();
      GoEOL();
    }
  }
  else
    col--;

  if(blockcol != -1)
    displine(currline, row);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::GoRight() {

  GFTRK("EditGoRight");

  _test_haltab(col > maxcol, col, maxcol);

  char _cursorchar = currline->txt[col];

  if((col == maxcol) or (_cursorchar == '\n') or (_cursorchar == NUL)) {
    if(currline->next != NULL) {
      GoDown();
      col = mincol;
    }
  }
  else
    col++;

  if(blockcol != -1)
    displine(currline, row);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

Line* IEclass::wrapit(Line** __currline, uint* __curr_col, uint* __curr_row, int __display) {

  _test_halt(__currline == NULL);
  _test_halt(*__currline == NULL);

  uint  _quotelen;
  char  _quotebuf[100];

  uint  _curscol = *__curr_col;
  uint  _cursrow = *__curr_row;

  uint  _thisrow = *__curr_row;
  Line* _thisline = *__currline;
  Line* _lastadded = _thisline;

  bool _wrapped_above = false;

  // Start wrapping from the current line onwards
  while(_thisline) {

    // Length of this line
    uint _thislen = _thisline->txt.length();

    uint _wrapmargin = (_thisline->type & GLINE_QUOT) ? marginquotes : margintext;

    // Does this line need wrapping?
    if(_thislen > _wrapmargin or (_thislen == _wrapmargin and not isspace(_thisline->txt[_thislen-1]))) {

      // Reset quote string length
      _quotelen = 0;

      // Is this line quoted?
      if((_thisline->type & GLINE_QUOT) and CFG__editquotewrap) {

        // Get quote string and length
        GetQuotestr(_thisline->txt.c_str(), _quotebuf, &_quotelen);
      }

      // wrapmargin = 40
      //                   1111111111222222222233333333334444444444
      //         01234567890123456789012345678901234567890123456789
      //         --------------------------------------------------
      //                                                v- wrapptr
      // Case 1: this is another test with a bit of text to wrap.
      // Case 2: this is a test with a line that need wrapping.
      // Case 3: thisxisxaxtestxwithxaxlinexthatxneedxwrapping.
      // Case 4: >thisxisxaxtestxwithxaxlinexthatxneedxwrapping.

      // Point to the last char inside the margin
      int _wrappos = _wrapmargin - 1;

      // Locate the word to be wrapped

      // Did we find a space?
      if(_thisline->txt[_wrappos] == ' ') {

        // Case 1: A space was found as the last char inside the margin
        //
        // Now we must locate the first word outside the margin.
        // NOTE: Leading spaces to this word will be nulled out!

        // Begin at the first char outside the margin
        _wrappos++;
      }
      else {

        // Case 2: A non-space was found as the last char inside the margin
        //
        // Now we must locate the beginning of the word we found.

        // Keep copy of original pointer
        int _atmargin = _wrappos;

        // Search backwards until a space or the beginning of the line is found
        while(_wrappos > 0 and (_thisline->txt[_wrappos-1] != ' '))
          _wrappos--;

        // Check if we hit leading spaces
        int _spacepos = _wrappos;
        if(_spacepos > 0) {
          do {
            _spacepos--;
          } while(_spacepos > 0 and _thisline->txt[_spacepos] == ' ');
        }

        // Did we search all the way back to the beginning of the line?
        if(_wrappos == 0 or _wrappos == _quotelen or _thisline->txt[_spacepos] == ' ') {

          // Case 3: There are no spaces within the margin or we hit leading spaces

          // We have to break it up at the margin
          _wrappos = _atmargin;
        }
      }

      // The wrapptr now points to the location to be wrapped or NUL

      // Get length of the wrapped part
      uint _wraplen = _thisline->txt.length() - _wrappos;

      // Is the line hard-terminated?
      if(_thisline->txt.find('\n', _wrappos) != _thisline->txt.npos) {

        // The line is hard-terminated.
        //
        // The wrapped part must be placed on a new line below.

        Line* _wrapline = _lastadded = insertlinebelow(_thisline, NULL, BATCH_MODE);

        // Copy the quote string, if any, to the new line first
        if(_quotelen)
          _wrapline->txt = _quotebuf;
        else
          _wrapline->txt = "";

        // Copy/append the wrapped part to the new line
        _wrapline->txt += _thisline->txt.substr(_wrappos);

        // Saves pointer to a line where from the wrapped part was copied, its begining
        // and length. While in Undo, appends the copied part to previous line and deletes
        // it on current, moving the rest over deleted.
        Undo->PushItem(EDIT_UNDO_WRAP_TEXT|BATCH_MODE, _wrapline, _quotelen, _wraplen);
        
        _wrapline->type = _thisline->type;
        // Make sure the type of the line is correct
        setlinetype(_wrapline);

        if(__display) {

          // Is there at least one line below this one?
          if(_thisrow < maxrow) {

            // Scroll the lines below to make room for the new line
            scrolldown(mincol, _thisrow+1, maxcol, maxrow);

            // Display the new line
            if(_wrapline->txt.length() <= (maxcol+1))
              displine(_wrapline, _thisrow+1);
          }
        }
      }
      else {

        // The line is not hard-terminated
        //
        // The wrapped part must be inserted into the next line

        // Indicate that one or more lines were wrapped in this way
        _wrapped_above = true;

        // Pointer to the next line
        Line* _nextline = _thisline->next;

        // Flag to indicate if a new line was added below
        bool _line_added_below = false;

        // Is there no next line or is the next line quoted?
        if((_nextline == NULL) or (_nextline->type & GLINE_QUOT)) {

          // The wrapped part must be placed on a new line below
          _lastadded = _nextline = insertlinebelow(_thisline, "", BATCH_MODE);
          _line_added_below = true;
        }

        // Was this line quoted?
        if(_quotelen) {

          // Copy the quote string
          _nextline->txt.insert(0, _quotebuf, _quotelen);
        }

        _nextline->txt.insert(_quotelen, _thisline->txt.substr(_wrappos));

        Undo->PushItem(EDIT_UNDO_WRAP_TEXT|BATCH_MODE, _nextline, 0, _quotelen+_wraplen);

        // Make sure the type of the line is correct
        setlinetype(_nextline);

        if(__display) {

          // Is there at least one line below this one?
          if(_line_added_below and (_thisrow < maxrow)) {

            // Scroll the lines below to make room for the new line
            scrolldown(mincol, _thisrow+1, maxcol, maxrow);
          }

          // Display the new/wrapped line
          if((_thisrow+1) <= maxrow and _nextline->txt.length() <= (maxcol+1))
            displine(_nextline, _thisrow+1);
        }
      }

      // Truncate at the wrapping location
      _thisline->txt.erase(_wrappos);

      // Was this line quoted?
      if(_quotelen) {

        // Trim spaces off the end of the line
        int _trimpos = _wrappos - 1;
        if(_thisline->txt[_trimpos] == ' ') {
          while(_trimpos > 0 and _thisline->txt[_trimpos-1] == ' ')
            _trimpos--;
          if(_quotelen and _trimpos < _quotelen)
            _trimpos++;
          Undo->PushItem(EDIT_UNDO_OVR_CHAR|BATCH_MODE, _thisline, _trimpos);
          Undo->PushItem(EDIT_UNDO_OVR_CHAR|BATCH_MODE, _thisline, _trimpos+1);
          _thisline->txt.erase(_trimpos);
        }
        else
          Undo->PushItem(EDIT_UNDO_OVR_CHAR|BATCH_MODE, _thisline, _wrappos);

        // Append a new linefeed
        _thisline->txt += "\n";
      }

      // Make sure the line type still is correct
      setlinetype(_thisline);

      if(__display) {

        // Display this line after wrapping
        if(_thisrow <= maxrow)
          displine(_thisline, _thisrow);
      }

      // If we are on the cursor line, check if the cursor char was wrapped
      if(_thisrow == *__curr_row and _thisline->txt.length() <= *__curr_col) {
        _curscol = _quotelen + ((*__curr_col > _wrappos) ? *__curr_col-_wrappos : 0);
        _cursrow++, thisrow++;
        UndoItem* i = Undo->last_item;
        do { i = i->prev; } while(i->action & BATCH_MODE);
        if(i->col.num >= i->line->txt.length()) {
          i->action |= PREV_LINE;
          i->col.sav = i->col.num;
          i->col.num = _curscol;
        }
      }
    }

    // If this line is hard-terminated, we have finished wrapping
    // Unless the next line has grown too large
    if(_thisline->txt.find('\n') != _thisline->txt.npos) {
      if(_thisline->next == NULL)
        break;
      _wrapmargin = (_thisline->next->type & GLINE_QUOT) ? marginquotes : margintext;
      if(_thisline->next->txt.length() <= _wrapmargin)
        break;
    }

    // Go to the next line
    _thisline = _thisline->next;
    _thisrow++;
  }

  if(__display) {

    // Display the current line after wrapping
    if(*__curr_row <= maxrow)
      displine(*__currline, *__curr_row);

    // Was the line or lines above wrapped?
    if(_wrapped_above) {

      // Display the last line in the paragraph
      if((_thisrow <= maxrow) and _thisline)
        displine(_thisline, _thisrow);
    }
  }

  // Move to the next line if the cursor was wrapped
  if(_cursrow != *__curr_row) {
    *__currline = (*__currline)->next;
    if(_cursrow > maxrow) {
      _cursrow = maxrow;
      scrollup(mincol, minrow, maxcol, maxrow);
      displine(*__currline, row);
    }
  }

  // Update cursor position
  *__curr_row = _cursrow;
  *__curr_col = _curscol;
  //thisrow = _cursrow;

  GFTRK(NULL);

  return _lastadded;
}


//  ------------------------------------------------------------------

Line* IEclass::wrapdel(Line** __currline, uint* __curr_col, uint* __curr_row, int __display) {

  GFTRK("Editwrapdel");

  Line *tmp = wrapit(__currline, __curr_col, __curr_row, __display);

  GFTRK(NULL);

  return tmp;
}


//  ------------------------------------------------------------------

Line* IEclass::wrapins(Line** __currline, uint* __curr_col, uint* __curr_row, int __display) {

  GFTRK("Editwrapins");

  Line *tmp = wrapit(__currline, __curr_col, __curr_row, __display);

  GFTRK(NULL);

  return tmp;
}


//  ------------------------------------------------------------------

void IEclass::insertchar(char __ch) {

  GFTRK("Editinsertchar");

#ifndef NDEBUG
  uint _currline_len = currline->txt.length();
  _test_haltab(col > _currline_len, col, _currline_len);
#endif
  // Insert or overwrite the char, replacing the block if any
  if((selecting ? (BlockCut(true), batch_mode = BATCH_MODE) : false) or
     (currline->txt[col] == '\n') or (currline->txt[col] == NUL) or insert) {
    Undo->PushItem(EDIT_UNDO_INS_CHAR|batch_mode);
    if(col == currline->txt.length() and __ch != ' ' and __ch != '\n')
      currline->txt += ' ';
    currline->txt.insert(col, 1, __ch);
  } else {
    Undo->PushItem(EDIT_UNDO_OVR_CHAR|batch_mode);
    currline->txt[col] = __ch;
  }
  batch_mode = BATCH_MODE;

  // Make sure the line type still is correct
  setlinetype(currline);

  // Move cursor
  col++;

  wrapins(&currline, &col, &row);

  // Adjust cursor position and display if necessary
  if(col > maxcol) {
    if(currline->next) {
      currline = currline->next;
      col = mincol;
      row++;
      if(row > maxrow) {
        row = maxrow;
        scrollup(mincol, minrow, maxcol, maxrow);
        displine(currline, row);
      }
    }
    else {
      col = maxcol;
    }
  }

  gotorowcol(col, row);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::DelChar() {

  GFTRK("EditDelChar");

  Line* _thisline = currline;
  Line* _nextline = currline->next;
  uint  _thislen = _thisline->txt.length();

  // Cannot delete at or beyond the nul-terminator
  if(col < _thislen) {
    Undo->PushItem(EDIT_UNDO_DEL_CHAR|batch_mode);
    _thisline->txt.erase(col, 1);
    batch_mode = BATCH_MODE;
  }

  // Did we delete the last char on the line or
  // was the cursor at or beyond the nul-terminator?
  // And is there a next line at all?
  if(((col+1) >= _thislen) and _nextline) {

    // Join the next line to this line

    // Is the next line quoted?
    // And is the cursor column non-zero?
    uint _quotelen = 0;
    if((_nextline->type & GLINE_QUOT) and CFG__editquotewrap and col) {

      // Get quote string length
      char _dummybuf[100];
      GetQuotestr(_nextline->txt.c_str(), _dummybuf, &_quotelen);
    }

    // Copy the next line's text to this line
    // Skip past the next line's quote string and blanks, if any
    const char* _nextptr = _nextline->txt.c_str()+_quotelen;
    if(not ((_nextline->type & GLINE_QUOT) and (col == 0))) {
      while(*_nextptr == ' ')
        _nextptr++;
    }
    _thisline->txt += _nextptr;

    Undo->PushItem(EDIT_UNDO_CUT_TEXT|batch_mode, _thisline, col);

    // Relink this line
    _thisline->next = _nextline->next;
    if(_thisline->next)
      _thisline->next->prev = _thisline;

    Undo->PushItem(EDIT_UNDO_DEL_LINE|BATCH_MODE, _nextline);
  }
  batch_mode = BATCH_MODE;

  // Make sure the line type still is correct
  setlinetype(_thisline);

  // Rewrap this line
  wrapdel(&currline, &col, &row);

  refresh(currline, row);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::DelLeft() {

  GFTRK("EditDelLeft");

  // Cannot backspace from the first column on the first line in the msg
  if(currline->prev == NULL)
    if(col == mincol) {
      GFTRK(NULL);
      return;
    }

  // Go left(/up) and delete the character there
  GoLeft();
  DelChar();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::GoWordLeft() {

  GFTRK("EditGoWordLeft");

  if(col == 0) {
    if(currline->prev) {
      GoUp();
      GoEOL();
    }
  }
  else {
 
    col--;
 
    if(not isxalnum(currline->txt[col])) {
      while(not isxalnum(currline->txt[col]) and (col > 0))
        col--;
      while(isxalnum(currline->txt[col]) and (col > 0))
        col--;
    }
    else {
      while(isxalnum(currline->txt[col]) and (col > 0))
        col--;
    }

    if(col != 0)
      col++;

    if(blockcol != -1)
      displine(currline, row);
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::GoWordRight() {

  GFTRK("EditGoWordRight");

  if(currline->txt.length() == col or currline->txt[col] == '\n') {
    if(currline->next) {
      GoDown();
      col = 0;
    }
   }
   else {
    if(not isxalnum(currline->txt[col])) {
      while(not isxalnum(currline->txt[col]) and ((col+1) <= currline->txt.length()))
        col++;
    }
    else {
      while(isxalnum(currline->txt[col]) and ((col+1) <= currline->txt.length()))
        col++;
      while(not isxalnum(currline->txt[col]) and ((col+1) <= currline->txt.length()))
        col++;
    }

    if(currline->txt[col-1] == '\n')
       col--;

    if(currline->txt.length() == col) {
      if(currline->next) {
        GoDown();
        col = 0;
      }
      else
        col--;
    }

    if(blockcol != -1)
      displine(currline, row);
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::Newline() {

  GFTRK("EditNewline");

  // Pointer to the split position
  int _splitpos = col;

  // Buffer for the second part of the split line
  char* _splitbuf = (char*)throw_malloc(EDIT_BUFLEN);
  *_splitbuf = NUL;

  // If the split line was quoted, get the quotestring
  // But do not get it if the cursor points to a linefeed or is
  uint _quotelen = 0;
  if(is_quote(currline->txt.c_str())) {
    GetQuotestr(currline->txt.c_str(), _splitbuf, &_quotelen);
    THROW_CHECKPTR(_splitbuf);
  }

  // Eliminate the quotestring if
  // - the cursor points to a linefeed or
  // - the cursor is located inside the quotestring
  if(_quotelen and ((currline->txt.length() == col) or (currline->txt[_splitpos] == '\n') or (col < _quotelen)))
    *_splitbuf = _quotelen = 0;

  // Append the second part to the split buffer
  strcat(_splitbuf, currline->txt.substr(_splitpos).c_str());
  THROW_CHECKPTR(_splitbuf);

  Undo->PushItem(EDIT_UNDO_INS_TEXT|batch_mode, currline, col, 1);
  batch_mode = BATCH_MODE;
  
  // Copy linefeed+nul to the split position
  currline->txt.erase(_splitpos);
  currline->txt += "\n";

  // Re-type and display the split line
  setlinetype(currline);
  displine(currline, row);

  // Insert a new line below, set the line text to the split-off part
  currline = insertlinebelow(currline, _splitbuf);

  //                  --v--
  // This line would be wrapped
  // This line would be
  // wrapped

  Undo->PushItem(EDIT_UNDO_WRAP_TEXT|BATCH_MODE, currline, _quotelen, strlen(_splitbuf) - _quotelen);

  setlinetype(currline);
  throw_free(_splitbuf);

  // Move down the cursor
  col = 0;
  row = downoneline(row);

  // Scroll the remaining lines if necessary
  if(row < maxrow)
    scrolldown(mincol, row, maxcol, maxrow);

  // Rewrap the split-off line
  wrapdel(&currline, &col, &row);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::CopyAboveChar() {

  GFTRK("EditCopyAboveChar");

  char _ch = ' ';
  if(currline->prev) {
    uint _len = currline->prev->txt.length();
    if(_len and _len > col)
      _ch = currline->prev->txt[col];
    if(_ch == '\n')
      _ch = ' ';
  }
  insertchar(_ch);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::DupLine() {

  GFTRK("EditDupLine");

  Undo->PushItem(EDIT_UNDO_VOID);
  
  Line* _nextline = insertlinebelow(currline, currline->txt.c_str(), BATCH_MODE);
  _nextline->type   = currline->type & ~GLINE_BLOK;
  _nextline->color  = currline->color;
  _nextline->kludge = currline->kludge;
  refresh(currline, row);
  GoDown();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------
//  PageUp behavior:
//
//  ... The top line becomes the bottom line.
//  ... Always remain at cursor row, except if we can't page up.
//  ... If we can't page up, move the cursor row to the top line.

void IEclass::GoPgUp() {

  GFTRK("EditGoPgUp");

  // Not at the first line in msg?
  if(currline->prev) {

    // Count lines
    int _count = row;

    // Move to the top line currently displayed
    Line* _topline = currline;
    while(_count and _topline->prev) {
      _topline = _topline->prev;
      _count--;
    }

    // The count must be zero at this point!
    _test_haltab(_count, _count, _count);

    // At we already fully at the top?
    if(_topline->prev == NULL) {

      // Yes, so move the cursor row to the top line
      row = minrow;
      currline = _topline;
      if(blockcol != -1)
        refresh(currline, row);
    }
    else {

      // We are not at the top, so continue with paging

      // Move a full page of lines, if possible
      _count = maxrow;
      while(_count and _topline->prev) {
        _topline = _topline->prev;
        _count--;
      }

      // Set the current line
      _count = row;
      currline = _topline;
      while(_count--)
        currline = currline->next;

      // Refresh display
      refresh(_topline, minrow);
    }
  }
  else {
    GoTopMsg();
  }

  if(col+1 > currline->txt.length())
    GoEOL();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------
//  PageDown behavior:
//
//  ... The bottom line becomes the top line.
//  ... Always remain at cursor row, except if at the last line.
//  ... If at the last line, move cursor to the last window line and
//      display the lines above.
//  ... If there are too few lines to display after the page down, the
//      rest are displayed blank.

void IEclass::GoPgDn() {

  GFTRK("EditGoPgDn");

  // Not at the last line in the msg?
  if(currline->next) {

    // Go down to the last displayed line in the window
    uint _newrow = row, _oldrow = row;
    Line *oldcurrline = currline;
    while((_newrow < maxrow) and currline->next) {
      currline = currline->next;
      _newrow++;
    }

    // If there are more lines after the last line, start displaying from the top
    if(currline->next) {
      Line *_topline = currline;

      // Set current line
      _newrow = 0;
      while((_newrow < row) and currline->next) {
        currline = currline->next;
        _newrow++;
      }

      // Move cursor row if necessary
      if(_newrow < row)
        row = _newrow;

      refresh(_topline, minrow);
    }
    else {
      row = _newrow;
      refresh(oldcurrline, _oldrow);
    }
  }
  else {
    GoBotMsg();
  }

  if(col+1 > currline->txt.length())
    GoEOL();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::Tab() {

  GFTRK("EditTab");

  int tabsz = CFG->disptabsize ? CFG->disptabsize : 1;

  // Move to the next tab position
  do {
    if(insert)
      insertchar(' ');
    else if(currline->txt[col] != '\n')
      GoRight();
    else
      break;
  } while(col % tabsz);


  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::ReTab() {

  GFTRK("EditTabReverse")

  int tabsz = CFG->disptabsize ? CFG->disptabsize : 1;

  // Move to the next tab position
  do {
    if(not col)
      break;

    if(insert)
      DelLeft();
    else
      GoLeft();
  } while(col % tabsz);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::DeleteEOL() {

  GFTRK("EditDeleteEOL");

  bool _has_linefeed = currline->txt.find('\n') != currline->txt.npos ? true : false;
 
  Undo->PushItem(EDIT_UNDO_DEL_TEXT, currline);

  currline->txt.erase(col);

  if(_has_linefeed)
    currline->txt += "\n";

  clreol();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::deleteline(bool zapquotesbelow) {

  GFTRK("Editdeleteline");

  bool done = false;

  do {
 
    // Break if need to zap quotes, but the current line is not quote and is not empty
    if(zapquotesbelow and not ((currline->type & GLINE_QUOT) or strblank(currline->txt.c_str())))
      break;

    // Pointer to the deleted line
    Line* _deletedline = currline;

    // If last line to be deleted delete to EOL and exit
    if(currline->next == NULL) {
      if(currline->txt.empty())
        break;
      insertlinebelow(currline, "", batch_mode);
      batch_mode = BATCH_MODE;
      done = true;
    }

    // Pointers to the previous and next lines
    Line* _prevline = currline->prev;
    Line* _nextline = currline->next;

    Undo->PushItem(EDIT_UNDO_PUSH_LINE|batch_mode);

    // Set the new current line to the next line (which may be NULL!)
    currline = _nextline;

    if(currline == NULL) {
      currline = _prevline;
      currline->next = NULL;
      _prevline = _prevline ? _prevline->prev : NULL;
    }

    // Link the new current line to the previous line
    currline->prev = _prevline;

    // Link the previous line to this line
    if(_prevline)
      _prevline->next = currline;

    if(_deletedline->type & GLINE_BLOK) {
      blockcol = -1;
      _deletedline->type &= ~GLINE_BLOK;
    }

    // Link the deleted line to the killbuffer
    if(Edit__killbuf) {
      Edit__killbuf->next = _deletedline;
      _deletedline->prev = Edit__killbuf;
      Edit__killbuf = _deletedline;
    }
    else {
      Edit__killbuf = _deletedline;
      Edit__killbuf->prev = NULL;
    }
    Edit__killbuf->next = NULL;

    // Move the cursor to EOL if necessary
    if(col+1 > currline->txt.length())
      GoEOL();

    if(not zapquotesbelow)
      break;

    batch_mode = BATCH_MODE;

  } while(not done);

  // Refresh display from cursor row
  refresh(currline, row);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::UnDelete(bool before) {

  GFTRK("EditUnDelete");

  // If there are deleted lines
  if(Edit__killbuf) {

    Line* _prevline = Edit__killbuf->prev;
    bool down = false;

    if(before) {
      Edit__killbuf->prev = currline ? currline->prev : NULL;
      Edit__killbuf->next = currline;
    }
    else {
      Edit__killbuf->prev = currline;
      Edit__killbuf->next = currline ? currline->next : NULL;
      if(row == maxrow)
        down = true;
      else if((row < maxrow) and currline)
        row++;
    }

    if(Edit__killbuf->prev)
      Edit__killbuf->prev->next = Edit__killbuf;
    if(Edit__killbuf->next)
      Edit__killbuf->next->prev = Edit__killbuf;
    currline = Edit__killbuf;
    Edit__killbuf = _prevline;
    if(Edit__killbuf)
      Edit__killbuf->next = NULL;

    Undo->PushItem(EDIT_UNDO_POP_LINE);

    // Move the cursor to EOL if necessary
    if(col+1 > currline->txt.length())
      GoEOL();

    if(down)
      GoDown();
    else
      refresh(currline, row);
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::ZapQuoteBelow() {

  GFTRK("ZapQuoteBelow");

  if(currline->prev)
    Undo->PushItem(EDIT_UNDO_VOID|PREV_LINE|batch_mode, currline->prev);
  else
    Undo->PushItem(EDIT_UNDO_VOID|batch_mode, currline);
  batch_mode = BATCH_MODE;

  deleteline(true);
  if(row) {
    GoUp();
    GoEOL();
    Newline();
  }
  else {
    UndoItem* i = Undo->last_item;
    do { i = i->prev; } while(i->action & BATCH_MODE);
    i->line = currline;
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

Line* IEclass::findtopline() {

  GFTRK("Editfindtopline");

  uint  _toprow  = row;
  Line* _topline = currline;

  while(_topline->prev and (_toprow > minrow)) {
    _topline = _topline->prev;
    _toprow--;
  }

  GFTRK(NULL);

  return _topline;
}


//  ------------------------------------------------------------------

void IEclass::savefile(int __status) {

  Subj statbak;

  GFTRK("Editsavefile");

  // Turn off cursor and put up a wait window
  int wascursoron = not vcurhidden();
  cursoroff();

  strcpy(statbak, information);
  update_statusline(LNG->Wait+1);

  // Open the save file
  const char* editorfile = AddPath(CFG->goldpath, EDIT->File());
  remove(editorfile);
  FILE* _fp = fsopen(editorfile, "wb", CFG->sharemode);
  if(_fp) {

    // Find the first line
    Line* _saveline = findfirstline();

    // First save the "unfinished" identifier
    if(__status == MODE_UPDATE) {
      fputs(unfinished, _fp);
      fputs("\r\n", _fp);
    }

    // Save as whole paragraphs
    while(_saveline) {

      // Copy the line to a buffer
      char _buf[EDIT_BUFLEN];
      strcpy(_buf, _saveline->txt.c_str());

      // If a LF was found, replace it with a CR/LF combo
      char* _lfptr = strchr(_buf, '\n');
      if(_lfptr)
        strcpy(_lfptr, "\r\n");

      // Save the line
      fputs(_buf, _fp);

      // Continue with the next line
      _saveline = _saveline->next;
    }

    // Close save file and remove wait window
    fclose(_fp);
  }

  update_statusline(statbak);

  if(wascursoron)
    cursoron();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::SaveFile() {

  GFTRK("EditSaveFile");

  savefile(MODE_UPDATE);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::SaveMsg() {

  GFTRK("EditSaveMsg");

  done = MODE_SAVE;

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

int IEclass::isempty(Line* __line) {

  if(__line == NULL)
    __line = currline;

  return (__line->txt.empty() or __line->txt[0] == '\n');
}


//  ------------------------------------------------------------------

int IEclass::reflowok(char* __qstr) {

  // Stop reflow if there is no next line
  if(currline->next == NULL)
    return false;

  // Stop reflow if the next line is empty
  if(isempty(currline->next))
    return false;

  // Stop reflow if the next line is a control line
  if(currline->next->type & (GLINE_KLUDGE|GLINE_TEAR|GLINE_ORIG|GLINE_TAGL))
    return false;

  // Stop reflow if the quotestring on the next line is not the same
  uint _qlen2;
  char _qstr2[100];
  GetQuotestr(currline->next->txt.c_str(), _qstr2, &_qlen2);
  if(not strieql(__qstr, _qstr2))
    return false;

  return true;
}


//  ------------------------------------------------------------------

void IEclass::Reflow() {

  GFTRK("EditReflow");

  // Skip empty lines
  while(isempty()) {
    if(currline->next)
      GoDown();
    else {
      GFTRK(NULL);
      return;
    }
  }

  // Get the first quotestring
  uint _qlen1;
  char _qstr1[100];
  GetQuotestr(currline->txt.c_str(), _qstr1, &_qlen1);
  const char* _qlenptr = currline->txt.c_str() + _qlen1;

  // Strip leading spaces from the first line
  const char* ptr = strskip_wht(_qlenptr);
  if(ptr != _qlenptr) {
    Undo->PushItem(EDIT_UNDO_DEL_TEXT, currline, _qlen1, ptr-_qlenptr);
    currline->txt.erase(_qlen1, ptr-_qlenptr);
  }

  // Perform the reflow
  while(reflowok(_qstr1)) {

    // Work on the current line until it is done
    Line* _thisline = currline;
    while(_thisline == currline) {

      // Stop reflow?
      if(not reflowok(_qstr1))
        break;

      // Go to the EOL, insert a space and delete the LF
      GoEOL();
      if(col+1 < maxcol) {
        insertchar(' ');
        DelChar();
      }
      else {
        GoDown();
        col = mincol;
      }
    }
  }

  // Go to the next line
  displine(currline,row);
  GoDown();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::ExitMsg() {

  GFTRK("EditExitMsg");

  done = MODE_QUIT;

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::DelLine() {

  GFTRK("EditDelLine");

  cursoroff();
  deleteline();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::ToUpper() {

  GFTRK("EditToUpper");

  Undo->PushItem(EDIT_UNDO_OVR_CHAR);
  currline->txt[col] = toupper(currline->txt[col]);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::ToLower() {

  GFTRK("EditToLower");

  Undo->PushItem(EDIT_UNDO_OVR_CHAR);
  currline->txt[col] = tolower(currline->txt[col]);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::ToggleCase() {

  GFTRK("EditToggleCase");

  Undo->PushItem(EDIT_UNDO_OVR_CHAR);
  if(toupper(currline->txt[col]) == currline->txt[col])
    currline->txt[col] = tolower(currline->txt[col]);
  else
    currline->txt[col] = toupper(currline->txt[col]);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::LookupCursor() {

  GFTRK("EditLookupCursor");

  LookupNode(msgptr, currline->txt.c_str()+col, LOOK_NAME);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::LookupDest() {

  GFTRK("EditLookupDest");

  LookupNode(msgptr, "", LOOK_DEST);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::LookupOrig() {

  GFTRK("EditLookupOrig");

  LookupNode(msgptr, "", LOOK_ORIG);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::Soundkill() {

  HandleGEvent(EVTT_STOPVOICE);
}


//  ------------------------------------------------------------------

void IEclass::statusline() {

  if(chartyped) {
    if(EDIT->Completion.First()) {
      do {
        const char* trig = EDIT->Completion.Trigger();
        uint tlen = strlen(trig);
        if(col >= tlen) {
          if(strneql(trig, currline->txt.c_str()+col-tlen, tlen)) {
            uint n;
            for(n=0; n<tlen; n++)
              DelLeft();
            const char* cptr = EDIT->Completion.Text();
            uint clen = strlen(cptr);
            for(n=0; n<clen; n++)
              insertchar(*cptr++);
            HandleGEvent(EVTT_EDITCOMPLETION);
            break;
          }
        }
      } while(EDIT->Completion.Next());
    }
  }

  char _buf[EDIT_BUFLEN];
  *_buf = NUL;

  if(EDIT->Comment.First()) {
    do {
      const char* trig = EDIT->Comment.Trigger();
      uint tlen = strlen(trig);
      if(col >= tlen) {
        if(strnieql(trig, currline->txt.c_str()+col-tlen, tlen)) {
          strcpy(_buf, EDIT->Comment.Text());
          break;
        }
      }
    } while(EDIT->Comment.Next());
  }

  update_statuslinef(LNG->EditStatus, 1+thisrow, 1+col, _buf);
  if(*_buf and CFG->switches.get(beepcomment)) {
    HandleGEvent(EVTT_EDITCOMMENT);
  }
}


//  ------------------------------------------------------------------

int IEclass::handlekey(gkey __key) {

  int rc = YES;

  switch(__key) {
    case KK_EditBlockRight:  __key = KK_EditGoRight;   break;
    case KK_EditBlockLeft:   __key = KK_EditGoLeft;    break;
    case KK_EditBlockUp:     __key = KK_EditGoUp;      break;
    case KK_EditBlockDown:   __key = KK_EditGoDown;    break;
    case KK_EditBlockHome:   __key = KK_EditGoBegLine; break;
    case KK_EditBlockEnd:    __key = KK_EditGoEOL;     break;
    case KK_EditBlockPgDn:   __key = KK_EditGoPgDn;    break;
    case KK_EditBlockPgUp:   __key = KK_EditGoPgUp;    break;

    case KK_EditCopy:
    case KK_EditCut:
    case KK_EditDelete:      goto noselecting;

    case KK_EditDelChar:
    case KK_EditDelLeft:
    case KK_EditPaste:
      if(selecting) {
        BlockCut(true);
        batch_mode = BATCH_MODE;
        if(__key != KK_EditPaste)
          __key = KK_EditUndefine;
      }
      goto noselecting;
      break;

    default:
      if(selecting) {
        Line *_line;
        selecting = NO;
        blockcol = -1;
        for(_line = findfirstline(); _line; _line = _line->next)
          _line->type &= ~GLINE_BLOK;
        // refresh screen
        int r = row;
        for(_line = currline; _line and r; _line = _line->prev)
          r--;
        refresh(_line, minrow);
      }
      goto noselecting;
  }

  if(not selecting) {
    Line *_line;
    for(_line = findfirstline(); _line; _line = _line->next)
      _line->type &= ~GLINE_BLOK;
    currline->type |= GLINE_BLOK;
    selecting = YES;
    blockcol = col;
    // refresh screen
    int r = row;
    for(_line = currline; _line and r; _line = _line->prev)
      r--;
    refresh(_line, minrow);
  }

noselecting:

  switch(__key) {
    case KK_EditAbort:            Abort();              break;
    case KK_EditAskExit:          AskExit();            break;
    case KK_EditClearDeleteBuf:   ClearDeleteBuf();     break;
    case KK_EditClearPasteBuf:    ClearPasteBuf();      break;
    case KK_EditCopyAboveChar:    CopyAboveChar();      break;
    case KK_EditDelChar:          DelChar();            break;
    case KK_EditDeleteEOL:        DeleteEOL();          break;
    case KK_EditDelLeft:          DelLeft();            break;
    case KK_EditDelLine:          DelLine();            break;
    case KK_EditDelLtWord:        DelLtWord();          break;
    case KK_EditDelRtWord:        DelRtWord();          break;
    case KK_EditDosShell:         DosShell();           break;
    case KK_EditDupLine:          DupLine();            break;
    case KK_EditExitMsg:          ExitMsg();            break;
    case KK_EditExportText:       ExportText();         break;
    case KK_EditGoBegLine:        GoBegLine();          break;
    case KK_EditGoBotLine:        GoBotLine();          break;
    case KK_EditGoBotMsg:         GoBotMsg();           break;
    case KK_EditGoDown:           GoDown();             break;
    case KK_EditGoEOL:            GoEOL();              break;
    case KK_EditGoLeft:           GoLeft();             break;
    case KK_EditGoPgDn:           GoPgDn();             break;
    case KK_EditGoPgUp:           GoPgUp();             break;
    case KK_EditGoRight:          GoRight();            break;
    case KK_EditGoTopLine:        GoTopLine();          break;
    case KK_EditGoTopMsg:         GoTopMsg();           break;
    case KK_EditGoUp:             GoUp();               break;
    case KK_EditGoWordLeft:       GoWordLeft();         break;
    case KK_EditGoWordRight:      GoWordRight();        break;
    case KK_EditHeader:           Header();             break;
    case KK_EditImportQuotebuf:   ImportQuotebuf();     break;
    case KK_EditImportText:       ImportText();         break;
    case KK_EditLoadFile:         LoadFile();           break;
    case KK_EditLookupCursor:     LookupCursor();       break;
    case KK_EditLookupDest:       LookupDest();         break;
    case KK_EditLookupOrig:       LookupOrig();         break;
    case KK_EditNewline:          Newline();            break;
    case KK_EditQuitNow:          QuitNow();            break;
    case KK_EditReflow:           Reflow();             break;
    case KK_EditSaveFile:         SaveFile();           break;
    case KK_EditSaveMsg:          SaveMsg();            break;
    case KK_EditSoundkill:        Soundkill();          break;
    case KK_EditSpellCheck:       SpellCheck();         break;
    case KK_EditTab:              Tab();                break;
    case KK_EditTabReverse:       ReTab();              break;
    case KK_EditToggleCase:       ToggleCase();         break;
    case KK_EditToggleInsert:     ToggleInsert();       break;
    case KK_EditToLower:          ToLower();            break;
    case KK_EditToUpper:          ToUpper();            break;
    case KK_EditUndefine:                               break;
    case KK_EditUnDelete:         UnDelete();           break;
    case KK_EditUndo:             Undo->PlayItem();     break;
    case KK_EditZapQuoteBelow:    ZapQuoteBelow();      break;

    // Block functions
    case KK_EditAnchor:           BlockAnchor();        break;
    case KK_EditCopy:             BlockCopy();          break;
    case KK_EditCut:              BlockCut();           break;
    case KK_EditDelete:           BlockCut(true);       break;
    case KK_EditPaste:            BlockPaste();         break;

    default:
      rc = PlayMacro(__key, KT_E);
  }

  if(__key != KK_EditUndo)
    undo_ready = NO;
  
  return rc;
}


//  ------------------------------------------------------------------

int IEclass::Start(int __mode, uint* __position, GMsg* __msg) {

  GFTRK("EditStart");

  thisrow = 0;
  quitnow = NO;
  col = mincol;
  row = minrow;
  msgptr = __msg;
  msgmode = __mode;
  currline = __msg->lin;

  if(AA->isinternet() and CFG->soupexportmargin <= CFG->dispmargin)
    margintext = CFG->soupexportmargin;
  else
    margintext = CFG->dispmargin;

  marginquotes = EDIT->QuoteMargin();
  if(marginquotes > margintext)
    marginquotes = margintext;

  whelppcat(H_Editor);

  if(currline == NULL) {
    currline = new Line("\n");
    throw_xnew(currline);
  }

  // Check if there is an unfinished backup message
  FILE* _fp = fsopen(AddPath(CFG->goldpath, EDIT->File()), "rt", CFG->sharemode);
  if(_fp) {
    char _buf[EDIT_BUFLEN];
    fgets(_buf, sizeof(_buf), _fp);
    fclose(_fp);
    if(striinc(unfinished, _buf)) {
      w_info(LNG->UnfinishedMsg);
      update_statusline(LNG->LoadUnfinished);
      HandleGEvent(EVTT_ATTENTION);
      gkey _ch = getxch();
      w_info(NULL);
      if(_ch != Key_Esc) {
        LoadFile();
        *__position = 0;
        remove(AddPath(CFG->goldpath, EDIT->File()));
      }
    }
  }

  if(*__position) {
    for(uint _posrow=0; _posrow < *__position; _posrow++) {
      if(currline->next) {
        currline = currline->next;
        row++;
      }
    }
    thisrow = row;
  }

  done = NO;

  // If the starting line is outside the first screenful
  if(*__position >= (maxrow+1)) {
    refresh(currline->prev, minrow);
    row = 1;
  }
  else {
    refresh(findfirstline(), minrow);
  }

  gotorowcol(mincol, minrow);
  dispins();

  time_t _lasttime = time(NULL);

  while(not done) {

    statusline();

    gotorowcol(col, row);
    batch_mode = 0;

    int backattr = 0;
    if(blockcol == -1) {
      backattr = dispchar(currline->txt[col], C_READC);
      gotorowcol(col, row);
    }

    cursoron();
    if(insert)
      vcursmall();
    else
      vcurlarge();

    gkey _ch;

    do {
      _ch = getxchtick();

      if(EDIT->AutoSave()) {
        time_t _thistime = time(NULL);
        if(_thistime >= (_lasttime+EDIT->AutoSave())) {
          _lasttime = _thistime;
          SaveFile();
        }
      }

      if(_ch == Key_Tick)
        CheckTick(KK_EditQuitNow);

    } while(_ch == Key_Tick);

    pcol = col; getthisrow(currline); prow = thisrow;

    int ismacro = false;
    gkey _kk = SearchKey(_ch, EditKey, EditKeys);
    if(_kk) {
      _ch = _kk;
    }
    else {
      ismacro = IsMacro(_ch, KT_E);
    }

    if(blockcol == -1)
      dispchar(currline->txt[col], backattr);

    chartyped = false;
    if((_ch < KK_Commands) and (_ch & 0xFF) and not ismacro) {
      chartyped = true;
      _ch &= 0xFF;
      insertchar((char)_ch);
      undo_ready = YES;
    }
    else if(handlekey(_ch)) {
      getthisrow(currline);
    }
  }

  cursoroff();

  msgptr->lin = findfirstline();
  savefile(quitnow ? MODE_UPDATE : MODE_SAVE);
  whelpop();

  // Prune killbuffer
  if(Edit__killbuf) {
    Line *__line = Edit__killbuf;

    int _count = EDIT->UnDelete();
    while(__line and _count--)
      __line = __line->prev;

    if(__line)
      if(__line->next)
        __line->next->prev = NULL;

    while(__line) {
      if(Undo->FixPushLine(__line)) {
        if(__line->prev) {
          __line = __line->prev;
          __line->next = NULL;
        }
        else {
          __line = NULL;
        }
      }
      else {
        if(__line->prev) {
          __line = __line->prev;
          throw_xdelete(__line->next);
        }
        else
          throw_xdelete(__line);
      }
    }
  }

  *__position = 1 + thisrow;

  GFTRK(NULL);

  return done;
}


//  ------------------------------------------------------------------

UndoStack::UndoStack(IEclass* this_editor) :

  editor(this_editor),
  row(editor->row),
  col(editor->col),
  pcol(editor->pcol),
  prow(editor->prow),
  minrow(editor->minrow),
  maxrow(editor->maxrow),
  thisrow(editor->thisrow),
  currline(editor->currline), 
  undo_ready(editor->undo_ready) {
  UndoItem::last_item = &last_item;
  last_item = NULL;
  undo_enabled = YES;
}


//  ------------------------------------------------------------------

UndoStack::~UndoStack() {

  while(last_item) {
    switch(last_item->action & EDIT_UNDO_ACTION) {
      case EDIT_UNDO_DEL_TEXT:
      case EDIT_UNDO_WRAP_TEXT:
        throw_delete(last_item->data.text_ptr);
        break;
      case EDIT_UNDO_DEL_LINE:
        throw_xdelete(last_item->data.line_ptr);
    }
    delete last_item;
  }
}


//  ------------------------------------------------------------------

bool UndoStack::FixPushLine(Line* __line) {

  UndoItem* item = last_item;

  while(item) {
    if(((item->action & EDIT_UNDO_ACTION) == EDIT_UNDO_PUSH_LINE) and (item->data.line_ptr == __line)) {
      item->action &= ~EDIT_UNDO_ACTION;
      item->action |= EDIT_UNDO_ORPHAN_LINE;
      return true;
    }
    item = item->prev;
  }
  return false;
}


//  ------------------------------------------------------------------

void UndoStack::PushItem(uint action, Line* __line, uint __col, uint __len) {

  if(undo_enabled) {

    throw_new(last_item = new UndoItem);
    last_item->col.num = (__col != NO_VALUE) ? __col : col;
    last_item->col.sav = 0;
    last_item->action = action;
    last_item->pcol = pcol;
    last_item->prow = prow;

    switch(action & EDIT_UNDO_ACTION) {
      case EDIT_UNDO_VOID:
      case EDIT_UNDO_INS_CHAR:
        last_item->line = __line ? __line : currline;
        break;
      case EDIT_UNDO_DEL_CHAR:
      case EDIT_UNDO_OVR_CHAR:
        last_item->line = __line ? __line : currline;
        last_item->data.char_int = last_item->line->txt[last_item->col.num];
        break;
      case EDIT_UNDO_DEL_TEXT:
        last_item->line = __line;
        if(__len == NO_VALUE)
          __len = strlen(__line->txt.c_str() + __col) + 1;
        throw_new(last_item->data.text_ptr = new(__len) text_item(__col, __len));
        memcpy(last_item->data.text_ptr->text, __line->txt.c_str() + __col, __len);
        break;
      case EDIT_UNDO_CUT_TEXT:
        last_item->line = __line;
        break;
      case EDIT_UNDO_INS_TEXT:
        last_item->line = __line;
        goto save_item;
      case EDIT_UNDO_WRAP_TEXT:
        last_item->line = __line->prev;
      save_item:
        throw_new(last_item->data.text_ptr = new text_item(__col, __len));
        break;
      case EDIT_UNDO_NEW_LINE:
        last_item->line = last_item->data.line_ptr = __line;
        break;
      case EDIT_UNDO_DEL_LINE:
        last_item->line = __line->prev ? __line->prev : __line->next;
        last_item->data.line_ptr = __line;
        break;
      case EDIT_UNDO_PUSH_LINE:
        if(currline->next)
          last_item->line = currline->next;
        else {
          last_item->action |= LAST_LINE;
          last_item->line = currline->prev;
        }
        last_item->data.line_ptr = currline;
        break;
      case EDIT_UNDO_POP_LINE:
        last_item->line = currline;
    }    
  }
}


//  ------------------------------------------------------------------

void UndoStack::PlayItem() {

  if(last_item) {

    UndoItem* item;

    // Don't save any new items while in Undo function
    undo_enabled = NO;

    // Find first of the batch items
    for(item = last_item; item->action & BATCH_MODE; item = item->prev);

    uint curr_row_num = thisrow;
    uint curr_col_num = col;
    currline = (item->action & PREV_LINE) ? item->line->next : item->line;
    editor->getthisrow(currline);
    col = item->col.num;

    if(curr_row_num != thisrow) {

      // Let user to see the position before performing Undo, unless it's a
      // neighbour line and the same column.
      undo_ready = (abs(int(curr_row_num - thisrow)) < 2 and (curr_col_num == col or col+1 > currline->txt.length()));

      // Move cursor up or down depending on where the undo line is,
      // then refresh window if the line is invisible.
      do {
        if(curr_row_num > thisrow) {
          if(row > minrow)
            curr_row_num--, row--;
          else {
            editor->refresh(currline, row);
            break;
          }
        }
        else {
          if(row < maxrow)
            curr_row_num++, row++;
          else {
            Line* l = currline;
            for(uint r = row; r; l = l->prev, r--) {}
            editor->refresh(l, minrow);
            break;
          }
        }
      } while(curr_row_num != thisrow);
    }
    else
      undo_ready = (abs(int(curr_col_num - col)) < 2 or col+1 > currline->txt.length());

    uint _pcol = item->pcol;
    uint _prow = item->prow;

    if(undo_ready) {

      bool in_batch;

      // Keep undoing until item with no BATCH_MODE flag is reached.
      do {

        uint undo_type = last_item->action & EDIT_UNDO_TYPE;
        uint undo_action = last_item->action & EDIT_UNDO_ACTION;
        in_batch = last_item->action & BATCH_MODE;
        currline = last_item->line;

        if(last_item->action & PREV_LINE) {
          col = last_item->col.num = last_item->col.sav;
          if(row > minrow)
            row--;
        }

        switch(undo_type) {

          case EDIT_UNDO_CHAR:
            switch(undo_action) {
              case EDIT_UNDO_INS_CHAR:
                currline->txt.erase(last_item->col.num,1);
                break;
              case EDIT_UNDO_DEL_CHAR:
                if(last_item->col.num == currline->txt.length() and last_item->data.char_int != ' ' and last_item->data.char_int != '\n')
                  currline->txt += ' ';
                currline->txt.insert(last_item->col.num, 1, last_item->data.char_int);
                break;
              case EDIT_UNDO_OVR_CHAR:
                currline->txt[last_item->col.num] = last_item->data.char_int;
                break;
            }
            editor->setlinetype(currline);
            break;

          case EDIT_UNDO_TEXT: {
            text_item* text_data = last_item->data.text_ptr;
            string& txt = currline->txt;
            switch(undo_action) {
              case EDIT_UNDO_DEL_TEXT:
                txt.insert(text_data->col, text_data->text, text_data->len);
                throw_delete(text_data);
                break;
              case EDIT_UNDO_CUT_TEXT:
                txt.erase(last_item->col.num);
                break;
              case EDIT_UNDO_WRAP_TEXT:
                txt.append(currline->next->txt.c_str()+text_data->col, text_data->len);
                txt = currline->next->txt;
                // fall through...
              case EDIT_UNDO_INS_TEXT:
                txt.erase(text_data->col, text_data->len);
                throw_delete(text_data);
                break;
            }
            editor->setlinetype(currline);
            break;
          }

          case EDIT_UNDO_LINE: {
            Line* thisline = last_item->data.line_ptr;
            switch(undo_action) {
              case EDIT_UNDO_NEW_LINE:
                if(thisline->next)
                  thisline->next->prev = thisline->prev;
                if(thisline->prev) {
                  thisline->prev->next = thisline->next;
                  currline = thisline->prev;
                }
                else
                  currline = thisline->next;
                throw_xdelete(thisline);
                break;
              case EDIT_UNDO_ORPHAN_LINE:
                if(last_item->action & LAST_LINE) {
                  thisline->prev = currline;
                  thisline->next = currline ? currline->next : NULL;
                  if((row < maxrow) and currline)
                    row++;
                }
                else {
                  thisline->prev = currline ? currline->prev : NULL;
                  thisline->next = currline;
                }
                // fall through...
              case EDIT_UNDO_DEL_LINE:
                if(thisline->prev)
                  thisline->prev->next = thisline;
                if(thisline->next)
                  thisline->next->prev = thisline;
                currline = thisline;
                break;
              case EDIT_UNDO_PUSH_LINE:
                editor->UnDelete((last_item->action & LAST_LINE) ? false : true);
                break;
              case EDIT_UNDO_POP_LINE:
                editor->DelLine();
                break;
            }
          }
        }

        _pcol = last_item->pcol;
        _prow = last_item->prow;
        delete last_item;

      } while(last_item and in_batch);

      editor->refresh(currline, row);
      undo_enabled = YES;
      editor->getthisrow(currline);
      uint temprow = thisrow, posrow;

      Line *templine = editor->findfirstline();
      if(templine) {
        for(posrow=0; posrow < _prow; posrow++)
          if(templine->next)
            templine = templine->next;
        thisrow = posrow; col = _pcol;
        currline = templine;

        if(not in_range(thisrow-temprow, minrow, maxrow)) {
          do {
            if(thisrow > temprow) {
              if(row > minrow)
                temprow--, row--;
              else {
                editor->refresh(templine, row);
                break;
              }
            }
            else {
              if(row < maxrow)
                temprow++, row++;
              else {
                editor->refresh(templine, row = minrow);
                break;
              }
            }
          } while(temprow != thisrow);
        }
        else {
          row += thisrow-temprow;
          editor->refresh(templine, row);
        }
      }
    }
    // Move the cursor to EOL if necessary
    else if(col+1 > currline->txt.length())
      editor->GoEOL();
    undo_ready = YES;
  }
}


//  ------------------------------------------------------------------
