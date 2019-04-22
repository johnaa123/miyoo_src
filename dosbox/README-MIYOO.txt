
How to compile DosBox for Miyoo:

./autogen.sh
SDL_CONFIG=/opt/miyoo/arm-miyoo-linux-uclibcgnueabi/sysroot/usr/bin/sdl-config ./configure --host=arm-linux --disable-opengl --disable-alsa-midi --disable-dynamic-x86 --disable-fpu-x86 --enable-core-inline CXXFLAGS="-g -O2 -march=armv5te -mtune=arm926ej-s -pipe -fno-builtin -fno-common -ffast-math -fomit-frame-pointer -fexpensive-optimizations -frename-registers" LIBS="-lSDL_gfx -lSDL_image"


edit config.h

-/* #undef C_ATTRIBUTE_ALWAYS_INLINE */
+#define C_ATTRIBUTE_ALWAYS_INLINE 1

-/* #undef C_DYNREC */
+#define C_DYNREC 1

-#define C_IPX 1
+//#define C_IPX 1

-#define C_MODEM 1
+//#define C_MODEM 1

-#define C_TARGETCPU UNKNOWN
+#define C_TARGETCPU ARMV4LE

-/* #undef C_UNALIGNED_MEMORY */
+#define C_UNALIGNED_MEMORY 1

-#define C_X11_XKB 1
+//#define C_X11_XKB 1

edit dosbox.conf

fullscreen=false
fulldouble=false
fullresolution=original
windowresolution=original
output=surface_dingux      # surface_dingux only!
autolock=true
sensitivity=100
waitonerror=true
priority=higher,normal
mapperfile=mapper.txt
usescancodes=false         # false

scaler=none
