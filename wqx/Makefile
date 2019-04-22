DEBUG = 0
CROSS = mipsel-linux-

ifeq ($(DEBUG),0)
  CC = $(CROSS)g++
  CFLAGS = -I/opt/ldk/mipsel-buildroot-linux-uclibc/sysroot/usr/include/SDL 
  LDFLAGS = -L/opt/ldk/mipsel-buildroot-linux-uclibc/sysroot/usr/lib
else
  CC = g++
  CFLAGS = -I/usr/include/SDL
  LDFLAGS = -L/usr/lib
endif

all:
	$(CC) nc1020.cpp main.cpp -o wqx $(CFLAGS) -lSDL -lSDL_ttf -lSDL_image -lSDL_gfx $(LDFLAGS)

clean:
	rm -f wqx
