

function build_arm32()
{
PREBUILT=$NDK_ROOT/toolchains/arm-linux-androideabi-4.8/prebuilt

PLATFORM=$NDK_ROOT/platforms/android-8/arch-arm

PREFIX=$(pwd)/android-x264/arm32/


./configure --prefix=$PREFIX \
--enable-static \
--enable-pic \
--disable-asm \
--disable-cli \
--host=arm-linux \
--cross-prefix=$PREBUILT/linux-x86_64/bin/arm-linux-androideabi- \
--sysroot=$PLATFORM
}


function build_arm64()
{
PREBUILT=$NDK_ROOT/toolchains/-4.9/prebuilt

PLATFORM=$NDK_ROOT/platforms/android-21/arch-arm64

PREFIX=$(pwd)/android-x264/arm64/


./configure --prefix=$PREFIX \
--enable-static \
--enable-pic \
--disable-asm \
--disable-cli \
--host=arm-linux \
--cross-prefix=$PREBUILT/linux-x86_64/bin/aarch64-linux-android- \
--sysroot=$PLATFORM

}
