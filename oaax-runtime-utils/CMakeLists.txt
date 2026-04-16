cmake_minimum_required(VERSION 3.10)

set(OAAX_UTILS_SRC_DIR     "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(OAAX_UTILS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB OAAX_UTILS_SRC "${OAAX_UTILS_SRC_DIR}/*.c")

add_library(oaax_utils STATIC ${OAAX_UTILS_SRC})
set_target_properties(oaax_utils PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(oaax_utils PUBLIC ${OAAX_UTILS_INCLUDE_DIR})
target_compile_features(oaax_utils PRIVATE c_std_99)

if(WIN32)
    target_compile_definitions(oaax_utils PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
