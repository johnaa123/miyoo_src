#!/bin/sh

./configure --host=mipsel-linux --disable-opengl --disable-alsa-midi --disable-dynamic-x86 --disable-fpu-x86 --enable-core-inline CXXFLAGS="-g -O2 -G0 -march=mips32 -mtune=mips32 -pipe -fno-builtin -fno-common -mno-shared -ffast-math -fomit-frame-pointer -fexpensive-optimizations -frename-registers" LIBS="-lSDL_gfx -lSDL_image"

OLD="\/\* #undef C_ATTRIBUTE_ALWAYS_INLINE \*\/"
NEW="#define C_ATTRIBUTE_ALWAYS_INLINE 1"
sed -i "s/$OLD/$NEW/g" config.h

OLD="\/\* #undef C_DYNREC \*\/"
NEW="#define C_DYNREC 1"
sed -i "s/$OLD/$NEW/g" config.h

OLD="#define C_TARGETCPU UNKNOWN"
NEW="#define C_TARGETCPU MIPSEL"
sed -i "s/$OLD/$NEW/g" config.h

OLD="\/\* #undef C_UNALIGNED_MEMORY \*\/"
NEW="#define C_UNALIGNED_MEMORY 1"
sed -i "s/$OLD/$NEW/g" config.h
