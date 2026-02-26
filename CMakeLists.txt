cmake_minimum_required(VERSION 3.16...3.21)
project(AudioPlayer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS 
    Widgets
    Multimedia
)

qt_standard_project_setup()

add_executable(AudioPlayer
    ./src/main.cpp
    ./src/gamewindow.cpp
    ./src/gamewindow.h
)

target_link_libraries(AudioPlayer
    Qt6::Widgets
    Qt6::Multimedia
)
