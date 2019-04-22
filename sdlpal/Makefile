TARGET = sdlpal
HOST = arm-linux-
SOURCES = . ./adplug
CFILES = $(foreach dir, $(SOURCES), $(wildcard $(dir)/*.c))
CPPFILES = $(foreach dir, $(SOURCES), $(wildcard $(dir)/*.cpp))
OFILES = $(CFILES:.c=.o) $(CPPFILES:.cpp=.o)

CFLAGS = -Wall -O2 `/opt/miyoo/arm-miyoo-linux-uclibcgnueabi/sysroot/usr/bin/sdl-config --cflags` -DDINGOO -mtune=arm926ej-s -march=armv5te
CXXFLAGS = $(CFLAGS)
LDFLAGS = `/opt/miyoo/arm-miyoo-linux-uclibcgnueabi/sysroot/usr/bin/sdl-config --libs`

$(TARGET): $(OFILES)
	$(HOST)g++ $(OFILES) -o $@ $(LDFLAGS)

%.o: %.c
	$(HOST)gcc $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(HOST)g++ $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OFILES)
