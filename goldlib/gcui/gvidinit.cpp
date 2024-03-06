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
//  Device-independent video functions. Initialise class GVid.
//  ------------------------------------------------------------------

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <locale.h>
#include <gmemall.h>
#include <gmemdbg.h>
#include <gstrall.h>
#if defined(__WATCOMC__) || defined(__DJGPP__)
    #include <conio.h>
#endif
#include <gvidall.h>

#if defined(__OS2__)
    #define INCL_BASE
    #include <os2.h>
#endif

#if defined(__WIN32__)
    #include <windows.h>
#endif

#if !defined(__USE_NCURSES__) && defined(__UNIX__)
    #include <sys/ioctl.h>
    #include <termios.h>
    #include <unistd.h>
    #include <errno.h>
#endif

#if defined(__DJGPP__)
    #include <sys/farptr.h>
    #include <go32.h>
#endif


//  ------------------------------------------------------------------
//  Check if Borland C++ for OS/2 1.0 header has been fixed

#if defined(__OS2__) && defined(__BORLANDC__)
    #if __BORLANDC__ <= 0x400
        #ifndef BCOS2_BSESUB_FIXED
            #error There is a bug in the BSESUB.H header. Please fix it.
            //
            // Add/change the following in BSESUB.H:
            //
            // #define BCOS2_BSESUB_FIXED
            // APIRET16  APIENTRY16    VioGetState (PVOID16 pState, HVIO hvio);
            // APIRET16  APIENTRY16    VioSetState (PVOID16 pState, HVIO hvio);
            //
            // Borland forgot this (was only PVOID)      ^^
            //
        #endif
    #endif
#endif


//  ------------------------------------------------------------------
//  Global video data

GVid *gvid;

#if defined(__USE_NCURSES__)

    // add statics here

#elif defined(__UNIX__)

    int gvid_stdout = -1;
    const char* gvid_acs_enable;
    const char* gvid_acs_disable;

    void _vputx(int row, int col, int atr, char chr, uint len);
    void gvid_printf(const char* fmt, ...) __attribute__ ((format (printf, 1, 2)));

#elif defined(__WIN32__)

    HANDLE gvid_hout = INVALID_HANDLE_VALUE;

#elif defined(__MSDOS__)

    int __gdvdetected = false;

#endif


//  ------------------------------------------------------------------
//  Video Class constructor

GVid::GVid()
{

#ifdef __DJGPP__
    dmaptr = dmadir = 0;
#else
    dmaptr = dmadir = NULL;
#endif
    bufchr = NULL;
    bufwrd = NULL;
    bufansi = NULL;

    init();
}


//  ------------------------------------------------------------------
//  Video Class destructor

GVid::~GVid()
{

#if defined(__USE_NCURSES__)

    attrset(A_NORMAL);
    if(0 == (--curses_initialized))
        endwin();

#elif defined(__UNIX__)

    // "\033<"        Enter ANSI mode
    // "\033[?5l"     Normal screen
    // "\033[0m"      Normal character attributes

    gvid_printf("\033<\033[?5l\033[0m");

#endif
#ifndef __DJGPP__
    if(dmaptr != dmadir)  throw_xfree(dmaptr);
#endif
    throw_xfree(bufwrd);
    throw_xfree(bufchr);
    throw_xfree(bufansi);
}


//  ------------------------------------------------------------------

void GVid::init()
{

#if defined(__USE_NCURSES__)
    // Both display and keyboard will be initialized at once
    if(0 == (curses_initialized++))
    {
        setlocale(LC_ALL, "");
        initscr();
        raw();
        noecho();
        nonl();
        intrflush(stdscr, FALSE);
        keypad(stdscr, TRUE);
    }
#endif

    // Detect video adapter
    detectadapter();

    // Detect original video information
    detectinfo(&orig);
    memcpy(&curr, &orig, sizeof(GVidInfo));

#if defined(__MSDOS__)
    device = GVID_DMA;
#elif defined(__OS2__)
    device = GVID_OS2;
#elif defined(__WIN32__)
    device = GVID_W32;
#endif

#if defined(__USE_NCURSES__)
    dmaptr = dmadir = NULL;
#elif defined(__WATCOMC__) && defined(__386__)
    dmaptr = dmadir = (gdma)(videoseg << 4);
#elif defined(__DJGPP__)
    dmaptr = dmadir = ScreenPrimary;
#elif defined(__OS2__) || defined(__WIN32__)
    dmaptr = dmadir = NULL;
#elif defined(__UNIX__)
    dmaptr = (gdma)throw_xcalloc((orig.screen.rows+1)*orig.screen.columns, sizeof(word));
    dmadir = NULL;
#else
    dmaptr = dmadir = (gdma)MK_FP(videoseg, 0);
#endif

    bufchr = NULL;
    bufwrd = NULL;
    bufansi = NULL;

    resetcurr();
}

