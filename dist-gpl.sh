#!/bin/bash
# $Id$
# Create a archive 'gpl-*.zip' (gplYMMDD.zip) with binary linux build
# of the Golded+.

srcdatefile=srcdate.h
build=`sed -n 's/.*"\([[:digit:]]\{8\}\)".*/\1/p' $srcdatefile`
date="$build"
shortdate=${date/???/}

platform="lnx"
binsuffix="lnx"
binesdir="bin"
dizfile="$binesdir/File_ID.Diz"
shortname=../gpl${shortdate}.zip
name=../gpl115-${date}.zip
requirements_file="${binesdir}/requirements.txt"

echo Build a Golded+/lnx binary package:  ${name} and ${shortname}

if [ ! -f golded3/mygolded.h ]; then
  cp golded3/mygolded.__h golded3/mygolded.h
  echo "golded3/mygolded.h is created now. Please edit this file"
  exit 1
fi

bines="${binesdir}/ged${binsuffix} ${binesdir}/gn${binsuffix} ${binesdir}/rddt${binsuffix}"
files="${bines} docs/copying docs/copying.lib ${requirements_file}"
files="${files} bin/golded bin/openbsd-vt220.sh  ${dizfile}"
files="${files} docs/golded.html docs/golded.txt docs/goldnode.html"
files="${files} docs/goldnode.txt docs/license.txt docs/notework.txt"
files="${files} docs/rddt.html docs/rddt.txt docs/readme.txt docs/notework.rus"
files="${files} docs/rusfaq.txt docs/rusfaq.koi8r docs/rusfaq.utf8 docs/tips.txt docs/todowork.txt"
files="${files} docs/tokencfg.txt docs/tokentpl.txt"
files="${files} docs/notework.rus docs/notework.koi8r docs/notework.utf8"

compilerver=`gcc -v 2>&1 | sed -n -e '/^gcc version/{s/gcc version \([[:digit:].]\{2,\}\)\s\((\([[:alnum:]]\{2,\}\)\sspecial\).*/\1 \3/p;q}'`
printf 'GoldED+1.1.5 beta at %8.8s\r\n' $build  >${dizfile}
printf '%-5s binaries compiled using\r\n' "`uname`" >>${dizfile}
printf 'GNU C/C++ %13.13s\r\n' "$compilerver" >>${dizfile}
printf 'This is  unstable release and\r\n' >>${dizfile}
printf 'it should be used for testing\r\n' >>${dizfile}
printf -- '-----------------------------\r\n' >>${dizfile}
printf ' *http://golded-plus.sf.net* \r\n' >>${dizfile}

for f in ${bines}; do rm $f; done
make PLATFORM=${platform} clean
make PLATFORM=${platform}
make PLATFORM=${platform} strip
make docs

for i in ${bines} ; do
  if [ ! -f ${i} ] ; then echo "File ${i} not exists, stop!"; exit 1 ; fi
done

iconv -c -f cp866 -t koi8-r docs/rusfaq.txt | sed 2s/cp866/koi8-r/ >docs/rusfaq.koi8r
iconv -c -f cp866 -t utf8 docs/rusfaq.txt |  sed 2s/cp866/utf-8/ >docs/rusfaq.utf8

iconv -c -f cp866 -t koi8-r docs/notework.rus | sed 2s/cp866/koi8-r/ >docs/notework.koi8r
iconv -c -f cp866 -t utf8 docs/notework.rus |  sed 2s/cp866/utf-8/ >docs/notework.utf8

echo Required libraries:  >bin/requirements.txt
ldd ${binesdir}/ged${binsuffix} | sed -e "s/ =.*//" -e /\\/lib\\//d -e "s/\\.so\\./ /" >>${requirements_file}

zip -9DXj ${name} $files
ln ${name} ${shortname} || cp ${name} ${shortname}
