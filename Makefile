# -*- makefile -*-

include GNUmakef.def

.PHONY: all clean distclean dirs sourcelists deps

LIBS=gall gcfg gmb3 glibc uulib
EXECUTABLES=golded3 goldnode rddt

all: sourcelists deps
	@$(SHELL) -ec 'for i in $(foreach dir,$(LIBS),goldlib/$(dir)); do cd $$i; $(MAKE) all; cd ../..; done'
	@$(SHELL) -ec 'for i in $(EXECUTABLES); do cd $$i; $(MAKE) all; cd ..; done'

clean: nodeps sourcelists
	@$(SHELL) -ec 'for i in $(foreach dir,$(LIBS),goldlib/$(dir)); do cd $$i; $(MAKE) clean; cd ../..; done'
	@$(SHELL) -ec 'for i in $(EXECUTABLES); do cd $$i; $(MAKE) clean; cd ..; done'
	@-rm -f $(DEPPATH)/$(PLATFORM)/source.lst
	@$(SHELL) -ec 'for i in $(foreach dir,$(LIBS),goldlib/$(dir)); do cd $$i; $(MAKE) clean_sl; cd ../..; done'
	@$(SHELL) -ec 'for i in $(EXECUTABLES); do cd $$i; $(MAKE) clean_sl; cd ..; done'

dirs:
	@mkdir -p $(BIN) $(LIBPATH)/$(PLATFORM)
	@mkdir -p $(foreach dir,$(LIBS),$(OBJPATH)/$(PLATFORM)/$(dir))
	@mkdir -p $(foreach dir,$(EXECUTABLES),$(OBJPATH)/$(PLATFORM)/$(dir))
	@mkdir -p $(foreach dir,$(LIBS),$(DEPPATH)/$(PLATFORM)/$(dir))
	@mkdir -p $(foreach dir,$(EXECUTABLES),$(DEPPATH)/$(PLATFORM)/$(dir))

deps:
	@$(SHELL) -ec 'for i in $(foreach dir,$(LIBS),goldlib/$(dir)); do cd $$i; $(MAKE) deps; cd ../..; done'
	@$(SHELL) -ec 'for i in $(EXECUTABLES); do cd $$i; $(MAKE) deps; cd ..; done'
	@echo -n >$(DEPPATH)/$(PLATFORM)/dep

nodeps:
	@-rm -f $(DEPPATH)/$(PLATFORM)/dep

sourcelists: dirs
	@$(SHELL) -ec 'for i in $(foreach dir,$(LIBS),goldlib/$(dir)); do cd $$i; $(MAKE) sourcelist; cd ../..; done'
	@$(SHELL) -ec 'for i in $(EXECUTABLES); do cd $$i; $(MAKE) sourcelist; cd ..; done'
	@echo -n >$(DEPPATH)/$(PLATFORM)/source.lst
