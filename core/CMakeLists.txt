file(GLOB_RECURSE higs_core_srcs
    source/*.cpp
    source/*.hpp
)
add_library(higs_core ${higs_core_srcs})

#target_include_directories(higs_core INTERFACE include)
target_link_libraries(higs_core PUBLIC Hermes::Hermes)


add_executable(higs_cli main.cpp)
target_link_libraries(higs_cli higs_core)