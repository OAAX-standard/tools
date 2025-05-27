set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(CROSS_ROOT "/opt/x86_64-unknown-linux-gnu-gcc-9.5.0")
set(COMPILER_PREFIX "x86_64-unknown-linux-gnu-")
set(SYSROOT "/opt/x86_64-unknown-linux-gnu-gcc-9.5.0/x86_64-unknown-linux-gnu/sysroot")
set(CMAKE_BUILD_FLAGS " -fno-math-errno -fopenmp -std=c99 -march=haswell")

set(CMAKE_C_COMPILER ${CROSS_ROOT}/bin/${COMPILER_PREFIX}gcc)
set(CMAKE_CXX_COMPILER ${CROSS_ROOT}/bin/${COMPILER_PREFIX}g++)
set(CMAKE_LINKER ${CROSS_ROOT}/bin/${COMPILER_PREFIX}ld)