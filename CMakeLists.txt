cmake_minimum_required(VERSION 3.30)
project(higs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)

find_package(fmt CONFIG REQUIRED)
find_package(Hermes REQUIRED)

add_subdirectory(core)