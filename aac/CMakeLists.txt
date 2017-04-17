cmake_minimum_required(VERSION 2.8)

PROJECT(Fdk-aac)

set (CMAKE_BUILD_TYPE DEBUG)

include_directories(${PROJECT_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "-D_CONSOLE -g -Wall -O0  -Wno-invalid-offsetof -fcheck-new -std=c++0x")

file(GLOB SRC_FILES 
    ${PROJECT_SOURCE_DIR}/libAACdec/src/*.cpp
    ${PROJECT_SOURCE_DIR}/libAACenc/src/*.cpp
    ${PROJECT_SOURCE_DIR}/libPCMutils/src/*.cpp
    ${PROJECT_SOURCE_DIR}/libFDK/src/*.cpp
    ${PROJECT_SOURCE_DIR}/libSYS/src/*.cpp
    ${PROJECT_SOURCE_DIR}/libMpegTPDec/src/*.cpp
    ${PROJECT_SOURCE_DIR}/libMpegTPEnc/src/*.cpp
    ${PROJECT_SOURCE_DIR}/libSBRdec/src/*.cpp
    ${PROJECT_SOURCE_DIR}/libSBRenc/src/*.cpp
    )


add_library(Fdk-aac STATIC ${SRC_FILES}) 




