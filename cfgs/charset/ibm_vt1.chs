;
; This file is a charset conversion module in text form.
;
; This module Converts IBM extended characters to VT100.
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
IBMPC		; from set
VT100		; to set
;
\0 C		; C with cedilla
u e		; u dieresis
\0 e		; e acute
\0 a		; a circumflex
a e		; a dieresis
\0 a		; a grave
\0 a		; a ring
\0 c		; c cedilla
\0 e		; e circumflex
e e		; e dieresis
\0 e		; e grave
i e		; i dieresis
\0 i		; i circumflex
\0 i		; i grave
A e		; A dieresis
\0 A		; A ring
\0 E		; E acute
a e		; ae
A E		; AE
\0 o		; o circumflex
o e		; o dieresis
\0 o		; o acute
\0 u		; u circumflex
\0 u		; u grave
y e		; y dieresis
O e		; O dieresis
U e		; U dieresis
\0 c		; cent
\d125 \0	; pound sterling
\0 Y		; yen
P t		; Pt
\0 f		; florin
\0 a		; a acute
\0 i		; i grave
\0 o		; o grave
\0 u		; u grave
\0 n		; n tilde
\0 N		; N tilde
\0 a		; ord feminine
\0 o		; ord masculine
\0 ?		; question downwards
\0 -		; 
\0 !		; logical not
. 5		; half fraction
\x1 ?		; quarter fraction
\0 !		; exclam downwards
< <		; guillemot left
> >		; guillemot right
\0 #		; grey
\d97 \0		; grey
\d97 \0		; grey
\d120 \0	;  |
\d117 \0	; -|
\d117 \0	; -|
\d117 \0	; -|
\d107 \0	;  +
\d107 \0	;  +
\d117 \0	; -|
\d120 \0	;  |
\d107 \0	;  +
\d106 \0	;  +
\d106 \0	;  +
\d106 \0	;  +
\d107 \0	;  +
\d109 \0	;  +
\d118 \0	;  -
\d119 \0	;  -
\d116 \0	;  |-
\d113 \0	;  -
\d110 \0	; -|-
\d116 \0	;  |-
\d116 \0	;  |-
\d109 \0	;  +
\d108 \0	;  +
\d118 \0	;  -
\d119 \0	;  -
\d116 \0	;  |-
\d113 \0	;  -
\d110 \0	; -|-
\d118 \0	;  -
\d118 \0	;  -
\d119 \0	;  -
\d119 \0	;  -
\d109 \0	;  +
\d109 \0	;  +
\d108 \0	;  +
\d108 \0	;  +
\d110 \0	; -|-
\d110 \0	; -|-
\d106 \0	;  +
\d108 \0	;  +
\0 #		;
\0 n		;
\0 |		;
\0 |		;
\0 ~		;
\0 a		; alpha
\0 B		; german double s (misused as Beta)
\x1 ?		; Gamma
\d123 \0	; pi
\x1 ?		; Sigma (summation)
\x1 ?		; sigma
m u		; mu
\x1 ?		; gamma
\x1 ?		; Phi
\x1 ?		; Theta
\0 O		; Omega
\0 d		; delta
\x1 ?		; infinity
\0 o		; o slash
\x1 ?		; element
\x1 ?		; intersection
= =		; equivalence
\d103 \0	; plusminus
\d122 \0	; greater equals
\d121 \0	; smaller equals
\x1 ?		; integral top
\x1 ?		; integral bottom
\0 /		; divide
~ =		; approx.
\d102 \0	; ring / degree
\d126 \0	; centered dot
\0 -		; en dash
\x1 ?		; radical
^ n		; to the n'th power
^ 2		; to the second power
\0 o		;
\0 \d32		; space
END
