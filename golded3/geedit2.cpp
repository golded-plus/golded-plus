
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
//  The Internal Editor (IE), part 2.
//  ------------------------------------------------------------------

#include <golded.h>
#include <geedit.h>
#include <gutlcode.h>
#include <gutlclip.h>


//  ------------------------------------------------------------------
//  Constructor

IEclass::IEclass(int __scol, int __ecol, int __srow, int __erow, int __border) {

  win_mincol = __scol;
  win_minrow = __srow;
  win_maxcol = __ecol;
  win_maxrow = __erow;
  win_border = __border;

  win_hasborder = (win_border == 5) ? 0 : 1;

  mincol  = 0;
  minrow  = 0;
  maxcol  = win_maxcol - win_mincol - (2*win_hasborder);
  maxrow  = win_maxrow - win_minrow - (2*win_hasborder);

  col  = 0;
  row  = 0;
  ccol = 0;
  crow = 0;

  chartyped    = false;
  currline     = NULL;
  done         = NO;
  insert       = YES;
  marginquotes = 0;
  margintext   = 0;
  msgmode      = 0;
  msgptr       = NULL;
  quitnow      = NO;
  thisrow      = 0;
  unfinished   = "+$!$+ GoldED Internal Editor: Unfinished Message!";
  blockcol     = -1;
  selecting    = NO;

  throw_new(Undo = new UndoStack(this));
  windowopen();
}


//  ------------------------------------------------------------------
//  Destructor

IEclass::~IEclass() {

  throw_delete(Undo);
  windowclose();
}


//  ------------------------------------------------------------------

void IEclass::windowopen() {

  // Open editor window without clearing the window area
  #define STYLE_NOCLEAR -1
  int _tmp = gwin.style;
  gwin.style = STYLE_NOCLEAR;
  editwin.open(win_minrow, win_mincol, win_maxrow, win_maxcol, 5, C_READB, C_READW, C_READPB);
  gwin.style = _tmp;
  whelppcat(H_Editor);
}


//  ------------------------------------------------------------------

void IEclass::windowclose() {

  whelpop();
  // Close editor window without removing the window itself
  editwin.unlink();
}


//  ------------------------------------------------------------------

void Edit__killpastebuf() {

  while(Edit__pastebuf) {
    if(Edit__pastebuf->next) {
      Edit__pastebuf = Edit__pastebuf->next;
      throw_xdelete(Edit__pastebuf->prev);
    }
    else
      throw_xdelete(Edit__pastebuf);
  }
}


//  ------------------------------------------------------------------

void Edit__killkillbuf() {

  while(Edit__killbuf) {
    if(Edit__killbuf->prev) {
      Edit__killbuf = Edit__killbuf->prev;
      throw_xdelete(Edit__killbuf->next);
    }
    else
      throw_xdelete(Edit__killbuf);
  }
}


//  ------------------------------------------------------------------

void IEclass::killkillbuf() {

  while(Edit__killbuf) {
    if(Undo->FixPushLine(Edit__killbuf)) {
      if(Edit__killbuf->prev) {
        Edit__killbuf = Edit__killbuf->prev;
        Edit__killbuf->next = NULL;
      }
      else {
        Edit__killbuf = NULL;
      }
    }
    else {
      if(Edit__killbuf->prev) {
        Edit__killbuf = Edit__killbuf->prev;
        throw_xdelete(Edit__killbuf->next);
      }
      else
        throw_xdelete(Edit__killbuf);
    }
  }
}


//  ------------------------------------------------------------------

void FreePastebuf() {

  Edit__killpastebuf();
  Edit__killkillbuf();
}


//  ------------------------------------------------------------------

