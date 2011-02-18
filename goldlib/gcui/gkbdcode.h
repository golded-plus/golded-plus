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
//  Define mnemonic symbols for full set of PC-Compatible non-display
//  control and extended keycodes.
//  ------------------------------------------------------------------

#ifndef __gkbdcode_h
#define __gkbdcode_h


//  ------------------------------------------------------------------
//  Notes and Caveats (from the original TCXL 6.0 TCXLCOD.H file)
//
//  1.  The keycode symbols suffixed with 'G' and marked in the 'G'
//      column are the "Grey" keys around the numeric keypad or in a
//      separate cursor-key cluster on "enhanced" keyboards. Those
//      marked in the 'N' column are on numeric keypad.
//  2.  The keycodes marked in the 'E' column are only available on
//      "enhanced" 101/102-key keyboards with extended keyboard BIOS
//      support enabled. The extended BIOS returns keycodes containing
//      0xE0 to distinguish duplicated keys, which are marked in the
//      'T' column.
//  3.  If an "enhanced" keyboard and extended BIOS is detected, the
//      DEFAULT is to enable extended BIOS support and translation of
//      any duplicate extended keycodes.
//  4.  Keycodes are ordered by character-code, then scan-code.
//  5.  Control-Break, which has an actual keycode of 0x000 is
//      returned as 0xFFFF.
//  6.  The ASCII control-code names are shown in square brackets.
//  7.  The Key_M_??? mouse-return codes are those returned by
//      _MouGet().
//  ------------------------------------------------------------------

#include <gdefs.h>


//  ------------------------------------------------------------------

#define  Key_Lsr     '<'
#define  Key_Gtr     '>'
#define  Key_Multi   '*'
#define  Key_Plus    '+'
#define  Key_Minus   '-'


//  ------------------------------------------------------------------
                                     //- Normal key -------- N G E T -
#define  Key_0       '0'             // <0>                  - - - -
#define  Key_1       '1'             // <1>                  - - - -
#define  Key_2       '2'             // <2>                  - - - -
#define  Key_3       '3'             // <3>                  - - - -
#define  Key_4       '4'             // <4>                  - - - -
#define  Key_5       '5'             // <5>                  - - - -
#define  Key_6       '6'             // <6>                  - - - -
#define  Key_7       '7'             // <7>                  - - - -
#define  Key_8       '8'             // <8>                  - - - -
#define  Key_9       '9'             // <9>                  - - - -

