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

set(SANITY_TEST sanity.cc)

add_executable(tests tests.cc
  ${SANITY_TEST}
)
target_link_libraries(tests PRIVATE Catch2::Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/contrib)
include(CTest)
include(Catch)
catch_discover_tests(tests)