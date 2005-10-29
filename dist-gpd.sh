#!/bin/sh

# Create a archive 'gpd-*.zip' (gpdYMMDD.zip) with DOS/DPMI (DJGPP) build
# of the Golded+.

if [ ! "$OSTYPE" = "msdosdjgpp" ]
then
  echo "This is not DJGPP enviroment, exit."
  exit
fi

date=`date +%Y%m%d`
shortdate=`echo ${date} | sed s/^...//`
name=../gpd${shortdate}.zip
dosname=..\\gpd${shortdate}.zip
file_id=bin/File_ID.Diz

sed -i.orig -e "s/\#define __GVER_POSTVERSION__ .*/\#define __GVER_POSTVERSION__   \"-b${date}\"/" golded3/mygolded.h

files="bin/geddjg.exe bin/gndjg.exe bin/rddtdjg.exe"
files="${files} docs/copying docs/copying.lib"
files="${files} docs/golded.html docs/golded.txt docs/goldnode.html"
files="${files} docs/goldnode.txt docs/license.txt docs/notework.txt"
files="${files} docs/rddt.html docs/rddt.txt docs/readme.txt"
files="${files} docs/rusfaq.txt docs/tips.txt docs/todowork.txt"
files="${files} docs/tokencfg.txt docs/tokentpl.txt ${file_id}"

echo 'GoldED+1.1.5   [DOS-DPMI binaries]'   >${file_id}
echo '[Compiled  using  DJGPP GNU C/C++]'  >>${file_id}
echo "Snapshot at ${date} (devel. ver.)" >>${file_id}
echo 'This is  unstable release  and  it'  >>${file_id}
echo 'should be used for testing purpose'  >>${file_id}
echo '----------------------------------'  >>${file_id}
echo 'http://golded-plus.sourceforge.net'  >>${file_id}

#make PLATFORM=djg clean
#make PLATFORM=djg
#make PLATFORM=djg strip
#make docs
#zip -9DXj ${name} ${file_id} $files
#7za a -tzip ${name} ${file_id} $files

echo ${files} | sed "s/ /\n/g" >files.lst
pkzip ${dosname} @files.lst