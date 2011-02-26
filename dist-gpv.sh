#!/bin/bash
# $Id$
# Create a archive 'gpwin-*.zip' (gpvYMMDD.zip) with Win32 (MS Visual C)
# build of the Golded+.

srcdatefile=srcdate.h
build=`sed -n 's/.*"\([[:digit:]]\{8\}\)".*/\1/p' $srcdatefile`
date="$build"
name=../gpwin-115-${date}.zip
shortdate=${date/???/}
shortname=../gpv${shortdate}.zip
binesdir="windows/bin/release/win32"
binsuffix="win.exe"
dizfile="$binesdir/file_id.diz"

echo Build a Golded+/w32mvc binary package:  ${name} and ${shortname}

if [ ! -f golded3/mygolded.h ]; then
  cp golded3/mygolded.__h golded3/mygolded.h
  echo "golded3/mygolded.h is created now. Please edit this file"
  exit 1
fi

bines="${binesdir}/ged${binsuffix} ${binesdir}/gn${binsuffix} ${binesdir}/rddt${binsuffix}"
files="${bines} docs/copying docs/copying.lib golded.bat"
files="${files} docs/golded.html docs/golded.txt docs/goldnode.html"
files="${files} docs/goldnode.txt docs/license.txt docs/notework.txt"
files="${files} docs/rddt.html docs/rddt.txt docs/readme.txt docs/notework.rus"
files="${files} docs/rusfaq.txt docs/tips.txt docs/todowork.txt"
files="${files} docs/tokencfg.txt docs/tokentpl.txt"

VSver=`nmake /v 2>&1 | sed -ne 's/.*Version \([0-9]\{1,\}\.[0-9]\{1,\}\).*/\1/p'`
printf 'GoldED+1.1.5 beta at %8.8s\r\n' $build  >${dizfile}
printf 'Win32 binaries compiled using\r\n' >>${dizfile}
printf 'Microsoft Visual Studio %5.5s\r\n' $VSver >>${dizfile}
printf 'This is  unstable release and\r\n' >>${dizfile}
printf 'it should be used for testing\r\n' >>${dizfile}
printf -- '------------------------------\r\n' >>${dizfile}
printf ' *http://golded-plus.sf.net*  \r\n' >>${dizfile}

#for f in ${bines}; do rm $f; done
#nmake /f gedwin.mak CFG="Release" clean
#nmake /f gedwin.mak CFG="Release"

make docs

for i in ${bines} ; do
  if [ ! -f ${i} ] ; then echo "File ${i} not exists, stop!"; exit 1 ; fi
done

zip -9DXj ${name} ${dizfile} $files
ln ${name} ${shortname} || cp ${name} ${shortname}
