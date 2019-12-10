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
//  GCUI: Golded+ Character-oriented User Interface.
//  Screen/video functions.
//  Based on CXL by Mike Smedley.
//  ------------------------------------------------------------------

#ifndef __gvidall_h
#define __gvidall_h
//  ------------------------------------------------------------------

#include <gdefs.h>
#if defined (__USE_NCURSES__)
#include <gcurses.h>
#endif
#if defined (__WIN32__)
#include <windows.h>
#endif

#if defined (_MSC_VER)
#pragma warning(disable: 4200)
#endif
//  ------------------------------------------------------------------

#ifdef BLINK
#undef BLINK
#endif
#define BLINK 128

#ifdef INTENSE
#undef INTENSE
#endif
#define INTENSE 8

#if defined (__UNIX__) && !defined (__USE_NCURSES__)
#define ACSET BLINK
#else
#define ACSET 0
#endif
//  ------------------------------------------------------------------

#if defined (__USE_NCURSES__)
typedef chtype vchar;     // Type of characters on-screen
typedef int    vattr;     // Type of screen attributes
typedef chtype vatch;     // Type of character-attribute groups
#elif defined (__WIN32__)
typedef char      vchar;  // Type of characters on-screen
typedef int       vattr;  // Type of screen attributes
typedef CHAR_INFO vatch;  // Type of character-attribute groups
#else
typedef char vchar;       // Type of characters on-screen
typedef int  vattr;       // Type of screen attributes
typedef word vatch;       // Type of character-attribute groups
#endif
//  ------------------------------------------------------------------
//  Attribute codes for functions that use them
const vattr DEFATTR   = -1;
const vattr BLACK_    = 0;
const vattr BLUE_     = 1;
const vattr GREEN_    = 2;
const vattr CYAN_     = 3;
const vattr RED_      = 4;
const vattr MAGENTA_  = 5;
const vattr BROWN_    = 6;
const vattr LGREY_    = 7;
const vattr DGREY_    = 8;
const vattr LBLUE_    = 9;
const vattr LGREEN_   = 10;
const vattr LCYAN_    = 11;
const vattr LRED_     = 12;
const vattr LMAGENTA_ = 13;
const vattr YELLOW_   = 14;
const vattr WHITE_    = 15;
const vattr _BLACK    = (BLACK_ << 4);
const vattr _BLUE     = (BLUE_ << 4);
const vattr _GREEN    = (GREEN_ << 4);
const vattr _CYAN     = (CYAN_ << 4);
const vattr _RED      = (RED_ << 4);
const vattr _MAGENTA  = (MAGENTA_ << 4);
const vattr _BROWN    = (BROWN_ << 4);
const vattr _LGREY    = (LGREY_ << 4);
const vattr _DGREY    = (DGREY_ << 4);
const vattr _LBLUE    = (LBLUE_ << 4);
const vattr _LGREEN   = (LGREEN_ << 4);
const vattr _LCYAN    = (LCYAN_ << 4);
const vattr _LRED     = (LRED_ << 4);
const vattr _LMAGENTA = (LMAGENTA_ << 4);
const vattr _YELLOW   = (YELLOW_ << 4);
const vattr _WHITE    = (WHITE_ << 4);
//  ------------------------------------------------------------------
//  Additional monochrome color values
const vattr UNDERLINE = 1;
const vattr NORMAL    = 7;
const vattr HIGHLIGHT = 15;
const vattr REVERSE   = 112;
//  ------------------------------------------------------------------
//  Display adapter types returned from vidtype()
//  If bit 0 is set, the adapter was detected in monochrome mode

#define GV_NONE 0x0000
#define V_MONO 0x0001
#define V_MDA 0x0011
#define V_HGC 0x0021
#define V_HGCPLUS 0x0041
#define V_INCOLOR 0x0080
#define V_CGA 0x0100
#define V_EGA 0x0200
#define V_EGAMONO 0x0201
#define V_MCGA 0x0400
#define V_MCGAMONO 0x0401
#define V_VGA 0x0800
#define V_VGAMONO 0x0801
//  ------------------------------------------------------------------
//  Video parameter settings

