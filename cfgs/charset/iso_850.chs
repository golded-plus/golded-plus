;
; This file is a charset conversion module in text form.
;
; This module converts ISO 8859-1 extended characters to IBM CP850 characters.
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
; \dnnn where nnn is a decimal number, is the ordinal value of the character
; \xnn where nn is a hexadecimal number
; e.g.: \d32 is the ASCII space character
; Two \\ is the character "\" itself.
;
0               ; ID number
0               ; version number
;
2               ; level number
;
LATIN-1         ; from set
CP850           ; to set
;
\0 \d128        ; 128 80 (missing) These codes are unused in the LATIN-1 set.
\0 \d129        ; 129 81 (missing) For transparency, these are not mapped.
\0 \d130        ; 130 82 (missing)
\0 \d131        ; 131 83 (missing) 4
\0 \d132        ; 132 84 (missing)
\0 \d133        ; 133 85 (missing)
\0 \d134        ; 134 86 (missing)
\0 \d135        ; 135 87 (missing) 8
\0 \d136        ; 136 88 (missing)
\0 \d137        ; 137 89 (missing)
\0 \d138        ; 138 8A (missing)
\0 \d139        ; 139 8B (missing) 12
\0 \d140        ; 140 8C (missing)
\0 \d141        ; 141 8D (missing)
\0 \d142        ; 142 8E (missing)
\0 \d143        ; 143 8F (missing) 16
\0 \d144        ; 144 90 (missing)
\0 \d145        ; 145 91 (missing)
\0 \d146        ; 146 92 (missing)
\0 \d147        ; 147 93 (missing) 20
\0 \d148        ; 148 94 (missing)
\0 \d149        ; 149 95 (missing)
\0 \d150        ; 150 96 (missing)
\0 \d151        ; 151 97 (missing) 24
\0 \d152        ; 152 98 (missing)
\0 \d153        ; 153 99 (missing)
\0 \d154        ; 154 9A (missing)
\0 \d155        ; 155 9B (missing) 28
\0 \d156        ; 156 9C (missing)
\0 \d157        ; 157 9D (missing)
\0 \d158        ; 158 9E (missing)
\0 \d159        ; 159 9F (missing) 32
\0 \xFF         ; 160 A0 no-break space
\0 \xAD         ; 161 A1 inverted exclamation mark
\0 \xBD         ; 162 A2 cent sign
\0 \x9C         ; 163 A3 pound sign
\0 \xCF         ; 164 A4 currency sign
\0 \xBE         ; 165 A5 yen sign
\0 \xDD         ; 166 A6 broken bar
\0 \xF5         ; 167 A7 section sign
\0 \xF9         ; 168 A8 diaeresis
\0 \xB8         ; 169 A9 copyright sign
\0 \xA6         ; 170 AA feminine ordinal indicator
\0 \xAE         ; 171 AB left-pointing double angle quotation mark
\0 \xAA         ; 172 AC not sign
\0 \xF0         ; 173 AD soft hyphen
\0 \xA9         ; 174 AE registered sign
\0 \xEE         ; 175 AF overline
\0 \xF8         ; 176 B0 degree sign
\0 \xF1         ; 177 B1 plus-minus sign
\0 \xFD         ; 178 B2 superscript two
\0 \xFC         ; 179 B3 superscript three
\0 \xEF         ; 180 B4 acute accent
\0 \xE6         ; 181 B5 micro sign
\0 \xF4         ; 182 B6 pilcrow sign
\0 \xFE         ; 183 B7 middle dot
\0 \xF7         ; 184 B8 cedilla
\0 \xFB         ; 185 B9 superscript one
\0 \xA7         ; 186 BA masculine ordinal indicator
\0 \xAF         ; 187 BB right-pointing double angle quotation mark
\0 \xAC         ; 188 BC vulgar fraction one quarter
\0 \xAB         ; 189 BD vulgar fraction one half
\0 \xF3         ; 190 BE vulgar fraction three quarters
\0 \xA8         ; 191 BF inverted question mark
\0 \xB7         ; 192 C0 latin capital letter a with grave
\0 \xB5         ; 193 C1 latin capital letter a with acute
\0 \xB6         ; 194 C2 latin capital letter a with circumflex
\0 \xC7         ; 195 C3 latin capital letter a with tilde
\0 \x8E         ; 196 C4 latin capital letter a with diaeresis
\0 \x8F         ; 197 C5 latin capital letter a with ring above
\0 \x92         ; 198 C6 latin capital letter ae
\0 \x80         ; 199 C7 latin capital letter c with cedilla
\0 \xD4         ; 200 C8 latin capital letter e with grave
\0 \x90         ; 201 C9 latin capital letter e with acute
\0 \xD2         ; 202 CA latin capital letter e with circumflex
\0 \xD3         ; 203 CB latin capital letter e with diaeresis
\0 \xDE         ; 204 CC latin capital letter i with grave
\0 \xD6         ; 205 CD latin capital letter i with acute
\0 \xD7         ; 206 CE latin capital letter i with circumflex
\0 \xD8         ; 207 CF latin capital letter i with diaeresis
\0 \xD1         ; 208 D0 latin capital letter eth (icelandic)
\0 \xA5         ; 209 D1 latin capital letter n with tilde
\0 \xE3         ; 210 D2 latin capital letter o with grave
\0 \xE0         ; 211 D3 latin capital letter o with acute
\0 \xE2         ; 212 D4 latin capital letter o with circumflex
\0 \xE5         ; 213 D5 latin capital letter o with tilde
\0 \x99         ; 214 D6 latin capital letter o with diaeresis
\0 \x9E         ; 215 D7 multiplication sign
\0 \x9D         ; 216 D8 latin capital letter o with stroke
\0 \xEB         ; 217 D9 latin capital letter u with grave
\0 \xE9         ; 218 DA latin capital letter u with acute
\0 \xEA         ; 219 DB latin capital letter u with circumflex
\0 \x9A         ; 220 DC latin capital letter u with diaeresis
\0 \xED         ; 221 DD latin capital letter y with acute
\0 \xE7         ; 222 DE latin capital letter thorn (icelandic)
\0 \xE1         ; 223 DF latin small letter sharp s (german)
\0 \x85         ; 224 E0 latin small letter a with grave
\0 \xA0         ; 225 E1 latin small letter a with acute
\0 \x83         ; 226 E2 latin small letter a with circumflex
\0 \xC6         ; 227 E3 latin small letter a with tilde
\0 \x84         ; 228 E4 latin small letter a with diaeresis
\0 \x86         ; 229 E5 latin small letter a with ring above
\0 \x91         ; 230 E6 latin small letter ae
\0 \x87         ; 231 E7 latin small letter c with cedilla
\0 \x8A         ; 232 E8 latin small letter e with grave
\0 \x82         ; 233 E9 latin small letter e with acute
\0 \x88         ; 234 EA latin small letter e with circumflex
\0 \x89         ; 235 EB latin small letter e with diaeresis
\0 \x8D         ; 236 EC latin small letter i with grave
\0 \xA1         ; 237 ED latin small letter i with acute
\0 \x8C         ; 238 EE latin small letter i with circumflex
\0 \x8B         ; 239 EF latin small letter i with diaeresis
\0 \xD0         ; 240 F0 latin small letter eth (icelandic)
\0 \xA4         ; 241 F1 latin small letter n with tilde
\0 \x95         ; 242 F2 latin small letter o with grave
\0 \xA2         ; 243 F3 latin small letter o with acute
\0 \x93         ; 244 F4 latin small letter o with circumflex
\0 \xE4         ; 245 F5 latin small letter o with tilde
\0 \x94         ; 246 F6 latin small letter o with diaeresis
\0 \xF6         ; 247 F7 division sign
\0 \x9B         ; 248 F8 latin small letter o with stroke
\0 \x97         ; 249 F9 latin small letter u with grave
\0 \xA3         ; 250 FA latin small letter u with acute
\0 \x96         ; 251 FB latin small letter u with circumflex
\0 \x81         ; 252 FC latin small letter u with diaeresis
\0 \xEC         ; 253 FD latin small letter y with acute
\0 \xE8         ; 254 FE latin small letter thorn (icelandic)
\0 \x98         ; 255 FF latin small letter y with diaeresis
END