#define  Key_A       0x1E61          // <a>                  - - - -
#define  Key_B       0x3062          // <b>                  - - - -
#define  Key_C       0x2E63          // <c>                  - - - -
#define  Key_D       0x2064          // <d>                  - - - -
#define  Key_E       0x1265          // <e>                  - - - -
#define  Key_F       0x2166          // <f>                  - - - -
#define  Key_G       0x2267          // <g>                  - - - -
#define  Key_H       0x2368          // <h>                  - - - -
#define  Key_I       0x1769          // <i>                  - - - -
#define  Key_J       0x246a          // <j>                  - - - -
#define  Key_K       0x256b          // <k>                  - - - -
#define  Key_L       0x266c          // <l>                  - - - -
#define  Key_M       0x326d          // <m>                  - - - -
#define  Key_N       0x316e          // <n>                  - - - -
#define  Key_O       0x186f          // <o>                  - - - -
#define  Key_P       0x1970          // <p>                  - - - -
#define  Key_Q       0x1071          // <q>                  - - - -
#define  Key_R       0x1372          // <r>                  - - - -
#define  Key_S       0x1F73          // <s>                  - - - -
#define  Key_T       0x1474          // <t>                  - - - -
#define  Key_U       0x1675          // <u>                  - - - -
#define  Key_V       0x2F76          // <v>                  - - - -
#define  Key_W       0x1177          // <w>                  - - - -
#define  Key_X       0x2D78          // <x>                  - - - -
#define  Key_Y       0x1579          // <y>                  - - - -
#define  Key_Z       0x2C7A          // <z>                  - - - -
#define  Key_StrG    0x372A          //   <*>                - G - -
#define  Key_PlsG    0x4E2B          //   <+>                - G - -
#define  Key_Com     0x332C          //   <,>                - - - -
#define  Key_MinG    0x4A2D          //   <->                - G - -
#define  Key_Dot     0x342E          //   <.>                - - - -
#define  Key_BS      0x0E08          //   <BS>               - - - -
#define  Key_5Num    0x4C00          //   <Cent>             N - E -
#define  Key_Del     0x5300          //   <Del>              N - - -
#define  Key_DelG    0x53E0          //   <Del>     Grey     - G E T
#define  Key_Dwn     0x5000          //   <Down>             N - - -
#define  Key_DwnG    0x50E0          //   <Down>    Grey     - G E T
#define  Key_End     0x4F00          //   <End>              N - - -
#define  Key_EndG    0x4FE0          //   <End>     Grey     - G E T
#define  Key_Ent     0x1C0D          //   <Enter>            - - - -
#define  Key_EntG    0xE00D          //   <Enter>   Grey     - G E T
#define  Key_Esc     0x011B          //   <Esc>              - - - -
#define  Key_F1      0x3B00          //   <F1>               - - - -
#define  Key_F2      0x3C00          //   <F2>               - - - -
#define  Key_F3      0x3D00          //   <F3>               - - - -
#define  Key_F4      0x3E00          //   <F4>               - - - -
#define  Key_F5      0x3F00          //   <F5>               - - - -
#define  Key_F6      0x4000          //   <F6>               - - - -
#define  Key_F7      0x4100          //   <F7>               - - - -
#define  Key_F8      0x4200          //   <F8>               - - - -
#define  Key_F9      0x4300          //   <F9>               - - - -
#define  Key_F10     0x4400          //   <F10>              - - - -
#define  Key_F11     0x8500          //   <F11>              - - E -
#define  Key_F12     0x8600          //   <F12>              - - E -
#define  Key_Home    0x4700          //   <Home>             N - - -
#define  Key_HomeG   0x47E0          //   <Home>    Grey     - G E T
#define  Key_Ins     0x5200          //   <Ins>              N - - -
#define  Key_InsG    0x52E0          //   <Ins>     Grey     - G E T
#define  Key_Lft     0x4B00          //   <Left>             N - - -
#define  Key_LftG    0x4BE0          //   <Left>    Grey     - G E T
#define  Key_PgDn    0x5100          //   <PgDn>             N - - -
#define  Key_PgDnG   0x51E0          //   <PgDn>    Grey     - G E T
#define  Key_PgUp    0x4900          //   <PgUp>             N - - -
#define  Key_PgUpG   0x49E0          //   <PgUp>    Grey     - G E T
#define  Key_Rgt     0x4D00          //   <Right>            N - - -
#define  Key_RgtG    0x4DE0          //   <Right>   Grey     - G E T
#define  Key_Space   0x3920          //   <Space>            - - - -
#define  Key_Tab     0x0F09          //   <Tab>              - - - -
#define  Key_Up      0x4800          //   <Up>               N - - -
#define  Key_UpG     0x48E0          //   <Up>      Grey     - G E T
#define  Key_Quo     0x2827          // <'>                  - - - -
#define  Key_Min     0x0C2D          // <->                  - - - -
#define  Key_Sls     0x352F          // </>                  - - - -
#define  Key_Smi     0x273B          // <;>                  - - - -
#define  Key_Equ     0x0D3D          // <=>                  - - - -
#define  Key_Lbr     0x1A5B          // <[>                  - - - -
#define  Key_Bsl     0x2B5C          // <\>                  - - - -
#define  Key_Rbr     0x1B5D          // <]>                  - - - -
#define  Key_Grv     0x2960          // <`>                  - - - -

//  ------------------------------------------------------------------
                                     //- Shift key --------- N G E T -
