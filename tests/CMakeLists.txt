include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB TEST_SOURCES *.cpp)

add_executable(run_tests ${TEST_SOURCES})

target_link_libraries(run_tests PRIVATE gtest_main data_structures)

include(GoogleTest)
gtest_discover_tests(run_tests)
