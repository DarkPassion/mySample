cmake_minimum_required(VERSION 2.8)

PROJECT(srsjson${MYVER})
set (CMAKE_BUILD_TYPE DEBUG)

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/)

link_directories(${PROJECT_BINARY_DIR}/bin  /usr/local/lib)
link_libraries(srsjson${MYVER})  
include_directories(/usr/local/include/ ${PROJECT_SOURCE_DIR}/ )
set(CMAKE_CXX_FLAGS "-D_CONSOLE -g -DUSE_GNUTLS -Wall -O2 -D_CONSOLE -pipe -Wno-invalid-offsetof -fcheck-new -D_ACE_SOCK -D_POSIX_THREADS -D_POSIX_THREAD_SAFE_FUNCTIONS -D_REENTRANT")
add_executable(srsjson${MYVER} srs_app_json.cpp main.cpp) 