//  ------------------------------------------------------------------
//  Video adapter detect

int GVid::detectadapter()
{

    // Initialize to a valid value
    adapter = GV_NONE;

#if defined(__USE_NCURSES__)

    start_color();

    setcolorpairs();

    adapter = V_VGA;

#elif defined(__MSDOS__)

    i86 cpu;

    // Get video mode
    cpu.ah(V_GET_MODE);
    cpu.genint(0x10);
    int _got_mode = cpu.al();

    // Check for PS/2 compatible video BIOS by calling the get
    // video configuration function. If it exists, the video
    // configuration code will be returned in BL.

    cpu.ax(0x1A00);
    cpu.genint(0x10);

    if(cpu.al() == 0x1A)
    {

        switch(cpu.bl())
        {
        case 0x00:
            adapter = GV_NONE;
            break;
        case 0x01:
            adapter = V_MDA;
            break;
        case 0x02:
            adapter = V_CGA;
            break;
        case 0x04:
            adapter = V_EGA;
            break;
        case 0x05:
            adapter = V_EGAMONO;
            break;
        case 0x07:
            adapter = (_got_mode == 7) ? V_VGAMONO : V_VGA;
            break;
        case 0x08:
            adapter = (_got_mode == 7) ? V_VGAMONO : V_VGA;
            break;
        case 0x0A:
        case 0x0C:
            adapter = V_MCGA;
            break;
        case 0x0B:
            adapter = V_MCGAMONO;
            break;
        default:
            adapter = V_VGA;    // We hope it is VGA compatible!
        }
    }
    else
    {

        // OK, we know that it's not a PS/2 BIOS, so check for an EGA.
        // If an EGA is not present, BH will be unchanged on return.

        cpu.ah(0x12);
        cpu.bl(0x10);
        cpu.genint(0x10);

        if(cpu.bl() - 0x10)
        {
            adapter = cpu.bh() ? V_EGAMONO : V_EGA;
        }
        else
        {

            // Now we know it's not an EGA. Get the BIOS equipment
            // flag and test for CGA, MDA, or no video adapter.

            cpu.genint(0x11);

            switch(cpu.al() & 0x30)
            {
            case 0x00:
                adapter = (_got_mode == 7) ? V_VGAMONO : V_VGA;   // EGA, VGA, or PGA
                break;
            case 0x10:
                adapter = GV_NONE;   // 40x25 color
                break;
            case 0x20:
                adapter = V_CGA;    // 80x25 color
                break;
            case 0x30:
                adapter = V_MDA;    // 80x25 monochrome
                break;
            }
        }
    }

#ifndef __DJGPP__

    // Set video segment
#if defined(__BORLANDC__) && defined(__DPMI32__)
    videoseg = (word)((adapter & V_MONO) ? __SegB000 : __SegB800);
#else
    videoseg = (word)((adapter & V_MONO) ? 0xB000 : 0xB800);
#endif

    // check for presence of DESQview by using the DOS Set
    // System Date function and trying to set an invalid date

    cpu.ax(0x2B01);  // DOS Set System Date
    cpu.ch('D');
    cpu.cl('E');
    cpu.dh('S');
    cpu.dl('Q');
    cpu.genint(0x21);

    // if error, then DESQview not present
    if(cpu.al() != 0xFF)
    {

        __gdvdetected = true;

#if defined(__WATCOMC__) && defined(__386__)
        memset(&RMI, 0, sizeof(RMI));
        RMI.EAX = 0x0000FE00;
        RMI.ES = videoseg;
        cpu.ax(0x0300);
        cpu.bl(0x10);
        cpu.bh(0);
        cpu.cx(0);
        cpu.es(FP_SEG(&RMI));
        cpu.edi(FP_OFF(&RMI));
        cpu.genint(0x31);
        videoseg = RMI.ES;
#else
        cpu.ah(0xFE);        // DV get alternate video buffer
        cpu.es(videoseg);
        cpu.di(0x0000);
        cpu.genint(0x10);
        videoseg = cpu.es();
#endif
    }

#endif // __DJGPP__

#elif defined(__OS2__)

    {
        VIOCONFIGINFO vioconfiginfo;
        memset(&vioconfiginfo, 0, sizeof(VIOCONFIGINFO));
        vioconfiginfo.cb = sizeof(VIOCONFIGINFO);
        VioGetConfig(0, &vioconfiginfo, 0);
        switch(vioconfiginfo.adapter)
        {
        case DISPLAY_MONOCHROME:
            adapter = V_MDA;
            break;
        case DISPLAY_CGA:
            adapter = V_CGA;
            break;
        case DISPLAY_EGA:
            adapter = V_EGA;
            break;
        case DISPLAY_VGA:
            adapter = V_VGA;
            break;
        default:
            adapter = V_VGA;    // We hope it is VGA compatible!
        }
        if(vioconfiginfo.display == DISPLAY_MONOCHROME)
            adapter |= V_MONO;
    }

#elif defined(__WIN32__)

    gvid_hout = CreateFile("CONOUT$", GENERIC_READ | GENERIC_WRITE,
                           FILE_SHARE_WRITE | FILE_SHARE_READ, NULL,
                           OPEN_EXISTING,
                           FILE_FLAG_NO_BUFFERING|FILE_FLAG_WRITE_THROUGH, NULL);

    adapter = V_VGA;

#elif defined(__linux__)

    for(int n=0; n<8; n++)
        __box_table[n] = __box_table[8];

    gvid_acs_enable  = "\016";
    gvid_acs_disable  = "\017";

    gvid_stdout = fileno(stdout);

    adapter = V_VGA;

#elif defined(__UNIX__) // code below should not be normally used...

    bool gvid_xterm = false;
    const char* term = getenv("TERM");

    if(term and strneql(term, "xterm", 5))
    {
        gvid_xterm = true;
        for(int n=0; n<8; n++)
            __box_table[n] = __box_table[8];
    }

    gvid_acs_enable  = gvid_xterm ? "\033)0\033(B\016" : "\033[11m";
    gvid_acs_disable = gvid_xterm ? "\033(B\033)B\017" : "\033[10m";

    gvid_stdout = fileno(stdout);

    adapter = V_VGA;

#endif

    return adapter;
}

