add_library(agenda STATIC agenda.c agenda.h)

target_include_directories(agenda PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(main main.c agenda.c)
target_link_libraries(main PRIVATE agenda)
