cmake_minimum_required(VERSION 3.10)
project(TwoSum C)

# Set the output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
SET(GCC_COVERAGE_COMPILE_FLAGS " -fsanitize=address -fprofile-arcs -ftest-coverage")
SET(GCC_COVERAGE_LINK_FLAGS    "-lgcov")

enable_testing()

add_subdirectory(src)
