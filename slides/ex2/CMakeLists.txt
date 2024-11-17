cmake_minimum_required(VERSION 3.10)
project(ex2 C)

# Set the output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)
