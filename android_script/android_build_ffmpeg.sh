


function build_arm32()
{
HOST="arm-linux-androideabi"

PREBUILT=$NDK_ROOT/toolchains/${HOST}-4.8/prebuilt

PLATFORM=$NDK_ROOT/platforms/android-14/arch-arm

PREFIX=$(pwd)/../android-ffmpeg/arm32/


GENERAL="\
    --enable-cross-compile \
    --extra-libs="-lgcc" \
    --arch=arm \
    --target-os=linux \
    --host-os=arm-linux \
    --cross-prefix=$PREBUILT/linux-x86_64/bin/arm-linux-androideabi-  \
    --nm=$PREBUILT/linux-x86_64/bin/arm-linux-androideabi-nm"

MODULES="\
    --disable-avdevice \
    --disable-filters \
    --disable-programs \
    --disable-network \
    --disable-avfilter \
    --disable-postproc \
    --disable-encoders \
    --disable-protocols \
    --disable-hwaccels \
    --disable-doc"

VIDEO_DECODERS="\
    --enable-decoder=h264 \
    --enable-decoder=mpeg4 \
    --enable-decoder=mpeg2video \
    --enable-decoder=mjpeg \
    --enable-decoder=mjpegb"

DEMUXERS="\
    --enable-demuxer=h264 \
    --enable-demuxer=m4v \
    --enable-demuxer=mpegvideo \
    --enable-demuxer=mpegps "

PARSERS="\
    --enable-parser=h264 \
    --enable-parser=mpeg4video \
    --enable-parser=mpegaudio \
    --enable-parser=mpegvideo \
    --enable-parser=aac \
    --enable-parser=aac_latm"



./configure \
    --prefix=$PREFIX \
    --disable-shared \
    --enable-static \
	${GENERAL} \
	${MODULES} \
	${VIDEO_DECODERS} \
	${DEMUXERS} \
	${PARSERS} \
    --target-os=linux \
    --extra-cflags="-O3 -fpic" \
    --extra-ldflags="-L${PLATFORM}/usr/lib/ -lc -lm -ldl -llog" \
    --sysroot=$PLATFORM


make && make install
}
function build_arm64()
{
HOST="aarch64-linux-android"

PREBUILT=$NDK_ROOT/toolchains/${HOST}-4.9/prebuilt

PLATFORM=$NDK_ROOT/platforms/android-21/arch-arm64

PREFIX=$(pwd)/../android-ffmpeg/arm64/

GENERAL="\
    --enable-cross-compile \
    --extra-libs="-lgcc" \
    --arch=aarch64 \
    --target-os=linux \
    --host-os=arm-linux \
    --cross-prefix=$PREBUILT/linux-x86_64/bin/aarch64-linux-android- \
    --nm=$PREBUILT/linux-x86_64/bin/aarch64-linux-android-nm"

MODULES="\
    --disable-avdevice \
    --disable-filters \
    --disable-programs \
    --disable-network \
    --disable-avfilter \
    --disable-postproc \
    --disable-encoders \
    --disable-protocols \
    --disable-hwaccels \
    --disable-doc"

VIDEO_DECODERS="\
    --enable-decoder=h264 \
    --enable-decoder=mpeg4 \
    --enable-decoder=mpeg2video \
    --enable-decoder=mjpeg \
    --enable-decoder=mjpegb"

DEMUXERS="\
    --enable-demuxer=h264 \
    --enable-demuxer=m4v \
    --enable-demuxer=mpegvideo \
    --enable-demuxer=mpegps "

PARSERS="\
    --enable-parser=h264 \
    --enable-parser=mpeg4video \
    --enable-parser=mpegaudio \
    --enable-parser=mpegvideo \
    --enable-parser=aac \
    --enable-parser=aac_latm"



./configure \
    --prefix=$PREFIX \
    --disable-shared \
    --enable-static \
    --target-os=linux \
    --arch=aarch64 \
	${GENERAL} \
	${MODULES} \
	${VIDEO_DECODERS} \
	${DEMUXERS} \
	${PARSERS} \
    --extra-cflags="-O3 -DANDROID -Dipv6mr_interface=ipv6mr_ifindex -fasm -Wno-psabi -fno-short-enums -fno-strict-aliasing" \
    --extra-ldflags="-Wl,-rpath-link=$PLATFORM/usr/lib -L$PLATFORM/usr/lib -nostdlib -lc -lm -ldl -llog" \
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

