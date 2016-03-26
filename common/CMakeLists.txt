cmake_minimum_required(VERSION 2.8)

PROJECT(libcommon${MYVER})
set (CMAKE_BUILD_TYPE DEBUG)

set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/libs/)

link_directories(${PROJECT_BINARY_DIR}/bin/ /usr/local/lib/)
link_libraries(libcommon${MYVER} pthread)  
include_directories( ${PROJECT_SOURCE_DIR}/.
${PROJECT_SOURCE_DIR}/src)

set(CMAKE_CXX_FLAGS "-D_CONSOLE -g -DUSE_GNUTLS -Wall -O2 -D_CONSOLE -pipe -Wno-invalid-offsetof -fcheck-new -D_POSIX_THREADS -D_POSIX_THREAD_SAFE_FUNCTIONS -D_REENTRANT")

set (LIB_SRC 
src/thread/thread.cpp
src/thread/lock.cpp
src/util/util.cpp
src/queue/queue.cpp)


add_library(libcommon${MYVER}
${LIB_SRC})





