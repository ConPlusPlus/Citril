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

function(add_citril_library base_name source_file)
    set(lib_name "${base_name}Lib")
    add_library(${lib_name} SHARED ${source_file})
    target_link_libraries(${lib_name} PRIVATE citril_core)
    set_target_properties(${lib_name} PROPERTIES
        OUTPUT_NAME ${lib_name}
function(add_citril_library target source_file)
    add_library(${target} SHARED ${source_file})
    target_link_libraries(${target} PRIVATE citril_core)
    set_target_properties(${target} PROPERTIES
        OUTPUT_NAME ${target}
        PREFIX ""
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/libraries
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/libraries
    )
endfunction()

add_citril_library(print libraries/print_library.cpp)
add_citril_library(types src/types_library.cpp)
add_citril_library(strings src/strings_library.cpp)
add_citril_library(time src/time_library.cpp)

enable_testing()
add_test(NAME citril_example_basics COMMAND citril ${CMAKE_SOURCE_DIR}/examples/basics.citril)
add_test(NAME citril_example_libraries COMMAND citril ${CMAKE_SOURCE_DIR}/examples/library_demo.citril)
add_test(NAME citril_basic COMMAND citril_tests)

add_library(sample_citril_lib SHARED
    src/sample_library.cpp
)
target_link_libraries(sample_citril_lib PRIVATE citril_core)
set_target_properties(sample_citril_lib PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/libraries
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/libraries
)


add_library(print_citril_lib SHARED
    libraries/print_library.cpp
)
target_link_libraries(print_citril_lib PRIVATE citril_core)
set_target_properties(print_citril_lib PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/libraries
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/libraries
)

