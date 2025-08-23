vcpkg_from_github(
        OUT_SOURCE_PATH SOURCE_PATH
        REPO ludvikjerabek/getopt-win
        REF cmake-support
        SHA512 <INSERT_SHA512_HASH>
)

# Configure with your custom build options
vcpkg_cmake_configure(
        SOURCE_PATH ${SOURCE_PATH}
        PREFER_NINJA
        OPTIONS
        -DBUILD_SHARED_LIB=ON
        -DBUILD_STATIC_LIB=OFF
        -DBUILD_TESTING=OFF
)

vcpkg_cmake_install()

vcpkg_copy_pdbs()

file(INSTALL ${SOURCE_PATH}/LICENSE DESTINATION ${CURRENT_PACKAGES_DIR}/share/getopt-win RENAME copyright)
