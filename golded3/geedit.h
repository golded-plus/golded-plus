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
//  The Internal Editor (IE).
//  ------------------------------------------------------------------
#ifndef __GEEDIT_H
#define __GEEDIT_H

#ifdef __GNUG__
#pragma interface "geedit.h"
#endif
//  ------------------------------------------------------------------
//  Screen coordinate models
//  ------------------------------------------------------------------
//
//  00  ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ  01
//  01   Msg# :                     02
//  02   From :                     03
//  03   To   :                     04
//  04   Subj :                     05
//  05  ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ  06
//  06  00  EDIT_MINROW             07  EDIT_MIN_ROW
//  07  01                          08
//  08  02                          09
//  09  03                          10
//  10  04                          11
//  11  05                          12
//  12  06                          13
//  13  07                          14
//  14  08                          15
//  15  09                          16
//  16  10                          17
//  17  11                          18
//  18  12                          19
//  19  13                          10
//  10  14                          21
//  21  15                          22
//  22  16                          23
//  23  17  EDIT_MAXROW             24  EDIT_MAX_ROW
//  24  ÛGoldED xxxx (statusline)   25
//   ³  ³                            ³
//   ³  ³                            ÀÄÄÄÄ one-based old-style coord
//   ³  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ editor window coordinates
//   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ actual screen coordinates
//
//  ------------------------------------------------------------------
//  ------------------------------------------------------------------
//  Defines

#define EDIT_BUFLEN 256
#define EDIT_PARABUFLEN 2048

#define EDIT_UNDO_CHAR 0x00000010U
#define EDIT_UNDO_INS_CHAR 0x00000011U
#define EDIT_UNDO_DEL_CHAR 0x00000012U
#define EDIT_UNDO_OVR_CHAR 0x00000014U

#define EDIT_UNDO_TEXT 0x00001000U
#define EDIT_UNDO_DEL_TEXT 0x00001100U
#define EDIT_UNDO_CUT_TEXT 0x00001200U
#define EDIT_UNDO_INS_TEXT 0x00001400U
#define EDIT_UNDO_WRAP_TEXT 0x00001800U

#define EDIT_UNDO_LINE 0x00100000U
#define EDIT_UNDO_DEL_LINE 0x00110000U
#define EDIT_UNDO_NEW_LINE 0x00120000U
#define EDIT_UNDO_PUSH_LINE 0x00140000U
#define EDIT_UNDO_POP_LINE 0x00180000U
#define EDIT_UNDO_ORPHAN_LINE 0x00190000U

#define EDIT_UNDO_VOID 0x00000000U

#define EDIT_UNDO_TYPE 0x00101010U
#define EDIT_UNDO_ACTION 0x001F1F1FU

#define LAST_LINE 0x20000000U
#define PREV_LINE 0x40000000U
#define BATCH_MODE 0x80000000U
#define NO_VALUE 0xFFFFFFFFU
//  ------------------------------------------------------------------
//  Globals
extern Line * Edit__killbuf;
extern Line * Edit__pastebuf;
extern Path Edit__exportfilename;
//  ------------------------------------------------------------------
//  Reality check define and function

#ifdef NDEBUG
#define _test_halt(__t)
#define _test_haltab(__t, __a, __b)
#else
#define _test_halt(__t) if(__t) {debugtest(#__t, 0, 0, __FILE__, __LINE__, false);}
#define _test_haltab(__t, __a, __b) if(__t) {debugtest(#__t, \
                                                       __a, \
                                                       __b, \
                                                       __FILE__, \
                                                       __LINE__, \
                                                       true);}
#endif
//  ------------------------------------------------------------------
//  Undo data structures
class text_item
{
public: uint col;  // Begining offset in the .text string
    uint len;      // Text length
    __extension__ char text[0]; // Text string itself
    text_item(uint __col, uint __len) : col(__col), len(__len){}
void * operator new(size_t size, uint text_len = 0)
{
    return malloc(sizeof(text_item) + size + text_len);
}

void operator delete(void * ptr)
{
    free(ptr);
}

void operator delete(void * ptr, uint)
{
    free(ptr);
}
};

