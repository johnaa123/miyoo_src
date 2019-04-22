TARGET=tombstone
TOMBSTONE_VERSION=1.0

SDL_CFLAGS = `/opt/miyoo/arm-miyoo-linux-uclibcgnueabi/sysroot/usr/bin/sdl-config --cflags`
SDL_LIBS = `/opt/miyoo/arm-miyoo-linux-uclibcgnueabi/sysroot/usr/bin/sdl-config --libs`

DEFINES = -DDINGUX_MODE -Wall -DGIF_SUPPORT -Iicons 

CROSS = arm-linux-
CC    = $(CROSS)gcc
CXX   = $(CROSS)g++
STRIP = $(CROSS)strip

LIBS += -lpng -lz -lm -lpthread -ldl -lstdc++ $(SDL_LIBS) -lgif

#CXXFLAGS+= $(SDL_CFLAGS) $(DEFINES) -I. \
#-O3 -fstrength-reduce -fthread-jumps -fexpensive-optimizations \
#-fomit-frame-pointer -frename-registers -pipe  \
#-DMPU_JZ4740 -ffast-math -ggdb

CXXFLAGS+= $(SDL_CFLAGS) $(DEFINES) -I. -Iicons -ggdb

OBJS = Gfx.o Font.o Help.o Tombstone.o

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) $(OBJS) $(LIBS) -o $(TARGET)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $*.o

clean:
	rm $(OBJS) $(TARGET)
