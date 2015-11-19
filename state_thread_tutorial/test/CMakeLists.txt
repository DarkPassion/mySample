cmake_minimum_required(VERSION 2.8)

PROJECT(myUdpTest${MYVER})
set (CMAKE_BUILD_TYPE DEBUG)

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/)

link_directories(${PROJECT_SOURCE_DIR}/bin  /usr/local/lib ${PROJECT_SOURCE_DIR}/../st-1.9/obj/)

link_libraries(myUdpTest${MYVER} st)  

include_directories(/usr/local/include/ ${PROJECT_SOURCE_DIR}/../st-1.9/obj/ ${PROJECT_SOURCE_DIR}/)

set(CMAKE_CXX_FLAGS "-D_CONSOLE -g -DUSE_GNUTLS -Wall -O2 -D_CONSOLE -pipe -Wno-invalid-offsetof -fcheck-new  -D_REENTRANT")


add_executable(myUdpTest${MYVER} main.cpp st_thread.cpp st_udp.cpp st_help.cpp st_tcp.cpp st_msg.cpp st_queue.cpp) 

