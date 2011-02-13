#!/bin/sh

# Create a archive 'gpw32-*.zip' (gpwYMMDD.zip) with Win32 (MinGW) build
# of the Golded+.

if [ ! "${TERM}" = "cygwin" ]
then
  echo "This is not Cygwin enviroment, exit."
  exit
fi

date=`date +%Y%m%d`
shortdate=`echo ${date} | sed s/^...//`
name=../gpw32-115-${date}.zip
shortname=../gpw${shortdate}.zip
dizfile=bin/file_id.diz

echo Build a Golded+/w32mingw binary package:  ${name} and ${shortname}

if [ ! -f golded3/mygolded.h ]; then
  cp golded3/mygolded.__h golded3/mygolded.h
  echo "golded3/mygolded.h is created now. Please edit this file"
  exit 1
fi
#sed -i.orig -e "s/\#define __GVER_POSTVERSION__ .*/\#define __GVER_POSTVERSION__   \"-b${date}\"/" golded3/mygolded.h

bines="bin/gedcyg.exe bin/gncyg.exe bin/rddtcyg.exe"
files="${bines} docs/copying docs/copying.lib golded.bat"
files="${files} docs/golded.html docs/golded.txt docs/goldnode.html"
files="${files} docs/goldnode.txt docs/license.txt docs/notework.txt"
files="${files} docs/rddt.html docs/rddt.txt docs/readme.txt"
files="${files} docs/rusfaq.txt docs/tips.txt docs/todowork.txt"
files="${files} docs/tokencfg.txt docs/tokentpl.txt"

printf 'GoldED+1.1.5  [Win32 binaries]\r\n'  >${dizfile}
printf '[Compiled using  MinGW/Cygwin]\r\n' >>${dizfile}
printf 'Snapshot (development version)\r\n' >>${dizfile}
printf 'This is  unstable release  and\r\n' >>${dizfile}
printf 'it should be used for testing.\r\n' >>${dizfile}
printf -- '------------------------------\r\n' >>${dizfile}
printf 'GoldED+ is a  successor of the\r\n' >>${dizfile}
printf 'wellknown  GoldED mail editor.\r\n' >>${dizfile}
printf -- '------------------------------\r\n' >>${dizfile}
printf '*golded-plus.sourceforge.net* \r\n' >>${dizfile}

rm bin/*cyg.exe
make clean
make
make strip
make docs

for i in ${bines} ; do
  if [ ! -f ${i} ] ; then echo "File ${i} not exists, stop!"; exit 1 ; fi
done

zip -9DXj ${name} ${dizfile} $files
ln ${name} ${shortname} || cp ${name} ${shortname}
