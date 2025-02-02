cmake_minimum_required(VERSION 3.30)

#set(CMAKE_SUPPRESS_REGENERATION ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(higs LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_XCODE_GENERATE_TOP_LEVEL_PROJECT_ONLY OFF)
set(CMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH OFF)

include_directories(include)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(fmt CONFIG REQUIRED)
find_package(gsl-lite CONFIG REQUIRED)

find_package(boost_pfr CONFIG REQUIRED)
find_package(boost_type_index CONFIG REQUIRED)
find_package(boost_program_options REQUIRED CONFIG)

find_package(Hermes CONFIG REQUIRED)
find_package(folly REQUIRED)

set(HIGS_RUN_UNITTESTS ON CACHE BOOL "Build and run unittests")

#set(CMAKE_XCODE_)

# Consider
# - https://github.com/AmokHuginnsson/replxx
# - https://github.com/jupyter-xeus/cpp-terminal
# - https://github.com/antirez/linenoise

if (${HIGS_RUN_UNITTESTS})
    find_package(gtest CONFIG REQUIRED)
endif ()

add_subdirectory(source)
add_subdirectory(tools)

if (${HIGS_RUN_UNITTESTS})
    add_subdirectory(unittests)
endif ()
