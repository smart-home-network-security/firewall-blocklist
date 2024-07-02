# Minimum required CMake version
cmake_minimum_required(VERSION 3.20)

## Test subdirectories
# Sample profiles
add_subdirectory(translator)
# Unit tests for runtime code
IF( NOT OPENWRT_CROSSCOMPILING )
    add_subdirectory(runtime)
ENDIF()
