

git clone https://github.com/guardianproject/openssl-android.git

cd openssl-android

vim ssl/Android.mk

SSL 修改为静态编译

#include $(BUILD_SHARED_LIBRARY)
include $(BUILD_STATIC_LIBRARY)

注释掉动态库链接
#LOCAL_SHARED_LIBRARIES += libcrypto


vim crypto/Android.mk

crypto 修改为静态编译
#include $(BUILD_SHARED_LIBRARY)
include $(BUILD_STATIC_LIBRARY)


$NDK_ROOT/ndk-build