void IEclass::ClearDeleteBuf() {

  GFTRK("EditClearDeleteBuf");

  killkillbuf();
  HandleGEvent(EVTT_JOBDONE);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::ClearPasteBuf() {

  GFTRK("EditClearPasteBuf");

  killpastebuf();
  HandleGEvent(EVTT_JOBDONE);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::GoBegLine() {

  GFTRK("EditGoBegLine");

  col = mincol;

  if(blockcol != -1)
    displine(currline, row);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::DelLtWord() {

  GFTRK("EditDelLtWord");

  if(col == 0) {
    DelLeft();
    GFTRK(NULL);
    return;
  }

  int _ptr, _ptr2;
  _ptr = _ptr2 = col;
  _ptr--;

  // test test test test     test test test test
  // test test test test ,   test test test test
  // test test test test ,,  test test test test

  if(currline->txt[_ptr] == ' ') {
    while((currline->txt[_ptr] == ' ') and (_ptr > 0))
      _ptr--;
    if(currline->txt[_ptr] != ' ')
      _ptr++;
  }
  else if(isxalnum(currline->txt[_ptr])) {
    while(isxalnum(currline->txt[_ptr]) and (_ptr > 0))
      _ptr--;
    while((currline->txt[_ptr] == ' ') and (_ptr > 0))
      _ptr--;
    if((currline->txt[_ptr] != ' ') and (_ptr > 0))
      _ptr++;
  }
  else {
    DelLeft();
    GFTRK(NULL);
    return;
  }

  col -= _ptr2-_ptr;

  Undo->PushItem(EDIT_UNDO_DEL_TEXT, currline, col, _ptr2-_ptr);
  currline->txt.erase(_ptr, _ptr2-_ptr);

  wrapdel(&currline, &col, &row);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::DelRtWord() {

  GFTRK("EditDelRtWord");

  if((currline->txt.length() == col+1) or (currline->txt[col+1] == '\n')) {
    DelChar();
    GFTRK(NULL);
    return;
  }
  
  int _ptr, _ptr2;
  _ptr = _ptr2 = col;

  // test test test test,   test test test test
  // test test test test,   test test test test

  if(currline->txt[_ptr] == ' ') {
    while(_ptr != currline->txt.length() and currline->txt[_ptr] == ' ')
      _ptr++;
  }
  else if(isxalnum(currline->txt[_ptr])) {
    // Delete word
    while(_ptr != currline->txt.length() and isxalnum(currline->txt[_ptr]))
      _ptr++;
    // Delete spaces after word
    while(_ptr != currline->txt.length() and currline->txt[_ptr] == ' ')
      _ptr++;
  }
  else {
    DelChar();
    GFTRK(NULL);
    return;
  }

  Undo->PushItem(EDIT_UNDO_DEL_TEXT, currline, col, _ptr-_ptr2);
  currline->txt.erase(_ptr2, _ptr-_ptr2);

  wrapdel(&currline, &col, &row);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::GoTopMsg() {

  GFTRK("EditGoTopMsg");

  currline = findfirstline();
  col = mincol;
  row = minrow;
  thisrow = 0;
  refresh(currline, minrow);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::GoBotMsg() {

  GFTRK("EditGoBotMsg");

  col = mincol;
  thisrow = 0;
  currline = findfirstline();

  // Go to the last line in the msg
  while(currline->next) {
    currline = currline->next;
    thisrow++;
  }

  // Pointer to the line to display at the top of the window
  Line* _topline = currline;

  // The new cursor row
  row = MinV(thisrow, maxrow);

  // How many lines to go back to get the top line
  int _count = row;

  // Go back to get the top line
  while(_count-- and _topline->prev)
    _topline = _topline->prev;

  // Refresh the display
  refresh(_topline, minrow);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::GoTopLine() {

  GFTRK("EditGoTopLine");

  int _count = row;

  while(_count-- and currline->prev) {
    currline = currline->prev;
    thisrow--;
  }

  col = mincol;
  row = minrow;

  if(blockcol != -1)
    refresh(currline, minrow);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::GoBotLine() {

  GFTRK("EditGoBotLine");

  Line *_oldcurrline = currline;
  int  _oldrow = row;
  
  while((row < maxrow) and currline->next) {
    currline = currline->next;
    thisrow++;
    row++;
  }

  col = mincol;

  if(blockcol != -1)
    refresh(_oldcurrline, _oldrow);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

Line* IEclass::findanchor() {

  GFTRK("Editfindanchor");

  // Rewind to the first line
  Line* _anchor = findfirstline();

  // Search all lines to find the anchor (a line with a block mark)
  while(not (_anchor->type & GLINE_BLOK) and _anchor->next)
    _anchor = _anchor->next;

  GFTRK(NULL);

  // Return pointer to the anchor line or NULL if no anchor was found
  return (_anchor->type & GLINE_BLOK) ? _anchor : (Line*)NULL;
}


//  ------------------------------------------------------------------

void IEclass::BlockAnchor() {

  GFTRK("EditBlockAnchor");

  Line* _anchor = findanchor();

  // Is there an anchor already?
  if(_anchor) {

    // Yes, so replace it with the current line

    // Remove block mark
    _anchor->type &= ~GLINE_BLOK;
    blockcol = -1;

    // Is the old anchor different from the current line?
    if(_anchor != currline) {

      // Set the new anchor
      _anchor = currline;
      _anchor->type |= GLINE_BLOK;
      blockcol = col;

      // Find the line at the top
      Line* _topline = findtopline();

      // Refresh display to remove the block color on the old anchor line
      // Just in case the old anchor line is visible
      refresh(_topline, minrow);
    }

    // Remove the old contents of the paste buffer
    killpastebuf();
  }
  else {

    for(Line* _line = findfirstline(); _line; _line = _line->next)
      _line->type &= ~GLINE_BLOK;

    // There was no anchor, so mark the current line as the new anchor
    currline->type |= GLINE_BLOK;
    blockcol = col;

    // Refresh the display
    Line* _topline = findtopline();
    refresh(_topline, minrow);
  }

  displine(currline, row);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::BlockCopy() {

  GFTRK("EditBlockCopy");

  // Find the anchor, if any
  Line* _anchor = findanchor();

  // Did we find the anchor?
  if(_anchor) {

    Line* _firstcopyline = currline;
    Line* _lastcopyline  = currline;
    int   firstcol = col, lastcol = col;

    // Search below to find the anchor line
    while(_lastcopyline->next and (_lastcopyline != _anchor))
      _lastcopyline = _lastcopyline->next;

    // Was the anchor line above or on the current line?
    if(_lastcopyline != _anchor) {

      // The last copy line is the current line
      _lastcopyline = currline;

      // Search above to find the anchor line
      while(_firstcopyline->prev and (_firstcopyline != _anchor))
        _firstcopyline = _firstcopyline->prev;
      firstcol = blockcol;
    }
    else {
      if(currline != _anchor or blockcol > col)
        lastcol = blockcol;
      else
        firstcol = blockcol;
    }

    // The _firstcopyline and _lastcopyline pointers
    // are now pointing where they should

    // Remove the old paste buffer
    killpastebuf();

    // Pointer to the previous line in the paste buffer
    Line* _prevline = NULL;

    // Copy lines to the paste buffer
    while(1) {

      // Allocate a new line
      Line* _copyline;

      // Copy text and type
      if(_prevline == NULL)
        _copyline = new Line(_firstcopyline->txt.c_str() + firstcol);
      else
        _copyline = new Line(_firstcopyline->txt.c_str());
      throw_xnew(_copyline);
      if(_firstcopyline == _lastcopyline) {
        if(_prevline)
          _copyline->txt.erase(lastcol);
        else
          _copyline->txt.erase(lastcol-firstcol);
      }
      _copyline->type = _firstcopyline->type & ~GLINE_BLOK;

      // Link in the new line
      _copyline->prev = _prevline;
      if(_prevline)
        _prevline->next = _copyline;
      _copyline->next = NULL;

      // Point the paste buffer to the first line of the copy
      if(Edit__pastebuf == NULL)
        Edit__pastebuf = _copyline;

      // Break out of the loop if the last line was copied
      if(_firstcopyline == _lastcopyline)
        break;

      // Keep pointer to the new line
      _prevline = _copyline;

      // Continue with the next line
      _firstcopyline = _firstcopyline->next;
    } 

    selecting = NO;
    blockcol = -1;
    for(Line* _line = findfirstline(); _line; _line = _line->next)
      _line->type &= ~GLINE_BLOK;

    // Refresh display to remove the block color
    Line* _topline = findtopline();
    refresh(_topline, minrow);

    Buf2Clip();
  }
  killpastebuf();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::BlockDel(Line* anchor) {

  GFTRK("EditBlockDel");

  Line* firstcutline = currline;
  Line* lastcutline  = currline;
  uint firstcutlinerow = row;
  uint lastcutlinerow  = row;
  int  firstcol = col, lastcol = col;

  // Search below to find the anchor line
  while(lastcutline->next and (lastcutline != anchor)) {
    lastcutline = lastcutline->next;
    lastcutlinerow++;
  }

  // Was the anchor line above or on the current line?
  if(lastcutline != anchor) {

    // The last cut line is the current line
    lastcutline = currline;
    lastcutlinerow = row;

    // Search above to find the anchor line
    while(firstcutline->prev and (firstcutline != anchor)) {
      firstcutline = firstcutline->prev;
      if(firstcutlinerow)
        firstcutlinerow--;
    }
    firstcol = blockcol;
  }
  else {
    if(currline != anchor or blockcol > col)
      lastcol = blockcol;
    else
      firstcol = blockcol;
  }

  // The firstcutline and lastcutline pointers
  // are now pointing where they should

  if(firstcutline != lastcutline) {
    Undo->PushItem(EDIT_UNDO_DEL_TEXT, firstcutline, firstcol);
    firstcutline->txt.erase(firstcol);
    firstcutline->txt += lastcutline->txt.c_str()+lastcol;
  }
  else {
    Undo->PushItem(EDIT_UNDO_DEL_TEXT, firstcutline, firstcol, lastcol-firstcol);
    firstcutline->txt.erase(firstcol, lastcol-firstcol);
  }
  setlinetype(firstcutline);
  firstcutline->type &= ~GLINE_BLOK;
  blockcol = -1;
  currline = firstcutline;
  row = firstcutlinerow;
  col = firstcol;
  if(firstcutline != lastcutline) {
    do {
      Undo->PushItem(EDIT_UNDO_DEL_LINE|BATCH_MODE, firstcutline = firstcutline->next);
    } while(firstcutline != lastcutline);

    currline->next = lastcutline->next;
    if(lastcutline->next)
      lastcutline->next->prev = currline;
  }

  // Refresh the display
  if(not RngV(row, minrow, maxrow)) {
    row = minrow;
    wrapdel(&currline, &col, &row, YES);
    refresh(currline, minrow);
  }
  else {
    row = MaxV(firstcutlinerow, minrow);
    Line* topline = findtopline();
    wrapdel(&currline, &col, &row, NO);
    refresh(topline, minrow);
  }

  Line* line;
  for(line = findfirstline(); line; line = line->next)
    line->type &= ~GLINE_BLOK;

  selecting = NO;

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::BlockCut(bool just_delete) {

  GFTRK("EditBlockCut");

  // Find the anchor, if any
  Line* _anchor = findanchor();

  // Did we find the anchor?
  if(_anchor) {

    int _blockcol = blockcol;

    if(not just_delete)
      BlockCopy();

    blockcol = _blockcol;

    BlockDel(_anchor);
  }
  else
    killpastebuf();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::BlockPaste() {

  GFTRK("EditBlockPaste");

  killpastebuf();
  Clip2Buf();

  if(Edit__pastebuf) {

    Line* _pasteline = Edit__pastebuf;

    if(not batch_mode)
      Undo->PushItem(EDIT_UNDO_VOID);

    // For each of the lines in the paste buffer
    while(_pasteline) {

      uint  curlen = currline->txt.length();
      uint  pastelen = _pasteline->txt.length();

      if(col > curlen)
        col = curlen;

      if(_pasteline->txt.find('\n') != _pasteline->txt.npos) {
        // append to current line
        Undo->PushItem(EDIT_UNDO_DEL_TEXT|BATCH_MODE, currline, col);
        Line* _newline = insertlinebelow(currline, currline->txt.c_str()+col, BATCH_MODE);
        currline->txt.erase(col);
        currline->txt += _pasteline->txt;
        setlinetype(currline);
        col = currline->txt.length();
        wrapins(&currline, &col, &row, NO);
        currline = _newline;
        col = 0;
        if(row < maxrow)
          row++;
      }
      else {
        // insert into current line
        currline->txt.insert(col, _pasteline->txt);
        Undo->PushItem(EDIT_UNDO_INS_TEXT|BATCH_MODE, currline, col, pastelen);
        col += pastelen;
        wrapins(&currline, &col, &row, NO);
      }

      setlinetype(currline);

      // Continue with the next line in the paste buffer
      _pasteline = _pasteline->next;
    }

    selecting = NO;
    blockcol = -1;
    for(Line* _line = findfirstline(); _line; _line = _line->next)
      _line->type &= ~GLINE_BLOK;

    // Refresh the display
    Line* _topline = findtopline();
    refresh(_topline, minrow);
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::LoadFile() {

  GFTRK("EditLoadFile");

  // Open the file to load
  FILE* _fp = fsopen(AddPath(CFG->goldpath, EDIT->File()), "rb", CFG->sharemode);
  if(_fp) {

    XlatName __oldxlatimport;

    // Pop up a wait window
    cursoroff();
    w_info(LNG->Wait);

    throw_delete(Undo);

    // Find the first line
    Line* _line = findfirstline();

    // Remove all lines
    while(_line) {
      Line* _nextline = _line->next;
      throw_xdelete(_line);
      _line = _nextline;
    }

    // Remove message text and reset pointers
    throw_release(msgptr->txt);
    currline = msgptr->lin = NULL;

    // Allocate space for new message text
    msgptr->txt = (char*)throw_calloc(1, (uint)(fsize(_fp)+256));

    // Eat the backup marking line
    char _buf[EDIT_BUFLEN];
    fgets(_buf, sizeof(_buf), _fp);
    if(not striinc(unfinished, _buf))
      rewind(_fp);

    // Load the file and close it
    fread(msgptr->txt, 1, (uint)fsize(_fp), _fp);
    fclose(_fp);

    // Save current charset
    strcpy(__oldxlatimport, AA->Xlatimport());
    AA->SetXlatimport(CFG->xlatlocalset);

    // Index message text
    msgptr->TextToLines(margintext-1);
    _line = currline = msgptr->lin;

    // Restore charset
    AA->SetXlatimport(__oldxlatimport);

    // Change lines to internal editor format
    while(_line) {
      strtrim(_line->txt);
      if(_line->type & GLINE_HARD)
        _line->txt += "\n";
      else
        _line->txt += " ";
      _line = _line->next;
    }

    // Remove the wait window
    w_info(NULL);

    // Display the loaded message
    Line* l = findfirstline();
    uint i;

    for(i=0; l->next and i<thisrow-row; i++)
      l = l->next;

    refresh(l, minrow);

    for(i=0; l->next and i<row; i++)
      l = l->next;

    currline = l;
    col = mincol;

    throw_new(Undo = new UndoStack(this));
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

inline char uuencode_enc(int c) { return (char)(c ? (c & 077) + ' ': '`'); }

void IEclass::editimport(Line* __line, char* __filename, bool imptxt) {

  XlatName __oldxlatimport;

  GFTRK("Editimport");

  // Save the unfinished msg first of all
  savefile(MODE_UPDATE);

  update_statusline(LNG->ImportFile);

  // Set initial import filename or wildcards
  if(__filename) {
    AA->SetInputfile(__filename);
  }
  else {
    if(*AA->Inputfile() == NUL)
      AA->SetInputfile("*");
  }

  strcpy(__oldxlatimport, AA->Xlatimport());
  AA->SetXlatimport(CFG->xlatlocalset);

  strcpy(CFG->inputfile, AA->Inputfile());

  int ImportMode;

  if(imptxt)
    ImportMode = 0;
  else {

    GMenuImportTxt MenuImportTxt;

    ImportMode = MenuImportTxt.Run();
  }

  string filenamebuf;
  Path tmpfile;
  bool isPipe = NO;
  bool fileselected = false;

  // Should the imported text be quoted or uuencoded?
  #define quoteit    (ImportMode == 1)
  #define uuencode   (ImportMode == 2)
  #define base64     (ImportMode == 3)
  #define getclip    (ImportMode == 4)
  #define binary     (uuencode or base64)

  if(in_range(ImportMode, 0, 3) and edit_pathname(CFG->inputfile, sizeof(Path), LNG->ImportWhichFile, H_ImportFile)) {

    AA->SetInputfile(CFG->inputfile);

    // Pointer to the filename string
    filenamebuf = AA->Inputfile();

    if(filenamebuf.c_str()[0] == '|'){
      Path cmdline;

      isPipe = YES;
      mktemp(strxcpy(tmpfile, AddPath(CFG->goldpath, "GIXXXXXX"), sizeof(Path)));
      strxmerge(cmdline, sizeof(Path), filenamebuf.c_str()+1, " > ", tmpfile, NULL);
      ShellToDos(cmdline, "", NO, NO);
      filenamebuf = tmpfile;
      fileselected = true;
    } else {
      // Check for wildcards
      // Is the filename a directory?
      if(is_dir(filenamebuf)) {

        // Does the filename contain wildcards?
        if(not strpbrk(filenamebuf.c_str(), "*?")) {

          // Add match-all wildcards
          AddBackslash(filenamebuf);
          filenamebuf += "*";
        }
      }

      fileselected = true;

      // Does the filename contain wildcards?
      if(strpbrk(filenamebuf.c_str(), "*?")) {

        // Set selection window title and statusline
        set_title(LNG->ImportTitle, TCENTER, C_MENUT);
        update_statuslinef(LNG->ImportStatus, filenamebuf.c_str());

        // Start the file picker
        fileselected = wpickfile(win_minrow, win_mincol, win_maxrow, win_maxcol, W_BMENU, C_MENUB, C_MENUW, C_MENUS, NO, filenamebuf, maketitle);
      }
    }
  }

  if(fileselected or getclip) {

    // Open the file/clipboard
    FILE* fp = NULL;
    gclipbrd clipbrd;

    if(getclip)
      filenamebuf = CLIP_NAME;

    if(getclip ? clipbrd.openread() :
       (fp = fsopen(filenamebuf.c_str(), binary ? "rb" : "rt", CFG->sharemode))!=NULL) {

      if (isPipe)
        filenamebuf = AA->Inputfile();

      const char *imp_filename = (getclip or isPipe) ? filenamebuf.c_str() : CleanFilename(filenamebuf.c_str());

      // we need to truncate filename to prevent unpredictable results
      int delta = strlen(imp_filename) - margintext;
      if(delta > 0) {
        filenamebuf.erase(filenamebuf.length()-delta);
        imp_filename = (getclip or isPipe) ? filenamebuf.c_str() : CleanFilename(filenamebuf.c_str());
      }

      update_statuslinef(LNG->ImportStatus, filenamebuf.c_str());

      // Allocate paragraph read buffer
      char* _parabuf = (char*)throw_malloc(EDIT_PARABUFLEN);

      if(__line->prev)
        Undo->PushItem(EDIT_UNDO_VOID|PREV_LINE|batch_mode, __line->prev);
      else
        Undo->PushItem(EDIT_UNDO_VOID|batch_mode, __line);
      batch_mode = BATCH_MODE;

      // Add import begin text, if any
      if(*CFG->importbegin) {
        sprintf(_parabuf, "%s\n", CFG->importbegin);
        strischg(_parabuf, "@file", imp_filename);
        _parabuf[margintext] = NUL;
        _parabuf[margintext-1] = '\n';
        __line = insertlinebelow(__line, _parabuf);
        setlinetype(__line);
      }

      if(uuencode) {

        sprintf(_parabuf, "begin 644 %s\n", imp_filename);
        _parabuf[margintext] = NUL;
        _parabuf[margintext-1] = '\n';
        __line = insertlinebelow(__line, _parabuf);
        setlinetype(__line);

        while(1) {
          char ibuf[80];
          char* iptr = ibuf;
          char* optr = _parabuf;
          int n = fread(ibuf, 1, 45, fp);
          if(n < 45)
            memset(ibuf+n, 0, 45-n);
          *optr++ = uuencode_enc(n);
          for(int i=0; i<n; i+=3,iptr+=3) {
            *optr++ = uuencode_enc(*iptr >> 2);
            *optr++ = uuencode_enc(((*iptr << 4) & 060) | ((iptr[1] >> 4) & 017));
            *optr++ = uuencode_enc(((iptr[1] << 2) & 074) | ((iptr[2] >> 6) & 03));
            *optr++ = uuencode_enc(iptr[2] & 077);
          }
          *optr++ = '\n';
          *optr = NUL;
          __line = insertlinebelow(__line, _parabuf);
          // set type to text
          __line->type &= ~GLINE_ALL;
          if(n <= 0)
            break;
        }

        __line = insertlinebelow(__line, "end\n");
        setlinetype(__line);
      }
      else if(base64) {

        base64_engine b64;

        sprintf(_parabuf, "Content-type: application/octet-stream; name=\"%s\"\n", imp_filename);
        strcpy(_parabuf+margintext-2, "\"\n");
        __line = insertlinebelow(__line, _parabuf);
        setlinetype(__line);
        sprintf(_parabuf, "Content-transfer-encoding: base64\n");
        __line = insertlinebelow(__line, _parabuf);
        setlinetype(__line);

        sprintf(_parabuf, "\n");
        __line = insertlinebelow(__line, _parabuf);
        setlinetype(__line);

        for(;;) {
          char ibuf[80];
          char* optr = _parabuf;
          int n = fread(ibuf, 1, 54, fp);
          optr = b64.encode(optr, ibuf, n);
          *optr++ = '\n';
          *optr = NUL;
          __line = insertlinebelow(__line, _parabuf);
          setlinetype(__line);
          if(n <= 0)
            break;
        }
      }
      else {

        int tabsz = CFG->disptabsize ? CFG->disptabsize : 1;
        __extension__ char spaces[tabsz+1];
        memset(spaces, ' ', tabsz);
        spaces[tabsz] = NUL;
        int level = LoadCharset(AA->Xlatimport(), CFG->xlatlocalset);
        char* buf = (char*) throw_malloc(EDIT_PARABUFLEN);
        Line* saveline = __line->next;
        __line->next = NULL;

        // Read paragraphs
        while(getclip ? clipbrd.read(_parabuf, EDIT_PARABUFLEN-7) : fgets(_parabuf, EDIT_PARABUFLEN-7, fp)) {

          XlatStr(buf, _parabuf, level, CharTable);

          // Insert a quotestring if asked
          if(quoteit)
            strins(" > ", buf, 0);
          else {
            // Invalidate tearline
            if(not CFG->invalidate.tearline.first.empty())
              doinvalidate(buf, CFG->invalidate.tearline.first.c_str(), CFG->invalidate.tearline.second.c_str(), true);

            // Invalidate originline
            if(not CFG->invalidate.origin.first.empty())
              doinvalidate(buf, CFG->invalidate.origin.first.c_str(), CFG->invalidate.origin.second.c_str());

            // Invalidate SEEN-BY's
            if(not CFG->invalidate.seenby.first.empty())
              doinvalidate(buf, CFG->invalidate.seenby.first.c_str(), CFG->invalidate.seenby.second.c_str());

            // Invalidate CC's
            if(not CFG->invalidate.cc.first.empty())
              doinvalidate(buf, CFG->invalidate.cc.first.c_str(), CFG->invalidate.cc.second.c_str());

            // Invalidate XC's
            if(not CFG->invalidate.xc.first.empty())
              doinvalidate(buf, CFG->invalidate.xc.first.c_str(), CFG->invalidate.xc.second.c_str());

            // Invalidate XP's
            if(not CFG->invalidate.xp.first.empty())
              doinvalidate(buf, CFG->invalidate.xp.first.c_str(), CFG->invalidate.xp.second.c_str());
          }

          // Replace tabs
          char *ht = buf;
          while((ht = strchr(ht, '\t')) != NULL) {
            int rposn = ht-buf;
            int rstart = rposn%tabsz+1;
            *ht = ' ';
            if(tabsz > rstart)
              strins(spaces+rstart, buf, rposn);
          }

          // Copy the paragraph to the new line and retype it
          Line* _newline = __line = insertlinebelow(__line, buf);
          setlinetype(_newline);

          // If the paragraph is longer than one line
          uint _wrapmargin = (_newline->type & GLINE_QUOT) ? marginquotes : margintext;
          if(_newline->txt.length() >= _wrapmargin) {

            // Wrap it
            uint _tmpcol = 0;
            uint _tmprow = 0;
            _newline = wrapins(&_newline, &_tmpcol, &_tmprow, NO);
          }

          __line = _newline;
        }

        while(__line->next)
          __line = __line->next;
        if(not __line->txt.empty() and (*(__line->txt.end()-1) != '\n')) {
          Undo->PushItem(EDIT_UNDO_INS_CHAR|BATCH_MODE, __line, __line->txt.length());
          __line->txt += '\n';
          // Wrap it
          uint _tmpcol = 0;
          uint _tmprow = 0;
          __line = wrapins(&__line, &_tmpcol, &_tmprow, NO);
        }
        __line->next = saveline;
        if(saveline)
          saveline->prev = __line;

        throw_free(buf);
      }

      // Add import end text, if any
      if(*CFG->importend or *CFG->importbegin) {
        sprintf(_parabuf, "%s\n", *CFG->importend ? CFG->importend : CFG->importbegin);
        strischg(_parabuf, "@file", imp_filename);
        _parabuf[margintext] = NUL;
        _parabuf[margintext-1] = '\n';
        __line = insertlinebelow(__line, _parabuf);
        setlinetype(__line);
      }

      throw_free(_parabuf);

      if(getclip)
        clipbrd.close();
      else
        fclose(fp);
    }
    else {
      w_infof(LNG->CouldNotOpen, filenamebuf.c_str());
      waitkeyt(10000);
      w_info(NULL);
    }

    if(isPipe)
      unlink(tmpfile);
  }

  AA->SetXlatimport(__oldxlatimport);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::imptxt(char* __filename, bool imptxt) {

  GFTRK("Editimptxt");

  msgptr->lin = findfirstline();
  editimport(currline, __filename, imptxt);
  refresh(currline, row);
  col = mincol;

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::ImportText() {

  GFTRK("EditImportText");

  imptxt(NULL);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::ImportQuotebuf() {

  GFTRK("EditImportQuotebuf");

  Path _quotebuf;

  GetCurrQuotebuf(_quotebuf);
  imptxt(_quotebuf, true);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::editexport(Line* __exportline, int __endat) {

  GFTRK("Editexport");

  update_statusline(LNG->ExportFile);

  if(edit_string(Edit__exportfilename, sizeof(Path), LNG->ExportWhatFile, H_ExportFile)) {

    // Pointer to export filename
    char* _filenameptr = Edit__exportfilename;

    // Is append requested?
    if(*_filenameptr == '+')
      _filenameptr++;

    FILE* _fp = fsopen(_filenameptr, (*Edit__exportfilename == '+') ? "at" : "wt", CFG->sharemode);
    if(_fp) {

      update_statuslinef(LNG->ExportStatus, Edit__exportfilename);
      fputc('\n', _fp);

      while((__endat ? __exportline != currline : 1) and __exportline) {
        fwrite(__exportline->txt.c_str(), 1, __exportline->txt.length(), _fp);
        if(__exportline->txt.find('\n') == __exportline->txt.npos)
          fputc('\n', _fp);
        __exportline = __exportline->next;
      }

      fclose(_fp);
    }
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::SpellCheck() {

  GFTRK("EditSpellCheck");

  char _buf[EDIT_BUFLEN];
  char _buf2[EDIT_BUFLEN];

  savefile(MODE_SAVE);
  strcpy(_buf, EDIT->SpellChecker());
  strcpy(_buf2, AddPath(CFG->goldpath, EDIT->File()));
  strchg(_buf2, GOLD_WRONG_SLASH_CHR, GOLD_SLASH_CHR);
  strischg(_buf, "@file", _buf2);
  sprintf(_buf2, LNG->SpellChecker, _buf);
  ShellToDos(_buf, _buf2, LGREY|_BLACK, YES);
  LoadFile();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::ExportText() {

  GFTRK("EditExportText");

  int endat = NO;
  // Line* exportline = findanchor();

  // if(exportline)
  //  endat = YES;
  // else
  Line* exportline = findfirstline();

  editexport(exportline, endat);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::DosShell() {

  GFTRK("EditDosShell");

  ShellToDos(getenv(GOLD_SHELL_ENV), LNG->DOS_Shell, LGREY|_BLACK, YES);
  cursoron();
  cursoroff();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::dispins() {

  GFTRK("Editdispins");

  if(insert)
    HeaderView->window.prints(5, MAXCOL-6, C_HEADT, LNG->Ins);
  else {
    vchar _lbuf[6];
    for(int c = 0; c < 5; c++)
      _lbuf[c] = _box_table(W_BHEAD,1);
    _lbuf[5] = NUL;
    HeaderView->window.printvs(5, MAXCOL-6, C_HEADB|ACSET, _lbuf);
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::ToggleInsert() {

  GFTRK("EditToggleInsert");

  insert = not insert;
  dispins();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::Header() {

  GFTRK("EditHeader");

  windowclose();
  EditHeaderinfo(msgmode, *HeaderView);
  windowopen();

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::Abort() {

  GFTRK("EditAbort");

  cursoroff();
  GMenuDropmsg MenuDropmsg;
  if(MenuDropmsg.Run()) {
    done = MODE_QUIT;
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::AskExit() {

  GFTRK("EditAskExit");

  cursoroff();
  GMenuQuit MenuQuit;
  gkbd.quitall = MenuQuit.Run();
  if(gkbd.quitall) {
    GMenuDropmsg MenuDropmsg;
    if(MenuDropmsg.Run())
      done = MODE_QUIT;
    else
      done = MODE_SAVE;
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void IEclass::QuitNow() {

  GFTRK("EditQuitNow");

  quitnow = CFG->switches.get(timeoutsavemsg) ? NO : YES;
  done = CFG->switches.get(timeoutsavemsg) ? MODE_SAVE : MODE_QUIT;
  gkbd.quitall = YES;

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

int EditMsg(int __mode, uint* __position, GMsg* __msg) {

  IEclass Editor(0, MAXCOL-1, MINROW, MAXROW-2, 5);
  return Editor.Start(__mode, __position, __msg);
}


//  ------------------------------------------------------------------

