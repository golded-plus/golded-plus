# -*- makefile -*-

TOP=..
SHORTTARGET=rddt
TARGET=rddt
INCS=-I$(TOP)/goldlib/gall
ifeq ($(findstring EMX, $(PATH)), EMX)
STDLIBS=-lstdcpp
else
ifeq ($(TERM),cygwin)
STDLIBS=-luser32
else
ifeq ($(TERM),msys)
STDLIBS=-luser32
endif
endif
endif
GLIBS=gall
CLEANS=

include $(TOP)/GNUmakef.inc
include $(TOP)/GNUmakef.prg
