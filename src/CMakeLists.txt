cmake_minimum_required(VERSION 3.5)
project(cfp_simulator)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(${PROJECT_SOURCE_DIR}/lib/IniFile)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/spdlog)
add_subdirectory(${PROJECT_SOURCE_DIR}/MainLogic)
add_subdirectory(${PROJECT_SOURCE_DIR}/ConfigParser)
add_subdirectory(${PROJECT_SOURCE_DIR}/WorkerThreads)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE MainLogic)


