# -*- makefile -*-

ifeq ($(TERM),msys)
MSYS=true
else
ifeq ($(OSTYPE),msys)
MSYS=true
endif
endif

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
ifdef MSYS
STDLIBS=-luser32
endif
endif
endif
GLIBS=gall
CLEANS=

include $(TOP)/GNUmakef.inc
include $(TOP)/GNUmakef.prg