#define  Key_S_0     0x0B29          // <0 )>                - - - -
#define  Key_S_1     0x0221          // <1 !>                - - - -
#define  Key_S_2     0x0340          // <2 @>                - - - -
#define  Key_S_3     0x0423          // <3 #>                - - - -
#define  Key_S_4     0x0524          // <4 $>                - - - -
#define  Key_S_5     0x0625          // <5 %>                - - - -
#define  Key_S_6     0x075E          // <6 ^>                - - - -
#define  Key_S_7     0x0826          // <7 &>                - - - -
#define  Key_S_8     0x092A          // <8 *>                - - - -
#define  Key_S_9     0x0A28          // <9 (>                - - - -
#define  Key_S_A     0x1E41          // <A>                  - - - -
#define  Key_S_B     0x3042          // <B>                  - - - -
#define  Key_S_C     0x2E43          // <C>                  - - - -
#define  Key_S_D     0x2044          // <D>                  - - - -
#define  Key_S_E     0x1245          // <E>                  - - - -
#define  Key_S_F     0x2146          // <F>                  - - - -
#define  Key_S_G     0x2247          // <G>                  - - - -
#define  Key_S_H     0x2348          // <H>                  - - - -
#define  Key_S_I     0x1749          // <I>                  - - - -
#define  Key_S_J     0x244a          // <J>                  - - - -
#define  Key_S_K     0x254b          // <K>                  - - - -
#define  Key_S_L     0x264c          // <L>                  - - - -
#define  Key_S_M     0x324d          // <M>                  - - - -
#define  Key_S_N     0x314e          // <N>                  - - - -
#define  Key_S_O     0x184f          // <O>                  - - - -
#define  Key_S_P     0x1950          // <P>                  - - - -
#define  Key_S_Q     0x1051          // <Q>                  - - - -
#define  Key_S_R     0x1352          // <R>                  - - - -
#define  Key_S_S     0x1F53          // <S>                  - - - -
#define  Key_S_T     0x1454          // <T>                  - - - -
#define  Key_S_U     0x1655          // <U>                  - - - -
#define  Key_S_V     0x2F56          // <V>                  - - - -
#define  Key_S_W     0x1157          // <W>                  - - - -
#define  Key_S_X     0x2D58          // <X>                  - - - -
#define  Key_S_Y     0x1559          // <Y>                  - - - -
#define  Key_S_Z     0x2C5a          // <Z>                  - - - -
#define  Key_S_Quo   0x2822          // <' ">                - - - -
#define  Key_S_Com   0x333C          // <, >>                - - - -
#define  Key_S_Min   0x0C5F          // <- _>                - - - -
#define  Key_S_Dot   0x343E          // <. <>                - - - -
#define  Key_S_Sls   0x353F          // </ ?>                - - - -
#define  Key_S_Smi   0x273A          // <; :>                - - - -
#define  Key_S_Equ   0x0D2B          // <= +>                - - - -
#define  Key_S_Lbr   0x1A7B          // <[ {>                - - - -
#define  Key_S_Bsl   0x2B7C          // <\ |>                - - - -
#define  Key_S_Rbr   0x1B7D          // <] }>                - - - -
#define  Key_S_Grv   0x297E          // <` ~>                - - - -
#define  Key_S_5Num  0x4C35          // S <Cent 5>           N - - -
#define  Key_S_F1    0x5400          // S <F1>               - - - -
#define  Key_S_F2    0x5500          // S <F2>               - - - -
#define  Key_S_F3    0x5600          // S <F3>               - - - -
#define  Key_S_F4    0x5700          // S <F4>               - - - -
#define  Key_S_F5    0x5800          // S <F5>               - - - -
#define  Key_S_F6    0x5900          // S <F6>               - - - -
#define  Key_S_F7    0x5A00          // S <F7>               - - - -
#define  Key_S_F8    0x5B00          // S <F8>               - - - -
#define  Key_S_F9    0x5C00          // S <F9>               - - - -
#define  Key_S_F10   0x5D00          // S <F10>              - - - -
#define  Key_S_F11   0x8700          // S <F11>              - - E -
#define  Key_S_F12   0x8800          // S <F12>              - - E -
#define  Key_S_Tab   0x0F00          // S <Tab>              - - - -
#if !defined(__UNIX__) || defined(__USE_NCURSES__)
#define  Key_S_Ins   0xD200          // S <Ins>     Grey     - G E T
#define  Key_S_Lft   0xCB00          // S <Left>    Grey     - G E T
#define  Key_S_PgDn  0xD100          // S <PgDn>    Grey     - G E T
#define  Key_S_PgUp  0xC900          // S <PgUp>    Grey     - G E T
#define  Key_S_Rgt   0xCD00          // S <Right>   Grey     - G E T
#define  Key_S_Up    0xC800          // S <Up>      Grey     - G E T
#define  Key_S_Dwn   0xD000          // S <Down>    Grey     - G E T
#define  Key_S_End   0xCF00          // S <End>     Grey     - G E T
#define  Key_S_Home  0xC700          // S <Home>    Grey     - G E T
#define  Key_S_Del   0xD300          // S <Del .>            N - - -
#else
#define  Key_S_Home  0x4737          // S <Home 7>           N - - -
#define  Key_S_HomeG 0x47E0          // S <Home>    Grey     - G E T
#define  Key_S_Ins   0x5230          // S <Ins 0>            N - - -
#define  Key_S_InsG  0x52E0          // S <Ins>     Grey     - G E T
#define  Key_S_Lft   0x4B34          // S <Left 4>           N - - -
#define  Key_S_LftG  0x4BE0          // S <Left>    Grey     - G E T
#define  Key_S_PgDn  0x5133          // S <PgDn 3>           N - - -
#define  Key_S_PgDnG 0x51E0          // S <PgDn>    Grey     - G E T
#define  Key_S_PgUp  0x4939          // S <PgUp 9>           N - - -
#define  Key_S_PgUpG 0x49E0          // S <PgUp>    Grey     - G E T
#define  Key_S_Rgt   0x4D36          // S <Right 6>          N - - -
#define  Key_S_RgtG  0x4DE0          // S <Right>   Grey     - G E T
#define  Key_S_Up    0x4838          // S <Up 8>             N - - -
#define  Key_S_UpG   0x48E0          // S <Up>      Grey     - G E T
#define  Key_S_Dwn   0x5032          // S <Down 2>           N - - -
#define  Key_S_DwnG  0x50E0          // S <Down>    Grey     - G E T
#define  Key_S_End   0x4F31          // S <End 1>            N - - -
#define  Key_S_EndG  0x4FE0          // S <End>     Grey     - G E T
#define  Key_S_Del   0x532E          // S <Del .>            N - - -
#define  Key_S_DelG  0x53E0          // S <Del>     Grey     - G E T
#endif

