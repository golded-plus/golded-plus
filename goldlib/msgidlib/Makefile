# -*- makefile -*-

TOP=../..
TARGET=msgidlib
#INCS=-I$(TOP)/goldlib/gall

include $(TOP)/GNUmakef.inc
include $(TOP)/GNUmakef.lib

ifeq ($(PLATFORM),be)
CFLAGS+=-DUNIX=__BEOS__
endif
