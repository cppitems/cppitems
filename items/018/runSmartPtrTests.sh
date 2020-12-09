#!/bin/bash

# This shell script executes all Tests in order

SCRIPT_DIR=`dirname $0`

for d in ${SCRIPT_DIR}/build/SmartPtrTests/*/ ; do
    # echo "$d" 
    # echo "$(basename $d)"
    testname=$(basename $d)
    if  [[ $testname == Test* ]] ;
    then
        fullname="$d""$testname"
        $fullname
    fi
done