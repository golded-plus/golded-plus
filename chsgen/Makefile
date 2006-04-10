TOP=..

include $(TOP)/GNUmakef.def

TARGET=chsgn$(PLATFORM)$(EXE)

all: $(TARGET)

$(TARGET):  chsgen.cc
	$(CXX) $(LNKFLAGS) $(CPPFLAGS) -o $@ $<
