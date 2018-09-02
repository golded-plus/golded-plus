#!/bin/bash
# $Id$
# Create a archive 'gpc*.zip' with configuration files examples.

srcdatefile=srcdate.h
build=`sed -n 's/.*"\([[:digit:]]\{8\}\)".*/\1/p' $srcdatefile`
date="$build"
shortdate=${date/???/}

shortname=../gpc${shortdate}.zip
name=../gpc115-${date}.zip

dir=cfgs

echo Build an archive with Golded+ configuration examples:  ${name} and ${shortname}

if [ -f ${name} ] ; then rm $name ; fi

find ${dir} | grep -v CVS | zip -9 -@ -X ${name}
ln ${name} ${shortname} || cp ${name} ${shortname}
