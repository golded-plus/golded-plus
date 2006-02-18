#!/bin/sh

# Create a archive 'gpwin-*.zip' (gpvYMMDD.zip) with Win32 (MS Visual C)
# builg of the Golded+.

date=`date +%Y%m%d`
name=../gpwin-115-${date}.zip
shortdate=`echo ${date} | sed s/^...//`
shortname=../gpv${shortdate}.zip

echo Build a Golded+/w32mvc binary package:  ${name} and ${shortname}

sed -i.orig -e "s/\#define __GVER_POSTVERSION__ .*/\#define __GVER_POSTVERSION__   \"-b${date}\"/" golded3/mygolded.h

files="Release/bin/gedwin.exe Release/bin/gnwin.exe Release/bin/rddtwin.exe"
files="${files} docs/copying docs/copying.lib golded.bat"
files="${files} docs/golded.html docs/golded.txt docs/goldnode.html"
files="${files} docs/goldnode.txt docs/license.txt docs/notework.txt"
files="${files} docs/rddt.html docs/rddt.txt docs/readme.txt"
files="${files} docs/rusfaq.txt docs/tips.txt docs/todowork.txt"
files="${files} docs/tokencfg.txt docs/tokentpl.txt"

nmake /f golded.mak CFG="Release" clean
nmake /f golded.mak CFG="Release"

printf 'GoldED+1.1.5  [Win32 binaries]\r\n'  >Release/bin/File_ID.Diz
printf '[Compiled using MS Visual C++]\r\n' >>Release/bin/File_ID.Diz
printf 'Snapshot (development version)\r\n' >>Release/bin/File_ID.Diz
printf 'This is  unstable release  and\r\n' >>Release/bin/File_ID.Diz
printf 'it should be  used for testing\r\n' >>Release/bin/File_ID.Diz
printf '------------------------------\r\n' >>Release/bin/File_ID.Diz
printf 'GoldED+  is a successor of the\r\n' >>Release/bin/File_ID.Diz
printf 'wellknown  GoldED mail editor.\r\n' >>Release/bin/File_ID.Diz
printf '------------------------------\r\n' >>Release/bin/File_ID.Diz
printf ' *http://golded-plus.sf.net*  \r\n' >>Release/bin/File_ID.Diz

make docs
zip -9DXj ${name} Release/bin/File_ID.Diz $files
cp ${name} ${shortname}
