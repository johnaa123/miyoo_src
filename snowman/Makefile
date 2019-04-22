CC = arm-linux-gcc

#==stuff linked to
DYNAMIC = -lSDL_ttf -lSDL_mixer -lSDL_image -lSDL -lm

#==global Flags. Even on the gp2x with 16 kb Cache, -O3 is much better then -Os
CFLAGS = -O3 -fsingle-precision-constant -fPIC -march=armv5te -mtune=arm926ej-s -I../sparrow3d/

# Testtweaks: -fgcse-lm -fgcse-sm -fsched-spec-load -fmodulo-sched -funsafe-loop-optimizations -Wunsafe-loop-optimizations -fgcse-las -fgcse-after-reload -fvariable-expansion-in-unroller -ftracer -fbranch-target-load-optimize
GENERAL_TWEAKS = -ffast-math

#==PC==
FLAGS = -g -DDESKTOP $(GENERAL_TWEAKS)
SDL = `/opt/miyoo/arm-miyoo-linux-uclibcgnueabi/sysroot/usr/bin/sdl-config --cflags`

SPARROW_FOLDER = ../sparrow3d
SPARROW3D_LIB = libsparrow3d.so
SPARROWNET_LIB = libsparrowNet.so
SPARROWSOUND_LIB = libsparrowSound.so

ifdef TARGET
include $(SPARROW_FOLDER)/target-files/$(TARGET).mk
BUILD = ./build/$(TARGET)/snowman
SPARROW_LIB = $(SPARROW_FOLDER)/build/$(TARGET)/sparrow3d
else
TARGET = "Default (change with make TARGET=otherTarget. See All targets with make targets)"
BUILD = .
SPARROW_LIB = $(SPARROW_FOLDER)
endif
LIB += -L$(SPARROW_LIB)
INCLUDE += -I$(SPARROW_FOLDER)
DYNAMIC += -lsparrow3d -lsparrowSound -lsparrowNet

CFLAGS += $(PARAMETER) $(FLAGS)

all: snowman
	@echo "=== Built for Target "$(TARGET)" ==="

targets:
	@echo "The targets are the same like for sparrow3d. :P"

snowman: ballbullet.h bullet_new.h drawlevel.h particle.h bullet.h drawcharacter.h enemy.h level.h splashscreen.h snow.h snowman.c makeBuildDir
	cp -u $(SPARROW_LIB)/$(SPARROW3D_LIB) $(BUILD)
	cp -u $(SPARROW_LIB)/$(SPARROWNET_LIB) $(BUILD)
	cp -u $(SPARROW_LIB)/$(SPARROWSOUND_LIB) $(BUILD)
	$(CC) $(CFLAGS) $(LINK_FLAGS) snowman.c $(SDL) $(INCLUDE) $(LIB) $(SDL_LIB) $(STATIC) $(DYNAMIC) -o $(BUILD)/snowman$(SUFFIX)

makeBuildDir:
	 @if [ ! -d $(BUILD:/snowman=/) ]; then mkdir $(BUILD:/snowman=/);fi
	 @if [ ! -d $(BUILD) ]; then mkdir $(BUILD);fi

clean:
	rm -f *.o
	rm -f snowman$(SUFFX)
