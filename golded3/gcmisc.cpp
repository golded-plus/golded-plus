
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
//  Compilers for help, screen and xlat.
//  ------------------------------------------------------------------

#include <golded.h>


//  ------------------------------------------------------------------

int ReadHelpCfg(int force) {

  char* ptr;
  long  offset;
  char  buf[1024];
  FILE* ifp;
  FILE* ofp;
  Hlpr* HlpL;
  int   count;
  int   counter;
  int   comment;

  if((force > 1) or (FiletimeCmp(CFG->helpcfg.fn, CFG->helpged) > 0)) {

    ifp = fsopen(AddPath(CFG->goldpath, CFG->helpcfg.fn), "rb", CFG->sharemode);
    if(ifp) {
      setvbuf(ifp, NULL, _IOFBF, 8192);
      ofp = fsopen(AddPath(CFG->goldpath, CFG->helpged), "wb", CFG->sharemode);
      if(ofp) {
        offset = 0L;
        CFG->helpcfg.ft = GetFiletime(AddPath(CFG->goldpath, CFG->helpcfg.fn));

        if(not quiet)
          cout << "* Reading " << AddPath(CFG->goldpath, CFG->helpcfg.fn) << endl;

        setvbuf(ofp, NULL, _IOFBF, 8192);

        count = 0;
        rewind(ifp);

        while(fgets(buf, sizeof(buf), ifp)) {
          if(strnieql(buf, "*B ", 3))
            count++;
        }

        HlpL = (Hlpr*)throw_calloc(count+2, sizeof(Hlpr));

        rewind(ifp);
        fputs("*I\r\n", ofp);
        fwrite(HlpL, count+1, sizeof(Hlpr), ofp);
        fputs("\r\n\r\n", ofp);
        offset += 4 + ((count+1)*sizeof(Hlpr)) + 4;

        counter = 0;
        comment = YES;
        while(fgets(buf, sizeof(buf), ifp)) {
          if(strnieql(buf, "*B ", 3)) {
            comment = NO;
            HlpL[counter].help = atow(buf+3);
            ptr = strchr(buf, ',');
            strbtrim(strcpy(HlpL[counter].category, ptr ? ptr+1 : ""));
            HlpL[counter].offset = offset + strlen(buf);
            counter++;
          }
          if(not comment) {
            fputs(buf, ofp);
            offset += strlen(buf);
          }
          if(strnieql(buf, "*E", 2))
            comment = YES;
        }
        HlpL[counter].offset = -1L;

        fseek(ofp, 0L, SEEK_SET);
        fputs("*I\r\n", ofp);
        fwrite(HlpL, count+1, sizeof(Hlpr), ofp);

        throw_free(HlpL);
        fclose(ofp);
      }

      fclose(ifp);
    }
  }

  // Init the help system
  whelpdef(CFG->helpged, Key_F1, C_HELPB, C_HELPW, C_HELPQ, C_HELPS, NULL);
  whelpwin(0, 0, MAXROW-2, MAXCOL-1, W_BHELP, NO);
  whelpcat(H_General);

  return 0;
}


//  ------------------------------------------------------------------
//  Window parts

const word CRC_BLOCK             = 0xC10C;
const word CRC_BORDER            = 0x97A7;
const word CRC_BTYPE             = 0x2A2F;
const word CRC_CURSOR            = 0x0D67;
const word CRC_EDIT              = 0xB32D;
const word CRC_HEADER            = 0x6EED;
const word CRC_HIGHLIGHT         = 0x5EBF;
const word CRC_INPUT             = 0xA25E;
const word CRC_KLUDHIDD          = 0x222E;
const word CRC_NOSELECT          = 0x950C;
const word CRC_QUOTE             = 0x75DA;
const word CRC_SELECTOR          = 0x5604;
const word CRC_STATUS            = 0x1D36;
const word CRC_TEARORIG          = 0x428E;
const word CRC_TITLE             = 0x636E;
const word CRC_WINDOW            = 0xCEFE;

