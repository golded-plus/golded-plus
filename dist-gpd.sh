#!/bin/bash
# $Id$
# Create a archive 'gpd*.zip' (gpdYMMDD.zip) with DOS/DPMI (DJGPP) build
# of the Golded+.

# To build, download following files:
#
# http://www.delorie.com/pub/djgpp/beta/v2/djdev204.zip
# http://www.delorie.com/pub/djgpp/beta/v2/djtzn204.zip
#
# http://www.delorie.com/pub/djgpp/beta/v2gnu/bnu219a.zip
# http://www.delorie.com/pub/djgpp/beta/v2gnu/bnu219b.zip
# http://www.delorie.com/pub/djgpp/beta/v2gnu/bsh205bbr3.zip
# http://www.delorie.com/pub/djgpp/beta/v2gnu/fil41b.zip
# http://www.delorie.com/pub/djgpp/beta/v2gnu/gcc442b.zip
# http://www.delorie.com/pub/djgpp/beta/v2gnu/gpp442b.zip
# http://www.delorie.com/pub/djgpp/beta/v2gnu/gro116b.zip
# http://www.delorie.com/pub/djgpp/beta/v2gnu/grp251ab.zip
# http://www.delorie.com/pub/djgpp/beta/v2gnu/licv192b.zip
# http://www.delorie.com/pub/djgpp/beta/v2gnu/mak381b.zip
# http://www.delorie.com/pub/djgpp/beta/v2gnu/sed421b.zip
# http://www.delorie.com/pub/djgpp/beta/v2gnu/shl2011b.zip
#
# http://www.delorie.com/pub/djgpp/current/v2misc/wmemu21b.zip
# http://www.delorie.com/pub/djgpp/current/v2tk/llocl02b.zip
#
# Next extract all these files into one directory (for example, C:\DJGPP\)
# Next set enviroment variable DJGPP to C:\DJGPP\DJGPP.ENV
# Last add the directory "C:\DJGPP\" into PATH enviroment variable.
# After this, run bash dist-gpd.sh, edir golded3/mygolded.h and run dist-gpd.sh second time.
#

if [ ! "$OSTYPE" = "msdosdjgpp" ]
then
  echo "This is not DJGPP enviroment, exit."
  exit
fi

srcdatefile=srcdate.h
build=`sed -n 's/.*"\([[:digit:]]\{8\}\)".*/\1/p' $srcdatefile`
date="$build"
#shortdate=`echo ${date} | sed s/^...//`
shortdate=${date/???/}
platform="djg"
binsuffix="${platform}.exe"
binesdir="bin"
dizfile="$binesdir/file_id.diz"
name=../gpd${shortdate}.zip
dosname=..\\gpd${shortdate}.zip

echo Build a Golded+/DPMI binary package:  ${name} and ${shortname}

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
files="${files} docs/tokencfg.txt docs/tokentpl.txt ${dizfile}"

compilerver=`gcc -v 2>&1 | sed -n -e '/^gcc version/{s/gcc version \([[:digit:].]\{2,\}\)\s\((\([[:alnum:]]\{2,\}\)\sspecial\).*/\1 \3/p;q}'`
printf 'GoldED+1.1.5 beta at %8.8s\r\n' $build  >${dizfile}
printf 'DOS32-DPMI binaries  compiled\r\n' >>${dizfile}
printf 'using DJGPP GNU C/C++ %-7.7s\r\n' "$compilerver" >>${dizfile}
printf 'This is  unstable release and\r\n' >>${dizfile}
printf 'it should be used for testing\r\n' >>${dizfile}
printf -- '-----------------------------\r\n' >>${dizfile}
printf 'https://github.com/golded-plus/\r\n' >>${dizfile}

for f in ${bines}; do rm $f; done

make PLATFORM=${platform} clean all strip docs || (echo "Error $?" ; exit)

for i in ${bines} ; do
  if [ ! -f ${i} ] ; then echo "File ${i} not exists, stop!"; exit 1 ; fi
done

#zip -9DXj ${name} $files
#7za a -tzip ${name} $files

echo ${files} | sed "s/ /\n/g" >files.lst
pkzip ${dosname} @files.lst
rm files.lst
