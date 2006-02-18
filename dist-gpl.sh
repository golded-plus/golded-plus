#!/bin/sh

# Create a archive 'gpl-*.zip' (gplYMMDD.zip) with binary linux build
# of the Golded+.

date=`date +%Y%m%d`
shortdate=`echo ${date} | sed s/^...//`
shortname=../gpl${shortdate}.zip
name=../gpl115-${date}.zip

echo Build a Golded+/lnx binary package:  ${name} and ${shortname}

if [ ! -f golded3/mygolded.h ]; then
  cp golded3/mygolded.__h golded3/mygolded.h
fi
sed -i.orig -e "s/\#define __GVER_POSTVERSION__ .*/\#define __GVER_POSTVERSION__   \"-b${date}\"/" golded3/mygolded.h

files="bin/gedlnx bin/gnlnx bin/rddtlnx"
files="${files} docs/copying docs/copying.lib"
files="${files} docs/golded.html docs/golded.txt docs/goldnode.html"
files="${files} docs/goldnode.txt docs/license.txt docs/notework.txt"
files="${files} docs/rddt.html docs/rddt.txt docs/readme.txt"
files="${files} docs/rusfaq.txt docs/tips.txt docs/todowork.txt"
files="${files} docs/tokencfg.txt docs/tokentpl.txt"

printf 'GoldED+1.1.5 [`uname` binaries]\r\n'  >bin/File_ID.Diz
printf 'Snapshot (development version)\r\n' >>bin/File_ID.Diz
printf 'This is  unstable release  and\r\n' >>bin/File_ID.Diz
printf 'it should be used for testing.\r\n' >>bin/File_ID.Diz
printf '------------------------------\r\n' >>bin/File_ID.Diz
printf 'GoldED+ is a  successor of the\r\n' >>bin/File_ID.Diz
printf 'wellknown  GoldED mail editor.\r\n' >>bin/File_ID.Diz
printf '------------------------------\r\n' >>bin/File_ID.Diz
printf '*golded-plus.sourceforge.net* \r\n' >>bin/File_ID.Diz

make PLATFORM=lnx clean
make PLATFORM=lnx
make PLATFORM=lnx strip
make docs
zip -9DXj ${name} bin/File_ID.Diz $files
cp ${name} ${shortname}
