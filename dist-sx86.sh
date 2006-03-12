#!/bin/sh

# Create an archive 'gpsunx86-115-*.zip' (gpsunx86-115-YMMDD.zip)
# with binary SunOS x86 build installable package

pkgdir=bin/pkg
vendor=`sed -n -e "/^#define __GVER_VENDOR_NAME__/s/#define __GVER_VENDOR_NAME__ \"\(.*\)\"/\1/p" golded3/mygolded.h`
email=`sed -n -e "/^#define __GVER_VENDOR_EMAIL__/s/#define __GVER_VENDOR_EMAIL__ \"\(.*\)\"/\1/p" golded3/mygolded.h`
date=`date +%Y%m%d`
name=bin/gpsunx86-115-${date}.zip
pkgname=golded-plus-x86-115-${date}.pkg

echo Build a Golded+/sunX86 binary package:  ${name} \(${pkgname}\)

if [ ! -f golded3/mygolded.h ]; then
  cp golded3/mygolded.__h golded3/mygolded.h
  echo "golded3/mygolded.h is created now. Please edit this file"
  exit 1
fi
gsed -i.orig -e "s/\#define __GVER_POSTVERSION__ .*/\#define __GVER_POSTVERSION__   \"-b${date}\"/" golded3/mygolded.h

if [ ! -d "bin" ] ; then mkdir bin; fi

printf "GoldED+1.1.5 [`uname` `uname -i` pkg]\r\n"  >bin/File_ID.Diz
printf 'Snapshot (development version)\r\n' >>bin/File_ID.Diz
printf 'This is  unstable release  and\r\n' >>bin/File_ID.Diz
printf 'it should be used for testing.\r\n' >>bin/File_ID.Diz
printf -- '------------------------------\r\n' >>bin/File_ID.Diz
printf 'GoldED+ is a  successor of the\r\n' >>bin/File_ID.Diz
printf 'wellknown  GoldED mail editor.\r\n' >>bin/File_ID.Diz
printf -- '------------------------------\r\n' >>bin/File_ID.Diz
printf '*golded-plus.sourceforge.net* \r\n' >>bin/File_ID.Diz

# make binaries

gmake PLATFORM=sun clean
gmake PLATFORM=sun
gmake PLATFORM=sun strip
cd docs
gmake tokentpl.txt tokencfg.txt
cd ..

for i in bin/gedsun bin/gnsun bin/rddtsun ; do
  if [ ! -f ${i} ] ; then echo "File ${i} not exists, stop!"; exit 1 ; fi
done

# creating package
if [ -d "$pkgdir" ] ; then rm -rf $pkgdir ; fi
mkdir $pkgdir
(echo "i pkginfo=./pkginfo" ;\
     echo "i depend=./depend" ;\
     echo "d none bin 0755 root root" ;\
     echo "d none share 0755 root root" ;\
     echo "d none share/golded+ 0755 root root" ;\
     echo "d none share/man 0755 root root" ;\
     echo "d none share/man/man1 0755 root root" ;\
     ((pkgproto bin/gedsun=bin/golded bin/gnsun=bin/goldnode bin/rddtsun=bin/rddt ;\
     for i in docs/*.1 ; do pkgproto $i=share/man/man1/`basename $i` ; done ;\
     pkgproto cfgs=share/golded+/cfgs ;\
     (pkgproto docs=share/golded+/docs | grep -v building.txt | grep -v linux | grep -v "\.1=" | grep -v "\.html=") ;\
     (pkgproto manuals=share/golded+/manuals | grep -v "\.tei=") ) | grep -v CVS | grep -v File_id.diz | grep -v Makefile |\
         gsed "s/\.\(chs\|bbs\|cfg\|esc\) 0755 /.\1 0644 /" | awk '{print $1 " " $2 " " $3 " " $4 " root root"}' )) \
    >$pkgdir/Prototype
echo 'P      CSWncurses   ncurses - ncurses library and utilities' > $pkgdir/depend
echo 'ARCH="i386"' > $pkgdir/pkginfo
echo 'CATEGORY="application"' >> $pkgdir/pkginfo
echo 'NAME="GoldED+1.1.5 - FTN mail editor"' >> $pkgdir/pkginfo
echo 'PKG="CSWgolded+"' >> $pkgdir/pkginfo
echo 'CLASSES="none"' >> $pkgdir/pkginfo
echo 'VERSION="1.1.5"' >> $pkgdir/pkginfo
echo 'BASEDIR="/opt/csw"' >> $pkgdir/pkginfo
echo "EMAIL=\"$email\"" >> $pkgdir/pkginfo
echo "VENDOR=\"http://golded-plus.sourceforge.net/ packaged by $vendor\"" >> $pkgdir/pkginfo
echo 'HOTLINE="http://golded-plus.sourceforge.net"' >> $pkgdir/pkginfo
echo 'MAXINST="1"' >> $pkgdir/pkginfo
cd $pkgdir
pkgmk -o -d . -f ./Prototype -r ../..
echo 1|pkgtrans -s . ../$pkgname
cd ../..
rm -rf $pkgdir

# zipping
zip -9DXj ${name} bin/File_ID.Diz bin/$pkgname