const word CRC_FROM              = 0x9290;
const word CRC_TO                = 0x544F;
const word CRC_SUBJECT           = 0xF861;
const word CRC_KLUDGE            = 0xA600;
const word CRC_HIDDEN            = 0xE465;
const word CRC_TAGLINE           = 0xC328;
const word CRC_TEARLINE          = 0xB385;
const word CRC_ORIGIN            = 0x4CE5;
const word CRC_PAGEBAR           = 0x4ECA;
const word CRC_UNREAD            = 0xFE2A;
const word CRC_UNSENT            = 0xC20B;
const word CRC_UNREADHIGH        = 0x66D0;
const word CRC_UNSENTHIGH        = 0xB4FD;
const word CRC_QUOTE2            = 0xF400;
const word CRC_QUOTE1            = 0xF403;
const word CRC_SIGNATURE         = 0x1A6E;


const word CRC_B                 = 0x0042;
const word CRC_I                 = 0x0049;
const word CRC_BI                = 0x4249;
const word CRC_U                 = 0x0055;
const word CRC_BU                = 0x4255;
const word CRC_IU                = 0x4955;
const word CRC_BIU               = 0x21D3;
const word CRC_R                 = 0x0052;
const word CRC_RB                = 0x5242;
const word CRC_RI                = 0x5249;
const word CRC_RBI               = 0x38FE;
const word CRC_RU                = 0x5255;
const word CRC_RBU               = 0x38E2;
const word CRC_RIU               = 0x33E2;
const word CRC_RBIU              = 0x490E;
const word CRC_ALL               = 0x14A9;


//  ------------------------------------------------------------------
//  Colors

const word CRC_BLACK             = 0x20C2;
const word CRC_BLUE              = 0xB7A3;
const word CRC_BROWN             = 0xF575;
const word CRC_CYAN              = 0xD20D;
const word CRC_DARKGRAY          = 0x0D32;
const word CRC_DARKGREY          = 0x0932;
const word CRC_DGRAY             = 0x6ED8;
const word CRC_DGREY             = 0x6AD8;
const word CRC_GREEN             = 0xADCF;
const word CRC_LBLUE             = 0xDF6F;
const word CRC_LCYAN             = 0xBAC1;
const word CRC_LGRAY             = 0xC779;
const word CRC_LGREEN            = 0x8C61;
const word CRC_LGREY             = 0xC379;
const word CRC_LIGHTBLUE         = 0x053B;
const word CRC_LIGHTCYAN         = 0x6095;
const word CRC_LIGHTGRAY         = 0x1D2D;
const word CRC_LIGHTGREEN        = 0xB256;
const word CRC_LIGHTGREY         = 0x192D;
const word CRC_LIGHTMAGENTA      = 0xBEA8;
const word CRC_LIGHTRED          = 0xAED2;
const word CRC_LMAGENTA          = 0xDE86;
const word CRC_LRED              = 0x7752;
const word CRC_MAGENTA           = 0xBF75;
const word CRC_NORMAL            = 0x4DA2;
const word CRC_RED               = 0x3FF3;
const word CRC_REVERSE           = 0xB4CD;
const word CRC_UNDERLINE         = 0xDC93;
const word CRC_WHITE             = 0xD293;
const word CRC_YELLOW            = 0xFEAF;


//  ------------------------------------------------------------------
//  Window areas

const word CRC_AREA              = 0x010B;
const word CRC_ASK               = 0x0BAE;
const word CRC_BACKGROUND        = 0x1365;
const word CRC_BRAG              = 0x505E;
const word CRC_HELP              = 0xD054;
const word CRC_INFO              = 0x5811;
const word CRC_MENU              = 0x2DA4;
const word CRC_READER            = 0x1DE1;
const word CRC_SHADOW            = 0x2301;
const word CRC_STYLECODE         = 0x8B6C;
const word CRC_STYLECODES        = 0x4CB0;


//  ------------------------------------------------------------------
//  Get color value

