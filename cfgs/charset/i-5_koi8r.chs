; $Id$
; This file is a charset conversion module for Golded+
; Source charset is ISO8859-5, 8-bit russian charset
; Destination charset is CP20866 (KOI8-R), 8-bit russian charset (unixes)
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
koi8-r	; to charset
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
\0 \x9a	; NO-BREAK SPACE
\0 \xb3	; CYRILLIC CAPITAL LETTER IO
\0 ?	; (CYRILLIC CAPITAL LETTER DJE)
\0 ?	; (CYRILLIC CAPITAL LETTER GJE)
\0 ?	; (CYRILLIC CAPITAL LETTER UKRAINIAN IE)
\0 ?	; (CYRILLIC CAPITAL LETTER DZE)
\0 \x49	; LATIN CAPITAL LETTER I
\0 ?	; (CYRILLIC CAPITAL LETTER YI)
\0 \x50	; LATIN CAPITAL LETTER P
\xec \xd8	; CYRILLIC CAPITAL LETTER EL + CYRILLIC SMALL LETTER SOFT SIGN
\xee \xd8	; CYRILLIC CAPITAL LETTER EN + CYRILLIC SMALL LETTER SOFT SIGN
\0 ?	; (CYRILLIC CAPITAL LETTER TSHE)
\0 ?	; (CYRILLIC CAPITAL LETTER KJE)
\0 ?	; (SOFT HYPHEN)
\0 ?	; (CYRILLIC CAPITAL LETTER SHORT U)
\0 ?	; (CYRILLIC CAPITAL LETTER DZHE)
\0 \xe1	; CYRILLIC CAPITAL LETTER A
\0 \xe2	; CYRILLIC CAPITAL LETTER BE
\0 \xf7	; CYRILLIC CAPITAL LETTER VE
\0 \xe7	; CYRILLIC CAPITAL LETTER GHE
\0 \xe4	; CYRILLIC CAPITAL LETTER DE
\0 \xe5	; CYRILLIC CAPITAL LETTER IE
\0 \xf6	; CYRILLIC CAPITAL LETTER ZHE
\0 \xfa	; CYRILLIC CAPITAL LETTER ZE
\0 \xe9	; CYRILLIC CAPITAL LETTER I
\0 \xea	; CYRILLIC CAPITAL LETTER SHORT I
\0 \xeb	; CYRILLIC CAPITAL LETTER KA
\0 \xec	; CYRILLIC CAPITAL LETTER EL
\0 \xed	; CYRILLIC CAPITAL LETTER EM
\0 \xee	; CYRILLIC CAPITAL LETTER EN
\0 \xef	; CYRILLIC CAPITAL LETTER O
\0 \xf0	; CYRILLIC CAPITAL LETTER PE
\0 \xf2	; CYRILLIC CAPITAL LETTER ER
\0 \xf3	; CYRILLIC CAPITAL LETTER ES
\0 \xf4	; CYRILLIC CAPITAL LETTER TE
\0 \xf5	; CYRILLIC CAPITAL LETTER U
\0 \xe6	; CYRILLIC CAPITAL LETTER EF
\0 \xe8	; CYRILLIC CAPITAL LETTER HA
\0 \xe3	; CYRILLIC CAPITAL LETTER TSE
\0 \xfe	; CYRILLIC CAPITAL LETTER CHE
\0 \xfb	; CYRILLIC CAPITAL LETTER SHA
\0 \xfd	; CYRILLIC CAPITAL LETTER SHCHA
\0 \xff	; CYRILLIC CAPITAL LETTER HARD SIGN
\0 \xf9	; CYRILLIC CAPITAL LETTER YERU
\0 \xf8	; CYRILLIC CAPITAL LETTER SOFT SIGN
\0 \xfc	; CYRILLIC CAPITAL LETTER E
\0 \xe0	; CYRILLIC CAPITAL LETTER YU
\0 \xf1	; CYRILLIC CAPITAL LETTER YA
\0 \xc1	; CYRILLIC SMALL LETTER A
\0 \xc2	; CYRILLIC SMALL LETTER BE
\0 \xd7	; CYRILLIC SMALL LETTER VE
\0 \xc7	; CYRILLIC SMALL LETTER GHE
\0 \xc4	; CYRILLIC SMALL LETTER DE
\0 \xc5	; CYRILLIC SMALL LETTER IE
\0 \xd6	; CYRILLIC SMALL LETTER ZHE
\0 \xda	; CYRILLIC SMALL LETTER ZE
\0 \xc9	; CYRILLIC SMALL LETTER I
\0 \xca	; CYRILLIC SMALL LETTER SHORT I
\0 \xcb	; CYRILLIC SMALL LETTER KA
\0 \xcc	; CYRILLIC SMALL LETTER EL
\0 \xcd	; CYRILLIC SMALL LETTER EM
\0 \xce	; CYRILLIC SMALL LETTER EN
\0 \xcf	; CYRILLIC SMALL LETTER O
\0 \xd0	; CYRILLIC SMALL LETTER PE
\0 \xd2	; CYRILLIC SMALL LETTER ER
\0 \xd3	; CYRILLIC SMALL LETTER ES
\0 \xd4	; CYRILLIC SMALL LETTER TE
\0 \xd5	; CYRILLIC SMALL LETTER U
\0 \xc6	; CYRILLIC SMALL LETTER EF
\0 \xc8	; CYRILLIC SMALL LETTER HA
\0 \xc3	; CYRILLIC SMALL LETTER TSE
\0 \xde	; CYRILLIC SMALL LETTER CHE
\0 \xdb	; CYRILLIC SMALL LETTER SHA
\0 \xdd	; CYRILLIC SMALL LETTER SHCHA
\0 \xdf	; CYRILLIC SMALL LETTER HARD SIGN
\0 \xd9	; CYRILLIC SMALL LETTER YERU
\0 \xd8	; CYRILLIC SMALL LETTER SOFT SIGN
\0 \xdc	; CYRILLIC SMALL LETTER E
\0 \xc0	; CYRILLIC SMALL LETTER YU
\0 \xd1	; CYRILLIC SMALL LETTER YA
\x4e \x6f	; LATIN CAPITAL LETTER N + LATIN SMALL LETTER O
\0 \xa3	; CYRILLIC SMALL LETTER IO
\0 ?	; (CYRILLIC SMALL LETTER DJE)
\0 ?	; (CYRILLIC SMALL LETTER GJE)
\0 ?	; (CYRILLIC SMALL LETTER UKRAINIAN IE)
\0 \x73	; LATIN SMALL LETTER S
\0 \x69	; LATIN SMALL LETTER I
\0 ?	; (CYRILLIC SMALL LETTER YI)
\0 \x6a	; LATIN SMALL LETTER J
\xcc \xd8	; CYRILLIC SMALL LETTER EL + CYRILLIC SMALL LETTER SOFT SIGN
\xce \xd8	; CYRILLIC SMALL LETTER EN + CYRILLIC SMALL LETTER SOFT SIGN
\0 ?	; (CYRILLIC SMALL LETTER TSHE)
\0 ?	; (CYRILLIC SMALL LETTER KJE)
\0 ?	; (SECTION SIGN)
\0 ?	; (CYRILLIC SMALL LETTER SHORT U)
\0 ?	; (CYRILLIC SMALL LETTER DZHE)
END
