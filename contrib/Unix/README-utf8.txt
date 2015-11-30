# vim:set sw=4 ts=8 tw=72 fileencoding=utf8::Кодировка:UTF-8[АБЁЪЯабёъя] 
# You can use `iconv -f utf-8 README-utf8.txt'

Описание файлов каталога golded+/contrib/Unix



1. putty-0-65.ti - расширенное terminfo описание терминала putty 0.65

    1.1 putty и putty-256color описание терминала PuTTY при настройке
	"The Functions keys and keypad = ESC[n~" с возможностями
	terminfo (capname): kri/kind (scroll-backward/forward key).
	(Не отличается от описания в последней версии
	<http://invisible-island.net/ncurses/ncurses.faq.html#which_terminfo>).

    1.2 putty-xterm и putty-xterm-256color описание терминала PuTTY при
	настройке "The Functions keys and keypad = Xterm R6" (этого
	описания в terminfo [1.554 2015/10/24] не было).

    1.3 sco-putty описание терминала PuTTY при настройке "The Functions
	keys and keypad = SCO" и "The Backspace key = Control-H". При
	этой настройке программам доступны не только Shift+Fn, но и 
	Ctrl+Fn клавиши. Конфликт между Shift+F2 и Shift+Tab разрешён в
	пользу последнего, т.е. Shift+F2 интерпретируется как KEY_BTAB
	(Back tab key).

    1.3 Применение в качестве собственного описания обычного
	пользователя (не root):

	    $ mkdir $HOME/.terminfo
	    $ tic -x contrib/Unix/putty-0-65.ti
	    $ TERM=putty; export TERM

	или

	    $ TERM=putty-xterm; export TERM

	или

	    $ TERM=sco-putty; export TERM

	в зависимости от настройки PuTTY.

	Значение переменной окружения TERM можно настроить в настройках
	PuTTY Connection->Data->"Terminal-type string".

2. XTerm-KOI8-U-strict.terminal - файл описания Терминала для OS X 10.9,
    10.10 и 10.11 с максимально полным соответствием XTerm R7.  (Получен
    с помощью make-term-profiles.sh и xterm.plist-template)

    2.1 Определяет Esc-последовательности клавиш согласно XTerm R7 по
	terminfo.src Revision: 1.554 $ Date: 2015/10/24 16:00:04
	<http://invisible-island.net/ncurses/ncurses.faq.html#which_terminfo>

    2.2 Задаёт преобразование <option>+<национальная буква> в
	<Esc><национальная буква> для стандартных белорусских, русских и
	украинских букв максимально близким к XTerm образом, остались
	отличия в части обработки [caps lock].

    2.3 Клавиша "Ins" эмулируется , либо F13 с модификаторами, либо на
	узких клавиатурах, <control>+<option>+I без модификаторов.

    2.4 XTerm-*-reverse.terminal - меняет местами "option" и "control"
	таким образом, что бы передвижение по словам было таким же как в
	других приложениях OS X. Возможно, предпочтительнее для тех, кто
	использует Golded+ только под OS X.

    2.5 XTerm-*-combined.terminal - <option>+<стрелка> и
	<control>+<стрелка> порождают одинаковые действия. Возможно,
	предпочтительнее для тех, кто не использует комбинаций Golded+
	<Alt>+<стрелка>.

    2.6 XTerm-UTF-8-*.terminal - предназначен для запуска Golded+ под
	screen или luit. Например, для использования белорусского языка,
	т.к. в Терминал OS X нет поддержки кодировки KOI8-RU.

    2.7 XTerm-KOI8-R-*.terminal - предназначен для эстетического
	созерцания псевдографики.
    
    2.8 Применение:

	$ open contrib/Unix/GED-KOI8-U-strict.terminal

	или открыть в Finder.

    2.9 Использование на 10.8:

	- Apple Terminal 2.3 (309) игнорирует назначения для
	  <option>+<национальная буква>, в остальном, вроде бы
	  нормально;

	- Из каталога MacOSX на Яндекс.Диск
	  https://yadi.sk/d/DKzCSJGhkRT8Y можно скачать файл
	  at-2.4-326-10.9.5.tar.gz (предназначен только для
	  пользователей компьютеров Apple).  Раскрыть его в домашнем
	  каталоге.
	  
	- Перед использованием рекомендуется сохранить конфигурацию
	  Терминала командой:

	  $ defaults export com.apple.Terminal Save-Terminal.plist

	  Если, в будущем, возникнут конфликты между версиям 2.3 (309) и
	  2.4 (326) можно будет восстановить сохранённую конфигурацию
	  командой:

	  $ defaults import com.apple.Terminal Save-Terminal.plist

	  и перезапустить терминал.

	- При использовании Automator и AppleScript могут возникать
	  определённые сложности, т.к. в системе будет два приложения с
	  идентичными названиями и другими атрибутами. Типа ключ '-a' у
	  команды `open' и т.п.
	
	- Так же, как вариант, вместо Терминала можно установить X11 и
	  использовать `koi8rxterm'. В нём, правда настройки
	  по-умолчанию странные. Alt - нет, Meta - <command>, и Meta
	  клавиши выдают Esc. В общем, с помощью xmodmap и какой-то
	  матери настроить наверное можно.


3. koi8rxterm-phonetic.sh - макет запуска koi8rxterm с заменой
    <Alt>+<национальная буква> для одного из вариантов русской
    фонетической клавиатуры ЯВЕРТЫ.


4. make-alt-translations.sh - Построение содержимого ресурса
    VT100.translations для xterm


5. ged-vttest-lite.py - Python скрипт для тестирования соответствия
    описания терминала, terminfo и самого терминала.

