# -*- makefile -*-

.PHONY: $(TARGET) $(GLIBS)

$(TARGET): $(TOP)/$(BIN)/$(SHORTTARGET)$(PLATFORM)$(EXEEXT)

ifeq ($(PLATFORM),emx)
LIBS=$(addprefix -llib,$(GLIBS))
else
LIBS=$(addprefix -l,$(GLIBS))
endif
LIBS+=$(STDLIBS)
FGLIBS=$(addprefix $(FLIBPATH)/lib, $(addsuffix $(LIBEXT), $(GLIBS)))

$(TOP)/$(BIN)/$(SHORTTARGET)$(PLATFORM)$(EXEEXT): $(OBJS) $(FGLIBS) $(ADDS)
	@echo -n Linking $(TARGET)...
	@$(CXX) $(LNKFLAGS) -o $@ $(FOBJPATH)/*$(OBJEXT) $(ADDS) $(LIBS) -L$(FLIBPATH)
	@echo done

$(FGLIBS): $(GLIBS)

$(GLIBS):
	@cd $(TOP)/goldlib/$@; $(MAKE); cd `pwd`
