#!/bin/sh
# $Id$
# Create a archive 'gpd*.zip' (gpdYMMDD.zip) with DOS/DPMI (DJGPP) build
# of the Golded+.

# To build, download from http://www.delorie.com/pub/djgpp/current/ following files:
#   djdev203.zip
#   djtzn203.zip
# And from http://www.delorie.com/pub/djgpp/current/v2gnu/ following more files:
#   bnu219a.zip
#   bnu219b.zip
#   bsh204br3.zip
#   fil41b.zip
#   gcc442b.zip
#   gpp442b.zip
#   grp251ab.zip
#   gro116b.zip
#   licv192b.zip
#   mak3791b.zip
#   sed421b.zip
# And from http://www.delorie.com/pub/djgpp/current/v2misc/ following more files:
#   wmemu21b.zip
# And from http://www.delorie.com/pub/djgpp/current/v2tk/ following more files:
#   llocl02b.zip
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

date=`date +%Y%m%d`
shortdate=`echo ${date} | sed s/^...//`
name=../gpd${shortdate}.zip
dosname=..\\gpd${shortdate}.zip
file_id=bin/File_ID.Diz

echo Build a Golded+/DPMI binary package:  ${name} and ${shortname}

if [ ! -f golded3/mygolded.h ]; then
  cp golded3/mygolded.__h golded3/mygolded.h
  echo "golded3/mygolded.h is created now. Please edit this file"
  exit 1
fi
sed -i.orig -e "s/\#define __GVER_POSTVERSION__ .*/\#define __GVER_POSTVERSION__   \"-b${date}\"/" golded3/mygolded.h

bines="bin/geddjg.exe bin/gndjg.exe bin/rddtdjg.exe"
files="${bines} docs/copying docs/copying.lib golded.bat"
files="${files} docs/golded.html docs/golded.txt docs/goldnode.html"
files="${files} docs/goldnode.txt docs/license.txt docs/notework.txt"
files="${files} docs/rddt.html docs/rddt.txt docs/readme.txt"
files="${files} docs/rusfaq.txt docs/tips.txt docs/todowork.txt"
files="${files} docs/tokencfg.txt docs/tokentpl.txt ${file_id}"

echo 'GoldED+1.1.5   [DOS-DPMI binaries]'   >${file_id}
echo '[Compiled  using  DJGPP GNU C/C++]'  >>${file_id}
echo "Snapshot at ${date} (devel.ver.)"    >>${file_id}
echo 'This is  unstable release  and  it'  >>${file_id}
echo 'should be used for testing purpose'  >>${file_id}
echo '----------------------------------'  >>${file_id}
echo 'GoldED+  is  a  successor  of  the'  >>${file_id}
echo 'wellknown GoldED mail editor.'       >>${file_id}
echo '----------------------------------'  >>${file_id}
echo 'http://golded-plus.sourceforge.net'  >>${file_id}

rm ${bines}

make PLATFORM=djg clean
make PLATFORM=djg
make PLATFORM=djg strip
make docs

for i in ${bines} ; do
  if [ ! -f ${i} ] ; then echo "File ${i} not exists, stop!"; exit 1 ; fi
done

#zip -9DXj ${name} ${file_id} $files
#7za a -tzip ${name} ${file_id} $files

echo ${files} | sed "s/ /\n/g" >files.lst
pkzip ${dosname} @files.lst
rm files.lst