void GVid::setcolorpairs(bool enabletransparent)
{
#if defined(__USE_NCURSES__)
    /* init colors */
    short mycolors[] = { COLOR_BLACK, COLOR_BLUE, COLOR_GREEN, COLOR_CYAN,
                         COLOR_RED, COLOR_MAGENTA, COLOR_YELLOW, COLOR_WHITE
                       };

    if(enabletransparent)
        use_default_colors();
    for(int i = 1; i < 64 and i < COLOR_PAIRS; i++)
    {
        short bg=mycolors[(i>>3)&0x07];
        init_pair(i, mycolors[(~i)&0x07], ((bg==COLOR_BLACK) && enabletransparent)?-1:bg);
    }
#endif
}

//  ------------------------------------------------------------------
//  Video info detect

void GVid::detectinfo(GVidInfo* _info)
{
    // Reset all original values
    memset(_info, 0, sizeof(GVidInfo));

#if defined(__USE_NCURSES__)

    _info->screen.mode = 0;
    _info->screen.rows = LINES;
    _info->screen.columns = COLS;
    getyx(stdscr, _info->cursor.row, _info->cursor.column);
    _info->color.textattr = 7;
    _info->cursor.start = 11;
    _info->cursor.end = 12;
    _info->cursor.attr = 7;
    _info->color.intensity = 1;
    _info->color.overscan = 0;

#elif defined(__MSDOS__)

    i86 cpu;

    // Get video mode and number of columns
    cpu.ah(V_GET_MODE);
    cpu.genint(0x10);
    _info->screen.mode = cpu.al();
    _info->screen.columns = cpu.ah();

    // Get the number of screen rows
    if(adapter >= V_EGA)
    {
        cpu.ax(V_GET_FONT_INFO);
        cpu.dx(0);
        cpu.genint(0x10);
        _info->screen.rows = cpu.dl() + ((adapter & V_EGA) ? 0 : 1);
        if(_info->screen.rows == 24)  // Normally nonsense
            _info->screen.rows++;
        //_info->screen.cheight = cpu.cx();
        _info->screen.cheight = 8;
        _info->screen.cwidth = 8;
    }
    else
    {
        _info->screen.rows = 25;
        _info->screen.cheight = 8;
        _info->screen.cwidth = 8;
    }

    // Get character attribute under the cursor
    cpu.ah(V_RD_CHAR_ATTR);
    cpu.bh(0);
    cpu.genint(0x10);
    _info->color.textattr = cpu.ah();

    // Get cursor position and form
    cpu.ah(V_GET_CURSOR_POS);
    cpu.bh(0);
    cpu.genint(0x10);
    _info->cursor.row = cpu.dh();
    _info->cursor.column = cpu.dl();
    _info->cursor.start = cpu.ch();
    _info->cursor.end = cpu.cl();
    _info->cursor.attr = (word)_info->color.textattr;
    // Get overscan color
    if(adapter & V_VGA)
    {
        cpu.ax(0x1008);
        cpu.bh(0xFF);
        cpu.genint(0x10);
        _info->color.overscan = cpu.bh();
    }

    // Get intensity state
    {
        // Check bit 5 at 0000:0465
#if defined(__DJGPP__)
        _info->color.intensity = (_farpeekb (_dos_ds, 0x465) & 0x20) ? 0 : 1;
#else
        byte* _bptr = (byte*)0x0465;
        _info->color.intensity = (*_bptr & 0x20) ? 0 : 1;
#endif

    }

#elif defined(__OS2__)

    // Get video mode and number of rows and columns
    {
        VIOMODEINFO viomodeinfo;
        memset(&viomodeinfo, 0, sizeof(VIOMODEINFO));
        viomodeinfo.cb = sizeof(VIOMODEINFO);
        VioGetMode(&viomodeinfo, 0);
        _info->screen.mode = viomodeinfo.fbType;
        _info->screen.rows = viomodeinfo.row;
        _info->screen.columns = viomodeinfo.col;
        _info->screen.cheight = viomodeinfo.vres / viomodeinfo.row;
        _info->screen.cwidth = viomodeinfo.hres / viomodeinfo.col;
    }

    // Get cursor position and character attribute under the cursor
    {
        USHORT usRow, usColumn;
        VioGetCurPos(&usRow, &usColumn, 0);
        _info->cursor.row = usRow;
        _info->cursor.column = usColumn;
        BYTE chat[2];
        USHORT len = 2;
#if defined(__EMX__)
        VioReadCellStr((PCH)chat, &len, usRow, usColumn, 0);
#else
        VioReadCellStr((CHAR*)chat, &len, usRow, usColumn, 0);
#endif
        _info->color.textattr = chat[1];
    }

    // Get cursor form
    {
        VIOCURSORINFO viocursorinfo;
        memset(&viocursorinfo, 0, sizeof(VIOCURSORINFO));
        VioGetCurType(&viocursorinfo, 0);
        _info->cursor.start = viocursorinfo.yStart;
        _info->cursor.end = viocursorinfo.cEnd;
        _info->cursor.attr = viocursorinfo.attr;
    }

    // Get intensity state
    {
        VIOINTENSITY viointensity;
        memset(&viointensity, 0, sizeof(VIOINTENSITY));
        viointensity.cb = sizeof(VIOINTENSITY);
        viointensity.type = 0x0002;
        VioGetState(&viointensity, 0);
        _info->color.intensity = viointensity.fs ? 1 : 0;
    }

    // Get overscan color
    {
        VIOOVERSCAN viooverscan;
        memset(&viooverscan, 0, sizeof(VIOOVERSCAN));
        viooverscan.cb = sizeof(VIOOVERSCAN);
        viooverscan.type = 0x0001;
        VioGetState(&viooverscan, 0);
        _info->color.overscan = (int)viooverscan.color;
    }

#elif defined(__WIN32__)

    // Get video mode and number of rows and columns
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int result = GetConsoleScreenBufferInfo(gvid_hout, &csbi);
    assert(result != 0);

    _info->screen.mode = 0;
    _info->screen.rows = csbi.dwSize.Y;
    _info->screen.columns = csbi.dwSize.X;

    // Get cursor position and character attribute under the cursor
    _info->cursor.row = csbi.dwCursorPosition.Y;
    _info->cursor.column = csbi.dwCursorPosition.X;
    _info->color.textattr = csbi.wAttributes;

    if(_info->screen.rows > 100)
    {
        _info->screen.rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        _info->screen.columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        _info->cursor.row = csbi.dwCursorPosition.Y - csbi.srWindow.Top + 1;
        _info->cursor.column = csbi.dwCursorPosition.X - csbi.srWindow.Left + 1;
    }

    // Get cursor form
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(gvid_hout, &cci);

    _info->cursor.start = (int) 1;
    _info->cursor.end = cci.bVisible ? (int) 15 : 0;
    _info->cursor.attr = (word)(cci.bVisible ? 7 : 0);

    // Get intensity state
    _info->color.intensity = 1;

    // Get overscan color
    _info->color.overscan = 0;

#elif defined(__UNIX__)

    int r = 0, c = 0;

    if(r <= 0)
    {
        char *s = getenv("LINES");
        if(s)
        {
            //printf("getenv(\"LINES\") = %s\n", s);
            r = atoi(s);
        }
    }

    if(c <= 0)
    {
        char *s = getenv("COLUMNS");
        if(s)
        {
            //printf("getenv(\"COLUMNS\") = %s\n", s);
            c = atoi(s);
        }
    }

    //printf("init1: c=%i, r=%i\n", c, r);

#if defined(TIOCGSIZE)
    if(r <= 0 or c <= 0)
    {
        struct ttysize sz;

        do
        {
            if((ioctl(1,TIOCGSIZE,&sz) == 0) or (ioctl(0, TIOCGSIZE, &sz) == 0) or (ioctl(2, TIOCGSIZE, &sz) == 0))
            {
                c = (int)sz.ts_cols;
                r = (int)sz.ts_lines;
                break;
            }
        }
        while(errno == EINTR);
    }
    //printf("init2: c=%i, r=%i\n", c, r);
#elif defined(TIOCGWINSZ)
    if(r <= 0 or c <= 0)
    {
        struct winsize wind_struct;

        do
        {
            if((ioctl(1,TIOCGWINSZ,&wind_struct) == 0) or (ioctl(0, TIOCGWINSZ, &wind_struct) == 0) or (ioctl(2, TIOCGWINSZ, &wind_struct) == 0))
            {
                c = (int)wind_struct.ws_col;
                r = (int)wind_struct.ws_row;
                break;
            }
        }
        while(errno == EINTR);
    }
    //printf("init3: c=%i, r=%i\n", c, r);
#endif


    if((r <= 0) or (r > 200))
        r = 24;
    if((c <= 0) or (c > 250))
        c = 80;

    //printf("init4: c=%i, r=%i\n", c, r);

    //*dmadir = 0;

    _info->screen.mode = 0;
    _info->screen.rows = r;
    _info->screen.columns = c;
    _info->cursor.row = 0;
    _info->cursor.column = 0;
    _info->color.textattr = 7;
    _info->cursor.start = 11;
    _info->cursor.end = 12;
    _info->cursor.attr = 7;
    _info->color.intensity = 1;
    _info->color.overscan = 0;

#endif

    getpalette(_info->color.palette);
}


