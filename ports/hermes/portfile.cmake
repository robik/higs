vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO facebook/hermes
    REF 1d7433bf196d57d2e50ca02e7e48095140e8bab9
    SHA512 9b755182ef540be48b9cb6c54257a3e3477d288e758fc21b91516daf5444f3647b79587e0b3a41da712e1be4caf93f212abeaf8639d9506d4dadd99ac3547458
    HEAD_REF static_h
)

vcpkg_cmake_configure(
    SOURCE_PATH ${SOURCE_PATH}
    OPTIONS
        -DHERMES_ENABLE_TEST_SUITE=OFF
        -DHERMES_ENABLE_TOOLS=ON
#        -DDEFAULT_BUILD_SHARED_LIBS=OFF
#        -DHERMES_BUILD_SHARED_JSI=OFF
)

vcpkg_cmake_build()

file(COPY
        "${SOURCE_PATH}/public/hermes"
        "${SOURCE_PATH}/API/hermes"
        "${SOURCE_PATH}/API/jsi/jsi"
    DESTINATION "${CURRENT_INSTALLED_DIR}/include"
    FILES_MATCHING
        PATTERN "*.h"
)

set(BUILD_DIR "${CURRENT_BUILDTREES_DIR}/${TARGET_TRIPLET}")

# Copy release libraries
#file(COPY
#        "${BUILD_DIR}-rel/API"
#        "${BUILD_DIR}-rel/jsi"
#    DESTINATION "${CURRENT_INSTALLED_DIR}/lib"
#    FILES_MATCHING
#        PATTERN "*.a"
#        PATTERN "*${CMAKE_SHARED_LIBRARY_SUFFIX}"
#)
file(COPY "${BUILD_DIR}-rel/lib/libhermesvm${CMAKE_SHARED_LIBRARY_SUFFIX}"
    DESTINATION "${CURRENT_INSTALLED_DIR}/lib"
)

# Copy debug libraries
#file(COPY
#        "${BUILD_DIR}-dbg/API/hermes"
#        "${BUILD_DIR}-dbg/jsi"
#    DESTINATION "${CURRENT_INSTALLED_DIR}/debug/lib"
#    FILES_MATCHING
#        PATTERN "*.a"
#        PATTERN "*${CMAKE_SHARED_LIBRARY_SUFFIX}"
#)
file(COPY "${BUILD_DIR}-dbg/lib/libhermesvm${CMAKE_SHARED_LIBRARY_SUFFIX}"
    DESTINATION "${CURRENT_INSTALLED_DIR}/debug/lib"
)