#define VP_DMA 0        // direct screen writes
#define VP_CGA 1        // direct screen writes, eliminate CGA snow
#define VP_BIOS 2       // BIOS screen writes
#define VP_MONO 3       // monochrome attribute translation on
#define VP_COLOR 4      // monochrome attribute translation off
#define VP_OS2VIO 6     // OS/2 vio screen writes
#define VP_W32CON 7     // WIN32 console screen writes
#define VP_CURSES 8     // Curses screen writes
//  ------------------------------------------------------------------
//  Video devices

#define GVID_DMA 0
#define GVID_CGA 1
#define GVID_BIO 2
#define GVID_OS2 4
#define GVID_W32 5
#define GVID_CUR 6
//  ------------------------------------------------------------------
//  Useful defines for video (0x10) interrupt function numbers

#if defined (__MSDOS__)
#define V_SET_MODE 0x00
#define V_SET_CURSOR_POS 0x02
#define V_GET_CURSOR_POS 0x03
#define V_SCROLL_UP 0x06
#define V_SCROLL_DOWN 0x07
#define V_RD_CHAR_ATTR 0x08
#define V_WR_CHAR_ATTR 0x09
#define V_WR_CHAR 0x0A
#define V_WR_TTY 0x0E
#define V_GET_MODE 0x0F
#define V_GET_FONT_INFO 0x1130
#endif
//  ------------------------------------------------------------------

#if defined (__MSDOS__)
struct __int10_ah1b_statebuf
{
    // Offset  Size    Description
    dword statfunctable; //  00h    DWORD   address of static funtionality table (see below)
    byte  videomode;    //  04h    BYTE    video mode in effect
    word  columns;      //  05h    WORD    number of columns
    word  regenbuflen;  //  07h    WORD    length of regen buffer in bytes
    word  regenbufstart; //  09h    WORD    starting address of regen buffer
    word  cursorpos0;   //  0Bh    WORD    cursor position for page 0
    word  cursorpos1;   //  0Dh    WORD    cursor position for page 1
    word  cursorpos2;   //  0Fh    WORD    cursor position for page 2
    word  cursorpos3;   //  11h    WORD    cursor position for page 3
    word  cursorpos4;   //  13h    WORD    cursor position for page 4
    word  cursorpos5;   //  15h    WORD    cursor position for page 5
    word  cursorpos6;   //  17h    WORD    cursor position for page 6
    word  cursorpos7;   //  19h    WORD    cursor position for page 7
    word  cursortype;   //  1Bh    WORD    cursor type
    byte  activepage;   //  1Dh    BYTE    active display page
    word  crctportaddr; //  1Eh    WORD    CRTC port address
    byte  curr_reg_3x8; //  20h    BYTE    current setting of register (3?8)
    byte  curr_reg_3x9; //  21h    BYTE    current setting of register (3?9)
    byte  rows;         //  22h    BYTE    number of rows
    word  bytesperchar; //  23h    WORD    bytes/character
    byte  dispcombcode; //  25h    BYTE    display combination code of active display
    byte  dcc;          //  26h    BYTE    DCC of alternate display
    word  numcolors;    //  27h    WORD    number of colors supported in current mode
    byte  numpages;     //  29h    BYTE    number of pages supported in current mode
    byte  numscanlines; //  2Ah    BYTE    number of scan lines active (0,1,2,3) =
                        // (200,350,400,480) Tseng ET3000: (4,5,6 = 512,600,768)
    byte primcharblock;  //  2Bh    BYTE    primary character block
    byte seccharblock;  //  2Ch    BYTE    secondary character block
    byte miscflags;     //  2Dh    BYTE    miscellaneous flags (see below)
    byte reserved1[3];  //  2Eh  3 BYTEs   reserved (00h)
    byte videomem;      //  31h    BYTE    video memory available 00h = 64K, 01h = 128K, 02h =
                        // 192K, 03h = 256K
    byte stateflags;    //  32h    BYTE    save pointer state flags (see below)
    byte reserved2[13];  //  33h 13 BYTEs   reserved (00h)
};