//  ------------------------------------------------------------------
//  Reset current video info

void GVid::resetcurr()
{

    currow = curr.cursor.row;
    curcol = curr.cursor.column;

    numrows = curr.screen.rows;
    numcols = curr.screen.columns;

    throw_xfree(bufchr);
    throw_xfree(bufwrd);
    throw_xfree(bufansi);

    bufchr = (vchar*)throw_xcalloc(sizeof(vchar), numcols+1);
    bufwrd = (vatch*)throw_xcalloc(sizeof(vatch), numcols+1);
    bufansi = (vchar*)throw_xcalloc(sizeof(vchar), (11*numcols)+1);

    setdevice(device);
}


//  ------------------------------------------------------------------
//  Sets video output device

void GVid::setdevice(int _device)
{

    device = _device;
}


//  ------------------------------------------------------------------
//  Sets video mode

void GVid::setmode(int _mode)
{

    if(_mode)
    {
#if defined(__MSDOS__)

        i86 cpu;
        cpu.ah(0x00);
        cpu.al((byte)_mode);
        cpu.genint(0x10);

#endif
    }

    detectinfo(&curr);
    resetcurr();
}


//  ------------------------------------------------------------------
//  Sets screen rows

void GVid::setrows(int _rows)
{

    int origrows = curr.screen.rows;

#if defined(__USE_NCURSES__)

    NW(_rows);

#elif defined(__MSDOS__)
    i86 cpu;

    // Set video mode 3 (80xNN)
    if(curr.screen.mode != 3)
    {
        cpu.ax(0x0003);
        cpu.genint(0x10);
    }

    if(adapter >= V_EGA)
    {
        if((_rows == 28) and (adapter >= V_VGA))    // vga-only
        {
            cpu.ax(0x1202);
            cpu.bl(0x30);
            cpu.genint(0x10);
            cpu.ax(0x1111);
            cpu.bl(0);
            cpu.genint(0x10);
        }
        else if(_rows >= 43)
        {
            cpu.ax(0x1112);    // Load 8x8 character set
            cpu.bl(0x00);
            cpu.genint(0x10);
            cpu.ax(0x1200);    // Select alternate print-screen routine
            cpu.bl(0x20);
            cpu.genint(0x10);
            if(adapter & V_EGA)
            {
                // Disable cursor size emulation
                byte* _bptr = (byte*)0x0487;
                *_bptr |= (byte)0x01;
                // Set cursor size
                cpu.ah(0x01);
                cpu.al((byte)orig.screen.mode);
                cpu.cx(0x0600);
                cpu.genint(0x10);
            }
        }
        else
        {
            if(adapter & V_EGA)
            {
                // Enable cursor size emulation
                byte* _bptr = (byte*)0x0487;
                *_bptr &= (byte)0xFE;
            }
            // Set cursor size
            cpu.ah(0x01);
            cpu.al((byte)orig.screen.mode);
            cpu.cx(0x0607);
            cpu.genint(0x10);
        }
    }

#elif defined(__OS2__)

    VIOMODEINFO viomodeinfo;
    memset(&viomodeinfo, 0, sizeof(VIOMODEINFO));
    viomodeinfo.cb = sizeof(VIOMODEINFO);
    VioGetMode(&viomodeinfo, 0);
    viomodeinfo.row = (USHORT)_rows;
    VioSetMode(&viomodeinfo, 0);

#elif defined(__WIN32__) || defined(__UNIX__)

    NW(_rows);

#endif

    if(origrows < _rows)
        vfill(origrows, 0, _rows, 80, ' ', LGREY_|_BLACK);

    detectinfo(&curr);
    resetcurr();
}


