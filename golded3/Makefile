# -*- makefile -*-

ifeq ($(TERM),cygwin)
MINGW=true
else
ifeq ($(OSTYPE),msys)
MINGW=true
endif
endif

TOP=..

include $(TOP)/Config.def
SHORTTARGET=ged
TARGET=golded3

GLIBS=gmb3 gcfg uulib gall gcui smblib
ifneq ($(findstring GCFG_NO_MYSPELL, $(CPPFLAGS)), GCFG_NO_MYSPELL)
GLIBS+=hunspell
endif

INCS=-I. -I$(TOP)/goldlib/gall -I$(TOP)/goldlib/gcui -I$(TOP)/goldlib/gcfg -I$(TOP)/goldlib/gmb3 -I$(TOP)/goldlib/uulib -I$(TOP)/goldlib/smblib

ifeq ($(PLATFORM),emx)
STDLIBS=-los2me
GLIBS+=glibc
INCS+=-I$(TOP)/goldlib/glibc
else
ifdef MINGW
STDLIBS=-luser32 -lwinmm
GLIBS+=glibc
INCS+=-I$(TOP)/goldlib/glibc
endif
endif

include $(TOP)/GNUmakef.inc

ifeq ($(USE_NCURSES),1)
STDLIBS+= $(LIBCURSES)
endif

include $(TOP)/GNUmakef.prg
