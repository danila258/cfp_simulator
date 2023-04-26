cmake_minimum_required(VERSION 3.5)
project(cfp_simulator)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# lots of warnings
if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# my specific path
set(CMAKE_PREFIX_PATH "/home/danila/SDK/Qt/5.15.2/gcc_64/lib/cmake")

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_subdirectory(${PROJECT_SOURCE_DIR}/lib/RTLib3/prj/cmake)
add_subdirectory(${PROJECT_SOURCE_DIR}/MainLogic)
add_subdirectory(${PROJECT_SOURCE_DIR}/GUI)
add_subdirectory(${PROJECT_SOURCE_DIR}/ObjectFactory)
add_subdirectory(${PROJECT_SOURCE_DIR}/ConfigParser)
add_subdirectory(${PROJECT_SOURCE_DIR}/WorkerThreads)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(${PROJECT_NAME} PRIVATE GUI)
#target_link_libraries(${PROJECT_NAME} PRIVATE MainLogic)
#target_link_libraries(${PROJECT_NAME} PRIVATE ObjectFactory)
target_link_libraries(${PROJECT_NAME} PRIVATE ConfigParser)



