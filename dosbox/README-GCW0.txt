
How to compile DosBox for GCW-Zero:

./autogen.sh
./configure --host=mipsel-linux --disable-opengl --disable-alsa-midi --disable-dynamic-x86 --disable-fpu-x86 --enable-core-inline CXXFLAGS="-g -O2 -G0 -march=mips32 -mtune=mips32r2 -pipe -fno-builtin -fno-common -mno-shared -ffast-math -fomit-frame-pointer -fexpensive-optimizations -frename-registers" LIBS="-lSDL_gfx -lSDL_image"

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
+#define C_TARGETCPU MIPSEL

-/* #undef C_UNALIGNED_MEMORY */
+#define C_UNALIGNED_MEMORY 1


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