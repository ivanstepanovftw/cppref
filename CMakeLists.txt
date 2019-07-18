cmake_minimum_required(VERSION 3.14)
project(algos)

find_package(Boost 1.69 REQUIRED COMPONENTS unit_test_framework)


file(GLOB_RECURSE HEADERS_LIST "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hh")
add_library(algos INTERFACE)
target_include_directories(algos
        INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        )


add_executable(algos_data test/test.cpp)
set_target_properties(algos_data PROPERTIES
        CXX_STANDARD 20
        )
target_compile_definitions(algos_data PUBLIC
        -DBOOST_TEST_DYN_LINK
        -DBOOST_TEST_MODULE=algos_data
        )
target_compile_options(algos_data PUBLIC
        -O3
        -g -ggdb
        )
target_link_libraries(algos_data PUBLIC
        algos
        Boost::unit_test_framework
        )


add_executable(scratch sandbox/scratch.cc)
set_target_properties(scratch PROPERTIES
        CXX_STANDARD 20
        )
target_compile_options(scratch PUBLIC
        -O0
        -g -ggdb
        -Wall -Wextra -Wpedantic -Werror
        )
target_link_libraries(scratch PUBLIC
        algos
        )

add_executable(virtual_keyword sandbox/virtual_keyword.cc)
set_target_properties(virtual_keyword PROPERTIES
        CXX_STANDARD 20
        )
target_compile_options(virtual_keyword PUBLIC
        -O0
        -g -ggdb
        -Wall -Wextra -Wpedantic
        )
