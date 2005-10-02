; $Id$
;
; This file is a charset conversion module in text form.
;
; This module converts russian codepage 866 itself.
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
0      ; ID number
0      ; version number
;
2      ; level number
;
CP866  ; from charset
CP866  ; to charset
;
\0 €
\0 
\0 ‚
\0 ƒ
\0 „
\0 …
\0 †
\0 ‡
\0 ˆ
\0 ‰
\0 Š
\0 ‹
\0 Œ
\0 H
\0 
\0 
\0 
\0 ‘
\0 ’
\0 “
\0 ”
\0 •
\0 –
\0 —
\0 ˜
\0 ™
\0 š
\0 ›
\0 œ
\0 
\0 
\0 Ÿ
\0  
\0 ¡
\0 ¢
\0 £
\0 ¤
\0 ¥
\0 ¦
\0 §
\0 ¨
\0 ©
\0 ª
\0 «
\0 ¬
\0 ­
\0 ®
\0 ¯
\0 °
\0 ±
\0 ²
\0 ³
\0 ´
\0 µ
\0 ¶
\0 ·
\0 ¸
\0 ¹
\0 º
\0 »
\0 ¼
\0 ½
\0 ¾
\0 ¿
\0 À
\0 Á
\0 Â
\0 Ã
\0 Ä
\0 Å
\0 Æ
\0 Ç
\0 È
\0 É
\0 Ê
\0 Ë
\0 Ì
\0 Í
\0 Î
\0 Ï
\0 Ğ
\0 Ñ
\0 Ò
\0 Ó
\0 Ô
\0 Õ
\0 Ö
\0 ×
\0 Ø
\0 Ù
\0 Ú
\0 Û
\0 Ü
\0 İ
\0 Ş
\0 ß
\0 à
\0 á
\0 â
\0 ã
\0 ä
\0 å
\0 æ
\0 ç
\0 è
\0 é
\0 ê
\0 ë
\0 ì
\0 í
\0 î
\0 ï
\0 ğ
\0 ñ
\0 ò
\0 ó
\0 ô
\0 õ
\0 ö
\0 ÷
\0 ø
\0 ù
\0 ú
\0 û
\0 ü
\0 ı
\0 ş
\0 ÿ
END
