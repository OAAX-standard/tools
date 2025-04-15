#!/bin/sh

# Exit immediately if a command exits with a non-zero status
set -e

# Change to the script's directory
cd $(dirname "$0")

BUILD_DIR=build

# Remove previous build directory if it exists
rm -rf $BUILD_DIR || true

# Create a fresh build directory
mkdir -p $BUILD_DIR
cd $BUILD_DIR

# Run CMake to configure the project
cmake ..

# Build the project
make

# Run the unit tests binary
./unit_tests
