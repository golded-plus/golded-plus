# -*- makefile -*-

ifeq ($(TERM),cygwin)
MINGW=true
else
ifeq ($(OSTYPE),msys)
MINGW=true
endif
endif

TOP=..
SHORTTARGET=gn
TARGET=goldnode
INCS=-I$(TOP)/goldlib -I$(TOP)/goldlib/gall -I$(TOP)/goldlib/gcfg -I$(TOP)/golded3/
ifdef MINGW
STDLIBS=-luser32
endif
GLIBS=gall
CLEANS=
ADDS+=$(TOP)/$(OBJPATH)/$(PLATFORM)/golded3/golded3$(OBJEXT)

include $(TOP)/GNUmakef.inc
include $(TOP)/GNUmakef.prg
