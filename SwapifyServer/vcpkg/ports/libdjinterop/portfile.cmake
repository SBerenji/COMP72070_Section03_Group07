vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO xsco/libdjinterop
    REF "${VERSION}"
    SHA512 591cbf8102e16b11337ec25ad90ef035bf65dbbd18591802d959044874f36bb61bce3d5db974b00ecee14b45c7ea2488542b226d823d9087c92a0b452d804ff3
    HEAD_REF master
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DCMAKE_DISABLE_FIND_PACKAGE_Boost=ON
    )
vcpkg_cmake_install()

vcpkg_cmake_config_fixup(PACKAGE_NAME djinterop CONFIG_PATH lib/cmake/DjInterop)
vcpkg_fixup_pkgconfig()
vcpkg_copy_pdbs()

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/share")
vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
