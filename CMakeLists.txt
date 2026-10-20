cmake_minimum_required(VERSION 3.10)
project(StressTesting CXX)
set(CMAKE_CXX_STANDARD 17)

include(CheckIncludeFileCXX)
check_include_file_cxx(bits/stdc++.h HAVE_BITS_STDCXX_H)
if(NOT HAVE_BITS_STDCXX_H)
    message(FATAL_ERROR
        "${CMAKE_CXX_COMPILER_ID} has no <bits/stdc++.h>. Configure with GCC instead, e.g.\n"
        "  macOS:   cmake -S . -B build -DCMAKE_CXX_COMPILER=g++-<version>\n"
        "  Windows: cmake -S . -B build -G \"MinGW Makefiles\"\n"
        "and delete the build directory first if it already exists.")
endif()

add_executable(generator cpp/generator.cpp)
add_executable(test cpp/test.cpp)
add_executable(correct cpp/correct.cpp)
if(EXISTS ${CMAKE_SOURCE_DIR}/cpp/checker.cpp)
    add_executable(checker cpp/checker.cpp)
endif()
