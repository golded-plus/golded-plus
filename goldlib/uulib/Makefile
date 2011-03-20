# -*- makefile -*-

VERSION=0.5
PATCH=15

TOP=../..
TARGET=uulib
INCS=-I$(TOP)/goldlib -I$(TOP)/goldlib/uulib -I$(TOP)/goldlib/gall -I$(TOP)/goldlib/glibc
CFLAGS=-DHAVE_CONFIG_H -DVERSION=\"$(VERSION)\" -DPATCH=\"$(PATCH)\"

include $(TOP)/GNUmakef.inc
include $(TOP)/GNUmakef.lib