int GetColor(char* ink) {

  word crc;
  int color=0;

  crc = strCrc16(strupr(ink));
  switch(crc) {
    // Black is the default
    case CRC_BLACK:
      color = 0;
      break;
    case CRC_BLUE:
      color = 1;
      break;
    case CRC_GREEN:
      color = 2;
      break;
    case CRC_CYAN:
      color = 3;
      break;
    case CRC_RED:
      color = 4;
      break;
    case CRC_MAGENTA:
      color = 5;
      break;
    case CRC_BROWN:
      color = 6;
      break;
    case CRC_LGRAY:
    case CRC_LGREY:
    case CRC_LIGHTGRAY:
    case CRC_LIGHTGREY:
      color = 7;
      break;
    case CRC_DGRAY:
    case CRC_DGREY:
    case CRC_DARKGRAY:
    case CRC_DARKGREY:
      color = 8;
      break;
    case CRC_LBLUE:
    case CRC_LIGHTBLUE:
      color = 9;
      break;
    case CRC_LGREEN:
    case CRC_LIGHTGREEN:
      color = 10;
      break;
    case CRC_LCYAN:
    case CRC_LIGHTCYAN:
      color = 11;
      break;
    case CRC_LRED:
    case CRC_LIGHTRED:
      color = 12;
      break;
    case CRC_LMAGENTA:
    case CRC_LIGHTMAGENTA:
      color = 13;
      break;
    case CRC_YELLOW:
      color = 14;
      break;
    case CRC_WHITE:
      color = 15;
      break;
    case CRC_UNDERLINE:
      color = UNDERLINE;
      break;
    case CRC_NORMAL:
      color = NORMAL;
      break;
    case CRC_HIGHLIGHT:
      color = HIGHLIGHT;
      break;
    case CRC_REVERSE:
      color = REVERSE;
      break;
  }
  return(color);
}


//  ------------------------------------------------------------------
//  Evaluates a color string

int MakeColor(char* colors) {

  static int paper=0;
  int ink=0, blink;
  char* ink_;
  char* paper_;

  #if defined(__UNIX__) || defined(__USE_NCURSES__)
  blink = 0;
  #else
  blink = (stridela("blinking", colors) ? 128 : 0);
  #endif

  ink_ = strtok(colors, " \t");         // Get the Ink color
  if(ink_) {
    ink = GetColor(ink_);
    paper_ = strtok(NULL, " \t");       // Skip the "On"
    if(paper_) {
      paper_ = strtok(NULL, " \t");     // Get the Paper color
      if(paper_)
        paper = GetColor(paper_);
    }
  }

  if(paper > 7)
    CFG->intensecolors = true;

  return ink | paper*16 | blink;
}


//  ------------------------------------------------------------------
//  Get color values

