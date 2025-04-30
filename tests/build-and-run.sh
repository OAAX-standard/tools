#!/bin/sh

# Exit immediately if a command exits with a non-zero status
set -e

# Change to the script's directory
cd $(dirname "$0")

HERE=$(pwd)
BUILD_DIR="$HERE/build"

# Remove previous build directory if it exists
rm -rf $BUILD_DIR || true

# Create a fresh build directory
mkdir -p $BUILD_DIR
cd $BUILD_DIR

# NOTE: Uncomment the following line to use a custom toolchain file
# CMAKE_TOOLCHAIN_FILE="$HERE/../toolchains/x86_64-to-windows.cmake"

# Run CMake to configure the project
cmake .. -DCMAKE_TOOLCHAIN_FILE=$CMAKE_TOOLCHAIN_FILE

# Build the project
cmake --build . --config Release

# Run the unit tests binary
# NOTE: Change the path to the unit tests binary if necessary
./unit_tests
