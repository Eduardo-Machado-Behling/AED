# Add Unity framework
include(FetchContent)

FetchContent_Declare(
  unity
  URL https://github.com/ThrowTheSwitch/Unity/archive/v2.5.2.tar.gz
)
FetchContent_MakeAvailable(unity)

add_executable(tests tests.c)

target_link_libraries(tests PRIVATE array)

target_include_directories(tests PRIVATE ${unity_SOURCE_DIR}/src)
target_sources(tests PRIVATE ${unity_SOURCE_DIR}/src/unity.c)

add_test(NAME ExampleTests COMMAND test_example)
