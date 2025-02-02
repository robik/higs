file(GLOB_RECURSE higs_src CONFIGURE_DEPENDS *.cpp)
file(GLOB_RECURSE higs_headers CONFIGURE_DEPENDS *.hpp *.h)

# Core library
add_library(higs ${higs_src} ${higs_headers})
target_sources(higs
    PUBLIC
    FILE_SET headers
        TYPE HEADERS
        BASE_DIRS ".."
        FILES ${higs_headers}
)
target_compile_definitions(higs PRIVATE HERMES_ENABLE_DEBUGGER=1)
target_include_directories(higs PRIVATE SYSTEM ${HIGS_SOURCE_ROOT})
target_link_libraries(higs PUBLIC
    jsrt jsrt_conv
    Boost::pfr
    Folly::folly
    gsl::gsl-lite-v1
)

