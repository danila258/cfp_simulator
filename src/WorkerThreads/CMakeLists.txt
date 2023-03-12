project(WorkerThreads)
cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)

add_library(${PROJECT_NAME} STATIC WorkerThreads.h WorkerThreads.cpp customMutex.cpp customMutex.h customQueue.h)

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} ConfigParser)
