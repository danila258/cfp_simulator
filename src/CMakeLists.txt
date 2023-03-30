cmake_minimum_required(VERSION 3.5)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 11)

project(cfp_simulator)

add_subdirectory(${PROJECT_SOURCE_DIR}/TRTIniFile)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/spdlog)
add_subdirectory(${PROJECT_SOURCE_DIR}/MainLogic)
add_subdirectory(${PROJECT_SOURCE_DIR}/ConfigParser)
add_subdirectory(${PROJECT_SOURCE_DIR}/WorkerThreads)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/RTLib3/prj/cmake)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} TRTIniFile)
target_link_libraries(${PROJECT_NAME} spdlog::spdlog)
target_link_libraries(${PROJECT_NAME} MainLogic)
target_link_libraries(${PROJECT_NAME} ConfigParser)
target_link_libraries(${PROJECT_NAME} WorkerThreads)