#endif // if defined (__MSDOS__)
//  ------------------------------------------------------------------
//  Border types

#define BT_SINGLE 0
#define BT_DOUBLE 1
#define BT_SINGLETOP 2
#define BT_DOUBLETOP 3
#define BT_BLANKS 4
#define BT_NONE 5
#define BT_BLOCKS 6
#define BT_ASCII 7
//  ------------------------------------------------------------------
//  Video information structure
struct GVidInfo
{
    // Screen info
    struct _screen
    {
        int mode;       // Video mode
        int rows;       // Number of rows
        int columns;    // Number of columns
        int cheight;    // Character height
        int cwidth;     // Character width
    } screen;

    // Cursor info
    struct _cursor
    {
        int  column;    // Cursor column
        int  row;       // Cursor row
        int  start;     // Cursor start line
        int  end;       // Cursor end line
        word attr;      // Cursor attribute. Hidden if attr == 0xFFFF
    } cursor;

    // Colors
    struct _color
    {
        vattr textattr; // Text attribute
        vattr overscan; // Overscan color
        int   intensity; // Background color state (intense or blinking)
        int   palette[16]; // Palette state
    } color;
};

//  ------------------------------------------------------------------

#ifdef __DJGPP__
typedef uint32_t gdma; // Video DMA linear address
#else
typedef word * gdma;   // Video DMA pointer
#endif
//  ------------------------------------------------------------------
//  Video information record
class GVid
{
public: int adapter;   // Video adapter type
    GVidInfo orig;     // Original video info
    GVidInfo curr;     // Current video info
    int device;        // Video device type
    gdma dmadir;       // Video DMA pointer (direct)
    gdma dmaptr;       // Video DMA pointer (direct or buffered)
    vchar * bufchr;    // Video line char buffer (char only)
    vatch * bufwrd;    // Video line word buffer (char+attr)
    vchar * bufansi;   // Video line ANSI buffer (11*numcols)
    int currow;        // Current cursor row
    int curcol;        // Current cursor column
    size_t numrows;    // number of displayed rows
    size_t numcols;    // number of displayed columns
    word videoseg;     // video buffer segment address
public: GVid();
    ~GVid();
public: void init();
    int detectadapter();
    void detectinfo(GVidInfo * _info);
    static void setcolorpairs(bool enabletransparent = false);
    void resetcurr();
    void setdevice(int _device);
    void setmode(int _mode);
    void setrows(int _rows);
    void setoverscan(vattr _overscan);
    void setintensity(int _intensity);
    void getpalette(int * _palette);
    void setpalette(int * _palette);

bool isdma()
{
    return device == GVID_DMA;
}

bool iscga()
{
    return device == GVID_CGA;
}

bool isbios()
{
    return device == GVID_BIO;
}

    void restore_cursor();
    void resize_screen(int columns, int rows);
};

extern GVid * gvid;
//  ------------------------------------------------------------------
//  Box characters table

#if !defined (__USE_NCURSES__)

extern char * __box_table[];
#define _box_table(i, j) (__box_table[i][j])

#else

chtype _box_table(int type, int c);

#endif
//  ------------------------------------------------------------------
//  Prototypes
int setvparam(int setting);
vattr mapattr(vattr attr);
vattr revsattr(vattr attr);

inline vattr attrib(int f, int b, int i, int bl)
{
    return (int)((b << 4) | (f) | (i << 3) | (bl << 7));
}

