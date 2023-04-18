cmake_minimum_required(VERSION 3.5)
project(ObjectFactory)

set(CMAKE_CXX_STANDARD 11)

add_library(${PROJECT_NAME} STATIC ObjectFactory.cpp Objects.cpp)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${PROJECT_NAME}
                    ${PROJECT_SOURCE_DIR}/lib/RTLib3/header/log
                    ${PROJECT_SOURCE_DIR}/lib/RTLib3/header/task
                    ${PROJECT_SOURCE_DIR}/lib/RTLib3/header/syns
                    ${PROJECT_SOURCE_DIR}/lib/RTLib3/header/data)

target_link_libraries(${PROJECT_NAME} PUBLIC RTLib3)

