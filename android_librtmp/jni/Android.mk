LOCAL_PATH:= $(call my-dir)

SSL := ${LOCAL_PATH}/../openssl/$(TARGET_ARCH_ABI)

include $(CLEAR_VARS)
LOCAL_MODULE := libssl
LOCAL_SRC_FILES := ${SSL}/libssl.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

include $(CLEAR_VARS)
LOCAL_MODULE := libcrypto
LOCAL_SRC_FILES := ${SSL}/libcrypto.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)


LOCAL_SRC_FILES:= \
	${LOCAL_PATH}/../librtmp/amf.c \
	${LOCAL_PATH}/../librtmp/hashswf.c \
	${LOCAL_PATH}/../librtmp/log.c \
	${LOCAL_PATH}/../librtmp/parseurl.c \
	${LOCAL_PATH}/../librtmp/rtmp.c \
	${LOCAL_PATH}/rtmp_conn.cpp



LOCAL_CFLAGS += -I$(SSL)/include -I${LOCAL_PATH}/../
#LOCAL_LDLIBS += -L$(SSL)/libs/$(TARGET_ARCH_ABI)
LOCAL_LDLIBS += -ldl -lz

LOCAL_STATIC_LIBRARIES := \
	libssl \
	libcrypto 



LOCAL_MODULE := librtmp

include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_STATIC_LIBRARY)


