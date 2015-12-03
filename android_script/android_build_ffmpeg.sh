


function build_arm32()
{
HOST="arm-linux-androideabi"

PREBUILT=$NDK_ROOT/toolchains/${HOST}-4.8/prebuilt

PLATFORM=$NDK_ROOT/platforms/android-14/arch-arm

PREFIX=$(pwd)/../android-ffmpeg/arm32/


./configure \
    --prefix=$PREFIX \
    --disable-shared \
    --enable-static \
    --enable-pic \
    --disable-everything \
    --enable-decoder=h264 \
    --disable-doc \
    --disable-avdevice \
    --disable-filters \
    --disable-programs \
    --disable-network \
    --target-os=linux \
    --arch=arm \
    --enable-cross-compile \
    --extra-cflags="-O3 -fpic" \
    --extra-ldflags="-L${PLATFORM}/usr/lib/ -lc -lm -ldl -llog" \
    --cross-prefix=$PREBUILT/darwin-x86_64/bin/${HOST}- \
    --sysroot=$PLATFORM


make && make install
}
function build_arm64()
{
HOST="aarch64-linux-android"

PREBUILT=$NDK_ROOT/toolchains/${HOST}-4.9/prebuilt

PLATFORM=$NDK_ROOT/platforms/android-21/arch-arm64

PREFIX=$(pwd)/../android-ffmpeg/arm64/


./configure \
    --prefix=$PREFIX \
    --disable-shared \
    --enable-static \
    --enable-pic \
    --disable-everything \
    --enable-decoder=h264 \
    --disable-doc \
    --disable-avdevice \
    --disable-filters \
    --disable-programs \
    --disable-network \
    --target-os=linux \
    --arch=aarch64 \
    --enable-cross-compile \
    --extra-cflags="-O3 -fpic" \
    --extra-ldflags="-L${PLATFORM}/usr/lib/ -lc -lm -ldl -llog" \
    --cross-prefix=$PREBUILT/darwin-x86_64/bin/${HOST}- \
    --sysroot=$PLATFORM


make && make install
}


FFMPEG_URL="http://ffmpeg.org/releases"
VER="ffmpeg-2.2.16"
FF_TAR_GZ="${VER}.tar.gz"

if [ ! -f $FF_TAR_GZ ];then
    curl ${FFMPEG_URL}/$FF_TAR_GZ -o $FF_TAR_GZ
fi

if [ ! -f $FF_TAR_GZ ];then
   echo "error $FF_TAR_GZ not exist!" 
   exit 1
fi

tar -xf $FF_TAR_GZ 

cd $VER


build_arm32

#build_arm64

echo "Build Done!"

