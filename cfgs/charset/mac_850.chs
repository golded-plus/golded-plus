;
; This file is a charset conversion module in text form.
;
; This module Converts Macintosh extended characters to IBM CP850 characters.
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
CP850           ; to set
;
\0 \x8E         ; 128 80 latin capital letter a with diaeresis
\0 \x8F         ; 129 81 latin capital letter a with ring above
\0 \x80         ; 130 82 latin capital letter c with cedilla
\0 \x90         ; 131 83 latin capital letter e with acute
\0 \xA5         ; 132 84 latin capital letter n with tilde
\0 \x94         ; 133 85 latin capital letter o with diaeresis
\0 \x9A         ; 134 86 latin capital letter u with diaeresis
\0 \xA0         ; 135 87 latin small letter a with acute
\0 \x85         ; 136 88 latin small letter a with grave
\0 \x83         ; 137 89 latin small letter a with circumflex
\0 \x84         ; 138 8A latin small letter a with diaeresis
\0 \xC6         ; 139 8B latin small letter a with tilde
\0 \x86         ; 140 8C latin small letter a with ring above
\0 \x87         ; 141 8D latin small letter c with cedilla
\0 \x82         ; 142 8E latin small letter e with acute
\0 \x8A         ; 143 8F latin small letter e with grave
\0 \x88         ; 144 90 latin small letter e with circumflex
\0 \x89         ; 145 91 latin small letter e with diaeresis
\0 \xA1         ; 146 92 latin small letter i with acute
\0 \x8D         ; 147 93 latin small letter i with grave
\0 \x8C         ; 148 94 latin small letter i with circumflex
\0 \x8B         ; 149 95 latin small letter i with diaeresis
\0 \xA4         ; 150 96 latin small letter n with tilde
\0 \xA2         ; 151 97 latin small letter o with acute
\0 \x95         ; 152 98 latin small letter o with grave
\0 \x96         ; 153 99 latin small letter o with circumflex
\0 \x94         ; 154 9A latin small letter o with diaeresis
\0 \xE4         ; 155 9B latin small letter o with tilde
\0 \xA3         ; 156 9C latin small letter u with acute
\0 \x97         ; 157 9D latin small letter u with grave
\0 \x96         ; 158 9E latin small letter u with circumflex
\0 \x81         ; 159 9F latin small letter u with diaeresis
\0 +            ; 160 A0 dagger
\0 \xF8         ; 161 A1 degree sign
\0 \x9B         ; 162 A2 cent sign
\0 \x9C         ; 163 A3 pound sign
\0 \x15         ; 164 A4 section sign
\0 \xFE         ; 165 A5 bullet operator
\0 \x14         ; 166 A6 pilcrow sign
\0 \xE1         ; 167 A7 latin small letter sharp s (german)
\0 \xA9         ; 168 A8 registered sign
\0 \xB8         ; 169 A9 copyright sign
t m             ; 170 AA trade mark sign
\0 \xEF         ; 171 AB acute accent
\0 \xF9         ; 172 AC diaeresis
< >             ; 173 AD not equal to
\0 \x92         ; 174 AE latin capital letter ae
\0 \x9D         ; 175 AF latin capital letter o with stroke
\0 \xEC         ; 176 B0 infinity
\0 \xF1         ; 177 B1 plus-minus sign
\0 \xF3         ; 178 B2 less-than or equal to
\0 \xF2         ; 179 B3 greater-than or equal to
\0 \x9D         ; 180 B4 yen sign
\0 \xE6         ; 181 B5 micro sign
\0 \xEB         ; 182 B6 partial differential
\0 \xE4         ; 183 B7 n-ary summation
\0 x            ; 184 B8 n-ary product
\0 \xE3         ; 185 B9 greek small letter pi
I n             ; 186 BA integral
\0 \xA6         ; 187 BB feminine ordinal indicator
\0 \xA7         ; 188 BC masculine ordinal indicator
\0 \xEA         ; 189 BD greek capital letter omega
\0 \x91         ; 190 BE latin small letter ae
\0 \x9b         ; 191 BF latin small letter o with stroke
\0 \xA8         ; 192 C0 inverted question mark
\0 \xAD         ; 193 C1 inverted exclamation mark
\0 \xAA         ; 194 C2 not sign
\0 \xFB         ; 195 C3 square root
\0 \x9F         ; 196 C4 dutch guilder sign (ibm437 159)
\0 \xF7         ; 197 C5 almost equal to
\0 \x7F         ; 198 C6 greek capital letter delta
\0 \xAF         ; 199 C7 left-pointing double angle quotation mark
\0 \xAE         ; 200 C8 right-pointing double angle quotation mark
. .             ; 201 C9 midline horizontal ellipsis
\0 \d32         ; 202 CA no-break space
\0 \xB7         ; 203 CB latin capital letter a with grave
\0 \xC7         ; 204 CC latin capital letter a with tilde
\0 \xE5         ; 205 CD latin capital letter o with tilde
O E             ; 206 CE latin capital ligature oe
o e             ; 207 CF latin small ligature oe
\0 \xFA         ; 208 D0 em dash
\0 -            ; 209 D1 en dash
\0 "            ; 210 D2 left double quotation mark
\0 "            ; 211 D3 right double quotation mark
\0 `            ; 212 D4 left single quotation mark
\0 '            ; 213 D5 right single quotation mark
\0 \xF6         ; 214 D6 division sign
\0 \x04         ; 215 D7 black diamond
\0 \x98         ; 216 D8 latin small letter y with diaeresis
\0 \x98         ; 217 D9 latin capital letter y with diaeresis
\0 /            ; 218 DA fraction slash
C u             ; 219 DB currency sign
\0 <            ; 220 DC single left-pointing angle quotation mark
\0 >            ; 221 DD single right-pointing angle quotation mark
f i             ; 222 DE latin small ligature fi
f l             ; 223 DF latin small ligature fl
+ +             ; 224 E0 double dagger
\0 .            ; 225 E1 middle dot
\0 '            ; 226 E2 single low-9 quotation mark
\0 "            ; 227 E3 double low-9 quotation mark
% .             ; 228 E4 per mille sign
\0 \xB6         ; 229 E5 latin capital letter a with circumflex
\0 \xD2         ; 230 E6 latin capital letter e with circumflex
\0 \xB5         ; 231 E7 latin capital letter a with acute
\0 \xD3         ; 232 E8 latin capital letter e with diaeresis
\0 \xD4         ; 233 E9 latin capital letter e with grave
\0 \xD6         ; 234 EA latin capital letter i with acute
\0 \xD7         ; 235 EB latin capital letter i with circumflex
\0 \xD8         ; 236 EC latin capital letter i with diaeresis
\0 \xDE         ; 237 ED latin capital letter i with grave
\0 \xE0         ; 238 EE latin capital letter o with acute
\0 \xE2         ; 239 EF latin capital letter o with circumflex
\x1 ?           ; 240 F0 ???
\0 \xE3         ; 241 F1 latin capital letter o with grave
\0 \xE9         ; 242 F2 latin capital letter u with acute
\0 \xEA         ; 243 F3 latin capital letter u with circumflex
\0 \xEB         ; 244 F4 latin capital letter u with grave
\0 i            ; 245 F5 latin small letter i dotless
\x1 ?           ; 246 F6 ???
\x1 ?           ; 247 F7 ???
\0 \xEE         ; 248 F8 overline
\0 u            ; 249 F9 breve
\0 \xFA         ; 250 FA dot above
\0 o            ; 251 FB ring above
\0 ,            ; 252 FC cedilla
\0 "            ; 253 FD double acute accent
\0 \xF7         ; 254 FE ogonek
\0 v            ; 255 FF caron
END
