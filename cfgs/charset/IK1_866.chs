; $Id$
;
; This file is a charset conversion module in text form.
;
; This module converts russian double coded text (ISO8859-5+KOI8R) to codepage 866.
;
; (c) Eugene Palenock (2:5020/2065.468@FidoNet)
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
;
ISO8859-5+KOI8R
CP866
;
\0 ·
\0 ‚
\0 ?
\0 Á
\0 ‰
\0 Â
\0 ?
\0 ?
\0 È
\0 Í
\0 Î
\0 Ï
\0 Ì
\0 Ó
\0 Ô
\0 ?
\0 ?
\0 ?
\0 ?
\0 ?
\0 Ê
\0 Ë
\0 „
\0 ?
\0 ?
\0 ?
\0 ?
\0 ?
\0 ?
\0 ?
\0 ‡
\0 ?
\0 ë
\0 í
\0 ß
\0 ó
\0 î
\0 ï
\0 ¶
\0 ™
\0 ô
\0 ö
\0 õ
\0 ú
\0 ù
\0 û
\0 ü
\0 †
\0 ?
\0 ?
\0 ?
\0 ?
\0 ?
\0 ?
\0 ?
\0 ?
\0 ?
\0 ?
\0 ?
\0 ?
\0 ?
\0 ?
\0 ?
\0 ?
\0 ?
\0 ?
\0 ?
\0 ?
\0 ?
\0 ?
\0 ?
\0 ?
\0 
\0 ?
\0 ?
\0 è
\0 ?
\0 ?
\0 ?
\0 ?
\0 â
\0 ?
\0 ?
\0 Ä
\0 ?
\0 ?
\0 ?
\0 ?
\0 Ò
\0 ¸
\0 ?
\0 ?
\0 ?
\0 ?
\0 ?
\0 ?
\0 ¢
\0 £
\0 §
\0 •
\0 ñ
\0 ò
\0 ì
\0 Æ
\0 ´
\0 ≠
\0 Ø
\0 ©
\0 ®
\0 ¨
\0 ê
\0 °
\0 É
\0 ?
\0 Ç
\0 Å
\0 Ñ
\0 Ö
\0 Ü
\0 á
\0 à
\0 ?
\0 ä
\0 ã
\0 å
\0 H
\0 é
\0 ?
END