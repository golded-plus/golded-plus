;
; This file is a charset conversion module in text form.
;
; This module converts ISO_8859-1 extended characters to Macintosh characters.
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
; \dnnn where nnn is a decimal number, is the ordinal value of the character
; \xnn where nn is a hexadecimal number
; e.g.: \d32 is the ASCII space character
; Two \\ is the character "\" itself.
;
0		; ID number
0		; version number
;
2		; level number
;
Latin-1		; from set
MAC		; to set
;
\0 ?		; (missing) these codes are unused
\0 ?		; (missing)
\0 ?		; (missing)
\0 ?		; (missing) 4
\0 ?		; (missing)
\0 ?		; (missing)
\0 ?		; (missing)
\0 ?		; (missing) 8
\0 ?		; (missing)
\0 ?		; (missing)
\0 ?		; (missing)
\0 ?		; (missing) 12
\0 ?		; (missing)
\0 ?		; (missing)
\0 ?		; (missing)
\0 ?		; (missing) 16
\0 ?		; (missing)
\0 ?		; (missing)
\0 ?		; (missing)
\0 ?		; (missing) 20
\0 ?		; (missing)
\0 ?		; (missing)
\0 ?		; (missing)
\0 ?		; (missing) 24
\0 ?		; (missing)
\0 ?		; (missing)
\0 ?		; (missing)
\0 ?		; (missing) 28
\0 ?		; (missing)
\0 ?		; (missing)
\0 ?		; (missing)
\0 ?		; (missing) 32
\0 \xca		; non-breaking space
\0 \xc1		; exclam downwards
\0 \xa2		; cent
\0 \xa3		; pound sterling
\x1 ?		; currency
\0 \xb4		; Yen
\0 |		; broken bar
\0 \xa4		; section
\0 \xac		; dieresis
\0 \xa9		; copyright
\0 \xbb		; ord feminine
\0 \xc7		; guillemot left
\0 \xc2		; logical not
\0 \xd1		; soft hyphen (or em dash)
\0 \xa8		; registered trademark
\x1 ?		; overbar (macron)
\0 \xa1		; ring / degree
\0 \xb1		; plusminus
^ 2		; superscript two (squared)
^ 3		; superscript three (cubed)
\0 \xab		; acute
\0 \xb5		; mu
\0 \xa6		; paragraph
\0 \xa5		; bullet
\0 ,		; cedilla
^ 1		; superscript one
\0 \xbc		; ord masculine
\0 \xc8		; guillemot right
\x1 ?		; one quarter
. 5		; half
\x1 ?		; three quarters
\0 \xc0		; question downwards
\0 A		; A grave
\0 A		; A acute
\0 A		; A circumflex
\0 A		; A tilde
\0 \x80		; A dieresis
\0 \x81		; A ring
\0 \xae		; AE
\0 \x82		; C cedilla
\0 E		; E grave
\0 \x83		; E acute
\0 E		; E circumflex
E e		; E dieresis
\0 I		; I grave
\0 I		; I acute
\0 I		; I circumflex
I e		; I dieresis
\0 D		; Eth
\0 \x84		; N tilde
\0 O		; O grave
\0 O		; O acute
\0 O		; O circumflex
\0 O		; O tilde
\0 \x85		; O dieresis
\0 x		; multiplication
\0 \xaf		; O slash
\0 U		; U grave
\0 U		; U acute
\0 U		; U circumflex
\0 \x86		; U dieresis
\0 Y		; Y acute
\x1 ?		; Thorn
\0 \xa7		; german double s / beta
\0 \x88		; a grave
\0 \x87		; a acute
\0 \x89		; a circumflex
\0 \x8b		; a tilde
\0 \x8a		; a dieresis
\0 \x8c		; a ring
\0 \xbe		; ae
\0 \x8d		; c cedilla
\0 \x8f		; e grave
\0 \x8e		; e acute
\0 \x90		; e circumflex
\0 \x91		; e dieresis
\0 \x93		; i grave
\0 \x92		; i acute
\0 \x94		; i circumflex
\0 \x95		; i dieresis
\x1 ?		; eth
\0 \x96		; n tilde
\0 \x98		; o grave
\0 \x97		; o acute
\0 \x99		; o circumflex
\0 \x9b		; o tilde
\0 \x9a		; o dieresis
\0 \xd6		; division
\0 \xbf		; o slash
\0 \x9d		; u grave
\0 \x9c		; u acute
\0 \x9e		; u circumflex
\0 \x9f		; u dieresis
y e		; y acute
\x1 ?		; thorn
\0 \xd7		; y dieresis
END
