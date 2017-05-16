cmake_minimum_required(VERSION 2.8)

PROJECT(Fdk-aac-test)

set (CMAKE_BUILD_TYPE DEBUG)
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR})
link_directories(${PROJECT_SOURCE_DIR}/libs)
link_libraries(Fdk-aac.a)

set(CMAKE_CXX_FLAGS "-D_CONSOLE -g -Wall -O0  -Wno-sequence-point -Wno-extra -std=c++98")

include_directories(
    ${PROJECT_SOURCE_DIR}/../libAACdec/include/
    ${PROJECT_SOURCE_DIR}/../libAACenc/include/
    ${PROJECT_SOURCE_DIR}/../libPCMutils/include/
    ${PROJECT_SOURCE_DIR}/../libFDK/include/
    ${PROJECT_SOURCE_DIR}/../libSYS/include/
    ${PROJECT_SOURCE_DIR}/../libMpegTPDec/include/
    ${PROJECT_SOURCE_DIR}/../libMpegTPEnc/include/
    ${PROJECT_SOURCE_DIR}/../libSBRdec/include/
    ${PROJECT_SOURCE_DIR}/../libSBRenc/include/
    )

file(GLOB SRC_FILES 
    ${PROJECT_SOURCE_DIR}/*.c
    )

add_executable(Fdk-aac-test ${SRC_FILES})






