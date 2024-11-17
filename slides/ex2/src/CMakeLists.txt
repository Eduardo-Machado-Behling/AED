add_library(array STATIC array.c array.h)

target_include_directories(array PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(main main.c array.c)
target_link_libraries(main PRIVATE array)
