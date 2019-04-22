CXX := arm-linux-g++
MACHINE := $(shell $(CXX) -dumpmachine)
SYSROOT := $(shell $(CXX) -print-sysroot)
CXXFLAGS := -ffunction-sections -ffast-math -fsingle-precision-constant
CXXFLAGS += $(shell $(SYSROOT)/usr/bin/sdl-config --cflags)
LIBS := $(shell $(SYSROOT)/usr/bin/sdl-config --libs) -lSDL_mixer

OUTDIR		:= output/$(MACHINE)
DATADIR		:= data
OPKDIR		:= opk_data
RELEASEDIR	:= $(OUTDIR)/release

ifneq ($(filter mipsel-gcw0-%,$(MACHINE)),)
CXXFLAGS += -mips32 -mtune=mips32 -mbranch-likely -G0
endif

CXXFLAGS += -Wextra -Wall
ifdef DEBUG
	CXXFLAGS += -ggdb3
	OUTDIR := $(OUTDIR)-debug
else
	CXXFLAGS += -O2 -fomit-frame-pointer
	LDFLAGS += -s
endif

CXXFLAGS += -DHOME_DIR -DPLATFORM_GCW0

BINDIR := $(OUTDIR)/bin
OBJDIR := $(OUTDIR)/obj

SRC := $(wildcard *.cpp)
OBJ := $(SRC:%.cpp=$(OBJDIR)/%.o)
EXE := $(BINDIR)/liero

.PHONY: all clean

all : $(SRC) $(EXE)

$(EXE): $(OBJ) | $(BINDIR)
	$(CXX) $(LDFLAGS) $(OBJ) $(LIBS) -o $@

$(OBJ): $(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(BINDIR) $(OBJDIR):
	mkdir -p $@

opk:
	mkdir -p $(RELEASEDIR)
	cp $(EXE) $(RELEASEDIR)
	cp -R $(DATADIR) $(RELEASEDIR)
	cp $(OPKDIR)/* $(RELEASEDIR)
	cp COPYRIGHT $(RELEASEDIR)
	cp README.md $(RELEASEDIR)
	mksquashfs $(RELEASEDIR) $(BINDIR)/liero.opk -all-root -noappend -no-exports -no-xattrs

clean:
	rm -rf output
	rm -rf $(RELEASEDIR)
