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

CMAKE_TOOLCHAIN_FILE="$HERE/../toolchains/x86_64-to-windows.cmake"

# Run CMake to configure the project
cmake .. -DCMAKE_TOOLCHAIN_FILE=$CMAKE_TOOLCHAIN_FILE

# Build the project
make

# Run the unit tests binary
./unit_tests
