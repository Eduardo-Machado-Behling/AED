include(FetchContent)

FetchContent_Declare(
  unity
  URL https://github.com/ThrowTheSwitch/Unity/archive/v2.5.2.tar.gz
)
FetchContent_MakeAvailable(unity)

add_library(solution STATIC solution.c solution.h)
target_link_libraries(solution PRIVATE m)
target_include_directories(solution PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(solution PROPERTIES LINKER_LANGUAGE C)

add_executable(main main.c)
add_test(NAME main COMMAND tests)
set_target_properties(main PROPERTIES LINKER_LANGUAGE C)

target_compile_definitions(
  main PUBLIC
  -DUNITY_INCLUDE_DOUBLE
  -DUNITY_DOUBLE_PRECISION=1e-5f 
)

target_link_libraries(main PRIVATE solution)
target_include_directories(main PRIVATE ${unity_SOURCE_DIR}/src)
target_sources(main PRIVATE ${unity_SOURCE_DIR}/src/unity.c)