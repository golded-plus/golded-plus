# -*- makefile -*-

ifeq ($(TERM),cygwin)
MINGW=true
else
ifeq ($(OSTYPE),msys)
MINGW=true
endif
endif

TOP=..
SHORTTARGET=rddt
TARGET=rddt
INCS=-I$(TOP)/goldlib/gall
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
