;
; This file is a charset conversion module in text form.
;
; This module Converts IBM extended characters to ASCII.
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
IBMPC           ; from set (actually CP865)
ASCII           ; to set
;
\0 C            ; 128 80 latin capital letter c with cedilla
u e             ; 129 81 latin small letter u with diaeresis
\0 e            ; 130 82 latin small letter e with acute
\0 a            ; 131 83 latin small letter a with circumflex
a e             ; 132 84 latin small letter a with diaeresis
\0 a            ; 133 85 latin small letter a with grave
a a             ; 134 86 latin small letter a with ring above
\0 c            ; 135 87 latin small letter c with cedilla
\0 e            ; 136 88 latin small letter e with circumflex
e e             ; 137 89 latin small letter e with diaeresis
\0 e            ; 138 8A latin small letter e with grave
i e             ; 139 8B latin small letter i with diaeresis
\0 i            ; 140 8C latin small letter i with circumflex
\0 i            ; 141 8D latin small letter i with grave
A e             ; 142 8E latin capital letter a with diaeresis
\0 A            ; 143 8F latin capital letter a with ring above
\0 E            ; 144 90 latin capital letter e with acute
a e             ; 145 91 latin small letter ae
A E             ; 146 92 latin capital letter ae
\0 o            ; 147 93 latin small letter o with circumflex
o e             ; 148 94 latin small letter o with diaeresis
\0 o            ; 149 95 latin small letter o with grave
\0 u            ; 150 96 latin small letter u with circumflex
\0 u            ; 151 97 latin small letter u with grave
y e             ; 152 98 latin small letter y with diaeresis
O e             ; 153 99 latin capital letter o with diaeresis
U e             ; 154 9A latin capital letter u with diaeresis
o e             ; 155 9B latin small letter o with stroke (CP865 mapping)
\0 #            ; 156 9C pound sign
O e             ; 157 9D latin capital letter o with stroke (CP865 mapping)
P t             ; 158 9E peseta sign
F l             ; 159 9F dutch guilder sign (ibm437 159)
\0 a            ; 160 A0 latin small letter a with acute
\0 i            ; 161 A1 latin small letter i with acute
\0 o            ; 162 A2 latin small letter o with acute
\0 u            ; 163 A3 latin small letter u with acute
\0 n            ; 164 A4 latin small letter n with tilde
\0 N            ; 165 A5 latin capital letter n with tilde
\0 a            ; 166 A6 feminine ordinal indicator
\0 o            ; 167 A7 masculine ordinal indicator
\0 ?            ; 168 A8 inverted question mark
\0 -            ; 169 A9 reversed not sign
\0 !            ; 170 AA not sign
1 / 2           ; 171 AB vulgar fraction one half
1 / 4           ; 172 AC vulgar fraction one quarter
\0 !            ; 173 AD inverted exclamation mark
< <             ; 174 AE left-pointing double angle quotation mark
> >             ; 175 AF right-pointing double angle quotation mark
\0 #            ; 176 B0 light shade
\0 #            ; 177 B1 medium shade
\0 #            ; 178 B2 dark shade
\0 |            ; 179 B3 box drawings light vertical
\0 |            ; 180 B4 box drawings light vertical and left
\0 |            ; 181 B5 box drawings vertical light and left heavy
\0 |            ; 182 B6 box drawings vertical heavy and left light
\0 +            ; 183 B7 box drawings down heavy and left light
\0 +            ; 184 B8 box drawings down light and left heavy
\0 |            ; 185 B9 box drawings heavy vertical and left
\0 |            ; 186 BA box drawings heavy vertical
\0 +            ; 187 BB box drawings heavy down and left
\0 +            ; 188 BC box drawings heavy up and left
\0 +            ; 189 BD box drawings up heavy and left light
\0 +            ; 190 BE box drawings up light and left heavy
\0 +            ; 191 BF box drawings light down and left
\0 +            ; 192 C0 box drawings light up and right
\0 -            ; 193 C1 box drawings light up and horizontal
\0 -            ; 194 C2 box drawings light down and horizontal
\0 |            ; 195 C3 box drawings light vertical and right
\0 -            ; 196 C4 box drawings light horizontal
\0 +            ; 197 C5 box drawings light vertical and horizontal
\0 |            ; 198 C6 box drawings vertical light and right heavy
\0 |            ; 199 C7 box drawings vertical heavy and right light
\0 +            ; 200 C8 box drawings heavy up and right
\0 +            ; 201 C9 box drawings heavy down and right
\0 =            ; 202 CA box drawings heavy up and horizontal
\0 =            ; 203 CB box drawings heavy down and horizontal
\0 |            ; 204 CC box drawings heavy vertical and right
\0 =            ; 205 CD box drawings heavy horizontal
\0 +            ; 206 CE box drawings heavy vertical and horizontal
\0 =            ; 207 CF box drawings up light and horizontal heavy
\0 -            ; 208 D0 box drawings up heavy and horizontal light
\0 =            ; 209 D1 box drawings down light and horizontal heavy
\0 -            ; 210 D2 box drawings down heavy and horizontal light
\0 +            ; 211 D3 box drawings up heavy and right light
\0 +            ; 212 D4 box drawings up light and right heavy
\0 +            ; 213 D5 box drawings down light and right heavy
\0 +            ; 214 D6 box drawings down heavy and right light
\0 |            ; 215 D7 box drawings vertical heavy and horizontal light
\0 +            ; 216 D8 box drawings vertical light and horizontal heavy
\0 +            ; 217 D9 box drawings light up and left
\0 +            ; 218 DA box drawings light down and right
\0 #            ; 219 DB full block
\0 -            ; 220 DC lower half block
\0 |            ; 221 DD left half block
\0 |            ; 222 DE right half block
\0 -            ; 223 DF upper half block
\0 a            ; 224 E0 greek small letter alpha
s s             ; 225 E1 greek small letter beta (here used as german ss)
\x1 ?           ; 226 E2 greek capital letter gamma
p i             ; 227 E3 greek small letter pi
\x1 ?           ; 228 E4 greek capital letter sigma
\x1 ?           ; 229 E5 greek small letter sigma
m u             ; 230 E6 greek small letter mu
\x1 ?           ; 231 E7 greek small letter tau
\x1 ?           ; 232 E8 greek capital letter phi
\x1 ?           ; 233 E9 greek capital letter theta
\0 O            ; 234 EA greek capital letter omega
\0 d            ; 235 EB greek small letter delta
\x1 ?           ; 236 EC infinity
\0 o            ; 237 ED empty set
\x1 ?           ; 238 EE greek small letter epsilon
\x1 ?           ; 239 EF intersection
= =             ; 240 F0 identical to
+ -             ; 241 F1 plus-minus sign
> =             ; 242 F2 greater-than or equal to
< =             ; 243 F3 less-than or equal to
\x1 ?           ; 244 F4 top half integral
\x1 ?           ; 245 F5 bottom half integral
\0 /            ; 246 F6 division sign
~ =             ; 247 F7 almost equal to
\0 '            ; 248 F8 ring operator
\0 .            ; 249 F9 middle dot
\0 -            ; 250 FA bullet operator
\x1 ?           ; 251 FB square root
^ n             ; 252 FC superscript latin small letter n
^ 2             ; 253 FD superscript two
\0 *            ; 254 FE black square
\0 \d32         ; 255 FF no-break space
END