//  ------------------------------------------------------------------
//  Set the screen border (overscan) color

void GVid::setoverscan(vattr _overscan)
{
#if defined(__USE_NCURSES__)

    NW (_overscan);

#elif defined(__MSDOS__)

    i86 cpu;
    cpu.ah(0x0B);
    cpu.bh(0x00);
    cpu.bl((byte)_overscan);
    cpu.genint(0x10);

#elif defined(__OS2__)

    VIOOVERSCAN viooverscan;
    memset(&viooverscan, 0, sizeof(VIOOVERSCAN));
    viooverscan.cb = sizeof(VIOOVERSCAN);
    viooverscan.type = 0x0001;
    VioGetState(&viooverscan, 0);
    viooverscan.color = (BYTE)_overscan;
    VioSetState(&viooverscan, 0);

#elif defined(__WIN32__) || defined(__UNIX__)

    NW(_overscan);

#endif
}


//  ------------------------------------------------------------------
//  Set intensity/blinking state

void GVid::setintensity(int _intensity)
{
#if defined(__USE_NCURSES__)

    NW(_intensity);

#elif defined(__MSDOS__)

#ifdef __DJGPP__

    if(_intensity)
        intensevideo();
    else
        blinkvideo();

#else

    if(adapter & V_CGA)
    {
        word* _wptr = (word*)0x0463;
        byte* _bptr = (byte*)0x0465;
        uint port = *_wptr + 4;
        uint reg = *_bptr;
        if(_intensity)
            reg &= 0xDF;
        else
            reg |= 0x20;
        outp(port, reg);
    }
    else
    {
        i86 cpu;
        cpu.ax(0x1003);
        cpu.bh(0x00);
        cpu.bl((byte)(_intensity ? 0 : 1));
        cpu.genint(0x10);
    }

#endif // __DJGPP__

#elif defined(__OS2__)

    VIOINTENSITY viointensity;
    memset(&viointensity, 0, sizeof(VIOINTENSITY));
    viointensity.cb = sizeof(VIOINTENSITY);
    viointensity.fs = (USHORT)(_intensity ? 1 : 0);
    viointensity.type = 0x0002;
    VioSetState(&viointensity, 0);

#elif defined(__WIN32__) || defined(__UNIX__)

    NW(_intensity);

#endif
}


