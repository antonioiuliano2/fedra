#!/bin/bash

if [ $# -eq 1 -a -f "$1" ]; then
    cat $1 | awk "/xcvsVERSION/ {gsub(\"\\\"\",\"\",\$3); gsub(\"/\",\"-\",\$3);print \$3}"
fi
