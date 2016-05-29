LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)


AAC_ROOT_PATH := $(LOCAL_PATH)/../

LOCAL_ARM_MODE := arm

aacdec_sources := $(sort $(wildcard $(AAC_ROOT_PATH)/libAACdec/src/*.cpp))

aacenc_sources := $(sort $(wildcard $(AAC_ROOT_PATH)/libAACenc/src/*.cpp))

pcmutils_sources := $(sort $(wildcard $(AAC_ROOT_PATH)/libPCMutils/src/*.cpp))

fdk_sources := $(sort $(wildcard $(AAC_ROOT_PATH)/libFDK/src/*.cpp))

sys_sources := $(sort $(wildcard $(AAC_ROOT_PATH)/libSYS/src/*.cpp))

mpegtpdec_sources := $(sort $(wildcard $(AAC_ROOT_PATH)/libMpegTPDec/src/*.cpp))

mpegtpenc_sources := $(sort $(wildcard $(AAC_ROOT_PATH)/libMpegTPEnc/src/*.cpp))

sbrdec_sources := $(sort $(wildcard $(AAC_ROOT_PATH)/libSBRdec/src/*.cpp))

sbrenc_sources := $(sort $(wildcard $(AAC_ROOT_PATH)/libSBRenc/src/*.cpp))

LOCAL_SRC_FILES := \
        $(aacdec_sources) \
        $(aacenc_sources) \
        $(pcmutils_sources) \
        $(fdk_sources) \
        $(sys_sources) \
        $(mpegtpdec_sources) \
        $(mpegtpenc_sources) \
        $(sbrdec_sources) \
        $(sbrenc_sources)

LOCAL_CFLAGS += -Wno-sequence-point -Wno-extra

LOCAL_C_INCLUDES := \
        $(AAC_ROOT_PATH)/libAACdec/include \
        $(AAC_ROOT_PATH)/libAACenc/include \
        $(AAC_ROOT_PATH)/libPCMutils/include \
        $(AAC_ROOT_PATH)/libFDK/include \
        $(AAC_ROOT_PATH)/libSYS/include \
        $(AAC_ROOT_PATH)/libMpegTPDec/include \
        $(AAC_ROOT_PATH)/libMpegTPEnc/include \
        $(AAC_ROOT_PATH)/libSBRdec/include \
        $(AAC_ROOT_PATH)/libSBRenc/include


LOCAL_CPPFLAGS += -std=c++98

LOCAL_MODULE:= libFraunhoferAAC

include $(BUILD_STATIC_LIBRARY)
