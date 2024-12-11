add_library(DArray darray.c darray.h)
add_library(HashMap hashmap.c hashmap.h)

add_executable(Part1 part1.c)
target_compile_options(Part1 PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

add_executable(Part2 part2.c)
target_compile_options(Part2 PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

target_link_libraries(HashMap PRIVATE DArray)
target_link_libraries(Part1 PRIVATE DArray HashMap)
target_link_libraries(Part2 PRIVATE DArray HashMap)
