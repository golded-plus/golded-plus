#!/bin/sh
# vim:set sw=4 ts=8 fileencoding=utf8::Кодировка:UTF-8[АБЁЪЯабёъя] 
#
#   Авторское право (с) 2015, Сергей Леонтьев, leo@sai.msu.ru
#
#   Разрешается повторное распространение и использование как в виде
#   исходника так и в двоичной форме, с модификациями или без, при
#   соблюдении следующих условий:
#
#   1. При повторном распространении исходного кода должно оставаться
#   указанное выше уведомление об авторском праве, этот список условий и
#   нижеследующий отказ от гарантий.
#
#   2. При повторном распространении двоичного кода должно
#   воспроизводиться указанное выше уведомление об авторском праве, этот
#   список условий и нижеследующий отказ от гарантий в документации
#   и/или в других материалах, поставляемых при распространении.
#
#   3. Ни название Крипто-Про, ни имена его сотрудников не могут быть
#   использованы в качестве поддержки или продвижения продуктов,
#   основанных на этом ПО без предварительного письменного разрешения.
#
#   ЭТО ПРОГРАММА ПРЕДОСТАВЛЕНА БЕСПЛАТНО ДЕРЖАТЕЛЯМИ АВТОРСКИХ ПРАВ
#   И/ИЛИ ДРУГИМИ СТОРОНАМИ "КАК ОНА ЕСТЬ" БЕЗ КАКОГО-ЛИБО ВИДА
#   ГАРАНТИЙ, ВЫРАЖЕННЫХ ЯВНО ИЛИ ПОДРАЗУМЕВАЕМЫХ, ВКЛЮЧАЯ, НО НЕ
#   ОГРАНИЧИВАЯСЬ ИМИ, ПОДРАЗУМЕВАЕМЫЕ ГАРАНТИИ КОММЕРЧЕСКОЙ ЦЕННОСТИ И
#   ПРИГОДНОСТИ ДЛЯ КОНКРЕТНОЙ ЦЕЛИ. НИ В КОЕМ СЛУЧАЕ, ЕСЛИ НЕ ТРЕБУЕТСЯ
#   СООТВЕТСТВУЮЩИМ ЗАКОНОМ, ИЛИ НЕ УСТАНОВЛЕНО В УСТНОЙ ФОРМЕ, НИ ОДИН
#   ДЕРЖАТЕЛЬ АВТОРСКИХ ПРАВ И НИ ОДНО ДРУГОЕ ЛИЦО, КОТОРОЕ МОЖЕТ
#   ИЗМЕНЯТЬ И/ИЛИ ПОВТОРНО РАСПРОСТРАНЯТЬ ПРОГРАММУ, КАК БЫЛО РАЗРЕШЕНО
#   ВЫШЕ, НЕ ОТВЕТСТВЕННЫ ПЕРЕД ВАМИ ЗА УБЫТКИ, ВКЛЮЧАЯ ЛЮБЫЕ ОБЩИЕ,
#   СЛУЧАЙНЫЕ, СПЕЦИАЛЬНЫЕ ИЛИ ПОСЛЕДОВАВШИЕ УБЫТКИ, ПРОИСТЕКАЮЩИЕ ИЗ
#   ИСПОЛЬЗОВАНИЯ ИЛИ НЕВОЗМОЖНОСТИ ИСПОЛЬЗОВАНИЯ ПРОГРАММЫ (ВКЛЮЧАЯ, НО
#   НЕ ОГРАНИЧИВАЯСЬ ПОТЕРЕЙ ДАННЫХ, ИЛИ ДАННЫМИ, СТАВШИМИ
#   НЕПРАВИЛЬНЫМИ, ИЛИ ПОТЕРЯМИ ПРИНЕСЕННЫМИ ИЗ-ЗА ВАС ИЛИ ТРЕТЬИХ ЛИЦ,
#   ИЛИ ОТКАЗОМ ПРОГРАММЫ РАБОТАТЬ СОВМЕСТНО С ДРУГИМИ ПРОГРАММАМИ),
#   ДАЖЕ ЕСЛИ ТАКОЙ ДЕРЖАТЕЛЬ ИЛИ ДРУГОЕ ЛИЦО БЫЛИ ИЗВЕЩЕНЫ О
#   ВОЗМОЖНОСТИ ТАКИХ УБЫТКОВ.
#

