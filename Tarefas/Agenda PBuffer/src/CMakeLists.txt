add_executable(agenda main.c agenda.c darray.c arena.c)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")

target_compile_options(agenda PRIVATE
	$<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
	$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fsanitize=address -Wall -Wextra -Wpedantic -Werror>
	$<$<CONFIG:Debug>:-fsanitize=address -O0 -g3 -ggdb -gdwarf-2 -D__DEBUG>
)
