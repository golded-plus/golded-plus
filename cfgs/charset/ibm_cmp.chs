;
; This file is a charset conversion module in text form.
;
; This module Converts IBM extended characters to Composed characters.
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
IBMPC           ; from set (actually CP865)
COMPOSED        ; to set
;
, \x8D C        ; 128 80 latin capital letter c with cedilla
u \x8D "        ; 129 81 latin small letter u with diaeresis
e \x8D '        ; 130 82 latin small letter e with acute
a \x8D ^        ; 131 83 latin small letter a with circumflex
a \x8D "        ; 132 84 latin small letter a with diaeresis
a \x8D `        ; 133 85 latin small letter a with grave
a \x8D *        ; 134 86 latin small letter a with ring above
, \x8D c        ; 135 87 latin small letter c with cedilla
e \x8D ^        ; 136 88 latin small letter e with circumflex
e \x8D "        ; 137 89 latin small letter e with diaeresis
e \x8D `        ; 138 8A latin small letter e with grave
i \x8D "        ; 139 8B latin small letter i with diaeresis
i \x8D ^        ; 140 8C latin small letter i with circumflex
i \x8D `        ; 141 8D latin small letter i with grave
A \x8D "        ; 142 8E latin capital letter a with diaeresis
A \x8D *        ; 143 8F latin capital letter a with ring above
E \x8D '        ; 144 90 latin capital letter e with acute
a \x8D e        ; 145 91 latin small letter ae
A \x8D E        ; 146 92 latin capital letter ae
o \x8D ^        ; 147 93 latin small letter o with circumflex
o \x8D "        ; 148 94 latin small letter o with diaeresis
o \x8D `        ; 149 95 latin small letter o with grave
u \x8D ^        ; 150 96 latin small letter u with circumflex
u \x8D `        ; 151 97 latin small letter u with grave
y \x8D "        ; 152 98 latin small letter y with diaeresis
O \x8D "        ; 153 99 latin capital letter o with diaeresis
U \x8D "        ; 154 9A latin capital letter u with diaeresis
o \x8D e        ; 155 9B latin small letter o with stroke (CP865 mapping)
P \x8D d        ; 156 9C pound sign
O \x8D E        ; 157 9D latin capital letter o with stroke (CP865 mapping)
P \x8D t        ; 158 9E peseta sign
F \x8D l        ; 159 9F dutch guilder sign (ibm437 159)
a \x8D '        ; 160 A0 latin small letter a with acute
i \x8D '        ; 161 A1 latin small letter i with acute
o \x8D '        ; 162 A2 latin small letter o with acute
u \x8D '        ; 163 A3 latin small letter u with acute
n \x8D ~        ; 164 A4 latin small letter n with tilde
N \x8D ~        ; 165 A5 latin capital letter n with tilde
a \x8D _        ; 166 A6 feminine ordinal indicator
o \x8D _        ; 167 A7 masculine ordinal indicator
? \x8D ?        ; 168 A8 inverted question mark
?               ; 169 A9 reversed not sign
?               ; 170 AA not sign
' \x8D 2        ; 171 AB vulgar fraction one half
' \x8D 4        ; 172 AC vulgar fraction one quarter
! \x8D !        ; 173 AD inverted exclamation mark
< \x8D <        ; 174 AE left-pointing double angle quotation mark
> \x8D >        ; 175 AF right-pointing double angle quotation mark
#               ; 176 B0 light shade
#               ; 177 B1 medium shade
#               ; 178 B2 dark shade
|               ; 179 B3 box drawings light vertical
|               ; 180 B4 box drawings light vertical and left
|               ; 181 B5 box drawings vertical light and left heavy
|               ; 182 B6 box drawings vertical heavy and left light
+               ; 183 B7 box drawings down heavy and left light
+               ; 184 B8 box drawings down light and left heavy
|               ; 185 B9 box drawings heavy vertical and left
|               ; 186 BA box drawings heavy vertical
+               ; 187 BB box drawings heavy down and left
+               ; 188 BC box drawings heavy up and left
+               ; 189 BD box drawings up heavy and left light
+               ; 190 BE box drawings up light and left heavy
+               ; 191 BF box drawings light down and left
+               ; 192 C0 box drawings light up and right
-               ; 193 C1 box drawings light up and horizontal
-               ; 194 C2 box drawings light down and horizontal
|               ; 195 C3 box drawings light vertical and right
-               ; 196 C4 box drawings light horizontal
+               ; 197 C5 box drawings light vertical and horizontal
|               ; 198 C6 box drawings vertical light and right heavy
|               ; 199 C7 box drawings vertical heavy and right light
+               ; 200 C8 box drawings heavy up and right
+               ; 201 C9 box drawings heavy down and right
=               ; 202 CA box drawings heavy up and horizontal
=               ; 203 CB box drawings heavy down and horizontal
|               ; 204 CC box drawings heavy vertical and right
=               ; 205 CD box drawings heavy horizontal
+               ; 206 CE box drawings heavy vertical and horizontal
=               ; 207 CF box drawings up light and horizontal heavy
-               ; 208 D0 box drawings up heavy and horizontal light
=               ; 209 D1 box drawings down light and horizontal heavy
-               ; 210 D2 box drawings down heavy and horizontal light
+               ; 211 D3 box drawings up heavy and right light
+               ; 212 D4 box drawings up light and right heavy
+               ; 213 D5 box drawings down light and right heavy
+               ; 214 D6 box drawings down heavy and right light
|               ; 215 D7 box drawings vertical heavy and horizontal light
+               ; 216 D8 box drawings vertical light and horizontal heavy
+               ; 217 D9 box drawings light up and left
+               ; 218 DA box drawings light down and right
#               ; 219 DB full block
-               ; 220 DC lower half block
|               ; 221 DD left half block
|               ; 222 DE right half block
-               ; 223 DF upper half block
?               ; 224 E0 greek small letter alpha
s \x8D s        ; 225 E1 greek small letter beta (here used as german ss)
?               ; 226 E2 greek capital letter gamma
p \x8D i        ; 227 E3 greek small letter pi
?               ; 228 E4 greek capital letter sigma
?               ; 229 E5 greek small letter sigma
?               ; 230 E6 greek small letter mu
?               ; 231 E7 greek small letter tau
?               ; 232 E8 greek capital letter phi
?               ; 233 E9 greek capital letter theta
?               ; 234 EA greek capital letter omega
?               ; 235 EB greek small letter delta
?               ; 236 EC infinity
?               ; 237 ED empty set
?               ; 238 EE greek small letter epsilon
?               ; 239 EF intersection
= \x8D =        ; 240 F0 identical to
+ \x8D -        ; 241 F1 plus-minus sign
> \x8D =        ; 242 F2 greater-than or equal to
< \x8D =        ; 243 F3 less-than or equal to
?               ; 244 F4 top half integral
?               ; 245 F5 bottom half integral
/               ; 246 F6 division sign
~ \x8D =        ; 247 F7 almost equal to
?               ; 248 F8 ring operator
.               ; 249 F9 middle dot
-               ; 250 FA bullet operator
?               ; 251 FB square root
^ \x8D n        ; 252 FC superscript latin small letter n
^ \x8D 2        ; 253 FD superscript two
*               ; 254 FE black square
\d32            ; 255 FF no-break space
END
