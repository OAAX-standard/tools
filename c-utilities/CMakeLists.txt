cmake_minimum_required(VERSION 3.10)

# Set the source and header directories
set(C_UTILITIES_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(C_UTILITIES_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Collect all source files (excluding windows subdir for non-Windows, and vice versa)
file(GLOB C_UTILITIES_SRC
    "${C_UTILITIES_SRC_DIR}/*.c"
)

if(WIN32)
    file(GLOB C_UTILITIES_WINDOWS_SRC
        "${C_UTILITIES_SRC_DIR}/windows/*.c"
    )
    list(APPEND C_UTILITIES_SRC ${C_UTILITIES_WINDOWS_SRC})
endif()

# Create the library
add_library(c_utilities STATIC ${C_UTILITIES_SRC})
set_target_properties(c_utilities PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Include directories
target_include_directories(c_utilities PUBLIC ${C_UTILITIES_INCLUDE_DIR})

# Platform-specific definitions
if(WIN32)
    target_compile_definitions(c_utilities PUBLIC _WIN32)
endif()

# Link math library for Unix/Linux
if(UNIX)
    target_link_libraries(c_utilities m)
endif()