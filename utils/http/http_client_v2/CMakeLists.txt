cmake_minimum_required(VERSION 3.3)
project(http_client_v2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp HttpParser.cpp HttpParser.h lib/http_parser.c HttpClient.cpp HttpClient.h)

include_directories( ${PROJECT_SOURCE_DIR}/. ${PROJECT_SOURCE_DIR}/lib)

add_executable(http_client_v2 ${SOURCE_FILES})
