;
; This file is a charset conversion module in text form.
;
; This module Converts IBM extended characters to ISO 8859-1 characters,
; and uses I51 compatible escape sequences for non-ISO chars.
;
; Format: ID, version, level,
;         from charset, to charset,
;         128 entries: first & second byte
;         "END"
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
0               ; ID number
0               ; version number
;
0               ; level number
;
IBMPC           ; from set
I51             ; to set
;
\xC7            ; C with cedilla
\xDC            ; u dieresis
\xE9            ; e acute
\xE2            ; a circumflex
\xE4            ; a dieresis
\xE0            ; a grave
\xE5            ; a ring
\xE7            ; c cedilla
\xEA            ; e circumflex
\xEB            ; e dieresis
\xE8            ; e grave
\xEF            ; i dieresis
\xEE            ; i circumflex
\xEC            ; i grave
\xC4            ; A dieresis
\xC5            ; A ring
\xC9            ; E acute
\xE6            ; ae
\xC6            ; AE
\xF4            ; o circumflex
\xF6            ; o dieresis
\xF3            ; o acute
\xDB            ; u circumflex
\xD9            ; u grave
\xFF            ; y dieresis
\xD6            ; O dieresis
\xDC            ; U dieresis
\xA2            ; cent
\xA3            ; pound sterling
\xA5            ; yen
\d2 $P          ; Pt
\d2 $f          ; florin
\xE1            ; a acute
\xCC            ; i grave
\xF2            ; o grave
\xF9            ; u grave
\xF1            ; n tilde
\xD1            ; N tilde
\xAA            ; ord feminine
\xBA            ; ord masculine
\xBF            ; question downwards
-               ;
\xAC            ; logical not
\xBD            ; half fraction
\xBC            ; quarter fraction
\xA1            ; exclam downwards
\xAB            ; guillemot left
\xBB            ; guillemot right
#               ;
#               ;
#               ;
|               ;
|               ;
|               ;
|               ;
+               ;
+               ;
|               ;
|               ;
+               ;
+               ;
+               ;
+               ;
+               ;
+               ;
-               ;
-               ;
|               ;
-               ;
+               ;
|               ;
|               ;
+               ;
+               ;
=               ;
=               ;
|               ;
=               ;
+               ;
=               ;
-               ;
=               ;
-               ;
+               ;
+               ;
+               ;
+               ;
|               ;
+               ;
+               ;
+               ;
#               ;
-               ;
|               ;
|               ;
-               ;
\d2 ga          ; alpha
\xDF            ; german double s (misused as Beta)
\d2 gG          ; Gamma
pi              ; pi
?               ; Sigma (summation)
?               ; sigma
\xB5            ; mu
?               ; gamma
?               ; Phi
?               ; Theta
?               ; Omega
\xF0            ; delta
?               ; infinity
\xF8            ; o slash
?               ; element
?               ; intersection
\d2 -=          ; equivalence
\xB1            ; plusminus
\d2 ->          ; greater equals
\d2 -<          ; smaller equals
?               ; integral top
?               ; integral bottom
\xF7            ; divide
\d2 -~          ; approx.
\xB0            ; ring / degree
.               ; centered dot
-               ; en dash
?               ; radical
^n              ; to the n'th power
\xB2            ; to the second power
\xB7            ; bullet
\xA0            ; non-breaking space
END
