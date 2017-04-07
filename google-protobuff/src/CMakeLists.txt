cmake_minimum_required(VERSION 2.8)

PROJECT(protobuf)

set (CMAKE_BUILD_TYPE DEBUG)

include_directories(${PROJECT_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "-D_CONSOLE -g -DUSE_GNUTLS -Wall -O2 -D_CONSOLE -pipe -Wno-invalid-offsetof -fcheck-new -std=c++0x")

file(GLOB SRC_FILES 
    ${PROJECT_SOURCE_DIR}/google/protobuf/*.cc
    ${PROJECT_SOURCE_DIR}/google/protobuf/*.h
    ${PROJECT_SOURCE_DIR}/google/protobuf/stubs/*.cc
    ${PROJECT_SOURCE_DIR}/google/protobuf/stubs/*.h
    ${PROJECT_SOURCE_DIR}/google/protobuf/io/*.cc
    ${PROJECT_SOURCE_DIR}/google/protobuf/io/*.h
    ${PROJECT_SOURCE_DIR}/google/protobuf/compiler/*.cc
    ${PROJECT_SOURCE_DIR}/google/protobuf/compiler/*.h
    )


add_library(protobuf STATIC ${SRC_FILES}) 



