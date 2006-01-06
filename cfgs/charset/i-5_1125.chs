; $Id$
; This file is a charset conversion module for Golded+
; Source charset is ISO8859-5, 8-bit russian charset
; Destination charset is CP1125 (UKR), 8-bit Ukrainian charset (DOS)
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
cp1125	; to charset
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
\0 \xff	; NO-BREAK SPACE
\0 \xf0	; CYRILLIC CAPITAL LETTER IO
\0 ?	; (CYRILLIC CAPITAL LETTER DJE)
\0 ?	; (CYRILLIC CAPITAL LETTER GJE)
\0 \xf4	; CYRILLIC CAPITAL LETTER UKRAINIAN IE
\0 ?	; (CYRILLIC CAPITAL LETTER DZE)
\0 \xf6	; CYRILLIC CAPITAL LETTER BYELORUSSIAN-UKRAINIAN I
\0 \xf8	; CYRILLIC CAPITAL LETTER YI
\0 \x50	; LATIN CAPITAL LETTER P
\x8b \xec	; CYRILLIC CAPITAL LETTER EL + CYRILLIC SMALL LETTER SOFT SIGN
\x8d \xec	; CYRILLIC CAPITAL LETTER EN + CYRILLIC SMALL LETTER SOFT SIGN
\0 ?	; (CYRILLIC CAPITAL LETTER TSHE)
\0 ?	; (CYRILLIC CAPITAL LETTER KJE)
\0 ?	; (SOFT HYPHEN)
\0 ?	; (CYRILLIC CAPITAL LETTER SHORT U)
\0 ?	; (CYRILLIC CAPITAL LETTER DZHE)
\0 \x80	; CYRILLIC CAPITAL LETTER A
\0 \x81	; CYRILLIC CAPITAL LETTER BE
\0 \x82	; CYRILLIC CAPITAL LETTER VE
\0 \x83	; CYRILLIC CAPITAL LETTER GHE
\0 \x84	; CYRILLIC CAPITAL LETTER DE
\0 \x85	; CYRILLIC CAPITAL LETTER IE
\0 \x86	; CYRILLIC CAPITAL LETTER ZHE
\0 \x87	; CYRILLIC CAPITAL LETTER ZE
\0 \x88	; CYRILLIC CAPITAL LETTER I
\0 \x89	; CYRILLIC CAPITAL LETTER SHORT I
\0 \x8a	; CYRILLIC CAPITAL LETTER KA
\0 \x8b	; CYRILLIC CAPITAL LETTER EL
\0 \x8c	; CYRILLIC CAPITAL LETTER EM
\0 \x8d	; CYRILLIC CAPITAL LETTER EN
\0 \x8e	; CYRILLIC CAPITAL LETTER O
\0 \x8f	; CYRILLIC CAPITAL LETTER PE
\0 \x90	; CYRILLIC CAPITAL LETTER ER
\0 \x91	; CYRILLIC CAPITAL LETTER ES
\0 \x92	; CYRILLIC CAPITAL LETTER TE
\0 \x93	; CYRILLIC CAPITAL LETTER U
\0 \x94	; CYRILLIC CAPITAL LETTER EF
\0 \x95	; CYRILLIC CAPITAL LETTER HA
\0 \x96	; CYRILLIC CAPITAL LETTER TSE
\0 \x97	; CYRILLIC CAPITAL LETTER CHE
\0 \x98	; CYRILLIC CAPITAL LETTER SHA
\0 \x99	; CYRILLIC CAPITAL LETTER SHCHA
\0 \x9a	; CYRILLIC CAPITAL LETTER HARD SIGN
\0 \x9b	; CYRILLIC CAPITAL LETTER YERU
\0 \x9c	; CYRILLIC CAPITAL LETTER SOFT SIGN
\0 \x9d	; CYRILLIC CAPITAL LETTER E
\0 \x9e	; CYRILLIC CAPITAL LETTER YU
\0 \x9f	; CYRILLIC CAPITAL LETTER YA
\0 \xa0	; CYRILLIC SMALL LETTER A
\0 \xa1	; CYRILLIC SMALL LETTER BE
\0 \xa2	; CYRILLIC SMALL LETTER VE
\0 \xa3	; CYRILLIC SMALL LETTER GHE
\0 \xa4	; CYRILLIC SMALL LETTER DE
\0 \xa5	; CYRILLIC SMALL LETTER IE
\0 \xa6	; CYRILLIC SMALL LETTER ZHE
\0 \xa7	; CYRILLIC SMALL LETTER ZE
\0 \xa8	; CYRILLIC SMALL LETTER I
\0 \xa9	; CYRILLIC SMALL LETTER SHORT I
\0 \xaa	; CYRILLIC SMALL LETTER KA
\0 \xab	; CYRILLIC SMALL LETTER EL
\0 \xac	; CYRILLIC SMALL LETTER EM
\0 \xad	; CYRILLIC SMALL LETTER EN
\0 \xae	; CYRILLIC SMALL LETTER O
\0 \xaf	; CYRILLIC SMALL LETTER PE
\0 \xe0	; CYRILLIC SMALL LETTER ER
\0 \xe1	; CYRILLIC SMALL LETTER ES
\0 \xe2	; CYRILLIC SMALL LETTER TE
\0 \xe3	; CYRILLIC SMALL LETTER U
\0 \xe4	; CYRILLIC SMALL LETTER EF
\0 \xe5	; CYRILLIC SMALL LETTER HA
\0 \xe6	; CYRILLIC SMALL LETTER TSE
\0 \xe7	; CYRILLIC SMALL LETTER CHE
\0 \xe8	; CYRILLIC SMALL LETTER SHA
\0 \xe9	; CYRILLIC SMALL LETTER SHCHA
\0 \xea	; CYRILLIC SMALL LETTER HARD SIGN
\0 \xeb	; CYRILLIC SMALL LETTER YERU
\0 \xec	; CYRILLIC SMALL LETTER SOFT SIGN
\0 \xed	; CYRILLIC SMALL LETTER E
\0 \xee	; CYRILLIC SMALL LETTER YU
\0 \xef	; CYRILLIC SMALL LETTER YA
\x4e \x6f	; LATIN CAPITAL LETTER N + LATIN SMALL LETTER O
\0 \xf1	; CYRILLIC SMALL LETTER IO
\0 ?	; (CYRILLIC SMALL LETTER DJE)
\0 ?	; (CYRILLIC SMALL LETTER GJE)
\0 \xf5	; CYRILLIC SMALL LETTER UKRAINIAN IE
\0 \x73	; LATIN SMALL LETTER S
\0 \xf7	; CYRILLIC SMALL LETTER BYELORUSSIAN-UKRAINIAN I
\0 \xf9	; CYRILLIC SMALL LETTER YI
\0 \x6a	; LATIN SMALL LETTER J
\xab \xec	; CYRILLIC SMALL LETTER EL + CYRILLIC SMALL LETTER SOFT SIGN
\xad \xec	; CYRILLIC SMALL LETTER EN + CYRILLIC SMALL LETTER SOFT SIGN
\0 ?	; (CYRILLIC SMALL LETTER TSHE)
\0 ?	; (CYRILLIC SMALL LETTER KJE)
\0 ?	; (SECTION SIGN)
\0 ?	; (CYRILLIC SMALL LETTER SHORT U)
\0 ?	; (CYRILLIC SMALL LETTER DZHE)
END
