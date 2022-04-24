#!/bin/bash
# $Id$
# Create an archive 'gpsunx86-115-*.zip' (gpsunx86-115-YMMDD.zip)
# with binary SunOS x86 build installable package

srcdatefile=srcdate.h
build=`sed -n 's/.*"\([[:digit:]]\{8\}\)".*/\1/p' $srcdatefile`
date="$build"
#shortdate=`echo ${date} | sed s/^...//`
shortdate=${date/???/}
binsuffix="sun"
platform="sun"
binesdir="bin"
dizfile="${binesdir}/file_id.diz"
pkgdir=${binesdir}/pkg
vendor=`sed -n -e "/^#define __GVER_VENDOR_NAME__/s/#define __GVER_VENDOR_NAME__ \"\(.*\)\"/\1/p" golded3/mygolded.h`
email=`sed -n -e "/^#define __GVER_VENDOR_EMAIL__/s/#define __GVER_VENDOR_EMAIL__ \"\(.*\)\"/\1/p" golded3/mygolded.h`
name=${binesdir}/gpsunx86-115-${date}.zip
pkgname=golded-plus-x86-115-${date}.pkg
bines="${binesdir}/ged${binsuffix} ${binesdir}/gn${binsuffix} ${binesdir}/rddt${binsuffix}"

echo Build a Golded+/sunX86 binary package:  ${name} \(${pkgname}\)

if [ ! -f golded3/mygolded.h ]; then
  cp golded3/mygolded.__h golded3/mygolded.h
  echo "golded3/mygolded.h is created now. Please edit this file"
  exit 1
fi

if [ ! -d "${binesdir}" ] ; then mkdir ${binesdir}; fi

compilerver=`gcc -v 2>&1 | sed -n -e '/^gcc version/{s/gcc version \([[:digit:].]\{2,\}\)\s\((\([[:alnum:]]\{2,\}\)\sspecial\).*/\1 \3/p;q}'`
printf 'GoldED+1.1.5 beta at %8.8s\r\n' $build  >${dizfile}

printf "`uname` `uname -i` pkg compiled\r\n"  >${dizfile}
printf 'using GNU C/C++ %13.13s\r\n' "$compilerver" >>${dizfile}
printf 'This is  unstable release and\r\n' >>${dizfile}
printf 'it should be used for testing\r\n' >>${dizfile}
printf -- '-----------------------------\r\n' >>${dizfile}
printf 'https://github.com/golded-plus/\r\n' >>${dizfile}

# make binaries

gmake PLATFORM=${platform} clean all strip || (echo "Error $?" ; exit)
cd docs
gmake tokentpl.txt tokencfg.txt
cd ..

for i in ${bines} ; do
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
     (pkgproto manuals=share/golded+/manuals | grep -v "\.tei=") ) | grep -v CVS | grep -v `basename ${dizfile}` | grep -v Makefile |\
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
echo "VENDOR=\"https://github.com/golded-plus/ packaged by $vendor\"" >> $pkgdir/pkginfo
echo 'HOTLINE="https://github.com/golded-plus/"' >> $pkgdir/pkginfo
echo 'MAXINST="1"' >> $pkgdir/pkginfo
cd $pkgdir
pkgmk -o -d . -f ./Prototype -r ../..
echo 1|pkgtrans -s . ../$pkgname
cd ../..
rm -rf $pkgdir

# zipping
zip -9DXj ${name} ${dizfile} ${binesdir}/$pkgname
