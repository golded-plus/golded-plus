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
INCS=-I$(TOP)/goldlib/gall -I$(TOP)/goldlib/gcfg -I$(TOP)/golded3/
ifeq ($(findstring EMX, $(PATH)), EMX)
STDLIBS=-lstdcpp
else
ifdef MINGW
STDLIBS=-luser32
endif
endif
GLIBS=gall
CLEANS=

include $(TOP)/GNUmakef.inc
include $(TOP)/GNUmakef.prg
