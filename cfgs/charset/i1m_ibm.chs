;
; This file is a charset conversion module in text form.
;
; This module converts ISO-8859-1 extended characters to IBM-PC characters.
; It contains modifications to convert a few MAC characters to IBMPC.
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
LATIN1MAC       ; from set
IBMPC           ; to set
;
\0 \d128        ; 128 (missing) These codes are unused in the LATIN-1 set.
\0 \x8f         ; 129 MAC: A ring
\0 \d130        ; 130 (missing)
\0 \d131        ; 131 (missing) 4
\0 \d132        ; 132 (missing)
\0 \d133        ; 133 (missing)
\0 \d134        ; 134 (missing)
\0 \d135        ; 135 (missing) 8
\0 \d136        ; 136 (missing)
\0 \d137        ; 137 (missing)
\0 \d138        ; 138 (missing)
\0 \d139        ; 139 (missing) 12
\0 \x86         ; 140 MAC: a ring
\0 \d141        ; 141 (missing)
\0 \d142        ; 142 (missing)
\0 \d143        ; 143 (missing) 16
\0 \d144        ; 144 (missing)
\0 \d145        ; 145 (missing)
\0 \d146        ; 146 (missing)
\0 \d147        ; 147 (missing) 20
\0 \d148        ; 148 (missing)
\0 \d149        ; 149 (missing)
\0 \d150        ; 150 (missing)
\0 \d151        ; 151 (missing) 24
\0 \d152        ; 152 (missing)
\0 \d153        ; 153 (missing)
\0 \d154        ; 154 (missing)
\0 \d155        ; 155 (missing) 28
\0 \d156        ; 156 (missing)
\0 \d157        ; 157 (missing)
\0 \d158        ; 158 (missing)
\0 \d159        ; 159 (missing) 32
\0 \xff         ; 160 non-breaking space
\0 \xad         ; 161 exclam downwards
\0 \x9b         ; 162 MAC: o slash
\0 \x9c         ; 163 pound sterling
\0 \x0f         ; 164 currency
\0 \x9d         ; 165 Yen
\0 |            ; 166 broken bar
\0 \x15         ; 167 section
\x1 ?           ; 168 dieresis
\0 c            ; 169 copyright
\0 \xa6         ; 170 ord feminine
\0 \xae         ; 171 guillemot left
\0 \xaa         ; 172 logical not
\0 -            ; 173 soft hyphen (or em dash)
\0 \x92         ; 174 MAC: AE
\x1 ?           ; 175 overbar (macron)
\0 \xf8         ; 176 ring / degree
\0 \xf1         ; 177 plusminus
\0 \xfd         ; 178 superscript two (squared)
^ 3             ; 179 superscript three (cubed)
\0 \x9d         ; 180 MAC: O slash
\0 \xe6         ; 181 mu
\0 \x14         ; 182 paragraph
\0 \xfe         ; 183 bullet
\0 ,            ; 184 cedilla
^ 1             ; 185 superscript one
\0 \xa7         ; 186 ord masculine
\0 \xaf         ; 187 guillemot right
\0 \xac         ; 188 one quarter
\0 \xab         ; 189 half
\0 \x91         ; 190 MAC: ae
\0 \xa8         ; 191 question downwards
\0 A            ; 192 A grave
\0 A            ; 193 A acute
\0 A            ; 194 A circumflex
\0 A            ; 195 A tilde
\0 \x8e         ; 196 A dieresis
\0 \x8f         ; 197 A ring
\0 \x92         ; 198 AE
\0 \x80         ; 199 C cedilla
\0 E            ; 200 E grave
\0 \x90         ; 201 E acute
\0 E            ; 202 E circumflex
E e             ; 203 E dieresis
\0 I            ; 204 I grave
\0 I            ; 205 I acute
\0 I            ; 206 I circumflex
I e             ; 207 I dieresis
\0 D            ; 208 Eth
\0 \xa5         ; 209 N tilde
\0 O            ; 210 O grave
\0 O            ; 211 O acute
\0 O            ; 212 O circumflex
\0 O            ; 213 O tilde
\0 \x99         ; 214 O dieresis
\0 x            ; 215 multiplication
\0 \x9d         ; 216 O slash (mapping for CP850/865)
\0 U            ; 217 U grave
\0 U            ; 218 U acute
\0 U            ; 219 U circumflex
\0 \x9a         ; 220 U dieresis
\0 Y            ; 221 Y acute
\x1 ?           ; 222 Thorn
\0 \xe1         ; 223 german double s / beta
\0 \x85         ; 224 a grave
\0 \xa0         ; 225 a acute
\0 \x83         ; 226 a circumflex
\0 a            ; 227 a tilde
\0 \x84         ; 228 a dieresis
\0 \x86         ; 229 a ring
\0 \x91         ; 230 ae
\0 \x87         ; 231 c cedilla
\0 \x8a         ; 232 e grave
\0 \x82         ; 233 e acute
\0 \x88         ; 234 e circumflex
\0 \x89         ; 235 e dieresis
\0 \x8d         ; 236 i grave
\0 \xa1         ; 237 i acute
\0 \x8c         ; 238 i circumflex
\0 \x8b         ; 239 i dieresis
\0 \xe7         ; 240 eth
\0 \xa4         ; 241 n tilde
\0 \x95         ; 242 o grave
\0 \xa2         ; 243 o acute
\0 \x93         ; 244 o circumflex
\0 o            ; 245 o tilde
\0 \x94         ; 246 o dieresis
\0 \xf6         ; 247 division
\0 \x9b         ; 248 o slash (mapping for CP 850/865)
\0 \x97         ; 249 u grave
\0 \xa3         ; 250 u acute
\0 \x96         ; 251 u circumflex
\0 \x81         ; 252 u dieresis
y e             ; 253 y acute
\x1 ?           ; 254 thorn
\0 \x98         ; 255 y dieresis
END
