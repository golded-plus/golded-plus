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
# Построение ресурсов для Apple Terminal и iTerm по БД терминала
# terminfo
#

file_version="make-term-profiles.sh, v 1, 18-11-2015"

########################################################################
# Разбор аргументов и т.д.
########################################################################

set -e
prog=$(basename "$0")

usage() {
    echo "$prog: Usage:
    	$prog [<terminal> [<terminfo source>]]" 1>&2
    exit 1
}

if [ $# -gt 3 ] ; then
    usage
fi

term="${1:-$TERM}"
tisrc="$2"

td=$(mktemp -d -t "mtp-XXXXXX")
echo create "$td"
trap 'echo remove "$td"; rm -r "$td"; exit 3' EXIT HUP INT TERM

########################################################################
# Все строки в файле в UTF-8
########################################################################

LANG="ru_RU.UTF-8"; export LANG
LC_ALL="ru_RU.UTF-8"; export LC_ALL
unset LC_COLLATE LC_CTYPE LC_MESSAGES LC_MONETARY LC_NUMERIC LC_TIME


########################################################################
# Получаем коды клавиш и коды кодировок от OS X
########################################################################

OSXF=/System/Library/Frameworks
SHDR=framework/Versions/Current/Headers
NSString_h="$OSXF/Foundation.$SHDR/NSString.h"
CFStringEncodingExt_h="$OSXF/CoreFoundation.$SHDR/CFStringEncodingExt.h"
NSEvent_h="$OSXF/AppKit.$SHDR/NSEvent.h"
NSText_h="$OSXF/AppKit.$SHDR/NSText.h"

get_value() {
    sed -n \
	"/[[:space:]]*$1[[:space:]]*=[[:space:]]*\([0-9xa-fXA-F]*\).*/s//\1/p" \
	"$2"
}

get_const() {
    eval "$1"="$(get_value $1 $2)"
}

get_const NSDeleteCharacter "$NSText_h"
get_const NSTabCharacter "$NSText_h"
get_const NSBackTabCharacter "$NSText_h" # <Shift>+<Tab> - это клавиша!
get_const NSNewlineCharacter "$NSText_h"

    #
    # Смотри также:
    # <http://www.unicode.org/Public/MAPPINGS/VENDORS/APPLE/CORPCHAR.TXT>
    #
get_const NSUpArrowFunctionKey "$NSEvent_h"
get_const NSDownArrowFunctionKey "$NSEvent_h"
get_const NSLeftArrowFunctionKey "$NSEvent_h"
get_const NSRightArrowFunctionKey "$NSEvent_h"

get_const NSF1FunctionKey "$NSEvent_h"
get_const NSF2FunctionKey "$NSEvent_h"
get_const NSF3FunctionKey "$NSEvent_h"
get_const NSF4FunctionKey "$NSEvent_h"
get_const NSF5FunctionKey "$NSEvent_h"
get_const NSF6FunctionKey "$NSEvent_h"
get_const NSF7FunctionKey "$NSEvent_h"
get_const NSF8FunctionKey "$NSEvent_h"
get_const NSF9FunctionKey "$NSEvent_h"
get_const NSF10FunctionKey "$NSEvent_h"
get_const NSF11FunctionKey "$NSEvent_h"
get_const NSF12FunctionKey "$NSEvent_h"

get_const NSF13FunctionKey "$NSEvent_h"
get_const NSF14FunctionKey "$NSEvent_h"
get_const NSF15FunctionKey "$NSEvent_h"

get_const NSF16FunctionKey "$NSEvent_h"
get_const NSF17FunctionKey "$NSEvent_h"
get_const NSF18FunctionKey "$NSEvent_h"
get_const NSF19FunctionKey "$NSEvent_h"

#get_const NSF20FunctionKey "$NSEvent_h" # Not on most Macintosh keyboards
#get_const NSF21FunctionKey "$NSEvent_h" # Not on most Macintosh keyboards
#get_const NSF22FunctionKey "$NSEvent_h" # Not on most Macintosh keyboards
#get_const NSF23FunctionKey "$NSEvent_h" # Not on most Macintosh keyboards
#get_const NSF24FunctionKey "$NSEvent_h" # Not on most Macintosh keyboards
#get_const NSF25FunctionKey "$NSEvent_h" # Not on most Macintosh keyboards
#get_const NSF26FunctionKey "$NSEvent_h" # Not on most Macintosh keyboards
#get_const NSF27FunctionKey "$NSEvent_h" # Not on most Macintosh keyboards
#get_const NSF28FunctionKey "$NSEvent_h" # Not on most Macintosh keyboards
#get_const NSF29FunctionKey "$NSEvent_h" # Not on most Macintosh keyboards
#get_const NSF30FunctionKey "$NSEvent_h" # Not on most Macintosh keyboards
#get_const NSF31FunctionKey "$NSEvent_h" # Not on most Macintosh keyboards
#get_const NSF32FunctionKey "$NSEvent_h" # Not on most Macintosh keyboards
#get_const NSF33FunctionKey "$NSEvent_h" # Not on most Macintosh keyboards
#get_const NSF34FunctionKey "$NSEvent_h" # Not on most Macintosh keyboards
#get_const NSF35FunctionKey "$NSEvent_h" # Not on most Macintosh keyboards

#get_const NSInsertFunctionKey "$NSEvent_h" # Not on most Macintosh keyboards
get_const NSDeleteFunctionKey "$NSEvent_h" # Forward Delete key
get_const NSHomeFunctionKey "$NSEvent_h"
#get_const NSBeginFunctionKey "$NSEvent_h" # Not on most Macintosh keyboards
get_const NSEndFunctionKey "$NSEvent_h"
get_const NSPageUpFunctionKey "$NSEvent_h"
get_const NSPageDownFunctionKey "$NSEvent_h"

#get_const NSPrintScreenFunctionKey "$NSEvent_h" # Not on most Macintosh 
#get_const NSScrollLockFunctionKey "$NSEvent_h" # Not on most Macintosh 
#get_const NSPauseFunctionKey "$NSEvent_h" # Not on most Macintosh keyboards
#get_const NSSysReqFunctionKey "$NSEvent_h" # Not on most Macintosh keyboards
#get_const NSBreakFunctionKey "$NSEvent_h" # Not on most Macintosh keyboards
#get_const NSResetFunctionKey "$NSEvent_h" # Not on most Macintosh keyboards
#get_const NSStopFunctionKey "$NSEvent_h" # Not on most Macintosh keyboards
#get_const NSMenuFunctionKey "$NSEvent_h" # Not on most Macintosh keyboards
#get_const NSUserFunctionKey "$NSEvent_h" # Not on most Macintosh keyboards
#get_const NSSystemFunctionKey "$NSEvent_h" # Not on most Macintosh keyboards
#get_const NSPrintFunctionKey "$NSEvent_h" # Not on most Macintosh keyboards

get_const NSClearLineFunctionKey "$NSEvent_h" # Clear/Num Lock key.
#get_const NSClearDisplayFunctionKey "$NSEvent_h" # Not on most Macintosh 
#get_const NSInsertLineFunctionKey "$NSEvent_h" # Not on most Macintosh 
#get_const NSDeleteLineFunctionKey "$NSEvent_h" # Not on most Macintosh 
#get_const NSInsertCharFunctionKey "$NSEvent_h" # Not on most Macintosh 
#get_const NSDeleteCharFunctionKey "$NSEvent_h" # Not on most Macintosh 
#get_const NSPrevFunctionKey "$NSEvent_h" # Not on most Macintosh keyboards
#get_const NSNextFunctionKey "$NSEvent_h" # Not on most Macintosh keyboards
#get_const NSSelectFunctionKey "$NSEvent_h" # Not on most Macintosh keyboards
#get_const NSExecuteFunctionKey "$NSEvent_h" # Not on most Macintosh keyboards
#get_const NSUndoFunctionKey "$NSEvent_h" # Not on most Macintosh keyboards
#get_const NSRedoFunctionKey "$NSEvent_h" # Not on most Macintosh keyboards
#get_const NSFindFunctionKey "$NSEvent_h" # Not on most Macintosh keyboards
get_const NSHelpFunctionKey "$NSEvent_h"
#get_const NSModeSwitchFunctionKey "$NSEvent_h" # Not on most Macintosh 

get_const NSUTF8StringEncoding "$NSString_h"

get_const kCFStringEncodingKOI8_R "$CFStringEncodingExt_h"
get_const kCFStringEncodingKOI8_U "$CFStringEncodingExt_h"

nsKOI8_R=$((kCFStringEncodingKOI8_R + 0x80000000))
nsKOI8_U=$((kCFStringEncodingKOI8_U + 0x80000000))
#
# Правда в fido7.mac.creature писали, что у русской кодировки ноги
# растут от kTextEncodingKOI8_R из TextCommon.h
# Date: Thu, 09 Mar 2006 11:33:20 +0300
# From: Ivan Kuvshinov <Ivan.Kuvshinov@p10110.f830.n5020.z2.fidonet.org>
# Subject: Русская раскладка в ОСИкс [0/0].
# Message-ID: <1141904373@p10110.f830.n5020.z2.FidoNet.ftn>
#

#printf "NSUpArrowFunctionKey %4X\n" $NSUpArrowFunctionKey
#printf "NSSystemFunctionKey %4X\n" $NSSystemFunctionKey
#printf "NSModeSwitchFunctionKey %4X\n" $NSModeSwitchFunctionKey
#printf "UTF-8: %d 0x%08x\n" $NSUTF8StringEncoding $NSUTF8StringEncoding
#printf "KOI8-R: %d 0x%08x\n" $nsKOI8_R $nsKOI8_R
#printf "KOI8-U: %d 0x%08x\n" $nsKOI8_U $nsKOI8_U
#set | grep -i ns


########################################################################
# Настраиваем БД терминалов terminfo
########################################################################

    # Биты маски DECFNK 
control=4
option=2
shift=1

if [ -n "$tisrc" ] ; then
    # http://invisible-island.net/ncurses/ncurses.faq.html#which_terminfo
    tic -x -o "$td" "$tisrc"
    TERMINFO="$td" ; export TERMINFO
    terminfo_version="$tisrc, "$(sed -n \
	    '/#.*\$[[:space:]]*\([DR].*\)\$/s//\1/p' "$tisrc")
else
    terminfo_version=$(uname -sr)", "$(type infocmp)", "$(infocmp -V)
fi
TERM="$term"; export TERM

hex() {
    echo 0x$(echo $(printf "$1" | iconv -t UCS-2LE | od -t x2 | cut -c 8-))
}

dcomm() {
    defaults write "$plist" "commentGenerator" \
    		-string "Generated by $file_version"
    defaults write "$plist" "commentCreator" \
    		-string "Created by $(whoami) at $(LANG=C LC_ALL=C date)"
    defaults write "$plist" "commentTerminfoVersion" -string "$1"
    defaults write "$plist" "commentTerminal" -string "$2"
}

dkey() {
    ms=
    if [ 0 -ne $((($1 - 1)&$control)) ]; then 
	ms="${ms}^"
    fi
    if [ 0 -ne $((($1 - 1)&$option)) ]; then 
	ms="${ms}~"
    fi
    if [ 0 -ne $((($1 - 1)&$shift)) ]; then 
	ms="${ms}\$"
    fi
    s=$(printf "%s%04X" "$ms" "$2")
    defaults write "$plist" "keyMapBoundKeys" -dict-add "$s" '"'"$3"'"'
}

dkey1() {
    tk=$(tput "$3" 2> /dev/null) && {
	echo "$3" >> "$td/tinfo-key"
	dkey "$1" "$2" "$tk"
    }
    return 0
}

dkey1esc() {
    tk=$(tput "$3" 2> /dev/null) && {
	dkey "$1" "$2" $(printf "\033")"$tk"
    }
    return 0
}

dkey3() {
    dkey1 1 "$1" "$2"
    if [ $# -gt 2 ] ; then
	dkey1 2 "$1" "$3"
	if [ $# -gt 3 ] ; then
	    mkn="$4"
	    dkey1 2 "$1" "$mkn"
	else
	    mkn="$3"
	fi
	for m in 3 4 5 6 7 8 9 10 11 12 13 14 15 16 ; do
	    dkey1 "$m" "$1" "$mkn$m"
	done 
    fi
    return 0
}

dkeyboard() {
    dkey3 $NSUpArrowFunctionKey    kcuu1 kri  kUP 
    dkey3 $NSDownArrowFunctionKey  kcud1 kind kDN
    dkey3 $NSLeftArrowFunctionKey  kcub1 kLFT
    dkey3 $NSRightArrowFunctionKey kcuf1 kRIT 
    dkey3 $NSDeleteFunctionKey     kdch1 kDC
    dkey3 $NSHomeFunctionKey       khome kHOM
    dkey3 $NSEndFunctionKey        kend  kEND
    dkey3 $NSPageUpFunctionKey     kpp   kPRV
    dkey3 $NSPageDownFunctionKey   knp   kNXT
    dkey3 $NSClearLineFunctionKey  kclr
    dkey3 $NSClearLineFunctionKey  kel   kEOL 
    dkey3 $NSHelpFunctionKey       khlp  kHLP 

	# <control>+<option>+I => Ins для узких клавиатур
    dkey1 $(($control + $option + 1)) $(hex i) kich1
    dkey1 $(($control + $option + $shift + 1)) $(hex i) kIC
    dkey3 $NSF13FunctionKey       kich1  kIC	# F13 => Ins для широких клав
     
    dkey3 $NSHomeFunctionKey      ka1
    dkey3 $NSPageUpFunctionKey    ka3
    dkey3 $NSEndFunctionKey       kc1
    dkey3 $NSPageDownFunctionKey  kc3

    #dkey3 $NSDeleteCharacter      kbs
    #dkey3 $NSNewlineCharacter     kent
    dkey3 $NSBackTabCharacter     kcbt

    dkey1esc $(($option + 1)) $NSDeleteCharacter      kbs
    dkey1esc $(($option + 1)) $NSDeleteFunctionKey    kent
    dkey1esc $(($option + 1)) $NSBackTabCharacter     kcbt

    n=0; while [ $n -lt 12 ] ; do n=$(($n + 1))
	eval dkey1                    1   \$NSF${n}FunctionKey kf${n}
	eval dkey1 $(($shift         +1)) \$NSF${n}FunctionKey kf$(($n + 12))
	eval dkey1 $(($control       +1)) \$NSF${n}FunctionKey kf$(($n + 24))
	eval dkey1 $(($control+$shift+1)) \$NSF${n}FunctionKey kf$(($n + 36))
	eval dkey1 $(($option        +1)) \$NSF${n}FunctionKey kf$(($n + 48))
	eval dkey1 $(($option +$shift+1)) \$NSF${n}FunctionKey kf$(($n + 60))
    done

    dcomm "$terminfo_version" "$(infocmp -x | head -2)"
    echo "Отчёт по terminfo. Последовательности, для которых нет кнопок:"
    touch "$td/tinfo-key"
    sort -u "$td/tinfo-key" > "$td/tinfo-key.sort"
    infocmp -x | sed '
		    /^[^[:space:]]/d
		    s/=[^,]*,/\
		    /g' \
		| sed -n '/^[[:space:]]*k/s//k/p' \
		| sort \
		| comm -3 - "$td/tinfo-key.sort" \
		| grep -v kmous

	# 
	# Имитация altSendsEscape xterm для белорусских, русских и
	# украинских символов
	# Обход ошибок в Apple Terminal и iTerm2
	#
    daltSendsEscape() {
	l=$(printf "\u0%x" $1 | iconv -f C99)
	u=$(printf "%s" "$l" | tr '[[:lower:]]' '[[:upper:]]')
	dkey $(($option          + 1)) $1          "\033$l"
	dkey $(($option + $shift + 1)) $(hex "$u") "\033$u"
    }

    c=$(($(hex "а")))
    while [ $c -le $(($(hex "я"))) ] ; do
	daltSendsEscape $c
	c=$(($c + 1))
    done
    daltSendsEscape $(hex "ё")
    daltSendsEscape $(hex "ў")
    daltSendsEscape $(hex "і")
    daltSendsEscape $(hex "ї")
    daltSendsEscape $(hex "ґ")
    daltSendsEscape $(hex "є")
}

plist="$td"/xterm.plist

cp $(dirname "$0")/xterm.plist-template "$plist"

dkeyboard

for type in strict reverse combined ; do
    case "$type" in
    strict)
	#dkey1 $(($control+1)) $NSLeftArrowFunctionKey  kLFT$(($control+1))
	#dkey1 $(($control+1)) $NSRightArrowFunctionKey kRIT$(($control+1))
	#dkey1 $(($option +1)) $NSLeftArrowFunctionKey  kLFT$(($option +1))
	#dkey1 $(($option +1)) $NSRightArrowFunctionKey kRIT$(($option +1))
    	;;
    reverse)
	dkey1 $(($control+1)) $NSLeftArrowFunctionKey  kLFT$(($option +1))
	dkey1 $(($control+1)) $NSRightArrowFunctionKey kRIT$(($option +1))
	dkey1 $(($option +1)) $NSLeftArrowFunctionKey  kLFT$(($control+1))
	dkey1 $(($option +1)) $NSRightArrowFunctionKey kRIT$(($control+1))
	;;
    combined)
	dkey1 $(($control+1)) $NSLeftArrowFunctionKey  kLFT$(($control+1))
	dkey1 $(($control+1)) $NSRightArrowFunctionKey kRIT$(($control+1))
	dkey1 $(($option +1)) $NSLeftArrowFunctionKey  kLFT$(($control+1))
	dkey1 $(($option +1)) $NSRightArrowFunctionKey kRIT$(($control+1))
	;;
    esac
    for charset in KOI8-R KOI8-U UTF-8 ; do
	defaults write "$plist" "name" -string "XTerm-${charset}-${type}"
	defaults write "$plist" "WindowTitle" -string "XTerm ${charset} ${type}"

	case "$charset" in
	UTF-8)
	    defaults write "$plist" "CharacterEncoding" \
	    		-int $NSUTF8StringEncoding
	    defaults delete "$plist" "CommandString" 2> /dev/null || true
	    ;;
	KOI8-R)
	    defaults write "$plist" "CharacterEncoding" \
	    		-int -$(((1<<32)-$nsKOI8_R))
	    defaults delete "$plist" "CommandString" 2> /dev/null || true
	    ;;
	KOI8-U)
	    defaults write "$plist" "CharacterEncoding" \
	    		-int -$(((1<<32)-$nsKOI8_U))
	    defaults write "$plist" "CommandString" \
			-string 'LANG="ru_RU.KOI8-R" exec -a "-bash" bash -l'
	    ;;
	*)
	    echo Internal Error 1>&2
	    exit 10
	esac

	terminal=$(dirname "$0")/XTerm-"$charset"-"$type".terminal
	plutil -convert xml1 -o - "$plist" | sed \
		-e "/-$(((1<<32)-$nsKOI8_R))/s//$(($nsKOI8_R))/" \
		-e "/-$(((1<<32)-$nsKOI8_U))/s//$(($nsKOI8_U))/" \
	    > "$terminal"

    done
done

