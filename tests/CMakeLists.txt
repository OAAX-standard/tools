cmake_minimum_required(VERSION 3.10)
project(unit_tests C)

set(CMAKE_C_STANDARD 99)

include_directories(
    ../c-utilities/include
)

add_executable(unit_tests
    main.c
    ../c-utilities/src/logger.c
    ../c-utilities/src/memory.c
    ../c-utilities/src/timer.c
)

target_link_libraries(unit_tests m)
