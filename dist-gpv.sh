#!/bin/sh

# Create a archive 'gpwin-*.zip' (gpvYMMDD.zip) with Win32 (MS Visual C)
# builg of the Golded+.

name=gpwin-115-`date +%Y%m%d`.zip

files="Release/bin/gedwin.exe Release/bin/gnwin.exe Release/bin/rddtwin.exe"
files="${files} docs/copying docs/copying.lib"
files="${files} docs/golded.html docs/golded.txt docs/goldnode.html"
files="${files} docs/goldnode.txt docs/license.txt docs/notework.txt"
files="${files} docs/rddt.html docs/rddt.txt docs/readme.txt"
files="${files} docs/rusfaq.txt docs/tips.txt docs/todowork.txt"
files="${files} docs/tokencfg.txt docs/tokentpl.txt"


printf 'GoldED+1.1.5  [Win32 binaries]\n\r'  >Release/bin/File_ID.Diz
printf '[Compiled using MS Visual C++]\n\r' >>Release/bin/File_ID.Diz
printf 'Snapshot (development version)\n\r' >>Release/bin/File_ID.Diz
printf 'This is  unstable release  and\n\r' >>Release/bin/File_ID.Diz
printf 'it should be  used for testing\n\r' >>Release/bin/File_ID.Diz
printf '------------------------------\n\r' >>Release/bin/File_ID.Diz
printf 'GoldED+  is a successor of the\n\r' >>Release/bin/File_ID.Diz
printf 'wellknown  GoldED mail editor.\n\r' >>Release/bin/File_ID.Diz
printf '------------------------------\n\r' >>Release/bin/File_ID.Diz
printf ' *http://golded-plus.sf.net*  \n\r' >>Release/bin/File_ID.Diz

make docs
zip -9DXj ${name} Release/bin/File_ID.Diz $files
