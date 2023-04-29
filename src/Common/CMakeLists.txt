cmake_minimum_required(VERSION 3.5)
project(Common)

set(CMAKE_CXX_STANDARD 11)

add_library(${PROJECT_NAME} STATIC UniversalString.cpp)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Widgets)
