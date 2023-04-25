cmake_minimum_required(VERSION 3.5)
project(ConfigParser)

set(CMAKE_CXX_STANDARD 11)

include_directories(../lib/rapidjson/include/)

add_library(${PROJECT_NAME} STATIC ConfigParser.cpp ConfigParser.h)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
