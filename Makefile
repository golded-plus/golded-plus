# -*- makefile -*-

include GNUmakef.def

.PHONY: all clean distclean dirs sourcelists deps

LIBS=gall gcfg gmb3 glibc uulib smblib
EXECUTABLES=golded3 goldnode rddt

all: sourcelists
	@$(SHELL) -ec 'for i in $(foreach dir,$(LIBS),goldlib/$(dir)); do cd $$i; $(MAKE) all; cd ../..; done'
	@$(SHELL) -ec 'for i in $(EXECUTABLES); do cd $$i; $(MAKE) all; cd ..; done'

clean:
	@$(SHELL) -ec 'for i in $(foreach dir,$(LIBS),goldlib/$(dir)); do cd $$i; $(MAKE) clean; cd ../..; done'
	@$(SHELL) -ec 'for i in $(EXECUTABLES); do cd $$i; $(MAKE) clean; cd ..; done'
	@-rm -f $(OBJPATH)/$(PLATFORM)/source.lst

dirs:
	@mkdir -p $(BIN) $(LIBPATH)/$(PLATFORM)
	@mkdir -p $(foreach dir,$(LIBS),$(OBJPATH)/$(PLATFORM)/$(dir))
	@mkdir -p $(foreach dir,$(EXECUTABLES),$(OBJPATH)/$(PLATFORM)/$(dir))

sourcelists: dirs
	@$(SHELL) -ec 'for i in $(foreach dir,$(LIBS),goldlib/$(dir)); do cd $$i; $(MAKE) sourcelist; cd ../..; done'
	@$(SHELL) -ec 'for i in $(EXECUTABLES); do cd $$i; $(MAKE) sourcelist; cd ..; done'
	@echo -n >$(OBJPATH)/$(PLATFORM)/source.lst
