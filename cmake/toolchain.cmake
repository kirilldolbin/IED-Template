# Toolchain file for cross-compiling

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(TOOLCHAIN_PATH /opt/arm_sysroot)
set(CMAKE_C_COMPILER ${TOOLCHAIN_PATH}/bin/aarch64-buildroot-linux-gnu-gcc
        CACHE INTERNAL "C compiler" FORCE)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}/bin/aarch64-buildroot-linux-gnu-g++
        CACHE INTERNAL "C++ compiler" FORCE)

# Set the sysroot path for cross-compilation
set(CMAKE_SYSROOT ${TOOLCHAIN_PATH}/aarch64-buildroot-linux-gnu/sysroot)
set(CMAKE_FIND_ROOT_PATH ${CMAKE_SYSROOT})
# Specify the C and C++ standard libraries within the sysroot
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

message(STATUS "Using sysroot at ${CMAKE_SYSROOT}")
