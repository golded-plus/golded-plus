# -*- makefile -*-

include GNUmakef.def

LIBS=gall gcfg gmb3 glibc uulib
EXECUTABLES=golded3 goldnode rddt

all:
	@$(MAKE) dirs
	@echo -n >$(DEPPATH)/$(PLATFORM)/dep
	@for i in $(foreach dir,$(LIBS),goldlib/$(dir)); do cd $$i && $(MAKE) && cd ../..; done
	@for i in $(EXECUTABLES); do cd $$i && $(MAKE) && cd ..; done

clean:
	@rm -f $(DEPPATH)/$(PLATFORM)/dep
	@for i in $(foreach dir,$(LIBS),goldlib/$(dir)); do cd $$i && $(MAKE) clean && cd ../..; done
	@for i in $(EXECUTABLES); do cd $$i && $(MAKE) clean && cd ..; done

dirs:
	@mkdir -p $(BIN) $(LIBPATH)/$(PLATFORM)
	@mkdir -p $(foreach dir,$(LIBS),$(OBJPATH)/$(PLATFORM)/$(dir))
	@mkdir -p $(foreach dir,$(EXECUTABLES),$(OBJPATH)/$(PLATFORM)/$(dir))
	@mkdir -p $(foreach dir,$(LIBS),$(DEPPATH)/$(PLATFORM)/$(dir))
	@mkdir -p $(foreach dir,$(EXECUTABLES),$(DEPPATH)/$(PLATFORM)/$(dir))
