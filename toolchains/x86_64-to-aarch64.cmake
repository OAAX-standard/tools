set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(CROSS_ROOT "/opt/gcc-arm-9.2-2019.12-x86_64-aarch64-none-linux-gnu")
set(COMPILER_PREFIX "aarch64-none-linux-gnu-")
set(CMAKE_BUILD_FLAGS " -fno-math-errno -fopenmp -std=c99")

set(CMAKE_C_COMPILER ${CROSS_ROOT}/bin/${COMPILER_PREFIX}gcc)
set(CMAKE_CXX_COMPILER ${CROSS_ROOT}/bin/${COMPILER_PREFIX}g++)
set(CMAKE_LINKER ${CROSS_ROOT}/bin/${COMPILER_PREFIX}ld)