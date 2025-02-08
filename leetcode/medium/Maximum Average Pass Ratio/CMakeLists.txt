cmake_minimum_required(VERSION 3.10)
project(TwoSum)

# Set the output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
option(BENCHMARK "enable benchmark for build, its setup is annoying be warned" OFF)

enable_testing()

message("Can enable benchmark by using -DBENCHMARK=ON")
add_subdirectory(src)
if(BENCHMARK)
add_subdirectory(benchmark)
endif()
