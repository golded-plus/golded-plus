#!/bin/bash

# Define the exclude list
exclude_list=(
     "memleak.cpp"
    "gcrc16tb.cpp"
     "hunspell.h"
     "gwinpcks.cpp"
     "gkbdbase.cpp"
    "gclang.cpp"
    "geline.cpp"
    "gckeys.cpp"
    "gccfgg.cpp"
    "goldnode.cpp"
)
# Convert the exclude list to a grep pattern
exclude_pattern=$(printf "|%s" "${exclude_list[@]}")
exclude_pattern="${exclude_pattern:1}"

find . -type f \( -name '*.c' -or -name '*.cpp' -or -name '*.cc' -or -name '*.h' \) \
 | grep -vE "$exclude_pattern" |xargs -n 1 uncrustify --no-backup -c uncrustify70.1.cfg
