cmake_minimum_required(VERSION 3.14)

project(PwrMonClient LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 COMPONENTS Core Bluetooth REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Bluetooth REQUIRED)

#Use STATIC or SHARED in the line below to choose the type of library to compile
add_library(PwrMonClient STATIC
  PwrMonClient_global.h
  pwrmonclient.cpp
  pwrmonclient_io/input.cpp
  pwrmonclient_io/output.cpp
  pwrmonclient.h
)

target_link_libraries(PwrMonClient PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Bluetooth)

target_compile_definitions(PwrMonClient PRIVATE PWRMONCLIENT_LIBRARY)
