# -*- makefile -*-

TOP=..
SHORTTARGET=rddt
TARGET=rddt
INCS=-I$(TOP)/goldlib/gall
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
