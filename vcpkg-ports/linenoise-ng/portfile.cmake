vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO pnnl/linenoise-ng
    REF v${VERSION}
    SHA512 f5d0e83d750ce81dd86cf470a894c18e1c72576d312f84c1a486662506d9ca79bc987939fd7c7a283511951f7e18af19091320623b4ad5a3c939c539f5c8e9e3
    HEAD_REF master
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    GENERATOR Ninja
)

vcpkg_cmake_build(TARGET all)
vcpkg_cmake_install()
#vcpkg_cmake_config_fixup(PACKAGE_NAME "linenoise-ng")

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

# Copy license file
#
# vcpkg is noisy if we don't.
vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")