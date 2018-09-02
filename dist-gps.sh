#!/bin/bash
# $Id$
# Create a archives 'gps*.tar.bz2' and 'gps*.tb2' with all source tree

srcdatefile=srcdate.h
build=`sed -n 's/.*"\([[:digit:]]\{8\}\)".*/\1/p' $srcdatefile`
date="$build"
shortdate=${date/???/}

shortname=gps${shortdate}.tb2
name=gps115-${date}.tar.bz2

echo Build a Golded+ sources package:  ${name} and ${shortname}

cd .. ; tar -jcf ${name} \
	--exclude CVS \
	--exclude golded+/golded3/mygolded.h \
	--exclude golded+/goldlib/gall/obj \
	--exclude golded+/obj \
	--exclude golded+/lib \
	--exclude golded+/bin/ged???* \
        --exclude golded+/bin/gn???* \
        --exclude golded+/bin/rddt???* \
	--exclude golded+/Release \
	--exclude golded+/Debug \
	--exclude golded+/debian/golded+ \
	--exclude golded+/windows/obj \
	--exclude 'golded+/*.bat' \
	--exclude 'golded+/*.BAT' \
	--exclude 'golded+/*.cmd' \
	--exclude 'golded+/*.CMD' \
	--exclude 'mygolded.h' \
	--exclude '.#*' \
	--exclude 'cscope.out' \
	--exclude '*.orig' \
	--exclude 'bld*.inc' \
	--exclude '*.diff' \
	--exclude '*.tar.gz' \
	--exclude '*.tgz' \
	--exclude '*.tar.bz' \
	--exclude '*.tbz' \
	--exclude '*.tar.bz2' \
	--exclude '*.tb2' \
	--exclude '*.cpz' \
	--exclude '*.rar' \
	--exclude '*.zip' \
	--exclude '*.7z' \
	--exclude '*.exe' \
	--exclude '*.a' \
	--exclude '*.o' \
	--exclude '**~' \
	golded+

ln ${name} ${shortname} || cp ${name} ${shortname}
