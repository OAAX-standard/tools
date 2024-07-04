cmake_minimum_required(VERSION 3.10.2)

project(c-utilities)

# set paths
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)


# add source files
file(GLOB SRC_FILES ${SRC_DIR}/*)
add_executable(c-utilities main.c ${SRC_FILES})

# Include directories
target_include_directories(c-utilities PUBLIC ${INCLUDE_DIR})

