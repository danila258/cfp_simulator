cmake_minimum_required(VERSION 3.5)
project(WorkerThreads)

set(CMAKE_CXX_STANDARD 11)

add_library(${PROJECT_NAME} STATIC WorkerThreads.cpp)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
