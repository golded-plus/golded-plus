;
; This file is a charset conversion module in text form.
;
; This module converts the Finnish set to IBMPC.
; You can use it as a basis for a level 1 converter.
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
; e.g.: \x32 is the ASCII space character
; Two \\ is the character "\" itself.
;
0               ; ID number
0               ; version number
;
1               ; level number
;
FINNISH         ; from set
IBMPC           ; to set
;
\0 \0           ; NUL
\0 \x1          ; SOH
\0 \x2          ; STX
\0 \x3          ; ETX
\0 \x4          ; EOT
\0 \x5          ; ENQ
\0 \x6          ; ACK
\0 \x7          ; BEL
\0 \x8          ; BS
\0 \x9          ; HT
\0 \xA          ; LF
\0 \xB          ; VT
\0 \xC          ; FF
\0 \xD          ; CR
\0 \xE          ; SO
\0 \xF          ; SI
\0 \x10         ; DLE
\0 \x11         ; DC1
\0 \x12         ; DC2
\0 \x13         ; DC3
\0 \x14         ; DC4
\0 \x15         ; NAK
\0 \x16         ; SYN
\0 \x17         ; ETB
\0 \x18         ; CAN
\0 \x19         ; EM
\0 \x1a         ; SUB
\0 \x1b         ; ESC
\0 \x1c         ; FS
\0 \x1d         ; GS
\0 \x1e         ; RS
\0 \x1f         ; US
\0 \x20         ; space
\0 !            ; exclam
\0 "            ; double quotes
\0 #            ; pound / number
\0 $            ; dollar
\0 %            ; percent
\0 &            ; ampersand
\0 '            ; quote right
\0 (            ; parenthesis left
\0 )            ; parenthesis right
\0 *            ; asterisk
\0 +            ; plus
\0 ,            ; comma
\0 -            ; minus
\0 .            ; period
\0 /            ; slash
\0 0            ; zero
\0 1            ; one
\0 2            ; two
\0 3            ; three
\0 4            ; four
\0 5            ; five
\0 6            ; six
\0 7            ; seven
\0 8            ; eight
\0 9            ; nine
\0 :            ; colon
\0 \d59         ; semicolon
\0 <            ; less than
\0 =            ; equals
\0 >            ; greater than
\0 ?            ; question
\0 @            ; at
\0 A            ;
\0 B            ;
\0 C            ;
\0 D            ;
\0 E            ;
\0 F            ;
\0 G            ;
\0 H            ;
\0 I            ;
\0 J            ;
\0 K            ;
\0 L            ;
\0 M            ;
\0 N            ;
\0 O            ;
\0 P            ;
\0 Q            ;
\0 R            ;
\0 S            ;
\0 T            ;
\0 U            ;
\0 V            ;
\0 W            ;
\0 X            ;
\0 Y            ;
\0 Z            ;
\0 \x8E         ; bracket left   -> A dieresis
\0 \x99         ; backslash      -> O dieresis
\0 \x8F         ; bracket right  -> A ring
\0 \x9A         ; circum         -> U dieresis
\0 _            ; underscore
\0 \x82         ; quote left     -> e acute
\0 a            ;
\0 b            ;
\0 c            ;
\0 d            ;
\0 e            ;
\0 f            ;
\0 g            ;
\0 h            ;
\0 i            ;
\0 j            ;
\0 k            ;
\0 l            ;
\0 m            ;
\0 n            ;
\0 o            ;
\0 p            ;
\0 q            ;
\0 r            ;
\0 s            ;
\0 t            ;
\0 u            ;
\0 v            ;
\0 w            ;
\0 x            ;
\0 y            ;
\0 z            ;
\0 \x84         ; brace left     -> a dieresis
\0 \x94         ; bar            -> o dieresis
\0 \x86         ; brace right    -> a ring
\0 \x81         ; tilde          -> u dieresis
\0 \x7F         ; DEL
END
