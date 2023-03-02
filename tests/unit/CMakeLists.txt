cmake_minimum_required(VERSION 3.5)

project(ICPCReference LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)

# Catch2
Include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v2.13.10
)
FetchContent_MakeAvailable(Catch2)

# Include parent directory so "code" directory can be imported
get_filename_component(PARENT_DIR ../../ ABSOLUTE)
include_directories(${PARENT_DIR})

set(SANITY_TEST sanity.cc)  # Sanity test
set(DATA_STRUCTURES_TESTS   # Data structures tests
  data_structures/fenwick_tree.cc
)
set(UTILITIES_TESTS
  utilities/random.cc
)

# Add the tests
add_executable(tests tests.cc
  ${SANITY_TEST}
  ${DATA_STRUCTURES_TESTS}
  ${UTILITIES_TESTS}
)
target_link_libraries(tests PRIVATE Catch2::Catch2)

# Add the testing frameworks and find the tests
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/contrib)
include(CTest)
include(Catch)
catch_discover_tests(tests)