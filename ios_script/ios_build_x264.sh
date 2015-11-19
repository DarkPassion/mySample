#!/bin/sh

DEST=~/ios_x264
SDK_VERSION="9.0"
echo "Building armv7"

ARM="armv7"
export CC=`xcodebuild -find clang`
DEVPATH=/Applications/XCode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS${SDK_VERSION}.sdk

echo $DEVPATH

./configure                     \
    --host=arm-apple-darwin     \
    --sysroot=$DEVPATH          \
    --prefix=$DEST/$ARM         \
    --extra-cflags="-arch $ARM" \
    --extra-ldflags="-L$DEVPATH/usr/lib/system -arch $ARM" \
    --enable-pic      \
    --enable-static   \
    --disable-asm

   make && make install && make clean

echo "Installed: $DEST/$ARM"


echo "Building i386"

SIM="i386"
CC=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/usr/bin/gcc
DEVPATH=/Applications/XCode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator${SDK_VERSION}.sdk

./configure                     \
    --host=i386-apple-darwin    \
     -sysroot=$DEVPATH          \
    --prefix=$DEST/$SIM         \
    --extra-cflags="-arch $SIM" \
    --extra-ldflags="-L$DEVPATH/usr/lib/system -arch $SIM" \
    --enable-pic    \
    --enable-static \
    --disable-asm

    make && make install && make clean

echo "Installed: $DEST/$SIM"


echo "Combining library ......"
BUILD_LIBS="libx264.a"
OUTPUT_DIR="output"
ARCHS="armv7 i386"

cd install
mkdir $OUTPUT_DIR
mkdir $OUTPUT_DIR/lib
mkdir $OUTPUT_DIR/include


LIPO_CREATE=""

for ARCH in $ARCHS; do
    LIPO_CREATE="$LIPO_CREATE $ARCH/lib/$BUILD_LIBS "
done

lipo -create $LIPO_CREATE -output $OUTPUT_DIR/lib/$BUILD_LIBS
cp -f $ARCH/include/*.* $OUTPUT_DIR/include/

echo "************************************************************"
lipo -i $OUTPUT_DIR/lib/$BUILD_LIBS
echo "************************************************************"

echo "OK, merge done!"