//  ----------------------------------------------------------------
union data_rec
{
    text_item * text_ptr;
    Line *      line_ptr;
    void *      void_ptr;
    char        char_int;
};

struct col_rec
{
    uint num;
    uint sav;
};

//  ----------------------------------------------------------------
class UndoItem
{
    static UndoItem ** last_item;
    friend class UndoStack;
public: UndoItem * prev;
    Line * line;       // Cursor line
    col_rec col;       // Cursor column
    uint pcol;         // After undo move cursor to pcol, prow
    uint prow;         //
    uint action;       // Undo action
    data_rec data;     // Undo data
    UndoItem()
    {
        this->prev = *last_item;
    }

    ~UndoItem()
    {
        *last_item = this->prev;
    }
};

//  ----------------------------------------------------------------
class IEclass;
class UndoStack
{
    IEclass * editor;
    uint & row;
    uint & col;
    uint & pcol;
    uint & prow;
    uint & minrow;
    uint & maxrow;
    uint & thisrow;
    Line * & currline;
    bool & undo_ready;
public: UndoItem * last_item;
    bool undo_enabled;
    UndoStack(IEclass * this_editor);
    ~UndoStack();
    bool FixPushLine(Line * __line);
    void PushItem(uint action,
                  Line * __line = NULL,
                  uint __col    = NO_VALUE,
                  uint __len    = NO_VALUE);
    void PlayItem();
};

//  ------------------------------------------------------------------
//  Internal Editor Class
class IEclass : public Container
{
public: gwindow editwin; // window
protected:
    //  ----------------------------------------------------------------
    //  Editor window data
    int win_mincol;      // First column
    int win_minrow;      // First row
    int win_maxcol;      // Last column
    int win_maxrow;      // Last row
    int win_border;      // Border type
    int win_hasborder;   // 1 == window has a border, 0  == no border
    //  ----------------------------------------------------------------
    //  Window-relative minimum/maximum values
    uint mincol;
    uint minrow;
    uint maxcol;
    uint maxrow;
    //  ----------------------------------------------------------------
    //  Cursor coordinates
    uint col;
    uint row;
    uint pcol;
    uint prow;
    uint ccol;
    uint crow;
    //  ----------------------------------------------------------------
    //  Undo stuff
    friend class UndoStack;
    UndoStack * Undo;
    long batch_mode;
    bool undo_ready;
    //  ----------------------------------------------------------------
    //  Misc.
    int chartyped;
    Line * currline;
    int done;
    int insert;
    int drawlines;
    int drawflag;
    int marginquotes;
    int margintext;
    int msgmode;
    GMsg * msgptr;
    int quitnow;
    uint thisrow;
    char * unfinished;
    int blockcol;
    int selecting;
    //  ----------------------------------------------------------------
    //  Speller (DLL)

#if defined (GCFG_SPELL_INCLUDED)
    CSpellChecker schecker;
#endif
    //  ----------------------------------------------------------------
    //  Internal helper functions
    void clreol(int __col = -1, int __row = -1);
    void cursoroff();
    void cursoron();
    void deleteline(bool zapquotesbelow = false);
    vattr dispchar(vchar __ch, vattr attr = DEFATTR);
    void dispins();
    void dispdl();
    void displine(Line * __line, uint __row);

#if defined (GCFG_SPELL_INCLUDED)
    void dispstringsc(char * __buf,
                      uint __beg,
                      uint __end,
                      uint __row,
                      uint __col,
                      char endchar);
    void dispstring(Line * line, uint __row);

#else
    void dispstring(const char * __string, uint __row, int attr = -1,
                    Line * line = NULL);

#endif
    int downoneline(uint __row);
    void editexport(Line * __exportline, int __endat);
    Line * findanchor();
    Line * findfirstline();
    Line * findtopline();
    void getthisrow(Line * __currline);
    void gotorowcol(uint __col, uint __row);
    int handlekey(gkey __key);
    void editimport(Line * __line, char * __filename, bool imptxt = false);
    void imptxt(char * __filename, bool imptxt = false);
    void insertchar(char __ch);
    Line * insertlinebelow(Line * __currline,
                           const char * __text = NULL,
                           long __batch_mode   = 0);
    int isempty(Line * __line                  = NULL);
    void killkillbuf();
    void killpastebuf();
    void prints(int wrow, int wcol, vattr atr, const char * str);
    int reflowok(char * __qstr);
    void refresh(Line * __currline, uint __row);
    void savefile(int __status);
    void scrolldown(int __scol, int __srow, int __ecol, int __erow, int __lines = 1);
    void scrollup(int __scol, int __srow, int __ecol, int __erow, int __lines = 1);
    void setcolor(Line * __line);
    void setlinetype(Line * __line);
    void statusline();
    void windowclose();
    void windowopen();
    Line * wrapit(Line ** __currline,
                  uint * __curr_col,
                  uint * __curr_row,
                  bool __display = true);
    Line * wrapdel(Line ** __currline,
                   uint * __curr_col,
                   uint * __curr_row,
                   bool __display = true);
    Line * wrapins(Line ** __currline,
                   uint * __curr_col,
                   uint * __curr_row,
                   bool __display = true);

