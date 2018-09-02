; $Id$
; This file is a charset conversion module for Golded+
; Source charset is KOI-7 (KOI7), 7-bit russian charset (unixes)
; Destination charset is CP866 (MS OEM), 8-bit russian charset (DOS)
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
CP866	; to charset
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
\0 \xfd	; CURRENCY SIGN
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
\0 \x9e	; CYRILLIC CAPITAL LETTER YU
\0 \x80	; CYRILLIC CAPITAL LETTER A
\0 \x81	; CYRILLIC CAPITAL LETTER BE
\0 \x96	; CYRILLIC CAPITAL LETTER TSE
\0 \x84	; CYRILLIC CAPITAL LETTER DE
\0 \x85	; CYRILLIC CAPITAL LETTER IE
\0 \x94	; CYRILLIC CAPITAL LETTER EF
\0 \x83	; CYRILLIC CAPITAL LETTER GHE
\0 \x95	; CYRILLIC CAPITAL LETTER HA
\0 \x88	; CYRILLIC CAPITAL LETTER I
\0 \x89	; CYRILLIC CAPITAL LETTER SHORT I
\0 \x8a	; CYRILLIC CAPITAL LETTER KA
\0 \x8b	; CYRILLIC CAPITAL LETTER EL
\0 \x8c	; CYRILLIC CAPITAL LETTER EM
\0 \x8d	; CYRILLIC CAPITAL LETTER EN
\0 \x8e	; CYRILLIC CAPITAL LETTER O
\0 \x8f	; CYRILLIC CAPITAL LETTER PE
\0 \x9f	; CYRILLIC CAPITAL LETTER YA
\0 \x90	; CYRILLIC CAPITAL LETTER ER
\0 \x91	; CYRILLIC CAPITAL LETTER ES
\0 \x92	; CYRILLIC CAPITAL LETTER TE
\0 \x93	; CYRILLIC CAPITAL LETTER U
\0 \x86	; CYRILLIC CAPITAL LETTER ZHE
\0 \x82	; CYRILLIC CAPITAL LETTER VE
\0 \x9c	; CYRILLIC CAPITAL LETTER SOFT SIGN
\0 \x9b	; CYRILLIC CAPITAL LETTER YERU
\0 \x87	; CYRILLIC CAPITAL LETTER ZE
\0 \x98	; CYRILLIC CAPITAL LETTER SHA
\0 \x9d	; CYRILLIC CAPITAL LETTER E
\0 \x99	; CYRILLIC CAPITAL LETTER SHCHA
\0 \x97	; CYRILLIC CAPITAL LETTER CHE
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