#
# Макет настройки Alt-символов Golded+ для фонетической клавиатуры ЯВЕРТЫ
#

ge="${1:-ge}"
gearg="$2"

koi8rxterm -xrm '*VT100.translations:     #override \n\
	    Shift <KeyPress> Prior: string(0x1b) string("[5;2~") \n\
	    Shift <KeyPress> Next:  string(0x1b) string("[6;2~") \n\
	    Alt <Key>Cyrillic_io:	string(0x1b) string("`") \n\
	    Alt <Key>Cyrillic_hardsign:	string(0x1b) string("=") \n\
	    Alt <Key>Cyrillic_ya:	string(0x1b) string("q") \n\
	    Alt <Key>Cyrillic_ve:	string(0x1b) string("w") \n\
	    Alt <Key>Cyrillic_ie:	string(0x1b) string("e") \n\
	    Alt <Key>Cyrillic_er:	string(0x1b) string("r") \n\
	    Alt <Key>Cyrillic_te:	string(0x1b) string("t") \n\
	    Alt <Key>Cyrillic_yeru:	string(0x1b) string("y") \n\
	    Alt <Key>Cyrillic_u:	string(0x1b) string("u") \n\
	    Alt <Key>Cyrillic_i:	string(0x1b) string("i") \n\
	    Alt <Key>Cyrillic_o:	string(0x1b) string("o") \n\
	    Alt <Key>Cyrillic_pe:	string(0x1b) string("p") \n\
	    Alt <Key>Cyrillic_sha:	string(0x1b) string("[") \n\
	    Alt <Key>Cyrillic_shcha:	string(0x1b) string("]") \n\
	    Alt <Key>Cyrillic_e:	string(0x1b) string("\") \n\
	    Alt <Key>Cyrillic_a:	string(0x1b) string("a") \n\
	    Alt <Key>Cyrillic_es:	string(0x1b) string("s") \n\
	    Alt <Key>Cyrillic_de:	string(0x1b) string("d") \n\
	    Alt <Key>Cyrillic_ef:	string(0x1b) string("f") \n\
	    Alt <Key>Cyrillic_ghe:	string(0x1b) string("g") \n\
	    Alt <Key>Cyrillic_ha:	string(0x1b) string("h") \n\
	    Alt <Key>Cyrillic_shorti:	string(0x1b) string("j") \n\
	    Alt <Key>Cyrillic_ka:	string(0x1b) string("k") \n\
	    Alt <Key>Cyrillic_el:	string(0x1b) string("l") \n\
	    Alt <Key>Cyrillic_che:	string(0x1b) string(";") \n\
	    Alt <Key>Cyrillic_yu:	string(0x1b) string("'"'"'") \n\
	    Alt <Key>Cyrillic_ze:	string(0x1b) string("z") \n\
	    Alt <Key>Cyrillic_softsign:	string(0x1b) string("x") \n\
	    Alt <Key>Cyrillic_tse:	string(0x1b) string("c") \n\
	    Alt <Key>Cyrillic_zhe:	string(0x1b) string("v") \n\
	    Alt <Key>Cyrillic_be:	string(0x1b) string("b") \n\
	    Alt <Key>Cyrillic_en:	string(0x1b) string("n") \n\
	    Alt <Key>Cyrillic_em:	string(0x1b) string("m")    '\
	-geometry 130x33 -e ${ge} ${gearg}

	#-xrm '*altIsNotMeta: true' \
	#-xrm '*altSendsEscape: true' \