//  ------------------------------------------------------------------
                                     //- Control key ------- N G E T -
#define  Key_C_2     0x0300          // C <2 @>     [NUL]    - - - -
#define  Key_C_A     0x1E01          // C <A>       [SOH]    - - - -
#define  Key_C_B     0x3002          // C <B>       [STX]    - - - -
#define  Key_C_C     0x2E03          // C <C>       [ETX]    - - - -
#define  Key_C_D     0x2004          // C <D>       [EOT]    - - - -
#define  Key_C_E     0x1205          // C <E>       [ENQ]    - - - -
#define  Key_C_F     0x2106          // C <F>       [ACK]    - - - -
#define  Key_C_G     0x2207          // C <G>       [BEL]    - - - -
#define  Key_C_H     0x2308          // C <H>       [BS]     - - - -
#define  Key_C_I     0x1709          // C <I>       [HT]     - - - -
#define  Key_C_J     0x240A          // C <J>       [LF]     - - - -
#define  Key_C_K     0x250B          // C <K>       [VT]     - - - -
#define  Key_C_L     0x260C          // C <L>       [FF]     - - - -
#define  Key_C_M     0x320D          // C <M>       [CR]     - - - -
#define  Key_C_N     0x310E          // C <N>       [SO]     - - - -
#define  Key_C_O     0x180F          // C <O>       [SI]     - - - -
#define  Key_C_P     0x1910          // C <P>       [DLE]    - - - -
#define  Key_C_Q     0x1011          // C <Q>       [DC1]    - - - -
#define  Key_C_R     0x1312          // C <R>       [DC2]    - - - -
#define  Key_C_S     0x1F13          // C <S>       [DC3]    - - - -
#define  Key_C_T     0x1414          // C <T>       [DC4]    - - - -
#define  Key_C_U     0x1615          // C <U>       [NAK]    - - - -
#define  Key_C_V     0x2F16          // C <V>       [SYN]    - - - -
#define  Key_C_W     0x1117          // C <W>       [ETB]    - - - -
#define  Key_C_X     0x2D18          // C <X>       [CAN]    - - - -
#define  Key_C_Y     0x1519          // C <Y>       [EM]     - - - -
#define  Key_C_Z     0x2C1A          // C <Z>       [SUB]    - - - -
#define  Key_C_StrG  0x7200          // C <*>       Grey     - G E -
#define  Key_C_PlsG  0x9000          // C <+>       Grey     - G E -
#define  Key_C_Min   0x0C1F          // C <- _>              - - - -
#define  Key_C_MinG  0x8E00          // C <->       Grey     - G E -
#define  Key_C_6     0x071E          // C <7 &>     [RS]     - - - -
#define  Key_C_Brk   0xFFFF          // C <Break>   Grey     - - - -
#define  Key_C_BS    0x0E7F          // C <BS>      [RUB]    - - - -
#define  Key_C_5Num  0x8F00          // C <Cent 5>           N - E -
#define  Key_C_Del   0x9300          // C <Del 2>            - - E -
#define  Key_C_DelG  0x93E0          // C <Del>     Grey     - G E T
#define  Key_C_Dwn   0x9100          // C <Dwn 2>            - - E -
#define  Key_C_DwnG  0x91E0          // C <Dwn>     Grey     - G E T
#define  Key_C_End   0x7500          // C <End 1>            N - - -
#define  Key_C_EndG  0x75E0          // C <End>     Grey     - G E T
#define  Key_C_EntG  0xE00A          // C <Enter>   Grey     - G E T
#define  Key_C_Ent   0x1C0A          // C <Enter>   [LF]     - - - -
#define  Key_C_F1    0x5E00          // C <F1>               - - - -
#define  Key_C_F2    0x5F00          // C <F2>               - - - -
#define  Key_C_F3    0x6000          // C <F3>               - - - -
#define  Key_C_F4    0x6100          // C <F4>               - - - -
#define  Key_C_F5    0x6200          // C <F5>               - - - -
#define  Key_C_F6    0x6300          // C <F6>               - - - -
#define  Key_C_F7    0x6400          // C <F7>               - - - -
#define  Key_C_F8    0x6500          // C <F8>               - - - -
#define  Key_C_F9    0x6600          // C <F9>               - - - -
#define  Key_C_F10   0x6700          // C <F10>              - - - -
#define  Key_C_F11   0x8900          // C <F11>              - - E -
#define  Key_C_F12   0x8A00          // C <F12>              - - E -
#define  Key_C_Home  0x7700          // C <Home 7>           N - - -
#define  Key_C_HomeG 0x77E0          // C <Home>    Grey     - G E T
#define  Key_C_Ins   0x9200          // C <Ins 0>            - - E -
#define  Key_C_InsG  0x92E0          // C <Ins>     Grey     - G E T
#define  Key_C_Lft   0x7300          // C <Left 4>           N - - -
#define  Key_C_LftG  0x73E0          // C <Left>    Grey     - G E T
#define  Key_C_PgDn  0x7600          // C <PgDn 3>           N - - -
#define  Key_C_PgDnG 0x76E0          // C <PgDn>    Grey     - G E T
#define  Key_C_PgUp  0x8400          // C <PgUp 9>           N - - -
#define  Key_C_PgUpG 0x84E0          // C <PgUp>    Grey     - G E T
#define  Key_C_Rgt   0x7400          // C <Right 6>          N - - -
#define  Key_C_RgtG  0x74E0          // C <Right>   Grey     - G E T
#define  Key_C_Tab   0x9400          // C <Tab>              - - E -
#define  Key_C_Up    0x8D00          // C <Up 8>             - - E -
#define  Key_C_UpG   0x8DE0          // C <Up>      Grey     - G E T
#define  Key_C_Lbr   0x1A1B          // C <[ {>     [ESC]    - - - -
#define  Key_C_Bsl   0x2B1C          // C <\ |>     [FS]     - - - -
#define  Key_C_Rbr   0x1B1D          // C <] }>     [GS]     - - - -

