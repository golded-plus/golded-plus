#!/bin/sh

# Create a archive 'gpc*.zip' with configuration files examples.

date=`date +%Y%m%d`
shortdate=`echo ${date} | sed s/^...//`
shortname=../gpc${shortdate}.zip
name=gpc115-${date}.tar.bz2

dir=cfgs

if [ -f ${name} ] ; then rm $name ; fi

find ${dir} | grep -v CVS | zip -9 -@ -X ${name}
cp ${name} ${shortname}
