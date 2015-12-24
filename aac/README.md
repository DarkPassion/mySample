

#
AAC android source code
http://androidxref.com/4.4.2_r2/xref/external/aac/
https://android.googlesource.com/platform/external/aac/
master branch
https://android.googlesource.com/platform/external/aac/+/master

```bash
wget https://android.googlesource.com/platform/external/aac/+archive/master.tar.gz

mkdir jni && cp Android.mk jni && cd jni && $NDK_ROOT/ndk-build

```

../libPCMutils/src/pcmutils_lib.cpp

注释掉#include <log/log.h>
添加宏定义 #define ALOGE(...)
这样就可以编译通过了





## android avcode 
 https://android.googlesource.com/platform/frameworks/av/+/59633a1/media/libstagefright/codecs




