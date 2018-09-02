; $Id$
; This file is a charset conversion module for Golded+
; Source charset is KOI-7 (KOI7), 7-bit russian charset (unixes)
; Destination charset is KOI-8 (KOI8), 8-bit russian charset (unixes)
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
1
KOI-7	; from charset
KOI8-R	; to charset
;
\0 \x0	; <control>
\0 \x1	; <control>
\0 \x2	; <control>
\0 \x3	; <control>
\0 \x4	; <control>
\0 \x5	; <control>
\0 \x6	; <control>
\0 \x7	; <control>
\0 \x8	; <control>
\0 \x9	; <control>
\0 \xa	; <control>
\0 \xb	; <control>
\0 \xc	; <control>
\0 \xd	; <control>
\0 \xe	; <control>
\0 \xf	; <control>
\0 \x10	; <control>
\0 \x11	; <control>
\0 \x12	; <control>
\0 \x13	; <control>
\0 \x14	; <control>
\0 \x15	; <control>
\0 \x16	; <control>
\0 \x17	; <control>
\0 \x18	; <control>
\0 \x19	; <control>
\0 \x1a	; <control>
\0 \x1b	; <control>
\0 \x1c	; <control>
\0 \x1d	; <control>
\0 \x1e	; <control>
\0 \x1f	; <control>
\0 \x20	; SPACE
\0 \x21	; EXCLAMATION MARK
\0 \x22	; QUOTATION MARK
\0 \x23	; NUMBER SIGN
\0 \x24	; (CURRENCY SIGN -> DOLLAR SIGN)
\0 \x25	; PERCENT SIGN
\0 \x26	; AMPERSAND
\0 \x27	; APOSTROPHE
\0 \x28	; LEFT PARENTHESIS
\0 \x29	; RIGHT PARENTHESIS
\0 \x2a	; ASTERISK
\0 \x2b	; PLUS SIGN
\0 \x2c	; COMMA
\0 \x2d	; HYPHEN-MINUS
\0 \x2e	; FULL STOP
\0 \x2f	; SOLIDUS
\0 \x30	; DIGIT ZERO
\0 \x31	; DIGIT ONE
\0 \x32	; DIGIT TWO
\0 \x33	; DIGIT THREE
\0 \x34	; DIGIT FOUR
\0 \x35	; DIGIT FIVE
\0 \x36	; DIGIT SIX
\0 \x37	; DIGIT SEVEN
\0 \x38	; DIGIT EIGHT
\0 \x39	; DIGIT NINE
\0 \x3a	; COLON
\0 \x3b	; SEMICOLON
\0 \x3c	; LESS-THAN SIGN
\0 \x3d	; EQUALS SIGN
\0 \x3e	; GREATER-THAN SIGN
\0 \x3f	; QUESTION MARK
\0 \x40	; COMMERCIAL AT
\0 \x41	; LATIN CAPITAL LETTER A
\0 \x42	; LATIN CAPITAL LETTER B
\0 \x43	; LATIN CAPITAL LETTER C
\0 \x44	; LATIN CAPITAL LETTER D
\0 \x45	; LATIN CAPITAL LETTER E
\0 \x46	; LATIN CAPITAL LETTER F
\0 \x47	; LATIN CAPITAL LETTER G
\0 \x48	; LATIN CAPITAL LETTER H
\0 \x49	; LATIN CAPITAL LETTER I
\0 \x4a	; LATIN CAPITAL LETTER J
\0 \x4b	; LATIN CAPITAL LETTER K
\0 \x4c	; LATIN CAPITAL LETTER L
\0 \x4d	; LATIN CAPITAL LETTER M
\0 \x4e	; LATIN CAPITAL LETTER N
\0 \x4f	; LATIN CAPITAL LETTER O
\0 \x50	; LATIN CAPITAL LETTER P
\0 \x51	; LATIN CAPITAL LETTER Q
\0 \x52	; LATIN CAPITAL LETTER R
\0 \x53	; LATIN CAPITAL LETTER S
\0 \x54	; LATIN CAPITAL LETTER T
\0 \x55	; LATIN CAPITAL LETTER U
\0 \x56	; LATIN CAPITAL LETTER V
\0 \x57	; LATIN CAPITAL LETTER W
\0 \x58	; LATIN CAPITAL LETTER X
\0 \x59	; LATIN CAPITAL LETTER Y
\0 \x5a	; LATIN CAPITAL LETTER Z
\0 \x5b	; LEFT SQUARE BRACKET
\0 \x5c	; REVERSE SOLIDUS
\0 \x5d	; RIGHT SQUARE BRACKET
\0 \x5e	; CIRCUMFLEX ACCENT
\0 \x5f	; LOW LINE
\0 \xe0	; CYRILLIC CAPITAL LETTER YU
\0 \xe1	; CYRILLIC CAPITAL LETTER A
\0 \xe2	; CYRILLIC CAPITAL LETTER BE
\0 \xe3	; CYRILLIC CAPITAL LETTER TSE
\0 \xe4	; CYRILLIC CAPITAL LETTER DE
\0 \xe5	; CYRILLIC CAPITAL LETTER IE
\0 \xe6	; CYRILLIC CAPITAL LETTER EF
\0 \xe7	; CYRILLIC CAPITAL LETTER GHE
\0 \xe8	; CYRILLIC CAPITAL LETTER HA
\0 \xe9	; CYRILLIC CAPITAL LETTER I
\0 \xea	; CYRILLIC CAPITAL LETTER SHORT I
\0 \xeb	; CYRILLIC CAPITAL LETTER KA
\0 \xec	; CYRILLIC CAPITAL LETTER EL
\0 \xed	; CYRILLIC CAPITAL LETTER EM
\0 \xee	; CYRILLIC CAPITAL LETTER EN
\0 \xef	; CYRILLIC CAPITAL LETTER O
\0 \xf0	; CYRILLIC CAPITAL LETTER PE
\0 \xf1	; CYRILLIC CAPITAL LETTER YA
\0 \xf2	; CYRILLIC CAPITAL LETTER ER
\0 \xf3	; CYRILLIC CAPITAL LETTER ES
\0 \xf4	; CYRILLIC CAPITAL LETTER TE
\0 \xf5	; CYRILLIC CAPITAL LETTER U
\0 \xf6	; CYRILLIC CAPITAL LETTER ZHE
\0 \xf7	; CYRILLIC CAPITAL LETTER VE
\0 \xf8	; CYRILLIC CAPITAL LETTER SOFT SIGN
\0 \xf9	; CYRILLIC CAPITAL LETTER YERU
\0 \xfa	; CYRILLIC CAPITAL LETTER ZE
\0 \xfb	; CYRILLIC CAPITAL LETTER SHA
\0 \xfc	; CYRILLIC CAPITAL LETTER E
\0 \xfd	; CYRILLIC CAPITAL LETTER SHCHA
\0 \xfe	; CYRILLIC CAPITAL LETTER CHE
\0 \x7f	; <control>
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
END
