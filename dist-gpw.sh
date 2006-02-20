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

echo Build a Golded+/w32mingw binary package:  ${name} and ${shortname}

if [ ! -f golded3/mygolded.h ]; then
  cp golded3/mygolded.__h golded3/mygolded.h
fi
sed -i.orig -e "s/\#define __GVER_POSTVERSION__ .*/\#define __GVER_POSTVERSION__   \"-b${date}\"/" golded3/mygolded.h

bines="bin/gedcyg.exe bin/gncyg.exe bin/rddtcyg.exe"
files="${bines} docs/copying docs/copying.lib golded.bat"
files="${files} docs/golded.html docs/golded.txt docs/goldnode.html"
files="${files} docs/goldnode.txt docs/license.txt docs/notework.txt"
files="${files} docs/rddt.html docs/rddt.txt docs/readme.txt"
files="${files} docs/rusfaq.txt docs/tips.txt docs/todowork.txt"
files="${files} docs/tokencfg.txt docs/tokentpl.txt"

printf 'GoldED+1.1.5  [Win32 binaries]\r\n'  >bin/File_ID.Diz
printf '[Compiled using  MinGW/Cygwin]\r\n' >>bin/File_ID.Diz
printf 'Snapshot (development version)\r\n' >>bin/File_ID.Diz
printf 'This is  unstable release  and\r\n' >>bin/File_ID.Diz
printf 'it should be used for testing.\r\n' >>bin/File_ID.Diz
printf '------------------------------\r\n' >>bin/File_ID.Diz
printf 'GoldED+ is a  successor of the\r\n' >>bin/File_ID.Diz
printf 'wellknown  GoldED mail editor.\r\n' >>bin/File_ID.Diz
printf '------------------------------\r\n' >>bin/File_ID.Diz
printf '*golded-plus.sourceforge.net* \r\n' >>bin/File_ID.Diz

rm bin/*cyg.exe
make clean
make
make strip
make docs

for i in ${bines} ; do
  if [ ! -f ${i} ] ; then echo "File ${i} not exists, stop!"; exit 1 ; done
done

zip -9DXj ${name} bin/File_ID.Diz $files
cp ${name} ${shortname}