void GetColors(char* value) {

  word crc;
  Win* wptr;
  int attr;
  char buf[256];
  char* win;
  char* colors;
  char* part=NULL;

  *buf = NUL;
  if((win = strtok(value, " \t")) == NULL)
    return;

  if(stricmp(win, "shadow")) {
    part = strtok(NULL,  " \t");
    if(part == NULL)
      return;
  }

  do {
    colors = strtok(NULL,  " \t\n");
    if(colors != NULL) {
      strcat(buf, colors);
      strcat(buf, " ");
    }
  } while(colors != NULL);

  crc = strCrc16(strupr(win));
  attr = MakeColor(buf);
  switch(crc) {
    case CRC_SHADOW:
      C_SHADOW = attr;
      return;
    case CRC_BACKGROUND:
      wptr = &GC_BACK_;
      break;
    case CRC_STATUS:
      wptr = &GC_STAT_;
      break;
    case CRC_BRAG:
      wptr = &GC_BRAG_;
      break;
    case CRC_AREA:
      wptr = &GC_AREA_;
      break;
    case CRC_READER:
      wptr = &GC_READ_;
      break;
    case CRC_HEADER:
      wptr = &GC_HEAD_;
      break;
    case CRC_ASK:
      wptr = &GC_ASK__;
      break;
    case CRC_MENU:
      wptr = &GC_MENU_;
      break;
    case CRC_HELP:
      wptr = &GC_HELP_;
      break;
    case CRC_INFO:
      wptr = &GC_INFO_;
      break;
    case CRC_STYLECODE:
    case CRC_STYLECODES:
      wptr = &GC_STYLE_;
      break;
    default:
      return;
  }

  crc = strCrc16(strupr(part));
  switch(crc) {
    case CRC_WINDOW:
      wptr->_Window = attr;
      if(wptr == &GC_HEAD_) {
        wptr->_From = attr;
        wptr->_To = attr;
        wptr->_Subject = attr;
      }
      if(wptr == &GC_READ_) {
        wptr->_Tagline = attr;
        GC_STYLE_._Bold = attr;
        GC_STYLE_._Italic = attr;
        GC_STYLE_._BoldItalic = attr;
        GC_STYLE_._Underscore = attr;
        GC_STYLE_._BoldUnderscore = attr;
        GC_STYLE_._ItalicUnderscore = attr;
        GC_STYLE_._BoldItalicUnderscore = attr;
        GC_STYLE_._Reverse = attr;
        GC_STYLE_._ReverseBold = attr;
        GC_STYLE_._ReverseItalic = attr;
        GC_STYLE_._ReverseBoldItalic = attr;
        GC_STYLE_._ReverseUnderscore = attr;
        GC_STYLE_._ReverseBoldUnderscore = attr;
        GC_STYLE_._ReverseItalicUnderscore = attr;
        GC_STYLE_._ReverseBoldItalicUnderscore = attr;
      }
      break;
    case CRC_BORDER:
      wptr->_Border = attr;
      wptr->_Pagebar = attr;
      break;
    case CRC_BTYPE:
      wptr->btype = atoi(buf);
      break;
    case CRC_TITLE:
      wptr->_Title = attr;
      break;
    case CRC_SELECTOR:
      wptr->_Selector = attr;
      break;
    case CRC_HIGHLIGHT:
      wptr->_Highlight = attr;
      break;
    case CRC_NOSELECT:
      wptr->_Noselect = attr;
      break;
    case CRC_QUOTE:
      wptr->_Quote = attr;
      if(wptr == &GC_READ_)
        GC_READ2_._Quote2 = attr;
      break;
    case CRC_CURSOR:
      wptr->_Cursor = attr;
      break;
    case CRC_KLUDHIDD:
      wptr->_Kludge = attr;
      wptr->_Hidden = attr;
      break;
    case CRC_TEARORIG:
      wptr->_Tearline = attr;
      wptr->_Origin = attr;
      break;
    case CRC_EDIT:
      wptr->_Edit = attr;
      break;
    case CRC_BLOCK:
      wptr->_Block = attr;
      break;
    case CRC_INPUT:
      wptr->_Input = attr;
      break;
    case CRC_FROM:
      wptr->_From = attr;
      break;
    case CRC_TO:
      wptr->_To = attr;
      break;
    case CRC_SUBJECT:
      wptr->_Subject = attr;
      break;
    case CRC_KLUDGE:
      wptr->_Kludge = attr;
      break;
    case CRC_HIDDEN:
      wptr->_Hidden = attr;
      break;
    case CRC_TAGLINE:
      wptr->_Tagline = attr;
      break;
    case CRC_TEARLINE:
      wptr->_Tearline = attr;
      break;
    case CRC_ORIGIN:
      wptr->_Origin = attr;
      break;
    case CRC_PAGEBAR:
      wptr->_Pagebar = attr;
      break;
    case CRC_UNREAD:
      wptr->_WindowUnread = attr;
      break;
    case CRC_UNSENT:
      wptr->_WindowUnsent = attr;
      break;
    case CRC_UNREADHIGH:
      wptr->_HighlightUnread = attr;
      break;
    case CRC_QUOTE1:
      wptr->_Quote = attr;
      break;
    case CRC_QUOTE2:
      if(wptr == &GC_READ_)
        GC_READ2_._Quote2 = attr;
      break;
    case CRC_SIGNATURE:
      if(wptr == &GC_READ_)
        GC_READ2_._Signature = attr;
      break;
    case CRC_UNSENTHIGH:
      wptr->_HighlightUnsent = attr;
      break;
    case CRC_B:     wptr->_Bold = attr;                         break;
    case CRC_I:     wptr->_Italic = attr;                       break;
    case CRC_BI:    wptr->_BoldItalic = attr;                   break;
    case CRC_U:     wptr->_Underscore = attr;                   break;
    case CRC_BU:    wptr->_BoldUnderscore = attr;               break;
    case CRC_IU:    wptr->_ItalicUnderscore = attr;             break;
    case CRC_BIU:   wptr->_BoldItalicUnderscore = attr;         break;
    case CRC_R:     wptr->_Reverse = attr;                      break;
    case CRC_RB:    wptr->_ReverseBold = attr;                  break;
    case CRC_RI:    wptr->_ReverseItalic = attr;                break;
    case CRC_RBI:   wptr->_ReverseBoldItalic = attr;            break;
    case CRC_RU:    wptr->_ReverseUnderscore = attr;            break;
    case CRC_RBU:   wptr->_ReverseBoldUnderscore = attr;        break;
    case CRC_RIU:   wptr->_ReverseItalicUnderscore = attr;      break;
    case CRC_RBIU:  wptr->_ReverseBoldItalicUnderscore = attr;  break;
    case CRC_ALL:
      if(wptr == &GC_STYLE_) {
        wptr->_Bold = attr;
        wptr->_Italic = attr;
        wptr->_BoldItalic = attr;
        wptr->_Underscore = attr;
        wptr->_BoldUnderscore = attr;
        wptr->_ItalicUnderscore = attr;
        wptr->_BoldItalicUnderscore = attr;
        wptr->_Reverse = attr;
        wptr->_ReverseBold = attr;
        wptr->_ReverseItalic = attr;
        wptr->_ReverseBoldItalic = attr;
        wptr->_ReverseUnderscore = attr;
        wptr->_ReverseBoldUnderscore = attr;
        wptr->_ReverseItalicUnderscore = attr;
        wptr->_ReverseBoldItalicUnderscore = attr;
      }
      break;
    default:
      return;
  }
}