//  ------------------------------------------------------------------
                                     //- Alt key ----------- N G E T -
#define  Key_A_0     0x8100          // A <0 )>              - - - -
#define  Key_A_1     0x7800          // A <1 !>              - - - -
#define  Key_A_2     0x7900          // A <2 @>              - - - -
#define  Key_A_3     0x7A00          // A <3 #>              - - - -
#define  Key_A_4     0x7B00          // A <4 ^>              - - - -
#define  Key_A_5     0x7C00          // A <5 %>              - - - -
#define  Key_A_6     0x7D00          // A <6 %>              - - - -
#define  Key_A_7     0x7E00          // A <7 &>              - - - -
#define  Key_A_8     0x7F00          // A <8 *>              - - - -
#define  Key_A_9     0x8000          // A <9 (>              - - - -
#define  Key_A_A     0x1E00          // A <A>                - - - -
#define  Key_A_B     0x3000          // A <B>                - - - -
#define  Key_A_C     0x2E00          // A <C>                - - - -
#define  Key_A_D     0x2000          // A <D>                - - - -
#define  Key_A_E     0x1200          // A <E>                - - - -
#define  Key_A_F     0x2100          // A <F>                - - - -
#define  Key_A_G     0x2200          // A <G>                - - - -
#define  Key_A_H     0x2300          // A <H>                - - - -
#define  Key_A_I     0x1700          // A <I>                - - - -
#define  Key_A_J     0x2400          // A <J>                - - - -
#define  Key_A_K     0x2500          // A <K>                - - - -
#define  Key_A_L     0x2600          // A <L>                - - - -
#define  Key_A_M     0x3200          // A <M>                - - - -
#define  Key_A_N     0x3100          // A <N>                - - - -
#define  Key_A_O     0x1800          // A <O>                - - - -
#define  Key_A_P     0x1900          // A <P>                - - - -
#define  Key_A_Q     0x1000          // A <Q>                - - - -
#define  Key_A_R     0x1300          // A <R>                - - - -
#define  Key_A_S     0x1F00          // A <S>                - - - -
#define  Key_A_T     0x1400          // A <T>                - - - -
#define  Key_A_U     0x1600          // A <U>                - - - -
#define  Key_A_V     0x2F00          // A <V>                - - - -
#define  Key_A_W     0x1100          // A <W>                - - - -
#define  Key_A_X     0x2D00          // A <X>                - - - -
#define  Key_A_Y     0x1500          // A <Y>                - - - -
#define  Key_A_Z     0x2C00          // A <Z>                - - - -
#define  Key_A_Quo   0x2800          // A <' ">              - - E -
#define  Key_A_PlsG  0x4E00          // A <+>       Grey     - G E -
#define  Key_A_Com   0x3300          // A <, <>              - - E -
#define  Key_A_Min   0x8200          // A <- _>              - - - -
#define  Key_A_MinG  0x4A00          // A <->       Grey     - G E -
#define  Key_A_Dot   0x3400          // A <. >>              - - E -
#define  Key_A_Sls   0x3500          // A </ ?>              - - E -
#define  Key_A_Smi   0x2700          // A <; :>              - - E -
#define  Key_A_Equ   0x8300          // A <= +>              - - - -
#define  Key_A_BS    0x0E00          // A <BS>               - - E -
#define  Key_A_5Num  0x4C00          // A <Cent 5>           N - E -
#define  Key_A_Del   0xA300          // A <Del>              - - E -
#define  Key_A_DelG  0xA300          // A <Del>     Grey     - G E -
#define  Key_A_Dwn   0xA000          // A <Dwn>              - - E -
#define  Key_A_DwnG  0xA000          // A <Dwn>     Grey     - G E -
#define  Key_A_End   0x9F00          // A <End>              - - E -
#define  Key_A_EndG  0x9F00          // A <End>     Grey     - G E -
#define  Key_A_EntG  0xA600          // A <Enter    Grey     - G E -
#define  Key_A_Ent   0x1C00          // A <Enter>            - - E -
#define  Key_A_Esc   0x0100          // A <Esc>              - - E -
#define  Key_A_F1    0x6800          // A <F1>               - - - -
#define  Key_A_F2    0x6900          // A <F2>               - - - -
#define  Key_A_F3    0x6A00          // A <F3>               - - - -
#define  Key_A_F4    0x6B00          // A <F4>               - - - -
#define  Key_A_F5    0x6C00          // A <F5>               - - - -
#define  Key_A_F6    0x6D00          // A <F6>               - - - -
#define  Key_A_F7    0x6E00          // A <F7>               - - - -
#define  Key_A_F8    0x6F00          // A <F8>               - - - -
#define  Key_A_F9    0x7000          // A <F9>               - - - -
#define  Key_A_F10   0x7100          // A <F10>              - - - -
#define  Key_A_F11   0x8B00          // A <F11>              - - E -
#define  Key_A_F12   0x8C00          // A <F12>              - - E -
#define  Key_A_Home  0x9700          // A <Home>             - - E -
#define  Key_A_HomeG 0x9700          // A <Home>    Grey     - G E -
#define  Key_A_Ins   0xA200          // A <Ins>              - - E -
#define  Key_A_InsG  0xA200          // A <Ins>     Grey     - G E -
#define  Key_A_Lft   0x9B00          // A <Left>             - - E -
#define  Key_A_LftG  0x9B00          // A <Left>    Grey     - G E -
#define  Key_A_PgDn  0xA100          // A <PgDn>             - - E -
#define  Key_A_PgDnG 0xA100          // A <PgDn>    Grey     - G E -
#define  Key_A_PgUp  0x9900          // A <PgUp>             - - E -
#define  Key_A_PgUpG 0x9900          // A <PgUp>    Grey     - G E -
#define  Key_A_Rgt   0x9D00          // A <Right             - - E -
#define  Key_A_RgtG  0x9D00          // A <Right    Grey     - G E -
#define  Key_A_Tab   0xA500          // A <Tab>              - - E -
#define  Key_A_Up    0x9800          // A <Up>               - - E -
#define  Key_A_UpG   0x9800          // A <Up>      Grey     - G E -
#define  Key_A_Lbr   0x1A00          // A <[ {>              - - E -
#define  Key_A_Bsl   0x2B00          // A <\ |>              - - E -
#define  Key_A_Rbr   0x1B00          // A <] }>              - - E -
#define  Key_A_Grv   0x2900          // A <` ~>              - - E -

