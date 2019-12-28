# $Id$
# Build Golded+, Goldnode and RDDT using GNU make and CNU C++ compuler.
# You may use following parameters:
# "PLATFORM=xxx", where xxx specifies platform, one of:
#                   "cyg" - for MinGW or Cygwin (MinGW build),
#                   "lnx" - for GNU/Linux or any BSD,
#                   "emx" - for OS/2 EMX
#                   "sun" - for SunOS (Solaris)
#                   "osx" - for OS X
#                   "djg" - for DJGPP (DOS with DPMI)
#                   "be"  - for BeOS
# "BUILD=minimal"   Compile Golded+ with support only most popular formats of
#                   message bases (OPUS, Jam, Squish).
# "USE_NCURSES=0"   if you dislike curses library (Linux version requires to
#                   use curses)
# "WIDE_NCURSES=1"  if it is needed to use wide characters with curses library
# "BUGGY_NCURSES=1" if Golded+ requires additional keypresses to pass areascan
# "KOI8=1"          Better koi-8 support for linux. Do not set this for
#                   international builds since latin-1 support will be broken.
# "ICONV=1"         Use iconv library for recoding text. Experimental! Do not
#                   set this if you don't sure!
# "OLD_SHIFT_FN=1"  Use original code, where use: F1-F12 and
#                   <Shift>+F3-F12 mapped to F15-F22.
#
#                   New code, by default, for orthodox terminals:
#                   xterm R6, PuTTY, urxvt, mapped <Shift>+F1-F10 to
#                   F11-F20 and <Ctrl>+<Shift>+F1-F10 to <Ctrl>+F11-F20.
#
#                   For modern terminals: xterm R7, gnome-terminal,
#                   Linux & FreeBSD console, DECFNK, mapped:
#                   <Shift>+F1-F12 to key_fn13 - key_fn24,
#                   <Ctrl>+F1-F12 to key_fn25 - key_fn36,
#                   <Ctrl>+<Shift>+F1-F12 to key_fn37 - key_fn48,
#                   <Alt>+F1-F12 to key_fn49 - key_fn60

include GNUmakef.def
include Config.def

.PHONY: all clean distclean dirs sourcelists deps docs

LIBS=gall gcui gcfg gmb3 glibc uulib smblib
ifneq ($(findstring GCFG_NO_MYSPELL, $(CPPFLAGS)), GCFG_NO_MYSPELL)
LIBS+=hunspell
endif

EXECUTABLES=golded3 goldnode rddt

all: sourcelists
	@$(SHELL) -ec 'for i in $(foreach dir,$(LIBS),goldlib/$(dir)); do cd $$i; $(MAKE) all; cd ../..; done'
	@$(SHELL) -ec 'for i in $(EXECUTABLES); do cd $$i; $(MAKE) all; cd ..; done'
	@echo 'To build HTML man pages run "make docs".'
	@echo 'To reduce binaries size (remove debug information) run "make strip".'

clean:
	@$(SHELL) -ec 'for i in $(foreach dir,$(LIBS),goldlib/$(dir)); do cd $$i; $(MAKE) clean; cd ../..; done'
	@$(SHELL) -ec 'for i in $(EXECUTABLES); do cd $$i; $(MAKE) clean; cd ..; done'
	@-rm -f $(OBJPATH)/$(PLATFORM)/source.lst

distclean:
	@-cd $(BIN); rm -f $(EXECUTABLES)

dirs:
	@mkdir -p $(BIN) $(LIBPATH)/$(PLATFORM)
	@mkdir -p $(foreach dir,$(LIBS),$(OBJPATH)/$(PLATFORM)/$(dir))
	@mkdir -p $(foreach dir,$(EXECUTABLES),$(OBJPATH)/$(PLATFORM)/$(dir))

sourcelists: dirs
	@$(SHELL) -ec 'for i in $(foreach dir,$(LIBS),goldlib/$(dir)); do cd $$i; $(MAKE) sourcelist; cd ../..; done'
	@$(SHELL) -ec 'for i in $(EXECUTABLES); do cd $$i; $(MAKE) sourcelist; cd ..; done'
	@echo -n >$(OBJPATH)/$(PLATFORM)/source.lst

docs:
	@cd $@ ; $(MAKE) all ; cd ..

strip:
	cd $(BIN)/ ; strip *$(PLATFORM)$(EXEEXT)

format:
	astyle --style=allman --indent-preproc-block -R *.cpp,*.h,*.c,*.cxx,*.hxx
