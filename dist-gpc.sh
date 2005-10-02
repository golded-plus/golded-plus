#!/bin/sh

# Create a archive 'gpc*.zip' with configuration files examples.

name=gpc115-`date +%Y%m%d`.zip
dir=cfgs

if [ -f ${name} ] ; then rm $name ; fi

find ${dir} | grep -v CVS | zip -9 -@ -X ${name}
