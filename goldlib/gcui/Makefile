##  -------------------------------------------------------------------
##  The Goldware Library. Copyright (C) Odinn Sorensen and Golded+ team
##  -------------------------------------------------------------------
## GCUI: Golded+ Character-oriented User Interface library.
##  -------------------------------------------------------------------
# $Id$
# Build library using GNU make and CNU C++ compuler.

ifeq ($(TERM),cygwin)
MINGW=true
else
ifeq ($(OSTYPE),msys)
MINGW=true
endif
endif

TOP=../..
TARGET=gcui
INCS=-I$(TOP)/goldlib -I$(TOP)/goldlib/gcui -I$(TOP)/goldlib/gall

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
