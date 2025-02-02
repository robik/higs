# add jsr.hpp as source so CLion catches it
file(GLOB_RECURSE jsrt_conv_headers CONFIGURE_DEPENDS jsrt/*.hpp)

add_library(jsrt_conv INTERFACE ${jsrt_conv_headers})
target_sources(jsrt_conv INTERFACE FILE_SET HEADERS
    TYPE HEADERS
    BASE_DIRS "${CMAKE_CURRENT_LIST_DIR}"
    FILES ${jsrt_conv_headers}
)
target_link_libraries(jsrt_conv INTERFACE jsrt)
target_link_libraries(jsrt_conv INTERFACE fmt::fmt Boost::type_index)

target_include_directories(jsrt_conv INTERFACE ${CMAKE_CURRENT_LIST_DIR})