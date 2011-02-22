#!/bin/sh

# Create a archive 'gpwin-*.zip' (gpvYMMDD.zip) with Win32 (MS Visual C)
# builg of the Golded+.

date=`date +%Y%m%d`
name=../gpwin-115-${date}.zip
shortdate=`echo ${date} | sed s/^...//`
shortname=../gpv${shortdate}.zip
dizfile=Release/bin/file_id.diz

echo Build a Golded+/w32mvc binary package:  ${name} and ${shortname}

if [ ! -f golded3/mygolded.h ]; then
  cp golded3/mygolded.__h golded3/mygolded.h
  echo "golded3/mygolded.h is created now. Please edit this file"
  exit 1
fi
#sed -i.orig -e "s/\#define __GVER_POSTVERSION__ .*/\#define __GVER_POSTVERSION__   \"-b${date}\"/" golded3/mygolded.h

bines="Release/bin/gedwin.exe Release/bin/gnwin.exe Release/bin/rddtwin.exe"
files="${bines} docs/copying docs/copying.lib golded.bat"
files="${files} docs/golded.html docs/golded.txt docs/goldnode.html"
files="${files} docs/goldnode.txt docs/license.txt docs/notework.txt"
files="${files} docs/rddt.html docs/rddt.txt docs/readme.txt docs/notework.rus"
files="${files} docs/rusfaq.txt docs/tips.txt docs/todowork.txt"
files="${files} docs/tokencfg.txt docs/tokentpl.txt"

rm ${bines}
nmake /f gedwin.mak CFG="Release" clean
nmake /f gedwin.mak CFG="Release"

printf 'GoldED+1.1.5  [Win32 binaries]\r\n'  >${dizfile}
printf '[Compiled using MS Visual C++]\r\n' >>${dizfile}
printf 'Snapshot (development version)\r\n' >>${dizfile}
printf 'This is  unstable release  and\r\n' >>${dizfile}
printf 'it should be  used for testing\r\n' >>${dizfile}
printf -- '------------------------------\r\n' >>${dizfile}
printf 'GoldED+  is a successor of the\r\n' >>${dizfile}
printf 'wellknown  GoldED mail editor.\r\n' >>${dizfile}
printf -- '------------------------------\r\n' >>${dizfile}
printf ' *http://golded-plus.sf.net*  \r\n' >>${dizfile}

make docs

for i in ${bines} ; do
  if [ ! -f ${i} ] ; then echo "File ${i} not exists, stop!"; exit 1 ; fi
done

zip -9DXj ${name} ${dizfile} $files
ln ${name} ${shortname} || cp ${name} ${shortname}
