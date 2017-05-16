cmake_minimum_required(VERSION 2.8)

PROJECT(Fdk-aac)

set (CMAKE_BUILD_TYPE DEBUG)

include_directories(${PROJECT_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "-D_CONSOLE -g -Wall -O0  -Wno-sequence-point -Wno-extra -std=c++98")

include_directories(
    ${PROJECT_SOURCE_DIR}/libAACdec/include/
    ${PROJECT_SOURCE_DIR}/libAACenc/include/
    ${PROJECT_SOURCE_DIR}/libPCMutils/include/
    ${PROJECT_SOURCE_DIR}/libFDK/include/
    ${PROJECT_SOURCE_DIR}/libSYS/include/
    ${PROJECT_SOURCE_DIR}/libMpegTPDec/include/
    ${PROJECT_SOURCE_DIR}/libMpegTPEnc/include/
    ${PROJECT_SOURCE_DIR}/libSBRdec/include/
    ${PROJECT_SOURCE_DIR}/libSBRenc/include/
    )

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






