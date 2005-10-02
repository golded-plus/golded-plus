; $Id$
;
; %Id: rus_ukr.chs,v 0.2 1999/07/28 porokh Exp $
;                  v 0.3 2003/01/20 Liutyi
;
; GoldED charset conversion module
;
; From: Cyrillic DOS (Alt_Codes aka CP866) encoding
;   To: Ukrainian RUSCII (RST 2018-91 aka CP1125) encoding
;
; Format: ID, version, level,
;         from charset, to charset,
;         128 entries: first & second byte
;	  "END"
; Lines beginning with a ";" or a ";" after the entries are comments
;
; Unkown characters are mapped to the "?" character.
;
; \ is the escape character: \0 means decimal zero,
; \dnnn where nnn is a decimal number is the ordinal value of the character
; \xnn where nn is a hexadecimal number
; e.g.: \d32 is the ASCII space character
; Two \\ is the character "\" itself.
;
0               ; ID number
1               ; version number
;
2               ; level number
;
CP866           ; from set
CP1125          ; to set
;
; replace in      from   UNICODE char name in CP866
; CP1125   <--   CP866
\0 \x80         ; 0x80   CYRILLIC CAPITAL LETTER A
\0 \x81         ; 0x81   CYRILLIC CAPITAL LETTER BE
\0 \x82         ; 0x82   CYRILLIC CAPITAL LETTER VE
\0 \x83         ; 0x83   CYRILLIC CAPITAL LETTER GHE
\0 \x84         ; 0x84   CYRILLIC CAPITAL LETTER DE
\0 \x85         ; 0x85   CYRILLIC CAPITAL LETTER IE
\0 \x86         ; 0x86   CYRILLIC CAPITAL LETTER ZHE
\0 \x87         ; 0x87   CYRILLIC CAPITAL LETTER ZE
\0 \x88         ; 0x88   CYRILLIC CAPITAL LETTER I
\0 \x89         ; 0x89   CYRILLIC CAPITAL LETTER SHORT I
\0 \x8a         ; 0x8a   CYRILLIC CAPITAL LETTER KA
\0 \x8b         ; 0x8b   CYRILLIC CAPITAL LETTER EL
\0 \x8c         ; 0x8c   CYRILLIC CAPITAL LETTER EM
\0 \x8d         ; 0x8d   CYRILLIC CAPITAL LETTER EN
\0 \x8e         ; 0x8e   CYRILLIC CAPITAL LETTER O
\0 \x8f         ; 0x8f   CYRILLIC CAPITAL LETTER PE
\0 \x90         ; 0x90   CYRILLIC CAPITAL LETTER ER
\0 \x91         ; 0x91   CYRILLIC CAPITAL LETTER ES
\0 \x92         ; 0x92   CYRILLIC CAPITAL LETTER TE
\0 \x93         ; 0x93   CYRILLIC CAPITAL LETTER U
\0 \x94         ; 0x94   CYRILLIC CAPITAL LETTER EF
\0 \x95         ; 0x95   CYRILLIC CAPITAL LETTER HA
\0 \x96         ; 0x96   CYRILLIC CAPITAL LETTER TSE
\0 \x97         ; 0x97   CYRILLIC CAPITAL LETTER CHE
\0 \x98         ; 0x98   CYRILLIC CAPITAL LETTER SHA
\0 \x99         ; 0x99   CYRILLIC CAPITAL LETTER SHCHA
\0 \x9a         ; 0x9a   CYRILLIC CAPITAL LETTER HARD SIGN
\0 \x9b         ; 0x9b   CYRILLIC CAPITAL LETTER YERU
\0 \x9c         ; 0x9c   CYRILLIC CAPITAL LETTER SOFT SIGN
\0 \x9d         ; 0x9d   CYRILLIC CAPITAL LETTER E
\0 \x9e         ; 0x9e   CYRILLIC CAPITAL LETTER YU
\0 \x9f         ; 0x9f   CYRILLIC CAPITAL LETTER YA
\0 \xa0         ; 0xa0   CYRILLIC SMALL LETTER A
\0 \xa1         ; 0xa1   CYRILLIC SMALL LETTER BE
\0 \xa2         ; 0xa2   CYRILLIC SMALL LETTER VE
\0 \xa3         ; 0xa3   CYRILLIC SMALL LETTER GHE
\0 \xa4         ; 0xa4   CYRILLIC SMALL LETTER DE
\0 \xa5         ; 0xa5   CYRILLIC SMALL LETTER IE
\0 \xa6         ; 0xa6   CYRILLIC SMALL LETTER ZHE
\0 \xa7         ; 0xa7   CYRILLIC SMALL LETTER ZE
\0 \xa8         ; 0xa8   CYRILLIC SMALL LETTER I
\0 \xa9         ; 0xa9   CYRILLIC SMALL LETTER SHORT I
\0 \xaa         ; 0xaa   CYRILLIC SMALL LETTER KA
\0 \xab         ; 0xab   CYRILLIC SMALL LETTER EL
\0 \xac         ; 0xac   CYRILLIC SMALL LETTER EM
\0 \xad         ; 0xad   CYRILLIC SMALL LETTER EN
\0 \xae         ; 0xae   CYRILLIC SMALL LETTER O
\0 \xaf         ; 0xaf   CYRILLIC SMALL LETTER PE
\0 \xb0         ; 0xb0   LIGHT SHADE
\0 \xb1         ; 0xb1   MEDIUM SHADE
\0 \xb2         ; 0xb2   DARK SHADE
\0 \xb3         ; 0xb3   BOX DRAWINGS LIGHT VERTICAL
\0 \xb4         ; 0xb4   BOX DRAWINGS LIGHT VERTICAL AND LEFT
\0 \xb5         ; 0xb5   BOX DRAWINGS VERTICAL SINGLE AND LEFT DOUBLE
\0 \xb6         ; 0xb6   BOX DRAWINGS VERTICAL DOUBLE AND LEFT SINGLE
\0 \xb7         ; 0xb7   BOX DRAWINGS DOWN DOUBLE AND LEFT SINGLE
\0 \xb8         ; 0xb8   BOX DRAWINGS DOWN SINGLE AND LEFT DOUBLE
\0 \xb9         ; 0xb9   BOX DRAWINGS DOUBLE VERTICAL AND LEFT
\0 \xba         ; 0xba   BOX DRAWINGS DOUBLE VERTICAL
\0 \xbb         ; 0xbb   BOX DRAWINGS DOUBLE DOWN AND LEFT
\0 \xbc         ; 0xbc   BOX DRAWINGS DOUBLE UP AND LEFT
\0 \xbd         ; 0xbd   BOX DRAWINGS UP DOUBLE AND LEFT SINGLE
\0 \xbe         ; 0xbe   BOX DRAWINGS UP SINGLE AND LEFT DOUBLE
\0 \xbf         ; 0xbf   BOX DRAWINGS LIGHT DOWN AND LEFT
\0 \xc0         ; 0xc0   BOX DRAWINGS LIGHT UP AND RIGHT
\0 \xc1         ; 0xc1   BOX DRAWINGS LIGHT UP AND HORIZONTAL
\0 \xc2         ; 0xc2   BOX DRAWINGS LIGHT DOWN AND HORIZONTAL
\0 \xc3         ; 0xc3   BOX DRAWINGS LIGHT VERTICAL AND RIGHT
\0 \xc4         ; 0xc4   BOX DRAWINGS LIGHT HORIZONTAL
\0 \xc5         ; 0xc5   BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL
\0 \xc6         ; 0xc6   BOX DRAWINGS VERTICAL SINGLE AND RIGHT DOUBLE
\0 \xc7         ; 0xc7   BOX DRAWINGS VERTICAL DOUBLE AND RIGHT SINGLE
\0 \xc8         ; 0xc8   BOX DRAWINGS DOUBLE UP AND RIGHT
\0 \xc9         ; 0xc9   BOX DRAWINGS DOUBLE DOWN AND RIGHT
\0 \xca         ; 0xca   BOX DRAWINGS DOUBLE UP AND HORIZONTAL
\0 \xcb         ; 0xcb   BOX DRAWINGS DOUBLE DOWN AND HORIZONTAL
\0 \xcc         ; 0xcc   BOX DRAWINGS DOUBLE VERTICAL AND RIGHT
\0 \xcd         ; 0xcd   BOX DRAWINGS DOUBLE HORIZONTAL
\0 \xce         ; 0xce   BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL
\0 \xcf         ; 0xcf   BOX DRAWINGS UP SINGLE AND HORIZONTAL DOUBLE
\0 \xd0         ; 0xd0   BOX DRAWINGS UP DOUBLE AND HORIZONTAL SINGLE
\0 \xd1         ; 0xd1   BOX DRAWINGS DOWN SINGLE AND HORIZONTAL DOUBLE
\0 \xd2         ; 0xd2   BOX DRAWINGS DOWN DOUBLE AND HORIZONTAL SINGLE
\0 \xd3         ; 0xd3   BOX DRAWINGS UP DOUBLE AND RIGHT SINGLE
\0 \xd4         ; 0xd4   BOX DRAWINGS UP SINGLE AND RIGHT DOUBLE
\0 \xd5         ; 0xd5   BOX DRAWINGS DOWN SINGLE AND RIGHT DOUBLE
\0 \xd6         ; 0xd6   BOX DRAWINGS DOWN DOUBLE AND RIGHT SINGLE
\0 \xd7         ; 0xd7   BOX DRAWINGS VERTICAL DOUBLE AND HORIZONTAL SINGLE
\0 \xd8         ; 0xd8   BOX DRAWINGS VERTICAL SINGLE AND HORIZONTAL DOUBLE
\0 \xd9         ; 0xd9   BOX DRAWINGS LIGHT UP AND LEFT
\0 \xda         ; 0xda   BOX DRAWINGS LIGHT DOWN AND RIGHT
\0 \xdb         ; 0xdb   FULL BLOCK
\0 \xdc         ; 0xdc   LOWER HALF BLOCK
\0 \xdd         ; 0xdd   LEFT HALF BLOCK
\0 \xde         ; 0xde   RIGHT HALF BLOCK
\0 \xdf         ; 0xdf   UPPER HALF BLOCK
\0 \xe0         ; 0xe0   CYRILLIC SMALL LETTER ER
\0 \xe1         ; 0xe1   CYRILLIC SMALL LETTER ES
\0 \xe2         ; 0xe2   CYRILLIC SMALL LETTER TE
\0 \xe3         ; 0xe3   CYRILLIC SMALL LETTER U
\0 \xe4         ; 0xe4   CYRILLIC SMALL LETTER EF
\0 \xe5         ; 0xe5   CYRILLIC SMALL LETTER HA
\0 \xe6         ; 0xe6   CYRILLIC SMALL LETTER TSE
\0 \xe7         ; 0xe7   CYRILLIC SMALL LETTER CHE
\0 \xe8         ; 0xe8   CYRILLIC SMALL LETTER SHA
\0 \xe9         ; 0xe9   CYRILLIC SMALL LETTER SHCHA
\0 \xea         ; 0xea   CYRILLIC SMALL LETTER HARD SIGN
\0 \xeb         ; 0xeb   CYRILLIC SMALL LETTER YERU
\0 \xec         ; 0xec   CYRILLIC SMALL LETTER SOFT SIGN
\0 \xed         ; 0xed   CYRILLIC SMALL LETTER E
\0 \xee         ; 0xee   CYRILLIC SMALL LETTER YU
\0 \xef         ; 0xef   CYRILLIC SMALL LETTER YA
\0 \xf0         ; 0xf0   CYRILLIC CAPITAL LETTER IO
\0 \xf1         ; 0xf1   CYRILLIC SMALL LETTER IO
\0 \xf4         ; 0xf2   CYRILLIC CAPITAL LETTER UKRAINIAN IE
\0 \xf5         ; 0xf3   CYRILLIC SMALL LETTER UKRAINIAN IE
\0 \xf8         ; 0xf4   CYRILLIC CAPITAL LETTER YI
\0 \xf9         ; 0xf5   CYRILLIC SMALL LETTER YI
\0 \x93         ; 0xf6   CYRILLIC CAPITAL LETTER SHORT U
\0 \xe3         ; 0xf7   CYRILLIC SMALL LETTER SHORT U
\0 \x6f         ; 0xf8   DEGREE SIGN
\0 \x2a         ; 0xf9   BULLET OPERATOR
\0 \xfa         ; 0xfa   MIDDLE DOT
\0 \xfb         ; 0xfb   SQUARE ROOT
\0 \xfc         ; 0xfc   NUMERO SIGN
\0 \xfd         ; 0xfd   CURRENCY SIGN
\0 \xfe         ; 0xfe   BLACK SQUARE
\0 \xff         ; 0xff   NO-BREAK SPACE
END
