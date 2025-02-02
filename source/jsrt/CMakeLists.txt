# Add headers as sources so IDE handles it correctly
file(GLOB_RECURSE ${jsrt_headers} CONFIGURE_DEPENDS jsrt/*.hpp)

add_library(jsrt INTERFACE ${jsrt_headers})
target_sources(jsrt INTERFACE FILE_SET HEADERS
    TYPE HEADERS
    BASE_DIRS "${CMAKE_CURRENT_LIST_DIR}"
    FILES ${jsrt_headers}
)
target_include_directories(jsrt INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(jsrt INTERFACE JSI)