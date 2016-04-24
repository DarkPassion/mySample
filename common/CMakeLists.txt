cmake_minimum_required(VERSION 2.8)

PROJECT(common${MYVER})
set (CMAKE_BUILD_TYPE DEBUG)

set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/libs/)

link_directories(${PROJECT_BINARY_DIR}/bin/ /usr/local/lib/ ${PROJECT_BINARY_DIR}/3rd/jsoncpp/lib)
link_libraries(common${MYVER} pthread jsoncpp)
include_directories( ${PROJECT_SOURCE_DIR}/.
${PROJECT_SOURCE_DIR}/src
${PROJECT_SOURCE_DIR}/3rd/jsoncpp/include)

set(CMAKE_CXX_FLAGS "-DWEBRTC_POSIX -g -DUSE_GNUTLS -Wall -O2 -D_CONSOLE -pipe -Wno-invalid-offsetof -fcheck-new -D_REENTRANT")

set (LIB_SRC
src/string/stringutils.cpp
src/string/stringencode.cpp
src/digest/md5.cpp
src/digest/base64.cpp
src/digest/crc32.cpp 
src/thread/thread.cpp
src/thread/lock.cpp
src/util/util.cpp
src/util/bytebuffer.cpp
src/util/log.cpp
src/util/json.cpp
src/queue/queue.cpp
src/queue/block_queue.cpp
src/timer/timer.cpp)


add_library(common${MYVER}
${LIB_SRC})





