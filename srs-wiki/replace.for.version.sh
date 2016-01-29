#!/bin/bash

uname -s |grep "Darwin" >/dev/null 2>&1
if [[ 0 -ne $? ]]; then
    echo "only support OSX"
    exit 1
fi
echo "OS ok."

function do_replace() {
    from=$1
    to=$2
    echo "do replace from $from to $to"
    
    files=`ls ${to}_*`
    exists=`ls ${to}_*|awk -F 'N_' '{print $2}'|awk -F '.md' '{print $1}'`
    for file in $files; do
        echo "process file $file from $from to $to"
        for exist in $exists; do
            #echo "for $file, replace $from to $to on $exist"
            sed -i '' "s/${from}_CN_${exist}/${to}_CN_${exist}/g" $file
            sed -i '' "s/${from}_EN_${exist}/${to}_EN_${exist}/g" $file
        done
    done
}

# for v2 wikis, update v1 to exists v2 wikis.
do_replace "v1" "v2"
# for v3 wikis, updata v1 to exists v3 wikis.
do_replace "v1" "v3"
# for v3 wikis, updata v2 to exists v3 wikis.
do_replace "v2" "v3"

echo "ok"
