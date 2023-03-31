cmake_minimum_required(VERSION 3.5)
project(ConfigParser)

set(CMAKE_CXX_STANDARD 11)

add_library(${PROJECT_NAME} STATIC ConfigParser.cpp)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME}
                      PRIVATE IniFile
                      PRIVATE WorkerThreads)

