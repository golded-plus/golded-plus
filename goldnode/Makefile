# -*- makefile -*-

TOP=..
SHORTTARGET=gn
TARGET=goldnode
INCS=-I$(TOP)/goldlib/gall -I$(TOP)/goldlib/gcfg -I$(TOP)/golded3/
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
