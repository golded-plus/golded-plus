;
; This file is a charset conversion module in text form.
;
; This module Converts IBM extended characters to K. Simonsen mnemonics.
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
MNEMONIC        ; to set
;
\d29 C ,        ; 128 80 latin capital letter c with cedilla
\d29 u :        ; 129 81 latin small letter u with diaeresis
\d29 e '        ; 130 82 latin small letter e with acute
\d29 a >        ; 131 83 latin small letter a with circumflex
\d29 a :        ; 132 84 latin small letter a with diaeresis
\d29 a !        ; 133 85 latin small letter a with grave
\d29 a a        ; 134 86 latin small letter a with ring above
\d29 c ,        ; 135 87 latin small letter c with cedilla
\d29 e >        ; 136 88 latin small letter e with circumflex
\d29 e :        ; 137 89 latin small letter e with diaeresis
\d29 e !        ; 138 8A latin small letter e with grave
\d29 i :        ; 139 8B latin small letter i with diaeresis
\d29 i >        ; 140 8C latin small letter i with circumflex
\d29 i !        ; 141 8D latin small letter i with grave
\d29 A :        ; 142 8E latin capital letter a with diaeresis
\d29 A A        ; 143 8F latin capital letter a with ring above
\d29 E '        ; 144 90 latin capital letter e with acute
\d29 a e        ; 145 91 latin small letter ae
\d29 A E        ; 146 92 latin capital letter ae
\d29 o >        ; 147 93 latin small letter o with circumflex
\d29 o :        ; 148 94 latin small letter o with diaeresis
\d29 o !        ; 149 95 latin small letter o with grave
\d29 u >        ; 150 96 latin small letter u with circumflex
\d29 u !        ; 151 97 latin small letter u with grave
\d29 y :        ; 152 98 latin small letter y with diaeresis
\d29 O :        ; 153 99 latin capital letter o with diaeresis
\d29 U :        ; 154 9A latin capital letter u with diaeresis
\d29 o /        ; 155 9B latin small letter o with stroke (CP865 mapping)
\d29 P d        ; 156 9C pound sign
\d29 O /        ; 157 9D latin capital letter o with stroke (CP865 mapping)
\d29 P t        ; 158 9E peseta sign
\d29 F l        ; 159 9F dutch guilder sign (ibm437 159)
\d29 a '        ; 160 A0 latin small letter a with acute
\d29 i '        ; 161 A1 latin small letter i with acute
\d29 o '        ; 162 A2 latin small letter o with acute
\d29 u '        ; 163 A3 latin small letter u with acute
\d29 n ?        ; 164 A4 latin small letter n with tilde
\d29 N ?        ; 165 A5 latin capital letter n with tilde
\d29 - a        ; 166 A6 feminine ordinal indicator
\d29 - o        ; 167 A7 masculine ordinal indicator
\d29 ? I        ; 168 A8 inverted question mark
\d29 N I        ; 169 A9 reversed not sign
\d29 N O        ; 170 AA not sign
\d29 1 2        ; 171 AB vulgar fraction one half
\d29 1 4        ; 172 AC vulgar fraction one quarter
\d29 ! I        ; 173 AD inverted exclamation mark
\d29 < <        ; 174 AE left-pointing double angle quotation mark
\d29 > >        ; 175 AF right-pointing double angle quotation mark
\d29 . S        ; 176 B0 light shade
\d29 : S        ; 177 B1 medium shade
\d29 ? S        ; 178 B2 dark shade
\d29 v v        ; 179 B3 box drawings light vertical
\d29 v l        ; 180 B4 box drawings light vertical and left
\d29 v L        ; 181 B5 box drawings vertical light and left heavy
\d29 V l        ; 182 B6 box drawings vertical heavy and left light
\d29 D l        ; 183 B7 box drawings down heavy and left light
\d29 d L        ; 184 B8 box drawings down light and left heavy
\d29 V L        ; 185 B9 box drawings heavy vertical and left
\d29 V V        ; 186 BA box drawings heavy vertical
\d29 L D        ; 187 BB box drawings heavy down and left
\d29 U L        ; 188 BC box drawings heavy up and left
\d29 U l        ; 189 BD box drawings up heavy and left light
\d29 u L        ; 190 BE box drawings up light and left heavy
\d29 d l        ; 191 BF box drawings light down and left
\d29 u r        ; 192 C0 box drawings light up and right
\d29 u h        ; 193 C1 box drawings light up and horizontal
\d29 d h        ; 194 C2 box drawings light down and horizontal
\d29 v r        ; 195 C3 box drawings light vertical and right
\d29 h h        ; 196 C4 box drawings light horizontal
\d29 v h        ; 197 C5 box drawings light vertical and horizontal
\d29 v R        ; 198 C6 box drawings vertical light and right heavy
\d29 V r        ; 199 C7 box drawings vertical heavy and right light
\d29 U R        ; 200 C8 box drawings heavy up and right
\d29 D R        ; 201 C9 box drawings heavy down and right
\d29 U H        ; 202 CA box drawings heavy up and horizontal
\d29 D H        ; 203 CB box drawings heavy down and horizontal
\d29 V R        ; 204 CC box drawings heavy vertical and right
\d29 H H        ; 205 CD box drawings heavy horizontal
\d29 V H        ; 206 CE box drawings heavy vertical and horizontal
\d29 u H        ; 207 CF box drawings up light and horizontal heavy
\d29 U h        ; 208 D0 box drawings up heavy and horizontal light
\d29 d H        ; 209 D1 box drawings down light and horizontal heavy
\d29 D h        ; 210 D2 box drawings down heavy and horizontal light
\d29 U r        ; 211 D3 box drawings up heavy and right light
\d29 u R        ; 212 D4 box drawings up light and right heavy
\d29 D r        ; 213 D5 box drawings down light and right heavy
\d29 d R        ; 214 D6 box drawings down heavy and right light
\d29 V h        ; 215 D7 box drawings vertical heavy and horizontal light
\d29 v H        ; 216 D8 box drawings vertical light and horizontal heavy
\d29 u l        ; 217 D9 box drawings light up and left
\d29 d r        ; 218 DA box drawings light down and right
\d29 F B        ; 219 DB full block
\d29 L B        ; 220 DC lower half block
\d29 l B        ; 221 DD left half block
\d29 R B        ; 222 DE right half block
\d29 T B        ; 223 DF upper half block
\d29 a *        ; 224 E0 greek small letter alpha
\d29 b *        ; 225 E1 greek small letter beta (here used as german ss)
\d29 G *        ; 226 E2 greek capital letter gamma
\d29 p *        ; 227 E3 greek small letter pi
\d29 S *        ; 228 E4 greek capital letter sigma
\d29 s *        ; 229 E5 greek small letter sigma
\d29 m *        ; 230 E6 greek small letter mu
\d29 t *        ; 231 E7 greek small letter tau
\d29 F *        ; 232 E8 greek capital letter phi
\d29 H *        ; 233 E9 greek capital letter theta
\d29 W *        ; 234 EA greek capital letter omega
\d29 d *        ; 235 EB greek small letter delta
\d29 0 0        ; 236 EC infinity
\d29 / 0        ; 237 ED empty set
\d29 e *        ; 238 EE greek small letter epsilon
\d29 ( U        ; 239 EF intersection
\d29 = 3        ; 240 F0 identical to
\d29 + -        ; 241 F1 plus-minus sign
\d29 > =        ; 242 F2 greater-than or equal to
\d29 < =        ; 243 F3 less-than or equal to
\d29 I u        ; 244 F4 top half integral
\d29 I l        ; 245 F5 bottom half integral
\d29 - :        ; 246 F6 division sign
\d29 ? 2        ; 247 F7 almost equal to
\d29 O b        ; 248 F8 ring operator
\d29 . M        ; 249 F9 middle dot
\d29 S b        ; 250 FA bullet operator
\d29 R T        ; 251 FB square root
\d29 n S        ; 252 FC superscript latin small letter n
\d29 2 S        ; 253 FD superscript two
\d29 f S        ; 254 FE black square
\d29 N S        ; 255 FF no-break space
END
