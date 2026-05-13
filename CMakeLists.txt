cmake_minimum_required(VERSION 3.16)
project(Citril VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(citril_core
    src/lexer.cpp
    src/parser.cpp
    src/environment.cpp
    src/interpreter.cpp
)

target_include_directories(citril_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_options(citril_core PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

add_executable(citril bin/citril.cpp)
target_link_libraries(citril PRIVATE citril_core)

add_executable(citril_tests tests/basic.cpp)
target_link_libraries(citril_tests PRIVATE citril_core)

enable_testing()
add_test(NAME citril_basic COMMAND citril_tests)

add_library(sample_citril_lib SHARED
    src/sample_library.cpp
)
target_link_libraries(sample_citril_lib PRIVATE citril_core)

