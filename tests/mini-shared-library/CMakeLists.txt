cmake_minimum_required(VERSION 3.5)
project(mini_lib C)

add_library(mini SHARED mini_lib.c)
set_target_properties(mini PROPERTIES OUTPUT_NAME "mini")

target_include_directories(mini PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
