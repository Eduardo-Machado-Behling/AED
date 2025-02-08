include(FetchContent)

FetchContent_Declare(
  unity
  URL https://github.com/ThrowTheSwitch/Unity/archive/v2.5.2.tar.gz
)
FetchContent_MakeAvailable(unity)

add_library(solution STATIC solution.c solution.h)
target_include_directories(solution PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(main main.c)
add_test(NAME main COMMAND tests)

target_compile_definitions(
  main PUBLIC
  -DUNITY_INCLUDE_DOUBLE
  -DUNITY_DOUBLE_PRECISION=1e-5f 
)

target_link_libraries(main PRIVATE solution)
target_include_directories(main PRIVATE ${unity_SOURCE_DIR}/src)
target_sources(main PRIVATE ${unity_SOURCE_DIR}/src/unity.c)