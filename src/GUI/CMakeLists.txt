cmake_minimum_required(VERSION 3.5)
project(GUI)

set(CMAKE_CXX_STANDARD 11)

add_library(${PROJECT_NAME} STATIC
            MainWidget.cpp
            ObjectCreatorWidget.cpp
            ObjectRowWidget.cpp
            ThreadsTreeWidget.cpp
            ConfigsWidget.cpp
            ActionDialog.cpp
            ActionWidget.cpp)

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE
                      Common
                      MainLogic
                      ConfigParser
                      Qt${QT_VERSION_MAJOR}::Widgets)