  #ifndef NDEBUG
    void debugtest(char * __test,
                   int __a,
                   int __b,
                   char * __file,
                   int __line,
                   int __values);

  #endif

    void Buf2Clip();
    void Clip2Buf();

public:
    //  ----------------------------------------------------------------
    //  Constructor and destructor
    IEclass(int __scol, int __ecol, int __srow, int __erow, int __border);
    ~IEclass();
    //  ----------------------------------------------------------------
    //  Function to start the editor
    int Start(int __mode, uint * __position, GMsg * __msg);

    //  ----------------------------------------------------------------
    //  User key functions
    void Abort();
    void AskExit();
    void BlockAnchor();
    void BlockCopy();
    void BlockCut(bool just_delete = false);
    void BlockPaste();
    void BlockDel(Line * _anchor);
    void ClearDeleteBuf();
    void ClearPasteBuf();
    void CopyAboveChar();
    void DelChar();
    void DeleteEOL();
    void DeleteSOL();
    void DelLeft();
    void DelLine();
    void DelLtWord();
    void DelRtWord();
    void DosShell();
    void DupLine();
    void ExitMsg();
    void ExportText();
    void GoBegLine();
    void GoBotLine();
    void GoBotMsg();
    void GoDown();
    void GoEOL();
    void GoLeft();
    void GoPgDn();
    void GoPgUp();
    void GoRight();
    void GoTopLine();
    void GoTopMsg();
    void GoUp();
    void GoWordLeft();
    void GoWordRight();
    void Header();
    void ImportQuotebuf();
    void ImportText();
    void LoadFile();
    void LookupCursor();
    void LookupDest();
    void LookupOrig();
    void Newline();
    void QuitNow();
    void Reflow();
    void ReTab();
    void SaveFile();
    void SaveMsg();

#if defined (GCFG_SPELL_INCLUDED)
    void SCheckerMenu();

#endif
    void Soundkill();

    // External spell check tool
    void SpellCheck();
    void Tab();
    void ToggleCase();
    void ToggleCaseBlock(gkey key);
    void SCodeChange(gkey key);
    void ToggleInsert();
    void ToggleDrawLines();
    void DrawLines(gkey key);
    void ToLower();
    void ToUpper();
    void UnDelete(bool before = true);
    void ZapQuoteBelow();

private: void ToggleCaseChar(gkey key, std::string::iterator it, Line * ln, int n);

    //  ----------------------------------------------------------------
};

//  ------------------------------------------------------------------
void Edit__killpastebuf(void);

inline void IEclass::killpastebuf()
{
    Edit__killpastebuf();
}

//  ------------------------------------------------------------------

#endif // ifndef __GEEDIT_H
