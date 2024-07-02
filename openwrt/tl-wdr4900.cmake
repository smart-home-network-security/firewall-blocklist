IF ( DEFINED ENV{STAGING_DIR} )
    set(CMAKE_SYSTEM_NAME Linux)
    set(CMAKE_SYSTEM_PROCESSOR powerpc)

    # Set OpenWrt variables
    set(ARCH "powerpc")
    set(CMAKE_C_COMPILER   $ENV{TOOLCHAIN_PATH}/bin/${ARCH}-openwrt-linux-gcc)
    set(CMAKE_CXX_COMPILER $ENV{TOOLCHAIN_PATH}/bin/${ARCH}-openwrt-linux-g++)

    SET(CMAKE_FIND_ROOT_PATH $ENV{OPENWRT_HOME} $ENV{TARGET_PATH} $ENV{TOOLCHAIN_PATH})
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
ENDIF()
