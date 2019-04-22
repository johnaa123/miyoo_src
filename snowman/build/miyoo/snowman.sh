#!/bin/sh
cd snowman
if [ ! -f /usr/lib/libsparrow3d.so ]; then
  mount -o remount rw /
  cp *.so /usr/lib/
  mount -o remount r /
fi
./snowman