//  ------------------------------------------------------------------
//  Compare escape table items

static int CmpEsc(const char* a, const char* b) {

  return strncmp(a, b, 5);
}


//  ------------------------------------------------------------------
//  Read the translation tables

void ReadXlatTables() {

  if(not CFG->xlatcharset.empty() or not CFG->xlatescset.empty()) {

    Esc EscTable;
    Chs ChsTable;
    char buf[256];
    char* ptr;
    char* ptr2;
    FILE *ifp, *ofp;
    int line, n, x, y, ch=0;

    ofp = fsopen(AddPath(CFG->goldpath, CFG->xlatged), "wb", CFG->sharemode);
    if(ofp) {

      // Compile CHARSET tables
      vector<Map>::iterator xlt;
      for(xlt = CFG->xlatcharset.begin(); xlt != CFG->xlatcharset.end(); xlt++) {

        // Assign table defaults
        memset(&ChsTable, 0, sizeof(Chs));
        for(n=0; n<256; n++) {
          ChsTable.t[n][0] = 1;
          ChsTable.t[n][1] = (unsigned char)n;  // The character
        }
        strcpy(buf, AddPath(CFG->xlatpath, xlt->mapfile));
        ifp = fsopen(buf, "rb", CFG->sharemode);
        if(ifp) {

          if(not quiet)
            cout << "* Reading " << buf << endl;

          // Read the definition file
          line = 1;
          while(fgets(buf, sizeof(buf), ifp)) {
            ptr = buf;
            if(*ptr != ';' and not strblank(ptr)) {
              if((ptr2 = strchr(ptr+2, ';')) != NULL)
                *ptr2 = NUL;
              strtrim(ptr);
              if(line < 6) {
                switch(line) {
                  case 1:
                    ChsTable.id = atol(ptr);
                    break;
                  case 2:
                    ChsTable.version = atoi(ptr);
                    break;
                  case 3:
                    ChsTable.level = atoi(ptr);
                    if((ChsTable.level == 1) or (ChsTable.id > 65535L))
                      ch = 0;
                    else
                      ch = 128;
                    break;
                  case 4:
                    strcpy(ChsTable.imp, strbtrim(ptr));
                    break;
                  case 5:
                    if(ChsTable.level)
                      sprintf(ChsTable.exp, "%s %d", strbtrim(ptr), ChsTable.level);
                    else
                      strcpy(ChsTable.exp, strbtrim(ptr));
                    break;
                }
              }
              else {
                ptr = strskip_wht(ptr);
                if(ch > 255 or strieql(ptr, "END"))   // End of table reached
                  break;
                x = 0;
                ChsTable.t[ch][0] = (byte)x;
                while(*ptr and x < 3) {
                  if(*ptr == '\\') {
                    ptr++;
                    switch(*ptr) {
                      case '\\':
                        ChsTable.t[ch][++x] = (byte)'\\';
                        ChsTable.t[ch][0] = (byte)x;
                        ptr = strskip_txt(ptr);
                        break;
                      case 'd':
                        y = (unsigned char)atoi(ptr+1);
                        ChsTable.t[ch][++x] = (byte)y;
                        ChsTable.t[ch][0] = (byte)x;
                        ptr = strskip_txt(ptr);
                        break;
                      case 'x':
                        y = (unsigned char)atox(ptr+1);
                        ChsTable.t[ch][++x] = (byte)y;
                        ChsTable.t[ch][0] = (byte)x;
                        ptr = strskip_txt(ptr);
                        break;
                      case '0':
                        ptr = strskip_txt(ptr);
                        break;
                    }
                  }
                  else {
                    ChsTable.t[ch][++x] = (byte)*ptr++;
                    ChsTable.t[ch][0] = (byte)x;
                  }
                  ptr = strskip_wht(ptr);
                }
                ch++;
              }
              line++;
            }
          }
          fclose(ifp);
        }

        fwrite(&ChsTable, sizeof(Chs), 1, ofp);
      }

      // Compile ESCSET tables
      for(xlt = CFG->xlatescset.begin(); xlt != CFG->xlatescset.end(); xlt++) {

        // Assign defaults
        memset(&EscTable, 0, sizeof(Esc));
        strcpy(buf, AddPath(CFG->xlatpath, xlt->mapfile));
        ifp = fsopen(buf, "rb", CFG->sharemode);
        if(ifp) {

          if(not quiet)
            cout << "* Reading " << buf << endl;

          // Read the definition file
          line = 1;
          while(fgets(buf, sizeof(buf), ifp)) {
            ptr = buf;
            if(*ptr != ';' and not strblank(ptr)) {
              if((ptr2 = strchr(ptr+2, ';')) != NULL)
                *ptr2 = NUL;
              strtrim(ptr);
              if(line < 2) {
                switch(line) {
                  case 1:
                    strcpy(EscTable.exp, strbtrim(ptr));
                    break;
                }
              }
              else {
                n = EscTable.size;
                if(n > 255 or strieql(ptr, "END"))   // End of table reached
                  break;
                EscTable.t[n][0] = ptr[0];
                EscTable.t[n][1] = ptr[1];
                x = 2;
                ptr += 2;
                ptr = strskip_wht(ptr);
                while(*ptr and x < 5) {
                  if(*ptr == '\\') {
                    ptr++;
                    switch(*ptr) {
                      case '\\':
                        EscTable.t[n][x++] = '\\';
                        ptr = strskip_txt(ptr);
                        break;
                      case 'd':
                        EscTable.t[n][x++] = (unsigned char)atoi(ptr+1);
                        ptr = strskip_txt(ptr);
                        break;
                      case 'x':
                        EscTable.t[n][x++] = (unsigned char)atox(ptr+1);
                        ptr = strskip_txt(ptr);
                        break;
                      case '0':
                        EscTable.t[n][x++] = NUL;
                        ptr = strskip_txt(ptr);
                        break;
                    }
                  }
                  else {
                    EscTable.t[n][x++] = *ptr++;
                  }
                  ptr = strskip_wht(ptr);
                }
                EscTable.size++;
              }
              line++;
            }
          }

          qsort(EscTable.t, EscTable.size, 5, (StdCmpCP)CmpEsc);

          fclose(ifp);
        }

        fwrite(&EscTable, sizeof(Esc), 1, ofp);
      }

      fclose(ofp);
    }
  }
}


//  ------------------------------------------------------------------

void CookieIndex(char* textfile, char* indexfile) {

  FILE* ifp = fsopen(textfile, "rb", CFG->sharemode);
  if(ifp) {
    setvbuf(ifp, NULL, _IOFBF, 32000);
    FILE* ofp = fsopen(indexfile, "wb", CFG->sharemode);
    if(ofp) {
      setvbuf(ofp, NULL, _IOFBF, 16000);
      char buf[256];
      long fpos = 0;
      long tpos = 0;
      bool was_blank = false;
      while(fgets(buf, sizeof(buf), ifp)) {
        if(strblank(buf)) {
          fwrite(&fpos, sizeof(long), 1, ofp);
          fpos = tpos + strlen(buf);
          was_blank = true;
        }
        else {
          was_blank = false;
        }
        tpos += strlen(buf);
      }
      if(not was_blank)
        fwrite(&fpos, sizeof(long), 1, ofp);
      fclose(ofp);
    }
    fclose(ifp);
  }
}


//  ------------------------------------------------------------------

