# -*- makefile -*-

TOP=..
SHORTTARGET=ged
TARGET=golded3
GLIBS=gmb3 gall gcfg uulib smblib
INCS=-I. -I$(TOP)/goldlib/gall -I$(TOP)/goldlib/gcfg -I$(TOP)/goldlib/gmb3 -I$(TOP)/goldlib/uulib -I$(TOP)/goldlib/smblib

ifeq ($(findstring EMX, $(PATH)), EMX)
STDLIBS=-los2me -lstdcpp
GLIBS+=glibc
else
ifeq ($(TERM),cygwin)
STDLIBS=-luser32 -lwinmm
GLIBS+=glibc
endif
endif

include $(TOP)/GNUmakef.inc
include $(TOP)/GNUmakef.prg
