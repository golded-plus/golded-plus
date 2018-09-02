; $Id$
;
; This file is a charset conversion module in text form.
;
; This module converts russian double coded text (ISO8859-5+KOI8R+H) to codepage 866.
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
100000
0
;
2
;
ISO-5+KOI8R+H
CP866
;
\0 \d0
\0 \d1
\0 \d2
\0 \d3
\0 \d4
\0 \d5
\0 \d6
\0 \d7
\0 \d8
\0 \d9
\0 \d10
\0 \d11
\0 \d12
\0 \d13
\0 \d14
\0 \d15
\0 \d16
\0 \d17
\0 \d18
\0 \d19
\0 \d20
\0 \d21
\0 \d22
\0 \d23
\0 \d24
\0 \d25
\0 \d26
\0 \d27
\0 \d28
\0 \d29
\0 \d30
\0 \d31
\0 \d32
\0 \d33
\0 \d34
\0 \d35
\0 \d36
\0 \d37
\0 \d38
\0 \d39
\0 \d40
\0 \d41
\0 \d42
\0 \d43
\0 \d44
\0 \d45
\0 \d46
\0 \d47
\0 \d48
\0 \d49
\0 \d50
\0 \d51
\0 \d52
\0 \d53
\0 \d54
\0 \d55
\0 \d56
\0 \d57
\0 \d58
\0 \d59
\0 \d60
\0 \d61
\0 \d62
\0 \d63
\0 \d64
\0 \d65
\0 \d66
\0 \d67
\0 \d68
\0 \d69
\0 \d70
\0 \d71
\0 Ó
\0 \d73
\0 \d74
\0 \d75
\0 \d76
\0 \d77
\0 \d78
\0 \d79
\0 \d80
\0 \d81
\0 \d82
\0 \d83
\0 \d84
\0 \d85
\0 \d86
\0 \d87
\0 \d88
\0 \d89
\0 \d90
\0 \d91
\0 \d92
\0 \d93
\0 \d94
\0 \d95
\0 \d96
\0 \d97
\0 \d98
\0 \d99
\0 \d100
\0 \d101
\0 \d102
\0 \d103
\0 \d104
\0 \d105
\0 \d106
\0 \d107
\0 \d108
\0 \d109
\0 \d110
\0 \d111
\0 \d112
\0 \d113
\0 \d114
\0 \d115
\0 \d116
\0 \d117
\0 \d118
\0 \d119
\0 \d120
\0 \d121
\0 \d122
\0 \d123
\0 \d124
\0 \d125
\0 \d126
\0 \d127
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