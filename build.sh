#!/bin/bash

# Build the project.
# Usage: build.sh [-C working_directory] [-t cmake_toolchain_file] [-d device]
#   -C working_directory: The directory to build the project in.
#   -t cmake_toolchain_file: The CMake toolchain file to use.

# Default values
WORKING_DIRECTORY=""
CMAKE_TOOLCHAIN_FILE=""

# Print usage information
usage() {
    echo "Usage: $0 [-C working_directory] [-t cmake_toolchain_file]" 1>&2
    exit 1
}

# Parse command line arguments
while getopts "C:t:d:" opt;
do
    case "${opt}" in
        C)
            # Working directory
            WORKING_DIRECTORY="${OPTARG}"
            echo "Building in directory ${WORKING_DIRECTORY}"
            ;;
        t)
            # CMake toolchain file
            CMAKE_TOOLCHAIN_FILE="${OPTARG}"
            echo "Using CMake toolchain file ${CMAKE_TOOLCHAIN_FILE}"
            ;;
        *)
            usage
            ;;
    esac
done
shift $((OPTIND-1))

# Change to working directory if specified
if [[ $WORKING_DIRECTORY ]]
then
    cd $WORKING_DIRECTORY
fi

# Clean directory
SUDO_PREFIX=""
if [[ $(whoami) != "root" ]]
then
    SUDO_PREFIX="sudo"
fi
$SUDO_PREFIX rm -rf build bin

## Set environmental variables
ENV_VARS=""
# CMake toolchain file
if [[ $CMAKE_TOOLCHAIN_FILE ]]
then
    ENV_VARS="$ENV_VARS -DCMAKE_TOOLCHAIN_FILE=$CMAKE_TOOLCHAIN_FILE"
fi

# Build project
mkdir build bin
cd build
cmake $ENV_VARS ..
cmake --build .
