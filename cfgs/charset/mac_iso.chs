;
; This file is a charset conversion module in text form.
;
; This module Converts Macintosh extended characters to ISO 8859-1.
;
; Format: ID, version, level,
;         from charset, to charset,
;         128 entries: first & second byte
;	  "END"
; Lines beginning with a ";" or a ";" after the entries are comments
;
; Unknown characters are mapped to the "?" character.
;
; cedilla = ,   ; dieresis = ..       ; acute = '
; grave = `     ; circumflex = ^      ; ring = o
; tilde = ~     ; caron = v 
; All of these are above the character, apart from the cedilla which is below.
;
; \ is the escape character: \0 means decimal zero,
; \dnnn where nnn is a decimal number is the ordinal value of the character
; \xnn where nn is a hexadecimal number
; e.g.: \d32 is the ASCII space character
; Two \\ is the character "\" itself.
;
0		; ID number
0		; version number
;
2		; level number
;
MAC		; from set
LATIN-1		; to set
;
\0 \xc4		; A dieresis
\0 \xc5		; A ring
\0 \xc7		; C cedilla
\0 \xc9		; E acute
\0 \xd1		; N tilde
\0 \xd6		; O dieresis
\0 \xdc		; U dieresis
\0 \xe1		; a acute
\0 \xe0		; a grave
\0 \xe2		; a circumflex
\0 \xe4		; a dieresis
\0 \xe3		; a tilde
\0 \xe5		; a ring
\0 \xe7		; c cedilla
\0 \xe9		; e acute
\0 \xe8		; e grave
\0 \xea		; e circumflex
\0 \xeb		; e dieresis
\0 \xed		; i acute
\0 \xec		; i grave
\0 \xee		; i circumflex
\0 \xef		; i dieresis
\0 \xf1		; n tilde
\0 \xf3		; o acute
\0 \xf2		; o grave
\0 \xf4		; o circumflex
\0 \xf6		; o dieresis
\0 \xf5		; o tilde
\0 \xfa		; u acute
\0 \xf9		; u grave
\0 \xfb		; u circumflex
\0 \xfc		; u dieresis
\0 +		; dagger
\0 \xb0		; ring / degree
\0 \xa2		; cent
\0 \xa3		; pound sterling
\0 \xa7		; section
\0 \xb7		; bullet
\0 \xb6		; paragraph
\0 \xdf		; german double s
\0 \xae		; registered trademark
\0 \xa9		; copyright
T M		; trademark
\0 \xb4		; acute
\0 \xa8		; dieresis
< >		; not equal
\0 \xc6		; AE
\0 \xd8		; O slash
\x1 ?		; infinity
\0 \xb1		; plusminus
< =		; smaller equals
> =		; greater equals
\0 \xa5		; Yen
\0 \xb5		; mu
\0 \xf0		; delta
\x1 ?		; Sigma (summation)
\x1 ?		; Pi
p i		; pi
\x1 ?		; integral
\0 \xaa		; ord feminine
\0 \xba		; ord masculine
\x1 ?		; Omega
\0 \xe6		; ae
\0 \xf8		; o slash
\0 \xbf		; question downwards
\0 \xa1		; exclam downwards
\0 \xac		; logical not
\x1 ?		; radical
f l		; florin
~ =		; approx.
\x1 ?		; Delta
\0 \xbb		; guillemot right
\0 \xab		; guillemot left
. .		; ellipsis
\0 \xa0		; non breaking space
\0 \xc1		; A acute
\0 \xc3		; A tilde
\0 \xd5		; O tilde
O E		; OE
o e		; oe
\0 -		; en dash
\0 \xad		; em dash
\0 "		; double quote left
\0 "		; double quote right
\0 `		; quote left
\0 \xb4		; quote right
\0 \xf7		; divide
\0 o		; lozenge
\0 \xff		; y dieresis
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
\0 ?		;
END
