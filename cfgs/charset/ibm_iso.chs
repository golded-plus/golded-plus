;
; This file is a charset conversion module in text form.
;
; This module Converts IBM extended characters to ISO 8859-1 characters.
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
LATIN-1		; to set
;
\0 \xC7         ; 128 80 latin capital letter c with cedilla
\0 \xFC         ; 129 81 latin small letter u with diaeresis
\0 \xE9         ; 130 82 latin small letter e with acute
\0 \xE2         ; 131 83 latin small letter a with circumflex
\0 \xE4         ; 132 84 latin small letter a with diaeresis
\0 \xE0         ; 133 85 latin small letter a with grave
\0 \xE5         ; 134 86 latin small letter a with ring above
\0 \xE7         ; 135 87 latin small letter c with cedilla
\0 \xEA         ; 136 88 latin small letter e with circumflex
\0 \xEB         ; 137 89 latin small letter e with diaeresis
\0 \xE8         ; 138 8A latin small letter e with grave
\0 \xEF         ; 139 8B latin small letter i with diaeresis
\0 \xEE         ; 140 8C latin small letter i with circumflex
\0 \xEC         ; 141 8D latin small letter i with grave
\0 \xC4         ; 142 8E latin capital letter a with diaeresis
\0 \xC5         ; 143 8F latin capital letter a with ring above
\0 \xC9         ; 144 90 latin capital letter e with acute
\0 \xE6         ; 145 91 latin small letter ae
\0 \xC6         ; 146 92 latin capital letter ae
\0 \xF4         ; 147 93 latin small letter o with circumflex
\0 \xF6         ; 148 94 latin small letter o with diaeresis
\0 \xF2         ; 149 95 latin small letter o with grave
\0 \xFB         ; 150 96 latin small letter u with circumflex
\0 \xF9         ; 151 97 latin small letter u with grave
\0 \xFF         ; 152 98 latin small letter y with diaeresis
\0 \xD6         ; 153 99 latin capital letter o with diaeresis
\0 \xDC         ; 154 9A latin capital letter u with diaeresis
\0 \xF8         ; 155 9B latin small letter o with stroke (CP865 mapping)
\0 \xA3         ; 156 9C pound sign
\0 \xD8         ; 157 9D latin capital letter o with stroke (CP865 mapping)
P t             ; 158 9E peseta sign
f l             ; 159 9F dutch guilder sign (ibm437 159)
\0 \xE1         ; 160 A0 latin small letter a with acute
\0 \xED         ; 161 A1 latin small letter i with acute
\0 \xF3         ; 162 A2 latin small letter o with acute
\0 \xFA         ; 163 A3 latin small letter u with acute
\0 \xF1         ; 164 A4 latin small letter n with tilde
\0 \xD1         ; 165 A5 latin capital letter n with tilde
\0 \xAA         ; 166 A6 feminine ordinal indicator
\0 \xBA         ; 167 A7 masculine ordinal indicator
\0 \xBF         ; 168 A8 inverted question mark
\0 -            ; 169 A9 reversed not sign
\0 \xAC         ; 170 AA not sign
\0 \xBD         ; 171 AB vulgar fraction one half
\0 \xBC         ; 172 AC vulgar fraction one quarter
\0 \xA1         ; 173 AD inverted exclamation mark
\0 \xAB         ; 174 AE left-pointing double angle quotation mark
\0 \xBB         ; 175 AF right-pointing double angle quotation mark
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
\0 n            ; 220 DC lower half block
\0 |            ; 221 DD left half block
\0 |            ; 222 DE right half block
\0 ~            ; 223 DF upper half block
\0 a            ; 224 E0 greek small letter alpha
\0 \xDF         ; 225 E1 greek small letter beta (here used as german ss)
\x1 ?           ; 226 E2 greek capital letter gamma
p i             ; 227 E3 greek small letter pi
\x1 ?           ; 228 E4 greek capital letter sigma
\x1 ?           ; 229 E5 greek small letter sigma
\0 \xB5         ; 230 E6 greek small letter mu
\x1 ?           ; 231 E7 greek small letter tau
\x1 ?           ; 232 E8 greek capital letter phi
\x1 ?           ; 233 E9 greek capital letter theta
\x1 ?           ; 234 EA greek capital letter omega
\0 \xF0         ; 235 EB greek small letter delta
\x1 ?           ; 236 EC infinity
/ 0             ; 237 ED empty set
\x1 ?           ; 238 EE greek small letter epsilon
\x1 ?           ; 239 EF intersection
= =             ; 240 F0 identical to
\0 \xB1         ; 241 F1 plus-minus sign
> =             ; 242 F2 greater-than or equal to
< =             ; 243 F3 less-than or equal to
\x1 ?           ; 244 F4 top half integral
\x1 ?           ; 245 F5 bottom half integral
\0 \xF7         ; 246 F6 division sign
~ =             ; 247 F7 almost equal to
\0 \xB0         ; 248 F8 ring operator
\0 .            ; 249 F9 middle dot
\0 -            ; 250 FA bullet operator
\x1 ?           ; 251 FB square root
^ n             ; 252 FC superscript latin small letter n
\0 \xB2         ; 253 FD superscript two
\0 \xB7         ; 254 FE black square
\0 \xA0         ; 255 FF no-break space
END
