;
; This file is a charset conversion module in text form.
;
; This module Converts LATIN-1 to LATIN-1. (no conversion)
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
LATIN-1		; from set
LATIN-1     ; to set
;
\0 \d128
\0 \d129
\0 \d130
\0 \d131
\0 \d132
\0 \d133
\0 \d134
\0 \d135
\0 \d136
\0 \d137
\0 \d138
\0 \d139
\0 \d140
\0 \d141
\0 \d142
\0 \d143
\0 \d144
\0 \d145
\0 \d146
\0 \d147
\0 \d148
\0 \d149
\0 \d150
\0 \d151
\0 \d152
\0 \d153
\0 \d154
\0 \d155
\0 \d156
\0 \d157
\0 \d158
\0 \d159
\0 \d160
\0 \d161
\0 \d162
\0 \d163
\0 \d164
\0 \d165
\0 \d166
\0 \d167
\0 \d168
\0 \d169
\0 \d170
\0 \d171
\0 \d172
\0 \d173
\0 \d174
\0 \d175
\0 \d176
\0 \d177
\0 \d178
\0 \d179
\0 \d180
\0 \d181
\0 \d182
\0 \d183
\0 \d184
\0 \d185
\0 \d186
\0 \d187
\0 \d188
\0 \d189
\0 \d190
\0 \d191
\0 \d192
\0 \d193
\0 \d194
\0 \d195
\0 \d196
\0 \d197
\0 \d198
\0 \d199
\0 \d200
\0 \d201
\0 \d202
\0 \d203
\0 \d204
\0 \d205
\0 \d206
\0 \d207
\0 \d208
\0 \d209
\0 \d210
\0 \d211
\0 \d212
\0 \d213
\0 \d214
\0 \d215
\0 \d216
\0 \d217
\0 \d218
\0 \d219
\0 \d220
\0 \d221
\0 \d222
\0 \d223
\0 \d224
\0 \d225
\0 \d226
\0 \d227
\0 \d228
\0 \d229
\0 \d230
\0 \d231
\0 \d232
\0 \d233
\0 \d234
\0 \d235
\0 \d236
\0 \d237
\0 \d238
\0 \d239
\0 \d240
\0 \d241
\0 \d242
\0 \d243
\0 \d244
\0 \d245
\0 \d246
\0 \d247
\0 \d248
\0 \d249
\0 \d250
\0 \d251
\0 \d252
\0 \d253
\0 \d254
\0 \d255
END
