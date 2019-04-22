#!/bin/sh
export PATH=":${PATH:-"/usr/bin:/bin:/usr/local/bin"}"
export LD_LIBRARY_PATH=":${LD_LIBRARY_PATH:-"/usr/lib:/lib"}"
export HOME="/mnt/utmp/glutexto" XDG_CONFIG_HOME="/mnt/utmp/glutexto"

if [ -d /mnt/utmp/glutexto/share ];then
	export XDG_DATA_DIRS=/mnt/utmp/glutexto/share:$XDG_DATA_DIRS:/usr/share
fi
export SDL_AUDIODRIVER="alsa"

LD_LIBRARY_PATH=. ./glutexto $*
