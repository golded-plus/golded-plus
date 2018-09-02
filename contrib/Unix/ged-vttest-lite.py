#!/usr/bin/env python 
# vim:set sw=4 ts=8 et fileencoding=utf8::Кодировка:UTF-8[АБЁЪЯабёъя] 
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
# Простое тестирование терминала на соответствие terminfo
#

import locale
import sys
import traceback

import curses
from curses import *

def test(y, x, ukey, ckeys, lckey=1):
    m = 2
    of = 15
    mysc.addstr(y,x,ukey+": ");
    mysc.addstr(y+m,m,"Press character 'n' twice with pause to next test") 
    n = 0
    while n < 2:
        key = mysc.getkey()
        for i in range(lckey):
            mysc.timeout(300)
            try:
                key += mysc.getkey()
            except:
                pass
            mysc.timeout(-1)
        found = False
        for k in ckeys:
            if k == key:
                found = True
        if found:
            mysc.addstr(y,x+of,"                       ")
            mysc.addstr(y,x+of,"Ok: " + key)
            break
        elif "n" == key:
            n += 1
        else:
            mysc.timeout(300)
            keys = "'" + key
            try:
                while 1:
                    key = mysc.getkey()
                    keys += key
            except:
                pass
            keys += "'"
            mysc.timeout(-1)
            mysc.addstr(y,x+of,"                       ")
            mysc.addstr(y,x+of,"Fail: " + keys)
    mysc.addstr(y+m,m,"                                                 ") 

#========================================================== 
#                       MAIN LOOP 
#========================================================== 
locale.setlocale(locale.LC_ALL, '')
mysc = initscr() 
try: 
    noecho()
    term = termname().decode("ascii")
    shift = 12
    shift2 = 12
    for t in [ "xterm-old", "xterm-r5", "xterm-r6", "screen.xterm-r6", 
                "rxvt", "screen.rxvt", "putty", "screen.putty", "linux",
                "screen.linux" ]:
        if t == term[0:len(t)]:
            shift = 10
    mysc.keypad(1)
    mysc.clear() 
    mysc.border(0)
    mysc.addstr(1,2, str(sys.version_info))
    mysc.addstr(3,8,term + " " + str(mysc.getmaxyx()))
    mysc.addstr(3,45,str(locale.getlocale()))
    y = 5
    xl = 3
    xr = 40
    if 'KOI8' == locale.nl_langinfo(locale.CODESET)[0:4]:
        mysc.addstr(y,xl, "Ok: " + locale.nl_langinfo(locale.CODESET))
        mysc.addstr(y,xr, "Test output: \364\305\323\324 '\312'")
    else:
        mysc.addstr(y,xl, "Fail: "+locale.nl_langinfo(locale.CODESET))
    y = y + 1
    test(y,xl,"F3",["KEY_F(3)"])
    test(y,xr,"Shift+F3",["KEY_F(" + str(3+shift) + ")"])
    y = y + 1
    test(y,xl,"F5",["KEY_F(5)"])
    test(y,xr,"Ctrl+F5",["KEY_F(" + str(5+shift+shift2) + ")"])
    y = y + 1
    test(y,xl,"F9",["KEY_F(9)"])
    test(y,xr,"Alt-F9",["KEY_F(" + str(9+2*(shift+shift2)) + ")"])
    y = y + 1
    test(y,xl,"Left",["KEY_LEFT"])
    test(y,xr,"Shift+Left",["KEY_SLEFT"])
    y = y + 1
    test(y,xl,"Ctrl+Left",["kLFT5"])
    test(y,xr,"Alt+Left",["kLFT3"])
    y = y + 1
    test(y,xl,"Up",["KEY_UP"])
    test(y,xr,"Shift+Up",["KEY_SR"])
    y = y + 1
    test(y,xl,"Ctrl+Up",["kUP5"])
    test(y,xr,"Alt+Up",["kUP3"])
    y = y + 1
    test(y,xl,"PgUp",["KEY_PPAGE"])
    test(y,xr,"Shift+PgUp",["KEY_SPREVIOUS"])
    y = y + 1
    test(y,xl,"Shift+Tab",["KEY_BTAB"])
    test(y,xr,"Alt+Tab",["\033\t"], 2)
    y = y + 1
    test(y,xl,"Alt+Backspace",["\033\177","\033\b","\033KEY_BACKSPACE"],2)
    test(y,xr,"Alt+Enter",["\033\n","\033KEY_ENTER"],2)
    y = y + 1
    test(y,xl,"Alt+z",["\033z","\033Z"],2)
    test(y,xr,"Alt+\312",["\033q","\033Q","\033\312","\033\352"],2)
    y = y + 1

    mysc.addstr(23,2,"Press <CR> to exit") 
    mysc.getch() 
except: 
    endwin()
    traceback.print_exc()
    exit(2)
endwin()
