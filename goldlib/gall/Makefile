# -*- makefile -*-

ifeq ($(TERM),cygwin)
MINGW=true
else
ifeq ($(OSTYPE),msys)
MINGW=true
endif
endif

TOP=../..
TARGET=gall
INCS=-I$(TOP)/goldlib -I$(TOP)/goldlib/gall
ifdef MINGW
INCS+=-I$(TOP)/goldlib/glibc
else
ifeq ($(PLATFORM),emx)
INCS+=-I$(TOP)/goldlib/glibc
endif
endif

include $(TOP)/Config.def
include $(TOP)/GNUmakef.inc

ifeq ($(PLATFORM),emx)
ifeq ($(AR),emxomfar)
AR += -p32
endif
endif

include $(TOP)/GNUmakef.lib
