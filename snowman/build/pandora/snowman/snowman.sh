#!/bin/sh
export PATH=":${PATH:-"/usr/bin:/bin:/usr/local/bin"}"
export LD_LIBRARY_PATH=":${LD_LIBRARY_PATH:-"/usr/lib:/lib"}"
export HOME="/mnt/utmp/snowman-reloaded" XDG_CONFIG_HOME="/mnt/utmp/snowman-reloaded"

if [ -d /mnt/utmp/snowman/share ];then
	export XDG_DATA_DIRS=/mnt/utmp/snowman-reloaded/share:$XDG_DATA_DIRS:/usr/share
fi
export SDL_AUDIODRIVER="alsa"

#cat /proc/pandora/nub0/mode > /tmp/nub0mode_before_snowman
#cat /proc/pandora/nub1/mode > /tmp/nub1mode_before_snowman
#echo absolute > /proc/pandora/nub0/mode
#echo absolute > /proc/pandora/nub1/mode

LD_LIBRARY_PATH=. ./snowman $*

#cat /tmp/nub0mode_before_snowman > /proc/pandora/nub0/mode
#cat /tmp/nub1mode_before_snowman > /proc/pandora/nub1/mode
 
#rm /tmp/nub0mode_before_snowman /tmp/nub1mode_before_snowman
