;
; This file is a charset conversion module in text form.
;
; This module Converts IBM CP850 to IBM CP850. (no conversion)
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
CP850           ; from set
CP850           ; to set
;
\0 \d128        ; 128 80 latin capital letter c with cedilla
\0 \d129        ; 129 81 latin small letter u with diaeresis
\0 \d130        ; 130 82 latin small letter e with acute
\0 \d131        ; 131 83 latin small letter a with circumflex
\0 \d132        ; 132 84 latin small letter a with diaeresis
\0 \d133        ; 133 85 latin small letter a with grave
\0 \d134        ; 134 86 latin small letter a with ring above
\0 \d135        ; 135 87 latin small letter c with cedilla
\0 \d136        ; 136 88 latin small letter e with circumflex
\0 \d137        ; 137 89 latin small letter e with diaeresis
\0 \d138        ; 138 8A latin small letter e with grave
\0 \d139        ; 139 8B latin small letter i with diaeresis
\0 \d140        ; 140 8C latin small letter i with circumflex
\0 \d141        ; 141 8D latin small letter i with grave
\0 \d142        ; 142 8E latin capital letter a with diaeresis
\0 \d143        ; 143 8F latin capital letter a with ring above
\0 \d144        ; 144 90 latin capital letter e with acute
\0 \d145        ; 145 91 latin small letter ae
\0 \d146        ; 146 92 latin capital letter ae
\0 \d147        ; 147 93 latin small letter o with circumflex
\0 \d148        ; 148 94 latin small letter o with diaeresis
\0 \d149        ; 149 95 latin small letter o with grave
\0 \d150        ; 150 96 latin small letter u with circumflex
\0 \d151        ; 151 97 latin small letter u with grave
\0 \d152        ; 152 98 latin small letter y with diaeresis
\0 \d153        ; 153 99 latin capital letter o with diaeresis
\0 \d154        ; 154 9A latin capital letter u with diaeresis
\0 \d155        ; 155 9B latin small letter o with stroke
\0 \d156        ; 156 9C pound sign
\0 \d157        ; 157 9D latin capital letter o with stroke
\0 \d158        ; 158 9E multiplication sign
\0 \d159        ; 159 9F dutch guilder sign (ibm437 159)
\0 \d160        ; 160 A0 latin small letter a with acute
\0 \d161        ; 161 A1 latin small letter i with acute
\0 \d162        ; 162 A2 latin small letter o with acute
\0 \d163        ; 163 A3 latin small letter u with acute
\0 \d164        ; 164 A4 latin small letter n with tilde
\0 \d165        ; 165 A5 latin capital letter n with tilde
\0 \d166        ; 166 A6 feminine ordinal indicator
\0 \d167        ; 167 A7 masculine ordinal indicator
\0 \d168        ; 168 A8 inverted question mark
\0 \d169        ; 169 A9 registered sign
\0 \d170        ; 170 AA not sign
\0 \d171        ; 171 AB vulgar fraction one half
\0 \d172        ; 172 AC vulgar fraction one quarter
\0 \d173        ; 173 AD inverted exclamation mark
\0 \d174        ; 174 AE left-pointing double angle quotation mark
\0 \d175        ; 175 AF right-pointing double angle quotation mark
\0 \d176        ; 176 B0 light shade
\0 \d177        ; 177 B1 medium shade
\0 \d178        ; 178 B2 dark shade
\0 \d179        ; 179 B3 box drawings light vertical
\0 \d180        ; 180 B4 box drawings light vertical and left
\0 \d181        ; 181 B5 latin capital letter a with acute
\0 \d182        ; 182 B6 latin capital letter a with circumflex
\0 \d183        ; 183 B7 latin capital letter a with grave
\0 \d184        ; 184 B8 copyright sign
\0 \d185        ; 185 B9 box drawings heavy vertical and left
\0 \d186        ; 186 BA box drawings heavy vertical
\0 \d187        ; 187 BB box drawings heavy down and left
\0 \d188        ; 188 BC box drawings heavy up and left
\0 \d189        ; 189 BD cent sign
\0 \d190        ; 190 BE yen sign
\0 \d191        ; 191 BF box drawings light down and left
\0 \d192        ; 192 C0 box drawings light up and right
\0 \d193        ; 193 C1 box drawings light up and horizontal
\0 \d194        ; 194 C2 box drawings light down and horizontal
\0 \d195        ; 195 C3 box drawings light vertical and right
\0 \d196        ; 196 C4 box drawings light horizontal
\0 \d197        ; 197 C5 box drawings light vertical and horizontal
\0 \d198        ; 198 C6 latin small letter a with tilde
\0 \d199        ; 199 C7 latin capital letter a with tilde
\0 \d200        ; 200 C8 box drawings heavy up and right
\0 \d201        ; 201 C9 box drawings heavy down and right
\0 \d202        ; 202 CA box drawings heavy up and horizontal
\0 \d203        ; 203 CB box drawings heavy down and horizontal
\0 \d204        ; 204 CC box drawings heavy vertical and right
\0 \d205        ; 205 CD box drawings heavy horizontal
\0 \d206        ; 206 CE box drawings heavy vertical and horizontal
\0 \d207        ; 207 CF currency sign
\0 \d208        ; 208 D0 latin small letter eth (icelandic)
\0 \d209        ; 209 D1 latin capital letter eth (icelandic)
\0 \d210        ; 210 D2 latin capital letter e with circumflex
\0 \d211        ; 211 D3 latin capital letter e with diaeresis
\0 \d212        ; 212 D4 latin capital letter e with grave
\0 \d213        ; 213 D5 latin small letter i dotless
\0 \d214        ; 214 D6 latin capital letter i with acute
\0 \d215        ; 215 D7 latin capital letter i with circumflex
\0 \d216        ; 216 D8 latin capital letter i with diaeresis
\0 \d217        ; 217 D9 box drawings light up and left
\0 \d218        ; 218 DA box drawings light down and right
\0 \d219        ; 219 DB full block
\0 \d220        ; 220 DC lower half block
\0 \d221        ; 221 DD broken bar
\0 \d222        ; 222 DE latin capital letter i with grave
\0 \d223        ; 223 DF upper half block
\0 \d224        ; 224 E0 latin capital letter o with acute
\0 \d225        ; 225 E1 latin small letter sharp s (german)
\0 \d226        ; 226 E2 latin capital letter o with circumflex
\0 \d227        ; 227 E3 latin capital letter o with grave
\0 \d228        ; 228 E4 latin small letter o with tilde
\0 \d229        ; 229 E5 latin capital letter o with tilde
\0 \d230        ; 230 E6 greek small letter mu
\0 \d231        ; 231 E7 latin capital letter thorn (icelandic)
\0 \d232        ; 232 E8 latin small letter thorn (icelandic)
\0 \d233        ; 233 E9 latin capital letter u with acute
\0 \d234        ; 234 EA latin capital letter u with circumflex
\0 \d235        ; 235 EB latin capital letter u with grave
\0 \d236        ; 236 EC latin small letter y with acute
\0 \d237        ; 237 ED latin capital letter y with acute
\0 \d238        ; 238 EE em dash
\0 \d239        ; 239 EF acute accent
\0 \d240        ; 240 F0 soft hyphen
\0 \d241        ; 241 F1 plus-minus sign
\0 \d242        ; 242 F2 left right double arrow
\0 \d243        ; 243 F3 vulgar fraction three quarters
\0 \d244        ; 244 F4 pilcrow sign
\0 \d245        ; 245 F5 section sign
\0 \d246        ; 246 F6 division sign
\0 \d247        ; 247 F7 ogonek
\0 \d248        ; 248 F8 degree sign
\0 \d249        ; 249 F9 diaeresis
\0 \d250        ; 250 FA dot above
\0 \d251        ; 251 FB superscript one
\0 \d252        ; 252 FC superscript three
\0 \d253        ; 253 FD superscript two
\0 \d254        ; 254 FE black square
\0 \d255        ; 255 FF no-break space
END
