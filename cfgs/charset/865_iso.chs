;
; This file is a charset conversion module in text form.
;
; This module Converts IBM codepage 865 to ISO 8859-1 characters.
;
; Format: ID, version, level,
;         from charset, to charset,
;         128 entries: first & second byte
;	  "END"
; Lines beginning with a ";" or a ";" after the entries are comments
;
; Unkown characters are mapped to the "?" character.
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
CP865		; from set
LATIN-1		; to set
;
\0 \xc7		; C with cedilla
\0 \xfc         ; u dieresis
\0 \xe9		; e acute
\0 \xe2		; a circumflex
\0 \xe4		; a dieresis
\0 \xe0		; a grave
\0 \xe5		; a ring
\0 \xe7		; c cedilla
\0 \xea		; e circumflex
\0 \xeb		; e dieresis
\0 \xe8		; e grave
\0 \xef		; i dieresis
\0 \xee		; i circumflex
\0 \xec		; i grave
\0 \xc4		; A dieresis
\0 \xc5		; A ring
\0 \xc9		; E acute
\0 \xe6		; ae
\0 \xc6		; AE
\0 \xf4		; o circumflex
\0 \xf6		; o dieresis
\0 \xf3		; o acute
\0 \xdb		; u circumflex
\0 \xd9		; u grave
\0 \xff		; y dieresis
\0 \xd6		; O dieresis
\0 \xdc		; U dieresis
\0 \xF8         ; 155 9B latin small letter o with stroke
\0 \xa3		; pound sterling
\0 \xD8         ; 157 9D latin capital letter o with stroke
P t		; Pt
f l		; florin
\0 \xe1		; a acute
\0 \xcc		; i grave
\0 \xf2		; o grave
\0 \xf9		; u grave
\0 \xf1		; n tilde
\0 \xd1		; N tilde
\0 \xaa		; ord feminine
\0 \xba		; ord masculine
\0 \xbf		; question downwards
\0 -		; 
\0 \xac		; logical not
\0 \xbd		; half fraction
\0 \xbc		; quarter fraction
\0 \xa1		; exclam downwards
\0 \xab		; guillemot left
\0 \xbb		; guillemot right
\0 #		;
\0 #		;
\0 #		;
\0 |		;
\0 |		;
\0 |		;
\0 |		;
\0 +		;
\0 +		;
\0 |		;
\0 |		;
\0 +		;
\0 +		;
\0 +		;
\0 +		;
\0 +		;
\0 +		;
\0 -		;
\0 -		;
\0 |		;
\0 -		;
\0 +		;
\0 |		;
\0 |		;
\0 +		;
\0 +		;
\0 =		;
\0 =		;
\0 |		;
\0 =		;
\0 +		;
\0 =		;
\0 -		;
\0 =		;
\0 -		;
\0 +		;
\0 +		;
\0 +		;
\0 +		;
\0 |		;
\0 +		;
\0 +		;
\0 +		;
\0 #		;
\0 n		;
\0 |		;
\0 |		;
\0 ~		;
\0 a		; alpha
\0 \xdf		; german double s (misused as Beta)
\x1 ?		; Gamma
p i		; pi
\x1 ?		; Sigma (summation)
\x1 ?		; sigma
\0 \xb5		; mu
\x1 ?		; gamma
\x1 ?		; Phi
\x1 ?		; Theta
\x1 ?		; Omega
\0 \xf0		; delta
\x1 ?		; infinity
\0 \xf8		; o slash
\x1 ?		; element
\x1 ?		; intersection
= =		; equivalence
\0 \xb1		; plusminus
> =		; greater equals
< =		; smaller equals
\x1 ?		; integral top
\x1 ?		; integral bottom
\0 \xf7		; divide
~ =		; approx.
\0 \xb0		; ring / degree
\0 .		; centered dot
\0 -		; en dash
\x1 ?		; radical
^ n		; to the n'th power
\0 \xb2		; to the second power
\0 \xb7		; bullet
\0 \xa0		; non-breaking space
END

