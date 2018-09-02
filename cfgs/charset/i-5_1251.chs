; $Id$
; This file is a charset conversion module for Golded+
; Source charset is ISO8859-5, 8-bit russian charset
; Destination charset is CP1251, 8-bit russian charset (Windows ANSI)
;
; Copyleft (c) Stas Degteff 2:5080/102@fidonet stas_degteff@users.sourceforge.net
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
0
0
;
2
iso8859-5	; from charset
cp1251	; to charset
;
\0 ?	; not defined
\0 ?	; not defined
\0 ?	; not defined
\0 ?	; not defined
\0 ?	; not defined
\0 ?	; not defined
\0 ?	; not defined
\0 ?	; not defined
\0 ?	; not defined
\0 ?	; not defined
\0 ?	; not defined
\0 ?	; not defined
\0 ?	; not defined
\0 ?	; not defined
\0 ?	; not defined
\0 ?	; not defined
\0 ?	; not defined
\0 ?	; not defined
\0 ?	; not defined
\0 ?	; not defined
\0 ?	; not defined
\0 ?	; not defined
\0 ?	; not defined
\0 ?	; not defined
\0 ?	; not defined
\0 ?	; not defined
\0 ?	; not defined
\0 ?	; not defined
\0 ?	; not defined
\0 ?	; not defined
\0 ?	; not defined
\0 ?	; not defined
\0 \xa0	; NO-BREAK SPACE
\0 \xa8	; CYRILLIC CAPITAL LETTER IO
\0 \x80	; CYRILLIC CAPITAL LETTER DJE
\0 \x81	; CYRILLIC CAPITAL LETTER GJE
\0 \xaa	; CYRILLIC CAPITAL LETTER UKRAINIAN IE
\0 \xbd	; CYRILLIC CAPITAL LETTER DZE
\0 \xb2	; CYRILLIC CAPITAL LETTER BYELORUSSIAN-UKRAINIAN I
\0 \xaf	; CYRILLIC CAPITAL LETTER YI
\0 \xa3	; CYRILLIC CAPITAL LETTER JE
\0 \x8a	; CYRILLIC CAPITAL LETTER LJE
\0 \x8c	; CYRILLIC CAPITAL LETTER NJE
\0 \x8e	; CYRILLIC CAPITAL LETTER TSHE
\0 \x8d	; CYRILLIC CAPITAL LETTER KJE
\0 \xad	; SOFT HYPHEN
\0 \xa1	; CYRILLIC CAPITAL LETTER SHORT U
\0 \x8f	; CYRILLIC CAPITAL LETTER DZHE
\0 \xc0	; CYRILLIC CAPITAL LETTER A
\0 \xc1	; CYRILLIC CAPITAL LETTER BE
\0 \xc2	; CYRILLIC CAPITAL LETTER VE
\0 \xc3	; CYRILLIC CAPITAL LETTER GHE
\0 \xc4	; CYRILLIC CAPITAL LETTER DE
\0 \xc5	; CYRILLIC CAPITAL LETTER IE
\0 \xc6	; CYRILLIC CAPITAL LETTER ZHE
\0 \xc7	; CYRILLIC CAPITAL LETTER ZE
\0 \xc8	; CYRILLIC CAPITAL LETTER I
\0 \xc9	; CYRILLIC CAPITAL LETTER SHORT I
\0 \xca	; CYRILLIC CAPITAL LETTER KA
\0 \xcb	; CYRILLIC CAPITAL LETTER EL
\0 \xcc	; CYRILLIC CAPITAL LETTER EM
\0 \xcd	; CYRILLIC CAPITAL LETTER EN
\0 \xce	; CYRILLIC CAPITAL LETTER O
\0 \xcf	; CYRILLIC CAPITAL LETTER PE
\0 \xd0	; CYRILLIC CAPITAL LETTER ER
\0 \xd1	; CYRILLIC CAPITAL LETTER ES
\0 \xd2	; CYRILLIC CAPITAL LETTER TE
\0 \xd3	; CYRILLIC CAPITAL LETTER U
\0 \xd4	; CYRILLIC CAPITAL LETTER EF
\0 \xd5	; CYRILLIC CAPITAL LETTER HA
\0 \xd6	; CYRILLIC CAPITAL LETTER TSE
\0 \xd7	; CYRILLIC CAPITAL LETTER CHE
\0 \xd8	; CYRILLIC CAPITAL LETTER SHA
\0 \xd9	; CYRILLIC CAPITAL LETTER SHCHA
\0 \xda	; CYRILLIC CAPITAL LETTER HARD SIGN
\0 \xdb	; CYRILLIC CAPITAL LETTER YERU
\0 \xdc	; CYRILLIC CAPITAL LETTER SOFT SIGN
\0 \xdd	; CYRILLIC CAPITAL LETTER E
\0 \xde	; CYRILLIC CAPITAL LETTER YU
\0 \xdf	; CYRILLIC CAPITAL LETTER YA
\0 \xe0	; CYRILLIC SMALL LETTER A
\0 \xe1	; CYRILLIC SMALL LETTER BE
\0 \xe2	; CYRILLIC SMALL LETTER VE
\0 \xe3	; CYRILLIC SMALL LETTER GHE
\0 \xe4	; CYRILLIC SMALL LETTER DE
\0 \xe5	; CYRILLIC SMALL LETTER IE
\0 \xe6	; CYRILLIC SMALL LETTER ZHE
\0 \xe7	; CYRILLIC SMALL LETTER ZE
\0 \xe8	; CYRILLIC SMALL LETTER I
\0 \xe9	; CYRILLIC SMALL LETTER SHORT I
\0 \xea	; CYRILLIC SMALL LETTER KA
\0 \xeb	; CYRILLIC SMALL LETTER EL
\0 \xec	; CYRILLIC SMALL LETTER EM
\0 \xed	; CYRILLIC SMALL LETTER EN
\0 \xee	; CYRILLIC SMALL LETTER O
\0 \xef	; CYRILLIC SMALL LETTER PE
\0 \xf0	; CYRILLIC SMALL LETTER ER
\0 \xf1	; CYRILLIC SMALL LETTER ES
\0 \xf2	; CYRILLIC SMALL LETTER TE
\0 \xf3	; CYRILLIC SMALL LETTER U
\0 \xf4	; CYRILLIC SMALL LETTER EF
\0 \xf5	; CYRILLIC SMALL LETTER HA
\0 \xf6	; CYRILLIC SMALL LETTER TSE
\0 \xf7	; CYRILLIC SMALL LETTER CHE
\0 \xf8	; CYRILLIC SMALL LETTER SHA
\0 \xf9	; CYRILLIC SMALL LETTER SHCHA
\0 \xfa	; CYRILLIC SMALL LETTER HARD SIGN
\0 \xfb	; CYRILLIC SMALL LETTER YERU
\0 \xfc	; CYRILLIC SMALL LETTER SOFT SIGN
\0 \xfd	; CYRILLIC SMALL LETTER E
\0 \xfe	; CYRILLIC SMALL LETTER YU
\0 \xff	; CYRILLIC SMALL LETTER YA
\0 \xb9	; NUMERO SIGN
\0 \xb8	; CYRILLIC SMALL LETTER IO
\0 \x90	; CYRILLIC SMALL LETTER DJE
\0 \x83	; CYRILLIC SMALL LETTER GJE
\0 \xba	; CYRILLIC SMALL LETTER UKRAINIAN IE
\0 \xbe	; CYRILLIC SMALL LETTER DZE
\0 \xb3	; CYRILLIC SMALL LETTER BYELORUSSIAN-UKRAINIAN I
\0 \xbf	; CYRILLIC SMALL LETTER YI
\0 \xbc	; CYRILLIC SMALL LETTER JE
\0 \x9a	; CYRILLIC SMALL LETTER LJE
\0 \x9c	; CYRILLIC SMALL LETTER NJE
\0 \x9e	; CYRILLIC SMALL LETTER TSHE
\0 \x9d	; CYRILLIC SMALL LETTER KJE
\0 \xa7	; SECTION SIGN
\0 \xa2	; CYRILLIC SMALL LETTER SHORT U
\0 \x9f	; CYRILLIC SMALL LETTER DZHE
END
