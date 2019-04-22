#!/bin/sh

mkdir -p ipkg/home/retrofw/emus/wqx
mkdir -p ipkg/home/retrofw/apps/gmenu2x/sections/emulators
cp -a borders ipkg/home/retrofw/emus/wqx/
cp -a roms ipkg/home/retrofw/emus/wqx/
cp wqx ipkg/home/retrofw/emus/wqx/
cp icon.png ipkg/home/retrofw/emus/wqx/wqx.png

cd ipkg

# https://unix.stackexchange.com/questions/219268/how-to-add-new-lines-when-using-echo
print()
	case    ${IFS- } in
	(\ *)   printf  %b\\n "$*";;
	(*)     IFS=\ $IFS
	printf  %b\\n "$*"
	IFS=${IFS#?}
esac

# Create GmenuNx entry file plus other things

print 'Package: WQX
Version: 20190321
Description: WQX NC1020 Emulator
Section: emus
Priority: optional
Maintainer: Steward
Architecture: mipsel
Homepage: https://github.com/steward-fu
Depends:
Source: https://github.com/steward-fu' > control

print '2.0' > debian-binary

print 'title=WQX
description=WQX NC1020
exec=/home/retrofw/emus/wqx/wqx
clock=600'> home/retrofw/apps/gmenu2x/sections/emulators/wqx

tar -czvf control.tar.gz control --owner=0 --group=0
tar -czvf data.tar.gz home --owner=0 --group=0
ar rv ../wqx.ipk control.tar.gz data.tar.gz debian-binary

cd ..
rm -r ipkg
