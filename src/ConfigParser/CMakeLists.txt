cmake_minimum_required(VERSION 3.5)
project(ConfigParser)

set(CMAKE_CXX_STANDARD 11)

include_directories("${CMAKE_SOURCE_DIR}/lib/rapidjson/include/rapidjson")

add_library(${PROJECT_NAME} STATIC ConfigParser.cpp)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
