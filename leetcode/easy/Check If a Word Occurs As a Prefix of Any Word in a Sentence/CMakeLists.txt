cmake_minimum_required(VERSION 3.10)
project(TwoSum C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set the output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

enable_testing()

add_subdirectory(src)
