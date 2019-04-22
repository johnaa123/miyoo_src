DEBUG = 0
CROSS = arm-linux-

ifeq ($(DEBUG),0)
  CC = $(CROSS)g++
  CFLAGS = -I/opt/miyoo/arm-miyoo-linux-uclibcgnueabi/sysroot/usr/include/SDL 
  LDFLAGS = -L/opt/miyoo/arm-miyoo-linux-uclibcgnueabi/sysroot/usr/lib
else
  CC = g++
  CFLAGS = -I/usr/include/SDL -ggdb
  LDFLAGS = -L/usr/lib
endif

all:
	$(CC) main.c -o calib $(CFLAGS) -lSDL -lSDL_ttf -lSDL_image -lSDL_gfx $(LDFLAGS)

clean:
	rm -f calib
