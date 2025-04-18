# SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
# SPDX-License-Identifier: MIT

project(test.md_bench)

if(ENABLE_COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif(ENABLE_COVERAGE)

if(MSVC)
    add_compile_options(/bigobj)
    add_compile_options(/utf-8)
endif()

set(SRC main.cpp)

file(GLOB MD_FILES data/*.md)
file(COPY ${MD_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/tests/bench/data)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../..
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../3rdparty)

find_package(Qt6Core REQUIRED)
find_package(Qt6Test REQUIRED)

add_executable(test.bench.qt ${SRC})
target_compile_definitions(test.bench.qt PUBLIC TRAIT=MD::QStringTrait)
target_compile_definitions(test.bench.qt PUBLIC MD4QT_QT_SUPPORT)
target_link_libraries(test.bench.qt Qt6::Core Qt6::Test)

add_test(NAME test.bench.qt
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/test.bench.qt
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../../bin)
