;
; This file is a charset conversion module in text form.
;
; This module Converts Macintosh extended characters to VT100.
;
; NOTE: this file uses an implementation dependant feature!
;       a 0 as the second character means "use DEC graphic set"
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
VT100		; to set (change to UK for UK map file)
;
A e		; A dieresis
\0 A		; A ring
\0 C		; C cedilla
\0 E		; E acute
\0 N		; N tilde
O e		; O dieresis
U e		; U dieresis
\0 a		; a acute
\0 a		; a grave
\0 a		; a circumflex
a e		; a dieresis
\0 a		; a tilde
\0 a		; a ring
\0 c		; c cedilla
\0 e		; e acute
\0 e		; e grave
\0 e		; e circumflex
e e		; e dieresis
\0 i		; i acute
\0 i		; i grave
\0 i		; i circumflex
i e		; i dieresis
\0 n		; n tilde
\0 o		; o acute
\0 o		; o grave
\0 o		; o circumflex
o e		; o dieresis
\0 o		; o tilde
\0 u		; u grave
\0 u		; u acute
\0 u		; u circumflex
u e		; u dieresis
\0 +		; dagger
\d102 \0	; ring / degree
\0 c		; cent
\d125 \0	; pound sterling
\0 S		; section
\0 o		; bullet
\x1 ?		; paragraph
\0 B		; german double s
\0 R		; registered trademark
\0 c		; copyright
T M		; trademark
\0 '		; acute
\x1 ?		; dieresis
\d124 \0	; not equal
A E		; AE
\0 0		; O slash
\x1 ?		; infinity
\d103 \0	; plusminus
\d121 \0	; smaller equals
\d122 \0	; greater equals
\0 Y		; Yen
m u		; mu
\0 d		; delta
\x1 ?		; Sigma (summation)
\x1 ?		; Pi
\d123 \0	; pi
\x1 ?		; integral
\0 a		; ord feminine
\0 o		; ord masculine
\0 O		; Omega
a e		; ae
\0 o		; o slash
\0 ?		; question downwards
\0 !		; exclam downwards
\0 !		; logical not
\x1 ?		; radical
\0 f		; florin
~ =		; approx.
\x1 ?		; Delta
> >		; guillemot right
< <		; guillemot left
. .		; ellipsis
\0 \d32		; non breaking space
\0 A		; A acute
\0 A		; A tilde
\0 O		; O tilde
O E		; OE
o e		; oe
\0 -		; en dash
\0 -		; em dash
\0 "		; double quote left
\0 "		; double quote right
\0 `		; quote left
\0 '		; quote right
\0 /		; divide
\d96 \0		; lozenge
y e		; y dieresis
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
