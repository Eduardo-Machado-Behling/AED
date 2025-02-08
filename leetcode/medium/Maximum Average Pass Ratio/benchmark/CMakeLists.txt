# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add Google Benchmark
find_package(benchmark REQUIRED)

# Add the source file containing your code and benchmarks
add_executable(bench main.cpp linkedlist.cpp)
set_target_properties(bench PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/benchmark)
target_link_libraries(bench PRIVATE solution)

# Link Google Benchmark library
if(benchmark_FOUND)
    target_link_libraries(bench PRIVATE benchmark::benchmark)
else()
    message(FATAL_ERROR "Google Benchmark not found")
endif()