//  ------------------------------------------------------------------

void GVid::getpalette(int* _palette)
{
#if defined(__USE_NCURSES__)

    NW(_palette);

#elif defined(__MSDOS__)

    // Get palette state
    if(adapter & V_VGA)
    {
        i86 cpu;
        for(byte n=0; n<16; n++)
        {
            cpu.ax(0x1007);    // GET INDIVIDUAL PALETTE REGISTER
            cpu.bh(0xFF);
            cpu.bl(n);
            cpu.genint(0x10);
            _palette[n] = cpu.bh();
        }
    }
    else
    {
        // Set to standard palette colors
        for(byte n=0; n<16; n++)
            _palette[n] = n + ((n > 7) ? 48 : 0);
    }

#elif defined(__OS2__)

    // Get palette state
    BYTE viopalstate[38];
    PVIOPALSTATE pviopalstate;
    memset(viopalstate, 0, sizeof(viopalstate));
    pviopalstate = (PVIOPALSTATE)viopalstate;
    pviopalstate->cb = sizeof(viopalstate);
    pviopalstate->type = 0;
    pviopalstate->iFirst = 0;
    VioGetState(pviopalstate, 0);
    for(byte n=0; n<16; n++)
        _palette[n] = pviopalstate->acolor[n];

#elif defined(__WIN32__) || defined(__UNIX__)

    NW(_palette);

#endif
}


