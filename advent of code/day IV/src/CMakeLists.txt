add_library(DArray darray.c darray.h)
add_executable(Part1 part1.c)
add_executable(Part2 part2.c)

target_link_libraries(Part1 DArray)
target_link_libraries(Part2 DArray)
