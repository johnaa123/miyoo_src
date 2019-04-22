CC = arm-linux-gcc
CFLAGS += -DPC_VERSION -DRES320X240 `/opt/miyoo/arm-miyoo-linux-uclibcgnueabi/sysroot/usr/bin/sdl-config --cflags`

all: pang
pang : main.o structures.o ball.o collisions.o plateforme.o player.o ladder.o bonus.o shoot.o objets.o levels.o PCGfxEngine.o PCSoundEngine.o
	$(CC) -lSDL -lSDL_gfx -lSDL_mixer -lSDL_image -o pang main.o structures.o ball.o collisions.o plateforme.o player.o ladder.o bonus.o shoot.o objets.o levels.o PCGfxEngine.o PCSoundEngine.o
.c.o: 
	$(CC) $(CFLAGS) -c $*.c -o $*.o 

clean:
	rm -rf *.o
	rm -rf pang 
	rm -rf *~

.PHONY: clean pang
