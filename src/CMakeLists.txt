cmake_minimum_required(VERSION 3.5)
project(cfp_simulator)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(TRTIniFile)
add_subdirectory(ConfigParser)
add_subdirectory(WorkerThreads)

add_executable(${PROJECT_NAME}
               main.cpp
              )

target_link_libraries(${PROJECT_NAME} TRTIniFile)
target_link_libraries(${PROJECT_NAME} ConfigParser)
target_link_libraries(${PROJECT_NAME} WorkerThreads)




