cmake_minimum_required(VERSION 3.10)
project(TwoSum C)

# Set the output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
add_compile_options(
"$<$<COMPILE_LANGUAGE:CXX>:-fsanitize=address;-g3>"
)

enable_testing()

add_subdirectory(src)
