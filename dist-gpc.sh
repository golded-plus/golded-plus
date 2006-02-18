#!/bin/sh

# Create a archive 'gpc*.zip' with configuration files examples.

date=`date +%Y%m%d`
shortdate=`echo ${date} | sed s/^...//`
shortname=../gpc${shortdate}.zip
name=../gpc115-${date}.zip

dir=cfgs

echo Build an archive with Golded+ configuration examples:  ${name} and ${shortname}

if [ -f ${name} ] ; then rm $name ; fi

find ${dir} | grep -v CVS | zip -9 -@ -X ${name}
cp ${name} ${shortname}
