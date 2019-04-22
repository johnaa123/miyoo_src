#!/bin/sh
rm miyoobuild -rf
mkdir miyoobuild
cd miyoobuild
cmake -DCMAKE_TOOLCHAIN_FILE="/opt/miyoo/share/buildroot/toolchainfile.cmake" ..
make