//  ------------------------------------------------------------------
                                     //- Mouse KeyCode Returns -------
#define  Key_M_Scn   0xD4            // Scan-code
#define  Key_M_Clk   0x30            // Single-click keycodes
#define  Key_M_ClkL  0xD431          //    Left-button
#define  Key_M_ClkR  0xD432          //    Right-button
#define  Key_M_ClkM  0xD434          //    Middle-button

#define  Key_M_DClk  0x20            // Double-click keycodes
#define  Key_M_DClkL 0xD421          //    Left-button
#define  Key_M_DClkR 0xD422          //    Right-button
#define  Key_M_DClkM 0xD424          //    Middle-button

#define  Key_M_Prs   0x30            // Single-press keycodes
#define  Key_M_PrsL  0xD431          //    Left-button
#define  Key_M_PrsR  0xD432          //    Right-button
#define  Key_M_PrsM  0xD434          //    Middle-button
#define  Key_M_Rel   0x40            // Single-release keycodes
#define  Key_M_RelL  0xD441          //    Left-button
#define  Key_M_RelR  0xD442          //    Right-button
#define  Key_M_RelM  0xD444          //    Middle-button

#define  Key_M_Mov   0x50            // Mouse motion keycodes
#define  Key_M_Up    0xD450          //    Motion [Up]
#define  Key_M_Dwn   0xD451          //    Motion [Down]
#define  Key_M_Lft   0xD452          //    Motion [Left]
#define  Key_M_Rgt   0xD453          //    Motion [Right]


//  ------------------------------------------------------------------
//  Goldware internal keycodes

#define  Key_Tick    0x0200          // Timer tick
#define  Key_Auto    0xFD00          // Auto macro
#define  Key_Macro   0xFE00          // Macro


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
