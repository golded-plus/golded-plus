# -*- makefile -*-

.PHONY: all clean sourcelist deps clean_sl

all: $(TARGET)

include $(TOP)/GNUmakef.def

# Messagebase formats
include $(TOP)/MSGBase.def

# Config files formats
include $(TOP)/Config.def

FOBJPATH=$(TOP)/$(OBJPATH)/$(PLATFORM)/$(TARGET)
FLIBPATH=$(TOP)/$(LIBPATH)/$(PLATFORM)
FDEPPATH=$(TOP)/$(DEPPATH)/$(PLATFORM)

.SUFFIXES: .c .cpp .all .rc

sourcelist: bld$(PLATFORM).inc

bld$(PLATFORM).inc: $(TARGET).all
	@echo making sourcelist
	@grep -w $(PLATFORM) $< \
		| sed 's;^\([[:alnum:]_]*\)[ ]*\([[:alnum:]_]*\).*;SOURCES+=\1.\2;' > $@; [ -s $@ ] || rm -f $@

$(FDEPPATH)/$(TARGET)/%.d: %.cpp
	@echo making depends for $<
	@$(SHELL) -ec '$(CXX) -c -M $(CPPFLAGS) $< \
		| sed '\''s;\($*\)\$(OBJEXT)[ :]*;$(subst /,\/,$(FOBJPATH))\/\1\$(OBJEXT) $(subst /,\/,$@): ;g'\'' > $@; [ -s $@ ] || rm -f $@'

$(FDEPPATH)/$(TARGET)/%.d: %.c
	@echo making depends for $<
	@$(SHELL) -ec '$(CC) -c -M $(CFLAGS) $< \
		| sed '\''s;\($*\)\$(OBJEXT)[ :]*;$(subst /,\/,$(FOBJPATH))\/\1\$(OBJEXT) $(subst /,\/,$@): ;g'\'' > $@; [ -s $@ ] || rm -f $@'

$(FOBJPATH)/%$(OBJEXT): %.cpp
	@echo building $(basename $<)$(OBJEXT)
	@$(CXX) -c $(CPPFLAGS) -o $@ $<

$(FOBJPATH)/%$(OBJEXT): %.c
	@echo building $(basename $<)$(OBJEXT)
	@$(CC) -c $(CFLAGS) -o $@ $<

ifeq ($(PLATFORM),cyg)
$(FOBJPATH)/%$(OBJEXT): %.rc
	@echo creating resources
	@$(WINDRES) -o $@ $<
endif

ifeq ($(PLATFORM),emx)
$(FOBJPATH)/%.res: %.rc
	@echo creating resources
	@rc $(subst -I,-i ,$(INCS)) -x1 -r $<
endif

SOURCES=
ifeq ($(FDEPPATH)/source.lst,$(wildcard $(FDEPPATH)/source.ls?))
include bld$(PLATFORM).inc
endif

ifeq ($(PLATFORM),cyg)
OBJS=$(addprefix $(FOBJPATH)/,$(patsubst %.rc,$(OBJEXT),$(patsubst %.c,$(OBJEXT),$(patsubst %.cpp,%$(OBJEXT),$(filter %.c %.cpp %.rc,$(SOURCES))))))
else
ifeq ($(PLATFORM),emx)
OBJS=$(addprefix $(FOBJPATH)/,$(patsubst %.rc,%.res,$(patsubst %.c,%$(OBJEXT),$(patsubst %.cpp,%$(OBJEXT),$(filter %.c %.cpp %.rc,$(SOURCES))))))
else
OBJS=$(addprefix $(FOBJPATH)/,$(patsubst %.c,%$(OBJEXT),$(patsubst %.cpp,%$(OBJEXT),$(filter %.c %.cpp %.rc,$(SOURCES)))))
endif
endif
DEPS=$(addprefix $(FDEPPATH)/$(TARGET)/,$(patsubst %.c,%.d,$(patsubst %.cpp,%.d,$(filter %.c %.cpp,$(SOURCES)))))

clean:
	@echo cleaning...
	@-rm -f $(OBJS) $(DEPS)

clean_sl:
	@echo deleting sourcelist
	@-rm -f bld$(PLATFORM).inc

deps: $(DEPS)

ifeq ($(FDEPPATH)/dep,$(wildcard $(FDEPPATH)/de?))
ifneq ($(DEPS),)
include $(DEPS)
endif
endif
