# -*- makefile -*-

ifeq ($(TERM),msys)
MSYS=true
else
ifeq ($(OSTYPE),msys)
MSYS=true
endif
endif

TOP=../..
TARGET=gall
INCS=-I$(TOP)/goldlib/gall
ifeq ($(findstring EMX, $(PATH)), EMX)
INCS+=-I$(TOP)/goldlib/glibc
else
ifeq ($(TERM),cygwin)
INCS+=-I$(TOP)/goldlib/glibc
else
ifdef MSYS
INCS+=-I$(TOP)/goldlib/glibc
endif
endif
endif

include $(TOP)/GNUmakef.inc

ifeq ($(PLATFORM),emx)
ifeq ($(AR),emxomfar)
AR += -p32
endif
endif

include $(TOP)/GNUmakef.lib
