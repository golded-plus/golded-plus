
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

#if defined(__USE_ALLOCA__)
    #include <malloc.h>
#endif


//  ------------------------------------------------------------------
//  Constructor

IEclass::IEclass(int __scol, int __ecol, int __srow, int __erow, int __border)
{

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
    drawlines    = 0;
    drawflag     = true;
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

IEclass::~IEclass()
{

    throw_delete(Undo);
    windowclose();
}


//  ------------------------------------------------------------------

void IEclass::windowopen()
{

    // Open editor window without clearing the window area
#define STYLE_NOCLEAR -1
    int _tmp = gwin.style;
    gwin.style = STYLE_NOCLEAR;
    editwin.open(win_minrow, win_mincol, win_maxrow, win_maxcol, 5, C_READB, C_READW, C_READPB);
    gwin.style = _tmp;
    whelppcat(H_Editor);
}


//  ------------------------------------------------------------------

void IEclass::windowclose()
{

    whelpop();
    // Close editor window without removing the window itself
    editwin.unlink();
}


//  ------------------------------------------------------------------

void Edit__killpastebuf()
{

    while(Edit__pastebuf)
    {
        if(Edit__pastebuf->next)
        {
            Edit__pastebuf = Edit__pastebuf->next;
            throw_xdelete(Edit__pastebuf->prev);
        }
        else
            throw_xdelete(Edit__pastebuf);
    }
}


//  ------------------------------------------------------------------

void Edit__killkillbuf()
{

    while(Edit__killbuf)
    {
        if(Edit__killbuf->prev)
        {
            Edit__killbuf = Edit__killbuf->prev;
            throw_xdelete(Edit__killbuf->next);
        }
        else
            throw_xdelete(Edit__killbuf);
    }
}


//  ------------------------------------------------------------------

void IEclass::killkillbuf()
{

    while(Edit__killbuf)
    {
        if(Undo->FixPushLine(Edit__killbuf))
        {
            if(Edit__killbuf->prev)
            {
                Edit__killbuf = Edit__killbuf->prev;
                Edit__killbuf->next = NULL;
            }
            else
            {
                Edit__killbuf = NULL;
            }
        }
        else
        {
            if(Edit__killbuf->prev)
            {
                Edit__killbuf = Edit__killbuf->prev;
                throw_xdelete(Edit__killbuf->next);
            }
            else
                throw_xdelete(Edit__killbuf);
        }
    }
}


//  ------------------------------------------------------------------

void FreePastebuf()
{

    Edit__killpastebuf();
    Edit__killkillbuf();
}


//  ------------------------------------------------------------------

void IEclass::ClearDeleteBuf()
{

    GFTRK("EditClearDeleteBuf");

    killkillbuf();
    HandleGEvent(EVTT_JOBDONE);

    GFTRK(0);
}


//  ------------------------------------------------------------------

void IEclass::ClearPasteBuf()
{

    GFTRK("EditClearPasteBuf");

    killpastebuf();
    HandleGEvent(EVTT_JOBDONE);

    GFTRK(0);
}


//  ------------------------------------------------------------------

void IEclass::GoBegLine()
{

    GFTRK("EditGoBegLine");

    col = mincol;

    if(blockcol != -1)
        displine(currline, row);

    GFTRK(0);
}


//  ------------------------------------------------------------------

void IEclass::DelLtWord()
{

    GFTRK("EditDelLtWord");

    if(col == 0)
    {
        DelLeft();
        GFTRK(0);
        return;
    }

    int _ptr, _ptr2;
    _ptr = _ptr2 = col;
    _ptr--;

    // test test test test     test test test test
    // test test test test ,   test test test test
    // test test test test ,,  test test test test

    if(currline->txt[_ptr] == ' ')
    {
        while((currline->txt[_ptr] == ' ') and (_ptr > 0))
            _ptr--;
        if(currline->txt[_ptr] != ' ')
            _ptr++;
    }
    else if(isxalnum(currline->txt[_ptr]))
    {
        while(isxalnum(currline->txt[_ptr]) and (_ptr > 0))
            _ptr--;
        while((currline->txt[_ptr] == ' ') and (_ptr > 0))
            _ptr--;
        if((currline->txt[_ptr] != ' ') and (_ptr > 0))
            _ptr++;
    }
    else
    {
        DelLeft();
        GFTRK(0);
        return;
    }

    col -= _ptr2-_ptr;

    Undo->PushItem(EDIT_UNDO_DEL_TEXT, currline, col, _ptr2-_ptr);
    currline->txt.erase(_ptr, _ptr2-_ptr);

    wrapdel(&currline, &col, &row);

    GFTRK(0);
}


//  ------------------------------------------------------------------

void IEclass::DelRtWord()
{

    GFTRK("EditDelRtWord");

    if((currline->txt.length() == col+1) or (currline->txt[col+1] == '\n'))
    {
        DelChar();
        GFTRK(0);
        return;
    }

    int _ptr, _ptr2;
    _ptr = _ptr2 = col;

    // test test test test,   test test test test
    // test test test test,   test test test test

    if(currline->txt[_ptr] == ' ')
    {
        while(_ptr != currline->txt.length() and currline->txt[_ptr] == ' ')
            _ptr++;
    }
    else if(isxalnum(currline->txt[_ptr]))
    {
        // Delete word
        while(_ptr != currline->txt.length() and isxalnum(currline->txt[_ptr]))
            _ptr++;
        // Delete spaces after word
        while(_ptr != currline->txt.length() and currline->txt[_ptr] == ' ')
            _ptr++;
    }
    else
    {
        DelChar();
        GFTRK(0);
        return;
    }

    Undo->PushItem(EDIT_UNDO_DEL_TEXT, currline, col, _ptr-_ptr2);
    currline->txt.erase(_ptr2, _ptr-_ptr2);

    wrapdel(&currline, &col, &row);

    GFTRK(0);
}


//  ------------------------------------------------------------------

void IEclass::GoTopMsg()
{

    GFTRK("EditGoTopMsg");

    currline = findfirstline();
    col = mincol;
    row = minrow;
    thisrow = 0;
    refresh(currline, minrow);

    GFTRK(0);
}


//  ------------------------------------------------------------------

void IEclass::GoBotMsg()
{

    GFTRK("EditGoBotMsg");

    col = mincol;
    thisrow = 0;
    currline = findfirstline();

    // Go to the last line in the msg
    while(currline->next)
    {
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

    GFTRK(0);
}


//  ------------------------------------------------------------------

void IEclass::GoTopLine()
{

    GFTRK("EditGoTopLine");

    int _count = row;

    while(_count-- and currline->prev)
    {
        currline = currline->prev;
        thisrow--;
    }

    col = mincol;
    row = minrow;

    if(blockcol != -1)
        refresh(currline, minrow);

    GFTRK(0);
}


//  ------------------------------------------------------------------

void IEclass::GoBotLine()
{

    GFTRK("EditGoBotLine");

    Line *_oldcurrline = currline;
    int  _oldrow = row;

    while((row < maxrow) and currline->next)
    {
        currline = currline->next;
        thisrow++;
        row++;
    }

    col = mincol;

    if(blockcol != -1)
        refresh(_oldcurrline, _oldrow);

    GFTRK(0);
}


//  ------------------------------------------------------------------

Line* IEclass::findanchor()
{

    GFTRK("Editfindanchor");

    // Rewind to the first line
    Line* _anchor = findfirstline();

    // Search all lines to find the anchor (a line with a block mark)
    while(not (_anchor->type & GLINE_BLOK) and _anchor->next)
        _anchor = _anchor->next;

    GFTRK(0);

    // Return pointer to the anchor line or NULL if no anchor was found
    return (_anchor->type & GLINE_BLOK) ? _anchor : (Line*)NULL;
}


//  ------------------------------------------------------------------

void IEclass::BlockAnchor()
{

    GFTRK("EditBlockAnchor");

    Line* _anchor = findanchor();

    // Is there an anchor already?
    if(_anchor)
    {

        // Yes, so replace it with the current line

        // Remove block mark
        _anchor->type &= ~GLINE_BLOK;
        blockcol = -1;

        // Is the old anchor different from the current line?
        if(_anchor != currline)
        {

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
    else
    {

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

    GFTRK(0);
}


//  ------------------------------------------------------------------

void IEclass::BlockCopy()
{

    GFTRK("EditBlockCopy");

    // Find the anchor, if any
    Line* _anchor = findanchor();

    // Did we find the anchor?
    if(_anchor)
    {

        Line* _firstcopyline = currline;
        Line* _lastcopyline  = currline;
        int   firstcol = col, lastcol = col;

        // Search below to find the anchor line
        while(_lastcopyline->next and (_lastcopyline != _anchor))
            _lastcopyline = _lastcopyline->next;

        // Was the anchor line above or on the current line?
        if(_lastcopyline != _anchor)
        {

            // The last copy line is the current line
            _lastcopyline = currline;

            // Search above to find the anchor line
            while(_firstcopyline->prev and (_firstcopyline != _anchor))
                _firstcopyline = _firstcopyline->prev;
            firstcol = blockcol;
        }
        else
        {
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
        while(1)
        {

            // Allocate a new line
            Line* _copyline;

            // Copy text and type
            if(_prevline == NULL)
                _copyline = new Line(_firstcopyline->txt.c_str() + firstcol);
            else
                _copyline = new Line(_firstcopyline->txt.c_str());
            throw_xnew(_copyline);
            if(_firstcopyline == _lastcopyline)
            {
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

    GFTRK(0);
}


//  ------------------------------------------------------------------

void IEclass::BlockDel(Line* anchor)
{

    GFTRK("EditBlockDel");

    Line* firstcutline = currline;
    Line* lastcutline  = currline;
    uint firstcutlinerow = row;
    uint lastcutlinerow  = row;
    int  firstcol = col, lastcol = col;

    // Search below to find the anchor line
    while(lastcutline->next and (lastcutline != anchor))
    {
        lastcutline = lastcutline->next;
        lastcutlinerow++;
    }

    // Was the anchor line above or on the current line?
    if(lastcutline != anchor)
    {

        // The last cut line is the current line
        lastcutline = currline;
        lastcutlinerow = row;

        // Search above to find the anchor line
        while(firstcutline->prev and (firstcutline != anchor))
        {
            firstcutline = firstcutline->prev;
            if(firstcutlinerow)
                firstcutlinerow--;
        }
        firstcol = blockcol;
    }
    else
    {
        if((currline != anchor) or (blockcol > col))
            lastcol = blockcol;
        else
            firstcol = blockcol;
    }

    // The firstcutline and lastcutline pointers
    // are now pointing where they should

    if(firstcutline != lastcutline)
    {
        size_t __len = firstcutline->txt.length();
        firstcutline->txt += lastcutline->txt.c_str()+lastcol;

        // We need to set up prow to prevent cosmetic bugs
        uint __prow = prow;
        getthisrow(firstcutline);
        prow = thisrow;
        getthisrow(currline);
        Undo->PushItem(EDIT_UNDO_INS_TEXT, firstcutline, __len);
        prow = __prow;

        Undo->PushItem(EDIT_UNDO_DEL_TEXT|BATCH_MODE, firstcutline, firstcol, __len-firstcol);
        firstcutline->txt.erase(firstcol, __len-firstcol);
    }
    else
    {
        Undo->PushItem(EDIT_UNDO_DEL_TEXT, firstcutline, firstcol, lastcol-firstcol);
        firstcutline->txt.erase(firstcol, lastcol-firstcol);
    }
    setlinetype(firstcutline);
    firstcutline->type &= ~GLINE_BLOK;
    blockcol = -1;
    currline = firstcutline;
    row = firstcutlinerow;
    col = firstcol;
    if(firstcutline != lastcutline)
    {
        do
        {
            Undo->PushItem(EDIT_UNDO_DEL_LINE|BATCH_MODE, firstcutline = firstcutline->next);
        }
        while(firstcutline != lastcutline);

        currline->next = lastcutline->next;
        if(lastcutline->next)
            lastcutline->next->prev = currline;
    }

    // Refresh the display
    if(not RngV(row, minrow, maxrow))
    {
        row = minrow;
        wrapdel(&currline, &col, &row, false);
        refresh(currline, minrow);
    }
    else
    {
        row = MaxV(firstcutlinerow, minrow);
        Line* topline = findtopline();
        wrapdel(&currline, &col, &row, false);
        refresh(topline, minrow);
    }

    Line* line;
    for(line = findfirstline(); line; line = line->next)
        line->type &= ~GLINE_BLOK;

    selecting = NO;

    GFTRK(0);
}


//  ------------------------------------------------------------------

void IEclass::BlockCut(bool just_delete)
{

    GFTRK("EditBlockCut");

    // Find the anchor, if any
    Line* _anchor = findanchor();

    // Did we find the anchor?
    if(_anchor)
    {

        int _blockcol = blockcol;

        if(not just_delete)
            BlockCopy();

        blockcol = _blockcol;

        BlockDel(_anchor);
    }
    else
        killpastebuf();

    GFTRK(0);
}


//  ------------------------------------------------------------------

void IEclass::BlockPaste()
{

    GFTRK("EditBlockPaste");

    killpastebuf();
    Clip2Buf();

    if(Edit__pastebuf)
    {

        Line* _pasteline = Edit__pastebuf;

        if(not batch_mode)
            Undo->PushItem(EDIT_UNDO_VOID);

        // For each of the lines in the paste buffer
        while(_pasteline)
        {

            uint  curlen = currline->txt.length();
            uint  pastelen = _pasteline->txt.length();

            if(col > curlen)
                col = curlen;

            if(_pasteline->txt.find('\n') != _pasteline->txt.npos)
            {
                // append to current line
                Undo->PushItem(EDIT_UNDO_DEL_TEXT|BATCH_MODE, currline, col);
                Line* _newline = insertlinebelow(currline, currline->txt.c_str()+col, BATCH_MODE);
                currline->txt.erase(col);
                currline->txt += _pasteline->txt;
                Undo->PushItem(EDIT_UNDO_INS_TEXT|BATCH_MODE, currline, col, pastelen);
                setlinetype(currline);
                col = currline->txt.length();
                wrapins(&currline, &col, &row, false);
                currline = _newline;
                col = 0;
                if(row < maxrow)
                    row++;
            }
            else
            {
                // insert into current line
                currline->txt.insert(col, _pasteline->txt);
                Undo->PushItem(EDIT_UNDO_INS_TEXT|BATCH_MODE, currline, col, pastelen);
                col += pastelen;
                wrapins(&currline, &col, &row, false);
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

    GFTRK(0);
}


//  ------------------------------------------------------------------

void IEclass::LoadFile()
{

    GFTRK("EditLoadFile");

    // Open the file to load
    gfile _fp(AddPath(CFG->goldpath, EDIT->File()), "rb", CFG->sharemode);
    if (_fp.isopen())
    {
        XlatName __oldxlatimport;

        // Pop up a wait window
        cursoroff();
        w_info(LNG->Wait);

        throw_delete(Undo);

        // Find the first line
        Line* _line = findfirstline();

        // Remove all lines
        while(_line)
        {
            Line* _nextline = _line->next;
            throw_xdelete(_line);
            _line = _nextline;
        }

        // Remove message text and reset pointers
        throw_release(msgptr->txt);
        currline = msgptr->lin = NULL;

        // Allocate space for new message text
        msgptr->txt = (char*)throw_calloc(1, (uint)(_fp.FileLength()+256));

        // Eat the backup marking line
        char _buf[EDIT_BUFLEN];
        _fp.Fgets(_buf, sizeof(_buf));
        if (not striinc(unfinished, _buf))
            _fp.Rewind();

        // Load the file and close it
        _fp.Fread(msgptr->txt, (uint)_fp.FileLength());
        _fp.Fclose();

        // Save current charset
        strcpy(__oldxlatimport, AA->Xlatimport());
        AA->SetXlatimport(CFG->xlatlocalset);

        // Index message text
        msgptr->TextToLines(margintext-1, false);
        _line = currline = msgptr->lin;

        // Restore charset
        AA->SetXlatimport(__oldxlatimport);

        // Change lines to internal editor format
        while(_line)
        {
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

        if(l->next == NULL)
            row = i;
        currline = l;
        col = mincol;

        throw_new(Undo = new UndoStack(this));
    }

    GFTRK(0);
}


//  ------------------------------------------------------------------

inline char uuencode_enc(int c)
{
    return (char)(c ? (c & 077) + ' ': '`');
}

void IEclass::editimport(Line* __line, char* __filename, bool imptxt)
{

    XlatName __oldxlatimport;

    GFTRK("Editimport");

    // Save the unfinished msg first of all
    savefile(MODE_UPDATE);

    update_statusline(LNG->ImportFile);

    // Set initial import filename or wildcards
    if(__filename)
    {
        AA->SetInputfile(__filename);
    }
    else
    {
        if(*AA->Inputfile() == NUL)
            AA->SetInputfile("*");
    }

    strcpy(__oldxlatimport, AA->Xlatimport());
    AA->SetXlatimport(CFG->xlatlocalset);

    strcpy(CFG->inputfile, AA->Inputfile());

    int ImportMode;

    if(imptxt)
        ImportMode = 0;
    else
    {

        GMenuImportTxt MenuImportTxt;

        ImportMode = MenuImportTxt.Run();
    }

    std::string filenamebuf;
    Path tmpfile;
    bool isPipe = NO;
    bool fileselected = false;

    // Should the imported text be quoted or uuencoded?
#define quoteit    (ImportMode == 1)
#define uuencode   (ImportMode == 2)
#define base64     (ImportMode == 3)
#define getclip    (ImportMode == 4)
#define binary     (uuencode or base64)

    if(in_range(ImportMode, 0, 3) and edit_pathname(CFG->inputfile, sizeof(Path), LNG->ImportWhichFile, H_ImportFile))
    {

        AA->SetInputfile(CFG->inputfile);

        // Pointer to the filename string
        filenamebuf = AA->Inputfile();

        if(filenamebuf.c_str()[0] == '|')
        {
            Path cmdline;

            if( not ((*CFG->temppath == NUL ) or is_dir(CFG->temppath)) )
            {
                w_info(LNG->TMPinvalidpath);
                SayBibi();
                waitkeyt(10000);
                w_info("");
                AA->SetXlatimport(__oldxlatimport);
                GFTRK(0);
                return;
            }

            isPipe = YES;
            mktemp(strxcpy(tmpfile, AddPath(CFG->temppath, "GIXXXXXX"), sizeof(Path)));
            strxmerge(cmdline, sizeof(Path), filenamebuf.c_str()+1, " > ", tmpfile, NULL);
            ShellToDos(cmdline, "", BLACK_|_BLACK, NO);
            filenamebuf = tmpfile;
            fileselected = true;
        }
        else
        {
            // Check for wildcards
            // Is the filename a directory?
            if(is_dir(filenamebuf))
            {

                // Does the filename contain wildcards?
                if(not strpbrk(filenamebuf.c_str(), "*?"))
                {

                    // Add match-all wildcards
                    AddBackslash(filenamebuf);
                    filenamebuf += "*";
                }
            }

            fileselected = true;

            // Does the filename contain wildcards?
            if(strpbrk(filenamebuf.c_str(), "*?"))
            {

                // Set selection window title and statusline
                set_title(LNG->ImportTitle, TCENTER, C_MENUT);
                update_statuslinef(LNG->ImportStatus, "ST_IMPORTSTATUS", filenamebuf.c_str());

                // Start the file picker
                fileselected = wpickfile(win_minrow, win_mincol, win_maxrow, win_maxcol, W_BMENU, C_MENUB, C_MENUW, C_MENUS, NO, filenamebuf, maketitle_and_status);
            }
        }
    }

    if (fileselected or getclip)
    {
        // Open the file/clipboard
        gfile fp;
        gclipbrd clipbrd;

        if(getclip)
            filenamebuf = CLIP_NAME;

        if(getclip ? clipbrd.openread() :
                (fp.Fopen(filenamebuf.c_str(), binary ? "rb" : "rt", CFG->sharemode))!=NULL)
        {
            if (isPipe)
                filenamebuf = AA->Inputfile();

            const char *imp_filename = (getclip or isPipe) ? filenamebuf.c_str() : CleanFilename(filenamebuf.c_str());

            // we need to truncate filename to prevent unpredictable results
            int delta = strlen(imp_filename) - margintext;
            if(delta > 0)
            {
                filenamebuf.erase(filenamebuf.length()-delta);
                imp_filename = (getclip or isPipe) ? filenamebuf.c_str() : CleanFilename(filenamebuf.c_str());
            }

            update_statuslinef(LNG->ImportStatus, "ST_IMPORTSTATUS", filenamebuf.c_str());

            // Allocate paragraph read buffer
            char* _parabuf = (char*)throw_malloc(EDIT_PARABUFLEN);

            if(__line->prev)
                Undo->PushItem(EDIT_UNDO_VOID|PREV_LINE|batch_mode, __line->prev);
            else
                Undo->PushItem(EDIT_UNDO_VOID|batch_mode, __line);
            batch_mode = BATCH_MODE;

            // Add import begin text, if any
            if (*CFG->importbegin)
            {
                sprintf(_parabuf, "%s\n", CFG->importbegin);
                strischg(_parabuf, "@file", imp_filename);
                TokenXlat(MODE_NEW, _parabuf, EDIT_PARABUFLEN, true, msgptr, msgptr, CurrArea);
                _parabuf[strlen(_parabuf)-1] = '\n';
                _parabuf[margintext] = NUL;
                _parabuf[margintext-1] = '\n';
                __line = insertlinebelow(__line, _parabuf);
                setlinetype(__line);
            }

            if(uuencode)
            {

                sprintf(_parabuf, "begin 644 %s\n", imp_filename);
                _parabuf[margintext] = NUL;
                _parabuf[margintext-1] = '\n';
                __line = insertlinebelow(__line, _parabuf);
                setlinetype(__line);

                while (true)
                {
                    char ibuf[80];
                    char* iptr = ibuf;
                    char* optr = _parabuf;
                    int n = fp.Fread(ibuf, 1, 45);
                    if (n < 45) memset(ibuf+n, 0, 45-n);
                    *optr++ = uuencode_enc(n);

                    for(int i=0; i<n; i+=3,iptr+=3)
                    {
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
            else if(base64)
            {

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

                while (true)
                {
                    char ibuf[80];
                    char* optr = _parabuf;
                    int n = fp.Fread(ibuf, 1, 54);
                    optr = b64.encode(optr, ibuf, n);
                    *optr++ = '\n';
                    *optr = NUL;
                    __line = insertlinebelow(__line, _parabuf);
                    setlinetype(__line);
                    if(n <= 0)
                        break;
                }
            }
            else
            {

                int tabsz = CFG->disptabsize ? CFG->disptabsize : 1;
                int level = LoadCharset(AA->Xlatimport(), CFG->xlatlocalset);
                char* buf = (char*) throw_malloc(EDIT_PARABUFLEN);
                Line* saveline = __line->next;
                __line->next = NULL;

                // Read paragraphs
                while (getclip ? clipbrd.read(_parabuf, EDIT_PARABUFLEN-7) : fp.Fgets(_parabuf, EDIT_PARABUFLEN-7))
                {
                    XlatStr(buf, _parabuf, level, CharTable);

                    std::string tmp = buf;
                    throw_free(buf);

                    // Insert a quotestring if asked
                    if(quoteit)
                        tmp.insert(0, " > ");
                    else
                    {
                        // Invalidate tearline
                        if(not CFG->invalidate.tearline.first.empty())
                            doinvalidate(tmp, CFG->invalidate.tearline.first, CFG->invalidate.tearline.second, true);

                        // Invalidate originline
                        if(not CFG->invalidate.origin.first.empty())
                            doinvalidate(tmp, CFG->invalidate.origin.first, CFG->invalidate.origin.second);

                        // Invalidate SEEN-BY's
                        if(not CFG->invalidate.seenby.first.empty())
                            doinvalidate(tmp, CFG->invalidate.seenby.first, CFG->invalidate.seenby.second);

                        // Invalidate CC's
                        if(not CFG->invalidate.cc.first.empty())
                            doinvalidate(tmp, CFG->invalidate.cc.first, CFG->invalidate.cc.second);

                        // Invalidate XC's
                        if(not CFG->invalidate.xc.first.empty())
                            doinvalidate(tmp, CFG->invalidate.xc.first, CFG->invalidate.xc.second);

                        // Invalidate XP's
                        if(not CFG->invalidate.xp.first.empty())
                            doinvalidate(tmp, CFG->invalidate.xp.first, CFG->invalidate.xp.second);
                    }

                    // Replace tabs

                    for (std::string::size_type n = 0; (n = tmp.find('\t', n) != std::string::npos); ++n)
                    {
                        tmp[n] = ' ';
                        const int rstart = n % tabsz + 1;
                        if (tabsz > rstart)
                        {
                            const int ins = tabsz - rstart;
                            tmp.insert(n + 1, ins, ' ');
                            n += ins;
                        }
                    }

                    // Copy the paragraph to the new line and retype it
                    Line* _newline = __line = insertlinebelow(__line, tmp.c_str());
                    setlinetype(_newline);

                    // If the paragraph is longer than one line
                    uint _wrapmargin = (_newline->type & GLINE_QUOT) ? marginquotes : margintext;
                    if(_newline->txt.length() >= _wrapmargin)
                    {

                        // Wrap it
                        uint _tmpcol = 0;
                        uint _tmprow = 0;
                        _newline = wrapins(&_newline, &_tmpcol, &_tmprow, false);
                    }

                    __line = _newline;
                }

                while(__line->next)
                    __line = __line->next;
                if(not __line->txt.empty() and (*(__line->txt.end()-1) != '\n'))
                {
                    Undo->PushItem(EDIT_UNDO_INS_CHAR|BATCH_MODE, __line, __line->txt.length());
                    __line->txt += '\n';
                    // Wrap it
                    uint _tmpcol = 0;
                    uint _tmprow = 0;
                    __line = wrapins(&__line, &_tmpcol, &_tmprow, false);
                }
                __line->next = saveline;
                if(saveline)
                    saveline->prev = __line;
            }

            // Add import end text, if any
            if (*CFG->importend or *CFG->importbegin)
            {
                sprintf(_parabuf, "%s\n", *CFG->importend ? CFG->importend : CFG->importbegin);
                strischg(_parabuf, "@file", imp_filename);
                TokenXlat(MODE_NEW, _parabuf, EDIT_PARABUFLEN, true, msgptr, msgptr, CurrArea);
                _parabuf[strlen(_parabuf)-1] = '\n';
                _parabuf[margintext] = NUL;
                _parabuf[margintext-1] = '\n';
                __line = insertlinebelow(__line, _parabuf);
                setlinetype(__line);
            }

            throw_free(_parabuf);

            if (getclip)
                clipbrd.close();
            else
                fp.Fclose();
        }
        else
        {
            w_infof(LNG->CouldNotOpen, filenamebuf.c_str());
            waitkeyt(10000);
            w_info(NULL);
        }

        if(isPipe)
            unlink(tmpfile);
    }

    AA->SetXlatimport(__oldxlatimport);

    GFTRK(0);
}


//  ------------------------------------------------------------------

void IEclass::imptxt(char* __filename, bool imptxt)
{

    GFTRK("Editimptxt");

    msgptr->lin = findfirstline();
    editimport(currline, __filename, imptxt);
    refresh(currline, row);
    col = mincol;

    GFTRK(0);
}


//  ------------------------------------------------------------------

void IEclass::ImportText()
{

    GFTRK("EditImportText");

    imptxt(NULL);

    GFTRK(0);
}


//  ------------------------------------------------------------------

void IEclass::ImportQuotebuf()
{

    GFTRK("EditImportQuotebuf");

    Path _quotebuf;

    GetCurrQuotebuf(_quotebuf);
    imptxt(_quotebuf, true);

    GFTRK(0);
}


//  ------------------------------------------------------------------

void IEclass::editexport(Line* __exportline, int __endat)
{

    GFTRK("Editexport");

    update_statusline(LNG->ExportFile);

    if (edit_string(Edit__exportfilename, sizeof(Path), LNG->ExportWhatFile, H_ExportFile))
    {
        // Pointer to export filename
        char* _filenameptr = Edit__exportfilename;

        // Is append requested?
        if(*_filenameptr == '+')
            _filenameptr++;

        gfile _fp(_filenameptr, (*Edit__exportfilename == '+') ? "at" : "wt", CFG->sharemode);
        if (_fp.isopen())
        {
            update_statuslinef(LNG->ExportStatus, "ST_EXPORTSTATUS", Edit__exportfilename);
            _fp.Fputc('\n');

            while ((__endat ? __exportline != currline : 1) and __exportline)
            {
                _fp.Fwrite(__exportline->txt.c_str(), __exportline->txt.length());
                if (__exportline->txt.find('\n') == __exportline->txt.npos)
                    _fp.Fputc('\n');
                __exportline = __exportline->next;
            }
        }
    }

    GFTRK(0);
}


//  ------------------------------------------------------------------

void IEclass::SpellCheck()
{
    GFTRK("EditSpellCheck");

    char _buf[EDIT_BUFLEN];
    char _buf2[EDIT_BUFLEN];

    savefile(MODE_SAVE);
    strcpy(_buf, EDIT->SpellChecker());
    strcpy(_buf2, AddPath(CFG->goldpath, EDIT->File()));
    strchg(_buf2, GOLD_WRONG_SLASH_CHR, GOLD_SLASH_CHR);
    strischg(_buf, "@file", _buf2);
    gsprintf(PRINTF_DECLARE_BUFFER(_buf2), LNG->SpellChecker, _buf);
    ShellToDos(_buf, _buf2, LGREY_|_BLACK, YES);
    LoadFile();

    GFTRK(0);
}


//  ------------------------------------------------------------------

void IEclass::ExportText()
{

    GFTRK("EditExportText");

    int endat = NO;
    // Line* exportline = findanchor();

    // if(exportline)
    //  endat = YES;
    // else
    Line* exportline = findfirstline();

    editexport(exportline, endat);

    GFTRK(0);
}


//  ------------------------------------------------------------------

void IEclass::DosShell()
{

    GFTRK("EditDosShell");

    char * shell = getenv(GOLD_SHELL_ENV);
    if( shell && *shell )
        ShellToDos(shell, LNG->DOS_Shell, LGREY_|_BLACK, YES);
    cursoron();
    cursoroff();

    GFTRK(0);
}


//  ------------------------------------------------------------------

void IEclass::dispins()
{

    GFTRK("Editdispins");

    if(insert)
        HeaderView->window.prints(5, MAXCOL-6, C_HEADT, LNG->Ins);
    else
    {
        vchar _lbuf[6];
        for(int c = 0; c < 5; c++)
            _lbuf[c] = _box_table(W_BHEAD,1);
        _lbuf[5] = NUL;
        HeaderView->window.printvs(5, MAXCOL-6, C_HEADB|ACSET, _lbuf);
    }

    GFTRK(0);
}


//  ------------------------------------------------------------------

void IEclass::dispdl()
{
    GFTRK("Editdispdl");

    if (drawlines)
    {
        const char *lng = (drawlines == 1) ? LNG->DrawSL : LNG->DrawDL;
        HeaderView->window.prints(5, MAXCOL-12, C_HEADT, lng);
    }
    else
    {
        vchar _lbuf[6];
        for(int c = 0; c < 5; c++)
            _lbuf[c] = _box_table(W_BHEAD,1);
        _lbuf[5] = NUL;
        HeaderView->window.printvs(5, MAXCOL-12, C_HEADB|ACSET, _lbuf);
    }

    GFTRK(0);
}


//  ------------------------------------------------------------------

void IEclass::ToggleInsert()
{

    GFTRK("EditToggleInsert");

    insert = not insert;
    dispins();

    GFTRK(0);
}


//  ------------------------------------------------------------------

void IEclass::ToggleDrawLines()
{
    GFTRK("EditToggleDrawLines");

    switch (drawlines)
    {
    case 0:
        drawlines = 1;
        break;
    case 1:
        drawlines = 2;
        break;
    case 2:
        drawlines = 0;
        break;
    }

    dispdl();

    GFTRK(0);
}


//  ------------------------------------------------------------------

static void ChrToLines(char chr, byte lines[4])
{
    lines[0] = lines[1] = lines[2] = lines[3] = 0;

    if      (chr == _box_table(0, 1))           //hs
        lines[0] = lines[1] = 1;
    else if (chr == _box_table(1, 1))           //hd
        lines[0] = lines[1] = 2;
    else if (chr == _box_table(0, 3))           //vs
        lines[2] = lines[3] = 1;
    else if (chr == _box_table(1, 3))           //vd
        lines[2] = lines[3] = 2;

    else if (chr == _box_table(0, 0))           //lsus
        lines[1] = lines[3] = 1;
    else if (chr == _box_table(1, 0))           //ldud
        lines[1] = lines[3] = 2;
    else if (chr == _box_table(0, 11))          //vsus
        lines[0] = lines[1] = lines[3] = 1;
    else if (chr == _box_table(1, 11))          //vdud
        lines[0] = lines[1] = lines[3] = 2;
    else if (chr == _box_table(0, 2))           //rsus
        lines[0] = lines[3] = 1;
    else if (chr == _box_table(1, 2))           //rdud
        lines[0] = lines[3] = 2;

    else if (chr == _box_table(0, 9))           //lshs
        lines[1] = lines[2] = lines[3] = 1;
    else if (chr == _box_table(1, 9))           //ldhd
        lines[1] = lines[2] = lines[3] = 2;
    else if (chr == _box_table(0, 8))           //vshs
        lines[0] = lines[1] = lines[2] = lines[3] = 1;
    else if (chr == _box_table(1, 8))           //vdhd
        lines[0] = lines[1] = lines[2] = lines[3] = 2;
    else if (chr == _box_table(0, 10))          //rshs
        lines[0] = lines[2] = lines[3] = 1;
    else if (chr == _box_table(1, 10))          //rdhd
        lines[0] = lines[2] = lines[3] = 2;

    else if (chr == _box_table(0, 5))           //lsds
        lines[1] = lines[2] = 1;
    else if (chr == _box_table(1, 5))           //lddd
        lines[1] = lines[2] = 2;
    else if (chr == _box_table(0, 12))          //vsds
        lines[0] = lines[1] = lines[2] = 1;
    else if (chr == _box_table(1, 12))          //vddd
        lines[0] = lines[1] = lines[2] = 2;
    else if (chr == _box_table(0, 7))           //rsds
        lines[0] = lines[2] = 1;
    else if (chr == _box_table(1, 7))           //rddd
        lines[0] = lines[2] = 2;

    else if (chr == _box_table(2, 0))           //ldus
    {
        lines[1] = 1;
        lines[3] = 2;
    }
    else if (chr == _box_table(3, 0))           //lsud
    {
        lines[1] = 2;
        lines[3] = 1;
    }
    else if (chr == _box_table(2, 11))          //vdus
    {
        lines[0] = lines[1] = 1;
        lines[3] = 2;
    }
    else if (chr == _box_table(3, 11))          //vsud
    {
        lines[0] = lines[1] = 2;
        lines[3] = 1;
    }
    else if (chr == _box_table(2, 2))           //rdus
    {
        lines[0] = 1;
        lines[3] = 2;
    }
    else if (chr == _box_table(3, 2))           //rsud
    {
        lines[0] = 2;
        lines[3] = 1;
    }

    else if (chr == _box_table(2, 9))           //ldhs
    {
        lines[1] = 1;
        lines[2] = lines[3] = 2;
    }
    else if (chr == _box_table(3, 9))           //lshd
    {
        lines[1] = 2;
        lines[2] = lines[3] = 1;
    }
    else if (chr == _box_table(2, 8))           //vdhs
    {
        lines[0] = lines[1] = 1;
        lines[2] = lines[3] = 2;
    }
    else if (chr == _box_table(3, 8))           //vshd
    {
        lines[0] = lines[1] = 2;
        lines[2] = lines[3] = 1;
    }
    else if (chr == _box_table(2, 10))          //rdhs
    {
        lines[0] = 1;
        lines[2] = lines[3] = 2;
    }
    else if (chr == _box_table(3, 10))          //rshd
    {
        lines[0] = 2;
        lines[2] = lines[3] = 1;
    }

    else if (chr == _box_table(2, 5))           //ldds
    {
        lines[1] = 1;
        lines[2] = 2;
    }
    else if (chr == _box_table(3, 5))           //lsdd
    {
        lines[1] = 2;
        lines[2] = 1;
    }
    else if (chr == _box_table(2, 12))          //vdds
    {
        lines[0] = lines[1] = 1;
        lines[2] = 2;
    }
    else if (chr == _box_table(3, 12))          //vsdd
    {
        lines[0] = lines[1] = 2;
        lines[2] = 1;
    }
    else if (chr == _box_table(2, 7))           //rdds
    {
        lines[0] = 1;
        lines[2] = 2;
    }
    else if (chr == _box_table(3, 7))           //rsdd
    {
        lines[0] = 2;
        lines[2] = 1;
    }
}


//  ------------------------------------------------------------------

static char LinesToChr(byte lines[4])
{
    if            (lines[0] == 0)
    {
        if          (lines[1] == 0)
        {
            if        (lines[2] == 0)
            {
                if      (lines[3] == 1) return _box_table(0, 3);    //vs
                if      (lines[3] == 2) return _box_table(1, 3);    //vd
            }
            else if (lines[2] == 1)
            {
                if      (lines[3] == 0) return _box_table(0, 3);    //vs
                if      (lines[3] == 1) return _box_table(0, 3);    //vs
            }
            else if (lines[2] == 2)
            {
                if      (lines[3] == 0) return _box_table(1, 3);    //vd
                if      (lines[3] == 2) return _box_table(1, 3);    //vd
            }
        }
        else if   (lines[1] == 1)
        {
            if        (lines[2] == 0)
            {
                if      (lines[3] == 0) return _box_table(0, 1);    //hs
                if      (lines[3] == 1) return _box_table(0, 0);    //lsus
                if      (lines[3] == 2) return _box_table(2, 0);    //ldus
            }
            else if (lines[2] == 1)
            {
                if      (lines[3] == 0) return _box_table(0, 5);    //lsds
                if      (lines[3] == 1) return _box_table(0, 9);    //lshs
            }
            else if (lines[2] == 2)
            {
                if      (lines[3] == 0) return _box_table(2, 5);    //ldds
                if      (lines[3] == 2) return _box_table(2, 9);    //ldhs
            }
        }
        else if   (lines[1] == 2)
        {
            if        (lines[2] == 0)
            {
                if      (lines[3] == 0) return _box_table(1, 1);    //hd
                if      (lines[3] == 1) return _box_table(3, 0);    //lsud
                if      (lines[3] == 2) return _box_table(1, 0);    //ldud
            }
            else if (lines[2] == 1)
            {
                if      (lines[3] == 0) return _box_table(3, 5);    //lsdd
                if      (lines[3] == 1) return _box_table(3, 9);    //lshd
            }
            else if (lines[2] == 2)
            {
                if      (lines[3] == 0) return _box_table(1, 5);    //lddd
                if      (lines[3] == 2) return _box_table(1, 9);    //ldhd
            }
        }
    }
    else if     (lines[0] == 1)
    {
        if          (lines[1] == 0)
        {
            if        (lines[2] == 0)
            {
                if      (lines[3] == 0) return _box_table(0, 1);    //hs
                if      (lines[3] == 1) return _box_table(0, 2);    //rsus
                if      (lines[3] == 2) return _box_table(2, 2);    //rdus
            }
            else if (lines[2] == 1)
            {
                if      (lines[3] == 0) return _box_table(0, 7);    //rsds
                if      (lines[3] == 1) return _box_table(0, 10);   //rshs
            }
            else if (lines[2] == 2)
            {
                if      (lines[3] == 0) return _box_table(2, 7);    //rdds
                if      (lines[3] == 2) return _box_table(2, 10);   //rdhs
            }
        }
        else if   (lines[1] == 1)
        {
            if        (lines[2] == 0)
            {
                if      (lines[3] == 0) return _box_table(0, 1);    //hs
                if      (lines[3] == 1) return _box_table(0, 11);   //vsus
                if      (lines[3] == 2) return _box_table(2, 11);   //vdus
            }
            else if (lines[2] == 1)
            {
                if      (lines[3] == 0) return _box_table(0, 12);   //vsds
                if      (lines[3] == 1) return _box_table(0, 8);    //vshs
            }
            else if (lines[2] == 2)
            {
                if      (lines[3] == 0) return _box_table(2, 12);   //vdds
                if      (lines[3] == 2) return _box_table(2, 8);    //vdhs
            }
        }
    }
    else if     (lines[0] == 2)
    {
        if          (lines[1] == 0)
        {
            if        (lines[2] == 0)
            {
                if      (lines[3] == 0) return _box_table(1, 1);    //hd
                if      (lines[3] == 1) return _box_table(3, 2);    //rsud
                if      (lines[3] == 2) return _box_table(1, 2);    //rdud
            }
            else if (lines[2] == 1)
            {
                if      (lines[3] == 0) return _box_table(3, 7);    //rsdd
                if      (lines[3] == 1) return _box_table(3, 10);   //rshd
            }
            else if (lines[2] == 2)
            {
                if      (lines[3] == 0) return _box_table(1, 7);    //rddd
                if      (lines[3] == 2) return _box_table(1, 10);   //rdhd
            }
        }
        else if   (lines[1] == 2)
        {
            if        (lines[2] == 0)
            {
                if      (lines[3] == 0) return _box_table(1, 1);   //hd
                if      (lines[3] == 1) return _box_table(3, 11);  //vsud
                if      (lines[3] == 2) return _box_table(1, 11);  //vdud
            }
            else if (lines[2] == 1)
            {
                if      (lines[3] == 0) return _box_table(3, 12);  //vsdd
                if      (lines[3] == 1) return _box_table(3, 8);   //vshd
            }
            else if (lines[2] == 2)
            {
                if      (lines[3] == 0) return _box_table(1, 12);  //vddd
                if      (lines[3] == 2) return _box_table(1, 8);   //vdhd
            }
        }
    }

    return 0x20;
}


//  ------------------------------------------------------------------

void IEclass::DrawLines(gkey key)
{
    GFTRK("EditDrawLines");

    static byte lines[4];
    static int drawx;
    static int drawy;

    byte type1 = 1;
    byte type2 = 1;

    (drawlines == 1) ? type2 = 2 : type1 = 2;

    //-------------------------
    if (drawflag || chartyped)
    {
        ChrToLines(currline->txt[col], lines);

        switch (key)
        {
        case KK_EditBlockRight:
            drawx = -1;
            drawy = 0;

            if ((lines[0] == type1) || (!lines[0] && (lines[2] || lines[3])))
                drawx++;
            else if ((lines[0] == type2) || (lines[1] == type2))
            {
                drawx++;
                lines[0] = lines[1] = 0;
            }

            break;

        case KK_EditBlockLeft:
            drawx = +1;
            drawy = 0;

            if ((lines[1] == type1) || (!lines[1] && (lines[2] || lines[3])))
                drawx--;
            else if ((lines[0] == type2) || (lines[1] == type2))
            {
                drawx--;
                lines[0] = lines[1] = 0;
            }

            break;

        case KK_EditBlockDown:
            drawx = 0;
            drawy = -1;

            if ((lines[2] == type1) || (!lines[2] && (lines[0] || lines[1])))
                drawy++;
            else if ((lines[2] == type2) || (lines[3] == type2))
            {
                drawy++;
                lines[2] = lines[3] = 0;
            }

            break;

        case KK_EditBlockUp:
            drawx = 0;
            drawy = +1;

            if ((lines[3] == type1) || (!lines[3] && (lines[0] || lines[1])))
                drawy--;
            else if ((lines[2] == type2) || (lines[3] == type2))
            {
                drawy--;
                lines[2] = lines[3] = 0;
            }

            break;
        }

        drawflag = false;
    }

    //-------------------------
    bool gonext = true;

    switch (key)
    {
    case KK_EditBlockRight:
        if (col >= (maxcol-1))
        {
            lines[1] = 0;
            gonext = false;
        }

        if (!drawx && gonext)
        {
            drawx = -1;
            lines[1] = type1;
            if (lines[0] == type2) lines[0] = 0;
        }
        else if (drawx == 1) drawx = -1;
        else if (drawx == -1)
        {
            gonext = false;
            drawx++;
            lines[0] = type1;
            if (lines[1] == type2) lines[1] = 0;
        }

        if (gonext && drawy)
        {
            if (drawy == -1)
            {
                drawy++;
                lines[2] = type1;
                lines[0] = lines[3] = 0;
            }
            else  //(drawy == 1)
            {
                drawy--;
                lines[3] = type1;
                lines[0] = lines[2] = 0;
            }
        }
        break;

    case KK_EditBlockLeft:
        if (col <= mincol)
        {
            lines[0] = 0;
            gonext = false;
        }

        if (!drawx && gonext)
        {
            drawx = 1;
            lines[0] = type1;
            if (lines[1] == type2) lines[1] = 0;
        }
        else if (drawx == -1) drawx = 1;
        else if (drawx == 1)
        {
            gonext = false;
            drawx--;
            lines[1] = type1;
            if (lines[0] == type2) lines[0] = 0;
        }

        if (gonext && drawy)
        {
            if (drawy == -1)
            {
                drawy++;
                lines[2] = type1;
                lines[1] = lines[3] = 0;
            }
            else  //(drawy == 1)
            {
                drawy--;
                lines[3] = type1;
                lines[1] = lines[2] = 0;
            }
        }
        break;

    case KK_EditBlockDown:
        if (!currline->next)
        {
            lines[3] = 0;
            gonext = false;
        }

        if (!drawy && gonext)
        {
            drawy = -1;
            lines[3] = type1;
            if (lines[2] == type2) lines[2] = 0;
        }
        else if (drawy == 1) drawy = -1;
        else if (drawy == -1)
        {
            gonext = false;
            drawy++;
            lines[2] = type1;
            if (lines[3] == type2) lines[3] = 0;
        }

        if (gonext && drawx)
        {
            if (drawx == -1)
            {
                drawx++;
                lines[0] = type1;
                lines[1] = lines[2] = 0;
            }
            else  //(drawx == 1)
            {
                drawx--;
                lines[1] = type1;
                lines[0] = lines[2] = 0;
            }
        }
        break;

    case KK_EditBlockUp:
        if (!currline->prev)
        {
            lines[2] = 0;
            gonext = false;
        }

        if (!drawy && gonext)
        {
            drawy = 1;
            lines[2] = type1;
            if (lines[3] == type2) lines[3] = 0;
        }
        else if (drawy == -1) drawy = 1;
        else if (drawy == 1)
        {
            gonext = false;
            drawy--;
            lines[3] = type1;
            if (lines[2] == type2) lines[2] = 0;
        }

        if (gonext && drawx)
        {
            if (drawx == -1)
            {
                drawx++;
                lines[0] = type1;
                lines[1] = lines[3] = 0;
            }
            else  //(drawx == 1)
            {
                drawx--;
                lines[1] = type1;
                lines[0] = lines[3] = 0;
            }
        }
        break;
    }

    //-------------------------
    char new_chr = LinesToChr(lines);

    if (new_chr != currline->txt[col])
    {
        if (col < (currline->txt.length()-1))
        {
            Undo->PushItem(EDIT_UNDO_OVR_CHAR);
            currline->txt[col] = new_chr;
        }
        else if (col < maxcol)
        {
            Undo->PushItem(EDIT_UNDO_INS_CHAR);
            currline->txt.insert(col, 1, new_chr);
        }

        setlinetype(currline);
        displine(currline, row);
    }

    //-------------------------
    if (gonext)
    {
        switch (key)
        {
        case KK_EditBlockRight:
            GoRight();
            break;
        case KK_EditBlockLeft:
            GoLeft();
            break;
        default:
            (key == KK_EditBlockDown) ? GoDown() : GoUp();
            if (col < pcol)
            {
                size_t len = pcol - col;
                Undo->PushItem(EDIT_UNDO_INS_TEXT|BATCH_MODE, currline, col, len);
                currline->txt.insert(col, len, ' ');
                GoEOL();
            }
            break;
        }

        gotorowcol(col, row);
        ChrToLines(currline->txt[col], lines);

        switch (key)
        {
        case KK_EditBlockRight:
            if ((drawx == -1) && (lines[0] == type1)) drawx++;
            break;
        case KK_EditBlockLeft:
            if ((drawx == +1) && (lines[1] == type1)) drawx--;
            break;
        case KK_EditBlockDown:
            if ((drawy == -1) && (lines[2] == type1)) drawy++;
            break;
        case KK_EditBlockUp:
            if ((drawy == +1) && (lines[3] == type1)) drawy--;
            break;
        }
    }

    GFTRK(0);
}


//  ------------------------------------------------------------------

void IEclass::Header()
{

    GFTRK("EditHeader");

    windowclose();
    EditHeaderinfo(msgmode, *HeaderView);
    windowopen();

    GFTRK(0);
}


//  ------------------------------------------------------------------

void IEclass::Abort()
{

    GFTRK("EditAbort");

    cursoroff();
    GMenuDropmsg MenuDropmsg;
    if(MenuDropmsg.Run())
    {
        done = MODE_QUIT;
    }

    GFTRK(0);
}


//  ------------------------------------------------------------------

void IEclass::AskExit()
{

    GFTRK("EditAskExit");

    cursoroff();
    GMenuQuit MenuQuit;
    gkbd.quitall = make_bool(MenuQuit.Run());
    if(gkbd.quitall)
    {
        GMenuDropmsg MenuDropmsg;
        if(MenuDropmsg.Run())
            done = MODE_QUIT;
        else
            done = MODE_SAVE;
    }

    GFTRK(0);
}


//  ------------------------------------------------------------------

void IEclass::QuitNow()
{
    GFTRK("EditQuitNow");

    if (CFG->switches.get(timeoutsavemsg))
    {
        quitnow = NO;
        done = MODE_SAVE;
        msgptr->attr.lok1();
    }
    else
    {
        quitnow = YES;
        done = MODE_QUIT;
    }

    gkbd.quitall = YES;

    GFTRK(0);
}


//  ------------------------------------------------------------------

int EditMsg(int __mode, uint* __position, GMsg* __msg)
{

    IEclass Editor(0, MAXCOL-1, MINROW, MAXROW-2, 5);
    return Editor.Start(__mode, __position, __msg);
}


//  ------------------------------------------------------------------
