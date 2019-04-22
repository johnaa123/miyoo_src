#!/bin/sh
rm -rf ipkg
mkdir -p ipkg/mnt/apps/wizwrite
mkdir -p ipkg/mnt/gmenu2x/sections/applications
cp wizwrite ../out
cp -a ../out ipkg/mnt/apps/wizwrite

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
print 'Package: wizwrite
Version: 20190313
Description: Writer for Miyoo
Section: apps
Priority: optional
Maintainer: Steward
Architecture: ARM
Homepage: https://github.com/steward-fu/miyoo_src/wizwrite
Depends:
Source: https://github.com/steward-fu/miyoo_src/wizwrite' > control

print '1.0' > debian-binary
print 'title=Writer
description=wizwrite apps
exec=/mnt/apps/wizwrite/wizwrite
clock=702
' > mnt/gmenu2x/sections/applications/wizwrite

tar -czvf control.tar.gz control --owner=0 --group=0
tar -czvf data.tar.gz mnt --owner=0 --group=0
ar rv ../wizwrite.ipk control.tar.gz data.tar.gz debian-binary

cd ..
rm -r ipkg
