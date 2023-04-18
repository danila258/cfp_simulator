cmake_minimum_required(VERSION 3.5)
project(cfp_simulator)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "-Wall -Werror")

add_subdirectory(${PROJECT_SOURCE_DIR}/lib/RTLib3/prj/cmake)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/IniFile)
add_subdirectory(${PROJECT_SOURCE_DIR}/MainLogic)
add_subdirectory(${PROJECT_SOURCE_DIR}/ObjectFactory)
add_subdirectory(${PROJECT_SOURCE_DIR}/ConfigParser)
add_subdirectory(${PROJECT_SOURCE_DIR}/WorkerThreads)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE MainLogic)
target_link_libraries(${PROJECT_NAME} PUBLIC ObjectFactory)


