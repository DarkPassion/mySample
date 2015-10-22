
export NDK_ROOT="/data/app/android-ndk-r10e"

export PATH="${PATH}:${NDK_ROOT}"

export PREBUILT=$NDK_ROOT/toolchains/arm-linux-androideabi-4.8/prebuilt

export PLATFORM=$NDK_ROOT/platforms/android-8/arch-arm

export PREFIX=~/android-x264

#echo $NDK_ROOT
#echo $PREFIX
#echo "$PREBUILT/linux-x86/bin/arm-linux-androideabi-"
#echo "$PLATFORM"


./configure --prefix=$PREFIX \
--enable-static \
--enable-pic \
--disable-asm \
--disable-cli \
--host=arm-linux \
--cross-prefix=$PREBUILT/linux-x86_64/bin/arm-linux-androideabi- \
--sysroot=$PLATFORM
