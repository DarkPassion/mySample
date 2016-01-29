#!/bin/bash

uname -s |grep "Darwin" >/dev/null 2>&1
if [[ 0 -ne $? ]]; then
    echo "only support OSX"
    exit 1
fi
echo "OS ok."

sed -i '' "s/v1_EN_/v1_CN_/g" v*_CN_*
sed -i '' "s/v2_EN_/v2_CN_/g" v*_CN_*
sed -i '' "s/v3_EN_/v3_CN_/g" v*_CN_*
echo "all EN correct to CN"

sed -i '' "s/v1_CN_/v1_EN_/g" v*_EN_*
sed -i '' "s/v2_CN_/v2_EN_/g" v*_EN_*
sed -i '' "s/v3_CN_/v3_EN_/g" v*_EN_*
echo "all CN correct to EN"

echo "ok"
