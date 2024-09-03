# Minimum required CMake version
cmake_minimum_required(VERSION 3.20)

## Test subdirectories
# Sample profiles
add_subdirectory(device)
# Unit tests
IF( NOT OPENWRT_CROSSCOMPILING )
    add_subdirectory(unit)
ENDIF()
