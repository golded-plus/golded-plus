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
\0 �
\0 �
\0 ?
\0 �
\0 �
\0 �
\0 ?
\0 ?
\0 �
\0 �
\0 �
\0 �
\0 �
\0 �
\0 �
\0 ?
\0 ?
\0 ?
\0 ?
\0 ?
\0 �
\0 �
\0 �
\0 ?
\0 ?
\0 ?
\0 ?
\0 ?
\0 ?
\0 ?
\0 �
\0 ?
\0 �
\0 �
\0 �
\0 �
\0 �
\0 �
\0 �
\0 �
\0 �
\0 �
\0 �
\0 �
\0 �
\0 �
\0 �
\0 �
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
\0 �
\0 ?
\0 ?
\0 �
\0 ?
\0 ?
\0 ?
\0 ?
\0 �
\0 ?
\0 ?
\0 �
\0 ?
\0 ?
\0 ?
\0 ?
\0 �
\0 �
\0 ?
\0 ?
\0 ?
\0 ?
\0 ?
\0 ?
\0 �
\0 �
\0 �
\0 �
\0 �
\0 �
\0 �
\0 �
\0 �
\0 �
\0 �
\0 �
\0 �
\0 �
\0 �
\0 �
\0 �
\0 ?
\0 �
\0 �
\0 �
\0 �
\0 �
\0 �
\0 �
\0 ?
\0 �
\0 �
\0 �
\0 H
\0 �
\0 ?
END