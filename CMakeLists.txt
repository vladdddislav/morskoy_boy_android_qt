
cmake_minimum_required(VERSION 3.16)
project(SeaBattle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

qt_add_executable(SeaBattle
    main.cpp
    mainwindow.cpp mainwindow.h
    classes.cpp classes.h
)

set_target_properties(SeaBattle PROPERTIES
   QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")

target_link_libraries(SeaBattle PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)