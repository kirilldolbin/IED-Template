cmake_minimum_required(VERSION 3.10)
project(IED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(third_party/CMakeLists.txt)

# Add executable
add_executable(ied_exe src/main.cpp)

# Add include directories
target_include_directories(ied_exe PRIVATE include ${LIBIEC_INCLUDES})

# Set compiler options
target_compile_options(ied_exe PRIVATE -Wall -Wextra -pedantic)
target_link_libraries(ied_exe PRIVATE iec61850)


