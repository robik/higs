add_library(higs_core)
file(GLOB SRCS *.cpp *.hpp)
target_sources(higs_core PRIVATE ${SRCS})