//  ------------------------------------------------------------------

void GVid::setpalette(int* _palette)
{
#if defined(__USE_NCURSES__)

    NW(_palette);

#elif defined(__MSDOS__)

    if(adapter & (V_EGA|V_MCGA|V_VGA))
    {
        i86 cpu;
        for(byte n=0; n<16; n++)
        {
            if(_palette[n] != -1)
            {
                cpu.ax(0x1000);
                cpu.bl(n);
                cpu.bh((byte)_palette[n]);
                cpu.genint(0x10);
            }
        }
    }

#elif defined(__OS2__)

    BYTE viopalstate[38];
    PVIOPALSTATE pviopalstate;
    memset(viopalstate, 0, sizeof(viopalstate));
    pviopalstate = (PVIOPALSTATE)viopalstate;
    pviopalstate->cb = sizeof(viopalstate);
    pviopalstate->type = 0;
    pviopalstate->iFirst = 0;
    VioGetState(pviopalstate, 0);
    for(byte n=0; n<16; n++)
        if(_palette[n] != -1)
            pviopalstate->acolor[n] = (USHORT)_palette[n];
    VioSetState(pviopalstate, 0);

#elif defined(__WIN32__) || defined(__UNIX__)

    NW(_palette);

#endif
}


//  ------------------------------------------------------------------

void GVid::restore_cursor()
{

    vcurset(orig.cursor.start, orig.cursor.end);
    vcurshow();
    vposset(orig.cursor.row-1, 0);
}


//  ------------------------------------------------------------------

void GVid::resize_screen(int columns, int rows)
{

    numcols = curr.screen.columns = columns;
    numrows = curr.screen.rows    = rows;

    bufchr = (vchar*)throw_xrealloc(bufchr, numcols+1);
    bufwrd = (vatch*)throw_xrealloc(bufwrd, (numcols+1)*sizeof(vatch));
    bufansi = (vchar*)throw_xrealloc(bufansi, 1+(11*numcols));

#if defined(__UNIX__) && !defined(__USE_NCURSES__) && !defined(__DJGPP__)
    dmaptr = (gdma)throw_xrealloc(dmaptr, (rows+1)*columns*sizeof(word));
#endif
}


//  ------------------------------------------------------------------
