#!/bin/bash
# $Id$
# Create a archive 'gpw32-*.zip' (gpwYMMDD.zip) with Win32 (MinGW) build
# of the Golded+.

if [ ! "${TERM}" = "cygwin" ]
then
  echo "This is not Cygwin enviroment, exit."
  exit
fi

srcdatefile=srcdate.h
build=`sed -n 's/.*"\([[:digit:]]\{8\}\)".*/\1/p' $srcdatefile`
date="$build"
shortdate=${date/???/}
name=../gpw32-115-${date}.zip
shortname=../gpw${shortdate}.zip
binsuffix="cyg.exe"
binesdir="bin"
dizfile="$binesdir/file_id.diz"

echo Build a Golded+/w32mingw binary package:  ${name} and ${shortname}

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

compilerver=`gcc -v 2>&1 | sed -n -e '/^gcc version/{s/gcc version \([[:digit:].]\{2,\}\)\s\((\([[:alnum:]]\{2,\}\)\sspecial\).*/\1 \3/p;q}'`
printf 'GoldED+1.1.5 beta at %8.8s\r\n' $build  >${dizfile}
printf 'Win32 binaries compiled using\r\n' >>${dizfile}
printf 'MinGW GNU C/C++ %13.13s\r\n' "$compilerver" >>${dizfile}
printf 'This is  unstable release and\r\n' >>${dizfile}
printf 'it should be used for testing\r\n' >>${dizfile}
printf -- '-----------------------------\r\n' >>${dizfile}
printf ' *http://golded-plus.sf.net* \r\n' >>${dizfile}

for f in ${bines}; do rm $f; done
make clean
make
make strip
make docs

for i in ${bines} ; do
  if [ ! -f ${i} ] ; then echo "File ${i} not exists, stop!"; exit 1 ; fi
done

zip -9DXj ${name} ${dizfile} $files
ln ${name} ${shortname} || cp ${name} ${shortname}
