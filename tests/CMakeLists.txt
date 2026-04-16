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
    test_timer.c
    test_logger.c
    test_memory.c
    test_tensors_struct.c
    test_queue.c
    test_utils.c
    test_threading.c
    test_lib_loader.c
    test_sysinfo.c
    test_oaax_utils.c
)

target_include_directories(c_utilities_tests PRIVATE ../oaax-runtime-utils/include)

target_link_libraries(c_utilities_tests ${PLATFORM_LIBS})

# Link C Utilities project
add_subdirectory(
    ../c-utilities
    ${CMAKE_CURRENT_BINARY_DIR}/c_utilities
)
target_link_libraries(c_utilities_tests c_utilities)

# Link OAAX Runtime Utils
add_subdirectory(
    ../oaax-runtime-utils
    ${CMAKE_CURRENT_BINARY_DIR}/oaax_runtime_utils
)
target_link_libraries(c_utilities_tests oaax_utils)

# Link mini shared library project
add_subdirectory(
    ./mini-shared-library
    ${CMAKE_CURRENT_BINARY_DIR}/mini_shared_library
)
