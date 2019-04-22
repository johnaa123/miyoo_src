TARGET  = mame
MAMEOS  = odx

DEBUG   = 0
VPATH   = src $(wildcard src/cpu/*)

ifeq ($(DEBUG), 0)
  CROSS   = arm-linux-
endif
CC      = $(CROSS)gcc
CPP     = $(CROSS)gcc
LD      = $(CROSS)gcc

MD      = @mkdir
RM      = @rm -f
CP      = @cp
MV      = @mv
DEVLIBS =
EMULATOR= $(TARGET)
DEFS    = -D__ODX__ -DLSB_FIRST -DALIGN_INTS -DALIGN_SHORTS -DINLINE="static inline" -Dasm="__asm__ __volatile__" \
          -DMAME_UNDERCLOCK -DMAME_FASTSOUND -DENABLE_AUTOFIRE -DBIGCASE

W_OPTS  = -Wall -Wno-write-strings -Wno-sign-compare -Wno-narrowing
ifeq ($(DEBUG), 0)
  F_OPTS  = -fpermissive -falign-functions -falign-loops -falign-labels -falign-jumps \
	        -ffast-math -fsingle-precision-constant -funsafe-math-optimizations \
	        -fomit-frame-pointer -fno-builtin -fno-exceptions -fno-common \
	        -fstrict-aliasing -fexpensive-optimizations -fno-pic \
	        -finline -finline-functions -fmerge-all-constants \
	        -ftree-vectorize -fweb -frename-registers
else
  F_OPTS  = -fpermissive -fno-exceptions
endif

ifeq ($(DEBUG), 0)
  CFLAGS  = -D_GCW0_ -O3 -march=armv5te -mtune=arm926ej-s -Isrc -Isrc/$(MAMEOS) -Isrc/zlib $(W_OPTS) $(F_OPTS)
else
  CFLAGS  = -D_GCW0_ -Isrc -Isrc/$(MAMEOS) -Isrc/zlib $(W_OPTS) $(F_OPTS) -fPIC
endif

LIBS    = -lSDL  -lpthread -lm -lgcc 
ifeq ($(DEBUG), 0)
  LDFLAGS = $(CFLAGS) $(LIBS) -s
else
  LDFLAGS = $(CFLAGS) $(LIBS)
endif

OBJ     = obj_$(TARGET)_$(MAMEOS)
OBJDIRS = $(OBJ) $(OBJ)/cpu $(OBJ)/sound $(OBJ)/$(MAMEOS) $(OBJ)/drivers $(OBJ)/machine $(OBJ)/vidhrdw $(OBJ)/sndhrdw $(OBJ)/zlib

all:	maketree $(EMULATOR)

# include the various .mak files
include src/core.mak
include src/$(TARGET).mak
include src/rules.mak
include src/sound.mak
include src/$(MAMEOS)/$(MAMEOS).mak

# combine the various definitions to one
CDEFS = $(DEFS) $(COREDEFS) $(CPUDEFS) $(SOUNDDEFS)

$(EMULATOR): $(COREOBJS) $(OSOBJS) $(DRVOBJS)
	$(LD) $(LDFLAGS) $(COREOBJS) $(OSOBJS) $(LIBS) $(DRVOBJS) -o $@
	$(MV) $(EMULATOR) out/mame4all.dge

$(EMULATOR).dge: $(FEOBJS)
	@echo Linking $@ ...
	$(LD) $(LDFLAGS) $(FEOBJS) $(LIBS) -o $@
	$(MV) $(EMULATOR).dge out/$(EMULATOR).dge

$(OBJ)/%.o: src/%.c
	@echo Compiling $<...
	$(CC) $(CDEFS) $(CFLAGS) -c $< -o $@

$(OBJ)/%.o: src/%.cpp
	@echo Compiling $<...
	$(CPP) $(CDEFS) $(CFLAGS) -fno-rtti -c $< -o $@

$(OBJ)/%.o: src/%.s
	@echo Compiling $<...
	$(CPP) $(CDEFS) $(CFLAGS) -c $< -o $@

$(OBJ)/%.o: src/%.S
	@echo Compiling $<...
	$(CPP) $(CDEFS) $(CFLAGS) -c $< -o $@

$(sort $(OBJDIRS)):
	$(MD) $@

maketree: $(sort $(OBJDIRS))

clean:
	$(RM) -r $(OBJ)
	$(RM) $(EMULATOR)
