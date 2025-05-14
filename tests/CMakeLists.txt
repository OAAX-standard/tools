cmake_minimum_required(VERSION 3.10)
project(unit_tests C)

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

add_executable(unit_tests
    main.c
    test_timer.c
    test_logger.c
    test_memory.c
    test_tensors_struct.c
    test_queue.c
    ../c-utilities/src/logger.c
    ../c-utilities/src/memory.c
    ../c-utilities/src/timer.c
    ../c-utilities/src/tensors_struct.c
    ../c-utilities/src/queue.c
    ../c-utilities/src/cp_platform.c
)

target_link_libraries(unit_tests ${PLATFORM_LIBS})