void vputw(int row, int col, vatch chat);
void vputws(int row, int col, vatch * buf, uint len);
void vputc(int row, int col, vattr atr, vchar chr);
void vputvs(int row, int col, vattr atr, const vchar * str);
void vputs(int row, int col, vattr atr, const char * str);
void vputs_box(int row, int col, vattr atr, const char * str);
void vputns(int row, int col, vattr atr, const char * str, uint len);
void vputx(int row, int col, vattr atr, vchar chr, uint len);
void vputy(int row, int col, vattr atr, vchar chr, uint len);
vatch vgetw(int row, int col);
void vgetc(int row, int col, vattr * atr, vchar * chr);
void vscroll(int srow, int scol, int erow, int ecol, vattr atr, int lines);
void vposget(int * row, int * col);
void vposset(int row, int col);
void vclrscr();
void vclrscr(vattr atr);         // Overloaded

typedef struct _vsavebuf
{
    int                 top, left, right, bottom;
    __extension__ vatch data[0];
} vsavebuf;
vsavebuf * vsave(int srow = -1, int scol = -1, int erow = -1, int ecol = -1);
void vrestore(vsavebuf * buf, int srow = -1, int scol = -1, int erow = -1,
              int ecol = -1);
void vcurget(int * sline, int * eline);
void vcurset(int sline, int eline);
void vcurhide();
void vcurshow();
bool vcurhidden();
void vcurlarge();
void vcursmall();
void vbox(int srow,
          int scol,
          int erow,
          int ecol,
          int box,
          vattr hiattr,
          vattr loattr = DEFATTR);
void vfill(int srow, int scol, int erow, int ecol, vchar chr, vattr atr);


#if defined (__USE_NCURSES__)

int gvid_dosattrcalc(int ourattr);
int gvid_attrcalc(int dosattr);

inline vchar vgchar(vatch chat)
{
    return chat & (A_CHARTEXT | A_ALTCHARSET);
}

inline vattr vgattr(vatch chat)
{
    return gvid_dosattrcalc(chat & ~(A_CHARTEXT | A_ALTCHARSET));
}

inline vatch vschar(vatch chat, vchar chr)
{
    return (chr & (A_CHARTEXT | A_ALTCHARSET)) | (chat & ~(A_CHARTEXT | A_ALTCHARSET));
}

inline vatch vsattr(vatch chat, vattr atr)
{
    return (chat & (A_CHARTEXT | A_ALTCHARSET)) | gvid_attrcalc(atr);
}

inline vatch vcatch(vchar chr, vattr atr)
{
    return chr | gvid_attrcalc(atr);
}

#elif defined (__WIN32__)

inline vchar vgchar(vatch chat)
{
    return chat.Char.AsciiChar;
}

inline vattr vgattr(vatch chat)
{
    return chat.Attributes;
}

inline vatch vschar(vatch chat, vchar chr)
{
    chat.Char.UnicodeChar = 0;
    chat.Char.AsciiChar   = chr;
    return chat;
}

inline vatch vsattr(vatch chat, vattr atr)
{
    chat.Attributes = WORD(atr);
    return chat;
}

inline vatch vcatch(vchar chr, vattr atr)
{
    vatch chat;

    chat.Char.UnicodeChar = 0;
    chat.Char.AsciiChar   = chr;
    chat.Attributes       = WORD(atr);
    return chat;
}

#else // if defined (__USE_NCURSES__)
inline vchar vgchar(vatch chat)
{
    return chat & 0xff;
}

inline vattr vgattr(vatch chat)
{
    return (chat >> 8) & 0xff;
}

inline vatch vschar(vatch chat, vchar chr)
{
    return (chat & 0xff00) | chr;
}

inline vatch vsattr(vatch chat, vattr atr)
{
    return (chat & 0xff) | (atr << 8);
}

inline vatch vcatch(vchar chr, vattr atr)
{
    return (chr & 0xff) | ((atr << 8) & 0xff00);
}

#endif // if defined (__USE_NCURSES__)
inline vchar vgetc(int row, int col)
{
    return vgchar(vgetw(row, col));
}

typedef void (* VidPutStrCP)(int, int, int, const char *);
void gvid_boxcvt(char * s);

//  ------------------------------------------------------------------

#endif // ifndef __gvidall_h
//  ------------------------------------------------------------------
