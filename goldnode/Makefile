# -*- makefile -*-

TOP=..
SHORTTARGET=gn
TARGET=goldnode
INCS=-I$(TOP)/goldlib/gall -I$(TOP)/goldlib/gcfg -I$(TOP)/golded3/
ifeq ($(findstring EMX, $(PATH)), EMX)
STDLIBS=-lstdcpp
endif
ifeq ($(TERM),cygwin)
STDLIBS=-luser32
endif
GLIBS=gall
CLEANS=

include $(TOP)/GNUmakef.inc
include $(TOP)/GNUmakef.prg
