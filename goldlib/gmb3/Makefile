# -*- makefile -*-

TOP=../..
TARGET=gmb3
INCS=-I$(TOP)/goldlib/gall -I$(TOP)/goldlib/gcfg -I$(TOP)/goldlib/gmb3

include $(TOP)/GNUmakef.inc
ifeq ($(PLATFORM),emx)
ifeq ($(AR),emxomfar)
AR += -p32
endif
endif
include $(TOP)/GNUmakef.lib
