# SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
# SPDX-License-Identifier: MIT

project(test.plugins.emphasis)

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
file(COPY ${MD_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../../../../bin/tests/plugins/emphasis/data)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../..
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../3rdparty)

find_package(Qt6Core REQUIRED)
find_package(Qt6Test REQUIRED)

add_executable(test.plugins.emphasis main.cpp)
target_compile_definitions(test.plugins.emphasis PUBLIC TRAIT=MD::QStringTrait)
target_compile_definitions(test.plugins.emphasis PUBLIC MD4QT_QT_SUPPORT)
target_link_libraries(test.plugins.emphasis Qt6::Core)

add_test(NAME test.plugins.emphasis
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../../../../bin/test.plugins.emphasis
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../../../bin)

add_executable(test.plugins.emphasis.bench bench.cpp)
target_compile_definitions(test.plugins.emphasis.bench PUBLIC TRAIT=MD::QStringTrait)
target_compile_definitions(test.plugins.emphasis.bench PUBLIC MD4QT_QT_SUPPORT)
target_link_libraries(test.plugins.emphasis.bench Qt6::Core Qt6::Test)

add_test(NAME test.plugins.emphasis.bench
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../../../../bin/test.plugins.emphasis.bench
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../../../bin)
