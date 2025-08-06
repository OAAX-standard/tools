cmake_minimum_required(VERSION 3.10)
project(c_utilities_tests C)

set(CMAKE_C_STANDARD 99)

include_directories(
    ../c-utilities/include
)

# Detect platform and set platform-specific options
if(WIN32)
    message(STATUS "Configuring for Windows")
    set(PLATFORM_LIBS ws2_32 psapi)  # Use lowercase 'psapi' for MinGW
elseif(UNIX)
    message(STATUS "Configuring for Unix/Linux")
    set(PLATFORM_LIBS pthread m)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

if(CMAKE_TOOLCHAIN_FILE)
    message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
endif()

add_executable(c_utilities_tests
    main.c
    test_tensors_struct.c
    test_utils.c
    test_threading.c
    test_lib_loader.c
)

target_link_libraries(c_utilities_tests ${PLATFORM_LIBS})

# Link C Utilities project
add_subdirectory(
    ../c-utilities
    ${CMAKE_CURRENT_BINARY_DIR}/c_utilities
)
target_link_libraries(c_utilities_tests c_utilities)

# Link mini shared library project
add_subdirectory(
    ./mini-shared-library
    ${CMAKE_CURRENT_BINARY_DIR}/mini_shared_library
)
