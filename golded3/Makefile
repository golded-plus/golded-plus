# -*- makefile -*-

ifeq ($(TERM),cygwin)
MINGW=true
else
ifeq ($(OSTYPE),msys)
MINGW=true
endif
endif

TOP=..
SHORTTARGET=ged
TARGET=golded3
GLIBS=gmb3 gall gcfg uulib smblib msgidlib
INCS=-I. -I$(TOP)/goldlib/gall -I$(TOP)/goldlib/gcfg -I$(TOP)/goldlib/gmb3 -I$(TOP)/goldlib/uulib -I$(TOP)/goldlib/smblib -I$(TOP)/goldlib/msgidlib

ifeq ($(findstring EMX, $(PATH)), EMX)
STDLIBS=-los2me -lstdcpp
GLIBS+=glibc
else
ifdef MINGW
STDLIBS=-luser32 -lwinmm
GLIBS+=glibc
endif
endif

include $(TOP)/GNUmakef.inc
include $(TOP)/GNUmakef.prg
