cmake_minimum_required(VERSION 3.10)
project(oaax_utils_tests C)

set(CMAKE_C_STANDARD 99)

if(WIN32)
    message(STATUS "Configuring for Windows")
elseif(UNIX)
    message(STATUS "Configuring for Unix/Linux")
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

if(CMAKE_TOOLCHAIN_FILE)
    message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
endif()

add_executable(oaax_utils_tests
    main.c
    test_oaax_utils.c
)

add_subdirectory(
    ../oaax-runtime-utils
    ${CMAKE_CURRENT_BINARY_DIR}/oaax_runtime_utils
)
target_link_libraries(oaax_utils_tests oaax_utils)
