;
; This file is a charset conversion module in text form.
;
; This module Converts CP866 to KOI8-R.
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
2               ; level number
;
CP866           ; from set
KOI8-R		; to set
;
\0 \xE1         ; 128 80 latin capital letter c with cedilla
\0 \xE2         ; 129 81 latin small letter u with diaeresis
\0 \xF7         ; 130 82 latin small letter e with acute
\0 \xE7         ; 131 83 latin small letter a with circumflex
\0 \xE4         ; 132 84 latin small letter a with diaeresis
\0 \xE5         ; 133 85 latin small letter a with grave
\0 \xF6         ; 134 86 latin small letter a with ring above
\0 \xFA         ; 135 87 latin small letter c with cedilla
\0 \xE9         ; 136 88 latin small letter e with circumflex
\0 \xEA         ; 137 89 latin small letter e with diaeresis
\0 \xEB         ; 138 8A latin small letter e with grave
\0 \xEC         ; 139 8B latin small letter i with diaeresis
\0 \xED         ; 140 8C latin small letter i with circumflex
\0 \xEE         ; 141 8D latin small letter i with grave
\0 \xEF         ; 142 8E latin capital letter a with diaeresis
\0 \xF0         ; 143 8F latin capital letter a with ring above
\0 \xF2         ; 144 90 latin capital letter e with acute
\0 \xF3         ; 145 91 latin small letter ae
\0 \xF4         ; 146 92 latin capital letter ae
\0 \xF5         ; 147 93 latin small letter o with circumflex
\0 \xE6         ; 148 94 latin small letter o with diaeresis
\0 \xE8         ; 149 95 latin small letter o with grave
\0 \xE3         ; 150 96 latin small letter u with circumflex
\0 \xFE         ; 151 97 latin small letter u with grave
\0 \xFB         ; 152 98 latin small letter y with diaeresis
\0 \xFD         ; 153 99 latin capital letter o with diaeresis
\0 \xFF         ; 154 9A latin capital letter u with diaeresis
\0 \xF9         ; 155 9B latin small letter o with stroke
\0 \xF8         ; 156 9C pound sign
\0 \xFC         ; 157 9D latin capital letter o with stroke
\0 \xE0         ; 158 9E multiplication sign
\0 \xF1         ; 159 9F dutch guilder sign (ibm437 159)
\0 \xC1         ; 160 A0 latin small letter a with acute
\0 \xC2         ; 161 A1 latin small letter i with acute
\0 \xD7         ; 162 A2 latin small letter o with acute
\0 \xC7         ; 163 A3 latin small letter u with acute
\0 \xC4         ; 164 A4 latin small letter n with tilde
\0 \xC5         ; 165 A5 latin capital letter n with tilde
\0 \xD6         ; 166 A6 feminine ordinal indicator
\0 \xDA         ; 167 A7 masculine ordinal indicator
\0 \xC9         ; 168 A8 inverted question mark
\0 \xCA         ; 169 A9 registered sign
\0 \xCB         ; 170 AA not sign
\0 \xCC         ; 171 AB vulgar fraction one half
\0 \xCD         ; 172 AC vulgar fraction one quarter
\0 \xCE         ; 173 AD inverted exclamation mark
\0 \xCF         ; 174 AE left-pointing double angle quotation mark
\0 \xD0         ; 175 AF right-pointing double angle quotation mark
\0 \x90         ; 176 B0 light shade
\0 \x91         ; 177 B1 medium shade
\0 \x92         ; 178 B2 dark shade
\0 \x81         ; 179 B3 box drawings light vertical
\0 \x87         ; 180 B4 box drawings light vertical and left
\0 \xB2         ; 181 B5 latin capital letter a with acute
\0 \xB4         ; 182 B6 latin capital letter a with circumflex
\0 \xA7         ; 183 B7 latin capital letter a with grave
\0 \xA6         ; 184 B8 copyright sign
\0 \xB5         ; 185 B9 box drawings heavy vertical and left
\0 \xA1         ; 186 BA box drawings heavy vertical
\0 \xA8         ; 187 BB box drawings heavy down and left
\0 \xAE         ; 188 BC box drawings heavy up and left
\0 \xAD         ; 189 BD cent sign
\0 \xAC         ; 190 BE yen sign
\0 \x83         ; 191 BF box drawings light down and left
\0 \x84         ; 192 C0 box drawings light up and right
\0 \x89         ; 193 C1 box drawings light up and horizontal
\0 \x88         ; 194 C2 box drawings light down and horizontal
\0 \x86         ; 195 C3 box drawings light vertical and right
\0 \x80         ; 196 C4 box drawings light horizontal
\0 \x8A         ; 197 C5 box drawings light vertical and horizontal
\0 \xAF         ; 198 C6 latin small letter a with tilde
\0 \xB0         ; 199 C7 latin capital letter a with tilde
\0 \xAB         ; 200 C8 box drawings heavy up and right
\0 \xA5         ; 201 C9 box drawings heavy down and right
\0 \xBB         ; 202 CA box drawings heavy up and horizontal
\0 \xB8         ; 203 CB box drawings heavy down and horizontal
\0 \xB1         ; 204 CC box drawings heavy vertical and right
\0 \xA0         ; 205 CD box drawings heavy horizontal
\0 \xBE         ; 206 CE box drawings heavy vertical and horizontal
\0 \xB9         ; 207 CF currency sign
\0 \xBA         ; 208 D0 latin small letter eth (icelandic)
\0 \xB6         ; 209 D1 latin capital letter eth (icelandic)
\0 \xB7         ; 210 D2 latin capital letter e with circumflex
\0 \xAA         ; 211 D3 latin capital letter e with diaeresis
\0 \xA9         ; 212 D4 latin capital letter e with grave
\0 \xA2         ; 213 D5 latin small letter i dotless
\0 \xA4         ; 214 D6 latin capital letter i with acute
\0 \xBD         ; 215 D7 latin capital letter i with circumflex
\0 \xBC         ; 216 D8 latin capital letter i with diaeresis
\0 \x85         ; 217 D9 box drawings light up and left
\0 \x82         ; 218 DA box drawings light down and right
\0 \x8D         ; 219 DB full block
\0 \x8C         ; 220 DC lower half block
\0 \x8E         ; 221 DD broken bar
\0 \x8F         ; 222 DE latin capital letter i with grave
\0 \x8B         ; 223 DF upper half block
\0 \xD2         ; 224 E0 latin capital letter o with acute
\0 \xD3         ; 225 E1 latin small letter sharp s (german)
\0 \xD4         ; 226 E2 latin capital letter o with circumflex
\0 \xD5         ; 227 E3 latin capital letter o with grave
\0 \xC6         ; 228 E4 latin small letter o with tilde
\0 \xC8         ; 229 E5 latin capital letter o with tilde
\0 \xC3         ; 230 E6 greek small letter mu
\0 \xDE         ; 231 E7 latin capital letter thorn (icelandic)
\0 \xDB         ; 232 E8 latin small letter thorn (icelandic)
\0 \xDD         ; 233 E9 latin capital letter u with acute
\0 \xDF         ; 234 EA latin capital letter u with circumflex
\0 \xD9         ; 235 EB latin capital letter u with grave
\0 \xD8         ; 236 EC latin small letter y with acute
\0 \xDC         ; 237 ED latin capital letter y with acute
\0 \xC0         ; 238 EE em dash
\0 \xD1         ; 239 EF acute accent
\0 \xB3         ; 240 F0 soft hyphen
\0 \xA3         ; 241 F1 plus-minus sign
\0 \x99         ; 242 F2 left right double arrow
\0 \x98         ; 243 F3 vulgar fraction three quarters
\0 \x93         ; 244 F4 pilcrow sign
\0 \x9B         ; 245 F5 section sign
\0 \x9F         ; 246 F6 division sign
\0 \x97         ; 247 F7 ogonek
\0 \x9C         ; 248 F8 degree sign
\0 \x95         ; 249 F9 diaeresis
\0 \x9E         ; 250 FA dot above
\0 \x96         ; 251 FB superscript one
\0 \xBF         ; 252 FC superscript three
\0 \x9D         ; 253 FD superscript two
\0 \x94         ; 254 FE black square
\0 \x9A         ; 255 FF no-break space
END
