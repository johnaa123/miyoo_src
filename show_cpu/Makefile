all:
	gcc main.c -o show_cpu -lSDL_image -lSDL -lSDL_ttf -I/usr/include/SDL -I/usr/include/SDL2
	#mipsel-linux-gcc main.c -o show_cpu -lSDL -lSDL_image -lSDL_ttf -I/opt/a320/usr/include/SDL --sysroot=/opt/a320 -static
	#mipsel-linux-gcc main.c -o show_cpu -I/opt/a320/usr/include/SDL --sysroot=/opt/a320 /opt/a320/usr/lib/libSDLmain.a -lSDLmain -lSDL -static -lpthread
clean:
	rm -rf show_cpu
