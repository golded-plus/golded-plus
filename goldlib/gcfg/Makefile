# -*- makefile -*-

ifeq ($(TERM),cygwin)
MINGW=true
else
ifeq ($(OSTYPE),msys)
MINGW=true
endif
endif

TOP=../..
TARGET=gcfg
INCS=-I$(TOP)/goldlib/gall -I$(TOP)/goldlib/gcfg
ifdef MINGW
INCS+=-I$(TOP)/goldlib/glibc
else
ifeq ($(findstring EMX, $(PATH)), EMX)
INCS+=-I$(TOP)/goldlib/glibc
endif
endif

include $(TOP)/GNUmakef.inc
include $(TOP)/GNUmakef.lib
